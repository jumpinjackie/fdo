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
#include "SpatialContextCollection.h"

FdoSmLpSqsSpatialContextCollection::FdoSmLpSqsSpatialContextCollection(FdoSmPhMgrP physicalSchema) :
	FdoSmLpSpatialContextCollection(physicalSchema)
{
}

FdoSmLpSqsSpatialContextCollection::~FdoSmLpSqsSpatialContextCollection(void)
{
}

FdoSmLpSpatialContextP FdoSmLpSqsSpatialContextCollection::NewSpatialContext(
    FdoSmPhSpatialContextReaderP scReader,
    FdoSmPhSpatialContextGroupReaderP scgReader,
    FdoSmPhMgrP physicalSchema
)
{
    FdoSmLpSpatialContextP sc = new FdoSmLpSqsSpatialContext(
        scReader, scgReader, physicalSchema );

    return sc;
}


FdoSmLpSpatialContextP FdoSmLpSqsSpatialContextCollection::NewSpatialContext(
        FdoString* name,
        FdoString* description,
        FdoString* coordinateSystem,
        FdoString* coordinateSystemWkt,
        FdoSpatialContextExtentType extentType,
        FdoByteArray * extent,
        double xyTolerance,
        double zTolerance,
        bool bIgnoreStates,
        FdoSmPhMgrP physicalSchema
)
{
    FdoSmLpSpatialContextP sc = new FdoSmLpSqsSpatialContext(
        name, description,
        coordinateSystem, coordinateSystemWkt,
        extentType, extent,
        xyTolerance, zTolerance,
        physicalSchema);

    return sc;
}

