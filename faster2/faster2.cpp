/***
 *  $Id$
 **
 *  File: faster2.cpp
 *  Created: Jun 14, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012-2014 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying file LICENSE.
 *
 *  This file is part of faster2.
 */

#include <iostream>
#include <string>

#include <jaz/string.hpp>

#include "index.hpp"
#include "pipe.hpp"


const char* VER = "0.29";
const char* DATE = "2014-03-17";


inline void usage() {
    std::cout << "Version: faster2 " << VER << " " << DATE << "\n";
    std::cout << "Copyright: (c) 2012-2014 Jaroslaw Zola <jaroslaw.zola@gmail.com>\n";
    std::cout << "License: Distributed under the MIT License\n";
    std::cout << "\n";
    std::cout << "Usage: faster2 DIR COMMAND|FILTER[,FILTER1,FILTER2,...]\n";
    std::cout << "where DIR is the database directory\n";
    std::cout << "and COMMAND is one of:\n";
    std::cout << "   index <'nt'|'aa'> ['fasta'|'fastq']           create database index\n";
    std::cout << "and FILTER is any of:\n";
    std::cout << "   filter <'N'|size>                             filter by string or size\n";
    std::cout << "   names [file]                                  write names of sequences\n";
    std::cout << "   print [file] ['fasta'|'fastq']                write sequences\n";
    std::cout << "   report [file]                                 write report\n";
    std::cout << "   sample <size> [seed]                          sample without replacement\n";
    std::cout << "   select '@'<name> | <name> [name1 name2 ...]   select by name\n";
    std::cout << "   anonymize                                     anonymize names\n";
} // usage


int main(int argc, char* argv[]) {
    if (argc < 3) {
	usage();
	return 0;
    }

    std::string dir = argv[1];
    std::string command = argv[2];

    bool res = true;
    std::string err = "";

    // here we go
    DBIndex db;

    if (command == "index") {
	// create index (list is to support paired reads)
	// not implemented for now
	if (argc < 4) {
	    usage();
	    return 0;
	}
	std::string alpha = argv[3];
	std::string type = "";
	if (argc == 5) type = argv[4];

	std::tie(res, err) = db.create(dir, alpha, type);

	if (res == false) {
	    std::cout << "Error: " << err << std::endl;
	    return -1;
	}
    } else {
	// construct pipe
	FasterPipe pipe(std::shared_ptr<FilterFactory>(new FilterFactory(dir)));
	std::tie(res, err) = pipe.set(jaz::join(FasterPipe::SEP, argv + 2, argv + argc));

	if (res == false) {
	    std::cout << "Error: " << err << std::endl;
	    return -1;
	}

	// read index
	std::tie(res, err) = db.read(dir);

	if (res == false) {
	    std::cout << "Error: " << err << std::endl;
	    return -1;
	}

	// execute pipe
	std::tie(res, err) = pipe.run(db.db());

	if (res == false) {
	    std::cout << "Error: " << err << std::endl;
	    return -1;
	}
    } // if

    return 0;
} // main
