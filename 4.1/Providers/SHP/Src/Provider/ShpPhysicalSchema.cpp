/*
 * 
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

#include "stdafx.h"
#include "ShpPhysicalSchema.h"


ShpPhysicalSchema::ShpPhysicalSchema(FdoString* tempDir) 
{
    mTempDir = tempDir;
}

ShpPhysicalSchema::~ShpPhysicalSchema(void)
{
    // cleanup:
	int count = (int)mFileSets.size ();
	for (int i = 0; i < count; i++)
        delete mFileSets[i];
    mFileSets.clear ();
}


FdoInt32 ShpPhysicalSchema::GetFileSetCount (void)
{
    return (FdoInt32)mFileSets.size();
}

ShpFileSet* ShpPhysicalSchema::GetFileSet (FdoInt32 filesetIndex)
{
    return mFileSets.at(filesetIndex);
}

ShpFileSet* ShpPhysicalSchema::GetFileSet (FdoString* class_name)
{
    ShpFileSet* ret;
	int count;

    ret = NULL;

    count = (int)mFileSets.size ();
	for (int i = 0; i < count; i++)
        if (0 == wcscmp (mFileSets[i]->GetBaseName (), class_name))
        {
            ret = mFileSets[i];
            break;
        }
    
    return (ret);
}


void ShpPhysicalSchema::AddFileSet(ShpFileSet* newFileSet)
{
    mFileSets.push_back(newFileSet);
}


void ShpPhysicalSchema::AddFileSet (const wchar_t* name)
{
    ShpFileSet* set = new ShpFileSet (name, mTempDir);
    mFileSets.push_back (set);
}

void ShpPhysicalSchema::AddFileSet (const char* name)
{
    wchar_t* wname;

    multibyte_to_wide (wname, name);
    AddFileSet (wname);
}

void ShpPhysicalSchema::RemoveFileSet (ShpFileSet* set)
{
    for (std::vector<ShpFileSet*>::iterator where = mFileSets.begin (); where != mFileSets.end (); where++)
        if (*where == set)
        {
            delete *where;
            mFileSets.erase (where);
            break;
        }
}

