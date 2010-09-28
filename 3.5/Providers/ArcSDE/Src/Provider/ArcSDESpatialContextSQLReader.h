/*
 * (C) Copyright 2004 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 */

#ifndef ARCSDESPATIALCONTEXTSQLREADER_H
#define ARCSDESPATIALCONTEXTSQLREADER_H

class ArcSDESpatialContextSQLReader :
    public FdoISpatialContextReader
{
public:
    // 0-argument constructor to please FdoPtr::operator->() :
    ArcSDESpatialContextSQLReader() { ArcSDESpatialContextSQLReader(NULL, NULL); }

    ArcSDESpatialContextSQLReader(ArcSDEConnection *connection, FdoISQLDataReader* sqlReader);
    virtual ~ArcSDESpatialContextSQLReader(void);

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
    void Validate(void);
    FdoStringP AdjustColumnName(FdoString *name);

    FdoPtr<ArcSDEConnection> mConnection;
    FdoPtr<FdoISQLDataReader> mSqlReader;
    bool mOnRowOfData;

    // cache for the current Spatial Context:
    FdoStringP mSpatialContextName;
    FdoStringP mDescription;
    FdoStringP mCoordSysWkt;
    FdoStringP mCoordSysName;
};

#endif // ARCSDESPATIALCONTEXTSQLREADER_H
