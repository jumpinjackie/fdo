//
// Copyright (C) 2004-2007  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef _CACHEVALUE_H_
#define _CACHEVALUE_H_

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>

class CacheValue : public FdoIDisposable
{

    public:

        // ********************************************************************
        // *                         Public Interfaces                        *
        // ********************************************************************

        // Create:
        //  Those functions create a new cache value using the provided data
        //  and return it back to the calling routine.

        static CacheValue *Create (FdoByte     value);
        static CacheValue *Create (FdoDateTime value);
        static CacheValue *Create (FdoDouble   value);
        static CacheValue *Create (FdoFloat    value);
        static CacheValue *Create (FdoInt16    value);
        static CacheValue *Create (FdoInt32    value);
        static CacheValue *Create (FdoInt64    value);
        static CacheValue *Create (FdoString   *value);

        // GetByteValue:
        //  The function returns the value of type BYTE stored in the cache
        //  value.

        FdoByte GetByteValue ();

        // GetDateTimeValue:
        //  The function returns the value of type DATETIME stored in the cache
        //  value.

        FdoDateTime GetDateTimeValue ();

        // GetDoubleValue:
        //  The function returns the value of type DOUBLE stored in the cache
        //  value.

        FdoDouble GetDoubleValue ();

        // GetFloatValue:
        //  The function returns the value of type FLOAT stored in the cache
        //  value.

        FdoFloat GetFloatValue ();

        // GetInt16Value:
        //  The function returns the value of type INT16 stored in the cache
        //  value.

        FdoInt16 GetInt16Value ();

        // GetInt32Value:
        //  The function returns the value of type INT32 stored in the cache
        //  value.

        FdoInt32 GetInt32Value ();

        // GetInt64Value:
        //  The function returns the value of type INT64 stored in the cache
        //  value.

        FdoInt64 GetInt64Value ();

        // GetStringValue:
        //  The function returns a reference to the value of type STRING stored
        //  in the cache value.

        FdoString *GetStringValue ();


    protected:

        // ********************************************************************
        // *                       Protected Interfaces                       *
        // ********************************************************************

        // CacheValue:
        //  Those functions represent the class constructors.

        CacheValue (FdoByte     value);
        CacheValue (FdoDateTime value);
        CacheValue (FdoDouble   value);
        CacheValue (FdoFloat    value);
        CacheValue (FdoInt16    value);
        CacheValue (FdoInt32    value);
        CacheValue (FdoInt64    value);
        CacheValue (FdoString   *value);

        // ~CacheValue:
        //  The functions represent the class destructor.

        virtual ~CacheValue ();

        // Dispose:
        //  The function disposes the current object.

        virtual void Dispose ();


        // ********************************************************************
        // *                     Protected Member Variables                   *
        // ********************************************************************

        // *_value:
        //  The following variables represent the cache variables.

        FdoByte     byte_value;
        FdoDateTime date_time_value;
        FdoDouble   double_value;
        FdoFloat    float_value;
        FdoInt16    int16_value;
        FdoInt32    int32_value;
        FdoInt64    int64_value;
        FdoString   *string_value;

};  //  class CacheValue

#endif


