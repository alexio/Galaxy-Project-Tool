/***
 *  $Id$
 **
 *  File: pipe.hpp
 *  Created: Jun 20, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying LICENSE.
 *
 *  This file is part of faster2.
 */


#ifndef PIPE_HPP
#define PIPE_HPP

#include <memory>
#include <string>
#include <vector>

#include <jaz/string.hpp>

#include "FilterFactory.hpp"


class FasterPipe {
public:
    enum { SEP = 255 };

    explicit FasterPipe(std::shared_ptr<FilterFactory> ff) : factory(ff) { }


    virtual std::pair<bool, std::string> set(const std::string& cmd) {
	if (factory.get() == nullptr) throw std::runtime_error("FilterFactory not set");

	std::vector<std::string> clst;
	jaz::split(',', cmd, std::back_inserter(clst));

	std::vector<std::string> filters(factory->filters());
	std::sort(filters.begin(), filters.end());

	std::vector<std::string> flt;

	for (auto iter = clst.begin() + 1; iter != clst.end(); ++iter) {
	    flt.clear();
	    jaz::split(static_cast<char>(SEP), *iter, std::back_inserter(flt));

	    std::string s = flt[0];
	    if (s[0] == static_cast<char>(SEP)) s = std::string(s.begin() + 1, s.end());

	    if (std::binary_search(filters.begin(), filters.end(), s) == false) {
		*(iter - 1) += "," + *iter;
		iter = clst.erase(iter) - 1;
	    }
	}

	for (auto iter = clst.begin(); iter != clst.end(); ++iter) {
	    flt.clear();
	    jaz::split(static_cast<char>(SEP), *iter, std::back_inserter(flt));

	    pipe.push_back(factory->create(flt[0], std::vector<std::string>(flt.begin() + 1, flt.end())));

	    if (pipe.back().get() == nullptr) return { false, "unknown filter " + flt[0] };
	    if (pipe.back()->error.empty() == false) return { false, pipe.back()->name + ", " + pipe.back()->error };
	}

	return { true, "" };
    } // set

    virtual std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	bool res = true;
	std::string err = "";

	for (auto iter = pipe.begin(); iter != pipe.end(); ++iter) {
	    std::tie(res, err) = iter->get()->run(db);
	    if (res == false) return { res, iter->get()->name + ", " + err };
	}

	return { true, "" };
    } // run


protected:
    std::shared_ptr<FilterFactory> factory;
    std::vector<std::shared_ptr<Filter>> pipe;

}; // class FasterPipe

#endif // PIPE_HPP
