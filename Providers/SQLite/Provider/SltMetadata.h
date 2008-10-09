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

//Data structure containing FDO metadata for an SQLite table
class SltMetadata
{
public:

    SltMetadata(SltConnection* connection, const char* name);
    ~SltMetadata();

    FdoClassDefinition*     ToClass(); 

    bool                    Failed() { return m_table == 0; };

    GeomFormat              GetGeomFormat() { return (GeomFormat)m_geomFormat; }

private:

    void FindSpatialContextName(int srid, std::wstring& ret);

    SltConnection*      m_connection;
    FdoClassDefinition* m_fc;
    Table*              m_table;
    Table*              m_mdtable;
    int                 m_iGeom;
    int                 m_geomFormat;
};

