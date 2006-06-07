#ifndef FDORDBMSUTIL_H
#define FDORDBMSUTIL_H  1
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
 *
 */

#include "Fdo/Schema/ClassDefinition.h"
#include "FdoGeometry.h"
#include "../../Gdbi/GdbiTypes.h"
#include "../../Gdbi/GdbiException.h"

#define         MAX_TMP_ARRAY_SIZE      10 // Used for Wchar to Char conversions

extern char *fdordbms_cat;
/*
typedef struct {
        int     type; // FdoDataType
        char    name[GDBI_DB_NAME_SIZE];
		union {
			void	*strvalue; // can be char* or wchar_t*
			double  dvalue;
			FdoInt64   llvalue;
			int     lvalue;
			short   svalue;
			char    cvalue;
		} value;
		bool    valueNeedsFree;
        int     len;
        GDBI_NI_TYPE   null_ind;
        char    propertyName[GDBI_SCHEMA_ELEMENT_NAME_SIZE];
        FdoIStreamReader    *reader;
        int     pos;
} FdoRdbmsBindDef;
*/
class FdoRdbmsUtil
{
private:
    char        mTempCharBuffer[MAX_TMP_ARRAY_SIZE][3*GDBI_MAXIMUM_TEXT_SIZE]; // Used as a temporary buffer to conver from unicode to UTF8
    wchar_t     mTempWcharBuffer[MAX_TMP_ARRAY_SIZE][GDBI_MAXIMUM_TEXT_SIZE];// Used as a temporary buffer to conver from UTF8 to unicode

    int         mNextCharBuffIdx;
    int         mNextWcharBuffIdx;




public:

    FdoRdbmsUtil()
    {
        mNextCharBuffIdx=0;
        mNextWcharBuffIdx=0;
    }
    ~FdoRdbmsUtil(){}

    //
    // returns a temporary buffer of size GDBI_MAXIMUM_TEXT_SIZE
    char*       newCharP();
    
    //
    // Returns a temporary wide char buffer of size 3*GDBI_MAXIMUM_TEXT_SIZE
    wchar_t*    newWcharP();

    //
    // Converts a wchar to a char and return a pointer to a statically allocated storage that can be overwritten
    // by subsequent calls to this method.
    const char* UnicodeToUtf8( const wchar_t* value );
    //
    // Converts a char to a wchar_t and return a pointer to a statically allocated storage that can be overwritten
    // by subsequent calls to this method.
    const wchar_t* Utf8ToUnicode( const char* value );

    //
    // Converts a dbi data type to an FDO data type
    static  FdoDataType  DbiToFdoType( int type /* Inc/rdbi.h */ );

    //
    // Converts a FDO data type to a dbi type
    static  int  FdoToDbiType( FdoDataType type );

    

    //
    // A portable string compare utility function
    static int  StrCmp( const wchar_t * str1, const wchar_t * str2 );

};

#endif // FDORDBMSUTIL_H
