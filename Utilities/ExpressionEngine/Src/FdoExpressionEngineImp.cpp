/*
 * 
* Copyright (C) 2004-2007  Autodesk, Inc.
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

#include <stdafx.h>
#include <FdoExpressionEngineImp.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonMiscUtil.h>

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

#include <wctype.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>

#include <FdoExpressionEngineIAggregateFunction.h>
#include <FdoCommonDataReader.h>

// None of the functions are yet implemented
const int NUMBER_FUNCTIONS = 0;

class InitializeClass
{

public:
	FdoExpressionEngineIFunction *m_Functions[NUMBER_FUNCTIONS+1];
	InitializeClass()
	{
		int i=0;
		//m_Functions[i++] = FdoFunctionMax::Create();
		//m_Functions[i++] = FdoFunctionConcat::Create();
		m_Functions[i++] = NULL;
	}

	~InitializeClass()
	{
		for (int i=0; m_Functions[i] != NULL; i++)
		{
			m_Functions[i]->Release();
		};
	};

	FdoExpressionEngineIFunction** GetStandardFunctions()
	{
		return m_Functions;
	}
};

InitializeClass init;

FdoExpressionEngineImp* FdoExpressionEngineImp::Create(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* identifiers,
		FdoExpressionEngineFunctionCollection *userDefinedFunctions)
{
	return new FdoExpressionEngineImp(reader, classDef, identifiers, userDefinedFunctions);
}


FdoExpressionEngineImp::FdoExpressionEngineImp(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* compIdents,
		FdoExpressionEngineFunctionCollection *userDefinedFunctions)
{

	m_reader = reader;  // NOTE: weak reference
	m_classDefinition = FDO_SAFE_ADDREF(classDef);

    m_compIdents = FDO_SAFE_ADDREF(compIdents);

	m_propIndex = new FdoCommonPropertyIndex(classDef, 0);

    m_AllFunctions = FdoFunctionDefinitionCollection::Create();

    if (userDefinedFunctions)
    {
        for (int i=0; i<userDefinedFunctions->GetCount(); i++)
        {
            FdoPtr<FdoExpressionEngineIFunction> userDefinedFunction = userDefinedFunctions->GetItem(i);
            FdoPtr<FdoFunctionDefinition> function = userDefinedFunction->GetFunctionDefinition();
            m_AllFunctions->Add(function);
        }
    }

    FdoExpressionEngineIFunction** functions = init.GetStandardFunctions();
    for (int i=0; functions[i] != NULL; i++)
    {
        FdoPtr<FdoFunctionDefinition> function = functions[i]->GetFunctionDefinition();
        m_AllFunctions->Add(function);
    }

	m_UserDefinedFunctions = FDO_SAFE_ADDREF(userDefinedFunctions);

	m_Size = 10;
	m_Current = 0;
	m_CacheFunc = (FunctionCache *) malloc(m_Size*sizeof(FunctionCache));
    m_CurrentIndex = 0;
}

FdoExpressionEngineImp::FdoExpressionEngineImp()
{
}

bool FdoExpressionEngineImp::IsAggregateFunction(FdoFunctionDefinitionCollection *funcDefs, FdoString *name)
{

	bool ret = false;
	for(int i=0; i<funcDefs->GetCount(); i++)
	{
		FdoPtr<FdoFunctionDefinition> funcDef = funcDefs->GetItem(i);
		if (wcscmp(name, funcDef->GetName()) == 0)
		{
			ret = funcDef->IsAggregate();
			break;
		}
	}
	return ret;
}



FdoExpressionEngineImp::~FdoExpressionEngineImp()
{
	m_propIndex->Release();
	for (int i=0; i<m_Current; i++)
	{
		FDO_SAFE_RELEASE(m_CacheFunc[i].function);
	}
	free(m_CacheFunc);

    for (retval_stack::iterator iter = m_retvals.begin (); iter != m_retvals.end (); iter++)
        delete *iter;
    for (std::vector<FdoBooleanValue*>::iterator i = mBooleanPool.begin (); i != mBooleanPool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoByteValue*>::iterator i = mBytePool.begin (); i != mBytePool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoDateTimeValue*>::iterator i = mDateTimePool.begin (); i != mDateTimePool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoDecimalValue*>::iterator i = mDecimalPool.begin (); i != mDecimalPool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoDoubleValue*>::iterator i = mDoublePool.begin (); i != mDoublePool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoInt16Value*>::iterator i = mInt16Pool.begin (); i != mInt16Pool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoInt32Value*>::iterator i = mInt32Pool.begin (); i != mInt32Pool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoInt64Value*>::iterator i = mInt64Pool.begin (); i != mInt64Pool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoSingleValue*>::iterator i = mSinglePool.begin (); i != mSinglePool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoStringValue*>::iterator i = mStringPool.begin (); i != mStringPool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoBLOBValue*>::iterator i = mBLOBPool.begin (); i != mBLOBPool.end (); i++)
        (*i)->Release ();
    for (std::vector<FdoCLOBValue*>::iterator i = mCLOBPool.begin (); i != mCLOBPool.end (); i++)
        (*i)->Release ();
}

void FdoExpressionEngineImp::Dispose ()
{
    delete this;
}


FdoCommonPropertyStub* FdoExpressionEngineImp::GetPropInfo(FdoString* name)
{
	FdoCommonPropertyStub* stub = m_propIndex->GetPropInfo(name);
    return (stub);
}



void FdoExpressionEngineImp::Reset ()
{
    for (retval_stack::iterator iter = m_retvals.begin (); iter != m_retvals.end (); iter++)
        RelinquishDataValue (*iter);

    m_retvals.clear ();
}

//returns the data type of the result
//of the expression/filter evaluation.
//For filters it is always boolean
FdoDataType FdoExpressionEngineImp::GetResultDataType ()
{
    if (GetResultPropertyType() != FdoPropertyType_DataProperty)
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    return (static_cast<FdoDataValue*>(m_retvals.back ())->GetDataType ());
}

FdoPropertyType FdoExpressionEngineImp::GetResultPropertyType ()
{
    if ( dynamic_cast<FdoDataValue*>(m_retvals.back ()) != NULL )
        return FdoPropertyType_DataProperty;
    else if ( dynamic_cast<FdoGeometryValue*>(m_retvals.back ()) != NULL )
        return FdoPropertyType_GeometricProperty;
    else
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
}

bool FdoExpressionEngineImp::IsResultNull ()
{
    FdoPropertyType propType = GetResultPropertyType();
    if (propType == FdoPropertyType_DataProperty)
        return ((FdoDataValue*)m_retvals.back())->IsNull();
    else if (propType == FdoPropertyType_GeometricProperty)
        return ((FdoGeometryValue*)m_retvals.back())->IsNull();
    else
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR)));
}


bool FdoExpressionEngineImp::GetBooleanResult (bool &bIsNull)
{
    bool ret = false;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_Boolean != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoBooleanValue*)dv)->GetBoolean ();

    RelinquishDataValue (dv);
    
    return (ret);
}


double FdoExpressionEngineImp::GetDoubleResult (bool &bIsNull)
{
    double ret = 0.0;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_Double != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoDoubleValue*)dv)->GetDouble ();

    RelinquishDataValue (dv);
    
    return (ret);
}

float FdoExpressionEngineImp::GetSingleResult (bool &bIsNull)
{
    float ret = 0.0f;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_Single != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoSingleValue*)dv)->GetSingle ();

    RelinquishDataValue (dv);
    
    return (ret);
}

double FdoExpressionEngineImp::GetDecimalResult (bool &bIsNull)
{
    double ret = 0.0;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_Decimal != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoDecimalValue*)dv)->GetDecimal ();

    RelinquishDataValue (dv);
    
    return (ret);
}

FdoByte FdoExpressionEngineImp::GetByteResult (bool &bIsNull)
{
    FdoByte ret = 0;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_Byte != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoByteValue*)dv)->GetByte ();

    RelinquishDataValue (dv);
    
    return (ret);
}

FdoInt16 FdoExpressionEngineImp::GetInt16Result (bool &bIsNull)
{
    FdoInt16 ret = 0;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_Int16 != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoInt16Value*)dv)->GetInt16 ();

    RelinquishDataValue (dv);
    
    return (ret);
}

FdoInt32 FdoExpressionEngineImp::GetInt32Result (bool &bIsNull)
{
    FdoInt32 ret = 0;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_Int32 != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoInt32Value*)dv)->GetInt32 ();

    RelinquishDataValue (dv);
    
    return (ret);
}

FdoInt64 FdoExpressionEngineImp::GetInt64Result (bool &bIsNull)
{
    FdoInt64 ret = 0;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_Int64 != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoInt64Value*)dv)->GetInt64 ();

    RelinquishDataValue (dv);
    
    return (ret);
}

const wchar_t* FdoExpressionEngineImp::GetStringResult (bool &bIsNull)
{
    const wchar_t* ret = NULL;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_String != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoStringValue*)dv)->GetString ();

    RelinquishDataValue (dv);
    
    return (ret);
}

FdoDateTime FdoExpressionEngineImp::GetDateTimeResult (bool &bIsNull)
{
    FdoDateTime ret;

    if (FdoPropertyType_DataProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (FdoDataType_DateTime != dv->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    bIsNull = dv->IsNull();
    if (!bIsNull)
        ret = ((FdoDateTimeValue*)dv)->GetDateTime ();

    RelinquishDataValue (dv);
    
    return (ret);
}

FdoGeometryValue* FdoExpressionEngineImp::GetGeometricResult (bool &bIsNull)
{
    FdoGeometryValue* ret = NULL;

    if (FdoPropertyType_GeometricProperty != GetResultPropertyType())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoGeometryValue* gv = (FdoGeometryValue*)m_retvals.back ();
    m_retvals.pop_back ();

    bIsNull = gv->IsNull();
    ret = gv;

    //We dont (yet) cache geometry values:
    //RelinquishDataValue (dv);
    
    return (ret);
}

FdoBooleanValue* FdoExpressionEngineImp::ObtainBooleanValue (bool bIsNull, bool value)
{
    FdoBooleanValue* ret;

    if (0 == mBooleanPool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoBooleanValue::Create();  // defaults to NULL
        else
            ret = FdoBooleanValue::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mBooleanPool.back ();
        mBooleanPool.pop_back ();
        if (bIsNull)
            ret->SetNull();
        else
            ret->SetBoolean (value);
    }

    return (ret);
}

FdoByteValue* FdoExpressionEngineImp::ObtainByteValue (bool bIsNull, FdoByte value)
{
    FdoByteValue* ret;

    if (0 == mBytePool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoByteValue::Create (); // defaults to NULL
        else
            ret = FdoByteValue::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mBytePool.back ();
        mBytePool.pop_back ();
        if (bIsNull)
            ret->SetNull();
        else
            ret->SetByte (value);
    }

    return (ret);
}

FdoDateTimeValue* FdoExpressionEngineImp::ObtainDateTimeValue (bool bIsNull, FdoDateTime value)
{
    FdoDateTimeValue* ret;

    if (0 == mDateTimePool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoDateTimeValue::Create ();  // defaults to NULL
        else
            ret = FdoDateTimeValue::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mDateTimePool.back ();
        mDateTimePool.pop_back ();
        if (bIsNull)
            ret->SetNull();
        else
            ret->SetDateTime (value);
    }

    return (ret);
}

FdoDecimalValue* FdoExpressionEngineImp::ObtainDecimalValue (bool bIsNull, double value)
{
    FdoDecimalValue* ret;

    if (0 == mDecimalPool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoDecimalValue::Create ();  // defaults to NULL
        else
            ret = FdoDecimalValue::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mDecimalPool.back ();
        mDecimalPool.pop_back ();
        if (bIsNull)
            ret->SetNull();
        else
            ret->SetDecimal (value);
    }

    return (ret);
}

FdoDoubleValue* FdoExpressionEngineImp::ObtainDoubleValue (bool bIsNull, double value)
{
    FdoDoubleValue* ret;

    if (0 == mDoublePool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoDoubleValue::Create ();  // defaults to NULL
        else
            ret = FdoDoubleValue::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mDoublePool.back ();
        mDoublePool.pop_back ();
        if (bIsNull)
            ret->SetNull ();
        else
            ret->SetDouble (value);
    }

    return (ret);
}

FdoInt16Value* FdoExpressionEngineImp::ObtainInt16Value (bool bIsNull, FdoInt16 value)
{
    FdoInt16Value* ret;

    if (0 == mInt16Pool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoInt16Value::Create ();  // defaults to NULL
        else
            ret = FdoInt16Value::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mInt16Pool.back ();
        mInt16Pool.pop_back ();
        if (bIsNull)
            ret->SetNull ();
        else
            ret->SetInt16 (value);
    }

    return (ret);
}

FdoInt32Value* FdoExpressionEngineImp::ObtainInt32Value (bool bIsNull, FdoInt32 value)
{
    FdoInt32Value* ret;

    if (0 == mInt32Pool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoInt32Value::Create ();  // defaults to NULL
        else
            ret = FdoInt32Value::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mInt32Pool.back ();
        mInt32Pool.pop_back ();
        if (bIsNull)
            ret->SetNull ();
        else
            ret->SetInt32 (value);
    }

    return (ret);
}

FdoInt64Value* FdoExpressionEngineImp::ObtainInt64Value (bool bIsNull, FdoInt64 value)
{
    FdoInt64Value* ret;

    if (0 == mInt64Pool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoInt64Value::Create ();  // defaults to NULL
        else
            ret = FdoInt64Value::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mInt64Pool.back ();
        mInt64Pool.pop_back ();
        if (bIsNull)
            ret->SetNull ();
        else
            ret->SetInt64 (value);
    }

    return (ret);
}

FdoSingleValue* FdoExpressionEngineImp::ObtainSingleValue (bool bIsNull, float value)
{
    FdoSingleValue* ret;

    if (0 == mSinglePool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoSingleValue::Create ();  // defaults to NULL
        else
            ret = FdoSingleValue::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mSinglePool.back ();
        mSinglePool.pop_back ();
        if (bIsNull)
            ret->SetNull ();
        else
            ret->SetSingle (value);
    }

    return (ret);
}

FdoStringValue* FdoExpressionEngineImp::ObtainStringValue (bool bIsNull, FdoString* value)
{
    FdoStringValue* ret;

    if (0 == mStringPool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoStringValue::Create ();  // defaults to NULL
        else
            ret = FdoStringValue::Create (value);
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mStringPool.back ();
        mStringPool.pop_back ();
        if (bIsNull)
            ret->SetNull ();
        else
            ret->SetString (value);
    }

    return (ret);
}

FdoBLOBValue* FdoExpressionEngineImp::ObtainBLOBValue (bool bIsNull, FdoByteArray* value)
{
    FdoBLOBValue* ret;

    if (0 == mBLOBPool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoBLOBValue::Create ();  // defaults to NULL;
        else
            ret = FdoBLOBValue::Create (value);  //TODO: there is probable a memory leak here
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mBLOBPool.back ();
        mBLOBPool.pop_back ();
        if (bIsNull)
            ret->SetNull ();
        else
            ret->SetData (value);
    }

    return (ret);
}

FdoCLOBValue* FdoExpressionEngineImp::ObtainCLOBValue (bool bIsNull, FdoByteArray* value)
{
    FdoCLOBValue* ret;

    if (0 == mCLOBPool.size ())
    {
        // if the pool is empty, create new
        if (bIsNull)
            ret = FdoCLOBValue::Create ();  // defaults to NULL
        else
            ret = FdoCLOBValue::Create (value);  //TODO: there is probable a memory leak here
    }
    else
    {  // otherwise get an object from the pool and initialize it
        ret = mCLOBPool.back ();
        mCLOBPool.pop_back ();
        if (bIsNull)
            ret->SetNull ();
        else
            ret->SetData (value);
    }

    return (ret);
}

FdoGeometryValue* FdoExpressionEngineImp::ObtainGeometryValue (bool bIsNull, FdoByteArray* value)
{
    FdoGeometryValue* ret;

    // No geometry pooling (yet)
    if (bIsNull)
        ret = FdoGeometryValue::Create ();
    else
        ret = FdoGeometryValue::Create (value);

    return ret;
}

void FdoExpressionEngineImp::RelinquishDataValue (FdoLiteralValue* data)
{
    if (data->GetLiteralValueType() == FdoLiteralValueType_Data)
    {
        switch (((FdoDataValue*)data)->GetDataType ())
        {
            case FdoDataType_Boolean:
                mBooleanPool.push_back ((FdoBooleanValue*)data);
                break;
            case FdoDataType_Byte:
                mBytePool.push_back ((FdoByteValue*)data);
                break;
            case FdoDataType_DateTime:
                mDateTimePool.push_back ((FdoDateTimeValue*)data);
                break;
            case FdoDataType_Decimal:
                mDecimalPool.push_back ((FdoDecimalValue*)data);
                break;
            case FdoDataType_Double:
                mDoublePool.push_back ((FdoDoubleValue*)data);
                break;
            case FdoDataType_Int16:
                mInt16Pool.push_back ((FdoInt16Value*)data);
                break;
            case FdoDataType_Int32:
                mInt32Pool.push_back ((FdoInt32Value*)data);
                break;
            case FdoDataType_Int64:
                mInt64Pool.push_back ((FdoInt64Value*)data);
                break;
            case FdoDataType_Single:
                mSinglePool.push_back ((FdoSingleValue*)data);
                break;
            case FdoDataType_String:
                mStringPool.push_back ((FdoStringValue*)data);
                break;
            case FdoDataType_BLOB:
                mBLOBPool.push_back ((FdoBLOBValue*)data);
                break;
            case FdoDataType_CLOB:
                mCLOBPool.push_back ((FdoCLOBValue*)data);
                break;
            default:
                data->Release ();
        }
    }
    else if (data->GetLiteralValueType() == FdoLiteralValueType_Geometry)
    {
        // nothing to do since we dont pool geometry values (yet)
    }
    else
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR)));
}

void FdoExpressionEngineImp::ProcessBinaryLogicalOperator (FdoBinaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> left = filter.GetLeftOperand ();
    FdoPtr<FdoFilter> right = filter.GetRightOperand ();

    //evaluate left hand side
    left->Process (this);
    bool bIsNull;
    bool argLeft = GetBooleanResult (bIsNull);
    if (bIsNull)
    {
        m_retvals.push_back (ObtainBooleanValue (true, false));
        return;
    }

    // optimize binary logic by looking at the value of the left operand and see
    // if we have to evaluate the right one also
    if (argLeft && filter.GetOperation () == FdoBinaryLogicalOperations_Or)
    {
        m_retvals.push_back (ObtainBooleanValue (false, true));
        return;
    }
    else if (!argLeft && filter.GetOperation () == FdoBinaryLogicalOperations_And)
    {
        m_retvals.push_back (ObtainBooleanValue (false, false));
        return;
    }
    
    // evaluate right hand side
    right->Process (this);
    bool argRight = GetBooleanResult (bIsNull);
    if (bIsNull)
        m_retvals.push_back (ObtainBooleanValue (true, false));
    else switch (filter.GetOperation ())
    {
        case FdoBinaryLogicalOperations_And: 
            m_retvals.push_back (ObtainBooleanValue (false, argLeft && argRight)); 
            break;
        case FdoBinaryLogicalOperations_Or : 
            m_retvals.push_back (ObtainBooleanValue (false, argLeft || argRight));
            break;
        default: 
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_82_UNSUPPORTED_LOGICAL_OPERATION)));
            break;
    }
}

void FdoExpressionEngineImp::ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> right = filter.GetOperand ();

    // evaluate right hand side
    right->Process (this);

    // get Process() results from return value stack
    bool bIsNull;
    bool argRight = GetBooleanResult (bIsNull);

    switch (filter.GetOperation ())
    {
        case FdoUnaryLogicalOperations_Not: 
            m_retvals.push_back (ObtainBooleanValue (bIsNull, bIsNull ? false : !argRight)); 
            break;
        default: 
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_82_UNSUPPORTED_LOGICAL_OPERATION)));break;
    }
}

void FdoExpressionEngineImp::ProcessComparisonCondition (FdoComparisonCondition& filter)
{
    FdoPtr<FdoExpression> left = filter.GetLeftExpression ();
    FdoPtr<FdoExpression> right = filter.GetRightExpression ();

    right->Process (this);
    left->Process (this);
    
    //get Process() results from return value stack
    FdoDataValue* argLeft = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();
    FdoDataValue* argRight = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    if (argLeft->IsNull() || argRight->IsNull())
        m_retvals.push_back (ObtainBooleanValue (true, false));
    else switch (filter.GetOperation ())
    {
        case FdoComparisonOperations_EqualTo : 
            m_retvals.push_back (ObtainBooleanValue (false, IsEqualTo (argLeft, argRight))); 
            break;
        case FdoComparisonOperations_NotEqualTo : 
            m_retvals.push_back (ObtainBooleanValue (false, !IsEqualTo (argLeft, argRight))); 
            break;
        case FdoComparisonOperations_GreaterThan : 
            m_retvals.push_back (ObtainBooleanValue (false, IsGreaterThan (argLeft, argRight))); 
            break;
        case FdoComparisonOperations_GreaterThanOrEqualTo : 
            m_retvals.push_back (ObtainBooleanValue (false, !IsLessThan (argLeft, argRight))); 
            break;
        case FdoComparisonOperations_LessThan : 
            m_retvals.push_back (ObtainBooleanValue (false, IsLessThan (argLeft, argRight))); 
            break;
        case FdoComparisonOperations_LessThanOrEqualTo : 
            m_retvals.push_back (ObtainBooleanValue (false, !IsGreaterThan (argLeft, argRight))); 
            break;
        case FdoComparisonOperations_Like :
            m_retvals.push_back (ObtainBooleanValue (false, Like (argLeft, argRight)));
            break;    
        default:
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_83_UNSUPPORTED_COMPARISON_OPERATION)));
    }

    RelinquishDataValue (argRight);
    RelinquishDataValue (argLeft);
}

void FdoExpressionEngineImp::ProcessInCondition (FdoInCondition& filter)
{
    //first get the value of the property we are checking
    FdoPtr<FdoIdentifier> prop = filter.GetPropertyName ();
    ProcessIdentifier (*(prop.p));

    //get the property value off the stack
    FdoDataValue* argLeft = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    FdoPtr<FdoValueExpressionCollection> vals = filter.GetValues ();

    bool result = false;

    //see if the value collection contains the property value
    FdoInt32 count = vals->GetCount ();
    for (int i = 0; !result && (i < count); i++)
    {
        FdoPtr<FdoValueExpression> expr = vals->GetItem (i);
        expr->Process (this);

        FdoDataValue* argRight = (FdoDataValue*)m_retvals.back ();
        m_retvals.pop_back ();

        if (IsEqualTo (argLeft, argRight))
            result = true;

        RelinquishDataValue (argRight);

        if (result)
            break;
    }

    //if prop val is not IN the val collection, push false on the stack
    m_retvals.push_back (ObtainBooleanValue (false, result));

    RelinquishDataValue (argLeft);
}

void FdoExpressionEngineImp::ProcessNullCondition (FdoNullCondition& filter)
{
    //first get the value of the property we are checking
    FdoPtr<FdoIdentifier> prop = filter.GetPropertyName ();
    
    bool isNull = m_reader->IsNull (prop->GetName ());

    m_retvals.push_back (ObtainBooleanValue (false, isNull));
}

void FdoExpressionEngineImp::ProcessSpatialCondition (FdoSpatialCondition& filter)
{
    FdoPtr<FdoByteArray> fgf = m_reader->GetGeometry (
        FdoPtr<FdoIdentifier>(filter.GetPropertyName ())->GetName ());

    //no geometry? trivially false.
    if (fgf->GetCount () == 0)
        m_retvals.push_back (ObtainBooleanValue (false, false));

    FdoPtr<FdoExpression> exprRight = filter.GetGeometry ();
    FdoGeometryValue* gvRight = dynamic_cast<FdoGeometryValue*>(exprRight.p);
    
    if (!gvRight)
        throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_84_SPATIAL_CONDITION_NOT_LITERAL_GEOMETRY)));

    FdoPtr<FdoByteArray> baRight = gvRight->GetGeometry ();

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance ();
    FdoPtr<FdoIGeometry> geomRight = gf->CreateGeometryFromFgf (baRight);
    FdoPtr<FdoIGeometry> geomLeft = gf->CreateGeometryFromFgf (fgf);

    //call on the geometry utility to evaluate the spatial operation
    bool ret = FdoSpatialUtility::Evaluate (geomLeft, filter.GetOperation (), geomRight);
    
    m_retvals.push_back (ObtainBooleanValue (false, ret));            
}

void FdoExpressionEngineImp::ProcessDistanceCondition (FdoDistanceCondition& filter)
{
    //TODO:
    printf ("distance condition on %ls\n", filter.GetPropertyName ());
    throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_85_DISTANCE_SPATIAL_CONDITION_NOT_SUPPORTED)));
}


void FdoExpressionEngineImp::ProcessBinaryExpression (FdoBinaryExpression& expr)
{
    FdoPtr<FdoExpression> left = expr.GetLeftExpression ();
    FdoPtr<FdoExpression> right = expr.GetRightExpression ();

    right->Process (this);
    left->Process (this);

    //get Process() results from return value stack
    FdoDataValue* argLeft = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();
    FdoDataValue* argRight = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    switch (expr.GetOperation ())
    {
        case FdoBinaryOperations_Add: 
            m_retvals.push_back (Add (argLeft, argRight));
            break;
        case FdoBinaryOperations_Multiply: 
            m_retvals.push_back (Multiply (argLeft, argRight));
            break;
        case FdoBinaryOperations_Subtract: 
            m_retvals.push_back (Subtract (argLeft, argRight));
            break;
        case FdoBinaryOperations_Divide: 
            m_retvals.push_back (Divide (argLeft, argRight));
            break;
        default: 
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_86_UNSUPPORTED_BINARY_OPERATION)));
    }

    RelinquishDataValue (argLeft);
    RelinquishDataValue (argRight);
}

void FdoExpressionEngineImp::ProcessUnaryExpression (FdoUnaryExpression& expr)
{
    FdoPtr<FdoExpression> right = expr.GetExpression ();

    right->Process (this);

    FdoDataValue* argRight = (FdoDataValue*)m_retvals.back ();
    m_retvals.pop_back ();

    switch (expr.GetOperation ())
    {
        case FdoUnaryOperations_Negate:
            m_retvals.push_back (Negate (argRight));
            break;
        default:
            throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_87_UNSUPPORTED_UNARY_OPERATION)));
    }

    RelinquishDataValue (argRight);
}

bool FdoExpressionEngineImp::AddToCache(FdoString *functionName, FdoExpressionEngineIFunction *functionExtension, FdoFunction& expr, bool* isAggregate)
{
    bool bAdded = false;
    FdoPtr<FdoFunctionDefinition> function = functionExtension->GetFunctionDefinition();
    if (wcscmp(function->GetName(), functionName) == 0)
    {
		if (m_Current >= m_Size)
		{
            m_Size = m_Size*2;
			m_CacheFunc = (FunctionCache *) realloc(m_CacheFunc, sizeof(FunctionCache)*m_Size);
		}
		m_CacheFunc[m_Current].address = &expr;
        if (dynamic_cast<FdoExpressionEngineINonAggregateFunction *>(functionExtension) != NULL)
        {
            m_CacheFunc[m_Current].isAggregate = false;
		    m_CacheFunc[m_Current].function = static_cast<FdoExpressionEngineINonAggregateFunction *> (functionExtension->CreateObject());
            *isAggregate = false;
        }
        else
        {
            m_CacheFunc[m_Current].isAggregate = true;
		    m_CacheFunc[m_Current].function = NULL; // not used if aggregate function
            *isAggregate = true;
        }
		m_Current++; 
        bAdded = true;
    }

    return bAdded;
}

void FdoExpressionEngineImp::ProcessFunction (FdoFunction& expr)
{

	FdoExpressionEngineINonAggregateFunction *func = NULL;
    FdoString* name = expr.GetName ();
	int i;

    bool isAggregate = false;
    bool bFunctionFound = false;

	for (i=0; i<m_Current; i++)
	{
		if (m_CacheFunc[i].address == &expr)
		{
			func = m_CacheFunc[i].function;
            isAggregate = m_CacheFunc[i].isAggregate;
            bFunctionFound = true;
			break;
		}
	}
	if (i == m_Current)
	{
		if (m_UserDefinedFunctions)
		{
			for (i=0; i<m_UserDefinedFunctions->GetCount(); i++)
			{
				FdoPtr<FdoExpressionEngineIFunction> functionExtension = m_UserDefinedFunctions->GetItem(i);
                bool bAdded = AddToCache(name, functionExtension, expr, &isAggregate);
                if (bAdded)
                {
                    if (!isAggregate)
					    func = static_cast<FdoExpressionEngineINonAggregateFunction *> (functionExtension.p);
                    bFunctionFound = true;
                    break;
                }
			}
		}
		if (m_UserDefinedFunctions == NULL || i == m_UserDefinedFunctions->GetCount())
		{
			FdoExpressionEngineIFunction** standardFunctions = init.GetStandardFunctions();
			for (i=0; standardFunctions[i] != NULL; i++)
			{
                bool bAdded = AddToCache(name, standardFunctions[i], expr, &isAggregate); 
                if (bAdded)
                {
                    if (!isAggregate)
					    func = static_cast<FdoExpressionEngineINonAggregateFunction *> (standardFunctions[i]);
                    bFunctionFound = true;
                    break;
                }
			}
		}
	}

	if (!bFunctionFound)
        throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_89_UNSUPPORTED_FUNCTION), name));

    if (isAggregate)
	{
		if (m_processingAggregate)
		{
	        FdoPtr<FdoLiteralValueCollection> functionParameters = FdoLiteralValueCollection::Create();
			FdoPtr<FdoExpressionCollection> args = expr.GetArguments ();
			for (int i=0; i<args->GetCount(); i++)
			{
				FdoPtr<FdoExpression> arg = args->GetItem(i);
				arg->Process(this);
            }

            for (int i=0; i<args->GetCount(); i++)
            {
				FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
				m_retvals.pop_back ();
				functionParameters->Insert(0, dv);
			}
			FdoExpressionEngineIAggregateFunction *func = m_AggregateFunctions.at(m_CurrentIndex);
			func->Process(functionParameters);
			for (int i=0; i<functionParameters->GetCount(); i++)
			{
				FdoPtr<FdoLiteralValue> literalValue = functionParameters->GetItem(i);
				RelinquishDataValue(literalValue);
			}
            functionParameters->Clear();
		}
		else
		{
			FdoExpressionEngineIAggregateFunction *func = m_AggregateFunctions.at(m_CurrentIndex);
			FdoLiteralValue* value = func->GetResult();
			m_retvals.push_back(value);
		}
	}
	else
	{

	    FdoPtr<FdoLiteralValueCollection> functionParameters = FdoLiteralValueCollection::Create();
		FdoPtr<FdoExpressionCollection> args = expr.GetArguments ();
		for (int i=0; i<args->GetCount(); i++)
		{
			FdoPtr<FdoExpression> arg = args->GetItem(i);
			arg->Process(this);
		}

        for (int i=0; i<args->GetCount(); i++)
        {
			FdoDataValue* dv = (FdoDataValue*)m_retvals.back ();
			m_retvals.pop_back ();
            functionParameters->Insert(0, dv);
        }

		FdoPtr<FdoLiteralValue> result = func->Evaluate(functionParameters);

        PushLiteralValue(result);
		for (int i=0; i<functionParameters->GetCount(); i++)
		{
			FdoPtr<FdoLiteralValue> literalValue = functionParameters->GetItem(i);
			RelinquishDataValue(literalValue);
		}
        functionParameters->Clear();
		return;
	}
}


FdoPropertyDefinition* FdoExpressionEngineImp::GetProperty(FdoClassDefinition* cls, FdoString* propName )
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

void FdoExpressionEngineImp::ProcessIdentifier (FdoIdentifier& expr)
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
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_71_DATA_TYPE_NOT_SUPPORTED), expr.GetName()));

	FdoPtr<FdoClassDefinition>assoc_class = ((FdoAssociationPropertyDefinition*)prop.p)->GetAssociatedClass();
	FdoPtr<FdoIFeatureReader>reader = static_cast<FdoIFeatureReader *>(m_reader)->GetFeatureObject( scope[0] );
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

void FdoExpressionEngineImp::ProcessComputedIdentifier (FdoComputedIdentifier& expr)
{
    //Computed identifiers:
    //Just evaluate the expression that the computed identifier represents.

    FdoPtr<FdoExpression> theexpr = expr.GetExpression ();
    theexpr->Process (this);
}

void FdoExpressionEngineImp::ProcessIdentifier (FdoString* name)
{
    FdoCommonPropertyStub* definition = GetPropInfo (name);

    if (definition != NULL)
    {
        if (definition->m_propertyType == FdoPropertyType_DataProperty)
        {
            bool bIsNull = (m_reader->IsNull(name));
            switch (definition->m_dataType)
            {
            case FdoDataType_Boolean : 
                m_retvals.push_back (ObtainBooleanValue (bIsNull, bIsNull ? false : m_reader->GetBoolean (name))); 
                break;
            case FdoDataType_Byte : 
                m_retvals.push_back (ObtainByteValue (bIsNull, bIsNull ? 0 : m_reader->GetByte (name))); 
                break;
            case FdoDataType_DateTime : 
                m_retvals.push_back (ObtainDateTimeValue (bIsNull, bIsNull ? FdoDateTime() : m_reader->GetDateTime (name))); 
                break;
            case FdoDataType_Decimal : 
                m_retvals.push_back (ObtainDecimalValue (bIsNull, bIsNull ? 0.0 : m_reader->GetDouble (name))); 
                break;
            case FdoDataType_Double : 
                m_retvals.push_back (ObtainDoubleValue (bIsNull, bIsNull ? 0.0 : m_reader->GetDouble (name))); 
                break;
            case FdoDataType_Int16 : 
                m_retvals.push_back (ObtainInt16Value (bIsNull, bIsNull ? 0 : m_reader->GetInt16 (name))); 
                break;
            case FdoDataType_Int32 : 
                m_retvals.push_back (ObtainInt32Value (bIsNull, bIsNull ? 0 : m_reader->GetInt32 (name))); 
                break;
            case FdoDataType_Int64 : 
                m_retvals.push_back (ObtainInt64Value (bIsNull, bIsNull ? 0 : m_reader->GetInt64 (name))); 
                break;
            case FdoDataType_Single : 
                m_retvals.push_back (ObtainSingleValue (bIsNull, bIsNull ? 0.0f : m_reader->GetSingle (name))); 
                break;
            case FdoDataType_String : 
                m_retvals.push_back (ObtainStringValue (bIsNull, bIsNull ? NULL : m_reader->GetString (name))); 
                break;
            case FdoDataType_BLOB : 
                m_retvals.push_back (ObtainBLOBValue (bIsNull, bIsNull ? NULL : m_reader->GetLOB (name)->GetData ())); 
                break;
            case FdoDataType_CLOB : 
                m_retvals.push_back (ObtainCLOBValue (bIsNull, bIsNull ? NULL : m_reader->GetLOB (name)->GetData ())); 
                break;
            default:
                throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoDataTypeToString(definition->m_dataType)));
            }
        }
        else if (definition->m_propertyType == FdoPropertyType_GeometricProperty)
        {
            bool bIsNull = (m_reader->IsNull(name));
            FdoPtr<FdoByteArray> geomBytes = m_reader->GetGeometry(name);
            m_retvals.push_back (ObtainGeometryValue (bIsNull, bIsNull ? NULL : geomBytes.p )); 
        }
        else
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_70_PROPERTY_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoPropertyTypeToString(definition->m_propertyType)));
    }
    else
    {
        //if user tried to use a computed identifier in an expression
        //we need some special handling
        FdoPtr<FdoComputedIdentifier> cid;
        if (m_compIdents) {
            cid = dynamic_cast<FdoComputedIdentifier*>(m_compIdents->GetItem (name));
        }
        
        if (cid==NULL)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_81_IDENTIFIER_NOT_FOUND), name));
        
        cid->Process (this);
    }
}

void FdoExpressionEngineImp::ProcessParameter (FdoParameter& expr)
{
    printf ("parameter %ls", expr.GetName ());
    throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_68_COMMAND_PARAMETERS_NOT_SUPPORTED)));
}

void FdoExpressionEngineImp::ProcessBooleanValue (FdoBooleanValue& expr)
{
    m_retvals.push_back (ObtainBooleanValue (expr.IsNull(), expr.IsNull() ? false : expr.GetBoolean ()));
}

void FdoExpressionEngineImp::ProcessByteValue (FdoByteValue& expr)
{
    m_retvals.push_back (ObtainByteValue (expr.IsNull(), expr.IsNull() ? 0 : expr.GetByte ()));
}

void FdoExpressionEngineImp::ProcessDateTimeValue (FdoDateTimeValue& expr)
{
    m_retvals.push_back (ObtainDateTimeValue (expr.IsNull(), expr.IsNull() ? FdoDateTime() : expr.GetDateTime ()));
}

void FdoExpressionEngineImp::ProcessDecimalValue (FdoDecimalValue& expr)
{
    m_retvals.push_back (ObtainDecimalValue (expr.IsNull(), expr.IsNull() ? 0.0 : expr.GetDecimal ()));
}

void FdoExpressionEngineImp::ProcessDoubleValue (FdoDoubleValue& expr)
{
    m_retvals.push_back (ObtainDoubleValue (expr.IsNull(), expr.IsNull() ? 0.0 : expr.GetDouble ()));
}

void FdoExpressionEngineImp::ProcessInt16Value (FdoInt16Value& expr)
{
    m_retvals.push_back (ObtainInt16Value (expr.IsNull(), expr.IsNull() ? 0 : expr.GetInt16 ()));
}

void FdoExpressionEngineImp::ProcessInt32Value (FdoInt32Value& expr)
{
    m_retvals.push_back (ObtainInt32Value (expr.IsNull(), expr.IsNull() ? 0 : expr.GetInt32 ()));
}

void FdoExpressionEngineImp::ProcessInt64Value (FdoInt64Value& expr)
{
    m_retvals.push_back (ObtainInt64Value (expr.IsNull(), expr.IsNull() ? 0 : expr.GetInt64 ()));
}

void FdoExpressionEngineImp::ProcessSingleValue (FdoSingleValue& expr)
{
    m_retvals.push_back (ObtainSingleValue (expr.IsNull(), expr.IsNull() ? 0.0f : expr.GetSingle ()));
}

void FdoExpressionEngineImp::ProcessStringValue (FdoStringValue& expr)
{
    m_retvals.push_back (ObtainStringValue (expr.IsNull(), expr.IsNull() ? NULL : expr.GetString ()));
}

void FdoExpressionEngineImp::ProcessBLOBValue (FdoBLOBValue& expr)
{
    m_retvals.push_back (ObtainBLOBValue (expr.IsNull(), expr.IsNull() ? NULL : expr.GetData ()));
}

void FdoExpressionEngineImp::ProcessCLOBValue (FdoCLOBValue& expr)
{
    m_retvals.push_back (ObtainCLOBValue (expr.IsNull(), expr.IsNull() ? NULL : expr.GetData ()));
}

void FdoExpressionEngineImp::ProcessGeometryValue (FdoGeometryValue& expr)
{
    FdoPtr<FdoByteArray> geomBytes = expr.GetGeometry();
    m_retvals.push_back (ObtainGeometryValue (expr.IsNull(), expr.IsNull() ? NULL : geomBytes.p ));
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
bool FdoExpressionEngineImp::MatchesHere (const wchar_t* pattern, const wchar_t* src)
{
    bool bMatches = false;

    if (*pattern == L'\0')
    {
        if (wcslen (src))
        {
            bMatches = false;
        }
        else
        {
            bMatches = true;
        }
    }
    else if (*pattern == L'%')
    {
        bMatches = MatchPercent (pattern+1, src);
    }
    else if (*pattern == L'[')
    {
        bMatches = MatchBracket (pattern+1, src);
    }
    else if (*src != L'\0' && ((*pattern == L'_') ||  FdoCommonOSUtil::wcsnicmp (pattern, src, 1) == 0))
    {
        bMatches = MatchesHere (pattern+1, src+1);
    }

    return bMatches;
}

// %    Any string of zero or more characters
bool FdoExpressionEngineImp::MatchPercent (const  wchar_t* pattern, const wchar_t* src)
{
    bool bMatches = false;
    
    while (!bMatches)
    {
        if (MatchesHere (pattern, src))
        {
            bMatches = true;
            break;
        }
        else if (*src == L'\0')
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
bool FdoExpressionEngineImp::MatchBracket (const wchar_t* pattern, const wchar_t* src)
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
            bMatches = MatchesHere (pattern+1, src+1);

        bMatches = false;
    }

    wchar_t start = *pattern++;

    //case of [a-b] or [^a-b]
    if (*pattern == L'-')
    {
        //go past - character and get range end character and go past range end character
        pattern++;
        wchar_t end = *pattern++;

        if ((*src >= start && *src <= end) ^ reverse)
        {
            bMatches = MatchesHere (pattern+1, src+1);
        }
        else
            bMatches = false;
    }

    //case of [abcdef] ot [^abcdef]
    std::vector<wchar_t> set;
    set.push_back (start);

    do 
    {
        set.push_back (*pattern++);
    }
    while (*pattern != L']');

    wchar_t srcchar = *src;
    bool isthere = false;

    //look for current character in the set
    for (unsigned i=0; i<set.size (); i++)
    {
        wchar_t current = set[i];
        if (FdoCommonOSUtil::wcsnicmp (&current, &srcchar, 1) == 0)
        {
            isthere = true;
            break;
        }
    }

    if (isthere ^ reverse)
        bMatches = MatchesHere (pattern+1, src+1);
    else
        bMatches = false;


    return bMatches;
}


bool FdoExpressionEngineImp::Like (FdoDataValue* argLeft, FdoDataValue* argRight)
{
    if (FdoDataType_String != argLeft->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    if (FdoDataType_String != argRight->GetDataType ())
        throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    return (MatchesHere (((FdoStringValue*)argRight)->GetString (), ((FdoStringValue*)argLeft)->GetString ()));
}

bool FdoExpressionEngineImp::IsEqualTo (FdoDataValue* argLeft, FdoDataValue* argRight)
{
    bool ret;

    // Handle NULL values:
    if (argLeft->IsNull() && argRight->IsNull())  // 2 out of 2 are NULL
        ret = true;
    else if ((argLeft->IsNull() && !argRight->IsNull()) || (!argLeft->IsNull() && argRight->IsNull()))  // 1 out of 2 are NULL
        ret = false;
    else switch (argLeft->GetDataType ())  // 0 out of 2 are NULL
    {
        case FdoDataType_Boolean:
            if (FdoDataType_Boolean != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
                ret = ((FdoBooleanValue*)argLeft)->GetBoolean () == ((FdoBooleanValue*)argRight)->GetBoolean ();
            break;
        case FdoDataType_Byte:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoByteValue*)argLeft)->GetByte () == ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoByteValue*)argLeft)->GetByte () == ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoByteValue*)argLeft)->GetByte () == ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoByteValue*)argLeft)->GetByte () == ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoByteValue*)argLeft)->GetByte () == ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoByteValue*)argLeft)->GetByte () == ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoByteValue*)argLeft)->GetByte () == ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_DateTime:
            if (FdoDataType_DateTime != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
            {
                FdoDateTime t1;
                FdoDateTime t2;

                t1 = ((FdoDateTimeValue*)argLeft)->GetDateTime ();
                t2 = ((FdoDateTimeValue*)argRight)->GetDateTime ();
                ret = ((t1.year == t2.year) &&
                       (t1.month == t2.month) &&
                       (t1.day == t2.day) &&
                       (t1.hour == t2.hour) &&
                       (t1.minute == t2.minute) &&
                       (t1.seconds == t2.seconds));
            }
            break;
        case FdoDataType_Decimal:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () == ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () == ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () == ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () == ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () == ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () == ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () == ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Double:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () == ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () == ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () == ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () == ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () == ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () == ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () == ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int16:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () == ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () == ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () == ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () == ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () == ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () == ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () == ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int32:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () == ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () == ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () == ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () == ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () == ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () == ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () == ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int64:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () == ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () == ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () == ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () == ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () == ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () == ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () == ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Single:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () == ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () == ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () == ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () == ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () == ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () == ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () == ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_String:
            if (FdoDataType_String != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
                ret = 0 == wcscmp (((FdoStringValue*)argLeft)->GetString (), ((FdoStringValue*)argRight)->GetString ());
            break;
        case FdoDataType_BLOB:
            if (FdoDataType_BLOB != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
            {
                FdoByteArray* b1;
                FdoByteArray* b2;
                FdoInt32 count1;
                FdoInt32 count2;

                b1 = ((FdoBLOBValue*)argLeft)->GetData ();
                b2 = ((FdoBLOBValue*)argRight)->GetData ();
                count1 = b1->GetCount ();
                count2 = b2->GetCount ();
                ret = count1 == count2;
                for (int i = 0; ret && (i < count1); i++)
                    if ((*b1)[i] != (*b2)[i])
                        ret = false;
            }
            break;
        case FdoDataType_CLOB:
            if (FdoDataType_CLOB != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
            {
                FdoByteArray* b1;
                FdoByteArray* b2;
                FdoInt32 count1;
                FdoInt32 count2;

                b1 = ((FdoCLOBValue*)argLeft)->GetData ();
                b2 = ((FdoCLOBValue*)argRight)->GetData ();
                count1 = b1->GetCount ();
                count2 = b2->GetCount ();
                ret = count1 == count2;
                for (int i = 0; ret && (i < count1); i++)
                    if ((*b1)[i] != (*b2)[i])
                        ret = false;
            }
            break;
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    }

    return (ret);
}

bool FdoExpressionEngineImp::IsLessThan (FdoDataValue* argLeft, FdoDataValue* argRight)
{
    bool ret;

    switch (argLeft->GetDataType ())
    {
        case FdoDataType_Boolean:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            break;
        case FdoDataType_Byte:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoByteValue*)argLeft)->GetByte () < ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoByteValue*)argLeft)->GetByte () < ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoByteValue*)argLeft)->GetByte () < ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoByteValue*)argLeft)->GetByte () < ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoByteValue*)argLeft)->GetByte () < ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoByteValue*)argLeft)->GetByte () < ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoByteValue*)argLeft)->GetByte () < ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_DateTime:
            if (FdoDataType_DateTime != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
            {
                FdoDateTime t1;
                FdoDateTime t2;

                t1 = ((FdoDateTimeValue*)argLeft)->GetDateTime ();
                t2 = ((FdoDateTimeValue*)argRight)->GetDateTime ();
                ret = ((t1.year < t2.year) ||
                      ((t1.year == t2.year) && (t1.month < t2.month)) ||
                      ((t1.year == t2.year) && (t1.month == t2.month) && (t1.day < t2.day)) ||
                      ((t1.year == t2.year) && (t1.month == t2.month) && (t1.day == t2.day) && (t1.hour < t2.hour)) ||
                      ((t1.year == t2.year) && (t1.month == t2.month) && (t1.day == t2.day) && (t1.hour == t2.hour) && (t1.minute < t2.minute)) ||
                      ((t1.year == t2.year) && (t1.month == t2.month) && (t1.day == t2.day) && (t1.hour == t2.hour) && (t1.minute == t2.minute) && (t1.seconds < t2.seconds)));
            }
            break;
        case FdoDataType_Decimal:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () < ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () < ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () < ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () < ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () < ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () < ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () < ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Double:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () < ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () < ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () < ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () < ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () < ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () < ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () < ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int16:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () < ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () < ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () < ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () < ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () < ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () < ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () < ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int32:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () < ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () < ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () < ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () < ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () < ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () < ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () < ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int64:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () < ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () < ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () < ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () < ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () < ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () < ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () < ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Single:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () < ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () < ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () < ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () < ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () < ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () < ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () < ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_String:
            if (FdoDataType_String != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
                ret = 0 > wcscmp (((FdoStringValue*)argLeft)->GetString (), ((FdoStringValue*)argRight)->GetString ());
            break;
        case FdoDataType_BLOB:
        case FdoDataType_CLOB:
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    }

    return (ret);
}

bool FdoExpressionEngineImp::IsGreaterThan (FdoDataValue* argLeft, FdoDataValue* argRight)
{
    bool ret;

    switch (argLeft->GetDataType ())
    {
        case FdoDataType_Boolean:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            break;
        case FdoDataType_Byte:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoByteValue*)argLeft)->GetByte () > ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoByteValue*)argLeft)->GetByte () > ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoByteValue*)argLeft)->GetByte () > ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoByteValue*)argLeft)->GetByte () > ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoByteValue*)argLeft)->GetByte () > ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoByteValue*)argLeft)->GetByte () > ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoByteValue*)argLeft)->GetByte () > ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_DateTime:
            if (FdoDataType_DateTime != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
            {
                FdoDateTime t1;
                FdoDateTime t2;

                t1 = ((FdoDateTimeValue*)argLeft)->GetDateTime ();
                t2 = ((FdoDateTimeValue*)argRight)->GetDateTime ();
                ret = ((t1.year > t2.year) ||
                      ((t1.year == t2.year) && (t1.month > t2.month)) ||
                      ((t1.year == t2.year) && (t1.month == t2.month) && (t1.day > t2.day)) ||
                      ((t1.year == t2.year) && (t1.month == t2.month) && (t1.day == t2.day) && (t1.hour > t2.hour)) ||
                      ((t1.year == t2.year) && (t1.month == t2.month) && (t1.day == t2.day) && (t1.hour == t2.hour) && (t1.minute > t2.minute)) ||
                      ((t1.year == t2.year) && (t1.month == t2.month) && (t1.day == t2.day) && (t1.hour == t2.hour) && (t1.minute == t2.minute) && (t1.seconds > t2.seconds)));
            }
            break;
        case FdoDataType_Decimal:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () > ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () > ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () > ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () > ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () > ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () > ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoDecimalValue*)argLeft)->GetDecimal () > ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Double:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () > ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () > ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () > ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () > ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () > ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () > ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoDoubleValue*)argLeft)->GetDouble () > ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int16:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () > ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () > ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () > ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () > ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () > ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () > ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt16Value*)argLeft)->GetInt16 () > ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int32:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () > ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () > ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () > ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () > ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () > ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () > ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt32Value*)argLeft)->GetInt32 () > ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int64:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () > ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () > ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () > ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () > ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () > ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () > ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoInt64Value*)argLeft)->GetInt64 () > ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Single:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () > ((FdoByteValue*)argRight)->GetByte ();
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () > ((FdoDecimalValue*)argRight)->GetDecimal ();
                    break;
                case FdoDataType_Double:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () > ((FdoDoubleValue*)argRight)->GetDouble ();
                    break;
                case FdoDataType_Int16:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () > ((FdoInt16Value*)argRight)->GetInt16 ();
                    break;
                case FdoDataType_Int32:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () > ((FdoInt32Value*)argRight)->GetInt32 ();
                    break;
                case FdoDataType_Int64:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () > ((FdoInt64Value*)argRight)->GetInt64 ();
                    break;
                case FdoDataType_Single:
                    ret = ((FdoSingleValue*)argLeft)->GetSingle () > ((FdoSingleValue*)argRight)->GetSingle ();
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_String:
            if (FdoDataType_String != argRight->GetDataType ())
                throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            else
                ret = 0 < wcscmp (((FdoStringValue*)argLeft)->GetString (), ((FdoStringValue*)argRight)->GetString ());
            break;
        case FdoDataType_BLOB:
        case FdoDataType_CLOB:
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    }

    return (ret);
}

FdoDataValue* FdoExpressionEngineImp::Add (FdoDataValue* argLeft, FdoDataValue* argRight)
{
    FdoDataValue* ret;

    bool bRetNull = (argLeft->IsNull() || argRight->IsNull());

    switch (argLeft->GetDataType ())
    {
        case FdoDataType_Boolean:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            break;
        case FdoDataType_Byte:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainByteValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () + ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () + ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () + ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () + ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () + ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () + ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () + ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_DateTime:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
        case FdoDataType_Decimal:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () + ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () + ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () + ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () + ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () + ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () + ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () + ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Double:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () + ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () + ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () + ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () + ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () + ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () + ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () + ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int16:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () + ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () + ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () + ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () + ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () + ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () + ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () + ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int32:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () + ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () + ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () + ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () + ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () + ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () + ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () + ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int64:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () + ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () + ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () + ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () + ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () + ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () + ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () + ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Single:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () + ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () + ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () + ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () + ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () + ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () + ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () + ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_String:
        case FdoDataType_BLOB:
        case FdoDataType_CLOB:
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    }

    return (ret);
}

FdoDataValue* FdoExpressionEngineImp::Subtract (FdoDataValue* argLeft, FdoDataValue* argRight)
{
    FdoDataValue* ret;

    bool bRetNull = (argLeft->IsNull() || argRight->IsNull());

    switch (argLeft->GetDataType ())
    {
        case FdoDataType_Boolean:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            break;
        case FdoDataType_Byte:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainByteValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () - ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () - ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () - ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () - ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () - ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () - ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () - ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_DateTime:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
        case FdoDataType_Decimal:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () - ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () - ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () - ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () - ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () - ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () - ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () - ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Double:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () - ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () - ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () - ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () - ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () - ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () - ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () - ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int16:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () - ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () - ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () - ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () - ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () - ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () - ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () - ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int32:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () - ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () - ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () - ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () - ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () - ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () - ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () - ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int64:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () - ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () - ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () - ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () - ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () - ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () - ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () - ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Single:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () - ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () - ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () - ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () - ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () - ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () - ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () - ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_String:
        case FdoDataType_BLOB:
        case FdoDataType_CLOB:
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    }

    // Handle NULL values  (use a coding shortcut):
    if (argLeft->IsNull() || argRight->IsNull())
        ret->SetNull();

    return (ret);
}

FdoDataValue* FdoExpressionEngineImp::Multiply (FdoDataValue* argLeft, FdoDataValue* argRight)
{
    FdoDataValue* ret;

    bool bRetNull = (argLeft->IsNull() || argRight->IsNull());

    switch (argLeft->GetDataType ())
    {
        case FdoDataType_Boolean:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            break;
        case FdoDataType_Byte:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainByteValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () * ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () * ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () * ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () * ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () * ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () * ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () * ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_DateTime:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
        case FdoDataType_Decimal:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () * ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () * ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () * ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () * ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () * ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () * ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () * ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Double:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () * ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () * ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () * ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () * ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () * ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () * ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () * ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int16:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () * ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () * ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () * ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () * ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () * ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () * ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () * ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int32:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () * ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () * ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () * ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () * ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () * ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () * ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () * ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int64:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () * ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () * ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () * ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () * ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () * ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () * ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () * ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Single:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () * ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () * ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () * ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () * ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () * ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () * ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () * ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_String:
        case FdoDataType_BLOB:
        case FdoDataType_CLOB:
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    }

    // Handle NULL values  (use a coding shortcut):
    if (argLeft->IsNull() || argRight->IsNull())
        ret->SetNull();

    return (ret);
}

FdoDataValue* FdoExpressionEngineImp::Divide (FdoDataValue* argLeft, FdoDataValue* argRight)
{
    FdoDataValue* ret;

    bool bRetNull = (argLeft->IsNull() || argRight->IsNull());

    switch (argLeft->GetDataType ())
    {
        case FdoDataType_Boolean:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            break;
        case FdoDataType_Byte:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainByteValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () / ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () / ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () / ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () / ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () / ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () / ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoByteValue*)argLeft)->GetByte () / ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_DateTime:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
        case FdoDataType_Decimal:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () / ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () / ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () / ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () / ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () / ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () / ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoDecimalValue*)argLeft)->GetDecimal () / ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Double:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () / ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () / ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () / ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () / ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () / ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () / ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoDoubleValue*)argLeft)->GetDouble () / ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int16:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () / ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () / ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () / ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () / ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () / ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () / ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt16Value*)argLeft)->GetInt16 () / ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int32:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () / ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () / ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () / ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () / ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () / ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () / ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt32Value*)argLeft)->GetInt32 () / ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Int64:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () / ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () / ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () / ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () / ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () / ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () / ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoInt64Value*)argLeft)->GetInt64 () / ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_Single:
            switch (argRight->GetDataType ())
            {
                case FdoDataType_Boolean:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Byte:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () / ((FdoByteValue*)argRight)->GetByte ());
                    break;
                case FdoDataType_DateTime:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
                case FdoDataType_Decimal:
                    ret = ObtainDecimalValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () / ((FdoDecimalValue*)argRight)->GetDecimal ());
                    break;
                case FdoDataType_Double:
                    ret = ObtainDoubleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () / ((FdoDoubleValue*)argRight)->GetDouble ());
                    break;
                case FdoDataType_Int16:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () / ((FdoInt16Value*)argRight)->GetInt16 ());
                    break;
                case FdoDataType_Int32:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () / ((FdoInt32Value*)argRight)->GetInt32 ());
                    break;
                case FdoDataType_Int64:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () / ((FdoInt64Value*)argRight)->GetInt64 ());
                    break;
                case FdoDataType_Single:
                    ret = ObtainSingleValue (bRetNull, bRetNull ? 0 : ((FdoSingleValue*)argLeft)->GetSingle () / ((FdoSingleValue*)argRight)->GetSingle ());
                    break;
                case FdoDataType_String:
                case FdoDataType_BLOB:
                case FdoDataType_CLOB:
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            }
            break;
        case FdoDataType_String:
        case FdoDataType_BLOB:
        case FdoDataType_CLOB:
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    }

    // Handle NULL values  (use a coding shortcut):
    if (argLeft->IsNull() || argRight->IsNull())
        ret->SetNull();

    return (ret);
}

FdoDataValue* FdoExpressionEngineImp::Negate (FdoDataValue* argLeft)
{
    FdoDataValue* ret;

    bool bRetNull = argLeft->IsNull();

    switch (argLeft->GetDataType ())
    {
        case FdoDataType_Boolean:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
            break;
        case FdoDataType_Byte:
            ret = ObtainByteValue (bRetNull, bRetNull ? 0 : -((FdoByteValue*)argLeft)->GetByte ());
            break;
        case FdoDataType_DateTime:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
        case FdoDataType_Decimal:
            ret = ObtainDecimalValue (bRetNull, bRetNull ? 0.0 : -((FdoDecimalValue*)argLeft)->GetDecimal ());
            break;
        case FdoDataType_Double:
            ret = ObtainDoubleValue (bRetNull, bRetNull ? 0.0 : -((FdoDoubleValue*)argLeft)->GetDouble ());
            break;
        case FdoDataType_Int16:
            ret = ObtainInt16Value (bRetNull, bRetNull ? 0 : -((FdoInt16Value*)argLeft)->GetInt16 ());
            break;
        case FdoDataType_Int32:
            ret = ObtainInt32Value (bRetNull, bRetNull ? 0 : -((FdoInt32Value*)argLeft)->GetInt32 ());
            break;
        case FdoDataType_Int64:
            ret = ObtainInt64Value (bRetNull, bRetNull ? 0 : -((FdoInt64Value*)argLeft)->GetInt64 ());
            break;
        case FdoDataType_Single:
            ret = ObtainSingleValue (bRetNull, bRetNull ? 0.0f : -((FdoSingleValue*)argLeft)->GetSingle ());
            break;
        case FdoDataType_String:
        case FdoDataType_BLOB:
        case FdoDataType_CLOB:
        default:
            throw FdoException::Create (FdoException::NLSGetMessage (FDO_NLSID (FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    }

    return (ret);
}


void FdoExpressionEngineImp::GetExpressionType(FdoFunctionDefinitionCollection *functionDefinitions, FdoClassDefinition* originalClassDef, FdoExpression *expr, FdoPropertyType &retPropType, FdoDataType &retDataType)
{
    VALIDATE_ARGUMENT(functionDefinitions);
    VALIDATE_ARGUMENT(originalClassDef);
    VALIDATE_ARGUMENT(expr);

    FdoPtr<FdoPropertyDefinitionCollection> properties = originalClassDef->GetProperties();
    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProperties = originalClassDef->GetBaseProperties();


    // Initialize return value to default:
    retPropType = FdoPropertyType_DataProperty;
    retDataType = FdoDataType_Int32;

    // Determine the proper return value:

    FdoUnaryExpression* unaryExpr = dynamic_cast<FdoUnaryExpression*>(expr);
    if (NULL != unaryExpr)
        GetExpressionType(functionDefinitions, originalClassDef, FdoPtr<FdoExpression>(unaryExpr->GetExpression()), retPropType, retDataType);

    FdoBinaryExpression* binaryExpr = dynamic_cast<FdoBinaryExpression*>(expr);
    if (NULL != binaryExpr)
    {
        FdoPropertyType leftPropType, rightPropType;
        FdoDataType leftDataType, rightDataType;
        GetExpressionType(functionDefinitions, originalClassDef, FdoPtr<FdoExpression>(binaryExpr->GetLeftExpression()), leftPropType, leftDataType);
        GetExpressionType(functionDefinitions, originalClassDef, FdoPtr<FdoExpression>(binaryExpr->GetRightExpression()), rightPropType, rightDataType);
        if ((leftPropType != FdoPropertyType_DataProperty) || (rightPropType != FdoPropertyType_DataProperty))
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR)));

        if (leftDataType==FdoDataType_Double)
        {
            if ((rightDataType==FdoDataType_Single) || (rightDataType==FdoDataType_Double) || (rightDataType==FdoDataType_Byte) || (rightDataType==FdoDataType_Decimal)
                 || (rightDataType==FdoDataType_Int16) || (rightDataType==FdoDataType_Int32) || (rightDataType==FdoDataType_Int64))
                retDataType = FdoDataType_Double;
            else
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_80_INVALID_EXPRESSION)));
        }
        else if (leftDataType==FdoDataType_Single)
        {
            if ((rightDataType==FdoDataType_Single) || (rightDataType==FdoDataType_Byte) 
                 || (rightDataType==FdoDataType_Int16) || (rightDataType==FdoDataType_Int32) || (rightDataType==FdoDataType_Int64))
                retDataType = FdoDataType_Single;
            else if (rightDataType==FdoDataType_Decimal)
                retDataType = FdoDataType_Decimal;
            else if (rightDataType==FdoDataType_Double)
                retDataType = FdoDataType_Double;
            else
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_80_INVALID_EXPRESSION)));
        }
        else if (leftDataType==FdoDataType_Decimal)
        {
            if (rightDataType==FdoDataType_Double)
                retDataType = FdoDataType_Double;
            else if ((rightDataType==FdoDataType_Byte) || (rightDataType==FdoDataType_Decimal) || (rightDataType==FdoDataType_Single)
                 || (rightDataType==FdoDataType_Int16) || (rightDataType==FdoDataType_Int32) || (rightDataType==FdoDataType_Int64))
                retDataType = FdoDataType_Decimal;
            else
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_80_INVALID_EXPRESSION)));
        }
        else if (leftDataType==FdoDataType_Int64)
        {
            if (rightDataType==FdoDataType_Single)
                retDataType = FdoDataType_Single;
            else if (rightDataType==FdoDataType_Double)
                retDataType = FdoDataType_Double;
            else if (rightDataType==FdoDataType_Decimal)
                retDataType = FdoDataType_Decimal;
            else if ((rightDataType==FdoDataType_Byte) || (rightDataType==FdoDataType_Int16) || (rightDataType==FdoDataType_Int32) || (rightDataType==FdoDataType_Int64))
                retDataType = FdoDataType_Int64;
            else
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_80_INVALID_EXPRESSION)));
        }
        else if (leftDataType==FdoDataType_Int32)
        {
            if (rightDataType==FdoDataType_Single)
                retDataType = FdoDataType_Single;
            else if (rightDataType==FdoDataType_Double)
                retDataType = FdoDataType_Double;
            else if (rightDataType==FdoDataType_Decimal)
                retDataType = FdoDataType_Decimal;
            else if (rightDataType==FdoDataType_Int64)
                retDataType = FdoDataType_Int64;
            else if ((rightDataType==FdoDataType_Byte) || (rightDataType==FdoDataType_Int16) || (rightDataType==FdoDataType_Int32))
                retDataType = FdoDataType_Int32;
            else
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_80_INVALID_EXPRESSION)));
        }
        else if (leftDataType==FdoDataType_Int16)
        {
            if (rightDataType==FdoDataType_Single)
                retDataType = FdoDataType_Single;
            else if (rightDataType==FdoDataType_Double)
                retDataType = FdoDataType_Double;
            else if (rightDataType==FdoDataType_Decimal)
                retDataType = FdoDataType_Decimal;
            else if (rightDataType==FdoDataType_Int64)
                retDataType = FdoDataType_Int64;
            else if (rightDataType==FdoDataType_Int32)
                retDataType = FdoDataType_Int32;
            else if ((rightDataType==FdoDataType_Byte) || (rightDataType==FdoDataType_Int16))
                retDataType = FdoDataType_Int16;
            else
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_80_INVALID_EXPRESSION)));
        }
        else if (leftDataType==FdoDataType_Byte)
        {
            if (rightDataType==FdoDataType_Single)
                retDataType = FdoDataType_Single;
            else if (rightDataType==FdoDataType_Double)
                retDataType = FdoDataType_Double;
            else if (rightDataType==FdoDataType_Decimal)
                retDataType = FdoDataType_Decimal;
            else if (rightDataType==FdoDataType_Int64)
                retDataType = FdoDataType_Int64;
            else if (rightDataType==FdoDataType_Int32)
                retDataType = FdoDataType_Int32;
            else if (rightDataType==FdoDataType_Int16)
                retDataType = FdoDataType_Int16;
            else if (rightDataType==FdoDataType_Byte)
                retDataType = FdoDataType_Byte;
            else
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_80_INVALID_EXPRESSION)));
        }
        else
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_80_INVALID_EXPRESSION)));
    }

    FdoComputedIdentifier* computedId = dynamic_cast<FdoComputedIdentifier*>(expr);
    if (NULL != computedId)
        GetExpressionType(functionDefinitions, originalClassDef, FdoPtr<FdoExpression>(computedId->GetExpression()), retPropType, retDataType);
    else
    {
        FdoIdentifier* noncomputedId = dynamic_cast<FdoIdentifier*>(expr);
        if (NULL != noncomputedId)
        {
            FdoPtr<FdoPropertyDefinition> propDef = properties->FindItem(noncomputedId->GetName());
            FdoDataPropertyDefinition* dataPropDef = dynamic_cast<FdoDataPropertyDefinition*>(propDef.p);
            FdoPtr<FdoPropertyDefinition> basePropDef;
            try
            {
                basePropDef = baseProperties->GetItem(noncomputedId->GetName());
            }
            catch (FdoException *e)
            {
                e->Release();
            }
            FdoDataPropertyDefinition* baseDataPropDef = dynamic_cast<FdoDataPropertyDefinition*>(basePropDef.p);

            if (propDef != NULL)
            {
                retPropType = propDef->GetPropertyType();
                if (dataPropDef != NULL)
                    retDataType = dataPropDef->GetDataType();
            }
            else if (basePropDef != NULL)
            {
                retPropType = basePropDef->GetPropertyType();
                if (baseDataPropDef != NULL)
                    retDataType = baseDataPropDef->GetDataType();
            }
            else
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_74_PROPERTY_NAME_NOT_FOUND), noncomputedId->GetName()));
        }
    }

    FdoBooleanValue* boolValue = dynamic_cast<FdoBooleanValue*>(expr);
    if (boolValue != NULL)
        retDataType = FdoDataType_Boolean;

    FdoByteValue* byteValue = dynamic_cast<FdoByteValue*>(expr);
    if (byteValue != NULL)
        retDataType = FdoDataType_Byte;

    FdoInt16Value* int16Value = dynamic_cast<FdoInt16Value*>(expr);
    if (int16Value != NULL)
        retDataType = FdoDataType_Int16;

    FdoInt32Value* int32Value = dynamic_cast<FdoInt32Value*>(expr);
    if (int32Value != NULL)
        retDataType = FdoDataType_Int32;

    FdoInt64Value* int64Value = dynamic_cast<FdoInt64Value*>(expr);
    if (int64Value != NULL)
        retDataType = FdoDataType_Int64;

    FdoSingleValue* singleValue = dynamic_cast<FdoSingleValue*>(expr);
    if (singleValue != NULL)
        retDataType = FdoDataType_Single;

    FdoDoubleValue* doubleValue = dynamic_cast<FdoDoubleValue*>(expr);
    if (doubleValue != NULL)
        retDataType = FdoDataType_Double;

    FdoDecimalValue* decimalValue = dynamic_cast<FdoDecimalValue*>(expr);
    if (decimalValue != NULL)
        retDataType = FdoDataType_Decimal;

    FdoDateTimeValue* datetimeValue = dynamic_cast<FdoDateTimeValue*>(expr);
    if (datetimeValue != NULL)
        retDataType = FdoDataType_DateTime;

    FdoStringValue* stringValue = dynamic_cast<FdoStringValue*>(expr);
    if (stringValue != NULL)
        retDataType = FdoDataType_String;

    FdoBLOBValue* blobValue = dynamic_cast<FdoBLOBValue*>(expr);
    if (blobValue != NULL)
        retDataType = FdoDataType_BLOB;

    FdoCLOBValue* CLOBValue = dynamic_cast<FdoCLOBValue*>(expr);
    if (CLOBValue != NULL)
        retDataType = FdoDataType_CLOB;

    FdoGeometryValue* geomValue = dynamic_cast<FdoGeometryValue*>(expr);
    if (geomValue != NULL)
    {
        retPropType = FdoPropertyType_GeometricProperty;
    }


    FdoFunction* function = dynamic_cast<FdoFunction*>(expr);
    if (NULL != function)
    {
        // Discover each argument 's property type and data type:
        FdoPtr<FdoExpressionCollection> args = function->GetArguments();
        FdoInt32 numArgs = args->GetCount();
        FdoPropertyType* argPropType = (FdoPropertyType*)alloca(sizeof(FdoPropertyType) * numArgs);
        FdoDataType*     argDataType = (FdoDataType*)    alloca(sizeof(FdoDataType)     * numArgs);
        for (int i=0; i<numArgs; i++)
        {
            FdoPtr<FdoExpression> arg = args->GetItem(i);
            GetExpressionType(functionDefinitions, originalClassDef, arg, argPropType[i], argDataType[i]);
        }

        // Match them up to the correct function signature:
        FdoPtr<FdoFunctionDefinition> funcDef = functionDefinitions->GetItem(function->GetName());
        FdoPtr<FdoReadOnlySignatureDefinitionCollection> sigDefs = funcDef->GetSignatures();
        bool bFound = false;
        for (int s=0; s<sigDefs->GetCount() && !bFound; s++)
        {
            FdoPtr<FdoSignatureDefinition> sigDef = sigDefs->GetItem(s);
            FdoPtr<FdoReadOnlyArgumentDefinitionCollection> sigArgs = sigDef->GetArguments();
            if (sigArgs->GetCount() != numArgs)
                continue;

            bFound = true;
            for (int a=0; a<numArgs && bFound; a++)
            {
                FdoPtr<FdoArgumentDefinition> sigArg = sigArgs->GetItem(a);

                bFound = (argPropType[a] == sigArg->GetPropertyType());
                if (bFound && (argPropType[a] == FdoPropertyType_DataProperty))
                    bFound = (argDataType[a] == sigArg->GetDataType());
            }

            if (bFound)
            {
                retPropType = sigDef->GetReturnPropertyType();
                retDataType = sigDef->GetReturnType();
            }
        }

        // If no matching signature was found, throw an exception:
        if (!bFound)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_183_INVALID_FUNCTION_ARG, "One or more arguments for function '%1$ls' did not match the expected argument types.", function->GetName()));
    }
}

void FdoExpressionEngineImp::ValidateFilter( FdoClassDefinition *cls, FdoFilter *filter, 
											  FdoIdentifierCollection *selIds,
											  FdoIFilterCapabilities *filterCapabilities )
{
    class FdoCommonFilterValidator : public virtual FdoIExpressionProcessor, public virtual FdoIFilterProcessor
    {
    private:
        FdoClassDefinition* m_class;
		FdoIdentifierCollection *m_selectedIds;
		FdoIFilterCapabilities	*m_filterCapabilities;

    protected:

        void HandleExpr( FdoExpression *exp )
        {
            exp->Process( this );
        }
        void HandleFilter( FdoFilter *filter )
        {
            filter->Process( this );
        }
    public:

        FdoCommonFilterValidator( FdoClassDefinition *cls, FdoIdentifierCollection *selIds, FdoIFilterCapabilities *filterCapabilities )
        {
            m_class = cls;
			m_selectedIds = selIds;
			m_filterCapabilities = filterCapabilities;
        }

        virtual void Dispose() { delete this; }

         /// <summary>Increase the reference count.</summary>
        /// <returns>Returns the new reference count (value for debugging use only).</returns>
        FdoInt32 AddRef()
        {
            // NOTE: due to multiple inheritance, there is an ambiguity in which AddRef() method to call.
            //  Calling BOTH AddRef() methods leads to instances of this class being prematurely released.
            return FdoIFilterProcessor::AddRef();
        }

        /// <summary>Decrease the reference count.</summary>
        /// <returns>Returns the new reference count (value for debugging use only).</returns>
        FdoInt32 Release ()
        {
            // NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
            //  Calling BOTH Release() methods leads to instances of this class being prematurely released.
            return FdoIFilterProcessor::Release();
        }

        virtual void ProcessBinaryExpression(FdoBinaryExpression& expr)
        {
            HandleExpr( FdoPtr<FdoExpression>(expr.GetLeftExpression()) );
            HandleExpr( FdoPtr<FdoExpression>(expr.GetRightExpression()) );
        }

// Supress warnings C4100: 'expr' : unreferenced formal parameter
#pragma warning( disable : 4100 )
        virtual void ProcessBooleanValue(FdoBooleanValue& expr) {  }
        virtual void ProcessByteValue(FdoByteValue& expr){   }
        virtual void ProcessDateTimeValue(FdoDateTimeValue& expr){   }
        virtual void ProcessDoubleValue(FdoDoubleValue& expr){   }
        virtual void ProcessDecimalValue(FdoDecimalValue& expr){   }
        virtual void ProcessInt16Value(FdoInt16Value& expr){  }
        virtual void ProcessInt32Value(FdoInt32Value& expr){  }
        virtual void ProcessInt64Value(FdoInt64Value& expr){  }
        virtual void ProcessSingleValue(FdoSingleValue& expr){  }
        virtual void ProcessStringValue(FdoStringValue& expr){  }
        virtual void ProcessBLOBValue(FdoBLOBValue& expr){   }
        virtual void ProcessCLOBValue(FdoCLOBValue& expr){  }
        virtual void ProcessFunction(FdoFunction& expr)
        {
            FdoPtr<FdoExpressionCollection>col = expr.GetArguments();
            for(int i=0; i<col->GetCount(); i++)
                HandleExpr( FdoPtr<FdoExpression>(col->GetItem( i ) ) );
        }
        virtual void ProcessGeometryValue(FdoGeometryValue& expr){  }
        virtual void ProcessParameter(FdoParameter& expr){  }
        virtual void ProcessUnaryExpression(FdoUnaryExpression& expr)
        {
            HandleExpr( FdoPtr<FdoExpression>(expr.GetExpression()) );
        }
        virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
        {
            HandleFilter( FdoPtr<FdoFilter>(filter.GetLeftOperand()) );
            HandleFilter( FdoPtr<FdoFilter>(filter.GetRightOperand()) );
        }
        virtual void ProcessComparisonCondition(FdoComparisonCondition& filter)
        {
            HandleExpr( FdoPtr<FdoExpression>(filter.GetLeftExpression()) );
            HandleExpr( FdoPtr<FdoExpression>(filter.GetRightExpression()) );
        }
        virtual void ProcessDistanceCondition(FdoDistanceCondition& filter){  }

        virtual void ProcessInCondition(FdoInCondition& filter)
        {
            ProcessIdentifier( *(FdoPtr<FdoIdentifier>(filter.GetPropertyName())) );
        }
        virtual void ProcessNullCondition(FdoNullCondition& filter)
        {
            ProcessIdentifier( *(FdoPtr<FdoIdentifier>(filter.GetPropertyName())) );
        }
        virtual void ProcessSpatialCondition(FdoSpatialCondition& filter)
		{  
			FdoSpatialOperations	op = filter.GetOperation();
			bool					found = false;
			int						numSpatialOps;

			FdoSpatialOperations	*supportedSpatialOps = m_filterCapabilities->GetSpatialOperations( numSpatialOps );

			for ( int i =0; i < numSpatialOps && !found; i++ )
			{
				found = ( op == supportedSpatialOps[i] );
			}
			if ( !found )
				throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_105_UNSUPPORTED_SPATIAL_OPERATION)));
		}

        virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
        {
            HandleFilter( FdoPtr<FdoFilter>(filter.GetOperand()) );
        }

        virtual void ProcessIdentifier(FdoIdentifier& expr)
        {
			// Return in case ids validation not required.
			if ( m_class == NULL )
				return;

             // This is used to validate the property names referenced in the filter.
            FdoPtr<FdoPropertyDefinitionCollection> props = m_class->GetProperties();
            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = m_class->GetBaseProperties();
            FdoPtr<FdoPropertyDefinition> prop;

            int length;
            // If any of the identifiers is a decorated name of the form <obj proper>.<property>, then this is
            // an association or object proeperty; assume it exist for now.
            if( expr.GetScope( length ) && length > 0 )
                return;

			// Check properties
            prop = props->FindItem( expr.GetName() );
			bool	found = ( prop != NULL );

            if( !found )
			{
				// Check base properties
				try
				{
					prop = baseProps->GetItem( expr.GetName() );
					found = true;
				}
				catch(FdoException *exp )
				{
					exp->Release();
				}

				// Check computed identifiers
				if( !found && m_selectedIds )
				{
					FdoPtr<FdoIdentifier>	id = m_selectedIds->FindItem( expr.GetName() );
					found = ( id != NULL );
				}
			}

			if ( !found )
				throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_81_IDENTIFIER_NOT_FOUND), expr.GetName()));
        }

        virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr)
        {
            HandleExpr( FdoPtr<FdoExpression>(expr.GetExpression()) );
        }
    };

    FdoCommonFilterValidator  validator(cls, selIds, filterCapabilities);
    filter->Process( &validator ); 
}

FdoFilter* FdoExpressionEngineImp::OptimizeFilter( FdoFilter *filter )
{
	// This is mostly a place holder for potential filter optimization

	// For now we just reduce a simple case with potentiel significant performance improvement
	// This is the case where a filter has 2 and'ed spatial conditions that can be reduced to a 
	// single spatial condition.
 class FdoCommonFilterOptimizer :  public virtual FdoIFilterProcessor
    {
    private:
        FdoPtr<FdoFilter>	 m_newFilter;
		bool				 m_isOptimized;
		FdoPtr<FdoIGeometry> m_geomRight;
		FdoPtr<FdoIGeometry> m_geomLeft;

    protected:
        void HandleFilter( FdoFilter *filter )
        {
            filter->Process( this );
        }
    public:

		FdoCommonFilterOptimizer( ): m_isOptimized(false)
        {
        }

		bool IsOptimized() { return m_isOptimized; }

		FdoFilter* OptimizedFilter() { return FDO_SAFE_ADDREF(m_newFilter.p); }

        virtual void Dispose() { delete this; }
       
        virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
        {
			if( filter.GetOperation() != FdoBinaryLogicalOperations_And )
			{
				m_isOptimized = false;
				return;
			}
            HandleFilter( FdoPtr<FdoFilter>(filter.GetLeftOperand()) );
            HandleFilter( FdoPtr<FdoFilter>(filter.GetRightOperand()) );
        }
        virtual void ProcessComparisonCondition(FdoComparisonCondition& filter)
        {
            m_isOptimized = false;
			return;
        }
        virtual void ProcessDistanceCondition(FdoDistanceCondition& filter)
		{  
			m_isOptimized = false;
			return;
		}

        virtual void ProcessInCondition(FdoInCondition& filter)
        {
			m_isOptimized = false;
			return;
        }
        virtual void ProcessNullCondition(FdoNullCondition& filter)
        {
            m_isOptimized = false;
			return;
        }
        virtual void ProcessSpatialCondition(FdoSpatialCondition& filter)
		{  
			bool isleft = ( m_geomLeft == NULL );
			
			FdoPtr<FdoExpression> exprRight = filter.GetGeometry ();
			FdoGeometryValue* gv = dynamic_cast<FdoGeometryValue*>(exprRight.p);
			if (!gv)
			{
				m_isOptimized = false;
				return;
			}

			if( filter.GetOperation() == FdoSpatialOperations_Disjoint )
			{
				m_isOptimized = false;
				return;
			}

			FdoPtr<FdoByteArray> ba = gv->GetGeometry ();
			FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance ();
			if( isleft )
			{
				m_geomLeft = gf->CreateGeometryFromFgf (ba);
				m_newFilter = FDO_SAFE_ADDREF(&filter);
			}
			else
			{
				m_geomRight = gf->CreateGeometryFromFgf (ba);

			
				bool ret = FdoSpatialUtility::Evaluate (m_geomLeft, FdoSpatialOperations_Inside, m_geomRight);
				if( ret )
				{
					m_isOptimized = true;
					return;
				}
				ret = FdoSpatialUtility::Evaluate (m_geomRight, FdoSpatialOperations_Inside, m_geomLeft );
				if( ret )
				{
					m_isOptimized = true;
					m_newFilter = FDO_SAFE_ADDREF(&filter);
					return;
				}
				ret = FdoSpatialUtility::Evaluate (m_geomRight, FdoSpatialOperations_Disjoint, m_geomLeft );
				if( ret )
				{
					// If the condition do not overlap, then replace it with a filter that returns 0 features.
#ifdef _WIN32
					double small_dbl  =(double)(-9223372036854775807i64 - 1);
#else
					double small_dbl  =(double)(-9223372036854775807LL - 1);
#endif
					m_isOptimized = true;
					FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

					double coords[] = { small_dbl, small_dbl, 
										small_dbl, small_dbl, 
										small_dbl, small_dbl, 
										small_dbl, small_dbl, 
										small_dbl, small_dbl }; 

					FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);

					FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);

					FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
					FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);
					m_newFilter = FdoSpatialCondition::Create(FdoPtr<FdoIdentifier>(filter.GetPropertyName())->GetName(), FdoSpatialOperations_EnvelopeIntersects, gv);
					return;
				}

				// Otherwise we make sure that the condition with the FdoSpatialOperations_EnvelopeIntersects is the left condition
				if( filter.GetOperation() == FdoSpatialOperations_EnvelopeIntersects )
				{
					// Let's re-arrage the filter to favor evaluating the envelope intersect first.
					m_newFilter = FdoFilter::Combine( &filter  , FdoBinaryLogicalOperations_And,  m_newFilter );
					m_isOptimized = true;
					return;
				}
			}
		}

        virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
        {
            m_isOptimized = false;
			return;
        }
    };

    FdoCommonFilterOptimizer  optimizer;
    filter->Process( &optimizer ); 

	return optimizer.IsOptimized()?optimizer.OptimizedFilter():FDO_SAFE_ADDREF(filter);
}

FdoLiteralValue* FdoExpressionEngineImp::Evaluate(FdoExpression *expression)
{
    FdoCommonExpressionType exprType;
    mAggrIdents = FdoCommonDataReader::GetAggregateFunctions(expression, exprType, m_AllFunctions);
    if ((mAggrIdents != NULL) && (mAggrIdents->GetCount() > 0))
    {
        EvaluateAggregateExpression();
    }

	expression->Process (this);
    FdoLiteralValue* result = (FdoLiteralValue*)m_retvals.back ();
	m_retvals.pop_back ();
    return result;
};

FdoLiteralValue *FdoExpressionEngineImp::Evaluate(FdoIdentifier& expr)
{
	ProcessIdentifier(expr);
	FdoLiteralValue* value = (FdoLiteralValue*)m_retvals.back ();
	m_retvals.pop_back();
	return value;
}

FdoLiteralValue *FdoExpressionEngineImp::Evaluate(FdoString* name)
{
    ProcessIdentifier(name);
    FdoLiteralValue* value = (FdoLiteralValue*)m_retvals.back ();
    m_retvals.pop_back();
    return value;
}

FdoFunctionDefinitionCollection* FdoExpressionEngineImp::GetStandardFunctions()
{
	FdoFunctionDefinitionCollection* functionDefinitions = FdoFunctionDefinitionCollection::Create();
	FdoExpressionEngineIFunction** standardFunctions = init.GetStandardFunctions();
	for (int i=0; standardFunctions[i] != NULL; i++)
	{
		FdoPtr<FdoFunctionDefinition> function = standardFunctions[i]->GetFunctionDefinition();
		functionDefinitions->Add(function);
	}

	return functionDefinitions;

}

FdoFunctionDefinitionCollection *FdoExpressionEngineImp::GetAllFunctions()
{
    return FDO_SAFE_ADDREF(m_AllFunctions.p);
}


void FdoExpressionEngineImp::ProcessAggregateFunctions()
{
    m_processingAggregate = true;

    for (FdoInt32 i=0; i<mAggrIdents->GetCount(); i++)
    {
	    FdoFunction* func = (*mAggrIdents)[i];
	    int j;

	    if (m_UserDefinedFunctions)
	    {
		    for (j=0; j<m_UserDefinedFunctions->GetCount(); j++)
		    {
			    FdoPtr<FdoExpressionEngineIFunction> functionExtension = m_UserDefinedFunctions->GetItem(j);
			    FdoPtr<FdoFunctionDefinition> function = functionExtension->GetFunctionDefinition();
			    if (wcscmp(function->GetName(), func->GetName()) == 0)
			    {
				    FdoExpressionEngineIAggregateFunction *aggregateFunction = static_cast<FdoExpressionEngineIAggregateFunction *>(functionExtension.p->CreateObject());
				    m_AggregateFunctions.push_back(aggregateFunction);
				    break;
			    }
		    }
	    }

        if (m_UserDefinedFunctions == NULL || i == m_UserDefinedFunctions->GetCount())
        {
	        FdoExpressionEngineIFunction** standardFunctions = init.GetStandardFunctions();
	        for (j=0; standardFunctions[j] != NULL; j++)
	        {
		        FdoPtr<FdoFunctionDefinition> functions = standardFunctions[j]->GetFunctionDefinition();
		        if (functions->IsAggregate())
		        {
			        if (wcscmp(functions->GetName(), func->GetName()) == 0)
			        {
				        FdoExpressionEngineIAggregateFunction *aggregateFunction = static_cast<FdoExpressionEngineIAggregateFunction *>(standardFunctions[j]->CreateObject());
				        m_AggregateFunctions.push_back(aggregateFunction);
				        break;
			        }
		        }
	        }
	        if (standardFunctions[j] == NULL)
                throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_89_UNSUPPORTED_FUNCTION), func->GetName()));
        }
    }
    while (m_reader->ReadNext())
    {
	    for (FdoInt32 i=0; i<mAggrIdents->GetCount(); i++)
	    {
		    m_CurrentIndex = i;
		    FdoFunction* func = (*mAggrIdents)[i];
		    func->Process(this);
	    }
    }
}



void FdoExpressionEngineImp::EvaluateAggregateExpression()
{
    ProcessAggregateFunctions();
	m_processingAggregate = false;
}

FdoPropertyValueCollection* FdoExpressionEngineImp::RunQuery()
{
	FdoCommonExpressionType exprType;
	mAggrIdents = FdoCommonDataReader::GetAggregateFunctions(m_compIdents, exprType, m_AllFunctions);

    if (mAggrIdents)
    {
        ProcessAggregateFunctions();
    }

	m_processingAggregate = false;

    FdoPtr<FdoPropertyValueCollection> retPropertyValues = FdoPropertyValueCollection::Create();
    for (FdoInt32 i=0; i<m_compIdents->GetCount(); i++)
    {
        FdoPtr<FdoIdentifier> selectedId = m_compIdents->GetItem(i);

        FdoPtr<FdoPropertyValue> pv = FdoPropertyValue::Create();
        pv->SetName(selectedId->GetName());
        FdoPtr<FdoLiteralValue> dv;

        selectedId->Process(this);

        switch (this->GetResultPropertyType())
        {
            case FdoPropertyType_DataProperty:
            {
                switch (this->GetResultDataType())
                {
                    case FdoDataType_Int32:
                    {
                        bool bIsNull;
                        FdoInt32 int32 = GetInt32Result(bIsNull);
                        if (bIsNull)
                            dv = FdoInt32Value::Create();  // defaults to NULL
                        else
                            dv = FdoInt32Value::Create(int32);
                    }
                    break;
                    case FdoDataType_Int64:   
                    {
                        bool bIsNull;
                        FdoInt64 int64 = GetInt64Result(bIsNull);
                        if (bIsNull)
                            dv = FdoInt64Value::Create();  // defaults to NULL
                        else
                            dv = FdoInt64Value::Create(int64);  
                    }
                    break;
                    case FdoDataType_Boolean: 
                    {
                        bool bIsNull;
                        bool boolean = GetBooleanResult(bIsNull);
                        if (bIsNull)
                            dv = FdoBooleanValue::Create();  // defaults to NULL
                        else
                            dv = FdoBooleanValue::Create(boolean);
                    }
                    break;
                    case FdoDataType_DateTime:
                    {
                        bool bIsNull;
                        FdoDateTime datetime = GetDateTimeResult(bIsNull);
                        if (bIsNull)
                            dv = FdoDateTimeValue::Create();  // defaults to NULL
                        else
                            dv = FdoDateTimeValue::Create(datetime);
                    }
                    break;
                    case FdoDataType_Double:  
                    {
                        bool bIsNull;
                        double dvalue = GetDoubleResult(bIsNull);
                        if (bIsNull)
                            dv = FdoDoubleValue::Create();  // defaults to NULL
                        else
                            dv = FdoDoubleValue::Create(dvalue);
                    }
                    break;
                    case FdoDataType_String:  
                    {
                        bool bIsNull;
                        FdoString* string = GetStringResult(bIsNull);
                        if (bIsNull)
                            dv = FdoStringValue::Create();  // defaults to NULL
                        else
                            dv = FdoStringValue::Create(string);
                    }
                    break;
                    default:
                        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoDataTypeToString(this->GetResultDataType())));
                }
            }
            break;

            case FdoPropertyType_GeometricProperty:
            {
                bool bIsNull;
                FdoPtr<FdoGeometryValue> geom = GetGeometricResult(bIsNull);
                if (bIsNull)
                    dv = FdoGeometryValue::Create();  // defaults to NULL
                else
                    dv = FDO_SAFE_ADDREF(geom.p);

            }
            break;

            default:
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_70_PROPERTY_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoPropertyTypeToString(this->GetResultPropertyType())));
        }

        pv->SetValue(dv);
        retPropertyValues->Add(pv);
    }

    return FDO_SAFE_ADDREF(retPropertyValues.p);

}

bool FdoExpressionEngineImp::ProcessFilter(FdoFilter *filter)
{
    bool passedFilter;
    bool bIsNull;
    Reset();
	filter->Process(this);
    passedFilter = GetBooleanResult(bIsNull);
    Reset();
    return passedFilter;
}


void FdoExpressionEngineImp::PushIdentifierValue(FdoIReader* reader, FdoString* name, FdoDataType type )
{
	switch (type)
    {
		case FdoDataType_Boolean : 
            m_retvals.push_back (ObtainBooleanValue (false, m_reader->GetBoolean (name))); 
			break;
		case FdoDataType_Byte : 
			m_retvals.push_back(ObtainInt64Value(false, reader->GetByte(name))); 
			break;
		case FdoDataType_DateTime : 
			m_retvals.push_back(ObtainDateTimeValue(false, reader->GetDateTime(name))); 
			break;
		case FdoDataType_Decimal : 
			m_retvals.push_back(ObtainDoubleValue(false, reader->GetDouble(name))); 
			break;
		case FdoDataType_Double : 
			m_retvals.push_back(ObtainDoubleValue(false, reader->GetDouble(name))); 
			break;
		case FdoDataType_Int16 : 
			m_retvals.push_back(ObtainInt64Value(false, reader->GetInt16(name))); 
			break;
		case FdoDataType_Int32 : 
			m_retvals.push_back(ObtainInt64Value(false, reader->GetInt32(name))); 
			break;
		case FdoDataType_Int64 : 
			m_retvals.push_back(ObtainInt64Value(false, reader->GetInt64(name))); 
			break;
		case FdoDataType_Single : 
			m_retvals.push_back(ObtainDoubleValue(false, reader->GetSingle(name))); 
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
			m_retvals.push_back(ObtainStringValue(false, val)); 
			}
			break;
		default: 
            throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_71_DATA_TYPE_NOT_SUPPORTED), name));
			
    }
}

void FdoExpressionEngineImp::PushLiteralValue(FdoLiteralValue *literalValue )
{
    switch (literalValue->GetLiteralValueType())
    {
        case FdoLiteralValueType_Data:
        {
            FdoDataValue *data = static_cast<FdoDataValue *> (literalValue);

            bool bIsNull = data->IsNull();

            switch(data->GetDataType())
            {
            
                case FdoDataType_Boolean : 
                    m_retvals.push_back (ObtainBooleanValue (bIsNull, bIsNull ? false : (static_cast<FdoBooleanValue *>(data))->GetBoolean()));
                    break;
                case FdoDataType_Byte : 
                    m_retvals.push_back (ObtainByteValue (bIsNull, bIsNull ? 0 : (static_cast<FdoByteValue *>(data))->GetByte()));
                    break;
                case FdoDataType_DateTime : 
                    m_retvals.push_back (ObtainDateTimeValue (bIsNull, bIsNull ? FdoDateTime() : (static_cast<FdoDateTimeValue *>(data))->GetDateTime()));
                    break;
                case FdoDataType_Decimal : 
                    m_retvals.push_back (ObtainDecimalValue (bIsNull, bIsNull ? 0.0 : (static_cast<FdoDecimalValue *>(data))->GetDecimal()));
                    break;
                case FdoDataType_Double : 
                    m_retvals.push_back (ObtainDoubleValue (bIsNull, bIsNull ? 0.0 : (static_cast<FdoDoubleValue *>(data))->GetDouble()));
                    break;
                case FdoDataType_Int16 : 
                    m_retvals.push_back (ObtainInt16Value (bIsNull, bIsNull ? 0 : (static_cast<FdoInt16Value *>(data))->GetInt16()));
                    break;
                case FdoDataType_Int32 : 
                    m_retvals.push_back (ObtainInt32Value (bIsNull, bIsNull ? 0 : (static_cast<FdoInt32Value *>(data))->GetInt32()));
                    break;
                case FdoDataType_Int64 : 
                    m_retvals.push_back (ObtainInt64Value (bIsNull, bIsNull ? 0 : (static_cast<FdoInt64Value *>(data))->GetInt64()));
                    break;
                case FdoDataType_Single : 
                    m_retvals.push_back (ObtainSingleValue (bIsNull, bIsNull ? 0.0f : (static_cast<FdoSingleValue *>(data))->GetSingle()));
                    break;
                case FdoDataType_String : 
                    m_retvals.push_back (ObtainStringValue (bIsNull, bIsNull ? NULL : (static_cast<FdoStringValue *>(data))->GetString()));
                    break;
                case FdoDataType_BLOB : 
                    m_retvals.push_back (ObtainBLOBValue (bIsNull, bIsNull ? NULL : (static_cast<FdoBLOBValue *>(data))->GetData()));
                    break;
                case FdoDataType_CLOB : 
                    m_retvals.push_back (ObtainCLOBValue (bIsNull, bIsNull ? NULL : (static_cast<FdoCLOBValue *>(data))->GetData()));
                    break;
                default:
                    throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_71_DATA_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoDataTypeToString(data->GetDataType())));
            }
            break;
        }

        case FdoLiteralValueType_Geometry:
        {
            FdoGeometryValue *geometry = static_cast<FdoGeometryValue *> (literalValue);
            bool bIsNull = geometry->IsNull();
            m_retvals.push_back (ObtainGeometryValue (bIsNull, bIsNull ? NULL : geometry->GetGeometry())); 
            break;
        }

        default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_57_UNEXPECTEDERROR)));
    }
}
