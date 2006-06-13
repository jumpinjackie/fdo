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

#ifndef FDORFPUTIL_H
#define FDORFPUTIL_H

#ifdef _WIN32
#pragma once
#endif
#include "FdoRfpRect.h"

class FdoRfpUtil
{
public:
	static FdoPtr<FdoByteArray> CreateGeometryAgfFromRect(const FdoRfpRect& rect)
	{
		FdoPtr<FdoFgfGeometryFactory> agfFactory = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoIEnvelope> envelope = agfFactory->CreateEnvelopeXY(rect.m_minX, rect.m_minY, rect.m_maxX, rect.m_maxY);
		FdoPtr<FdoIGeometry> geometry = agfFactory->CreateGeometry(envelope);
		return agfFactory->GetFgf(geometry);
	}

	static FdoRfpRect CreateRectFromGeometryAgf(const FdoPtr<FdoByteArray>& geoValue)
	{
		FdoPtr<FdoFgfGeometryFactory> agfFactory = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoIGeometry> geometry = agfFactory->CreateGeometryFromFgf(geoValue);
		FdoPtr<FdoIEnvelope> envelope = geometry->GetEnvelope();
		return 	FdoRfpRect(envelope->GetMinX(), envelope->GetMinY(), envelope->GetMaxX(), envelope->GetMaxY());
	}

	static FdoPtr<FdoFilter> CreateSpatialCondition(FdoString* propertyName, FdoSpatialOperations operation, const FdoRfpRect& rect)
	{
		FdoPtr<FdoGeometryValue> geoValue = FdoGeometryValue::Create(CreateGeometryAgfFromRect(rect));
		return FdoSpatialCondition::Create(propertyName, operation, geoValue);
	}

	// parse the qualified propery name. The name may be one of the following formats:
	// 1. feature:class.propertyName
	// 2. class.propertyName
	// 3. propertyName
	static void ParseQPropertyName(FdoString* qname, FdoStringP& featureName, FdoStringP& className, FdoStringP& propertyName)
	{
		std::wstring qname1(qname);
		// try to find the feature name
		std::wstring::size_type delimiter = qname1.find(L':');
		if (delimiter != -1)
		{
			featureName = qname1.substr(0, delimiter).c_str();
			qname1.erase(0, delimiter + 1);
		}
		
		// try to find the class name
		delimiter = qname1.find(L'.');
		if (delimiter != -1)
		{
			className = qname1.substr(0, delimiter).c_str();
			qname1.erase(0, delimiter + 1);
		}

		// find the property name
		propertyName = qname1.c_str();
	}

	// parse the qualified class name. The name may be one of the following formats:
	// 1. featureName:className
	// 2. className
	static void ParseQClassName(FdoString* qname, FdoStringP& featureName, FdoStringP& className)
	{
		std::wstring qname1(qname);
		// try to find the feature name
		std::wstring::size_type delimiter = qname1.find(L':');
		if (delimiter != -1)
		{
			featureName = qname1.substr(0, delimiter).c_str();
			qname1.erase(0, delimiter + 1);
		}

		// find the class name
		className = qname1.c_str();
	}

};

#endif
