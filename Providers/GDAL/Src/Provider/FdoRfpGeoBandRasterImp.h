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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpGeoBandRasterImp.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */
#ifndef FDORFPGEORASTERIMP_H
#define FDORFPGEORASTERIMP_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include "FdoRfpGeoBandRaster.h"

class FdoRfpGeoBandRasterRot : public FdoRfpGeoBandRaster
{
//
// data members
//
private:
    bool        m_haveGeotransform;
    double      m_insertionX, m_insertionY;
    double      m_resX, m_resY;
    double      m_rotationX, m_rotationY ;

    int         m_width, m_height;

    bool        m_haveBounds;
    double      m_minX;
    double      m_minY;
    double      m_maxX;
    double      m_maxY;
//
// ctor and dtor
//
public:
    FdoRfpGeoBandRasterRot(FdoRfpConnection* conn,
                           FdoString* imagePath, 
                           double insertionX, double insertionY,
                           double resX, double resY,
                           int width, int height,
                           double rotationX, double rotationY);
    FdoRfpGeoBandRasterRot(FdoRfpConnection* conn,
                           FdoString* imagePath, int frameNumber,  
                           double insertionX, double insertionY,
                           double resX, double resY,
                           int width, int height,
                           double rotationX, double rotationY);
    FdoRfpGeoBandRasterRot(FdoRfpConnection* conn,
                           FdoString* imagePath, int frameNumber);
protected:
    virtual ~FdoRfpGeoBandRasterRot();
private:
    FdoRfpGeoBandRasterRot();
    
    bool loadImageInfo();
                            
//
// exposed and virtual functions
//
public:
    virtual bool    IsRotated();
    virtual double  GetResolutionX();
    virtual double  GetResolutionY();
    virtual FdoIGeometry* GetGeometry();
    virtual FdoRfpRect GetBounds();

    void            SetBounds( double minX, double minY, double maxX, double maxY );
    void            SetGeotransform( double insertionX, double insertionY, 
                                     double resX, double resY,
                                     double rotationX, double rotationY );
//
// protected virtual functions
//
protected:
    virtual void PixelToWorld(double x, double y, double *x_out, double *y_out);
};


#endif

