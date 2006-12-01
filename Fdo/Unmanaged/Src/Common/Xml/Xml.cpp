// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <FdoCommon.h>

const FdoStringP FdoXml::mXmlnsPref( L"xmlns", true );

const FdoStringP FdoXml::mXsNs( 	 L"xs", true );
const FdoStringP FdoXml::mXsUri(	 L"http://www.w3.org/2001/XMLSchema", true );
const FdoStringP FdoXml::mXsiNs(	 L"xsi", true );
const FdoStringP FdoXml::mXsiUri(	 L"http://www.w3.org/2001/XMLSchema-instance", true );
const FdoStringP FdoXml::mXlinkNs(	 L"xlink", true );
const FdoStringP FdoXml::mXlinkUri(	 L"http://www.w3.org/1999/xlink", true );
const FdoStringP FdoXml::mGmlNs(	 L"gml", true ); 
const FdoStringP FdoXml::mGmlUri(	 L"http://www.opengis.net/gml", true );
const FdoStringP FdoXml::mFdoNs(	 L"fdo", true );
const FdoStringP FdoXml::mFdoUri(	 L"http://fdo.osgeo.org/schemas", true );
const FdoStringP FdoXml::mFdsNs(	 L"fds", true );
const FdoStringP FdoXml::mFdsUri(	 L"http://fdo.osgeo.org/schemas/fds", true );
const FdoStringP FdoXml::mWfsNs(	 L"wfs", true );
const FdoStringP FdoXml::mWfsUri(	 L"http://www.opengis.net/wfs", true );
 
const FdoStringP FdoXml::mDefaultRoot(FdoXml::mFdoNs + L":DataStore", true);

const FdoStringP FdoXml::mGmlProviderName( L"OSGeo.GML.1.0", true );

const FdoStringP FdoXml::mFeatureCollectionName(L"FeatureCollection", true);
const FdoStringP FdoXml::mFeatureMemberName(L"featureMember", true);

