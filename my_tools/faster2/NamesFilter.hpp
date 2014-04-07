/***
 *  $Id$
 **
 *  File: NamesFilter.hpp
 *  Created: Jun 22, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef NAMES_FILTER_HPP
#define NAMES_FILTER_HPP

class NamesFilter : public Filter {
public:
    explicit NamesFilter(const std::vector<std::string>& args)
	: Filter("NamesFilter"), name_("-") {
	if (args.size() > 1) error = "incorrect arguments";
	if (args.empty() == false) name_ = args[0];
    } // NamesFilter

    std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	std::ostream* os = &std::cout;
	std::ofstream of;

	if (name_ != "-") {
	    of.open(name_.c_str());
	    if (!of) return { false, "failed to create " + name_ };
	    os = &of;
	}

	if (db.empty() == true) return { true, "" };

	for (db_entry& de : db) *os << de.name << "\n";
	*os << std::endl;

	return { true, "" };
    } // run


private:
    std::string name_;

}; // class NamesFilter

#endif // NAMES_FILTER_HPP
