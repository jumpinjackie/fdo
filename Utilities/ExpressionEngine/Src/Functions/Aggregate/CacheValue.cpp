// 
//  
//  Copyright (C) 2004-2007  Autodesk, Inc.
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

#include <stdafx.h>
#include <Functions/Aggregate/CacheValue.h>
#include "StringUtility.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

CacheValue::CacheValue (FdoByte value)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{
    byte_value = value;

}  //  CacheValue ()

CacheValue::CacheValue (FdoDateTime value)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{
    date_time_value = value;

}  //  CacheValue ()

CacheValue::CacheValue (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{
    double_value = value;

}  //  CacheValue ()

CacheValue::CacheValue (FdoFloat  value)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    float_value = value;

}  //  CacheValue ()

CacheValue::CacheValue (FdoInt16  value)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    int16_value = value;

}  //  CacheValue ()

CacheValue::CacheValue (FdoInt32  value)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    int32_value = value;

}  //  CacheValue ()

CacheValue::CacheValue (FdoInt64  value)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    int64_value = value;

}  //  CacheValue ()

CacheValue::CacheValue (FdoString  *value)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    if (string_value != NULL) {

        delete[] string_value;
        string_value = NULL;

    }  //  if (string_value != NULL) ...

    string_value = FdoStringUtility::MakeString(value);

}  //  CacheValue ()

CacheValue::~CacheValue ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
}  //  ~CacheValue ()

void CacheValue::Dispose ()

// +---------------------------------------------------------------------------
// | The function disposes the current object.
// +---------------------------------------------------------------------------

{

    delete this;

}  //  Dispose ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

CacheValue *CacheValue::Create (FdoByte value)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

	return new CacheValue(value);

}  //  Create ()

CacheValue *CacheValue::Create (FdoDateTime value)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

	return new CacheValue(value);

}  //  Create ()

CacheValue *CacheValue::Create (FdoDouble value)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

	return new CacheValue(value);

}  //  Create ()

CacheValue *CacheValue::Create (FdoFloat value)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

	return new CacheValue(value);

}  //  Create ()

CacheValue *CacheValue::Create (FdoInt16 value)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

	return new CacheValue(value);

}  //  Create ()

CacheValue *CacheValue::Create (FdoInt32 value)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

	return new CacheValue(value);

}  //  Create ()

CacheValue *CacheValue::Create (FdoInt64 value)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

	return new CacheValue(value);

}  //  Create ()

CacheValue *CacheValue::Create (FdoString *value)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

	return new CacheValue(value);

}  //  Create ()

FdoByte CacheValue::GetByteValue ()

// +---------------------------------------------------------------------------
// | The function returns the value of type BYTE stored in the cache value.
// +---------------------------------------------------------------------------

{

    return byte_value;

}  //  GetByteValue ()

FdoDateTime CacheValue::GetDateTimeValue ()

// +---------------------------------------------------------------------------
// | The function returns the value of type DATETIME stored in the cache value.
// +---------------------------------------------------------------------------

{

    return date_time_value;

}  //  GetDateTimeValue ()

FdoDouble CacheValue::GetDoubleValue ()

// +---------------------------------------------------------------------------
// | The function returns the value of type DOUBLE stored in the cache value.
// +---------------------------------------------------------------------------

{

    return double_value;

}  //  GetDoubleValue ()

FdoFloat CacheValue::GetFloatValue ()

// +---------------------------------------------------------------------------
// | The function returns the value of type FLOAT stored in the cache value.
// +---------------------------------------------------------------------------

{

    return float_value;

}  //  GetFloatValue ()

FdoInt16 CacheValue::GetInt16Value ()

// +---------------------------------------------------------------------------
// | The function returns the value of type INT16 stored in the cache value.
// +---------------------------------------------------------------------------

{

    return int16_value;

}  //  GetInt16Value ()

FdoInt32 CacheValue::GetInt32Value ()

// +---------------------------------------------------------------------------
// | The function returns the value of type INT32 stored in the cache value.
// +---------------------------------------------------------------------------

{

    return int32_value;

}  //  GetInt32Value ()

FdoInt64 CacheValue::GetInt64Value ()

// +---------------------------------------------------------------------------
// | The function returns the value of type INT64 stored in the cache value.
// +---------------------------------------------------------------------------

{

    return int64_value;

}  //  GetInt64Value ()

FdoString *CacheValue::GetStringValue ()

// +---------------------------------------------------------------------------
// | The function returns a reference to the value of type STRING stored in the
// | cache value.
// +---------------------------------------------------------------------------

{

    return string_value;

}  //  GetStringValue ()


