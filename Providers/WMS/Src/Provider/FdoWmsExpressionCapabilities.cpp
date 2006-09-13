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

    FdoString* desc = NlsMsgGet(FDOWMS_RESAMPLE_RASTER_ARGUMENT_DESC, "An argument that identifies the raster property definition.");
    FdoPtr<FdoArgumentDefinition> rasterArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ResampleFunctionRasterArgumentName, 
                                                                            desc, 
                                                                            FdoDataType_BLOB);
    args->Add(rasterArg);

    desc = NlsMsgGet(FDOWMS_RESAMPLE_MINX_ARGUMENT_DESC,
                     "The X coordinate corresponding to the lower left corner of the RESAMPLE boundary.");
    FdoPtr<FdoArgumentDefinition> minXArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ResampleFunctionMinXArgumentName, 
                                                                          desc,
                                                                          FdoDataType_Double);
    args->Add(minXArg);

    desc = NlsMsgGet(FDOWMS_RESAMPLE_MINY_ARGUMENT_DESC,
                     "The Y coordinate corresponding to the lower left corner of the RESAMPLE boundary.");
    FdoPtr<FdoArgumentDefinition> minYArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ResampleFunctionMinYArgumentName, 
                                                                          desc, 
                                                                          FdoDataType_Double);
    args->Add(minYArg);

    desc = NlsMsgGet(FDOWMS_RESAMPLE_MAXX_ARGUMENT_DESC,
                     "The X coordinate corresponding to the upper right corner of the RESAMPLE boundary.");
    FdoPtr<FdoArgumentDefinition> maxXArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ResampleFunctionMaxXArgumentName, 
                                                                          desc, 
                                                                          FdoDataType_Double);
    args->Add(maxXArg);

    desc = NlsMsgGet(FDOWMS_RESAMPLE_MAXY_ARGUMENT_DESC,
                     "The Y coordinate corresponding to the upper right corner of the RESAMPLE boundary."); 
    FdoPtr<FdoArgumentDefinition> maxYArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ResampleFunctionMaxYArgumentName, 
                                                                          desc, 
                                                                          FdoDataType_Double);
    args->Add(maxYArg);

    desc = NlsMsgGet(FDOWMS_RESAMPLE_HEIGHT_ARGUMENT_DESC, "The HEIGHT of the output raster image.");
	FdoPtr<FdoArgumentDefinition> heightArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ResampleFunctionHeightArgumentName,
		                                                                    desc,
		                                                                    FdoDataType_Int32);
	args->Add(heightArg);

    desc = NlsMsgGet(FDOWMS_RESAMPLE_WIDTH_ARGUMENT_DESC, "The WIDTH of the output raster image.");
	FdoPtr<FdoArgumentDefinition> widthArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ResampleFunctionWidthArgumentName,
		                                                                   desc,
		                                                                   FdoDataType_Int32);
	args->Add(widthArg);

    desc = NlsMsgGet(FDOWMS_RESAMPLE_FUNCTION_DESC, "The RESAMPLE function returns a raster with the specified extents and size.");
    FdoPtr<FdoSignatureDefinition> resampleSignatureDef = FdoSignatureDefinition::Create(FdoDataType_BLOB, args);
    FdoPtr<FdoSignatureDefinitionCollection> resampleSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    resampleSignatureDefCol->Add(resampleSignatureDef);
    FdoPtr<FdoFunctionDefinition> resampleFunction = FdoFunctionDefinition::Create(FdoWmsGlobals::ResampleFunctionName,
                                                                                   desc,
                                                                                   true,
                                                                                   resampleSignatureDefCol);
    ret->Add(resampleFunction);

	// CLIP function
	args = FdoArgumentDefinitionCollection::Create();
    desc = NlsMsgGet(FDOWMS_CLIP_RASTER_ARGUMENT_DESC, "An argument that identifies the raster property definition.");
	rasterArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ClipFunctionRasterArgumentName, desc, FdoDataType_BLOB);
	args->Add(rasterArg);

    desc = NlsMsgGet(FDOWMS_CLIP_MINX_ARGUMENT_DESC, "The X coordinate corresponding to the lower left corner of the CLIP boundary.");
	minXArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ClipFunctionMinXArgumentName, desc, FdoDataType_Double);
    args->Add(minXArg);

    desc = NlsMsgGet(FDOWMS_CLIP_MINY_ARGUMENT_DESC, "The Y coordinate corresponding to the lower left corner of the CLIP boundary.");
    minYArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ClipFunctionMinYArgumentName, desc, FdoDataType_Double);
    args->Add(minYArg);

    desc = NlsMsgGet(FDOWMS_CLIP_MAXX_ARGUMENT_DESC, "The X coordinate corresponding to the upper right corner of the CLIP boundary.");
    maxXArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ClipFunctionMaxXArgumentName, desc, FdoDataType_Double);
    args->Add(maxXArg);

    desc = NlsMsgGet(FDOWMS_CLIP_MAXY_ARGUMENT_DESC, "The Y coordinate corresponding to the upper right corner of the CLIP boundary.");
    maxYArg = FdoArgumentDefinition::Create(FdoWmsGlobals::ClipFunctionMaxYArgumentName, desc, FdoDataType_Double);
    args->Add(maxYArg);

    desc = NlsMsgGet(FDOWMS_CLIP_FUNCTION_DESC, "The CLIP function returns a raster with the specified extents.");
    FdoPtr<FdoSignatureDefinition> clipSignatureDef = FdoSignatureDefinition::Create(FdoDataType_BLOB, args);
    FdoPtr<FdoSignatureDefinitionCollection> clipSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    clipSignatureDefCol->Add(clipSignatureDef);
	FdoPtr<FdoFunctionDefinition> clipFunction = FdoFunctionDefinition::Create(FdoWmsGlobals::ClipFunctionName,
                                                                               desc,
                                                                               false,
                                                                               clipSignatureDefCol);
    ret->Add(clipFunction);

    return (FDO_SAFE_ADDREF (ret.p));
}
