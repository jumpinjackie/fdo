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

#ifndef ARCSDESPATIALCONTEXTREADER_H
#define ARCSDESPATIALCONTEXTREADER_H

class ArcSDESpatialContextReader :
    public FdoISpatialContextReader
{
public:
    // 0-argument constructor to please FdoPtr::operator->() :
    ArcSDESpatialContextReader() { ArcSDESpatialContextReader(NULL, false); }

    ArcSDESpatialContextReader(ArcSDEConnection* connection, bool bActiveOnly);
    ArcSDESpatialContextReader(ArcSDEConnection* connection, long lSridOnly);
    virtual ~ArcSDESpatialContextReader(void);

    // methods inherited from FdoISpatialContextReader:
public:
    virtual FdoString* GetName();
    virtual FdoString* GetDescription();
    virtual FdoString* GetCoordinateSystem();
    virtual FdoString* GetCoordinateSystemWkt();
    virtual FdoSpatialContextExtentType GetExtentType();
    virtual FdoByteArray* GetExtent();
    virtual const double GetXYTolerance();
    virtual const double GetZTolerance();
    virtual const bool IsActive();
    virtual bool ReadNext();
    virtual void Dispose();

    // non-exposed public methods:
    virtual long GetSRID();

protected:
    void Init(ArcSDEConnection* connection);
    void Validate(void);

    FdoPtr<ArcSDEConnection> mConnection;
    SE_SPATIALREFINFO *mArrSpatialRefs;
    LONG mlSpatialRefCount;
    LONG mlCurrentSpatialRef;
    long mlActiveOnlySRID;


    // cache for the current Spatial Context:
    FdoStringP mSpatialContextName;
    FdoStringP mDescription;
    FdoStringP mCoordSysWkt;
    FdoStringP mCoordSysName;
};

#endif // ARCSDESPATIALCONTEXTREADER_H

