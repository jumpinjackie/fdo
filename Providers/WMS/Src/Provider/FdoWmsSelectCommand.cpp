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
#include "FdoWmsSelectCommand.h"
#include "FdoWmsFeatureReader.h"
#include "FdoWmsGlobals.h"
#include "FdoWmsLayer.h"
#include "FdoWmsLayerCollection.h"
#include <WMS/Override/FdoWmsOvPhysicalSchemaMapping.h>

#include <malloc.h>

FdoWmsSelectCommand::FdoWmsSelectCommand (FdoWmsConnection* connection) :
    FdoWmsFeatureCommand<FdoISelect> (connection),
    mType (FdoLockType_None),
    mStrategy (FdoLockStrategy_All)
{
	mPropertiesToSelect = FdoIdentifierCollection::Create ();
}

/** Do not implement the copy constructor. **/
//FdoWmsSelectCommand::FdoWmsSelectCommand (const FdoWmsSelectCommand &right) { }

FdoWmsSelectCommand::~FdoWmsSelectCommand (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void FdoWmsSelectCommand::Dispose ()
{
    delete this;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* FdoWmsSelectCommand::GetPropertyNames ()
{
    if (mPropertiesToSelect == NULL)
        mPropertiesToSelect = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(mPropertiesToSelect.p));
}

/// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
/// <returns>Returns the lock type.</returns> 
FdoLockType FdoWmsSelectCommand::GetLockType ()
{
    return (mType);
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void FdoWmsSelectCommand::SetLockType (FdoLockType value)
{
	throw FdoSchemaException::Create(NlsMsgGet(FDOWMS_LOCKING_NOT_SUPPORTED, "Locking not supported."));
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy FdoWmsSelectCommand::GetLockStrategy ()
{
    return (mStrategy);
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void FdoWmsSelectCommand::SetLockStrategy (FdoLockStrategy value)
{
	throw FdoSchemaException::Create(NlsMsgGet(FDOWMS_LOCKING_NOT_SUPPORTED, "Locking not supported."));
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* FdoWmsSelectCommand::Execute ()
{
	VALIDATE_ARGUMENT (mClassName);

	FdoWmsDelegateP wmsDelegate = _getWmsDelegate ();

	FdoStringP schemaName = mClassName->GetSchemaName ();
	FdoStringP clsName = mClassName->GetName ();	
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
		featClass = static_cast<FdoFeatureClass *> (featClasses->GetItem (clsName));
	}
	else
	{
		FdoIDisposableCollection* featClasses = schemas->FindClass (clsName);	
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
	FdoStringsP styleNames = FdoStringCollection::Create ();
    FdoPtr<FdoWmsLayerCollection> selectedLayers = FdoWmsLayerCollection::Create ();
	FdoStringP spatialContextName;
	FdoStringP imageFormat;
	FdoBoolean bTransparent = false;
	FdoStringP bgColor = L"0xFFFFFF";
	FdoStringP timeDimension;
	FdoStringP elevation;

	// Retrieve the layer name that corresponds to the FDO clas name
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
		for (FdoInt32 i=0; i<cntMappings; i++)
		{
			FdoPtr<FdoPhysicalSchemaMapping> schemaMapping = schemaMappings->GetItem (i);
			if (schemaName.GetLength () > 0)
			{
				if (wcscmp (schemaMapping->GetName (), schemaName) != 0)
					continue;
			}

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
				for (FdoInt32 k=0; k<layers->GetCount (); k++)
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

					FdoWmsOvStyleDefinitionP style = layer->GetStyle ();
					if (style != NULL)
						styleNames->Add (style->GetName ());
				}

				if (layerNames->GetCount () == 0)
					throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_PHYSICAL_CLASS_DEFINITION_HAS_NO_LAYERS, 
							"No WMS layers are found in physical class definition '%1$ls'.",
							(FdoString*)clsName));

				// Get the spatial context used for the layers
				spatialContextName = rasterDefinition->GetSpatialContextName ();

				// Get the prefered image format from the overrides
				imageFormat = _getImageFormat (rasterDefinition->GetFormatType ());

				// Get the "Transparent" value
				bTransparent = rasterDefinition->GetTransparent ();

				// Get the "BackgroundColor" value
				bgColor = rasterDefinition->GetBackgroundColor ();

				// Get the "Time" value
				timeDimension = rasterDefinition->GetTimeDimension ();

				// Get the "Elevation" value
				elevation = rasterDefinition->GetElevationDimension ();
			}			
		}		
	}
	
	FdoWmsServiceMetadataP metadata = mConnection->GetWmsServiceMetadata ();
	FdoPtr<FdoWmsCapabilities> capa = dynamic_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
	
	FdoWmsLayerCollectionP layers = capa->GetLayers ();
	FdoWmsLayerP layer;
	FdoStringP layerName;
	
	if (!mConnection->IsConfigured ())
	{
		if (layers->GetCount () > 0)
		{
			// If configuration file is not used, the first item in the layer names
			// must be the layer name the user requested.
			layerName = layerNames->GetString(0);
			layer = mConnection->FindLayer (layers, layerName);

			if (layer == NULL)
			{
				throw FdoSchemaException::Create (NlsMsgGet (FDOWMS_12001_LAYER_NOT_EXIST, "The WMS layer '%1$ls' does not exist.", (FdoString*)layerName));
			}
            selectedLayers->Add (layer);
		}
	}

	// Check whether the spatial context is supported by the layer.
	// If the user has not set the spatial context by calling ActivateSpatialContext command,
	// the active spatial context should be NULL. In this case, the first spatial context in
	// the list of the spatial contexts associated to the WMS layer will be used as default.
	FdoStringP srsName = mConnection->GetActiveSpatialContext ();
	FdoPtr<FdoWmsBoundingBox> bbox = FdoWmsBoundingBox::Create ();
	
	// HEIGHT and WIDTH paramater in the GetMap request
	FdoSize height = 0;
	FdoSize width = 0; 

    if (spatialContextName.GetLength () > 0)
        srsName = spatialContextName;
	
	if (srsName != NULL)
	{
		for (FdoInt32 i=0; i<layerNames->GetCount(); i++)
		{
			FdoString* layerName = layerNames->GetString (i);
			FdoWmsLayerP layer = mConnection->FindLayer (layers, layerName);
            if (layer == NULL)
            {
                throw FdoCommandException::Create (
                    NlsMsgGet(FDOWMS_12001_LAYER_NOT_EXIST, "The WMS layer '%1$ls' does not exist.", layerName));
            }

            selectedLayers->Add (layer);

			bool crsSupported = layer->FindCoordinateReferenceSystem (srsName);		
			if (crsSupported) 
			{
				// associate the spatial context to the raster property of the class
				_setSpatialContextAssociation (featClass, srsName);
				bbox->SetCRS (srsName);
			}
			else 
			{
				throw FdoException::Create (
					NlsMsgGet (FDOWMS_12003_SPATIALCONTEXT_NOT_SUPPORTED_BY_LAYER, 
					"The spatial context '%1$ls' is not supported by the layer '%2$ls'.",
					(FdoString*)srsName,
					(FdoString*)layerName));
			}
		}
	}
	else
	{
		// get the default spatial context associated with the class
		srsName = _getSpatialContextAssociation (featClass);
		bbox->SetCRS (srsName);
	}

	if (!mConnection->IsConfigured ())
	{
		imageFormat = _getDefaultImageFormat ();
	}

    // Calculate the bounding box of the layer(s)
    _calcLayersBoundingBox (selectedLayers, srsName, bbox);

	// Analysis the properties
	FdoStringP computedProperty;
	_analysisIdentifier (srsName, bbox, computedProperty, height, width);
	
	// Get the selected properties
	FdoPtr<FdoClassDefinition> classDefPruned = _getPrunedClassDefinition ();

	// Verify the styles and layers have the same size
	if (styleNames->GetCount () > 0 && styleNames->GetCount () != layerNames->GetCount ())
			throw FdoException::Create (NlsMsgGet (FDOWMS_12004_STYLES_LAYERS_NOT_CORRESPONDING, "The WMS styles and layers are not corresponding."));

	// Get the WMS version
	FdoStringP wmsVersion = metadata->GetVersion ();
    // Retrieve the raster stream through the WMS GetMap Request
	FdoPtr<FdoIoStream> stream = wmsDelegate->GetMap (layerNames, styleNames, bbox, imageFormat, height, width, bTransparent, bgColor, timeDimension, elevation, wmsVersion);

	// Create a FeatureReader on the stream and return it to the user
    FdoPtr<FdoWmsFeatureReader> ret;
	FdoWmsRect rect(bbox->GetMinX(), bbox->GetMinY(), bbox->GetMaxX(), bbox->GetMaxY());
    ret = new FdoWmsFeatureReader (stream, rect, featClass, classDefPruned);

    return (FDO_SAFE_ADDREF (ret.p));
}

