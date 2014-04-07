/***
 *  $Id$
 **
 *  File: FilterFilter.hpp
 *  Created: Jun 21, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef FILTER_FILTER_HPP
#define FILTER_FILTER_HPP

#include <cstdlib>
#include <functional>
#include <string>
#include <vector>

#include "AbstractFilter.hpp"
#include "index.hpp"


class FilterFilter : public Filter {
public:
    struct hasN {
	bool operator()(const db_entry& de) const { return de.hasN; }
    }; // struct hasN


    explicit FilterFilter(const std::vector<std::string>& args)
	: Filter("FilterFilter"), N_(false), oper_(0), size_(0) {
	if ((args.size() != 1) || (args[0].empty() == true)) error = "incorrect arguments";

	oper_ = args[0][0];

	if (oper_ == 'N') N_ = true;
	else if ((oper_ != '-') && (oper_ != '+')) {
	    error = "incorrect size sign";
	} else {
	    size_ = std::atoi(std::string(args[0].begin() + 1, args[0].end()).c_str());
	    if (size_ == 0) error = "incorrect size";
	}
    } // FilterFilter


    std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	auto iter = db.end();

	if (N_ == true) iter = std::remove_if(db.begin(), db.end(), hasN());
	else {
	    switch (oper_) {
	      case '-':
		  iter = std::remove_if(db.begin(), db.end(),
					[=](const db_entry& de) -> bool {
					    return size_ < de.size;
					});
		  break;

	      case '+':
		  iter = std::remove_if(db.begin(), db.end(),
					[=](const db_entry& de) -> bool {
					    return de.size < size_;
					});
		  break;
	    } // switch
	}

	db.erase(iter, db.end());

	return { true, "" };
    } // run


private:
    bool N_;
    char oper_;
    unsigned int size_;

}; // class FilterFilter

#endif // FILTER_FILTER_HPP
