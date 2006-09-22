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
#include "FdoWfsSelectAggregatesCommand.h"
#include "FdoWfsSpatialExtentsAggregateReader.h"
#include <../Message/Inc/WFSMessage.h>

// Default Constructor
FdoWfsSelectAggregatesCommand::FdoWfsSelectAggregatesCommand (FdoIConnection *connection) :
    FdoCommonFeatureCommand<FdoISelectAggregates, FdoWfsConnection> (connection),
    m_bDistinct(false),
    m_eOrderingOption(FdoOrderingOption_Ascending)
{
    m_GroupingIds = FdoIdentifierCollection::Create();
    m_OrderingIds = FdoIdentifierCollection::Create();
}

// Default Destructor
FdoWfsSelectAggregatesCommand::~FdoWfsSelectAggregatesCommand (void)
{
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* FdoWfsSelectAggregatesCommand::GetPropertyNames ()
{
    if (mPropertiesToSelect == NULL) {
        mPropertiesToSelect = FdoIdentifierCollection::Create();
    }

    return (FDO_SAFE_ADDREF(mPropertiesToSelect.p));
}

/// <summary>Executes the select command and returns a reference to an FdoIDataReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIDataReader* FdoWfsSelectAggregatesCommand::Execute ()
{
    if (mConnection->GetConnectionState() != FdoConnectionState_Open) {
        throw FdoCommandException::Create(
            NlsMsgGet(FDO_NLSID(WFS_CONNECTION_CLOSED)));
    }

    FdoStringP className = mClassName->GetName();	
    if (className.GetLength() == 0) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(WFS_NAMED_CLASS_NOT_FOUND), (FdoString*)className));
    }

    if (mPropertiesToSelect->GetCount()!=1) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(WFS_NO_QUERY_PROPERTIES_SELECTED), (FdoString*)className));
    }

    FdoPtr<FdoIdentifier> identifier = mPropertiesToSelect->GetItem(0);
    FdoString* identifierName = identifier->GetName();
    FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier*>(identifier.p);
    if (!computedIdentifier) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(WFS_QUERY_NON_COMPUTED_PROPERTIES), identifierName, (FdoString*)className));
    }

    FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
    FdoFunction* function = dynamic_cast<FdoFunction*>(expr.p);
    if (!function) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(WFS_QUERY_NON_COMPUTED_PROPERTIES), identifierName, (FdoString*)className));
    }

    if (wcscmp(function->GetName(), FDO_FUNCTION_SPATIALEXTENTS) != 0) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(WFS_QUERY_NON_FUNCTION_SPATIALEXTENTS), function->GetName()));
    }

    FdoPtr<FdoExpressionCollection> args = function->GetArguments();
    if (args->GetCount() != 1) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(WFS_SPATIALEXTENTS_TOO_MANY_ARGUMENTS)));
    }

    FdoPtr<FdoExpression> arg = args->GetItem(0);
    FdoIdentifier* argId = dynamic_cast<FdoIdentifier*>(arg.p);
    if (!argId) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(WFS_SELECTAGGREGATES_INVALID_ARGUMENT)));
    }


    FdoPtr<FdoFilter> filter = GetFilter();
    if (filter) {
        throw FdoCommandException::Create (
            NlsMsgGet(FDO_NLSID(WFS_SELECTAGGREGATES_FILTER_NOT_SUPPORTED)));
    }

    if (mConnection->IsSchemaLoadedFromServer()) {
        FdoFeatureSchemasP schemas = mConnection->GetSchemas();
        FdoStringP schemaName = mClassName->GetSchemaName();
        FdoClassDefinitionP classDef;

        if (schemaName.GetLength() > 0) {
	        FdoFeatureSchemaP schema = schemas->FindItem (schemaName);
            if (schema == NULL) {
                throw FdoCommandException::Create (
                    NlsMsgGet(FDO_NLSID(WFS_NAMED_SCHEMA_NOT_FOUND), (FdoString*)schemaName));
            }

	        FdoClassesP featureClasses = schema->GetClasses();
	        classDef = featureClasses->FindItem (className);
        }
        else {
            FdoPtr<FdoIDisposableCollection> classes = schemas->FindClass(className);
            if (classes->GetCount() > 1) {
                throw FdoCommandException::Create (
                    NlsMsgGet(FDO_NLSID(WFS_AMBIGUOUS_CLASS_NAME), (FdoString*)className));
            }
            else if (classes->GetCount() == 1) {
                classDef = static_cast<FdoClassDefinition*>(classes->GetItem(0));
            }
        }

        if (classDef == NULL) {
            throw FdoCommandException::Create (
                NlsMsgGet(FDO_NLSID(WFS_NAMED_CLASS_NOT_FOUND), (FdoString*)className));
        }

        if (classDef->GetClassType() != FdoClassType_FeatureClass) {
            throw FdoCommandException::Create (
                NlsMsgGet(FDO_NLSID(WFS_SPATIALEXTENTS_NON_FEATURE_CLASS), (FdoString*)className));
        }

	    FdoFeatureClass* featureClass = static_cast<FdoFeatureClass*>(classDef.p);
        if (featureClass->GetIsAbstract()) {
            throw FdoCommandException::Create (
                NlsMsgGet(FDO_NLSID(WFS_CANNOT_QUERY_ABSTRACT_CLASS), (FdoString*)className));
        }

        FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClass->GetGeometryProperty();
        if (!geomProp) {
            throw FdoCommandException::Create (
                NlsMsgGet(FDO_NLSID(WFS_FEATURE_NO_GEOMETRY_PROPERTY), (FdoString*)className));
        }

        if (wcscmp(argId->GetName(), geomProp->GetName()) != 0) {
            throw FdoCommandException::Create (
                NlsMsgGet(FDO_NLSID(WFS_SELECTAGGREGATES_INVALID_ARGUMENT_TYPE), argId->GetName(), (FdoString*)className));
        }
    }

    return new FdoWfsSpatialExtentsAggregateReader(mConnection, (FdoString*)className, computedIdentifier->GetName());
}


