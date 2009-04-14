#ifndef FDO_XML_BLOBPROPERTY_H_
#define FDO_XML_BLOBPROPERTY_H_
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

class FdoXmlBLOBProperty: public FdoDisposable
{
private:
	FdoStringP m_name;
	FdoPtr<FdoBLOBValue> m_value;

protected:
    FdoXmlBLOBProperty();
    FdoXmlBLOBProperty(FdoString* name, FdoBLOBValue* value) : m_name(name), m_value(value) {
        FDO_SAFE_ADDREF(value);
    }
    virtual ~FdoXmlBLOBProperty() {}
    virtual void Dispose () { delete this;}

public:
    FdoString* GetName() const { return m_name; }
    FdoBoolean CanSetName() { return false; }
    FdoBLOBValue* GetValue() const { return FDO_SAFE_ADDREF(m_value.p); }

    static FdoXmlBLOBProperty* Create(FdoString* name, FdoBLOBValue* value){
        return new FdoXmlBLOBProperty(name, value);
    }



};

typedef FdoPtr<FdoXmlBLOBProperty> FdoXmlBLOBPropertyP;

class FdoXmlBLOBPropertyCollection : public FdoNamedCollection<FdoXmlBLOBProperty, FdoException>
{
public:
    static FdoXmlBLOBPropertyCollection* Create() {
        return new FdoXmlBLOBPropertyCollection();
    }

protected:
    FdoXmlBLOBPropertyCollection() {}
    virtual ~FdoXmlBLOBPropertyCollection() {}
    virtual void Dispose() { delete this; }
};

typedef FdoPtr<FdoXmlBLOBPropertyCollection> FdoXmlBLOBPropertiesP;


#endif

