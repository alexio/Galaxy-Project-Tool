/***
 *  $Id$
 **
 *  File: ReportFilter.hpp
 *  Created: Jun 21, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012-2014 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying file LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef REPORT_FILTER_HPP
#define REPORT_FILTER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <jaz/algorithm.hpp>

#include "AbstractFilter.hpp"
#include "index.hpp"


class ReportFilter : public Filter {
public:
    explicit ReportFilter(const std::vector<std::string>& args)
	: Filter("ReportFilter"), args_(args) {
	if (args_.size() > 1) error = "incorrect arguments";
    } // ReportFilter


    std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	std::ostream* os = &std::cout;
	std::ofstream of;

	if ((args_.size() > 0) && (args_[0] != "-")) {
	    of.open(args_[0].c_str());
	    if (!of) return { false, "failed to create " + args_[0] };
	    os = &of;
	}


	std::sort(db.begin(), db.end(),
		  [](const db_entry& l, const db_entry& r) -> bool {
		      return l.file < r.file;
		  });

	*os << "total files\t" << jaz::count_unique(db.begin(), db.end(),
						    [](const db_entry& l, const db_entry& r) -> bool {
							return l.file == r.file;
						    })
	    << std::endl;


	*os << "total sequences\t" << db.size() << std::endl;

	if (db.empty() == true) return { true, "" };


	*os << "quality scores\t" << (db.back().fastq ? "yes" : "no") << std::endl;

	*os << "clean sequences\t" << std::count_if(db.begin(), db.end(),
						    [](const db_entry& de) -> bool {
							return de.hasN == false;
						    })
	    << std::endl;


	double avg = 0.0;
	for (auto iter = db.begin(); iter != db.end(); ++iter) avg += iter->size;

	*os << "average sequence\t" << avg / db.size() << std::endl;


	*os << "shortest sequence\t" << std::min_element(db.begin(), db.end(),
							 [](const db_entry& l, const db_entry& r) -> bool {
							     return l.size < r.size;
							 })->size
	    << std::endl;


	*os << "longest sequence\t" << std::max_element(db.begin(), db.end(),
							[](const db_entry& l, const db_entry& r) -> bool {
							    return l.size < r.size;
							})->size
	    << std::endl;


    	return { true, "" };
    } // run


private:
    std::vector<std::string> args_;

}; // class ReportFilter

#endif // REPORT_FILTER_HPP