/// <summary>Executes the select command and returns a reference to an
/// IFeatureReader.</summary> 
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* FdoWmsSelectCommand::ExecuteWithLock ()
{
    FdoPtr<FdoWmsFeatureReader> ret;

    ret = new FdoWmsFeatureReader ();

    return (FDO_SAFE_ADDREF (ret.p));
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* FdoWmsSelectCommand::GetLockConflicts ()
{
    throw FdoCommandException::Create (NlsMsgGet (FDOWMS_LOCKING_NOT_SUPPORTED, "Locking not supported."));
}

FdoClassDefinition* FdoWmsSelectCommand::_getPrunedClassDefinition ()
{
	// Get the original class definition
	FdoPtr<FdoFeatureClass> clsDef;
	FdoStringP clsName;
	FdoStringP schemaName;

	FdoPtr<FdoIdentifier> identity = this->GetFeatureClassName ();
	clsName = identity->GetName ();
	schemaName = identity->GetSchemaName ();

	FdoFeatureSchemasP schemas = mConnection->GetSchemas ();

	if (schemaName == L"")
	{		
		FdoPtr<FdoIDisposableCollection> clsColl = schemas->FindClass (clsName);
		if (clsColl->GetCount () == 0)
			throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_NAMED_SCHEMACLASS_NOT_FOUND, "FDO Feature Class '%1$ls' was not found.", (FdoString*)clsName ));
		
		clsDef = dynamic_cast<FdoFeatureClass *> (clsColl->GetItem(0));
	}
	else
	{
		FdoPtr<FdoFeatureSchema> schema = schemas->FindItem (schemaName);
		if (schema == NULL)
			throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_NAMED_SCHEMA_NOT_FOUND, "FDO Schema '%1$ls' was not found.", (FdoString*)schemaName));

		FdoPtr<FdoClassCollection> clsColl = schema->GetClasses ();
		clsDef = dynamic_cast<FdoFeatureClass *> (clsColl->FindItem (clsName));
	}

	if (mPropertiesToSelect && mPropertiesToSelect->GetCount () == 0)
	{
		return FDO_SAFE_ADDREF (clsDef.p);
	}

	FdoPtr<FdoClassDefinition> ret = FdoCommonSchemaUtil::DeepCopyFdoFeatureClass (clsDef);
	
	FdoPtr<FdoPropertyDefinition> prop;
	FdoPtr<FdoPropertyDefinitionCollection> properties = ret->GetProperties ();
	FdoPtr<FdoDataPropertyDefinitionCollection> ids = ret->GetIdentityProperties();

	// find the raster property first in the properties
	FdoPtr<FdoRasterPropertyDefinition> rasterProp;
	bool bRaster = false;
	for (FdoInt32 i=0; i<properties->GetCount(); i++)
	{
		FdoPtr<FdoPropertyDefinition> prop = properties->GetItem (i);
		if (prop->GetPropertyType () == FdoPropertyType_RasterProperty)
		{
			// we found that raster property
			rasterProp = static_cast<FdoRasterPropertyDefinition *> (FDO_SAFE_ADDREF (prop.p));
			bRaster = true;
			break;
		}
	}

	// if the raster property is not in properties, search it in the base properties 
	if (!bRaster)
	{
		FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = ret->GetBaseProperties ();
		for (FdoInt32 i=0; i<baseProps->GetCount(); i++)
		{
			FdoPtr<FdoPropertyDefinition> prop = baseProps->GetItem (i);
			if (prop->GetPropertyType () == FdoPropertyType_RasterProperty)
			{
				// we found that raster property
				rasterProp = static_cast<FdoRasterPropertyDefinition *> (FDO_SAFE_ADDREF (prop.p));
				bRaster = true;
				break;
			}
		}
	}

	// prune off the properties the caller hasn't provided in collection
	for (int j = properties->GetCount()-1; j >= 0; j--)
	{
		prop = properties->GetItem(j);
		bool bFound = false;

		for (int i = 0; i < mPropertiesToSelect->GetCount (); i++)
		{
			FdoPtr<FdoIdentifier> propValName = mPropertiesToSelect->GetItem (i);
			FdoString *propValNameString = propValName->GetName();

			if (0==wcscmp(prop->GetName(), propValNameString))
			{
				bFound = true; 
				break;
			}
		}

		// Remove this property, if not in propsToSelect
		if (!bFound)
		{
			// Remove from identity property collection, if necessary:
			if (prop->GetPropertyType() == FdoPropertyType_DataProperty)
			{
				FdoDataPropertyDefinition *dataProperty = static_cast<FdoDataPropertyDefinition*>(prop.p);
				if (ids->Contains(dataProperty))
					ids->Remove(dataProperty);
			}

			// Remove from main property collection:
			properties->Remove(prop);
		}
	}

	// Add the computed properties
	for (FdoInt32 i = 0; i < mPropertiesToSelect->GetCount (); i++)
	{
		FdoPtr<FdoIdentifier> identifier = mPropertiesToSelect->GetItem (i);
		FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier*> (identifier.p);
		if (computedIdentifier)
		{
			FdoString* rasterPropName = computedIdentifier->GetName ();
			FdoPtr<FdoPropertyDefinition> prop = FdoCommonSchemaUtil::DeepCopyFdoPropertyDefinition (rasterProp);
			prop->SetName (rasterPropName);
			properties->Add (prop);
			ret->SetIsComputed (true);
		}
	}

	return FDO_SAFE_ADDREF (ret.p);
}

