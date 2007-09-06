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
#include "SdfSelectAggregates.h"
#include "SdfConnection.h"
#include "SdfSpatialExtentsAggregateReader.h"

SdfSelectAggregates::SdfSelectAggregates(SdfConnection* connection)
    : SdfSelectAggregatesCommand(connection)
{
}

SdfSelectAggregates::~SdfSelectAggregates()
{
}

FdoIDataReader* SdfSelectAggregates::Execute()
{
    FdoPtr<SdfConnection> conn = (SdfConnection*)GetConnection();
    // verify connection
    if (conn == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_39_NO_CONNECTION)));

    // verify connection is open - this implicitly verifies the
    // connection string is set and is valid
    if (conn->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    FdoStringP className = mClassName->GetName();
    // verify the feature class name is specified
    if (className.GetLength() == 0)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_41_NULL_FEATURE_CLASS)));

    //get feature class
    FdoPtr<FdoClassDefinition> classDef = FdoPtr<FdoClassCollection>(
        conn->GetSchema()->GetClasses())->GetItem(className);
    FdoFeatureClass* featureClassDef = dynamic_cast<FdoFeatureClass*>(classDef.p);

    // Verify if this is a special case we can optimize (no filter, no grouping fitler,
    // and specific aggregate functions (SpatialExtents() on the main geometry property of the class
    // and the Count() function) 
    // SpatialExtent and Count can be derived from the RTree and Datadb without needing a full table scan.
    // This optimization can be a huge performance improvement if the caller wants to find out the extent and the 
    // number of objects for a given class.
    bool bCanOptimizeSpatialExtents = false;
    FdoStringP  extentIdName;
    FdoStringP  countName;

    FdoComputedIdentifier* computedIdentifier = NULL;
    int propsCount = mPropertiesToSelect->GetCount();
    if (!mFilter && !m_GroupingFilter && (propsCount == 1 || propsCount == 2))
    {
        bCanOptimizeSpatialExtents = true;
        for(int i=0; i<mPropertiesToSelect->GetCount() && bCanOptimizeSpatialExtents;i++)
        {
            FdoPtr<FdoIdentifier> identifier = mPropertiesToSelect->GetItem(i);
            computedIdentifier = dynamic_cast<FdoComputedIdentifier*>(identifier.p);
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
                        if (! (geomProp && argId && 0==wcscmp(argId->GetName(), geomProp->GetName()) ) )
                            bCanOptimizeSpatialExtents = false;
                        else
                            extentIdName = computedIdentifier->GetName();
                    }
                }
                else if (func && 0==wcscmp(func->GetName(), FDO_FUNCTION_COUNT) && propsCount == 2 )
					countName = computedIdentifier->GetName();
                else
                    bCanOptimizeSpatialExtents = false;
            }
            else
            {
                bCanOptimizeSpatialExtents = false;
            }
        }
    }

    // Now perform the actual select aggregates and return the data reader:
    if (bCanOptimizeSpatialExtents)
    {
        return new SdfSpatialExtentsAggregateReader(conn, featureClassDef, extentIdName, countName );
    }
    else
    {
        return SdfSelectAggregatesCommand::Execute();
    }
}
