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
#ifndef PROPERTYINDEX_H
#define PROPERTYINDEX_H

//defines that helps us use hash_map in the same way 
//under Linux and Windows.
#ifdef WIN32
#include <hash_map>
#else
#include <ext/hash_map>
namespace stdext = ::__gnu_cxx;
#endif

#include <string>


struct PropertyStub
{
    wchar_t* m_name;
    int m_recordIndex;
    FdoDataType m_dataType;
	FdoPropertyType m_propertyType;
    bool m_isAutoGen;
};

//Utility class that maps a class' property names
//to their index inside the database record.
//Identity properties are indexed using negative numbers (starting at -1), 
//since they are stored in the key record, not the 
//data record
class PropertyIndex
{
public:

    PropertyIndex(FdoClassDefinition* clas, unsigned int fcid);
    ~PropertyIndex();

    PropertyStub* GetPropInfo(FdoString* name);
    PropertyStub* GetPropInfo(int index);
    bool IsPropAutoGen(FdoString* name);
    int GetNumProps();
    FdoClassDefinition* GetBaseClass();
    FdoFeatureClass* GetBaseFeatureClass();
    unsigned int GetFCID();
    bool HasAutoGen();

    static FdoGeometricPropertyDefinition* FindGeomProp(FdoClassDefinition* clas);

private:

    PropertyIndex(){};

    int m_numProps;
	int m_lastIndex;
    PropertyStub* m_vProps;
    FdoClassDefinition* m_baseClass;
    FdoFeatureClass* m_baseFeatureClass;
    unsigned int m_fcid;
    bool m_bHasAutoGen;

};


#endif

