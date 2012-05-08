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

#ifndef FDOWFSOGCSCALARCAPABILITIES_H
#define FDOWFSOGCSCALARCAPABILITIES_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWfsOgcScalarCapabilities : 
    public FdoIDisposable,
    public virtual FdoXmlSaxHandler
{
    typedef FdoXmlSaxHandler BaseType;
public:
    enum ComparisonOperators
    {
        ComparisonOperators_Simple = 0x1,
        ComparisonOperators_Like = 0x1 << 1,
        ComparisonOperators_NullCheck = 0x1 << 2,
        ComparisonOperators_Between = 0x1 << 3,
    };

private:
    FdoInt32 m_comparisonOperators;
    bool m_logicalOperators;
    bool m_simpleArithmetic;
    FdoXmlCharDataHandlerP mXmlContentHandler;

protected:
    FdoWfsOgcScalarCapabilities();
    virtual ~FdoWfsOgcScalarCapabilities();
    virtual void Dispose() { delete this; }


public:
    static FdoWfsOgcScalarCapabilities* Create();

    bool SupportLogicalOperators() { return m_logicalOperators; }
    bool SupportSimpleArithmetic() { return m_simpleArithmetic; }
    FdoInt32 GetComparisonOperators() { return m_comparisonOperators; }

	virtual void InitFromXml(FdoXmlSaxContext* pContext, FdoXmlAttributeCollection* attrs);
    virtual FdoXmlSaxHandler* XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
    );
    virtual FdoBoolean XmlEndElement(FdoXmlSaxContext* context, FdoString* uri, FdoString* name, FdoString* qname);


};

typedef FdoPtr<FdoWfsOgcScalarCapabilities> FdoWfsOgcScalarCapabilitiesP;

#endif

