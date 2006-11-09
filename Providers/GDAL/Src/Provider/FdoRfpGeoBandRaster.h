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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpGeoBandRaster.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPGEOBANDRASTER_H
#define FDORFPGEOBANDRASTER_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include "FdoRfpRect.h"
class FdoRfpKeyColorCollection;
class FdoRfpConnection;

//
// class forwards
//
class FdoRfpImage;

class FdoRfpGeoBandRaster : public FdoDisposable
{
// 
// data members
//
protected:
    FdoPtr<FdoRfpConnection> m_connection;

    FdoPtr<FdoRfpImage>     m_image;
    FdoStringP              m_imagePath;
    int                     m_frameNumber;

//
// ctor and dtor
//
protected:
    FdoRfpGeoBandRaster(FdoRfpConnection* connection, FdoString* imagePath);
    FdoRfpGeoBandRaster(FdoRfpConnection* connection, FdoString* imagePath, int frameNumber);
    virtual ~FdoRfpGeoBandRaster();
private:
    FdoRfpGeoBandRaster();

//
// exposed functions
//
public:
    FdoString*      GetId();
    FdoRfpImage*    GetImage();
    bool            GetKeyColor(FdoPtr<FdoRfpKeyColorCollection>& keyColorColl);

//
// exposed and virtual functions
//
public:
    virtual bool    IsRotated() = 0;
    virtual double  GetResolutionX() = 0;
    virtual double  GetResolutionY() = 0;
    virtual FdoIGeometry* GetGeometry() = 0;
    virtual FdoRfpRect GetBounds() = 0;


//
// protected virtual functions
//
protected:
    virtual void PixelToWorld(double x, double y, double *x_out, double *y_out) = 0;

};

typedef FdoPtr<FdoRfpGeoBandRaster> FdoRfpGeoBandRasterP;


class FdoRfpGeoBandRasterCollection : public FdoCollection<FdoRfpGeoBandRaster, FdoException>
{
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpGeoBandRasterCollection() {}
	virtual ~FdoRfpGeoBandRasterCollection() {}
	void Dispose() { delete this; }
public:
	static FdoRfpGeoBandRasterCollection* Create();
};

typedef FdoPtr<FdoRfpGeoBandRasterCollection> FdoRfpGeoBandRastersP;


#endif

