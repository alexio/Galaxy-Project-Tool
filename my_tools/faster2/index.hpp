/***
 *  $Id$
 **
 *  File: index.hpp
 *  Created: Jun 14, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef INDEX_HPP
#define INDEX_HPP

#include <fstream>
#include <string>

#include <bio/fastx_iterator.hpp>
#include <jaz/files.hpp>

#include "stream.hpp"


struct db_entry {
    std::string file;
    std::string name;
    unsigned int pos;
    unsigned long int offset;
    unsigned int size;
    bool hasN;
    bool fastq;
}; // struct db_entry

inline std::istream& operator>>(std::istream& is, db_entry& de) {
    is >> de.pos >> de.offset >> de.size >> de.hasN;
    std::getline(is, de.name);
    std::getline(is, de.name);
    std::getline(is, de.file);
    return is;
} // operator>>

inline std::ostream& operator<<(std::ostream& os, const db_entry& de) {
    os << de.pos << " " << de.offset << " " << de.size << " " << de.hasN << std::endl;
    os << de.name << std::endl;
    os << de.file;
    return os;
} // operator<<


inline bool non_dna(char c) {
    switch (c) {
      case 'A': case 'a': return false;
      case 'C': case 'c': return false;
      case 'G': case 'g': return false;
      case 'T': case 't': return false;
    }
    return true;
} // non_dna


class non_protein {
public:
    non_protein() {
	std::memset(sig_, 0, 256);
	sig_['A'] = sig_['a'] = 1;
	sig_['C'] = sig_['c'] = 1;
	sig_['D'] = sig_['d'] = 1;
	sig_['E'] = sig_['e'] = 1;
	sig_['F'] = sig_['f'] = 1;
	sig_['G'] = sig_['g'] = 1;
	sig_['H'] = sig_['h'] = 1;
	sig_['I'] = sig_['i'] = 1;
	sig_['K'] = sig_['k'] = 1;
	sig_['L'] = sig_['l'] = 1;
	sig_['M'] = sig_['m'] = 1;
	sig_['N'] = sig_['n'] = 1;
	sig_['P'] = sig_['p'] = 1;
	sig_['Q'] = sig_['q'] = 1;
	sig_['R'] = sig_['r'] = 1;
	sig_['S'] = sig_['s'] = 1;
	sig_['T'] = sig_['t'] = 1;
	sig_['V'] = sig_['v'] = 1;
	sig_['W'] = sig_['w'] = 1;
	sig_['Y'] = sig_['y'] = 1;
    } // non_protein

    bool operator()(char c) const { return !sig_[c]; }

private:
    char sig_[256];

}; // nom_protein


class DBIndex {
public:
    // *** I/O interface ***

    std::pair<bool, std::string> create(const std::string& dir, const std::string& alpha, const std::string& type = "") {
	if ((alpha != "nt") && (alpha != "aa")) return { false, "unknown sequence type" };

	bool is_dna = true;
	if (alpha == "aa") is_dna = false;

	bool res = false;
	std::string err = "unknown index type";

	if ((type.empty() == true) || (type == "fasta")) {
	    std::tie(res, err) = prv_create__<bio::fasta_input_iterator<>>(dir, is_dna);
	} else if (type == "fastq") {
	    std::tie(res, err) = prv_create__<bio::fastq_input_iterator<>>(dir, is_dna);
	}

	if (res == false) return { res, err };

	// write to file
	std::ofstream f((dir + "/" + DB_IDX_FILE).c_str());
	if (!f) return { false, "failed to write index" };

	boost::iostreams::filtering_ostream fo;
        fo.push(boost::iostreams::bzip2_compressor());
        fo.push(f);

	fo << VERSION << " ";

	if (type == "fastq") {
	    for (db_entry& de : db_) de.fastq = true;
	    fo << "1" << std::endl;
	} else fo << "0" << std::endl;

        std::copy(db_.begin(), db_.end(), std::ostream_iterator<db_entry>(fo, "\n"));

	return { true, "" };
    } // create

    std::pair<bool, std::string> read(const std::string& dir) {
        std::ifstream f((dir + "/" + DB_IDX_FILE).c_str(), std::ios_base::binary);
        if (!f) return { false, "failed to open index" };

        boost::iostreams::filtering_istream fi;
        fi.push(boost::iostreams::bzip2_decompressor());
        fi.push(f);

	double ver = 0.0;
	bool fastq = false;

	fi >> ver >> fastq;
	if (!fi) return { false, "failed to read index version" };

	// index 1.0 and 1.1 are not compatible with 1.2
	if (ver < 1.2) return { false, "obsolete index version" };

	db_.clear();

        std::copy(std::istream_iterator<db_entry>(fi), std::istream_iterator<db_entry>(),
                  std::back_inserter(db_));

	if (fi.eof() == false) return { false, "failed to read index" };

	db_.shrink_to_fit();
	for (db_entry& de : db_) de.fastq = fastq;

	return { true, "" };
    } // read


    // *** get ***

    bool is_paired() const { return is_paired_; }

    bool is_fastq() const { return is_fastq_; }

    std::vector<db_entry>& db() { return db_; }


private:
    static std::string DB_IDX_FILE;
    static std::string VERSION;

    template <typename FastaIter>
    std::pair<bool, std::string> prv_create__(const std::string& dir, bool is_dna) {
	std::vector<fs::path> files;

	// get list of files
	if (jaz::files(dir, std::back_inserter(files)) == false) {
	    return { false, "failed to scan " + dir };
	}

	is_paired_ = false;
	is_fastq_ = false;

	db_.clear();
	db_.shrink_to_fit();

	// iterate over files
	for (auto iter = files.begin(); iter != files.end(); ++iter) {
	    if (fs::is_regular_file(*iter) == false) continue;

	    std::ifstream fs;
	    boost::iostreams::filtering_stream<boost::iostreams::input> cs;

	    std::istream* is = open_stream(*iter, fs, cs);
	    if (is == 0) return { false, "failed to open " + iter->string() };

	    bool is_cs = false;
	    if (is == &cs) is_cs = true;

	    FastaIter fii(*is), end;

	    for (; fii != end; ++fii) {
		db_entry de;
		const std::string& s = std::get<1>(*fii);
		de.file = iter->filename().string();
		de.name = std::get<0>(*fii);
		de.pos = fii.pos();
		if (is_cs == true) de.offset = 0;
	        else de.offset = fii.tellg();
		de.size = s.size();
		if (is_dna == true) de.hasN = (std::find_if(s.begin(), s.end(), non_dna) != s.end());
		else de.hasN = (std::find_if(s.begin(), s.end(), non_protein()) != s.end());
		de.fastq = false;
		db_.emplace_back(de);
	    }

	    if (is->eof() == false) return { false, "failed to read " + iter->string() };
	} // for

	if (db_.empty() == true) return { false, "failed to build index" };

	return { true, "" };
    } // prv_create__

    std::vector<db_entry> db_;

    bool is_paired_;
    bool is_fastq_;

}; // class DBIndex


std::string DBIndex::DB_IDX_FILE = ".f2index";
std::string DBIndex::VERSION = "1.2";


#endif // INDEX_HPP
