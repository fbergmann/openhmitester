// -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
/*
 *   This program is free software; you can redistribute it and/or
 *   modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 3.0 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *   02111-1307 USA
 *
 *   This file is part of the Open-HMI Tester,
 *   http://openhmitester.sourceforge.net
 *
 */
#ifndef DATAMODELADAPTER_H
#define DATAMODELADAPTER_H

#include <datamodel.h>
#include <boost/shared_ptr.hpp>
#include <exception>


class DataModelAdapter
{
public:

    virtual ~DataModelAdapter() {}

    // conversion error exception
    class conversion_error_exception : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "conversion_error";
        }
    };

    // id method
    virtual std::string id() = 0;

    // transformation methods
    virtual DataModel::TestSuite*
    file2testSuite(const std::string& filename) throw (conversion_error_exception)  = 0;

    virtual void testSuite2file(const DataModel::TestSuite&,
                                const std::string& filename) throw (conversion_error_exception) = 0;
};

#endif // DATAMODELADAPTER_H
