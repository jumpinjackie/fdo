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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSelectAggregate.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpConnection.h"
#include "FdoRfpSelectAggregate.h"
#include "FdoRfpDataReader.h"
#include "FdoRfpQueryExecutor.h"
#include "FdoRfpClassData.h"


FdoRfpSelectAggregate::FdoRfpSelectAggregate (FdoIConnection *connection) :
    FdoRfpFeatureCommand<FdoISelectAggregates> (connection)
{
}

FdoRfpSelectAggregate::~FdoRfpSelectAggregate()
{
}


/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* FdoRfpSelectAggregate::GetPropertyNames ()
{
    if (m_propertiesToSelect == NULL)
        m_propertiesToSelect = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(m_propertiesToSelect.p));
}

/// <summary>Executes the select command and returns a reference to an FdoIDataReader.</summary>
/// <returns>Returns the data reader.</returns> 
FdoIDataReader* FdoRfpSelectAggregate::Execute()
{
	FdoRfpConnectionP connection = static_cast<FdoRfpConnection*>(GetConnection());

	//Verify the feature class name is specified
	if (mClassName == NULL)
		throw FdoCommandException::Create(NlsMsgGet(GRFP_49_CLASS_NAME_MUST_BE_SPECIFIED, "Feature class name must be specified."));		

	FdoPtr<FdoClassDefinition> classDef;
	connection->GetClassDef(mClassName, classDef);
	if (classDef == NULL)
		throw FdoCommandException::Create(NlsMsgGet1(GRFP_11_FEATURE_CLASS_NOT_FOUND, "Feature class '%1$ls' not found in schema.", mClassName->GetName()));

	//Get the class data
	FdoPtr<FdoRfpClassData> classData = connection->GetClassData(classDef);


	FdoRfpQueryExecutor executor(classDef, mFilter, m_propertiesToSelect, classData);

	FdoPtr<FdoRfpQueryResult> queryResult = executor.Execute();

	return FdoRfpDataReader::Create(queryResult);
}

/// <summary>Set the distinct option of the selection. Note that grouping criteria is not supported with Distinct. 
/// Non-simple properties such as object properties, geometry properties, raster properties, association properties, etc. will not be supported with Distinct.</summary>
/// <param name="value">true or false; when set to true, only distinct values are returned. Otherwise all values are returned</param> 
/// <returns>Returns nothing</returns>  
void FdoRfpSelectAggregate::SetDistinct( bool value )
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"Distinct"));
}

/// <summary>Get the distinct option.</summary>
/// <returns>Returns true if distinct is set, false otherwise.</returns> 
bool FdoRfpSelectAggregate::GetDistinct()
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"Distinct"));
	return true;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of group by property names. If empty no grouping is used. This list is initially
/// empty and the caller need to add the property that the command should use as a group by criteria. No LOB or Geometry type properties
/// can be used for ordering.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* FdoRfpSelectAggregate::GetGrouping()
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"Grouping"));
	return NULL;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of order by property names. If empty no ordering is used. This list is initially
/// empty and the caller need to add the property that the command should use as a order by criteria.</summary>
/// <returns>Returns the list of group by property names.</returns> 
FdoIdentifierCollection* FdoRfpSelectAggregate::GetOrdering()
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"Ordering"));
	return NULL;
}

/// <summary>Set the ordering option of the selection. This is only used if the ordering collection is not empty.</summary>
/// <param name="option">Is the ordering option and should be set to one of FdoOrderingOption_Ascending or FdoOrderingOption_Descending.
/// FdoOrderingOption_Ascending is the default value.</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpSelectAggregate::SetOrderingOption( FdoOrderingOption  option )
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"Ordering"));
}

/// <summary>Gets the ordering option.</summary>
/// <returns>Returns the ordering option.</returns> 
FdoOrderingOption FdoRfpSelectAggregate::GetOrderingOption( )
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"Ordering"));
	return FdoOrderingOption_Ascending;
}

/// <summary>Set the grouping by filter. Use the grouping filter to restrict the groups of returned properties to those groups for 
/// which the specified filter is TRUE. For example "order by city" and  "min(lanes) = 2". The FdoFilter have to evalute to a 
/// binary value(true or false).</summary>
/// <param name="filter">The grouping filter.</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpSelectAggregate::SetGroupingFilter( FdoFilter* filter )
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"Grouping Filter"));
}

/// <summary>Gets the grouping by filter.</summary>
/// <returns>Returns the grouping filter.</returns> 
FdoFilter* FdoRfpSelectAggregate::GetGroupingFilter( )
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"Grouping Filter"));
	return NULL;
}
