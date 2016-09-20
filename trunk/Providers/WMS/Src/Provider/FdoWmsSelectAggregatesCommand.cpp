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
 *         $Id: //providers/WMS/src/Provider/FdoWmsSelectAggregatesCommand.cpp#13 $
 *     $Author: gregb $
 *   $DateTime: 2006/03/18 03:03:51 $
 *     $Change: 11595 $
 *
 */

#include "stdafx.h"
#include "FdoWmsSelectAggregatesCommand.h"
#include "FdoWmsGlobals.h"
#include "FdoWmsEnvelope.h"
#include "FdoWmsLayer.h"
#include "FdoWmsLayerCollection.h"
#include "FdoWmsSpatialExtentsAggregateReader.h"
#include <OWS/FdoOwsGeographicBoundingBox.h>
#include <WMS/Override/FdoWmsOvPhysicalSchemaMapping.h>


#include <malloc.h>

FdoWmsSelectAggregatesCommand::FdoWmsSelectAggregatesCommand (FdoWmsConnection* connection) :
    FdoWmsFeatureCommand<FdoISelectAggregates> (connection)
{
	mPropertiesToSelect = FdoIdentifierCollection::Create ();
}

/** Do not implement the copy constructor. **/
//FdoWmsSelectAggregatesCommand::FdoWmsSelectAggregatesCommand (const FdoWmsSelectAggregatesCommand &right) { }

FdoWmsSelectAggregatesCommand::~FdoWmsSelectAggregatesCommand (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void FdoWmsSelectAggregatesCommand::Dispose ()
{
    delete this;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* FdoWmsSelectAggregatesCommand::GetPropertyNames ()
{
    if (mPropertiesToSelect == NULL)
        mPropertiesToSelect = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(mPropertiesToSelect.p));
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIDataReader* FdoWmsSelectAggregatesCommand::Execute ()
{
    if (mConnection->GetConnectionState() != FdoConnectionState_Open) {
        throw FdoCommandException::Create(
            NlsMsgGet(FDO_NLSID(FDOWMS_CONNECTION_CLOSED)));
    }

    FdoStringP className = mClassName->GetName();	
    if (className.GetLength() == 0) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_NAMED_SCHEMACLASS_NOT_FOUND), (FdoString*)className));
    }

    if (mPropertiesToSelect->GetCount()!=1) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_NO_QUERY_PROPERTIES_SELECTED), (FdoString *) className));
    }

    FdoPtr<FdoIdentifier> identifier = mPropertiesToSelect->GetItem(0);
    FdoString* identifierName = identifier->GetName();
    FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier*>(identifier.p);
    if (!computedIdentifier) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_QUERY_NON_COMPUTED_PROPERTIES), identifierName, (FdoString*)className));
    }

    FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
    FdoFunction* function = dynamic_cast<FdoFunction*>(expr.p);
    if (!function) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_QUERY_NON_COMPUTED_PROPERTIES), identifierName, (FdoString*)className));
    }

    if (wcscmp(function->GetName(), FDO_FUNCTION_SPATIALEXTENTS) != 0) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_QUERY_NON_FUNCTION_SPATIALEXTENTS), function->GetName()));
    }

    FdoPtr<FdoExpressionCollection> args = function->GetArguments();
    if (args->GetCount() != 1) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_SPATIALEXTENTS_TOO_MANY_ARGUMENTS)));
    }

    FdoPtr<FdoExpression> arg = args->GetItem(0);
    FdoIdentifier* argId = dynamic_cast<FdoIdentifier*>(arg.p);
    if (!argId) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_SELECTAGGREGATES_INVALID_ARGUMENT)));
    }

    FdoPtr<FdoFilter> filter = GetFilter();
    if (filter) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_SELECTAGGREGATES_FILTER_NOT_SUPPORTED)));
    }

    FdoFeatureSchemasP schemas = mConnection->GetSchemas();
    FdoStringP schemaName = mClassName->GetSchemaName();
    FdoClassDefinitionP classDef;

    if (schemaName.GetLength() > 0) {
        FdoFeatureSchemaP schema = schemas->FindItem (schemaName);
        if (schema == NULL) {
            throw FdoCommandException::Create (
                NlsMsgGet(FDO_NLSID(FDOWMS_NAMED_SCHEMA_NOT_FOUND), (FdoString*)schemaName));
        }

        FdoClassesP featureClasses = schema->GetClasses();
        classDef = featureClasses->FindItem (className);
    }
    else {
        FdoPtr<FdoIDisposableCollection> classes = schemas->FindClass(className);
        if (classes->GetCount() > 1) {
            throw FdoCommandException::Create (
                NlsMsgGet(FDO_NLSID(FDOWMS_AMBIGUOUS_CLASS_NAME), (FdoString*)className));
        }
        else if (classes->GetCount() == 1) {
            classDef = static_cast<FdoClassDefinition*>(classes->GetItem(0));
        }
    }

    if (classDef == NULL) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_NAMED_SCHEMACLASS_NOT_FOUND), (FdoString*)className));
    }

    if (classDef->GetClassType() != FdoClassType_FeatureClass) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_SPATIALEXTENTS_NON_FEATURE_CLASS), (FdoString*)className));
    }

    FdoFeatureClass* featureClass = static_cast<FdoFeatureClass*>(classDef.p);
    if (featureClass->GetIsAbstract()) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_CANNOT_QUERY_ABSTRACT_CLASS), (FdoString*)className));
    }

    FdoPtr<FdoRasterPropertyDefinition> rasterProp = mConnection->FindRasterProperty(featureClass);
    if (!rasterProp) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_FEATURE_NO_RASTER_PROPERTY), (FdoString*)className));
    }

    if (wcscmp(argId->GetName(), rasterProp->GetName()) != 0) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(FDOWMS_SELECTAGGREGATES_INVALID_ARGUMENT_TYPE), argId->GetName(), (FdoString*)className));
    }

    return new FdoWmsSpatialExtentsAggregateReader(mConnection, mClassName, computedIdentifier->GetName());
}

