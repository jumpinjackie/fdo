#ifndef _SQLITEDATA_H
#define _SQLITEDATA_H
// 
//  Copyright (C) 2004-2005  Autodesk, Inc.
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
#ifdef _WIN32
#pragma once
#endif

/*
** A wrapper class for the size/data pair used to insert into the BTree
*/
class SQLiteData
{
private:
    void        *mData;
    unsigned int   mSize;

public:

    SQLiteData(void *data, unsigned int size):mData(NULL),mSize(0)
    {
        mData = data;
        mSize = size;
    }

    SQLiteData():mData(NULL),mSize(0) {}

    ~SQLiteData() 
    {
    }

    unsigned int get_size() const             
    { 
        return mSize; 
    }

    void set_size(unsigned int value)         
    { 
        mSize =  value;
    }

    void *get_data() const                 
    { 
        return mData; 
    }

    void set_data(void *value)             
    {
        mData = value;
    }
};

#endif

