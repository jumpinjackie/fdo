/*
 * Copyright (C) 2011.  Oyvind Idland.
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

#ifndef ARCSDEGEOMETRYBUFFER_H
#define ARCSDEGEOMETRYBUFFER_H

#include <sdetype.h>


class ArcSDEGeometryBuffer
{
private:
    LONG mShapeType;
    LONG mNumParts;
    LONG mNumSubParts;
    LONG mNumPoints;
    bool mbHasZ;
    bool mbHasM;
  
    DynamicBuffer<LONG> mGeomPartOffsets;
    DynamicBuffer<LONG> mGeomSubpartOffsets;
    DynamicBuffer<SE_POINT> mGeomXY;
    DynamicBuffer<LFLOAT> mGeomPointsZ;
    DynamicBuffer<LFLOAT> mGeomPointsM;

    DynamicBuffer<double> mGeomOrdinates;
    DynamicBuffer<LONG> mGeomOffsets;

    bool GetPointsSegment(int part, int subPart, long &numPointsForSubPart, long &offset);
    void InitBuffers(FdoInt32 dim, int totalNumPoints, SE_POINT** points, LFLOAT** zBuffer, LFLOAT** mBuffer);
    bool FetchOrdinates(DynamicBuffer<double> &destOrdinates, int offset, int count);

public:
    ArcSDEGeometryBuffer();

    bool bHasZ() {return mbHasZ;}
    bool bHasM() {return mbHasM;}
    LONG shapeType() {return mShapeType;}
    int numPoints() {return mNumPoints;}
    int numParts() {return mNumParts;}
    int numSubParts(int partIdx);

    void LoadFromSdeGeometry(SE_SHAPE shape);
    bool GetOrdinates(DynamicBuffer<double> &destOrdinates, int part, int subPart);
    bool GetOrdinates(DynamicBuffer<double> &destOrdinates); // Fetches all ordinates
    bool GetCoorOrdinates(long &count, SE_POINT * &xy, LFLOAT *&z, LFLOAT * &m, int part, int subPart); // Fetches pointers to xy/x/m
    FdoByteArray *ToFGF(FdoFgfGeometryFactory *gf);
    SE_SHAPE FgfToShape(FdoFgfGeometryFactory* gf, FdoByteArray* fgf, SE_CONNECTION sdeConnection, SE_COORDREF coordref, bool bCropToExtents = false);
};






#endif //ARCSDEGEOMETRYBUFFER_H