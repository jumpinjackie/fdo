/*
* Copyright (C) 2004-2006  Autodesk, Inc.
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

#pragma once

#include <string.h>
#include <FdoCommon.h>

class StringToUni
{
private:
	void* m_ptr;

public:
	StringToUni(System::Object * str)
	{
		m_ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(__try_cast<System::String*>(str)).ToPointer();
	}
	StringToUni(System::String * str)
	{
		m_ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str).ToPointer();
	}
	~StringToUni()
	{
		System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr(m_ptr));
	}
	operator const FdoString*()
	{
		return (const FdoString*)m_ptr;
	}
}; 

class StringToAnsi
{
private:
	void* m_ptr;

public:
	StringToAnsi(System::Object * str)
	{
		m_ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(__try_cast<System::String*>(str)).ToPointer();
	}
	StringToAnsi(System::String * str)
	{
		m_ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer();
	}
	~StringToAnsi()
	{
		System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr(m_ptr));
	}
	operator const char*()
	{
		return (const char*)m_ptr;
	}
};   

inline System::Byte FdoByteArrayToByteArray(const FdoByte* umBuffer, FdoInt32 len) []
{
	System::Byte mgBuffer __gc[] = new System::Byte[len];
	for (FdoInt32 i = 0; i < len; i++)
	{
		mgBuffer[i] = umBuffer[i];
	}
	return mgBuffer;
}

/// \brief
/// Convert a managed ByteArray buffer to a new unmanaged ByteArray buffer.
/// This new buffer should be released when it isn't used.
inline FdoByteArray* ByteArrayToFdoByteArray(System::Byte mgBuffer __gc[])
{
	FdoByte* umBuffer = new FdoByte[mgBuffer->Length];
	for (FdoInt32 i = 0; i < mgBuffer->Length; i++)
	{
		umBuffer[i] = mgBuffer[i];
	}
	return FdoByteArray::Create(umBuffer, mgBuffer->Length);
}

inline System::String* FdoStringArrayToStringArray(const FdoString** umArray, FdoInt32 len) []
{
	System::String* mgArray __gc[] = __gc new System::String*[len];
	for (FdoInt32 i = 0; i < len; i++)
	{
		mgArray[i] = new System::String(umArray[i]);
	}
	return mgArray;
}

inline FdoString** UnwrapStringArray(System::String* mgArray[])
{
	FdoString** umArray = new FdoString*[mgArray->Length];

	for (FdoInt32 i = 0; i < mgArray->Length; i++)
	{
		wchar_t* temp = new wchar_t[mgArray[i]->Length];
		wcscpy(temp, StringToUni(mgArray[i]));
		umArray[i] = temp;
	}
	return umArray;
}


inline System::DateTime FdoDateTimeToDateTime(FdoDateTime& date)
{
	return System::DateTime(date.year, date.month, date.day, date.hour, date.minute, System::Int32(date.seconds));;
}

inline FdoDateTime SystemDateToFdoDateTime(System::DateTime date)
{
	return FdoDateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute, FdoFloat(date.Second));
}

inline System::Boolean* FdoBoolToBoolean(FdoBoolean* b)
{
}

inline static FdoBoolean* BooleanToFdoBool(System::Boolean* b)
{	
	System::Boolean __pin* bb = b;
	return bb;
}


