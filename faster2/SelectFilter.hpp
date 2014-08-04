/***
 *  $Id$
 **
 *  File: SelectFilter.hpp
 *  Created: Jun 21, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012-2014 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying file LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef SELECT_FILTER_HPP
#define SELECT_FILTER_HPP

#include <fstream>
#include <string>
#include <vector>

#include <jaz/iterator.hpp>


class SelectFilter : public Filter {
public:

    class not_in_args {
    public:
	explicit not_in_args(const std::vector<std::string>& args) : args_(args) { }

	bool operator()(const db_entry& de) const {
	    return !std::binary_search(args_.begin(), args_.end(), de.name);
	} // operator()

    private:
	const std::vector<std::string>& args_;

    }; // class not_in_args


    explicit SelectFilter(const std::vector<std::string>& args)
	: Filter("SelectFilter"), args_(args) { }


    std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	if (args_.empty()) return { false, "empty name list" };
	if (args_[0][0] == '@') {
	    std::ifstream f(args_[0].c_str() + 1);
	    if (!f) return { false, "failed to open name list" };
	    args_.clear();
	    jaz::getline_iterator<> it(f), end;
	    std::copy(it, end, std::back_inserter(args_));
	    f.close();
	}
	std::sort(args_.begin(), args_.end());
	db.erase(std::remove_if(db.begin(), db.end(), not_in_args(args_)), db.end());
	return { true, "" };
    } // run


private:
    std::vector<std::string> args_;

}; // class SelectFilter

#endif // SELECT_FILTER_HPP
