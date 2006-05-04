#ifndef FDO_XML_DATAPROPERTY_H_
#define FDO_XML_DATAPROPERTY_H_
//

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

#ifdef _WIN32
#pragma once
#endif

#include <Std.h>


class FdoXmlDataProperty: public FdoDisposable
{
public:
	
	FdoXmlDataProperty(FdoString* name, FdoString* value);
	~FdoXmlDataProperty();
	FdoString* GetName() const;
	FdoString* GetValue() const;

protected:
    virtual void Dispose ()
    {
        delete this;
    }


private:
	FdoStringP m_name;
	FdoStringP m_value;
};

typedef FdoPtr<FdoXmlDataProperty> FdoXmlDataPropertyP;


#endif

