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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpExpressionCapabilities.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpExpressionCapabilities.h"
#include "FdoRfpGlobals.h"

FdoRfpExpressionCapabilities::FdoRfpExpressionCapabilities(void)
{
}

FdoRfpExpressionCapabilities::~FdoRfpExpressionCapabilities(void)
{
}

FdoExpressionType* FdoRfpExpressionCapabilities::GetExpressionTypes(int& length)
{
    static FdoExpressionType types[] =
    {
      FdoExpressionType_Function 
    };

    length = sizeof (types) / sizeof (FdoExpressionType);

    return (types);
}

FdoFunctionDefinitionCollection* FdoRfpExpressionCapabilities::GetFunctions()
{
    FdoPtr<FdoFunctionDefinitionCollection> ret = FdoFunctionDefinitionCollection::Create ();

	// MOSAIC()
    FdoPtr<FdoArgumentDefinitionCollection> args = FdoArgumentDefinitionCollection::Create();
	FdoPtr<FdoArgumentDefinition> rasterArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::RasterArgumentName, NlsMsgGet(GRFP_81_RASTER_ARGUMENT_DESC, "An argument that accepts any expression that evaluates to a raster."), FdoDataType_BLOB);
    args->Add(rasterArg);
	FdoPtr<FdoFunctionDefinition> mosaicFunction = FdoFunctionDefinition::Create(FdoGrfpGlobals::MosaicFunctionName, NlsMsgGet(GRFP_82_MOSAIC_FUNCTION_DESC, "Returns the stitched raster of a series of rasters."), FdoDataType_BLOB, args);
    ret->Add(mosaicFunction);

	// CLIP()
	args = FdoArgumentDefinitionCollection::Create();
	rasterArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::RasterArgumentName, NlsMsgGet(GRFP_81_RASTER_ARGUMENT_DESC, "An argument that accepts any expression that evaluates to a raster."), FdoDataType_BLOB);
    args->Add(rasterArg);

	FdoString* dbDesc = NlsMsgGet(GRFP_86_DOUBLE_ARGUMENT_DESC, "An argument that accepts a double value.");
	FdoPtr<FdoArgumentDefinition> minXArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::MinXArgumentName, dbDesc, FdoDataType_Double);
	args->Add(minXArg);
	FdoPtr<FdoArgumentDefinition> minYArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::MinYArgumentName, dbDesc, FdoDataType_Double);
	args->Add(minYArg);
	FdoPtr<FdoArgumentDefinition> maxXArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::MaxXArgumentName, dbDesc, FdoDataType_Double);
	args->Add(maxXArg);
	FdoPtr<FdoArgumentDefinition> maxYArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::MaxYArgumentName, dbDesc, FdoDataType_Double);
	args->Add(maxYArg);
	FdoPtr<FdoFunctionDefinition> clipFunction = FdoFunctionDefinition::Create(FdoGrfpGlobals::ClipFunctionName, NlsMsgGet(GRFP_87_CLIP_FUNCTION_DESC, "Returns subset of the given raster."), FdoDataType_BLOB, args);
    ret->Add(clipFunction);

	// RESAMPLE()
	args = FdoArgumentDefinitionCollection::Create();
	rasterArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::RasterArgumentName, NlsMsgGet(GRFP_81_RASTER_ARGUMENT_DESC, "An argument that accepts any expression that evaluates to a raster."), FdoDataType_BLOB);
    args->Add(rasterArg);

	dbDesc = NlsMsgGet(GRFP_86_DOUBLE_ARGUMENT_DESC, "An argument that accepts a double value.");
	FdoString* uiDesc = NlsMsgGet(GRFP_107_UINT_ARGUMENT_DESC, "An argument that accepts a unsigned integer value.");
	minXArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::MinXArgumentName, dbDesc, FdoDataType_Double);
	args->Add(minXArg);
	minYArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::MinYArgumentName, dbDesc, FdoDataType_Double);
	args->Add(minYArg);
	maxXArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::MaxXArgumentName, dbDesc, FdoDataType_Double);
	args->Add(maxXArg);
	maxYArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::MaxYArgumentName, dbDesc, FdoDataType_Double);
	args->Add(maxYArg);
	FdoPtr<FdoArgumentDefinition> heightArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::HeightArgumentName, uiDesc, FdoDataType_Int32);
	args->Add(heightArg);
	FdoPtr<FdoArgumentDefinition> widthArg = FdoArgumentDefinition::Create(FdoGrfpGlobals::WidthArgumentName, uiDesc, FdoDataType_Int32);
	args->Add(widthArg);
	FdoPtr<FdoFunctionDefinition> resampleFunction = FdoFunctionDefinition::Create(FdoGrfpGlobals::ResampleFunctionName, NlsMsgGet(GRFP_106_RESAMPLE_FUNCTION_DESC, "Resample subset of the given raster."), FdoDataType_BLOB, args);
    ret->Add(resampleFunction);

    return (FDO_SAFE_ADDREF (ret.p));
}

void FdoRfpExpressionCapabilities::Dispose()
{
	delete this;
}
