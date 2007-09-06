// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "ShpSelectAggregates.h"
#include "ShpConnection.h"
#include "ShpOptimizedAggregateReader.h"

ShpSelectAggregates::ShpSelectAggregates(ShpConnection* connection)
    : ShpSelectAggregatesCommand(connection)
{
}

ShpSelectAggregates::~ShpSelectAggregates()
{
}

FdoIDataReader* ShpSelectAggregates::Execute()
{
    FdoPtr<ShpConnection> conn = (ShpConnection*)GetConnection();
    // verify connection
    if (conn == NULL)
        throw FdoCommandException::Create(NlsMsgGet(SHP_CONNECTION_INVALID, "Connection is invalid."));

    // verify connection is open - this implicitly verifies the
    // connection string is set and is valid
    if (conn->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGet(SHP_CONNECTION_INVALID, "Connection is invalid."));

    FdoStringP className = mClassName->GetName();
    // verify the feature class name is specified
    if (className.GetLength() == 0)
        throw FdoCommandException::Create(NlsMsgGet(SHP_FEATURE_CLASS_NOT_FOUND, "FDO class '%1$ls' not found in schema.", L""));

    //get feature class
    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition (conn, className);
    FdoPtr<FdoClassDefinition> classDef = lpClass->GetLogicalClass();
    FdoFeatureClass* featureClassDef = dynamic_cast<FdoFeatureClass*>(classDef.p);

    // Verify if this is a special case we can optimize (no filter, no grouping fitler,
    // and only aggregate functions Count() and/or SpatialExtents())
 	bool		bOtherAggrSelected = false;
	aggr_list	*selAggrList = new aggr_list;

    if (!mFilter && !m_GroupingFilter)
    {
		for (int i = 0; i <  mPropertiesToSelect->GetCount() && !bOtherAggrSelected; i++ )
		{
			FdoPtr<FdoIdentifier> identifier = mPropertiesToSelect->GetItem(i);
			FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier*>(identifier.p);
			
			if (computedIdentifier) 
			{
				FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
				FdoFunction* func = dynamic_cast<FdoFunction*>(expr.p);
				if (func && 0==wcscmp(func->GetName(), FDO_FUNCTION_SPATIALEXTENTS))
				{
					FdoPtr<FdoExpressionCollection> args = func->GetArguments();
					FdoPtr<FdoExpression> arg = args->GetItem(0);
					FdoIdentifier* argId = dynamic_cast<FdoIdentifier*>(arg.p);
					if (featureClassDef)
					{
						FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClassDef->GetGeometryProperty();
						if (geomProp && argId && 0==wcscmp(argId->GetName(), geomProp->GetName()))
						{
							AggregateElement *id = new AggregateElement;
							id->name = computedIdentifier->GetName();
							id->type = FdoPropertyType_GeometricProperty;
							selAggrList->push_back( id );
						}
					}
				}
				else if (func && 0 == wcscmp(func->GetName(), FDO_FUNCTION_COUNT))
				{
					AggregateElement *id = new AggregateElement;
					id->name = computedIdentifier->GetName();
					id->type = FdoPropertyType_DataProperty;
					selAggrList->push_back( id );
				}
				else
				{
					// Sorry, no optimization. Clean up.
					for ( size_t i = 0; i < selAggrList->size(); i++ )
						delete selAggrList->at(i);
					delete selAggrList;

					bOtherAggrSelected = true;
				}
			}
		}
    }

    // Now perform the actual select aggregates and return the data reader:
    if ( !bOtherAggrSelected && ( selAggrList->size() > 0 ))
    {
		// The reader takes ownership of the selAggrList
        return new ShpOptimizedAggregateReader(conn, featureClassDef, selAggrList );
    }
    else			
    {
        return ShpSelectAggregatesCommand::Execute();
    }
}
