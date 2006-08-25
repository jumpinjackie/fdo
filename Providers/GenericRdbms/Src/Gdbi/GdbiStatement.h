#ifndef _GDBISTATEMENT_
#define _GDBISTATEMENT_
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
 *
 */
#ifdef _WIN32
#pragma once
#endif

#include "GdbiQueryResult.h"
#include "Inc/Geometry/IGeometry.h"
#include "GdbiQueryIdentifier.h"

class GdbiStatement
{
    friend class GdbiConnection;
private:
    GdbiCommands*     m_pGdbiCommands;
    mutable char      buffer[16];  // Used to convert integer indexes to a string indexes
	GdbiQueryIdentifier	*m_QueryId;


    GdbiStatement(GdbiCommands* command, int qid);

public:

     ~GdbiStatement(void);
    //
    // Bind overrides
    int Bind( int parmIndex, int size, const char* szValue, GDBI_NI_TYPE *nullInd = NULL);

    int Bind( int parmIndex, int size, const wchar_t* szValue, GDBI_NI_TYPE *nullInd = NULL );

	int Bind( int parmIndex, short *sValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, int *nValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, double *dValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, float *fValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, FdoInt64 *i64Value, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, FdoIGeometry* gValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind( int parmIndex, FdoDateTime *dValue, GDBI_NI_TYPE *nullInd = NULL );

    int Bind(
        int parmIndex,       /* column/expression position               */
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE  *null_ind   /* pointer to null indicator variables      */
            );

    int BindNull( int parmIndex );
    // End bind functions

    int ParmCount( );

    int ExecuteNonQuery( );
	int Free();

    GdbiQueryResult*  ExecuteQuery( );

	bool SupportsInt64Binding();

	int set_array_size(int arraySize);
};

#endif //_GDBISTATEMENT_
