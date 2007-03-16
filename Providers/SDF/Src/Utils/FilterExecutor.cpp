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
#include "FilterExecutor.h"

#include "DataValuePool.h"
#include "Int64Value.h"
#include "DoubleValue.h"
#include "StringValue.h"
#include "BooleanValue.h"
#include "DateTimeValue.h"
#include "NullValue.h"
#include <FdoSpatial.h>
#include "FdoCommonMiscUtil.h"
#include <wctype.h>
#include <malloc.h>
#include <math.h>

FilterExecutor::FilterExecutor(FdoIFeatureReader* featureReader, PropertyIndex* propIndex, FdoIdentifierCollection* compIdents, FdoClassDefinition* classDef)
{
    //don't think we need an addref since the FeatureReader owns us.
    m_reader = featureReader;
    m_propIndex = propIndex;
    m_classDefinition = classDef;

    m_pPool = new DataValuePool();

    m_compIdents = compIdents;
    FDO_SAFE_ADDREF(m_compIdents);
}

FilterExecutor::~FilterExecutor()
{
    while (!m_retvals.empty())
        delete m_retvals.pop();

    delete m_pPool;

    FDO_SAFE_RELEASE(m_compIdents);
}

FilterExecutor* FilterExecutor::Create(FdoIFeatureReader* featureReader, PropertyIndex* propIndex, FdoIdentifierCollection* compIdents, FdoClassDefinition* classDef)
{
    return new FilterExecutor(featureReader, propIndex, compIdents, classDef);
}


void FilterExecutor::Dispose()
{
    delete this;
}


void FilterExecutor::Reset()
{
    while (!m_retvals.empty())
        m_pPool->RelinquishDataValue(m_retvals.pop());
}

//returns the data type of the result
//of the expression/filter evaluation.
//For filters it is always boolean
DataValueType FilterExecutor::GetResultType()
{
    return m_retvals.top()->GetType();
}


bool FilterExecutor::GetBooleanResult()
{
    _ASSERT(m_retvals.top()->GetType() == Dvt_Boolean);
    
    //pop the result off the top of the stack and return it
    DataValue* dv = m_retvals.pop();
    bool ret = dv->GetAsBoolean();
    m_pPool->RelinquishDataValue(dv);
    
    return ret;
}


double FilterExecutor::GetDoubleResult()
{
    _ASSERT(m_retvals.top()->GetType() == Dvt_Double);

    //pop the result off the top of the stack and return it
    DataValue* dv = m_retvals.pop();
    double ret = dv->GetAsDouble();
    m_pPool->RelinquishDataValue(dv);

    return ret;
}

FdoInt64 FilterExecutor::GetInt64Result()
{
    _ASSERT(m_retvals.top()->GetType() == Dvt_Int64);

    //pop the result off the top of the stack and return it
    DataValue* dv = m_retvals.pop();
    FdoInt64 ret = dv->GetAsInt64();
    m_pPool->RelinquishDataValue(dv);

    return ret;
}


wchar_t* FilterExecutor::GetStringResult()
{
    _ASSERT(m_retvals.top()->GetType() == Dvt_String);

    //pop the result off the top of the stack and return it
    DataValue* dv = m_retvals.pop();
    wchar_t* ret = dv->GetAsString();

    if (ret == NULL)
        return NULL;

    wchar_t* retcpy = new wchar_t[wcslen(ret)+1];
    wcscpy(retcpy, ret);

    m_pPool->RelinquishDataValue(dv);

    return retcpy;
}

FdoDateTime FilterExecutor::GetDateTimeResult()
{
    //only one DataValue should be left on the evaluation stack after
    //executing a query
    _ASSERT(m_retvals.top()->GetType() == Dvt_DateTime);

    //pop the result off the top of the stack and return it
    DataValue* dv = m_retvals.pop();
    FdoDateTime ret = dv->GetAsDateTime();
    m_pPool->RelinquishDataValue(dv);

    return ret;
}

