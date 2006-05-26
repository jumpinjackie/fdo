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
#include "BinaryWriter.h"

class PropertyIndex;

class KeyDb 
{
public:

    KeyDb(SQLiteDataBase* env, const char* filename, const char* dbname, bool bReadOnly, bool useIntKey);
    virtual ~KeyDb();
    
    void InsertKey(SQLiteData* key, REC_NO recno);

    void DeleteKey(FdoClassDefinition* fc, FdoIFeatureReader* reader);
    void DeleteKey(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc, REC_NO recno);
    void DeleteKey(SQLiteData* key);

    REC_NO FindRecno(FdoClassDefinition* fc, FdoPropertyValueCollection* pvc);
    
    bool KeyExists(SQLiteData* key);

	void CloseCursor();

	bool NeedsAFlush(bool partial=false) { return m_db->needs_a_flush(partial); }

	void Flush() { m_db->flush(); }

	bool IndexNeedsRegen() { return m_IndexNeedsRegen; }

	void Regened() { m_IndexNeedsRegen = false; }

	void DropIndex() { m_db->Recreate(); }

private:

    SQLiteTable* m_db;

	bool		 m_IndexNeedsRegen;

};


