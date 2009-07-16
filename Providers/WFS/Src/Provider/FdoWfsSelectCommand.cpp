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
#include "FdoWfsSelectCommand.h"
#include "FdoWfsFeatureReader.h"
#include "FdoWfsFeatureTypeList.h"
#include "FdoWfsFeatureType.h"
#include "FdoWfsServiceMetadata.h"
#include "FdoWfsDelegate.h"
#include <FdoExpressionEngine.h>
#include <Util/FdoExpressionEngineUtilDataReader.h>
#include <Util/FdoExpressionEngineUtilFeatureReader.h>

#include <malloc.h>

FdoWfsSelectCommand::FdoWfsSelectCommand (FdoWfsConnection* connection) :
    FdoCommonFeatureCommand<FdoISelect, FdoWfsConnection> (connection),
    mType (FdoLockType_None),
    mStrategy (FdoLockStrategy_All)
{
}

/** Do not implement the copy constructor. **/
//FdoWfsSelectCommand::FdoWfsSelectCommand (const FdoWfsSelectCommand &right) { }

FdoWfsSelectCommand::~FdoWfsSelectCommand (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void FdoWfsSelectCommand::Dispose ()
{
    delete this;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* FdoWfsSelectCommand::GetPropertyNames ()
{
    if (mPropertiesToSelect == NULL)
        mPropertiesToSelect = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(mPropertiesToSelect.p));
}

/// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
/// <returns>Returns the lock type.</returns> 
FdoLockType FdoWfsSelectCommand::GetLockType ()
{
    return (mType);
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsSelectCommand::SetLockType (FdoLockType value)
{
    mType = value;
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy FdoWfsSelectCommand::GetLockStrategy ()
{
    return (mStrategy);
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsSelectCommand::SetLockStrategy (FdoLockStrategy value)
{
    mStrategy = value;
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* FdoWfsSelectCommand::Execute ()
{
	if (mClassName == NULL)
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(WFS_SELECT_INVALID_NAME_PARAMETER)));

	// first we must find out the proper WFS feature type according to the FDO class name
	// that is , FDO class name --> GML global element name
	// and that global element name actually represents the valid WFS feature type
	// that is recognized by WFS servers.
	FdoString* schemaFeatureName = NULL;
	FdoString* featureTypeName = NULL;
    FdoStringP featureTypeNameCheck;
	FdoString* targetNamespace = L"";
	FdoPtr<FdoFeatureSchemaCollection> schemas = mConnection->GetSchemas();
	FdoPtr<FdoPhysicalSchemaMappingCollection> mappings = schemas->GetXmlSchemaMappings();
    FdoPtr<FdoXmlClassMapping> elementClass;

	if (schemas->GetCount() > 0 && mappings != NULL) {
        FdoInt32 count = schemas->GetCount();
        for (int iSchema = count - 1; iSchema >= 0; iSchema--) {
		    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(iSchema);
		    FdoStringP schemaName = mClassName->GetSchemaName();
		    if (schemaName.GetLength() == 0 || schemaName == schema->GetName()) {
			    FdoPtr<FdoXmlSchemaMapping> mapping = static_cast<FdoXmlSchemaMapping*>(mappings->GetItem(FdoXml::mGmlProviderName, schema->GetName()));
			    if (mapping != NULL) {
				    FdoPtr<FdoXmlElementMappingCollection> elementMappings = mapping->GetElementMappings();
				    // iterate to find out the element whose FDO class name matches the given name
				    FdoInt32 count = elementMappings->GetCount();
				    for (int i = 0; i < count; i++) {
					    FdoPtr<FdoXmlElementMapping> elementMapping = elementMappings->GetItem(i);                        
					    if (wcscmp(elementMapping->GetClassName(), mClassName->GetName()) == 0) {
						    // we found it
                            elementClass = elementMapping->GetClassMapping();
						    featureTypeName = elementMapping->GetName();
                            schemaFeatureName = schema->GetName();
						    targetNamespace = mapping->GetTargetNamespace();
						    break;
					    }
				    }
			    }
		    }
            if (featureTypeName != NULL)
            {                
                featureTypeNameCheck = featureTypeName;
                if (featureTypeNameCheck.Contains(FdoWfsGlobals::Dot)) 
                {
                    featureTypeNameCheck = featureTypeNameCheck.Replace(FdoWfsGlobals::Dot, L".");                     
                    featureTypeName = featureTypeNameCheck;
                }
                break;
            }
        }
	}

	// validate the featureType by service metadata
	// and at the same time find out the srs name that this feature type uses
	FdoString* srsName = L"EPSG:4326";
	if (featureTypeName != NULL)
    {
        FdoPtr<FdoWfsServiceMetadata> metadata = mConnection->GetServiceMetadata();
        FdoPtr<FdoWfsFeatureType> featureType = metadata->GetFeatureType(mClassName);
        if (featureType != NULL)
            srsName = featureType->GetSRS();
	}

	if (featureTypeName == NULL || srsName == NULL) { // no match found
		throw FdoException::Create (NlsMsgGet (WFS_COMMAND_CLASS_NOT_RECOGNIZED, 
			"Feature class '%1$ls' is not recognized by WFS server.", mClassName->GetText()));
	}

	FdoPtr<FdoFeatureSchema> schema;
	FdoPtr<FdoClassCollection> clsdefs;
	FdoPtr<FdoClassDefinition> clsdef;
	FdoStringP schemaName = mClassName->GetSchemaName();		
	if (schemaName != NULL)
	{
		schema = schemas->GetItem(schemaName);
		clsdefs = schema->GetClasses();
		if (clsdefs == NULL)
		{
			throw FdoException::Create (NlsMsgGet (WFS_COMMAND_CLASS_NOT_RECOGNIZED, 
				"Feature class '%1$ls' is not recognized by WFS server.", mClassName->GetText()));
		}

		FdoStringP clsname = mClassName->GetName();
		clsdef = clsdefs->GetItem(clsname);
		if (clsdef == NULL)
		{
			throw FdoException::Create (NlsMsgGet (WFS_COMMAND_CLASS_NOT_RECOGNIZED, 
				"Feature class '%1$ls' is not recognized by WFS server.", mClassName->GetText()));
		}
	}
	else
	{
		FdoPtr<FdoIDisposableCollection> clscoll = schemas->FindClass(mClassName->GetName());
		if (clscoll != NULL && clscoll->GetCount() != 0)
		{
			clsdef = dynamic_cast<FdoClassDefinition *>(clscoll->GetItem(0));
		}
		else
		{
			throw FdoException::Create (NlsMsgGet (WFS_COMMAND_CLASS_NOT_RECOGNIZED, 
				"Feature class '%1$ls' is not recognized by WFS server.", mClassName->GetText()));
		}
	}
    FdoStringP sPropAlias;
    FdoStringP sPropName;
    if ((mFilter != NULL || mPropertiesToSelect != NULL) && elementClass != NULL)
    {
        // check if we have gml geometries
        FdoPtr<FdoXmlElementMappingCollection> elements = elementClass->GetElementMappings();
        for (int k = 0; k < elements->GetCount(); k++)
        {
            FdoPtr<FdoXmlElementMapping> element = elements->GetItem(k);
            FdoPtr<FdoStringCollection> pAliasNames = element->GetAliasNames();
            if (pAliasNames != NULL && pAliasNames->GetCount() != 0)
            {
                sPropName = element->GetName();
                sPropAlias = L"gml/";
                sPropAlias += pAliasNames->GetString(0);
                break;
            }
        }
    }

    bool bHasComputedProperties = false;

	FdoPtr<FdoStringCollection> props = FdoStringCollection::Create();
	if (mPropertiesToSelect != NULL) {
		FdoInt32 count = mPropertiesToSelect->GetCount();
		for (int i = 0; i < count; i++) {
			FdoPtr<FdoIdentifier> identifier = mPropertiesToSelect->GetItem(i);
            if ( dynamic_cast<FdoComputedIdentifier*>(identifier.p) == NULL ) {
			    FdoStringP propName = identifier->GetName();
			    if (propName.Contains(FdoWfsGlobals::Dot)) {
				    FdoStringP propName1 = propName.Replace(FdoWfsGlobals::Dot, L".");
				    props->Add(propName1);
			    } else {
				    props->Add(propName);
			    }
            }
            else {
                bHasComputedProperties = true;
            }
		}
	}
    if (sPropName.GetLength() != 0)
    {
        int poz = props->IndexOf(sPropName);
        if (poz != -1)
        {
            props->RemoveAt(poz);
            props->Add(sPropAlias);
        }
        if (mFilter != NULL)
            UpdateFilter(mFilter.p, sPropAlias, sPropName);
    }

	// yeah, all the parameters that WfsDeleget::GetFeature needs are ready
	FdoPtr<FdoWfsDelegate> delegate = mConnection->GetWfsDelegate();
	FdoPtr<FdoIFeatureReader> ret = delegate->GetFeature(
        schemas, 
        mappings, 
        targetNamespace, 
        srsName, 
        bHasComputedProperties ? (FdoStringCollection*) NULL : props, // Computed properties may reference other properties so retrieve all of them.
        featureTypeName, 
        mFilter, 
        schemaFeatureName
    );

	FdoWfsFeatureReader* reader = dynamic_cast<FdoWfsFeatureReader *>(ret.p);
	if (reader)
	{
		if (mPropertiesToSelect != NULL && 
			mPropertiesToSelect->GetCount() != 0)
		{
			reader->SetClassDefinition(clsdef.p, mPropertiesToSelect.p);
		}
		else
		{
			reader->SetClassDefinition(clsdef.p, NULL);
		}
	}

    if ( bHasComputedProperties ) {
        // WFS does not support computed properties in the select list so always need
        // to evaluate them using expression engine.

        //TODO: Use Expression Engine to handle filters that reference functions not supported
        //by the WFS. Need to parse out function list from WFS capabilities first.
        ret =  new FdoExpressionEngineUtilFeatureReader(
            clsdef,
            ret, 
            NULL,
            mPropertiesToSelect,
            NULL);
    }

    return (FDO_SAFE_ADDREF (ret.p));
}

/// <summary>Executes the select command and returns a reference to an
/// IFeatureReader.</summary> 
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* FdoWfsSelectCommand::ExecuteWithLock ()
{
    throw FdoException::Create (NlsMsgGet (WFS_LOCKING_NOT_SUPPORTED, "Locking not supported."));
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* FdoWfsSelectCommand::GetLockConflicts ()
{
    throw FdoException::Create (NlsMsgGet (WFS_LOCKING_NOT_SUPPORTED, "Locking not supported."));
}

void FdoWfsSelectCommand::UpdateFilter(FdoFilter* pFilter, FdoString* pPropAlias, FdoString* pPropName)
{
    if (pFilter == NULL)
        return;
    FdoBinaryLogicalOperator* pBin = dynamic_cast<FdoBinaryLogicalOperator*>(pFilter);
    if (pBin != NULL)
    {
        UpdateFilter(pBin->GetLeftOperand(), pPropAlias, pPropName);
        UpdateFilter(pBin->GetRightOperand(), pPropAlias, pPropName);
    }
    else
    {
        FdoUnaryLogicalOperator* pUnary = dynamic_cast<FdoUnaryLogicalOperator*>(pFilter);
        if (pUnary != NULL)
            UpdateFilter(pUnary->GetOperand(), pPropAlias, pPropName);
        else
        {
            FdoGeometricCondition* pGeom = dynamic_cast<FdoGeometricCondition*>(pFilter);
            if (pGeom != NULL)
            {
                FdoPtr<FdoIdentifier> pProp = pGeom->GetPropertyName();
                if(wcscmp(pProp->GetText(), pPropName) == 0)
                    pGeom->SetPropertyName(FdoIdentifier::Create(pPropAlias));
            }
            else
            {
                FdoNullCondition* pNull = dynamic_cast<FdoNullCondition*>(pFilter);
                if (pNull != NULL)
                {
                    FdoPtr<FdoIdentifier> pProp = pNull->GetPropertyName();
                    if(wcscmp(pProp->GetText(), pPropName) == 0)
                        pNull->SetPropertyName(FdoIdentifier::Create(pPropAlias));
                }
                else
                {
                    FdoInCondition* pIn = dynamic_cast<FdoInCondition*>(pFilter);
                    if (pIn != NULL)
                    {
                        FdoPtr<FdoIdentifier> pProp = pIn->GetPropertyName();
                        if(wcscmp(pProp->GetText(), pPropName) == 0)
                            pIn->SetPropertyName(FdoIdentifier::Create(pPropAlias));
                    }
                }
            }
        }
    }
}
