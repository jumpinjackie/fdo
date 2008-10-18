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

#ifndef SHAPEFILEBASE_H
#define SHAPEFILEBASE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <FdoCommonFile.h>

class ShapeFileBase : public FdoCommonFile
{
protected:

    // used to decode 64-bit reals
    typedef union
    {
        double fl;
        unsigned long i[2];
    } DoubleLong;

    int m_nFileCode;
    ULONG m_nFileLength;
    int m_nFileVersion;
    eShapeTypes m_nFileShapeType;
    bool m_bMDataPresent;

    double m_dXMin;
    double m_dXMax;
    double m_dYMin;
    double m_dYMax;
    double m_dZMin;
    double m_dZMax;
    double m_dMMin;
    double m_dMMax;

    bool mHeaderDirty;

public:
    ShapeFileBase ();
    virtual ~ShapeFileBase (void);

    void OpenRead (const wchar_t* name);  
    void OpenWrite (const wchar_t* name, eShapeTypes shape_type, bool has_m);
	void Reopen(OpenFlags openFlags);

    // Bounding box methods
    double GetBoundingBoxMinX ()     { return m_dXMin; }
    double GetBoundingBoxMaxX ()     { return m_dXMax; }
    double GetBoundingBoxMinY ()     { return m_dYMin; }
    double GetBoundingBoxMaxY ()     { return m_dYMax; }
    double GetBoundingBoxMinZ ()     { return m_dZMin; }
    double GetBoundingBoxMaxZ ()     { return m_dZMax; }
    double GetBoundingBoxMinM ()     { return m_dMMin; }
    double GetBoundingBoxMaxM ()     { return m_dMMax; }
    void SetBoundingBoxMinX (double d)    { m_dXMin = d; }
    void SetBoundingBoxMaxX (double d)    { m_dXMax = d; }
    void SetBoundingBoxMinY (double d)    { m_dYMin = d; }
    void SetBoundingBoxMaxY (double d)    { m_dYMax = d; }
    void SetBoundingBoxMinZ (double d)    { m_dZMin = d; }
    void SetBoundingBoxMaxZ (double d)    { m_dZMax = d; }
    void SetBoundingBoxMinM (double d)    { m_dMMin = d; }
    void SetBoundingBoxMaxM (double d)    { m_dMMax = d; }

    // General information methods
    int GetFileCode()               { return m_nFileCode; }
    ULONG GetFileLength()             { return m_nFileLength; }
    int GetFileVersion()            { return m_nFileVersion; }
    eShapeTypes GetFileShapeType()          { return m_nFileShapeType; }
    void SetFileShapeType(eShapeTypes type) { m_nFileShapeType = type; }
    bool HasMData ()                { return (m_bMDataPresent); }
    void SetHasMData (bool has_m)   { m_bMDataPresent = has_m; }

    // Header methods
    void GetFileHeaderDetails ();
    void PutFileHeaderDetails ();
    void CheckBoundingBox(double dMinValue, double dMaxValue, eMinMaxTypes eMinMaxType);
    void SetHeaderDirty (bool dirty) { mHeaderDirty = dirty; }
    bool IsHeaderDirty ()            { return (mHeaderDirty); }

    static wchar_t* ShapeTypeToString (eShapeTypes type);
};

#endif // SHAPEFILEBASE_H

