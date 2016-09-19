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
#include "SdfCommand.h"

/// <summary>The IGetSpatialContexts interface defines the GetSpatialContexts command,
/// which enumerates the existing spatial contexts.</summary>
class SdfGetSpatialContexts : public SdfCommand<FdoIGetSpatialContexts>
{
    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

public:

    SdfGetSpatialContexts(SdfConnection* connection);

protected:

    virtual ~SdfGetSpatialContexts();


    //-------------------------------------------------------
    // FdoIGetSpatialContexts implementation
    //-------------------------------------------------------

public:

    /// <summary> Gets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.</summary>
    /// <returns>Returns Boolean value</returns> 
    SDF_API virtual const bool GetActiveOnly();

    /// <summary> Sets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.</summary>
    /// <param name="value">Input the Boolean flag</param> 
    /// <returns>Returns nothing</returns> 
    SDF_API virtual void SetActiveOnly(const bool value);

    /// <summary>Executes the GetSpatialContexts command returning an FdoISpatialContextReader.</summary>
    /// <returns>Returns nothing</returns> 
    SDF_API virtual FdoISpatialContextReader* Execute();

private:

    bool m_bActiveOnly;
};


