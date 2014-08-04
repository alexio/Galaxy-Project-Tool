/***
 *  $Id$
 **
 *  File: PrintFilter.hpp
 *  Created: Jun 21, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012-2014 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying file LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef PRINT_FILTER_HPP
#define PRINT_FILTER_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <bio/fastx_iterator.hpp>

#include "AbstractFilter.hpp"
#include "index.hpp"
#include "stream.hpp"


class PrintFilter : public Filter {
public:

    struct for_access {
	bool operator()(const db_entry& lhs, const db_entry& rhs) const {
	    return (lhs.file < rhs.file) || ((lhs.pos < rhs.pos) && !(rhs.file < lhs.file));
	}
    }; // struct for_access


    PrintFilter(const std::string& path, const std::vector<std::string>& args)
	: Filter("PrintFilter"), dir_(path), name_("-"), fastq_(false) {
	if (args.size() > 2) error = "incorrect arguments";
	if (args.empty() == false) name_ = args[0];
	if (args.size() == 2) {
	    if (args[1] == "fasta") fastq_ = false;
	    else if (args[1] == "fastq") fastq_ = true;
	    else error = "unknown output type";
	}
    } // PrintFilter


    std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	std::ostream* os = &std::cout;
	std::ofstream of;

	if (name_ != "-") {
	    of.open(name_.c_str());
	    if (!of) return { false, "failed to create " + name_ };
	    os = &of;
	}

	if (db.empty() == true) return { true, "" };

	if ((db.back().fastq == false) && (fastq_ == true)) {
	    return { false, "scores are not available" };
	}

	std::sort(db.begin(), db.end(), for_access());

	if (db.back().fastq == true) return prv_run__<bio::fastq_input_iterator<>>(*os, db);
	return prv_run__<bio::fasta_input_iterator<>>(*os, db);
    } // run


private:
    void prv_print__(std::ostream& os, const std::string& name, bio::fasta_input_iterator<> iter) {
	// os << ">" << std::get<0>(*iter) << "\n" << std::get<1>(*iter) << "\n";
	os << ">" << name << "\n" << std::get<1>(*iter) << "\n";
	os << std::endl;
    } // prv_print__

    void prv_print__(std::ostream& os, const std::string& name, bio::fastq_input_iterator<> iter) {
	if (fastq_ == false) {
	    // os << ">" << std::get<0>(*iter) << "\n" << std::get<1>(*iter) << "\n";
	    os << ">" << name << "\n" << std::get<1>(*iter) << "\n";
	} else {
	    // os << "@" << std::get<0>(*iter) << "\n" << std::get<1>(*iter) << "\n";
	    os << "@" << name << "\n" << std::get<1>(*iter) << "\n";
	    os << "+" << "\n";
	    os << std::get<2>(*iter) << "\n";
	}
	os << std::endl;
    } // prv_print__

    template <typename FastaIter>
    std::pair<bool, std::string> prv_run__(std::ostream& os, std::vector<db_entry>& db) {
	std::string file = "";
	std::string name = "";

	std::ifstream fs;
	boost::iostreams::filtering_stream<boost::iostreams::input> cs;

	std::istream* is = 0;
	bool is_cs = false;

	unsigned int pos = 0;
	FastaIter fii;

	for (auto iter = db.begin(); iter != db.end(); ++iter) {
	    if (file != iter->file) {
		file = iter->file;
		name = dir_ + "/" + iter->file;
		is = open_stream(name, fs, cs);
		if (is == 0) return { false, "failed to read " + name };
		is_cs = (is == &cs);
		if (is_cs == true) fii = FastaIter(*is);
		pos = 0;
	    }

	    if (is_cs == false) {
		is->seekg(iter->offset);
		fii = FastaIter(*is);
	    } else {
		std::advance(fii, iter->pos - pos);
		pos = iter->pos;
	    }

	    if ((is->eof() == false) && (is->fail() == true)) return { false, "stream failed for " + name };

	    prv_print__(os, iter->name, fii);
	} // for

	return { true, "" };
    } // prv_run__

    std::string dir_;
    std::string name_;
    bool fastq_;

}; // class PrintFilter

#endif // PRINT_FILTER_HPP