void FilterExecutor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> left = filter.GetLeftOperand();
    FdoPtr<FdoFilter> right = filter.GetRightOperand();

    //evaluate left hand side
    left->Process(this);

    //get Process() results from return value stack
    DataValue* argLeft = m_retvals.pop();

    //Optimize binary logic by looking at the value of the left operand and see
    //if we have to evaluate the right one also
    if (argLeft->GetAsBoolean() && filter.GetOperation() == FdoBinaryLogicalOperations_Or)
    {
        m_retvals.push(m_pPool->ObtainBooleanValue(true));
        m_pPool->RelinquishDataValue(argLeft);
        return;
    }
    else if (!argLeft->GetAsBoolean() 
        && filter.GetOperation() == FdoBinaryLogicalOperations_And)
    {
        m_retvals.push(m_pPool->ObtainBooleanValue(false));
        m_pPool->RelinquishDataValue(argLeft);
        return;
    }
    
    //evaluate right hand side
    right->Process(this);
    DataValue* argRight = m_retvals.pop();

    switch (filter.GetOperation())
    {
    case FdoBinaryLogicalOperations_And: 
        m_retvals.push(m_pPool->ObtainBooleanValue(argLeft->GetAsBoolean() && argRight->GetAsBoolean())); 
        break;
    case FdoBinaryLogicalOperations_Or : 
        m_retvals.push(m_pPool->ObtainBooleanValue(argLeft->GetAsBoolean() || argRight->GetAsBoolean()));
        break;
    default: 
        throw FdoException::Create(L"Invalid logical operation type");break;
    }

    m_pPool->RelinquishDataValue(argLeft);
    m_pPool->RelinquishDataValue(argRight);
}

void FilterExecutor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> right = filter.GetOperand();

    //evaluate right hand side
    right->Process(this);

    //get Process() results from return value stack
    DataValue* argRight = m_retvals.pop();

    switch (filter.GetOperation())
    {
    case FdoUnaryLogicalOperations_Not: 
        m_retvals.push(m_pPool->ObtainBooleanValue(!argRight->GetAsBoolean())); 
        break;
    default: 
        throw FdoException::Create(L"Invalid logical operation type");break;
    }

    m_pPool->RelinquishDataValue(argRight);
   
}

void FilterExecutor::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
    FdoPtr<FdoExpression> left = filter.GetLeftExpression();
    FdoPtr<FdoExpression> right = filter.GetRightExpression();

    right->Process(this);
    left->Process(this);
    
    //get Process() results from return value stack
    DataValue* argLeft = m_retvals.pop();
    DataValue* argRight = m_retvals.pop();

    switch (filter.GetOperation())
    {
    case FdoComparisonOperations_EqualTo : 
        m_retvals.push(m_pPool->ObtainBooleanValue(argLeft->IsEqualTo(*argRight))); 
        break;
    case FdoComparisonOperations_NotEqualTo : 
        m_retvals.push(m_pPool->ObtainBooleanValue(argLeft->IsNotEqualTo(*argRight))); 
        break;
    case FdoComparisonOperations_GreaterThan : 
        m_retvals.push(m_pPool->ObtainBooleanValue(argLeft->IsGreaterThan(*argRight))); 
        break;
    case FdoComparisonOperations_GreaterThanOrEqualTo : 
        m_retvals.push(m_pPool->ObtainBooleanValue(argLeft->IsGreaterThanOrEqualTo(*argRight))); 
        break;
    case FdoComparisonOperations_LessThan : 
        m_retvals.push(m_pPool->ObtainBooleanValue(argLeft->IsLessThan(*argRight))); 
        break;
    case FdoComparisonOperations_LessThanOrEqualTo : 
        m_retvals.push(m_pPool->ObtainBooleanValue(argLeft->IsLessThanOrEqualTo(*argRight))); 
        break;
    case FdoComparisonOperations_Like : m_retvals.push(m_pPool->ObtainBooleanValue(MatchesHere(argRight->GetAsString(), argLeft->GetAsString())));
        break;    
    default: throw FdoException::Create(L"Invalid comparison operation type");
    }

    m_pPool->RelinquishDataValue(argRight);
    m_pPool->RelinquishDataValue(argLeft);
}

void FilterExecutor::ProcessInCondition(FdoInCondition& filter)
{
    //first get the value of the property we are checking
    FdoPtr<FdoIdentifier> prop = filter.GetPropertyName();
    ProcessIdentifier(*(prop.p));

    //get the property value off the stack
    DataValue* argLeft = m_retvals.pop();

    FdoPtr<FdoValueExpressionCollection> vals = filter.GetValues();

    bool result = false;

    //see if the value collection contains the property value
    for (int i=0; i<vals->GetCount(); i++)
    {
        FdoPtr<FdoValueExpression> expr = vals->GetItem(i);
        expr->Process(this);

        DataValue* argRight = m_retvals.pop();

        if (argLeft->IsEqualTo(*argRight))
            result = true;

		m_pPool->RelinquishDataValue(argRight);

        if (result)
            break;
    }

    //if prop val is not IN the val collection, push false on the stack
    m_retvals.push(m_pPool->ObtainBooleanValue(result));

	m_pPool->RelinquishDataValue(argLeft);
}