void FdoWmsSelectCommand::_setSpatialContextAssociation (FdoFeatureClass* featClass, FdoString* spatialContext)
{
	// firstly we search the raster property in the BaseProperties
	FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = featClass->GetBaseProperties ();
	for (FdoInt32 i=0; i<baseProps->GetCount (); i++)
	{
		FdoPtr<FdoPropertyDefinition> baseProp = baseProps->GetItem (i);
		FdoRasterPropertyDefinition* rasterProp = dynamic_cast<FdoRasterPropertyDefinition *> (baseProp.p);
		if (rasterProp != NULL)
		{
			rasterProp->SetSpatialContextAssociation (spatialContext);
			return;
		}
	}

	FdoPtr<FdoPropertyDefinitionCollection> props = featClass->GetProperties ();
	for (FdoInt32 i=0; i<props->GetCount (); i++)
	{
		FdoPtr<FdoPropertyDefinition> prop = props->GetItem (i);
		FdoRasterPropertyDefinition* rasterProp = dynamic_cast<FdoRasterPropertyDefinition *> (prop.p);
		if (rasterProp != NULL)
		{
			rasterProp->SetSpatialContextAssociation (spatialContext);
			return;
		}
	}
}

FdoStringP FdoWmsSelectCommand::_getSpatialContextAssociation (FdoFeatureClass* featClass)
{
	FdoStringP spatialContext;	

	// firstly we search the raster property in the BaseProperties
	FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = featClass->GetBaseProperties ();
	for (FdoInt32 i=0; i<baseProps->GetCount (); i++)
	{
		FdoPtr<FdoPropertyDefinition> baseProp = baseProps->GetItem (i);
		FdoRasterPropertyDefinition* rasterProp = dynamic_cast<FdoRasterPropertyDefinition *> (baseProp.p);
		if (rasterProp != NULL)
		{
			spatialContext = rasterProp->GetSpatialContextAssociation ();
			if (spatialContext == NULL)
			{
				throw FdoSchemaException::Create (NlsMsgGet(
					FDOWMS_RASTERPROPERTY_HAS_NO_SPATIALCONTEXTASSOCIATION, 
					"Raster property has no spatial context association."));
			}

			return spatialContext;
		}
	}

	FdoPtr<FdoPropertyDefinitionCollection> props = featClass->GetProperties ();
	for (FdoInt32 i=0; i<props->GetCount (); i++)
	{
		FdoPtr<FdoPropertyDefinition> prop = props->GetItem (i);
		FdoRasterPropertyDefinition* rasterProp = dynamic_cast<FdoRasterPropertyDefinition *> (prop.p);
		if (rasterProp != NULL)
		{
			spatialContext = rasterProp->GetSpatialContextAssociation ();
			if (spatialContext == NULL)
			{
				throw FdoSchemaException::Create (NlsMsgGet(
					FDOWMS_RASTERPROPERTY_HAS_NO_SPATIALCONTEXTASSOCIATION, 
					"Raster property has no spatial context association."));
			}

			return spatialContext;
		}
	}

	return spatialContext;	
}

