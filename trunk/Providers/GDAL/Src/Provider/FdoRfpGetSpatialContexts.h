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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpGetSpatialContexts.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPGETSPATIALCONTEXTS_H
#define FDORFPGETSPATIALCONTEXTS_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include "FdoRfpCommand.h"

class FdoRfpConnection;

class FdoRfpGetSpatialContexts : public FdoRfpCommand<FdoIGetSpatialContexts>
{
	friend class FdoRfpConnection;
private:
	bool m_bActiveOnly;
protected:
	FdoRfpGetSpatialContexts(FdoIConnection* connection);
	virtual ~FdoRfpGetSpatialContexts();
	void Dispose() { delete this; }
public:
    /// <summary> Gets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.</summary>
    /// <returns>Returns Boolean value</returns> 
    virtual const bool GetActiveOnly();

    /// <summary> Sets a Boolean flag that indicates if the GetSpatialContexts command
    /// will return only the active spatial context or all spatial contexts. The
    /// default value of this flag is false, return all spatial contexts.</summary>
    /// <param name="value">Input the Boolean flag</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetActiveOnly(const bool value);

    /// <summary>Executes the GetSpatialContexts command returning an FdoISpatialContextReader.</summary>
    /// <returns>Returns nothing</returns> 
    virtual FdoISpatialContextReader* Execute();

};

typedef FdoPtr<FdoRfpGetSpatialContexts> FdoRfpGetSpatialContextsP;

#endif
