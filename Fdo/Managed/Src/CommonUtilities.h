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

/// \cond DOXYGEN-IGNORE

ref class StringToUni : System::IDisposable
{
private:
	void* m_ptr;

public:
	StringToUni(System::String^ str)
	{
		m_ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(str).ToPointer();
	}
	!StringToUni()
	{
		System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr(m_ptr));
	}
    ~StringToUni()
    {
        this->!StringToUni();
    }
	operator const FdoString*()
	{
		return (FdoString*)m_ptr;
	}
}; 

ref class StringToAnsi : System::IDisposable
{
private:
	void* m_ptr;

public:
	StringToAnsi(System::String^ str)
	{
		m_ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str).ToPointer();
	}
	!StringToAnsi()
	{
		System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr(m_ptr));
	}
    ~StringToAnsi()
    {
        this->!StringToAnsi();
    }
	operator const char*()
	{
		return (const char*)m_ptr;
	}
};   

inline array<System::Byte>^ FdoByteArrayToByteArray(const FdoByte* umBuffer, FdoInt32 len)
{
	array<System::Byte>^ mgBuffer = gcnew array<System::Byte>(len);
    pin_ptr<FdoByte> umgBuffer = &mgBuffer[0];
    memcpy(umgBuffer, umBuffer, len);
	return mgBuffer;
}

/// \brief
/// Convert a managed ByteArray buffer to a new unmanaged ByteArray buffer.
/// This new buffer should be released when it isn't used.
inline FdoByteArray* ByteArrayToFdoByteArray(array<System::Byte>^ mgBuffer)
{
    if (mgBuffer ==nullptr || mgBuffer->Length == 0)
        return nullptr;
    
    pin_ptr<FdoByte> umgBuffer = &mgBuffer[0];
    FdoByteArray* ba = FdoByteArray::Create(umgBuffer, mgBuffer->Length);
    
    return ba;
}

inline array<System::String^>^ FdoStringArrayToStringArray(const FdoString** umArray, FdoInt32 len)
{
	array<System::String^>^ mgArray = gcnew array<System::String^>(len);
	for (FdoInt32 i = 0; i < len; i++)
	{
		mgArray[i] = CHECK_STRING(umArray[i]);
	}
	return mgArray;
}

inline FdoString** UnwrapStringArray(array<System::String^>^ mgArray)
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
    FdoInt8 hour = date.hour;
    FdoInt8 minute = date.minute;
    float seconds = date.seconds;
    FdoInt8 day = date.day;
    FdoInt8 month = date.month;
    FdoInt16 year = date.year;

    if (date.IsDate())
    {
        if ((day   > 0  &&  day   <= 31)     &&
            (month > 0  &&  month <= 12)     &&
            (year  > 0  &&  year  <= 9999))
        {
    	    return System::DateTime(year, month, day);
        }
    }
    else
    {
        if ((day     >  0  &&  day     <= 31)     &&
            (month   >  0  &&  month   <= 12)     &&
            (year    >  0  &&  year    <= 9999)   &&
            (hour    >= 0  &&  hour    <  24)     &&
            (minute  >= 0  &&  minute  <  60)     &&
            (seconds >= 0  &&  seconds <  60))
        {
            int intSeconds = System::Int32(seconds);
            return System::DateTime(year, month, day, hour, minute, intSeconds, System::Int32((seconds-System::Single(intSeconds))*1000.0f));
        }
    }

    return System::DateTime();
}

inline FdoDateTime SystemDateToFdoDateTime(System::DateTime date)
{
    return FdoDateTime(date.Year, date.Month, date.Day, date.Hour, date.Minute, FdoFloat(date.Second) + FdoFloat(date.Millisecond)/1000.0f);
}

/// \endcond