void FilterExecutor::ProcessNullCondition(FdoNullCondition& filter)
{
    //first get the value of the property we are checking
    FdoPtr<FdoIdentifier> prop = filter.GetPropertyName();
    
    bool isnull = m_reader->IsNull(prop->GetName());

    m_retvals.push(m_pPool->ObtainBooleanValue(isnull));
}

void FilterExecutor::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    int fgfLen = 0;
	const FdoByte* fgf = NULL;
	FdoString* name = FdoPtr<FdoIdentifier>(filter.GetPropertyName())->GetName();
	if (!m_reader->IsNull(name))
		fgf = m_reader->GetGeometry(name, &fgfLen);

    //no geometry? trivially false.
    if (fgfLen == 0)
        m_retvals.push(m_pPool->ObtainBooleanValue(false));

    FdoPtr<FdoExpression> exprRight = filter.GetGeometry();
    FdoGeometryValue* gvRight = dynamic_cast<FdoGeometryValue*>(exprRight.p);
    
    if (!gvRight)
        throw FdoException::Create(L"Only literal geometric values allowed in spatial conditions.");        

    FdoPtr<FdoByteArray> baRight = gvRight->GetGeometry();

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geomRight = gf->CreateGeometryFromFgf(baRight);
    FdoPtr<FdoIGeometry> geomLeft = gf->CreateGeometryFromFgf(fgf, fgfLen);

	// A polygon might be actually a multi polygon. 
	FdoPtr<FdoIGeometry> geometry = ReconstructPolygon( geomLeft );

    //call on the geometry utility to evaluate the spatial operation
    bool ret = FdoSpatialUtility::Evaluate( geometry? geometry : geomLeft, filter.GetOperation(), geomRight);
    
    m_retvals.push(m_pPool->ObtainBooleanValue(ret));            
}

void FilterExecutor::ProcessDistanceCondition(FdoDistanceCondition& filter)
{
    //TODO:
    throw FdoException::Create(L"DISTANCE condition not supported");
}


void FilterExecutor::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
    FdoPtr<FdoExpression> left = expr.GetLeftExpression();
    FdoPtr<FdoExpression> right = expr.GetRightExpression();

    right->Process(this);
    left->Process(this);
    
    //get Process() results from return value stack
    DataValue* argLeft = m_retvals.pop();
    DataValue* argRight = m_retvals.pop();

    switch (expr.GetOperation())
    {
    case FdoBinaryOperations_Add: 
        m_retvals.push(argLeft->Add(*argRight, m_pPool));
        break;
    case FdoBinaryOperations_Multiply: 
        m_retvals.push(argLeft->Multiply(*argRight, m_pPool));
        break;
    case FdoBinaryOperations_Subtract: 
        m_retvals.push(argLeft->Subtract(*argRight, m_pPool));
        break;
    case FdoBinaryOperations_Divide: 
        m_retvals.push(argLeft->Divide(*argRight, m_pPool));
        break;
    default: 
        throw FdoException::Create(L"Unknown binary operation");
    }

    m_pPool->RelinquishDataValue(argLeft);
    m_pPool->RelinquishDataValue(argRight);
}

void FilterExecutor::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    FdoPtr<FdoExpression> right = expr.GetExpression();

    right->Process(this);

    DataValue* argRight = m_retvals.pop();

    switch (expr.GetOperation())
    {
    case FdoUnaryOperations_Negate: m_retvals.push(argRight->Negate(m_pPool));
        break;
    default : throw FdoException::Create(L"Unknown unary operation");
    }

}

