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

template <typename WrapperType, typename Base>
class VirtualObject : public Base
{
private:
	gcroot<WrapperType^> wrapper;

public:
	VirtualObject() 
	{
	}

	virtual ~VirtualObject()
	{
		wrapper = NULL;
	}

	WrapperType^ GetWrapper() const 
	{ 
		return wrapper;
	}
	
	System::Void SetWrapper(WrapperType^ wrapper)
	{
		this->wrapper = wrapper;
	}
};

class WrapperCallWrapper
{
private:
	int& m_bits;
	int m_mask;

public:
	static bool IsCalling(int value, int mask) 
	{
		return (value & mask) == mask; 
	}

	WrapperCallWrapper(int& value, int mask):m_bits(value)
	{
		m_mask = mask;
		m_bits|= m_mask;
	}

	~WrapperCallWrapper()
	{
		m_bits&=~m_mask;
	}

};