FdoStringP FdoWmsSelectCommand::_getImageFormat (FdoWmsOvFormatType formatType)
{
	switch (formatType)
	{
	case FdoWmsOvFormatType_Png:
		return FdoWmsGlobals::RasterMIMEFormat_PNG;
	case FdoWmsOvFormatType_Tif:
		return FdoWmsGlobals::RasterMIMEFormat_TIFF;
	case FdoWmsOvFormatType_Jpg:
		return FdoWmsGlobals::RasterMIMEFormat_JPEG;
	case FdoWmsOvFormatType_Gif:
		return FdoWmsGlobals::RasterMIMEFormat_GIF;	
	}

	return FdoWmsGlobals::RasterMIMEFormat_PNG;
}

FdoStringP FdoWmsSelectCommand::_getDefaultImageFormat ()
{
	FdoStringP imageFormat;

	FdoWmsServiceMetadataP metadata = mConnection->GetWmsServiceMetadata ();
	FdoPtr<FdoWmsCapabilities> capa = dynamic_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
	FdoStringsP imageFormats = capa->GetMIMETypes ();

	// Find the most suitable image format which the server supports. If the user doesn't use
	// configuration file, the image format will be determined as following:
	//	"PNG" is prefered is it's supported; Otherwise "TIFF"; Otherwise "JPEG"; Otherwise "GIF"	    

	if (imageFormats->IndexOf (FdoWmsGlobals::RasterMIMEFormat_PNG) != -1)
		imageFormat = FdoWmsGlobals::RasterMIMEFormat_PNG;
	else if (imageFormats->IndexOf (FdoWmsGlobals::RasterMIMEFormat_TIFF) != -1)
		imageFormat = FdoWmsGlobals::RasterMIMEFormat_TIFF;
	else if (imageFormats->IndexOf (FdoWmsGlobals::RasterMIMEFormat_JPEG) != -1)
		imageFormat = FdoWmsGlobals::RasterMIMEFormat_JPEG;
	else if (imageFormats->IndexOf (FdoWmsGlobals::RasterMIMEFormat_GIF) != -1)
		imageFormat = FdoWmsGlobals::RasterMIMEFormat_GIF;

	return imageFormat;
}