void FilterExecutor::ProcessFunction(FdoFunction& expr)
{
    FdoString* name = expr.GetName();

    
    if (wcscmp(name, FDO_FUNCTION_CONCAT) == 0)
    {
        FdoPtr<FdoExpressionCollection> args = expr.GetArguments();

        if (args->GetCount() != 2)
            throw FdoException::Create(L"Invalid number of arguments.");

        FdoPtr<FdoExpression> left = args->GetItem(0);
        left->Process(this);

        FdoPtr<FdoExpression> right = args->GetItem(1);
        right->Process(this);

        DataValue* argRight = m_retvals.pop();

        DataValue* argLeft = m_retvals.pop();
		if( argLeft->GetType() == Dvt_Null || argRight->GetType() == Dvt_Null )
		{
			m_retvals.push( m_pPool->ObtainNullValue() );
		}
		else
		{
			size_t len = wcslen(argLeft->GetAsString()) + wcslen(argRight->GetAsString()) + 1;

			wchar_t* res = new wchar_t[len];
			res[0] = 0;

			wcscat(res, argLeft->GetAsString());
			wcscat(res, argRight->GetAsString());

			m_retvals.push(m_pPool->ObtainStringValue(res, true));
		}
    }
    else if (wcscmp(name, L"ARGB")==0)
    {
        return ExecuteARGB(expr);
    }
    else if (wcscmp (name, FDO_FUNCTION_CEIL) == 0 || wcscmp (name, FDO_FUNCTION_FLOOR) == 0)
    {
        FdoPtr<FdoExpressionCollection> args = expr.GetArguments ();

        if (args->GetCount () != 1)
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_75_INVALID_NUM_ARGUMENTS), expr.GetName(), 1, args->GetCount()));

        FdoPtr<FdoExpression> left = args->GetItem (0);
        left->Process (this);

        DataValue* argLeft = m_retvals.pop ();

		ProcessFunctionCeilFloor( name, argLeft );

		m_pPool->RelinquishDataValue(argLeft);
    }
    else if (wcscmp (name, FDO_FUNCTION_LOWER) == 0)
    {
        FdoPtr<FdoExpressionCollection> args = expr.GetArguments ();

        if (args->GetCount () != 1)
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_75_INVALID_NUM_ARGUMENTS), expr.GetName(), 1, args->GetCount()));

        FdoPtr<FdoExpression> left = args->GetItem (0);
        left->Process (this);

        DataValue* argLeft = m_retvals.pop ();

        if (FdoDataType_String != argLeft->GetType ())
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_88_INVALID_FUNCTION_ARG_TYPE), 1, name, FdoCommonMiscUtil::FdoDataTypeToString(argLeft->GetType()), FdoCommonMiscUtil::FdoDataTypeToString(FdoDataType_String)));

        if ( argLeft->GetType() == Dvt_Null  )
            m_retvals.push ( m_pPool->ObtainNullValue() );
        else
        {
            FdoString* arg = argLeft->GetAsString();
            size_t len = wcslen (arg) + 1;
            wchar_t* res = new wchar_t[len];
            wcscpy (res, arg);
            for (size_t i = 0; i < len; i++)
                res[i] = towlower (res[i]);
            m_retvals.push (m_pPool->ObtainStringValue (res,true) );
        }
		m_pPool->RelinquishDataValue(argLeft);
    }
    else if (wcscmp (name, FDO_FUNCTION_UPPER) == 0)
    {
        FdoPtr<FdoExpressionCollection> args = expr.GetArguments ();

        if (args->GetCount () != 1)
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_75_INVALID_NUM_ARGUMENTS), expr.GetName(), 1, args->GetCount()));

        FdoPtr<FdoExpression> left = args->GetItem (0);
        left->Process (this);

        DataValue* argLeft = m_retvals.pop ();

        if (FdoDataType_String != argLeft->GetType ())
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_88_INVALID_FUNCTION_ARG_TYPE), 1, name, FdoCommonMiscUtil::FdoDataTypeToString(argLeft->GetType()), FdoCommonMiscUtil::FdoDataTypeToString(FdoDataType_String)));

        if ( argLeft->GetType() == Dvt_Null )
            m_retvals.push ( m_pPool->ObtainNullValue() );
        else
        {
            FdoString* arg = argLeft->GetAsString();
            size_t len = wcslen (arg) + 1;
            wchar_t* res = new wchar_t[len];
            wcscpy (res, arg);
            for (size_t i = 0; i < len; i++)
                res[i] = towupper (res[i]);
            m_retvals.push (m_pPool->ObtainStringValue ( res, true ));
        }
		m_pPool->RelinquishDataValue(argLeft);
    }
    else
    {
        throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_89_UNSUPPORTED_FUNCTION), name));
    }
}

