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
#ifdef WIN32
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "SdfSchemaMergeContext.h"


class ExInfoDb 
{
public:

    ExInfoDb(SdfConnection* conn, SQLiteDataBase* env, const char* filename, bool bReadOnly);
    virtual ~ExInfoDb();

	void CloseCursor();

    void ReadExtendedInfo(void);
    void WriteExtendedInfo(FdoFeatureSchema* schema);
    void WriteExtendedSchemaInfo(FdoFeatureSchema* schema);

private:
    SdfConnection* m_connection;  // NOTE: weak reference (to prevent circular reference issues)
    SQLiteTable* m_db;
    SQLiteDataBase* m_env;
    bool m_bReadOnly;
};

