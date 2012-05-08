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

#include "stdafx.h"
#include "SpatialContextGroupWriter.h"
#include <Sm/Ph/Rd/QueryReader.h>
#include <Sm/Ph/CommandWriter.h>


FdoSmPhPostGisSpatialContextGroupWriter::FdoSmPhPostGisSpatialContextGroupWriter(FdoSmPhMgrP mgr) : 
	FdoSmPhGrdSpatialContextGroupWriter(mgr)
{
    this->SetSubWriter( MakeWriter(mgr) );
}

FdoSmPhPostGisSpatialContextGroupWriter::~FdoSmPhPostGisSpatialContextGroupWriter(void)
{
}

FdoStringP FdoSmPhPostGisSpatialContextGroupWriter::ColNameXMin()
{
    return L"minx";
}

FdoStringP FdoSmPhPostGisSpatialContextGroupWriter::ColNameYMin()
{
    return L"miny";
}

FdoStringP FdoSmPhPostGisSpatialContextGroupWriter::ColNameZMin()
{
    return L"minz";
}

FdoStringP FdoSmPhPostGisSpatialContextGroupWriter::ColNameXMax()
{
    return L"maxx";
}

FdoStringP FdoSmPhPostGisSpatialContextGroupWriter::ColNameYMax()
{
    return L"maxy";
}

FdoStringP FdoSmPhPostGisSpatialContextGroupWriter::ColNameZMax()
{
    return L"maxz";
}
