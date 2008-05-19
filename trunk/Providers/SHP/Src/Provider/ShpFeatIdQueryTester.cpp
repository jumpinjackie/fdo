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
#include "ShpFeatIdQueryTester.h"
#include <FdoExpressionEngineImp.h>


ShpFeatIdQueryTester::ShpFeatIdQueryTester()
{    
}

ShpFeatIdQueryTester* ShpFeatIdQueryTester::Create( FdoIReader* reader )
{
    ShpFeatureReader* my_reader = (ShpFeatureReader *)reader;
    FdoPtr<ShpConnection>  connection = my_reader->GetConnection();
    FdoPtr<FdoClassDefinition> classDef = ShpSchemaUtilities::GetLogicalClassDefinition (connection, my_reader->GetClassName(), NULL);

    return new ShpFeatIdQueryTester( reader, classDef ); 
}

ShpFeatIdQueryTester::ShpFeatIdQueryTester( FdoIReader* reader, FdoClassDefinition* classDef ):
    FdoExpressionEngineImp (reader, classDef, NULL, NULL)
{
    ShpFeatureReader* my_reader = (ShpFeatureReader *)reader;
    m_Connection = my_reader->GetConnection();

    m_Class = classDef; 
    FDO_SAFE_ADDREF (m_Class.p);

    mProperties = classDef->GetProperties ();

    FdoPtr<FdoDataPropertyDefinitionCollection> idProps = classDef->GetIdentityProperties();
    FdoPtr<FdoDataPropertyDefinition> idProp = idProps->GetItem(0);

    m_LogicalIdentityPropertyName = idProp->GetName();
    m_IsFeatidQuery = true;
}

ShpFeatIdQueryTester::~ShpFeatIdQueryTester()
{    
}

void ShpFeatIdQueryTester::Dispose()
{
    delete this;
}

bool ShpFeatIdQueryTester::IsFeatidQuery( )
{
    return m_IsFeatidQuery;
}

FdoPropertyDefinition* ShpFeatIdQueryTester::GetPropInfo (FdoString* name)
{
    return (mProperties->FindItem (name));
}

void ShpFeatIdQueryTester::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
   
    FdoPtr<FdoFilter> left = filter.GetLeftOperand();
    FdoPtr<FdoFilter> right = filter.GetRightOperand();

    left->Process (this);
    right->Process(this);
}

void ShpFeatIdQueryTester::ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> right = filter.GetOperand ();

    right->Process(this);
}

void ShpFeatIdQueryTester::ProcessComparisonCondition(FdoComparisonCondition& filter)
{

    FdoPtr<FdoExpression> left = filter.GetLeftExpression();
    FdoPtr<FdoExpression> right = filter.GetRightExpression();

    FdoIdentifier* ident = dynamic_cast<FdoIdentifier*>(left.p);
    if (ident == NULL)
        m_IsFeatidQuery = false;
    else
    {
        FdoDataValue* val = dynamic_cast<FdoDataValue*>(right.p);
        if (val == NULL)
            m_IsFeatidQuery = false;
        else
        {
            if (0 != wcscmp (ident->GetName(), m_LogicalIdentityPropertyName) != 0)
            {
                m_IsFeatidQuery = false;
            }
        }
    }
}

void ShpFeatIdQueryTester::ProcessInCondition(FdoInCondition& filter)
{
    FdoPtr<FdoExpression> left = filter.GetPropertyName();
 
    FdoPtr<FdoIdentifier> ident = filter.GetPropertyName();
    FdoPtr<FdoValueExpressionCollection> vals = filter.GetValues();

    if (0 != wcscmp (ident->GetName(), m_LogicalIdentityPropertyName) != 0)
    {
        m_IsFeatidQuery = false;
    }
}

void ShpFeatIdQueryTester::ProcessNullCondition (FdoNullCondition& filter)
{
    m_IsFeatidQuery = false;
}

void ShpFeatIdQueryTester::ProcessDistanceCondition (FdoDistanceCondition& filter)
{
    // Until it is implemented...
    m_IsFeatidQuery = false;
}

void ShpFeatIdQueryTester::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    // Yes, it's a feature query
}