// Analysis the properties to check whether the RESAMPLE or CLIP function is invoked.
void FdoWmsSelectCommand::_analysisIdentifier (FdoString* srsName, FdoWmsBoundingBox* bbox, FdoStringP& propertyName, FdoSize& height, FdoSize& width)
{
	bool bResample = false;
	bool bClip = false;

	FdoPtr<FdoIdentifierCollection> identifiers = GetPropertyNames ();

	for (FdoInt32 i=0; i<identifiers->GetCount (); i++)
	{
		FdoPtr<FdoIdentifier> identifier = identifiers->GetItem (i);
		FdoStringP propName = identifier->GetName ();
		FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier*> (identifier.p);

		if (computedIdentifier != NULL)
		{
			FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
			FdoFunction* func = dynamic_cast<FdoFunction*> (expr.p);

			FdoWmsRect clippingBounds(0, 0, 0, 0);

			// CLIP() function can be nested
			while (true)
			{
				FdoString* funcName = func->GetName ();
				if (wcscmp (funcName, FdoWmsGlobals::ResampleFunctionName) == 0)
					bResample = true;
				else if (wcscmp (funcName, FdoWmsGlobals::ClipFunctionName) == 0)
					bClip = true;
				else
					throw FdoCommandException::Create (NlsMsgGet (FDOWMS_ONLY_RESAMPLE_CLIP_FUNCTION_SUPPORTED, "Only RESAMPLE() and CLIP() function are supported."));

				FdoPtr<FdoExpressionCollection> args = func->GetArguments ();
				if (bResample && args->GetCount () != 7)
					throw FdoCommandException::Create (NlsMsgGet (FDOWMS_RESAMPLE_FUNCTION_INCORRECTLY_USED, "RESAMPLE() function is incorrectly used."));
				if (bClip && args->GetCount() != 5)
					throw FdoCommandException::Create (NlsMsgGet (FDOWMS_CLIP_FUNCTION_INCORRECTLY_USED, "CLIP() function is incorrectly used."));

				// Check the arguments types.
				FdoPtr<FdoExpression> arg0 = args->GetItem(0);

				FdoPtr<FdoExpression> arg = args->GetItem(1);
				FdoDataValue* minX = dynamic_cast<FdoDataValue*> (arg.p);
				arg = args->GetItem(2);
				FdoDataValue* minY = dynamic_cast<FdoDataValue*> (arg.p);
				arg = args->GetItem(3);
				FdoDataValue* maxX = dynamic_cast<FdoDataValue*> (arg.p);
				arg = args->GetItem(4);
				FdoDataValue* maxY = dynamic_cast<FdoDataValue*> (arg.p);

				FdoDataValue* heightArg = NULL;
				FdoDataValue* widthArg = NULL;

				if (bResample)
				{
					arg = args->GetItem(5);
					heightArg = dynamic_cast<FdoDataValue*> (arg.p);
					arg = args->GetItem(6);
					widthArg = dynamic_cast<FdoDataValue*> (arg.p);
					if (minX == NULL || minY == NULL || maxX == NULL || maxY == NULL ||
						heightArg == NULL || widthArg == NULL ||
						(minX->GetDataType() != FdoDataType_Double && 
						minX->GetDataType() != FdoDataType_Int16 &&
						minX->GetDataType() != FdoDataType_Int32 && 
						minX->GetDataType() != FdoDataType_Int64) ||
						(minY->GetDataType() != FdoDataType_Double && 
						minY->GetDataType() != FdoDataType_Int16 &&
						minY->GetDataType() != FdoDataType_Int32 && 
						minY->GetDataType() != FdoDataType_Int64)  ||
						(maxX->GetDataType() != FdoDataType_Double && 
						maxX->GetDataType() != FdoDataType_Int16 &&
						maxX->GetDataType() != FdoDataType_Int32 && 
						maxX->GetDataType() != FdoDataType_Int64)  ||
						(maxY->GetDataType() != FdoDataType_Double && 
						maxY->GetDataType() != FdoDataType_Int16 &&
						maxY->GetDataType() != FdoDataType_Int32 && 
						maxY->GetDataType() != FdoDataType_Int64) ||
						(heightArg->GetDataType() != FdoDataType_Double && 
						heightArg->GetDataType() != FdoDataType_Int16 &&
						heightArg->GetDataType() != FdoDataType_Int32 && 
						heightArg->GetDataType() != FdoDataType_Int64) ||
						(widthArg->GetDataType() != FdoDataType_Double && 
						widthArg->GetDataType() != FdoDataType_Int16 &&
						widthArg->GetDataType() != FdoDataType_Int32 && 
						widthArg->GetDataType() != FdoDataType_Int64) )
						throw FdoCommandException::Create(NlsMsgGet(FDOWMS_RESAMPLE_FUNCTION_INCORRECTLY_USED, "RESAMPLE() function is incorrectly used."));
				}
				else if (bClip)
				{
					if (minX == NULL || minY == NULL || maxX == NULL || maxY == NULL ||					
						(minX->GetDataType() != FdoDataType_Double && 
						minX->GetDataType() != FdoDataType_Int16 &&
						minX->GetDataType() != FdoDataType_Int32 && 
						minX->GetDataType() != FdoDataType_Int64) ||
						(minY->GetDataType() != FdoDataType_Double && 
						minY->GetDataType() != FdoDataType_Int16 &&
						minY->GetDataType() != FdoDataType_Int32 && 
						minY->GetDataType() != FdoDataType_Int64)  ||
						(maxX->GetDataType() != FdoDataType_Double && 
						maxX->GetDataType() != FdoDataType_Int16 &&
						maxX->GetDataType() != FdoDataType_Int32 && 
						maxX->GetDataType() != FdoDataType_Int64)  ||
						(maxY->GetDataType() != FdoDataType_Double && 
						maxY->GetDataType() != FdoDataType_Int16 &&
						maxY->GetDataType() != FdoDataType_Int32 && 
						maxY->GetDataType() != FdoDataType_Int64) )
						throw FdoCommandException::Create(NlsMsgGet(FDOWMS_CLIP_FUNCTION_INCORRECTLY_USED, "CLIP() function is incorrectly used."));
				}

				FdoWmsRect bounds(atof ((const char*)FdoStringP(minX->ToString ())),
					atof ((const char*)FdoStringP(minY->ToString ())),
					atof ((const char*)FdoStringP(maxX->ToString ())),
					atof ((const char*)FdoStringP(maxY->ToString ())));

				if (clippingBounds.IsValid())
					clippingBounds = clippingBounds.Intersect (bounds);
				else
					clippingBounds = bounds;

				if (bResample)
				{
					height = atoi ((const char*)FdoStringP(heightArg->ToString ()));
					width = atoi ((const char*)FdoStringP(widthArg->ToString ()));
				}
				else
				{
					height = width = 0;
				}

				// try to find the next nested function
				// first try function
				func = dynamic_cast<FdoFunction *> (arg0.p);
				if (func == NULL)
				{
					// then try computed property
					FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier *> (arg0.p);
					if (computedIdentifier != NULL)
					{
						FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
						func = dynamic_cast<FdoFunction *> (expr.p);
					}
				}

				// Now arg0 must be an identifier, otherwise there must be somthing wrong with user's input
				if (func == NULL)
				{
					FdoIdentifier* id = dynamic_cast<FdoIdentifier *> (arg0.p);
					if (id == NULL)
					{
						if (bResample)
							throw FdoCommandException::Create(NlsMsgGet(FDOWMS_RESAMPLE_FUNCTION_INCORRECTLY_USED, "RESAMPLE() function is incorrectly used."));
						else if (bClip)
							throw FdoCommandException::Create(NlsMsgGet(FDOWMS_CLIP_FUNCTION_INCORRECTLY_USED, "CLIP() function is incorrectly used."));
					}
					propertyName = id->GetText ();
					break;
				}
			}

			bbox->SetMinX (clippingBounds.m_minX);
			bbox->SetMinY (clippingBounds.m_minY);
			bbox->SetMaxX (clippingBounds.m_maxX);
			bbox->SetMaxY (clippingBounds.m_maxY);

			// since we've got the computed property, stop the loop here
			break;
		}
	}

	if (!bClip && !bResample)
	{
		// The user hasn't set the bbox by using CLIP/RESAMPLE function, 
		// so we can get the extent corresponding to the spatial context.
		//FdoWmsServiceMetadataP metadata = mConnection->GetWmsServiceMetadata ();
		//FdoPtr<FdoWmsCapabilities> capa = static_cast<FdoWmsCapabilities *> (metadata->GetCapabilities ());
		//FdoStringsP srsNames = capa->GetCRSNames ();
		//FdoWmsBoundingBoxCollectionP bboxes = capa->GetCRSExtents ();
		//FdoInt32 idx = srsNames->IndexOf (srsName);
		//if (idx != -1)
		//{
		//	FdoWmsBoundingBoxP extent = bboxes->GetItem (idx);
		//	bbox->SetMaxX (extent->GetMaxX ());
		//	bbox->SetMinX (extent->GetMinX ());
		//	bbox->SetMaxY (extent->GetMaxY ());
		//	bbox->SetMinY (extent->GetMinY ());
		//}

		// Get the default HEIGHT value and calculate the WIDTH value.
		height = _getDefaultImageHeight ();		
		width = (FdoSize)((bbox->GetMaxX() - bbox->GetMinX()) / (bbox->GetMaxY() - bbox->GetMinY()) * height);
	}
}

