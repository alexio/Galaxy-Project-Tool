/***
 *  $Id$
 **
 *  File: stream.hpp
 *  Created: Jun 21, 2012
 *
 *  Author: Jaroslaw Zola <jaroslaw.zola@gmail.com>
 *  Copyright (c) 2012-2014 Jaroslaw Zola
 *  Distributed under the MIT License.
 *  See accompanying file LICENSE.
 *
 *  This file is part of faster2.
 */

#ifndef STREAM_HPP
#define STREAM_HPP

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>


template <typename FilterStream>
inline std::istream* open_stream(const fs::path& name, std::ifstream& fs, FilterStream& cs) {
    std::istream* is = 0;

    fs.close();
    cs.reset();

    if ((name.extension() != ".bz2") && (name.extension() != ".gz")) {
        fs.open(name.string().c_str());
        if (fs) is = &fs;
    } else {
        fs.open(name.string().c_str(), std::ios_base::binary);
        if (fs) {
            if (name.extension() == ".gz") cs.push(boost::iostreams::gzip_decompressor());
            else cs.push(boost::iostreams::bzip2_decompressor());
            cs.push(fs);
	    is = &cs;
        }
    }

    return is;
} // open_stream

#endif // STREAM_HPP
