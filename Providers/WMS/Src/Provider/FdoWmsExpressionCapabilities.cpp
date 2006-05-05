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
#include "FdoWmsGlobals.h"
#include "FdoWmsExpressionCapabilities.h"

FdoWmsExpressionCapabilities::FdoWmsExpressionCapabilities ()
{
}

FdoWmsExpressionCapabilities::~FdoWmsExpressionCapabilities ()
{
}

void FdoWmsExpressionCapabilities::Dispose ()
{
    delete this;
}

/// <summary>Returns an array of FdoExpressionType objects the feature provider supports.</summary>
/// <param name="length">Input the number of expression types</param> 
/// <returns>Returns the list of expression types</returns> 
FdoExpressionType* FdoWmsExpressionCapabilities::GetExpressionTypes (FdoInt32& length)
{
    static FdoExpressionType types[] =
    {
        FdoExpressionType_Function
    };

    length = sizeof (types) / sizeof (FdoExpressionType);

    return (types);
}

/// <summary>Returns a collection of FdoFunctionDefinition objects the feature provider supports within expressions.</summary>
/// <returns>Returns the collection of function definitions</returns> 
FdoFunctionDefinitionCollection* FdoWmsExpressionCapabilities::GetFunctions()
{
    FdoPtr<FdoFunctionDefinitionCollection> ret = FdoFunctionDefinitionCollection::Create ();

    // RESAMPLE function
    FdoPtr<FdoArgumentDefinitionCollection> args = FdoArgumentDefinitionCollection::Create();

    FdoPtr<FdoArgumentDefinition> rasterArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ResampleFunctionRasterArgumentName, 
        NlsMsgGet(FDOWMS_RESAMPLE_RASTER_ARGUMENT_DESC, "An argument that identifies the raster property definition."), 
        FdoDataType_BLOB);
    args->Add(rasterArg);

    FdoPtr<FdoArgumentDefinition> minXArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ResampleFunctionMinXArgumentName, 
        NlsMsgGet(FDOWMS_RESAMPLE_MINX_ARGUMENT_DESC, "The X coordinate corresponding to the lower left corner of the RESAMPLE boundary."), 
        FdoDataType_Double);
    args->Add(minXArg);

    FdoPtr<FdoArgumentDefinition> minYArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ResampleFunctionMinYArgumentName, 
        NlsMsgGet(FDOWMS_RESAMPLE_MINY_ARGUMENT_DESC, "The Y coordinate corresponding to the lower left corner of the RESAMPLE boundary."), 
        FdoDataType_Double);
    args->Add(minYArg);

    FdoPtr<FdoArgumentDefinition> maxXArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ResampleFunctionMaxXArgumentName, 
        NlsMsgGet(FDOWMS_RESAMPLE_MAXX_ARGUMENT_DESC, "The X coordinate corresponding to the upper right corner of the RESAMPLE boundary."), 
        FdoDataType_Double);
    args->Add(maxXArg);

    FdoPtr<FdoArgumentDefinition> maxYArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ResampleFunctionMaxYArgumentName, 
        NlsMsgGet(FDOWMS_RESAMPLE_MAXY_ARGUMENT_DESC, "The Y coordinate corresponding to the upper right corner of the RESAMPLE boundary."), 
        FdoDataType_Double);
    args->Add(maxYArg);

	FdoPtr<FdoArgumentDefinition> heightArg = FdoArgumentDefinition::Create(
		FdoWmsGlobals::ResampleFunctionHeightArgumentName,
		NlsMsgGet(FDOWMS_RESAMPLE_HEIGHT_ARGUMENT_DESC, "The HEIGHT of the output raster image."),
		FdoDataType_Int32);
	args->Add(heightArg);

	FdoPtr<FdoArgumentDefinition> widthArg = FdoArgumentDefinition::Create(
		FdoWmsGlobals::ResampleFunctionWidthArgumentName,
		NlsMsgGet(FDOWMS_RESAMPLE_WIDTH_ARGUMENT_DESC, "The WIDTH of the output raster image."),
		FdoDataType_Int32);
	args->Add(widthArg);

    FdoPtr<FdoFunctionDefinition> resampleFunction = FdoFunctionDefinition::Create(
        FdoWmsGlobals::ResampleFunctionName, 
        NlsMsgGet(FDOWMS_RESAMPLE_FUNCTION_DESC, "The RESAMPLE function returns a raster with the specified extents and size."), 
        FdoDataType_BLOB, 
        args);
    ret->Add(resampleFunction);

	// CLIP function
	args = FdoArgumentDefinitionCollection::Create();
	rasterArg = FdoArgumentDefinition::Create(
		FdoWmsGlobals::ClipFunctionRasterArgumentName,
		NlsMsgGet(FDOWMS_CLIP_RASTER_ARGUMENT_DESC, "An argument that identifies the raster property definition."),
		FdoDataType_BLOB);
	args->Add(rasterArg);

	minXArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ClipFunctionMinXArgumentName, 
        NlsMsgGet(FDOWMS_CLIP_MINX_ARGUMENT_DESC, "The X coordinate corresponding to the lower left corner of the CLIP boundary."), 
        FdoDataType_Double);
    args->Add(minXArg);

    minYArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ClipFunctionMinYArgumentName, 
        NlsMsgGet(FDOWMS_CLIP_MINY_ARGUMENT_DESC, "The Y coordinate corresponding to the lower left corner of the CLIP boundary."), 
        FdoDataType_Double);
    args->Add(minYArg);

    maxXArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ClipFunctionMaxXArgumentName, 
        NlsMsgGet(FDOWMS_CLIP_MAXX_ARGUMENT_DESC, "The X coordinate corresponding to the upper right corner of the CLIP boundary."), 
        FdoDataType_Double);
    args->Add(maxXArg);

    maxYArg = FdoArgumentDefinition::Create(
        FdoWmsGlobals::ClipFunctionMaxYArgumentName, 
        NlsMsgGet(FDOWMS_CLIP_MAXY_ARGUMENT_DESC, "The Y coordinate corresponding to the upper right corner of the CLIP boundary."), 
        FdoDataType_Double);
    args->Add(maxYArg);

	FdoPtr<FdoFunctionDefinition> clipFunction = FdoFunctionDefinition::Create(
		FdoWmsGlobals::ClipFunctionName,
		NlsMsgGet(FDOWMS_CLIP_FUNCTION_DESC, "The CLIP function returns a raster with the specified extents."),
		FdoDataType_BLOB,
		args);
	ret->Add(clipFunction);


    return (FDO_SAFE_ADDREF (ret.p));
}
