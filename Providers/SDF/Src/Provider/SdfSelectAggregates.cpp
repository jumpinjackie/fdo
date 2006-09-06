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
    : FdoCommonSelectAggregatesCommand(connection)
{
}

SdfSelectAggregates::~SdfSelectAggregates()
{
}

FdoIDataReader* SdfSelectAggregates::Execute()
{
    // verify connection
    FdoPtr<SdfConnection> conn = (SdfConnection*)GetConnection();
    if (conn == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_39_NO_CONNECTION)));

    // verify connection is open - this implicitly verifies the
    // connection string is set and is valid
    if (conn->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    // verify the feature class name is specified
    FdoStringP className = mClassName->GetName();
    if (className.GetLength() == 0)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_41_NULL_FEATURE_CLASS)));

    // get feature class
    FdoPtr<FdoClassDefinition> classDef = FdoPtr<FdoClassCollection>(conn->GetSchema()->GetClasses())->GetItem(className);
    FdoFeatureClass* featureClassDef = dynamic_cast<FdoFeatureClass*>(classDef.p);

    // ensure we have at least 1 property to select
    if (mPropertiesToSelect->GetCount()<=0) {
        throw FdoCommandException::Create(
            NlsMsgGetMain(
                FDO_NLSID(SDFPROVIDER_90_NO_QUERY_PROPERTIES_SELECTED), (FdoString*)className));
    }

    // determine if we are attempting to execute the SelectAggregates(SpatialFilter) command
    FdoPtr<FdoIdentifier> identifier = mPropertiesToSelect->GetItem(0);
    FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier*>(identifier.p);
    if (computedIdentifier) 
    {
        // retrieve the computed identifier expression and get the function
        FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
        FdoFunction* function = dynamic_cast<FdoFunction*>(expr.p);
        if (function) 
        {
            // if we have a SpatialExtents function, keep processing
            if (wcscmp(function->GetName(), FDO_FUNCTION_SPATIALEXTENTS) == 0) 
            {
                // a filter cannot be specified with the spatial extents function
                if (mFilter || m_GroupingFilter) {
                    throw FdoCommandException::Create(
                        NlsMsgGetMain(
                            FDO_NLSID(SDFPROVIDER_91_SELECTAGGREGATES_FILTER_NOT_SUPPORTED)));
                }

                // the spatial extents function cannot take more than 1 argument
                FdoPtr<FdoExpressionCollection> args = function->GetArguments();
                if (args->GetCount() != 1) {
                    throw FdoCommandException::Create(
                        NlsMsgGetMain(
                            FDO_NLSID(SDFPROVIDER_92_SPATIALEXTENTS_TOO_MANY_ARGUMENTS)));
                }

                // the argument to the spatial extents function must be an identifier
                FdoPtr<FdoExpression> arg = args->GetItem(0);
                FdoIdentifier* argId = dynamic_cast<FdoIdentifier*>(arg.p);
                if (!argId) {
                    throw FdoCommandException::Create(
                        NlsMsgGetMain(
                            FDO_NLSID(SDFPROVIDER_93_SELECTAGGREGATES_INVALID_ARGUMENT_TYPE), argId->GetName(), (FdoString*)className));
                }

                // the argument to the spatial extents function must be the name of the geometry property
                FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClassDef->GetGeometryProperty();
                if (wcscmp(argId->GetName(), geomProp->GetName()) != 0) {
                    throw FdoCommandException::Create(
                        NlsMsgGetMain(
                            FDO_NLSID(SDFPROVIDER_93_SELECTAGGREGATES_INVALID_ARGUMENT_TYPE), argId->GetName(), (FdoString*)className));
                }

                // create and return an instance of teh spatial extents reader
                return new SdfSpatialExtentsAggregateReader(conn, featureClassDef, computedIdentifier->GetName());
            }
        }
    }

    // Execute the common aggretaes command to get non SpatialExtents support
    return FdoCommonSelectAggregatesCommand::Execute();
}
