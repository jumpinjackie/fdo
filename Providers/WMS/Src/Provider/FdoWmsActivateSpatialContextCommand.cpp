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
#include "FdoWmsActivateSpatialContextCommand.h"

FdoWmsActivateSpatialContextCommand::FdoWmsActivateSpatialContextCommand (FdoWmsConnection* connection)
	: FdoWmsFeatureCommand<FdoIActivateSpatialContext> (connection)
{
}

void FdoWmsActivateSpatialContextCommand::Execute()
{	
	FdoWmsServiceMetadataP metadata = mConnection->GetWmsServiceMetadata ();
	FdoWmsCapabilitiesP capability = static_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());

	// get all the supported CRS by the server 
	FdoPtr<FdoStringCollection> crsNames = capability->GetCRSNames ();

	// Check whether the spatial context is supported by the server. If not,
	// an exception will be thrown out.
	if (crsNames->IndexOf (mActiveSpatialContext) == -1)
	{
		throw FdoCommandException::Create (
			NlsMsgGet(FDOWMS_SPATIALCONTEXT_NOT_SUPPORTED, 
			"The spatial context '%1$ls' is not supported by the server.", 
			(FdoString*)mActiveSpatialContext));
	}
}

FdoString* FdoWmsActivateSpatialContextCommand::GetName ()
{
	return mActiveSpatialContext;
}

void FdoWmsActivateSpatialContextCommand::SetName (FdoString* value)
{
	if (value == NULL || wcslen (value) == 0)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method"));

	mActiveSpatialContext = value;
	mConnection->SetActiveSpatialContext (mActiveSpatialContext);
}
