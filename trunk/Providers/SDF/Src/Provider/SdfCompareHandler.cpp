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
#include "stdafx.h"
#include "SDF/SdfCompareHandler.h"

SdfCompareHandler::SdfCompareHandler()
{
}

SdfCompareHandler::~SdfCompareHandler()
{
}

SdfCompareHandler* SdfCompareHandler::Create() 
{ 
	return new SdfCompareHandler(); 
}

int SdfCompareHandler::Compare( const wchar_t* name, double dbl1, double dbl2 )
{
	if( dbl1 > dbl2 )
		return 1;
	
	if( dbl1 < dbl2 )
		return -1;

	return 0;
}

int SdfCompareHandler::Compare( const wchar_t* name, float val1, float val2 )
{
	if( val1 > val2 )
		return 1;
	
	if( val1 < val2 )
		return -1;

	return 0;
}

int SdfCompareHandler::Compare( const wchar_t* name, FdoInt64 val1, FdoInt64 val2 )
{
	FdoInt64 int64val = val1 - val2;

	if( int64val > 0 )
	  return 1;
	
	if( int64val < 0 )
	  return -1;

	return 0;
}

int SdfCompareHandler::Compare( const wchar_t* name, FdoInt32 val1, FdoInt32 val2 )
{
	return val1 - val2;
}

int SdfCompareHandler::Compare( const wchar_t* name, FdoInt16 val1, FdoInt16 val2 )
{
	return val1 - val2;
}

int SdfCompareHandler::Compare( const wchar_t* name, const wchar_t* val1, const wchar_t* val2 )
{
#ifdef WIN32
	return wcscoll(val1,val2); 
#else
    return wcscmp(val1,val2); 
#endif
}

int SdfCompareHandler::Compare( const wchar_t* name, FdoDateTime tm1, FdoDateTime tm2 )
{
	if( tm1.year != tm2.year )
	  return tm1.year - tm2.year;

	if (tm1.month != tm2.month )
	  return tm1.month - tm2.month;

	if (tm1.day != tm2.day )
	  return tm1.day - tm2.day;

	if (tm1.hour != tm2.hour )
	  return tm1.hour - tm2.hour;

	if (tm1.minute != tm2.minute )
	  return tm1.minute - tm2.minute;

	if (tm1.seconds != tm2.seconds )
	{
		float val = tm1.seconds - tm2.seconds;
		if( val > 0 )
			return 1;
	
		if( val < 0 )
			return -1;
	}
	  
	return 0;
}

int SdfCompareHandler::Compare( const wchar_t* name, unsigned char val1, unsigned char val2 )
{
	return val1 - val2;
}