FdoWmsDelegate* FdoWmsSelectCommand::_getWmsDelegate ()
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

    return FdoWmsDelegate::Create(location, user, password);
}

// Get the default image height which is specified by the user in the connection string.
// if it's not set, use 600 pixels arbitarily as default height value.
FdoSize FdoWmsSelectCommand::_getDefaultImageHeight ()
{
	FdoPtr<FdoIConnectionInfo> info = mConnection->GetConnectionInfo ();
    FdoPtr<FdoIConnectionPropertyDictionary> dictionary = info->GetConnectionProperties ();
	FdoStringP defaultHeight = dictionary->GetProperty (FdoWmsGlobals::ConnectionPropertyDefaultImageHeight);

	return (defaultHeight == L"") ? 600 : (defaultHeight.ToLong ());
}

FdoString* FdoWmsSelectCommand::_getOriginalLayerName (FdoString* mangledLayerName)
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

void FdoWmsSelectCommand::_calcLayerBoundingBox (FdoWmsLayer* layer, FdoString* srsName, FdoWmsBoundingBox* bbox)
{   
    // First we need to check whether the SRS is supported by the layer.
    // If so, the extent for the SRS is saved to "bbox".
    // Because the SRS attribute can be inheritanced, so we need to check
    // the layer's parents when it's necessary.
    bool bFound = false;

    FdoPtr<FdoStringCollection> srsNames = layer->GetCoordinateReferenceSystems ();
    FdoInt32 idx = srsNames->IndexOf (srsName);
    if (idx != -1)
    {
        FdoPtr<FdoWmsBoundingBoxCollection> bboxes = layer->GetBoundingBoxes ();
        FdoWmsBoundingBoxP extent = bboxes->GetItem (idx);
        bbox->SetMaxX (extent->GetMaxX ());
        bbox->SetMinX (extent->GetMinX ());
        bbox->SetMaxY (extent->GetMaxY ());
        bbox->SetMinY (extent->GetMinY ());
        bFound = true;
    }
    // We need special care for EPSG:4326. Sometimes some layers support EPSG:4326,
    // however they don't provide the corresponding extents for EPSG:4326. Instead,
    // they provide "LatLongBoundingBox". In this case, we will make use of the 
    // "LatLongBoundingBox" and take it as same as "EPSG:4326". Although they are not
    // exactly the same.
    else if (wcscmp (srsName, L"EPSG:4326") == 0)
    {
        FdoPtr<FdoOwsGeographicBoundingBox> geoBBox = layer->GetGeographicBoundingBox ();
        if (geoBBox != NULL)
        {
            bbox->SetMinX (geoBBox->GetWestBoundLongitude ());
            bbox->SetMaxX (geoBBox->GetEastBoundLongitude ());
            bbox->SetMinY (geoBBox->GetSouthBoundLatitude ());
            bbox->SetMaxY (geoBBox->GetNorthBoundLatitude ());
            bFound = true;
        }
    }

    if (!bFound)
    {
        FdoPtr<FdoWmsLayer> parentLayer = layer->GetParent ();
        if (parentLayer != NULL)
            _calcLayerBoundingBox (parentLayer, srsName, bbox);
    }
}

