/*
 * Copyright (C) 2004-2011  Autodesk, Inc.
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
#include "FdoWmsGetFeatureInfo.h"
#include "FdoWmsGlobals.h"
#include "FdoWmsXmlGlobals.h"
#include "FdoWmsRequestMetadata.h"
#include "FdoWmsDelegate.h"


FdoWmsGetFeatureInfo::FdoWmsGetFeatureInfo(FdoIConnection *connection) :
	FdoWmsCommand<FdoWmsIGetFeatureInfo> (connection), mFeatureCount(1)
{
    mHeight = 0;
    mWidth = 0;
    mBoundingBox = NULL;
}

FdoWmsGetFeatureInfo::~FdoWmsGetFeatureInfo(void)
{
}

/// <summary>Dispose this object.</summary>
/// <return>Returns nothing.</return> 
void FdoWmsGetFeatureInfo::Dispose ()
{
	delete this;
}

/// Execute the FdoWmsIGetFeatureInfo command and returns a 
/// FdoIoStream* pointer. It could be a text, image, video and any 
/// binary data stream, ect. The result depends on the specified output
/// format.
FdoIoStream* FdoWmsGetFeatureInfo::Execute()
{
    VALIDATE_ARGUMENT(mFeatureClass);
    VALIDATE_ARGUMENT(mPosition);

    FdoStringsP cachedLayerNames;
    FdoStringsP cachedStyleNames;
    FdoWmsBoundingBoxP cachedBBox;
    FdoStringP cachedImgFormat;
    FdoSize cachedHeight;
    FdoSize cachedWidth;
    FdoBoolean cachedTransparent;
    FdoStringP cachedBackgroundColor;
    FdoStringP cachedTimeDimension;
    FdoStringP cachedElevation;
    FdoStringP cachedExceptionFormat;
    
    // Get the cached GetMap parameters.
    mConnection->GetGetMapParametersCache(
        cachedLayerNames,
        cachedStyleNames,
        cachedBBox,
        cachedImgFormat,
        cachedHeight,
        cachedWidth,
        cachedTransparent,
        cachedBackgroundColor,
        cachedTimeDimension,
        cachedElevation,
        cachedExceptionFormat);

    // Verify the parameters are not NULL.
    VALIDATE_ARGUMENT(cachedLayerNames);
    VALIDATE_ARGUMENT(cachedStyleNames);
    VALIDATE_ARGUMENT(cachedBBox);

    // Verify the cached styles and layers have the same size
    if (cachedStyleNames->GetCount () > 0 && cachedStyleNames->GetCount () != cachedLayerNames->GetCount ())
        throw FdoException::Create (NlsMsgGet (FDOWMS_12004_STYLES_LAYERS_NOT_PAIRED, "The specified WMS layer styles and layers aren't one-to-one paired."));

	FdoStringP schemaName = mFeatureClass->GetSchemaName ();
	FdoStringP clsName = mFeatureClass->GetName ();	
	VALIDATE_ARGUMENT (clsName);

    // Get the class in the schemas
	FdoFeatureClassP featClass;
	FdoFeatureSchemasP schemas = mConnection->GetSchemas ();
	if (schemaName != NULL && schemaName.GetLength() > 0)
	{
		FdoFeatureSchemaP schema = schemas->GetItem (schemaName);
		if (schema == NULL)
			throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_NAMED_SCHEMA_NOT_FOUND, "FDO Schema '%1$ls' was not found.", (FdoString*)schemaName));

		FdoPtr<FdoClassCollection> featClasses = schema->GetClasses ();
		featClass = static_cast<FdoFeatureClass*> (featClasses->GetItem (clsName));
	}
	else
	{
		FdoPtr<FdoIDisposableCollection> featClasses = schemas->FindClass (clsName);	
		if (featClasses->GetCount () == 0)
			throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_NAMED_SCHEMACLASS_NOT_FOUND, "FDO Feature Class '%1$ls' was not found.", (FdoString*)clsName ));

		if (featClasses->GetCount () > 1)
			throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_NAMED_SCHEMACLASS_NOT_FOUND, "Duplicate FDO Feature Class '%1$ls' found in Schema.", (FdoString*)clsName ));

		featClass = static_cast<FdoFeatureClass *> (featClasses->GetItem (0));
	}
    
	if (featClass == NULL)
		throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_NAMED_SCHEMACLASS_NOT_FOUND, "FDO Feature Class '%1$ls' was not found.", (FdoString*)clsName));

	// Check whether the feat class is abstract or not.
	if (featClass->GetIsAbstract ())
		throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_CANNOT_QUERY_ABSTRACT_CLASS, "FDO Query cann't be performed on abstract class '%1$ls'.", (FdoString*)clsName ));

    FdoStringsP layerNames = FdoStringCollection::Create ();

    // Retrieve the layer names that corresponds to the FDO feature class
	if (!mConnection->IsConfigured ())
	{
		FdoDictionaryP namedLayerMappings = mConnection->GetNamedLayerMappings();
		FdoDictionaryElementP layerElement = namedLayerMappings->GetItem(clsName);
		if (layerElement)
		{
			layerNames->Add (layerElement->GetValue());
		}
		else
			throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_12001_LAYER_NOT_EXIST, "The WMS layer '%1$ls' does not exist.", (FdoString*)clsName));
	}
	else
	{
		FdoSchemaMappingsP schemaMappings = mConnection->GetSchemaMappings ();
		FdoInt32 cntMappings = schemaMappings->GetCount ();
		for (FdoInt32 i=0; i < cntMappings; ++i)
		{
			FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = schemaMappings->GetItem (i);
            if (wcscmp (schemaMapping->GetName (), schemaName) != 0 && schemaName.GetLength () > 0)
                continue;

			FdoWmsOvPhysicalSchemaMapping* physicalMapping = static_cast<FdoWmsOvPhysicalSchemaMapping *> (schemaMapping.p);
			if (physicalMapping == NULL)
				throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_SCHEMA_MAPPING_NOT_FOUND, "Physical schema mapping '%1$ls' was not found.", (FdoString*)schemaName));
			
			FdoWmsOvClassesP classMappings = physicalMapping->GetClasses ();
			FdoWmsOvClassDefinitionP clsDefinition = classMappings->GetItem (clsName);
			if (clsDefinition != NULL)
			{
				// Now we've got the feature class. Then we can get the layer names and styles.
				FdoWmsOvRasterDefinitionP rasterDefinition = clsDefinition->GetRasterDefinition ();
				FdoWmsOvLayersP layers = rasterDefinition->GetLayers ();
                FdoInt32 count = layers->GetCount ();
				for (FdoInt32 k = 0; k < count; ++k)
				{
					FdoWmsOvLayerDefinitionP layer = layers->GetItem (k);
					FdoStringP layerName = layer->GetName ();
					if (layerName != NULL && layerName.GetLength () > 0)
                    {
                        FdoStringP originalName = _getOriginalLayerName (layerName);
                        // if the layer is not found, the name used here can be
                        // qualified name of the feature class, such as "WMS_Schema:SomeLayer"
                        // or "WMS_Schema:Some Layer".
                        if (originalName.GetLength() == 0)
                        {
                            FdoStringP tempLayerName = layerName.Right (L":");
                            originalName = _getOriginalLayerName (tempLayerName);
                        }

                        if (originalName.GetLength () > 0)
                            layerNames->Add (originalName);
                        else
                        {
                            // The layer name must be wrong.
                            throw FdoCommandException::Create (
                                NlsMsgGet(FDOWMS_12001_LAYER_NOT_EXIST, "The WMS layer '%1$ls' does not exist.", (FdoString *)layerName));
                        }
                    }
					else
						throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_PHYSICAL_SCHEMA_LAYER_HAS_NO_NAME, 
								"A WMS layer has no name in physical shema mappings."));
				}

                // Skip out the loop.
                break;
			}			
		}		
	}

    if (layerNames->GetCount () == 0)
        throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_PHYSICAL_CLASS_DEFINITION_HAS_NO_LAYERS, 
        "No WMS layers are found in physical class definition '%1$ls'.",
        (FdoString*)clsName));

    // Update the cached bounding box if explicitly specified.
    if (mBoundingBox != NULL && !mBoundingBox->GetIsEmpty())
    {
        cachedBBox->SetMaxX(mBoundingBox->GetMaxX());
        cachedBBox->SetMaxY(mBoundingBox->GetMaxY());
        cachedBBox->SetMinX(mBoundingBox->GetMinX());
        cachedBBox->SetMinY(mBoundingBox->GetMinY());
    }

    // Update the cached height/width if explicitly specified.
    if (mHeight != 0 && mWidth != 0)
    {
        cachedHeight = mHeight;
        cachedWidth = mWidth;
    }

    // Get the default feature informat format if not set by user.
    if (mOutputFormat == NULL || mOutputFormat.GetLength() == 0)
        mOutputFormat = _getDefaultFeatureInfoFormat();

    // Get the WMS version.
    FdoWmsServiceMetadataP metadata = mConnection->GetWmsServiceMetadata ();
    FdoStringP wmsVersion = metadata->GetVersion ();

    // Retrieve the stream through the WMS GetFeatureInfo request
    FdoWmsDelegateP wmsDelegate = _getWmsDelegate ();
    FdoPtr<FdoIoStream> stream = wmsDelegate->GetFeatureInfo (
        cachedLayerNames,
        cachedStyleNames,
        cachedBBox,
        cachedImgFormat,
        cachedHeight,
        cachedWidth,
        cachedTransparent,
        cachedBackgroundColor,
        cachedTimeDimension,
        cachedElevation,
        wmsVersion,
        cachedExceptionFormat,
        layerNames,
        mOutputFormat,
        mPosition->GetX(),
        mPosition->GetY(), 
        mFeatureCount
        );

    return FDO_SAFE_ADDREF(stream.p);
}

void FdoWmsGetFeatureInfo::SetFeatureClassName(FdoIdentifier* value)
{
    mFeatureClass = FDO_SAFE_ADDREF(value);
}

/// <summary>Get the name of the feature class for which feature information 
/// will be retrieved.</summary>
/// <return>Input the query feature class name.</return>
FdoIdentifier* FdoWmsGetFeatureInfo::GetFeatureClassName()
{
    return FDO_SAFE_ADDREF(mFeatureClass.p);
}

/// <summary>Set the output feature information format. The supported values should
/// be listed in one or more <Format> elements inside the <Request><FeatureInfo>
/// element of the Capabilities XML.
/// An exception would be thrown if the value isn't in the supported list when
/// executing the command.</summary>
/// <return>Input the output feature information format.</return>
void FdoWmsGetFeatureInfo::SetOutputFormat(FdoString* value)
{
    mOutputFormat = value;
}

/// <summary>Get the format used when returning the feature information.</summary>
/// <return>Return the output feature information format.</return>
FdoString* FdoWmsGetFeatureInfo::GetOutputFormat()
{
    return mOutputFormat;
}

void FdoWmsGetFeatureInfo::SetBoundingBox(FdoIEnvelope* value)
{
    mBoundingBox = FDO_SAFE_ADDREF(value);
}

FdoIEnvelope* FdoWmsGetFeatureInfo::GetBoundingBox()
{
    return FDO_SAFE_ADDREF(mBoundingBox.p);
}

void FdoWmsGetFeatureInfo::SetHeight(FdoSize value)
{
    mHeight = value;
}

FdoSize FdoWmsGetFeatureInfo::GetHeight()
{
    return mHeight;
}

void FdoWmsGetFeatureInfo::SetWidth(FdoSize value)
{
    mWidth = value;
}

FdoSize FdoWmsGetFeatureInfo::GetWidth()
{
    return mWidth;
}

/// <summary>Set the position (X, Y) which indicates a point of interest on the map that was
/// produced by the embedded GetMap request. The point(X, Y) is a
/// point in the (i, j) space defined by the Map CS.
/// This function is mandatory; if not specified, 
/// execution of the command will throw an exception.</summary>
void FdoWmsGetFeatureInfo::SetPosition(FdoIDirectPosition* pos)
{
    mPosition = FDO_SAFE_ADDREF(pos);
}

/// <summary>Get the position (X, Y) which indicates a point of interest on the map that was
/// produced by the embedded !GetMap request. The point (X, Y) is a
/// point in the (i, j) space defined by the Map CS.</summary>
/// <return>Return the interesting position.</return>
FdoIDirectPosition* FdoWmsGetFeatureInfo::GetPosition()
{
    return FDO_SAFE_ADDREF(mPosition.p);
}

/// <summary>Set the number of features about which to return information (default=1).</summary>
void FdoWmsGetFeatureInfo::SetFeatureCount(FdoInt32 featureCount)
{
    mFeatureCount = featureCount;
}

/// <summary>Get the number of features about which to return information.</summary>
/// <return>Returns candidate feature number.</return>
FdoInt32 FdoWmsGetFeatureInfo::GetFeatureCount ()
{
    return mFeatureCount;
}

FdoWmsDelegate* FdoWmsGetFeatureInfo::_getWmsDelegate ()
{
	FdoPtr<FdoIConnectionInfo> info = mConnection->GetConnectionInfo ();
    FdoPtr<FdoIConnectionPropertyDictionary> dictionary = info->GetConnectionProperties ();

    FdoStringP location = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyFeatureServer);
    if (0 == location.GetLength()) 
	{
        throw FdoConnectionException::Create (NlsMsgGet(FDOWMS_CONNECTION_REQUIRED_PROPERTY_NULL, 
			"The required connection property '%1$ls' cannot be set to NULL.", 
			FdoWmsGlobals::ConnectionPropertyFeatureServer));
    }

    FdoStringP user = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyUsername);
    FdoStringP password = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyPassword);
    FdoStringP proxyHost = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyProxyServer);
    FdoStringP proxyPort = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyProxyPort);
    FdoStringP proxyUser = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyProxyUsername);
    FdoStringP proxyPassword = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyProxyPassword);

    FdoPtr<FdoWmsDelegate> ret = FdoWmsDelegate::Create(location, user, password, proxyHost, proxyPort, proxyUser, proxyPassword);
    FdoPtr<FdoWmsServiceMetadata> svcMetadata = mConnection->GetWmsServiceMetadata ();
    FdoPtr<FdoOwsCapabilities> capa = svcMetadata->GetCapabilities ();
    FdoPtr<FdoOwsRequestMetadataCollection> reqMetadatas = capa->GetRequestMetadatas ();
    ret->SetRequestMetadatas (reqMetadatas);

    return FDO_SAFE_ADDREF (ret.p);
}

FdoString* FdoWmsGetFeatureInfo::_getOriginalLayerName (FdoString* mangledLayerName)
{
    FdoPtr<FdoDictionary> nameMappings = mConnection->GetNamedLayerMappings ();
    if (nameMappings != NULL)
    {
        // 1. the layer name is mangled
        FdoPtr<FdoDictionaryElement> nameMapping = nameMappings->FindItem (mangledLayerName);
        if (nameMapping != NULL)
        {
            return nameMapping->GetValue ();
        }

        // 2. the input layer name may be the original one, so we should also verify it.
        FdoInt32 cnt = nameMappings->GetCount ();
        for (int i=0; i<cnt; i++)
        {
            FdoPtr<FdoDictionaryElement> nameMapping = nameMappings->GetItem (i);
            FdoStringP originalName = nameMapping->GetValue ();
            if (wcscmp (originalName, mangledLayerName) == 0)
                return mangledLayerName;
        }
    }
    // if not found, just return empty string.
    return L"";
}

// Get the first feature info format in the capabilities document as the default one.
FdoString* FdoWmsGetFeatureInfo::_getDefaultFeatureInfoFormat()
{
    FdoString* ret = NULL;

    FdoWmsServiceMetadataP metadata = mConnection->GetWmsServiceMetadata ();
    FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
    FdoPtr<FdoOwsRequestMetadataCollection> reqMetadatas = capa->GetRequestMetadatas ();

    FdoPtr<FdoOwsRequestMetadata> reqMetadata = reqMetadatas->FindItem (FdoWmsXmlGlobals::WmsGetFeatureInfoRequest);
    if (reqMetadata == NULL) {
        throw FdoCommandException::Create (NlsMsgGet (FDOWMS_12006_GETFEATUREINFO_NOT_SUPPORTED, "The WMS GetFeatInfo request is not supported."));
    }

    FdoWmsRequestMetadata* getFeatureInfoMetadata = static_cast<FdoWmsRequestMetadata*>(reqMetadata.p);
    FdoStringsP featInfoFormats = getFeatureInfoMetadata->GetFormats ();

    // Get the first format as the default one.
    if (featInfoFormats != NULL && featInfoFormats->GetCount() > 0)
        ret = featInfoFormats->GetString(0);
	
    return ret;
}
