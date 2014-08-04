/***
 *  $Id$
 **
 *  File: SampleFilter.hpp
 *  Created: Jun 21, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012-2014 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying file LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef SAMPLE_FILTER_HPP
#define SAMPLE_FILTER_HPP

#include <string>
#include <random>
#include <vector>

#include <jaz/algorithm.hpp>

#include "AbstractFilter.hpp"
#include "index.hpp"


class SampleFilter : public Filter {
public:
    explicit SampleFilter(const std::vector<std::string>& args)
	: Filter("SampleFilter"), sample_(0), seed_(0) {
	if ((args.size() < 1) || (args.size() > 2)) error = "incorrect arguments";

	sample_ = std::atoi(args[0].c_str());
	if (sample_ == 0) error = "incorrect sample size";

	if (args.size() == 2) {
	    seed_ = std::atoi(args[1].c_str());
	    if (seed_ == 0) error = "incorrect seed";
	}
    } // SampleFilter


    std::pair<bool, std::string> run(std::vector<db_entry>& db) {
	std::mt19937 rand;
	std::random_device rd;

	if (seed_ == 0) rand.seed(rd()); else rand.seed(seed_);

	std::vector<db_entry> out;
	jaz::random_sample_n(db.begin(), db.end(), std::back_inserter(out), sample_, rand);

	db = out;

	return { true, "" };
    } // run


private:
    unsigned int sample_;
    unsigned int seed_;

}; // class SampleFilter

#endif // SAMPLE_FILTER_HPP
