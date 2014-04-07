/***
 *  $Id$
 **
 *  File: SelectFilter.hpp
 *  Created: Jun 21, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef SELECT_FILTER_HPP
#define SELECT_FILTER_HPP

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
	: Filter("SelectFilter"), args_(args) {
	std::sort(args_.begin(), args_.end());
    } // SelectFilter


    std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	db.erase(std::remove_if(db.begin(), db.end(), not_in_args(args_)), db.end());
	return { true, "" };
    } // run


private:
    std::vector<std::string> args_;

}; // class SelectFilter

#endif // SELECT_FILTER_HPP
