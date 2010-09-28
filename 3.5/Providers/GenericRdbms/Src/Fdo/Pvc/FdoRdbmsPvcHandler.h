#ifndef FDORDBMSPVCHANDLER_H
#define FDORDBMSPVCHANDLER_H    1
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
#include <Sm/SchemaManager.h>
#include <../../Gdbi/GdbiCommands.h>
// Temporary datatype until FdoDataType_Geometry is available
#define FdoRdbmsDataType_Geometry    FdoDataType_CLOB + 2


typedef struct {
        int     type; // FdoDataType
        wchar_t    name[GDBI_COLUMN_NAME_SIZE];
		union {
			void		*strvalue; // can be char* or wchar_t*
			double		gvalue;
			float		fvalue;
			FdoInt64    llvalue;
			int			lvalue;
			short		svalue;
			char		cvalue;
		} value;
		bool			 valueNeedsFree;
        int				 len;
        GDBI_NI_TYPE	 *null_ind;
        wchar_t			 propertyName[GDBI_SCHEMA_ELEMENT_NAME_SIZE];
        FdoIStreamReader *reader;
        int				 pos;
} FdoRdbmsPvcBindDef;

//
// Defines an abstract class for the Property Value Collection handlers
class FdoRdbmsPvcHandler
{

public:
    /// <summary>Given a PVC operation, execute the handler.</summary>
    /// <returns>Returns the number of affected objects</returns> 
    virtual long Execute( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection  *propValCollection, bool revisionNumberUpdate, bool handleForeignAutoincrementedId) = 0;

protected:

    /// <summary>Formats a geometry ordinate value to the precision for its column. Inserts and 
    /// Updates failed for some RDBMS's (e.g. SQL Server) if too much precision is supplied.</summary>
    /// <param>buffer (output) - the formatted value</param> 
    /// <param>doubleValue (input) - the ordinate value</param> 
    /// <param>columnDefinition (input) - provides column precision</param> 
    void SetGeomOrdinateBindValue( char* buffer, double doubleValue, const FdoSmPhColumn* columnDefinition );
};

#endif

