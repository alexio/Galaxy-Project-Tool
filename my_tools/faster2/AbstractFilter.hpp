/***
 *  $Id$
 **
 *  File: AbstractFilter.hpp
 *  Created: Jun 21, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef ABSTRACT_FILTER_HPP
#define ABSTRACT_FILTER_HPP

#include <string>
#include <vector>

#include "index.hpp"


class Filter {
public:
    explicit Filter(const std::string& arg) : name(arg) { }
    virtual ~Filter() { }

    virtual std::pair<bool, std::string> run(std::vector<db_entry>& db) = 0;

    std::string name;
    std::string error;
}; // class Filter

#endif // ABSTRACT_FILTER_HPP
