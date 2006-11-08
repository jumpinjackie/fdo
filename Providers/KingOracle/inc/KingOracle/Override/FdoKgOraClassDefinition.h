/*
 * Copyright (C) 2006  SL-King d.o.o
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
 */
#ifndef FdoKgOraClassDefinition_H
#define FdoKgOraClassDefinition_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <KingOracle/Override/FdoKgOraPropertyDefinitionCollection.h>

class FdoKgOraClassDefinition : public FdoPhysicalClassMapping
{
	typedef FdoPhysicalClassMapping BaseType;

public:
    FDOKGORA_API static FdoKgOraClassDefinition* Create();

public:
    FDOKGORA_API FdoKgOraPropertyDefinitionCollection* GetProperties() const;
    
    FDOKGORA_API FdoString* GetOracleFullTableName() const { return m_OracleTableFullName; }
    FDOKGORA_API void SetOracleFullTableName(FdoString * OracleFullName) { m_OracleTableFullName=OracleFullName; };
    
    FDOKGORA_API void SetUseSequenceForIdentity(FdoString * TableSequence) { m_UseSequenceForIdentity=TableSequence; };
    FDOKGORA_API FdoString* GetUseSequenceForIdentity() const { return m_UseSequenceForIdentity; }
    
    FDOKGORA_API void SetOraTableAliasNum(int AliasNum) { char tbuff[16]; sprintf(tbuff,"a%d",AliasNum);  m_OraTableAlias=tbuff; };
    FDOKGORA_API const char* GetOraTableAlias() { return  m_OraTableAlias; };
	
	

    FDOKGORA_API virtual void InitFromXml(GisXmlSaxContext* Context, GisXmlAttributeCollection* Attrs);
    FDOKGORA_API virtual GisXmlSaxHandler* XmlStartElement(GisXmlSaxContext* Context,FdoString* Uri, FdoString* Name, FdoString* QName, GisXmlAttributeCollection* Attrs);
    FDOKGORA_API virtual GisBoolean XmlEndElement(GisXmlSaxContext* Context, FdoString* Uri, FdoString* Name, FdoString* QName);
    FDOKGORA_API virtual void _writeXml(GisXmlWriter* Writer, const FdoXmlFlags* Flags);

    /// find property with column name
    FDOKGORA_API FdoKgOraPropertyDefinition* FindByColumnName(FdoString* ColumnName);

protected:
    FdoKgOraClassDefinition(void);
    virtual ~FdoKgOraClassDefinition(void);
    virtual void Dispose(void);

private:
	  FdoStringP m_OracleTableFullName; // complete table name in Oracle (owner.table) for fdo class
	  FdoStringP m_OraTableAlias; // table allias used for in SQL (generated in describe schema)
	  FdoStringP m_UseSequenceForIdentity; // if this
	  
    FdoKgOraPropertyDefinitionCollectionP m_Properties;
};

typedef GisPtr<FdoKgOraClassDefinition> FdoKgOraClassDefinitionP;

#endif // FdoKgOraClassDefinition_H


