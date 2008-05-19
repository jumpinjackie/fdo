#ifndef PHYSNAME_H
#define PHYSNAME_H
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

// Encapsulates an SDF data, key or rtree table name and its encoding
class PhysName
{
public:
    // Create name from wide char
    PhysName( 
        FdoString* wPrefix, // prepend to name
        FdoString* wName,   // the class name of the table.
        bool isUTF8         // true: char* version of table is in UTF8
                            // false: char* version in multibyte.
    );
    // Create name from char*
    PhysName( 
        const char* sPrefix,  // prepend to name
        const char* sName,    // the class name of the table.
        bool isUTF8           // true: sName is in UTF8
                              // false: sName is in multibyte.
    );

    // Copy constructor
	PhysName(const PhysName& oName);

    virtual ~PhysName();
    
    // true: if char* version of name is in UTF8
    // false: if multibyte 
    bool IsUTF8();

    // Return wide char version of table name
  	operator FdoString*( );

    // Return char version of table name. Encoding
    // depends in IsUTF8().
	operator const char*( );

private:
    void SetWName( FdoString* wPrefix, FdoString* wName );
    void SetSName( const char* sPrefix, const char* sName );

    bool mIsUTF8;

    FdoStringP mFdoName;

    wchar_t* mWName;
    char* mSName;
};
#endif