void FilterExecutor::ProcessFunctionCeilFloor( FdoString *name, DataValue* argLeft )
{
    FdoDataType		type = argLeft->GetType ();

	// TODO: Int16 and Int32 are handled by Int64Value. Therefore for those types the retuned type will be always FdoDataType_Int64
	//       We need to define a specific Value classes for these types.
    if ((FdoDataType_Double != type) && (FdoDataType_Single != type) && (FdoDataType_Decimal != type) && (FdoDataType_Int64 != type) &&
	    (FdoDataType_Int32 != type) && (FdoDataType_Int16 != type) )
	{
		FdoStringP	allowedTypes =	FdoStringP(FdoCommonMiscUtil::FdoDataTypeToString(FdoDataType_Double))  + L" / " +
									FdoStringP(FdoCommonMiscUtil::FdoDataTypeToString(FdoDataType_Single))  + L" / " +
									FdoStringP(FdoCommonMiscUtil::FdoDataTypeToString(FdoDataType_Decimal)) + L" / " +
									FdoStringP(FdoCommonMiscUtil::FdoDataTypeToString(FdoDataType_Int32))   + L" / " +
									FdoStringP(FdoCommonMiscUtil::FdoDataTypeToString(FdoDataType_Int16));

		throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_88_INVALID_FUNCTION_ARG_TYPE), 1, name, 
																FdoCommonMiscUtil::FdoDataTypeToString(type), 
	  														    (FdoString *)allowedTypes));
	}
	else
	{
		if ( argLeft->GetType() == Dvt_Null )
            m_retvals.push ( m_pPool->ObtainNullValue() );
        else
        {
            double d = 0.0;

            if (FdoDataType_Double == type || FdoDataType_Decimal == type)
                d = argLeft->GetAsDouble();
            else if ( FdoDataType_Single == type ) 
                d = argLeft->GetAsDouble();
			else if ( FdoDataType_Int32 == type || FdoDataType_Int64 == type ) 
				d = argLeft->GetAsInt64();
			else if ( FdoDataType_Int16 == type ) 
				d = argLeft->GetAsInt64();

			d = ( wcscmp (name, FDO_FUNCTION_CEIL) == 0) ? ceil (d) : floor (d);

            m_retvals.push (m_pPool->ObtainDoubleValue (d));
        }
	}
}

FdoPropertyDefinition* GetProperty(FdoClassDefinition* cls, FdoString* propName )
{
	FdoPropertyDefinition* prop = FdoPtr<FdoPropertyDefinitionCollection>(cls->GetProperties())->FindItem( propName );
	if( prop == NULL )
	{
		try
		{
			prop = FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(cls->GetBaseProperties())->GetItem( propName );
		}
		catch(FdoException *exp)
		{
			exp->Release();
			prop = NULL;
		}
	}
	return prop;
}

void FilterExecutor::ProcessIdentifier(FdoIdentifier& expr)
{
	int   length;
	const wchar_t** scope = expr.GetScope( length );
	if( length == 0 )
	{
		ProcessIdentifier(expr.GetName());
		return;
	}
	// else it must be an association based filter
	FdoPtr<FdoPropertyDefinition>prop = GetProperty( m_classDefinition, scope[0] );
	if( prop->GetPropertyType() != FdoPropertyType_AssociationProperty )
		throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_68_DATATYPE_NOT_SUPPORTED), expr.GetName() ));

	FdoPtr<FdoClassDefinition>assoc_class = ((FdoAssociationPropertyDefinition*)prop.p)->GetAssociatedClass();
	FdoPtr<FdoIFeatureReader>reader = m_reader->GetFeatureObject( scope[0] );
	for(int i=1; i<length; i++ )
	{
		if( reader == NULL || ! reader->ReadNext() )
			break;
		prop = GetProperty( assoc_class, scope[i] );
		if( prop->GetPropertyType() != FdoPropertyType_AssociationProperty )
			return;
		FdoPtr<FdoClassDefinition>assoc_class = ((FdoAssociationPropertyDefinition*)prop.p)->GetAssociatedClass();

		reader = reader->GetFeatureObject( scope[i] );	
	}

	if( reader != NULL && reader->ReadNext() )
	{
		prop = GetProperty( assoc_class, expr.GetName() );

		if( prop->GetPropertyType() == FdoPropertyType_DataProperty )
			PushIdentifierValue( reader, expr.GetName(), ((FdoDataPropertyDefinition*)prop.p)->GetDataType() );
	}
}

