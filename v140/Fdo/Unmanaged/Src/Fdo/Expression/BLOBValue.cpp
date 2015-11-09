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
#include <Fdo/Expression/BLOBValue.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include "../Schema/DataTypeMapper.h"
#include "StringUtility.h"

#include <time.h>

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoBLOBValue* FdoBLOBValue::Create()
{
	return new FdoBLOBValue();
}

// Constructs an instance of a BLOB or CLOB DataValue using the specified
// arguments.
FdoBLOBValue* FdoBLOBValue::Create(FdoByteArray* value)
{
	return new FdoBLOBValue(value);
}

FdoBLOBValue* FdoBLOBValue::Create(
    FdoDataValue* src, 
    FdoBoolean nullIfIncompatible,
    FdoBoolean shift, 
    FdoBoolean truncate
)
{
    FdoBLOBValue* ret = NULL;

    if ( !src->IsNull() ) 
    {
        switch ( src->GetDataType() ) 
        {
        case FdoDataType_BLOB:
            // Same types, simple copy.
            {
                FdoPtr<FdoByteArray> val = static_cast<FdoBLOBValue*>(src)->GetData();
                ret = FdoBLOBValue::Create( val );
            }
            break;

        default:
            // src and dest types incompatible
            if ( !nullIfIncompatible )
                throw FdoExpressionException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(EXPRESSION_22_INCOMPATIBLEDATATYPES),
                        src->ToString(),
                        (FdoString*) FdoDataTypeMapper::Type2String(src->GetDataType()),
                        (FdoString*) FdoDataTypeMapper::Type2String(FdoDataType_BLOB)
                    )
                );
            // else return null value 
            break;
        }
   }

    if ( !ret ) 
        // return null data value instead of NULL pointer.
        ret = FdoBLOBValue::Create();

    return ret;
}

// Constructs a default instance of a DataValue with data type string and a
// value of null.
FdoBLOBValue::FdoBLOBValue()
{
    m_data = NULL;
}

// Constructs an instance of a BLOB or CLOB DataValue using the specified
// arguments.
FdoBLOBValue::FdoBLOBValue(FdoByteArray* value)
{
    if (value == NULL)
    {
		m_isNull = true;
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_1_BLOBVALUENULL)));
        throw pExcept;
    }
	m_isNull = false;
	m_data = FDO_SAFE_ADDREF(value);
}

FdoBLOBValue::~FdoBLOBValue()
{
    FDO_SAFE_RELEASE(m_data);
}

void FdoBLOBValue::Dispose()
{
    delete this;
}

// Gets the data type of the DataValue.
FdoDataType FdoBLOBValue::GetDataType()
{
	return FdoDataType_BLOB;
}

// Gets the DataValue as a byte array.
FdoByteArray* FdoBLOBValue::GetData()
{
    if (IsNull())
    {
        FdoExpressionException* pExcept = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_1_BLOBVALUENULL)));
        throw pExcept;
    }

	return FDO_SAFE_ADDREF(m_data);
}

// Sets the DataValue as a byte array.
void FdoBLOBValue::SetData(FdoByteArray* value)
{
	// NULL is valid so no exception
	FDO_SAFE_RELEASE(m_data);
	m_isNull = (value == NULL);
	m_data = FDO_SAFE_ADDREF(value);
}

// Sets the DataValue to a null value of the specified type.
void FdoBLOBValue::SetNull()
{
    SetData(NULL);
    FdoDataValue::SetNull();
}

// Overrides Expression.Process to pass the DataValue to the appropriate
// expression processor operation.
void FdoBLOBValue::Process(FdoIExpressionProcessor* p)
{
    p->ProcessBLOBValue(*this); 
}

// Returns the well defined text representation of this expression.
FdoString* FdoBLOBValue::ToString()
{
    FdoStringUtility::ClearString(m_toString);

    if (IsNull())
    {
        m_toString = FdoStringUtility::MakeString(L"{NULL}");
        return m_toString;
    }

//TODO:    m_toString = FdoStringUtility::MakeString(m_data, m_length);
    m_toString = FdoStringUtility::MakeString(L"{UNDEFINED}");
    return m_toString;
}