void FdoWmsSelectCommand::_calcLayersBoundingBox (FdoWmsLayerCollection* layers, FdoString* srsName, FdoWmsBoundingBox* bbox)
{
    FdoInt32 cnt = layers->GetCount ();
    FdoPtr<FdoWmsBoundingBox> temp = FdoWmsBoundingBox::Create ();
    for (int i=0; i<cnt; i++)
    {
        FdoPtr<FdoWmsLayer> layer = layers->GetItem (i);
        _calcLayerBoundingBox (layer, srsName, temp);
        if (bbox->GetMaxX () == bbox->GetMinX () || 
            bbox->GetMaxY () == bbox->GetMinY () )
        {
            bbox->SetMaxX (temp->GetMaxX ());
            bbox->SetMinX (temp->GetMinX ());
            bbox->SetMaxY (temp->GetMaxY ());
            bbox->SetMinY (temp->GetMinY ());
        }
        else
        {
            if (bbox->GetMinX () > temp->GetMinX ())
                bbox->SetMinX (temp->GetMinX ());
            if (bbox->GetMaxX () < temp->GetMaxX ())
                bbox->SetMaxX (temp->GetMaxX ());
            if (bbox->GetMinY () > temp->GetMinY ())
                bbox->SetMinY (temp->GetMinY ());
            if (bbox->GetMaxY () < temp->GetMaxY ())
                bbox->SetMaxY (temp->GetMaxY ());
        }
    }
}
