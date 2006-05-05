//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef FDOWMSDELEGATE_H
#define FDOWMSDELEGATE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <OWS/FdoOwsDelegate.h>
#include "FdoWmsBoundingBox.h"

class FdoWmsServiceMetadata;

class FdoWmsDelegate : public FdoOwsDelegate
{
protected:
    FdoWmsDelegate();
    FdoWmsDelegate(FdoString* defaultUrl, FdoString* userName, FdoString* passwd);
    virtual ~FdoWmsDelegate();
    virtual void Dispose();

public:
    static FdoWmsDelegate* Create(FdoString* defaultUrl, FdoString* userName, FdoString* passwd);

public:
    FdoWmsServiceMetadata* GetServiceMetadata();

	FdoIoStream* GetMap(FdoStringCollection* layerNames, 
		FdoStringCollection* styleNames,
		FdoWmsBoundingBox* bbox, 
		FdoString* imgFormat,
		FdoSize height,
		FdoSize width,
		FdoBoolean bTransparent,
		FdoString* backgroundColor,
		FdoString* timeDimension,
		FdoString* elevation,
		FdoString* version);
};

typedef FdoPtr<FdoWmsDelegate> FdoWmsDelegateP;

#endif


