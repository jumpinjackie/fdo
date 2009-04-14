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
 */

#ifndef SHPGETSPATIALCONTEXTSCOMMAND_H
#define SHPGETSPATIALCONTEXTSCOMMAND_H


class ShpGetSpatialContextsCommand :
    public FdoCommonCommand<FdoIGetSpatialContexts, ShpConnection>
{
protected:
    bool mActiveOnly;

public:
    // 0-argument constructor to please FdoPtr::operator->() :
    ShpGetSpatialContextsCommand ();

    ShpGetSpatialContextsCommand (FdoIConnection *connection);
    virtual ~ShpGetSpatialContextsCommand (void);

public:

    //
    // FdoIGetSpatialContexts interface
    //

    /// <summary> Gets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.</summary>
    /// <returns>Returns Boolean value</returns> 
    virtual const bool GetActiveOnly ();

    /// <summary> Sets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.</summary>
    /// <param name="value">Input the Boolean flag</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetActiveOnly (const bool value);

    /// <summary>Executes the GetSpatialContexts command returning an FdoISpatialContextReader.</summary>
    /// <returns>Returns nothing</returns> 
    virtual FdoISpatialContextReader* Execute ();
};


#endif // SHPGETSPATIALCONTEXTSCOMMAND_H

