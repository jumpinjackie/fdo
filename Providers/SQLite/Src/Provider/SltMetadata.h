// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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

#ifndef SLT_METADATA_H
#define SLT_METADATA_H

enum GeomFormat
{
    eFGF = 0,
    eWKB = 1,
    eWKT = 2
};

class SltConnection;

typedef std::pair<std::string, std::string> StlNamePair;
typedef std::vector< StlNamePair > StlMapNamesList;

typedef std::pair<std::string, StlNamePair> StlPropNamePair;
typedef std::vector< StlPropNamePair > StlMapPropNamesList;

//Data structure containing FDO metadata for an SQLite table
class SltMetadata
{
public:

    SltMetadata(SltConnection* connection, const char* name, bool bUseFdoMetadata);
    ~SltMetadata();

    FdoClassDefinition*     ToClass(); 

    GeomFormat              GetGeomFormat() { return (GeomFormat)m_geomFormat; }
    const wchar_t*          GetGeomName()   { return m_geomName; }
    const wchar_t*          GetIdName()     { return m_idName; }
    const char*             GetMainViewTable() { return m_tablename.c_str(); }
    int                     GetGeomIndex()  { return m_geomIndex; }
    int                     GetIDIndex()    { return m_idIndex; }

    static FdoDataValue* GenerateConstraintValue(FdoDataType type, FdoString* value);

    bool IsView() { return m_bIsView; }
    bool IsMultipleSelectSrcView() { return m_bIsMSelectView; }    

private:
    class SQLiteExpression
    {
    public:
        unsigned char op;
        std::wstring name;
        std::vector<std::wstring> values;
    public:
        SQLiteExpression(unsigned char pOp)
        {
            op = pOp;
        }
    };
    bool ExtractConstraints(Expr* node, std::vector<SQLiteExpression>& result);
    void GenerateConstraint(FdoDataPropertyDefinition* prop, SQLiteExpression& operation);

    void ProcessViewProperties(Table* pTable);
    void FindSpatialContextName(int srid, std::wstring& ret);
    // returns true in case extra checking needs to be done
    bool ExtractViewDetailsInfo(StlMapNamesList& sources, StlMapPropNamesList& properties, StlMapNamesList& expressions, Table* pTable);

    SltConnection*      m_connection;
    FdoClassDefinition* m_fc;
    int                 m_geomFormat;
    bool                m_bUseFdoMetadata;
    std::string         m_tablename;
    bool                m_bIsView;
    const wchar_t*      m_idName; // in case is a view and we have one
    const wchar_t*      m_geomName;
    int                 m_idIndex;
    int                 m_geomIndex;
    bool                m_bIsMSelectView;
};

#endif
