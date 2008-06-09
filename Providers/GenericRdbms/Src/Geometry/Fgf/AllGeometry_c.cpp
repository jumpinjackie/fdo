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

#include <Inc/Nls/rdbi_msg.h>
#include <stdio.h>
#include "FdoGeometry.h"

#include <Inc/Geometry/Fgf/AllGeometry_c.h>	

// C functions for wrapping C++ calls 

///////////////////////////////////////////////////////////////////////////
int IGeometry_CreateGeometry( pIGeometry_def geometry, pIGeometry_def *geomOut)
{
    FdoFgfGeometryFactory*  gf = 0;
  
    int  rc = IGeometry_CheckNullArg2( (void **)geomOut, true ) && 
              IGeometry_CheckNullArg( geometry, true );
 
    if ( rc )
    {
        try
        {
            *geomOut = 0;
            FdoIGeometry * g = (FdoIGeometry *) geometry;
            FdoGeometryType type;

            rc = IGeometry_GetDerivedType( geometry, &type );

            if ( rc )
            {
                if (  type == FdoGeometryType_None )
                {
                    IGeometry_AddRef( geometry );
                    *geomOut = geometry;
                }
                else 
                {
                    gf = FdoFgfGeometryFactory::GetInstance();
                    *geomOut = (pIGeometry_def) gf->CreateGeometry( g ); 
                }
            }
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    if ( gf != 0 )
        gf->Release();

     return rc;
}

///////////////////////////////////////////////////////////////////////////
// The caller should check for NULL return value
///////////////////////////////////////////////////////////////////////////
pByteArray_def IByteArray_Create(unsigned char* bytes, int count)
{
    FdoByteArray *da = 0;

    try 
    {
        da = FdoByteArray::Create(bytes, count);
    }
    catch (FdoException *ex) 
    {
        ex->Release();
    }
    return (pByteArray_def)da;
}

void IByteArray_Release(pByteArray_def baIn)
{
    FdoByteArray* ba = (FdoByteArray *) baIn;
    if ( ba != 0 )
        ba->Release();

}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetWkb( pIGeometry_def geometry, pByteArray_def *baOut )
{
    FdoFgfGeometryFactory*  gf = 0;

    int  rc = IGeometry_CheckNullArg2( (void **)baOut, true );

    *baOut = 0;
    if ( rc )
    {
        try
        {
            if ( geometry != 0 )
            {
                FdoIGeometry * g = (FdoIGeometry *) geometry;
                gf = FdoFgfGeometryFactory::GetInstance();
                
                *baOut = (pByteArray_def) gf->GetWkb( g ); 
            }
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    if ( gf != 0 )
        gf->Release();

    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetWkbData( pByteArray_def ba, unsigned char **data, int *size )
{

    int  rc = IGeometry_CheckNullArg2( (void **)data, true ) &&
              IGeometry_CheckNullArg( (void *)size, true );  

    *data = 0;
    *size = 0;

    if ( rc && ba != 0 )
    {
        FdoByteArray* wkb = (FdoByteArray*) ba;
        *data = wkb->GetData();
        *size = wkb->GetCount();
    }

    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetFgf( pIGeometry_def geometry, pByteArray_def *baOut )
{
    FdoFgfGeometryFactory*  gf = 0;

    int  rc = IGeometry_CheckNullArg2( (void **)baOut, true );

    *baOut = 0;
    if ( rc )
    {
        try
        {
            if ( geometry != 0 )
            {
                FdoIGeometry * g = (FdoIGeometry *) geometry;
                gf = FdoFgfGeometryFactory::GetInstance();
                
                *baOut = (pByteArray_def) gf->GetFgf( g ); 
            }
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    if ( gf != 0 )
        gf->Release();

    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetByteArrayData( pByteArray_def ba, unsigned char **data, int *size )
{
    int  rc = IGeometry_CheckNullArg2( (void **)data, true ) &&
              IGeometry_CheckNullArg( (void *)size, true );  

    *data = 0;
    if ( rc && ba != 0 )
    {
        FdoByteArray *ba2 = (FdoByteArray *) ba;
        *data = ba2->GetData();
        *size = ba2->GetCount();
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
int  IGeometry_CreateGeometryFromWkb( pByteArray_def ba, pIGeometry_def *geomOut, pIGeometry_def *geomOutCopy )
{
    FdoFgfGeometryFactory*  gf = 0;
  
    int  rc = IGeometry_CheckNullArg2( (void **)geomOut, true );
 
    if ( rc )
    {
        try
        {
            *geomOut = 0;
            if ( ba != NULL )
            {
                gf = FdoFgfGeometryFactory::GetInstance();
                *geomOut = (pIGeometry_def) gf->CreateGeometryFromWkb( (FdoByteArray *)ba ); 
            }
			if ( geomOutCopy )
			{
				*geomOutCopy = *geomOut;
			}
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    if ( gf != 0 )
        gf->Release();

     return rc;
   
}

///////////////////////////////////////////////////////////////////////
int  IGeometry_CreateGeometryFromFgf( pByteArray_def ba, pIGeometry_def *geomOut, pIGeometry_def *geomOutCopy )
{
    FdoFgfGeometryFactory*  gf = 0;
  
    int  rc = IGeometry_CheckNullArg2( (void **)geomOut, true );
 
    if ( rc )
    {
        try
        {
            *geomOut = 0;
            if ( ba != NULL )
            {
                gf = FdoFgfGeometryFactory::GetInstance();
                *geomOut = (pIGeometry_def) gf->CreateGeometryFromFgf( (FdoByteArray *)ba ); 
            }
			if ( geomOutCopy )
			{
				*geomOutCopy = *geomOut;
			}
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    if ( gf != 0 )
        gf->Release();

     return rc;
   
}

///////////////////////////////////////////////////////////////////////
int IGeometry_AddRef(pIGeometry_def geometry)
{
    int    rc = IGeometry_CheckNullArg( geometry, true );

    if ( rc )
    {
	    FdoIGeometry * g = (FdoIGeometry *) geometry;
	    g->AddRef();
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
FdoInt32 IGeometry_Release( pIGeometry_def geometry)
{
    FdoInt32 refs = 0;

    if ( geometry != 0 )
    {
	    FdoIGeometry *g = (FdoIGeometry *) geometry;
        if (g != 0)
        {
	        refs = g->Release();
        }
    }
    return refs;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetFgftString( pIGeometry_def geometry, const wchar_t **akwtOut )
{
    int    rc = IGeometry_CheckNullArg( geometry, true ) && 
                IGeometry_CheckNullArg2( (void **) akwtOut, true );

    if ( rc )
    {
        try
        {
	        FdoIGeometry * g = (FdoIGeometry *) geometry;
            *akwtOut = g->GetText();
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetDerivedType( pIGeometry_def geometry, FdoGeometryType *geomType )
{
    int    rc = IGeometry_CheckNullArg( geometry, true ) &&
                IGeometry_CheckNullArg( geomType, true );
    if ( rc )
    {
        try
        {
	        FdoIGeometry * g = (FdoIGeometry *) geometry;
	        *geomType = g->GetDerivedType();
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_GetDimensionality( pIGeometry_def geometry, int *geomDim )
{
    int    rc = IGeometry_CheckNullArg( geometry, true ) &&
                IGeometry_CheckNullArg( geomDim, true );

    if ( rc )
    {
        try
        {
	        FdoIGeometry * g = (FdoIGeometry *) geometry;
	        *geomDim = g->GetDimensionality();
        }
        catch (FdoException *ex) 
        {
            ex->Release();
            rc = false;
        }
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////
int IGeometry_CheckNullArg( void* arg, int report )
{
    return ( arg != 0 );
}

///////////////////////////////////////////////////////////////////////
int IGeometry_CheckNullArg2( void** arg, int report )
{
    return ( arg != 0 );
}

