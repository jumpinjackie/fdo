//
//  Copyright (C) 2001-2003 Autodesk, Inc. All Rights Reserved.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
//  CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
//  IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
//  DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
//  DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject
//  to restrictions set forth in FAR 52.227-19 (Commercial Computer
//  Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
//  (Rights in Technical Data and Computer Software), as applicable.


#include "stdafx.h"
#include "ShpCreateSpatialContextCommand.h"

ShpCreateSpatialContextCommand::ShpCreateSpatialContextCommand()
: FdoCommonCommand<FdoICreateSpatialContext, ShpConnection>(NULL)
{
    mConnection = (ShpConnection *)NULL;
    mExtentType = FdoSpatialContextExtentType_Static;
    mExtent = NULL;
    mXYTolerance = 0;
    mZTolerance = 0;
    mUpdateExisting = false;
}

ShpCreateSpatialContextCommand::ShpCreateSpatialContextCommand(FdoIConnection *connection)
: FdoCommonCommand<FdoICreateSpatialContext, ShpConnection>(connection)
{
    mConnection = (ShpConnection *)connection;
    mExtentType = FdoSpatialContextExtentType_Static;
    mExtent = NULL;
    mXYTolerance = 0;
    mZTolerance = 0;
    mUpdateExisting = false;
}

ShpCreateSpatialContextCommand::~ShpCreateSpatialContextCommand()
{
}

FdoString* ShpCreateSpatialContextCommand::GetName()
{
    return mSCName;
}

void ShpCreateSpatialContextCommand::SetName(FdoString* value)
{
    mSCName = value;
}

FdoString* ShpCreateSpatialContextCommand::GetDescription()
{
    return mDescription;
}

void ShpCreateSpatialContextCommand::SetDescription(FdoString* value)
{
    mDescription = value;
}

FdoString* ShpCreateSpatialContextCommand::GetCoordinateSystem()
{
    return mCoordSysName;
}

void ShpCreateSpatialContextCommand::SetCoordinateSystem(FdoString* value)
{
    mCoordSysName = value;
}

FdoString* ShpCreateSpatialContextCommand::GetCoordinateSystemWkt()
{
    return mCoordSysWkt;
}

void ShpCreateSpatialContextCommand::SetCoordinateSystemWkt(FdoString* value)
{
    mCoordSysWkt = value;
}

FdoSpatialContextExtentType ShpCreateSpatialContextCommand::GetExtentType()
{
    return mExtentType;
}

void ShpCreateSpatialContextCommand::SetExtentType(FdoSpatialContextExtentType value)
{
    mExtentType = value;
}

FdoByteArray* ShpCreateSpatialContextCommand::GetExtent()
{
    return FDO_SAFE_ADDREF( mExtent.p );
}

void ShpCreateSpatialContextCommand::SetExtent(FdoByteArray* value)
{
    mExtent = FDO_SAFE_ADDREF( value );
}

const double ShpCreateSpatialContextCommand::GetXYTolerance()
{
    return mXYTolerance;
}

void ShpCreateSpatialContextCommand::SetXYTolerance(double value)
{
    mXYTolerance = value;
}

const double ShpCreateSpatialContextCommand::GetZTolerance()
{
    return mZTolerance;
}

void ShpCreateSpatialContextCommand::SetZTolerance(const double value)
{
    mZTolerance = value;
}

const bool ShpCreateSpatialContextCommand::GetUpdateExisting()
{
    return mUpdateExisting;
}

void ShpCreateSpatialContextCommand::SetUpdateExisting(const bool value)
{
    mUpdateExisting = value;
}

void ShpCreateSpatialContextCommand::Execute()
{
	// Ignore all the settings except the WKT.
	// Extract the useful info from wkt.
	
	if ( mCoordSysWkt == L"" )
		throw FdoException::Create( NlsMsgGet( SHP_SPATIALCONTEXT_EMPTYWKT, "WKT cannot be empty." ));

	FdoStringP	right;
	FdoStringP	csName;

	// Parse the WKT. Ignore spaces. The same is done PRJ
	if ( mCoordSysWkt.Contains( L"PROJCS" ) )
		right = mCoordSysWkt.Right( L"PROJCS" );
	else if ( mCoordSysWkt.Contains( L"GEOGCS" ) )
		right = mCoordSysWkt.Right( L"GEOGCS" );
	else if ( mCoordSysWkt.Contains( L"LOCAL_CS" ) )
		right = mCoordSysWkt.Right( L"LOCAL_CS" );

	if ( right != L"" )
	{
		FdoStringP right2 = right.Right( L"[" );
		FdoStringP right3 = right2.Right( L"\"" );
		csName = right3.Left( L"\"" );
	}

	if ( csName == L"" )
	        throw FdoException::Create( NlsMsgGet( SHP_SPATIALCONTEXT_INVALIDWKT, "Invalid WKT. Cannot extract coordinate system name." ) );

	// If passed in, the SC name should match.
	if ( mCoordSysName != L"" && mCoordSysName != csName ) {
	        throw FdoException::Create( NlsMsgGet( SHP_SPATIALCONTEXT_CONFLICTINGWKT, 
													"Spatial Context name conflicts with the coordinate system name in WKT." ) );
	}

	mCoordSysName = csName;
	mSCName = mCoordSysName;

    mConnection->CreateSpatialContext(
        mSCName, mDescription,
        mCoordSysName, mCoordSysWkt,
        mExtentType, mExtent,
        mXYTolerance, mZTolerance,
		false );

#pragma message ("ToDo: Support updating spatial contexts")
}
