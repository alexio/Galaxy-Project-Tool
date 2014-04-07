/***
 *  $Id$
 **
 *  File: FilterFactory.hpp
 *  Created: Jun 20, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef FILTER_FACTORY_HPP
#define FILTER_FACTORY_HPP

#include <memory>
#include <string>
#include <vector>

#include "FilterFilter.hpp"
#include "NamesFilter.hpp"
#include "PrintFilter.hpp"
#include "ReportFilter.hpp"
#include "SampleFilter.hpp"
#include "SelectFilter.hpp"

#include "index.hpp"


class FilterFactory {
public:
    explicit FilterFactory(const std::string& dir) : path(dir) {
	filters_list = { "filter", "names", "print", "report", "sample", "select" };
    }; // FilterFactory

    virtual const std::vector<std::string>& filters() const { return filters_list; }

    virtual std::shared_ptr<Filter> create(const std::string& name,
					   const std::vector<std::string>& args) {

	if (name == "filter") return std::shared_ptr<Filter>(new FilterFilter(args));
	if (name == "names") return std::shared_ptr<Filter>(new NamesFilter(args));
	if (name == "print") return std::shared_ptr<Filter>(new PrintFilter(path, args));
	if (name == "report") return std::shared_ptr<Filter>(new ReportFilter(args));
	if (name == "sample") return std::shared_ptr<Filter>(new SampleFilter(args));
	if (name == "select") return std::shared_ptr<Filter>(new SelectFilter(args));

	return std::shared_ptr<Filter>(nullptr);
    } // create

protected:
    std::string path;
    std::vector<std::string> filters_list;

}; // class FilterFactory

#endif // FILTER_FACTORY_HPP