void FilterExecutor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    //Computed identifiers:
    //Just evaluate the expression that the computed identifier represents.

    FdoPtr<FdoExpression> theexpr = expr.GetExpression();
    theexpr->Process(this);
}

void FilterExecutor::PushIdentifierValue(FdoIFeatureReader* reader, FdoString* name, FdoDataType type )
{
	switch (type)
    {
		case FdoDataType_Boolean : 
			m_retvals.push(m_pPool->ObtainBooleanValue(reader->GetBoolean(name))); 
			break;
		case FdoDataType_Byte : 
			m_retvals.push(m_pPool->ObtainInt64Value(reader->GetByte(name))); 
			break;
		case FdoDataType_DateTime : 
			m_retvals.push(m_pPool->ObtainDateTimeValue(reader->GetDateTime(name))); 
			break;
		case FdoDataType_Decimal : 
			m_retvals.push(m_pPool->ObtainDoubleValue(reader->GetDouble(name))); 
			break;
		case FdoDataType_Double : 
			m_retvals.push(m_pPool->ObtainDoubleValue(reader->GetDouble(name))); 
			break;
		case FdoDataType_Int16 : 
			m_retvals.push(m_pPool->ObtainInt64Value(reader->GetInt16(name))); 
			break;
		case FdoDataType_Int32 : 
			m_retvals.push(m_pPool->ObtainInt64Value(reader->GetInt32(name))); 
			break;
		case FdoDataType_Int64 : 
			m_retvals.push(m_pPool->ObtainInt64Value(reader->GetInt64(name))); 
			break;
		case FdoDataType_Single : 
			m_retvals.push(m_pPool->ObtainDoubleValue(reader->GetSingle(name))); 
			break;
		case FdoDataType_String : 
			{
			wchar_t* val = NULL;
			wchar_t* str = (wchar_t*)reader->GetString(name);
			if( str )
			{
				val = new wchar_t[wcslen(str)+1];
				wcscpy(val,str);
			}
			m_retvals.push(m_pPool->ObtainStringValue(val, true)); 
			}
			break;
		default: 
			throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_68_DATATYPE_NOT_SUPPORTED), name ));
    }
}

void FilterExecutor::ProcessIdentifier(FdoString* name)
{
    //get property info. It will be NULL if it is computed identifier
    PropertyStub* ps = m_propIndex->GetPropInfo(name);

    if (ps)
    {
		if( m_reader->IsNull(name) )
			m_retvals.push(m_pPool->ObtainNullValue() );
		else
		{
			switch (ps->m_dataType)
			{
			case FdoDataType_Boolean : 
				m_retvals.push(m_pPool->ObtainBooleanValue(m_reader->GetBoolean(name))); 
				break;
			case FdoDataType_Byte : 
				m_retvals.push(m_pPool->ObtainInt64Value(m_reader->GetByte(name))); 
				break;
			case FdoDataType_DateTime : 
				m_retvals.push(m_pPool->ObtainDateTimeValue(m_reader->GetDateTime(name))); 
				break;
			case FdoDataType_Decimal : 
				m_retvals.push(m_pPool->ObtainDoubleValue(m_reader->GetDouble(name))); 
				break;
			case FdoDataType_Double : 
				m_retvals.push(m_pPool->ObtainDoubleValue(m_reader->GetDouble(name))); 
				break;
			case FdoDataType_Int16 : 
				m_retvals.push(m_pPool->ObtainInt64Value(m_reader->GetInt16(name))); 
				break;
			case FdoDataType_Int32 : 
				m_retvals.push(m_pPool->ObtainInt64Value(m_reader->GetInt32(name))); 
				break;
			case FdoDataType_Int64 : 
				m_retvals.push(m_pPool->ObtainInt64Value(m_reader->GetInt64(name))); 
				break;
			case FdoDataType_Single : 
				m_retvals.push(m_pPool->ObtainDoubleValue(m_reader->GetSingle(name))); 
				break;
			case FdoDataType_String : 
				m_retvals.push(m_pPool->ObtainStringValue((wchar_t*)m_reader->GetString(name))); 
				break;
			default: 
				throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_68_DATATYPE_NOT_SUPPORTED), name ));
			}
		}
    }
    else
    {
        //if user tried to use a computed identifier in an expression
        //we need some special handling
        //TODO: is dynamic cast necessary here?
        FdoPtr<FdoComputedIdentifier> cid = (FdoComputedIdentifier*)(m_compIdents->GetItem(name));
        cid->Process(this);
    }
}

