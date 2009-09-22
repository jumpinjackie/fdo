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

enum GeomFormat
{
    eFGF = 0,
    eWKB = 1,
    eWKT = 2
};

class SltConnection;

typedef std::vector<std::pair<std::string, std::string> > StlMapNamesList;

//Data structure containing FDO metadata for an SQLite table
class SltMetadata
{
public:

    SltMetadata(SltConnection* connection, const char* name, bool bUseFdoMetadata);
    ~SltMetadata();

    FdoClassDefinition*     ToClass(); 

    GeomFormat              GetGeomFormat() { return (GeomFormat)m_geomFormat; }

    static FdoDataValue* GenerateConstraintValue(FdoDataType type, FdoString* value);

    bool IsView() { return m_bIsView; }
    bool IsOptimized() { return m_bIsOptimized; }

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

    void FindSpatialContextName(int srid, std::wstring& ret);
    void ExtractViewDetailsInfo(StlMapNamesList& sources, StlMapNamesList& properties, StlMapNamesList& expressions, Table* pTable);

    SltConnection*      m_connection;
    FdoClassDefinition* m_fc;
    int                 m_geomFormat;
    bool                m_bUseFdoMetadata;
    std::string         m_tablename;
    bool                m_bIsView;
    bool                m_bIsOptimized;
};

