/***
 *  $Id$
 **
 *  File: AnonymizeFilter.hpp
 *  Created: Mar 17, 2014
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2014 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying file LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef ANONYMIZE_FILTER_HPP
#define ANONYMIZE_FILTER_HPP

#include "AbstractFilter.hpp"


class AnonymizeFilter : public Filter {
public:
    explicit AnonymizeFilter(const std::vector<std::string>& args)
	: Filter("AnonymizeFilter") {
	if (args.empty() == false) error = "incorrect arguments";
    } // AnonymizeFilter

    std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	int pos = 0;
	for (auto& x : db) x.name = std::to_string(pos++);
	return { true, "" };
    } // run

}; // class AnonymizeFilter

#endif // ANONYMIZE_FILTER_HPP
