#ifndef FDOSMLPSQSSPATIALCONTEXTCOLLECTION_H
#define FDOSMLPSQSSPATIALCONTEXTCOLLECTION_H		1
// 
//  Copyright (C) 2005 Autodesk, Inc. All Rights Reserved.
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

#ifdef _WIN32
#pragma once
#endif

#include "SpatialContext.h"
#include <Sm/Lp/SpatialContextCollection.h>

// Represents a collection of Spatial Contexts in Logical/Physical form.
class FdoSmLpSqsSpatialContextCollection : public FdoSmLpSpatialContextCollection
{
public:
	// Create an empty collection.
	FdoSmLpSqsSpatialContextCollection(void);

	// Create a populated collection.
	//
	// Parameters:
	//		physicalSchema: Physical Schema Manager. All spatial contexts at 
    //          the current connection are added to this collection.
	FdoSmLpSqsSpatialContextCollection( FdoSmPhMgrP physicalSchema );
	~FdoSmLpSqsSpatialContextCollection(void);

protected:
    // Construct a Spatial Context from readers
    virtual FdoSmLpSpatialContextP NewSpatialContext(
        FdoSmPhSpatialContextReaderP scReader,
        FdoSmPhSpatialContextGroupReaderP scgReader,
        FdoSmPhMgrP physicalSchema);

    // Construct a Spatial Context from the given attributes
    virtual FdoSmLpSpatialContextP NewSpatialContext(
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
	);


};

typedef FdoPtr<FdoSmLpSqsSpatialContextCollection> FdoSmLpSqsSpatialContextsP;


#endif