void FilterExecutor::ProcessParameter(FdoParameter& expr)
{
    throw FdoException::Create(L"Parameters not supported");
}

void FilterExecutor::ProcessBooleanValue(FdoBooleanValue& expr)
{
    m_retvals.push(m_pPool->ObtainBooleanValue(expr.GetBoolean()));
}

void FilterExecutor::ProcessByteValue(FdoByteValue& expr)
{
    m_retvals.push(m_pPool->ObtainInt64Value(expr.GetByte()));
}

void FilterExecutor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    m_retvals.push(m_pPool->ObtainDateTimeValue(expr.GetDateTime()));
}

void FilterExecutor::ProcessDecimalValue(FdoDecimalValue& expr)
{
    m_retvals.push(m_pPool->ObtainDoubleValue(expr.GetDecimal()));
}

void FilterExecutor::ProcessDoubleValue(FdoDoubleValue& expr)
{
    m_retvals.push(m_pPool->ObtainDoubleValue(expr.GetDouble()));
}

void FilterExecutor::ProcessInt16Value(FdoInt16Value& expr)
{
    m_retvals.push(m_pPool->ObtainInt64Value(expr.GetInt16()));
}

void FilterExecutor::ProcessInt32Value(FdoInt32Value& expr)
{
    m_retvals.push(m_pPool->ObtainInt64Value(expr.GetInt32()));
}

void FilterExecutor::ProcessInt64Value(FdoInt64Value& expr)
{
    m_retvals.push(m_pPool->ObtainInt64Value(expr.GetInt64()));
}

void FilterExecutor::ProcessSingleValue(FdoSingleValue& expr)
{
    m_retvals.push(m_pPool->ObtainDoubleValue(expr.GetSingle()));
}

void FilterExecutor::ProcessStringValue(FdoStringValue& expr)
{
    m_retvals.push(m_pPool->ObtainStringValue((wchar_t*)expr.GetString()));
}

void FilterExecutor::ProcessBLOBValue(FdoBLOBValue& expr)
{
    throw FdoException::Create(L"BLOBs not supported");
}

void FilterExecutor::ProcessCLOBValue(FdoCLOBValue& expr)
{
    throw FdoException::Create(L"CLOBs not supported");
}

void FilterExecutor::ProcessGeometryValue(FdoGeometryValue& expr)
{
    //m_tokens.push(new GeometryVal());
    throw FdoException::Create(L"Geometry not supported");
}


//Matches strings according to the rules of the LIKE condition
// Wildcard characters supported in the pattern are:
//
// %    Any string of zero or more characters
// _    Any single character
// []   Any single character within the specified range ([a-f]) or set 
// ([abcdef]).
// [^]  Any single character not within the specified range ([^a-f]) or 
// set ([^abcdef]).
bool FilterExecutor::MatchesHere(wchar_t* pattern, wchar_t* src)
{
	bool bMatches = FALSE;

	if( src == NULL )
		return bMatches;

	if(*pattern == L'\0')
	{
		if(wcslen(src))
		{
			bMatches = false;
		}
		else
		{
			bMatches = true;
		}
	}
	else if(*pattern == L'%')
	{
		bMatches = MatchPercent(pattern+1, src);
	}
    else if(*pattern == L'[')
    {
        bMatches = MatchBracket(pattern+1, src);
    }
	else if(*src != L'\0' && ((*pattern == L'_') || wcsncmp(pattern, src, 1) == 0))
	{
		bMatches = MatchesHere(pattern+1, src+1);
	}

	return bMatches;
}

// %    Any string of zero or more characters
bool FilterExecutor::MatchPercent(wchar_t* pattern, wchar_t* src)
{
	bool bMatches = false;
	
	while(true)
	{
		if(MatchesHere(pattern, src))
		{
			bMatches = true;
			break;
		}
		if(*src == L'\0')
		{
			break;
		}
		src++;
	}

	return bMatches;
}

