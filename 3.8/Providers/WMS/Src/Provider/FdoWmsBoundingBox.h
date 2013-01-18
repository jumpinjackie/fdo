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

#ifndef FDOWMSBOUNDINGBOX_H
#define FDOWMSBOUNDINGBOX_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWmsBoundingBox : public FdoXmlSaxHandler, public FdoIDisposable
{
	typedef FdoXmlSaxHandler BaseType;

public:
    static FdoWmsBoundingBox* Create(void);

public:
    FdoDouble GetMinX() const;
    void SetMinX(FdoDouble value);

    FdoDouble GetMinY() const;
    void SetMinY(FdoDouble value);

    FdoDouble GetMaxX() const;
    void SetMaxX(FdoDouble value);

    FdoDouble GetMaxY() const;
    void SetMaxY(FdoDouble value);

    FdoDouble GetResX() const;
    void SetResX(FdoDouble value);

    FdoDouble GetResY() const;
    void SetResY(FdoDouble value);

    FdoString* GetCRS() const;
    void SetCRS(FdoString* value);

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
    FdoWmsBoundingBox(void);
    virtual ~FdoWmsBoundingBox(void);
    void Dispose();

private:
    FdoDouble mMinX;
    FdoDouble mMinY;
    FdoDouble mMaxX;
    FdoDouble mMaxY;
    FdoDouble mResX;
    FdoDouble mResY;
    FdoStringP mCRS;
};

typedef FdoPtr<FdoWmsBoundingBox> FdoWmsBoundingBoxP;

#endif // FDOWMSBOUNDINGBOX_H


