/*
 * 
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
#include "ArcSDESelectAggregatesCommand.h"
#include "ArcSDEDataReader.h"
#include "Util/FdoExpressionEngineUtilFeatureReader.h"
#include "FdoExpressionEngine.h"


ArcSDESelectAggregatesCommand::ArcSDESelectAggregatesCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoISelectAggregates> (connection),
    m_bDistinct(false),
    m_eOrderingOption(FdoOrderingOption_Ascending)
{
    m_GroupingIds = FdoIdentifierCollection::Create();
    m_OrderingIds = FdoIdentifierCollection::Create();
}

/** Do not implement the copy constructor. **/
//ArcSDESelectAggregatesCommand::ArcSDESelectAggregatesCommand (const ArcSDESelectAggregatesCommand &right) { }

ArcSDESelectAggregatesCommand::~ArcSDESelectAggregatesCommand (void)
{
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* ArcSDESelectAggregatesCommand::GetPropertyNames ()
{
    if (mPropertiesToSelect == NULL)
        mPropertiesToSelect = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(mPropertiesToSelect.p));
}

/// <summary>Executes the select command and returns a reference to an FdoIDataReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIDataReader* ArcSDESelectAggregatesCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    FdoPtr<FdoClassDefinition> definition;
    FdoPtr<FdoFilter> filter;
    FdoPtr<ArcSDEDataReader> ret;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // verify the feature class name is specified
    if (mClassName == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_FEATURE_CLASS_UNSPECIFIED, "Feature class name not specified."));

    // Get the class definition which reflects the requested feature class name
    definition = connection->GetRequestedClassDefinition (mClassName);

    // get any filtering
    filter = GetFilter ();

    // create the feature reader
	bool filterValid, selectListValid;

	ret = new ArcSDEDataReader (connection, definition, filter, mPropertiesToSelect,
			m_bDistinct, m_eOrderingOption, m_OrderingIds, m_GroupingFilter, m_GroupingIds);

	if ( ret->ContainsSDEValidExpressionsOnly (filterValid, selectListValid) )
	{
#ifdef MIMIC_OTHER_PROVIDER
		// SPR 612054 SelectCommand error reporting inconsistency with the Oracle Provider when using invalid property name in filter.
		// we call prepare stream to discover errors now, rather than at ReadNext() time
		ret->PrepareStream ();
		return (FDO_SAFE_ADDREF (ret.p));
#endif // MIMIC_RDBMS
	}
	else
	{
		// transfer over the identifiers to the basic select command:
		FdoPtr<FdoIdentifierCollection> propertiesToSelect = FdoIdentifierCollection::Create();
		FdoPtr<FdoPropertyDefinitionCollection> propDefs = definition->GetProperties();
		for (int i=0; i<propDefs->GetCount(); i++)
		{
			FdoPtr<FdoPropertyDefinition> propDef = propDefs->GetItem(i);
			FdoPtr<FdoIdentifier> localId = FdoIdentifier::Create(propDef->GetName());
			propertiesToSelect->Add(localId);
		}
		FdoPtr<FdoReadOnlyPropertyDefinitionCollection> basePropDefs = definition->GetBaseProperties();
		for (int i=0; i<basePropDefs->GetCount(); i++)
		{
			FdoPtr<FdoPropertyDefinition> basePropDef = basePropDefs->GetItem(i);
			FdoPtr<FdoIdentifier> localId = FdoIdentifier::Create(basePropDef->GetName());
			propertiesToSelect->Add(localId);
		}

		FdoPtr<ArcSDEDataReader> sub_reader;
		FdoPtr<FdoIDataReader>	 reader;

		FdoCommonExpressionType exprType;
		FdoPtr<FdoIExpressionCapabilities> expressionCaps = connection->GetExpressionCapabilities();
		FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
		FdoPtr< FdoArray<FdoFunction*> > aggrIdents = FdoExpressionEngineUtilDataReader::GetAggregateFunctions(functions, mPropertiesToSelect, exprType);

		// Delegate processing to the Expression engine.
		if ( filterValid )
		{
			// CASE 1: the filter is supported. Create a new sub-reader over all properties with the filter.
			sub_reader = new ArcSDEDataReader (connection, definition, filter, NULL, false, m_eOrderingOption, NULL, NULL, NULL);
			reader = new FdoExpressionEngineUtilDataReader(functions, sub_reader, definition, mPropertiesToSelect/*propertiesToSelect*/, m_bDistinct, m_OrderingIds, m_eOrderingOption, mPropertiesToSelect, aggrIdents);
		}
		else
		{
			// CASE 2: the filter is not supported. Create a new sub-reader with no filtering over all properties.
			sub_reader = new ArcSDEDataReader (connection, definition, NULL, NULL, false, m_eOrderingOption, NULL, NULL, NULL);
			reader = new FdoExpressionEngineUtilDataReader(functions, sub_reader, definition, mPropertiesToSelect, m_bDistinct, m_OrderingIds, m_eOrderingOption, NULL/*ids*/, aggrIdents);
		}

		return FDO_SAFE_ADDREF(reader.p);
	}
}

/// <summary>Set the distinct option of the selection. Note that grouping criteria is not supported with Distinct. 
/// Non-simple properties such as object properties, geometry properties, raster properties, association properties, etc. will not be supported with Distinct.</summary>
/// <param name="value">true or false; when set to true, only distinct values are returned. Otherwise all values are returned</param> 
/// <returns>Returns nothing</returns>  
void ArcSDESelectAggregatesCommand::SetDistinct( bool value )
{
    m_bDistinct = value;
}

/// <summary>Get the distinct option.</summary>
/// <returns>Returns true if distinct is set, false otherwise.</returns> 
bool ArcSDESelectAggregatesCommand::GetDistinct( )
{
    return m_bDistinct;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of group by property names. If empty no grouping is used. This list is initially
/// empty and the caller need to add the property that the command should use as a group by criteria. No LOB or Geometry type properties
/// can be used for ordering.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* ArcSDESelectAggregatesCommand::GetGrouping()
{
    return FDO_SAFE_ADDREF(m_GroupingIds.p);
}

/// <summary>Set the grouping by filter. Use the grouping filter to restrict the groups of returned properties to those groups for 
/// which the specified filter is TRUE. For example "order by city" and  "min(lanes) = 2". The FdoFilter have to evalute to a 
/// binary value(true or false).</summary>
/// <param name="filter">The grouping filter.</param> 
/// <returns>Returns nothing</returns> 
void ArcSDESelectAggregatesCommand::SetGroupingFilter(FdoFilter* filter)
{
    m_GroupingFilter = FDO_SAFE_ADDREF(filter);
}

/// <summary>Gets the grouping by filter.</summary>
/// <returns>Returns the grouping filter.</returns> 
FdoFilter* ArcSDESelectAggregatesCommand::GetGroupingFilter(void)
{
    return FDO_SAFE_ADDREF(m_GroupingFilter.p);
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
/// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* ArcSDESelectAggregatesCommand::GetOrdering(void)
{
    return FDO_SAFE_ADDREF(m_OrderingIds.p);
}

/// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
/// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
/// FdoOrderingOption_Ascending is the default value.</param> 
/// <returns>Returns nothing</returns> 
void ArcSDESelectAggregatesCommand::SetOrderingOption(FdoOrderingOption option)
{
    m_eOrderingOption = option;
}

/// <summary>Gets the ordering option.</summary>
/// <returns>Returns the ordering option.</returns> 
FdoOrderingOption ArcSDESelectAggregatesCommand::GetOrderingOption(void)
{
    return m_eOrderingOption;
}

