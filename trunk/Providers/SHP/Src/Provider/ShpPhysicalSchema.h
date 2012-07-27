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
 */

#ifndef SHPPHYSICALSCHEMA_H_
#define SHPPHYSICALSCHEMA_H_     1

#ifdef _WIN32
#pragma once
#endif

#include <string>


class ShpPhysicalSchema : public FdoDisposable
{
public:
    ShpPhysicalSchema(FdoString* tempDir = NULL);

public:
    // Read from FileSet list:
    FdoInt32 GetFileSetCount (void);
    ShpFileSet* GetFileSet (FdoInt32 filesetIndex);
    ShpFileSet* GetFileSet (FdoString* class_name);

    // Add/remove from FileSet list:
    void AddFileSet (ShpFileSet* newFileSet);
    void AddFileSet (const wchar_t* name);
    void AddFileSet (const char* name);
    void RemoveFileSet (ShpFileSet* set);

protected:
    virtual ~ShpPhysicalSchema(void);

private:
    // The file sets that belong to this physical schema:
    std::vector<ShpFileSet*> mFileSets;
    FdoStringP mTempDir;
};

typedef FdoPtr<ShpPhysicalSchema> ShpPhysicalSchemaP;


#endif // SHPPHYSICALSCHEMA_H_