/// <summary>Set the distinct option of the selection. Note that grouping criteria is not supported with Distinct. 
/// Non-simple properties such as object properties, geometry properties, raster properties, association properties, etc. will not be supported with Distinct.</summary>
/// <param name="value">true or false; when set to true, only distinct values are returned. Otherwise all values are returned</param> 
/// <returns>Returns nothing</returns>  
void FdoWfsSelectAggregatesCommand::SetDistinct( bool value )
{
    m_bDistinct = value;
}

/// <summary>Get the distinct option.</summary>
/// <returns>Returns true if distinct is set, false otherwise.</returns> 
bool FdoWfsSelectAggregatesCommand::GetDistinct( )
{
    return m_bDistinct;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of group by property names. If empty no grouping is used. This list is initially
/// empty and the caller need to add the property that the command should use as a group by criteria. No LOB or Geometry type properties
/// can be used for ordering.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* FdoWfsSelectAggregatesCommand::GetGrouping()
{
    return FDO_SAFE_ADDREF(m_GroupingIds.p);
}

/// <summary>Set the grouping by filter. Use the grouping filter to restrict the groups of returned properties to those groups for 
/// which the specified filter is TRUE. For example "order by city" and  "min(lanes) = 2". The FdoFilter have to evalute to a 
/// binary value(true or false).</summary>
/// <param name="filter">The grouping filter.</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsSelectAggregatesCommand::SetGroupingFilter(FdoFilter* filter)
{
    m_GroupingFilter = FDO_SAFE_ADDREF(filter);
}

/// <summary>Gets the grouping by filter.</summary>
/// <returns>Returns the grouping filter.</returns> 
FdoFilter* FdoWfsSelectAggregatesCommand::GetGroupingFilter(void)
{
    return FDO_SAFE_ADDREF(m_GroupingFilter.p);
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
/// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* FdoWfsSelectAggregatesCommand::GetOrdering(void)
{
    return FDO_SAFE_ADDREF(m_OrderingIds.p);
}

/// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
/// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
/// FdoOrderingOption_Ascending is the default value.</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsSelectAggregatesCommand::SetOrderingOption(FdoOrderingOption option)
{
    m_eOrderingOption = option;
}

/// <summary>Gets the ordering option.</summary>
/// <returns>Returns the ordering option.</returns> 
FdoOrderingOption FdoWfsSelectAggregatesCommand::GetOrderingOption(void)
{
    return m_eOrderingOption;
}
