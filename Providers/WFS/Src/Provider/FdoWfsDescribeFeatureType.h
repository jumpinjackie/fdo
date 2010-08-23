/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef FDOWFSDESCRIBEFEATURETYPE_H
#define FDOWFSDESCRIBEFEATURETYPE_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsRequest.h>

class FdoWfsDescribeFeatureType : public FdoOwsRequest
{
private:
    FdoPtr<FdoStringCollection> m_typeNames;

protected:
    FdoWfsDescribeFeatureType();
    FdoWfsDescribeFeatureType(FdoStringCollection* typeNames);
	FdoWfsDescribeFeatureType(FdoStringCollection* typeNames,FdoString* version);
    virtual ~FdoWfsDescribeFeatureType();
    virtual void Dispose() { delete this; }

public:
    static FdoWfsDescribeFeatureType* Create(FdoStringCollection* typeNames);
	static FdoWfsDescribeFeatureType* Create(FdoStringCollection* typeNames,FdoString* version);

    virtual FdoStringP EncodeKVP();
    virtual FdoStringP EncodeXml();

   
};

typedef FdoPtr<FdoWfsDescribeFeatureType> FdoWfsDescribeFeatureTypeP;


#endif

