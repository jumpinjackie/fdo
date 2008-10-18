//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifndef FDOWMSXMLDIMENSION_H
#define FDOWMSXMLDIMENSION_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWmsDimension : public FdoXmlSaxHandler, public FdoIDisposable
{
	typedef FdoXmlSaxHandler BaseType;

public:
    static FdoWmsDimension* Create(void);

public:
    FdoString* GetName() const;
    FdoBoolean CanSetName() const;
    void SetName(FdoString* value);

    FdoString* GetUnits() const;
    void SetUnits(FdoString* title);

    FdoString* GetUnitSymbol() const;
    void SetUnitSymbol(FdoString* value);

    FdoString* GetDefault() const;
    void SetDefault(FdoString* value);

    FdoBoolean GetMultipleValues() const;
    void SetMultipleValues(FdoBoolean value);

    FdoBoolean GetNearestValue() const;
    void SetNearestValue(FdoBoolean value);

    FdoBoolean GetCurrent() const;
    void SetCurrent(FdoBoolean value);

    void InitFromXml(FdoXmlSaxContext* context, FdoXmlAttributeCollection* attrs);

    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );

    FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);
    
protected:
    FdoWmsDimension(void);
    virtual ~FdoWmsDimension(void);
    void Dispose();

private:
    FdoStringP mName;
    FdoStringP mUnits;
    FdoStringP mUnitSymbol;
    FdoStringP mDefault;
    FdoBoolean mMultipleValues;
    FdoBoolean mNearestValue;
    FdoBoolean mCurrent;
    FdoXmlCharDataHandlerP mXmlContentHandler;
};

typedef FdoPtr<FdoWmsDimension> FdoWmsDimensionP;

#endif // FDOWMSXMLDIMENSION_H


