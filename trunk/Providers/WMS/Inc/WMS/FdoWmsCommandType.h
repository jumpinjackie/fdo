#ifndef WMSCOMMANDTYPE_H
#define WMSCOMMANDTYPE_H

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

#ifdef _WIN32
#pragma once
#endif

#include "Fdo/Commands/CommandType.h"

// This number is defined so our custom command would not clash with other
// custom provider commands.
#define WMS_FIRST_PROVIDER_COMMAND 1800

/// \brief
/// The WmsCommandType enumeration defines the list of WMS+ commands.

enum FdoWmsCommandType{

    /// Represents the GetImageFormats command.
    FdoWmsCommandType_GetImageFormats =  FdoCommandType_FirstProviderCommand + WMS_FIRST_PROVIDER_COMMAND,
    FdoWmsCommandType_GetFeatureClassStyles,
    FdoWmsCommandType_GetFeatureClassCRSNames,
    FdoWmsCommandType_GetFeatureInfo
};

#endif


