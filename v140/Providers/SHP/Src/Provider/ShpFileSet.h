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

#ifndef SHPFILESET_H
#define SHPFILESET_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <FdoCommonThreadMutex.h>

#include <ShapeFile.h>
#include <ShapeDBF.h>
#include <ShapeIndex.h>
#include <ShpSpatialIndex.h>
#include <ShapePRJ.h>
#include <ShapeCPG.h>

typedef std::map< FdoStringP, std::pair<int, bool> > FileSetRefCounterType;

class ShpConnection;

class ShpFileSet
{
    static const int BUFFER_SIZE = 65536;

    ShapeFile* mShp;
    ShapeDBF* mDbf;
    ShapeIndex* mShx;
    ShapePRJ* mPrj;
    ShpSpatialIndex* mSSI;
	ShapeCPG* mCpg;
    bool mFilesExist;

    FdoStringP mBaseName;
	bool	 mHasDeletedRecords;

	FdoStringP			mTmpDir;
    ShapeFile*			mShpC;
    ShapeDBF*			mDbfC;
    ShapeIndex*			mShxC;
    ShpSpatialIndex*	mSSIC;

	FdoStringP			mSSIFileName;
	bool				mIsSSITempFile;

    // Globals to keep track of FileSet for every SHP file and compress after 
    // delete command. This compression is triggered in destructing of last FileSet.
	static	FdoCommonThreadMutex   mMutex;
    static  FileSetRefCounterType  mGlobalRefCountOfFileSet;

public:
    ShpFileSet (FdoString* shp_file, FdoString* tmp_dir = NULL);
    virtual ~ShpFileSet (void);

    // utility methods
    static FdoString* CreateBaseName (FdoString* name);
    FdoString* GetBaseName ();
    ShapeFile* GetShapeFile ();
    ShapeDBF* GetDbfFile ();
    ShapePRJ* GetPrjFile ();
	ShapeCPG* GetCpgFile ();
    ShapeIndex* GetShapeIndexFile ();
    ShpSpatialIndex* GetSpatialIndex ( bool populateRtree = false );
    int GetNumRecords ();

    void GetObjectAt (RowData** row, eShapeTypes& type, Shape** shape, int nRecordNumber);
    void SetObjectAt (RowData* row, Shape* shape, bool batch = false, bool useCopyFiles = false);
    void DeleteObjectAt (int nRecordNumber);
    void Flush (bool useCopyFiles = false);
    void PutData (ShpConnection* connection, FdoString* class_name, FdoPropertyValueCollection* values, RowData* row, Shape* shape, bool batch = false);
	void ReopenFileset(FdoCommonFile::OpenFlags flags );
	void FlushFileset();
	void SetFilesDeleted();
    bool IsWritable();

	void SetShapeFileC (ShapeFile *shp);
	void SetDbfFileC (ShapeDBF *dbf);
	void SetShapeIndexFileC (ShapeIndex *shx);
	void SetSpatialIndexC (ShpSpatialIndex *ssi);
    ShapeFile* GetShapeFileC ();
    ShapeDBF* GetDbfFileC ();
    ShapeIndex* GetShapeIndexFileC ();
    ShpSpatialIndex* GetSpatialIndexC ();

private:
    /// <summary>Fill the RTree from the shape file contents.</summary>
    /// <returns>Returns nothing.</returns> 
    void PopulateRTree ();

    /// <summary>Expand or contract the disk space for a shape.</summary>
    /// <param name="nRecordNumber">Input the record number of the shape.</param> 
    /// <param name="offset">Input the offset to the shape.</param> 
    /// <param name="length">Input the original length of the shape.</param> 
    /// <param name="new_length">Input the new length of the shape.</param> 
    /// <returns>Returns nothing.</returns> 
    void MakeSpace (int nRecordNumber, ULONG offset, int length, int new_length, bool useCopyFiles);

    /// <summary>Adjust the extents of the files.</summary>
    /// <param name="shape">Input the shape to be inserted or deleted.</param> 
    /// <param name="remove">If true, the shape will be removed from the extents.</param> 
    /// <returns>Returns true if the extents of the file set have changed.</returns> 
    bool AdjustExtents (Shape* shape, bool remove, bool useCopyFiles);

    /// <summary>Compresses a file set (eliminates the deleted rows)</summary>
	/// <param name="baseName">Fileset full path</param> 
	/// <returns>Returns nothing</returns> 
	void CompressFileSet (const wchar_t* baseName);
};

inline FdoString* ShpFileSet::GetBaseName ()
{
    return (mBaseName);
}

inline ShapeFile* ShpFileSet::GetShapeFile ()
{
    return (mShp);
}

inline ShapeDBF* ShpFileSet::GetDbfFile ()
{
    return (mDbf);
}

inline ShapeIndex* ShpFileSet::GetShapeIndexFile ()
{
    return (mShx);
}

inline ShapePRJ* ShpFileSet::GetPrjFile ()
{
    return (mPrj);
}

inline ShapeCPG* ShpFileSet::GetCpgFile ()
{
    return (mCpg);
}

inline void ShpFileSet::SetShapeFileC (ShapeFile *shp)
{
    mShpC = shp;
}

inline void ShpFileSet::SetDbfFileC (ShapeDBF *dbf)
{
    mDbfC = dbf;
}

inline void ShpFileSet::SetShapeIndexFileC (ShapeIndex *shx)
{
    mShxC = shx;
}

inline void ShpFileSet::SetSpatialIndexC (ShpSpatialIndex *ssi)
{
    mSSIC = ssi;
}

inline ShapeFile* ShpFileSet::GetShapeFileC ()
{
    return (mShpC);
}

inline ShapeDBF* ShpFileSet::GetDbfFileC ()
{
    return (mDbfC);
}

inline ShapeIndex* ShpFileSet::GetShapeIndexFileC ()
{
    return (mShxC);
}

inline ShpSpatialIndex* ShpFileSet::GetSpatialIndexC ()
{
    return (mSSIC);
}

#endif // SHPFILESET_H