// []   Any single character within the specified range ([a-f]) or set 
// ([abcdef]).
// [^]  Any single character not within the specified range ([^a-f]) or 
// set ([^abcdef]).
bool FilterExecutor::MatchBracket(wchar_t* pattern, wchar_t* src)
{
    bool bMatches = false;
    bool reverse = false;

    //invalid case, unclosed bracket
    if (*pattern == L'\0')
        bMatches = false;

    if (*pattern == L'^')
    {
        reverse = true;
        pattern++;
    }

    //case of [] -- not a useful case
    if (*pattern == L']')
    {
        if (*src == L'\0')
            bMatches = true;

        if (reverse)
            bMatches = MatchesHere(pattern+1, src+1);

        bMatches = false;
    }

    wchar_t start = *pattern++;

    //case of [a-b] or [^a-b]
    if (*pattern == L'-')
    {
        //go past - character and get range end character and go past range end character
        pattern++;
        wchar_t end = *pattern++;

        _ASSERT(end != NULL);

        if ((*src >= start && *src <= end) ^ reverse)
        {
            bMatches = MatchesHere(pattern+1, src+1);
        }
        else
            bMatches = false;
    }

    //case of [abcdef] ot [^abcdef]
    std::vector<wchar_t> set;
    set.push_back(start);

    do 
    {
        set.push_back(*pattern++);
    }
    while (*pattern != L']');

    wchar_t srcchar = *src;
    bool isthere = false;

    //look for current character in the set
    for (unsigned i=0; i<set.size(); i++)
    {
        wchar_t current = set[i];
        if (wcsncmp(&current, &srcchar, 1) == 0)
        {
            isthere = true;
            break;
        }
    }

    if (isthere ^ reverse)
        bMatches = MatchesHere(pattern+1, src+1);
    else
        bMatches = false;


    return bMatches;
}

//not used for SDF. Mapguide Server Mapping Service only.
void FilterExecutor::ExecuteARGB(FdoFunction& function)
{
    FdoPtr<FdoExpressionCollection> args = function.GetArguments();

    int count = args->GetCount();

    //make sure we have at least 3 and at most 4 arguments.
    //if there are 3, they are treated as R,G,B with full alpha (255)
    //otherwise they are treated as R,G,B,A
    _ASSERT(count == 4);

    //evaluate each argument
    for (int i=count-1; i>=0; i--)
    {
        FdoPtr<FdoExpression> arg = args->GetItem(i);
        arg->Process(this);
    }

    DataValue* dvAlpha = m_retvals.pop();
    FdoInt64 alpha = (int)dvAlpha->GetAsInt64();
    m_pPool->RelinquishDataValue(dvAlpha);

    DataValue* dvRed = m_retvals.pop();
    FdoInt64 red = dvRed->GetAsInt64();
    m_pPool->RelinquishDataValue(dvRed);

    DataValue* dvGreen = m_retvals.pop();
    FdoInt64 green = dvGreen->GetAsInt64();
    m_pPool->RelinquishDataValue(dvGreen);

    DataValue* dvBlue = m_retvals.pop();
    FdoInt64 blue = dvBlue->GetAsInt64();
    m_pPool->RelinquishDataValue(dvBlue);

    alpha &= 0xFF;
    red &= 0xFF;
    green &= 0xFF;
    blue &= 0xFF;
        
    FdoInt64 color = (alpha << 24) | (red << 16) | (green << 8) | blue;

    m_retvals.push(m_pPool->ObtainInt64Value(color));
}

// Returns NULL in case geometry was not reconstructed
FdoIGeometry *FilterExecutor::ReconstructPolygon( FdoIGeometry *geometry )
{
	FdoPtr<FdoIGeometry> newGeometry;

	if ( geometry->GetDerivedType() == FdoGeometryType_Polygon )
	{
		FdoIPolygon * poly = (FdoIPolygon *)geometry;
		if ( poly->GetInteriorRingCount() != 0 )
		{
			FdoPtr<FdoLinearRingCollection> rings = FdoLinearRingCollection::Create ();
			for (int i = 0; i < poly->GetInteriorRingCount(); i++)
			{
				FdoPtr<FdoILinearRing>	ring = poly->GetInteriorRing(i);
				rings->Add(ring);
			}
			newGeometry = FdoSpatialUtility::CreateGeometryFromRings (rings, true);
		}
	}
	return FDO_SAFE_ADDREF(newGeometry.p);
}


