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
#include "DataValueStack.h"

//forward declare
class DataValue;
class Int64Value;
class StringValue;
class DoubleValue;
class BooleanValue;
class DateTimeValue;
class NullValue;


//Keeps around pools of DataValue object instances
//as quicker alternative to memory allocation/object creation
//during query execution, since we use and then throw away a lot 
//of DataValue objects when executing queries for every single feature.
//In theory we could overload the new/delete of DataValues to use this pool
//but why not keep things simpler?
//Very rudimentary implementation, the user can shoot themselves in
//the foot if not careful when releasing (or not!) pooled objects:
class DataValuePool
{
public:
    DataValuePool();
    virtual ~DataValuePool();

    Int64Value* ObtainInt64Value(FdoInt64 val);

    DoubleValue* ObtainDoubleValue(double val);

    StringValue* ObtainStringValue(wchar_t* val, bool dispose = false);

    BooleanValue* ObtainBooleanValue(bool val);

    DateTimeValue* ObtainDateTimeValue(FdoDateTime val);

	NullValue* ObtainNullValue();

    void RelinquishDataValue(DataValue* dv);

private:

    void RelinquishBooleanValue(BooleanValue* dv);
    void RelinquishStringValue(StringValue* dv);
    void RelinquishDoubleValue(DoubleValue* dv);
    void RelinquishInt64Value(Int64Value* dv);
    void RelinquishDateTimeValue(DateTimeValue* dv);

    DataValueStack<Int64Value> m_int64Pool;
    DataValueStack<DoubleValue> m_doublePool;
    DataValueStack<StringValue> m_stringPool;
    DataValueStack<BooleanValue> m_boolPool;
    DataValueStack<DateTimeValue> m_datePool;
	NullValue	*m_NullValue;
};

