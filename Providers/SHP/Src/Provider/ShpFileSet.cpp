/*
 * 
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

#include "stdafx.h"

#include <FdoCommonStringUtil.h>
#include <FdoCommonFile.h>

#include "ShpFileSet.h"

#define SHP_DO_COMPRESSION	true
#define SHP_SAVE_EXT		L"_save"

FdoCommonThreadMutex ShpFileSet::mMutex;
extern std::vector<std::wstring> ShpConnGlobalFilesToCompress;

static bool match (const wchar_t* name, size_t length, const wchar_t* base, size_t base_length, const wchar_t* pattern, size_t pattern_length)
{
    bool ret;

    ret = false;

    if (0 == wcsncmp (base, name, base_length))
        if (base_length + pattern_length == length)
            if (0 == FdoCommonOSUtil::wcsicmp (name + length - pattern_length, pattern))
                ret = true;

    return (ret);
}

ShpFileSet::ShpFileSet (FdoString* base_name, FdoString* tmp_dir) :
    mTmpDir (tmp_dir ),
    mShp (NULL),
    mDbf (NULL),
    mShx (NULL),
    mPrj (NULL),
    mSSI (NULL),
	mCpg (NULL),
    mShpC (NULL),
    mDbfC (NULL),
    mShxC (NULL),
    mSSIC (NULL),
	mIsSSITempFile(false)
{
    int status;
    size_t length;
    wchar_t* name;
    wchar_t* dir;
    std::vector<std::wstring> files;
    int count;
    wchar_t* p;
    size_t base_length;
    wchar_t* shp_file;
    wchar_t* dbf_file;
    wchar_t* shx_file;
    wchar_t* idx_file;
    wchar_t* prj_file;
	wchar_t* cpg_file;

	mFilesExist = true;
	mHasDeletedRecords = false;

    try
    {
        // get the base name and the directory
        length = wcslen (base_name);
        name = (wchar_t*)alloca (sizeof (wchar_t) *(length + 1));
        wcscpy (name, base_name);
        p = &name[length - 1];
        while ((p >= name) && (*p != FILE_PATH_DELIMITER))
            p--;
        if (p >= name)
        {
            p++;
            mBaseName = p;
            dir = (wchar_t*)alloca (sizeof (wchar_t) * ((p - name) + 1));
            wcsncpy (dir, base_name, (p - name));
            dir[(p - name)] = L'\0';
        }
        else
        {
            mBaseName = name;
            dir = (wchar_t*)alloca (sizeof (wchar_t) * 3);
            dir[0] = L'.';
            dir[1] = FILE_PATH_DELIMITER;
            dir[2] = L'\0';
        }

        // scan the directory and match extensions
        FdoCommonFile::GetAllFiles (dir, files);
	    count = (int)files.size ();
        base_length = wcslen (mBaseName);
        shp_file = NULL;
        dbf_file = NULL;
        shx_file = NULL;
        idx_file = NULL;
        prj_file = NULL;
		cpg_file = NULL;

	    for (int i = 0; i < count; i++)
	    {
            const wchar_t* name;
            size_t length;
            std::wstring path;
            
            name = files[i].c_str ();
            length = wcslen (name);
            if      (match (name, length, mBaseName, base_length, SHP_EXTENSION, ELEMENTS(SHP_EXTENSION) - 1))
            {
                shp_file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (dir) + wcslen (name) + 1));
                wcscpy (shp_file, dir);
                wcscat (shp_file, name);
            }
            else if (match (name, length, mBaseName, base_length, DBF_EXTENSION, ELEMENTS(DBF_EXTENSION) - 1))
            {
                dbf_file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (dir) + wcslen (name) + 1));
                wcscpy (dbf_file, dir);
                wcscat (dbf_file, name);
            }
            else if (match (name, length, mBaseName, base_length, SHX_EXTENSION, ELEMENTS(SHX_EXTENSION) - 1))
            {
                shx_file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (dir) + wcslen (name) + 1));
                wcscpy (shx_file, dir);
                wcscat (shx_file, name);
            }
            else if (match (name, length, mBaseName, base_length, IDX_EXTENSION, ELEMENTS(IDX_EXTENSION) - 1))
            {
                idx_file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (dir) + wcslen (name) + 1));
                wcscpy (idx_file, dir);
                wcscat (idx_file, name);
            }
            else if (match (name, length, mBaseName, base_length, PRJ_EXTENSION, ELEMENTS(PRJ_EXTENSION) - 1))
            {
                prj_file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (dir) + wcslen (name) + 1));
                wcscpy (prj_file, dir);
                wcscat (prj_file, name);
            }
            else if (match (name, length, mBaseName, base_length, CPG_EXTENSION, ELEMENTS(CPG_EXTENSION) - 1))
            {
                cpg_file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (dir) + wcslen (name) + 1));
                wcscpy (cpg_file, dir);
                wcscat (cpg_file, name);
            }
        }

        if (NULL != shp_file)
        {
            mShp = new ShapeFile (shp_file);
            if (NULL != shx_file)
                mShx = new ShapeIndex (shx_file);
            else
            {
                shx_file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (base_name) + wcslen (SHX_EXTENSION) + 1));
                wcscpy (shx_file, base_name);
                wcscat (shx_file, SHX_EXTENSION);
                mShx = new ShapeIndex (shx_file, GetShapeFile (), tmp_dir);
            }
        }

        if (NULL != dbf_file)
        {
            mDbf = new ShapeDBF (dbf_file);
            if (NULL != GetShapeIndexFile ())
                // check that the number of features matches
                if (GetDbfFile ()->GetNumRecords () != GetShapeIndexFile ()->GetNumObjects ())
                    throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_MISMATCH, "The file '%1$ls' is corrupt or does not have the same number of objects as the file '%2$ls'.", dbf_file, shx_file));
        }
        else
        {
            // Try to create the new DBF file in the same location as the SHP file; if this fails, try to create it in the temp folder:

            ColumnInfo* info;
            ShapeDBF* dbf;
            int count;
            RowData* data;

            FdoStringP dbfFileName = FdoStringP::Format(L"%ls%ls", base_name, DBF_EXTENSION);
            info = new (0) ColumnInfo (0);

            bool bIsTempFile = false;
            try
            {
                dbf = new ShapeDBF (dbfFileName, info);
                bIsTempFile = false;
            }
            catch (FdoException *e)
            {
                e->Release();

                // We seem to have failed to create a file in the same location as the SHP file,
                // possibly because the volume is read-only. Try creating the DBF file in a temp location instead:
                wchar_t* temp = NULL;
                if (!FdoCommonFile::GetTempFile (&temp, tmp_dir))
                    throw FdoException::Create (NlsMsgGet(SHP_TEMPORARY_FILE_PROBLEM, "A temporary file could not be created in directory '%1$ls'.", tmp_dir));

                dbfFileName = temp;
                delete[] temp;

                dbf = new ShapeDBF (dbfFileName, info);
                bIsTempFile = true;
            }


            delete dbf;
            delete info;
            // open it (for read)
            mDbf = new ShapeDBF (dbfFileName);

			// Reopen for write
			mDbf->Reopen( FdoCommonFile::IDF_OPEN_UPDATE);

            mDbf->SetTemporaryFile(bIsTempFile);

			mDbf->PutFileHeaderDetails ();
            if (NULL != GetShapeIndexFile ())
            {
                // fill it with empty records
                count = GetShapeIndexFile ()->GetNumObjects ();
                data = mDbf->NewRow ();
                for (int i = 0; i < count; i++)
                    mDbf->SetRowAt (data, i, true);
                delete data;

                mDbf->PutFileHeaderDetails ();
				mDbf->Reopen( FdoCommonFile::IDF_OPEN_READ);
            }
        }

        // if there is a dbf file but no shp file, create a temporary shp with that many null shapes
        if ((NULL == mShp) && (NULL != mDbf))
        {
            wchar_t* temp;
            ShapeFile* shp;
            int count;

            if (FdoCommonFile::GetTempFile (&temp, tmp_dir))
            {
                shp = new ShapeFile (temp, eNullShape, false);
                delete shp;
                // open it (for read by default )
                mShp = new ShapeFile (temp);

				// Reopen for write
				mShp->Reopen( FdoCommonFile::IDF_OPEN_UPDATE);

                delete[] temp;
                mShp->SetTemporaryFile (true);

				mShp->PutFileHeaderDetails ();
                // fill it with empty shapes
                count = GetDbfFile ()->GetNumRecords ();
                NullShape* null = NullShape::NewNullShape (0);
                for (int i = 0; i < count; i++)
                {
                    null->SetRecordNum (i + 1);
                    mShp->SetObjectAt (null, true);
                }
                delete null;
                mShp->PutFileHeaderDetails ();
				mShp->Reopen( FdoCommonFile::IDF_OPEN_READ);

                // create a shx file
                wchar_t* temp;
                if (FdoCommonFile::GetTempFile (&temp, tmp_dir))
                {
                    mShx = new ShapeIndex (temp, GetShapeFile (), tmp_dir);
                    mShx->SetTemporaryFile (true);
                }
                else
                    throw FdoException::Create (NlsMsgGet(SHP_TEMPORARY_FILE_PROBLEM, "A temporary file could not be created in directory '%1$ls'.", tmp_dir));
                delete[] temp;

                // create a spatial index name but defer file creation
                if (FdoCommonFile::GetTempFile (&temp, tmp_dir))
                {
					mSSIFileName = temp;
					mIsSSITempFile = true;
                }
                else
                    throw FdoException::Create (NlsMsgGet(SHP_TEMPORARY_FILE_PROBLEM, "A temporary file could not be created in directory '%1$ls'.", tmp_dir));
                delete[] temp;
            }
            else
                throw FdoException::Create (NlsMsgGet(SHP_TEMPORARY_FILE_PROBLEM, "A temporary file could not be created in directory '%1$ls'.", tmp_dir));
        }
        else
        {
            // create a spatial index name but defer file creation
            if (NULL == idx_file)
            {
                idx_file = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (base_name) + wcslen (IDX_EXTENSION) + 1));
                wcscpy (idx_file, base_name);
                wcscat (idx_file, IDX_EXTENSION);
            }
			mSSIFileName = idx_file;
        }

        if (NULL != prj_file)
            mPrj = new ShapePRJ (prj_file, status);
        // else
        //     it's not an error if it doesn't exist

        if (NULL != cpg_file)
            mCpg = new ShapeCPG (cpg_file, status);

	}
    catch (FdoException* ge)
    {
        if (NULL != mShp)
            delete mShp;
        if (NULL != mDbf)
            delete mDbf;
        if (NULL != mShx)
            delete mShx;
        if (NULL != mSSI)
            delete mSSI;
        if (NULL != mPrj)
            delete mPrj;
        if (NULL != mCpg)
            delete mCpg;
        throw ge;
    }
}

ShpFileSet::~ShpFileSet (void)
{
	if ( mFilesExist )
	{
		ReopenFileset( FdoCommonFile::IDF_OPEN_READ );
	}

	// Remember to compress this file set (on final connection Close())
	if ( SHP_DO_COMPRESSION && mHasDeletedRecords &&
		 !mDbf->IsTemporaryFile() && !mShx->IsTemporaryFile() && 
		 !mShp->IsTemporaryFile() && (mSSI && !mSSI->IsTemporaryFile()))
	{
		FdoStringP	fullName = FdoStringP(mDbf->FileName()).Left(DBF_EXTENSION);
		bool		found = false;


		// Add this file set to the list in case not already there.
		mMutex.Enter();

		for (size_t i = 0; i < ShpConnGlobalFilesToCompress.size() && !found; i++ )
		{
			found = ( wcscmp((FdoString *)fullName, ShpConnGlobalFilesToCompress[i].c_str() ) == 0 );
		}

		if ( !found )	
			ShpConnGlobalFilesToCompress.push_back((FdoString *)fullName);
		
		mMutex.Leave();
	}

    delete mShp;
    delete mDbf;
    delete mShx;
    delete mPrj;
    delete mSSI;
	delete mCpg;
}

FdoString* ShpFileSet::CreateBaseName (FdoString* name)
{
#ifdef _WIN32

    wchar_t drive[_MAX_DRIVE];
    wchar_t dir[_MAX_DIR];
    wchar_t file[_MAX_PATH];
    wchar_t ext[_MAX_EXT];
    size_t length;
    wchar_t* ret;

    _wsplitpath (name, drive, dir, file, ext);
    length = wcslen (drive) + wcslen (dir) + wcslen (file);
    ret = new wchar_t[length + 1];
    _snwprintf (ret, length + 1, L"%s%s%s", drive, dir, file);

#else

    size_t length;
    wchar_t* part;
    wchar_t* p;
    wchar_t* ret;

    length = wcslen (name);
    part = (wchar_t*)alloca (sizeof (wchar_t) * (length + 1));
    wcscpy (part, name);
    p = &part[length - 1];
    while ((p >= part) && (*p != L'.'))
        p--;
    if (p >= part)
        *p = L'\0';
    length = wcslen (part);
    ret = new wchar_t[length + 1];
    wcscpy (ret, part);

#endif

    return (ret);
}

int ShpFileSet::GetNumRecords ()
{
    return (GetDbfFile ()->GetNumRecords ());
}

ShpSpatialIndex* ShpFileSet::GetSpatialIndex ( bool populateRtree )
{
	// Create the Spatial index object 
	if ( mSSI == NULL && populateRtree )
	{
        mSSI = new ShpSpatialIndex (mSSIFileName, mTmpDir, GetShapeIndexFile ()->GetFileShapeType (), GetShapeIndexFile ()->HasMData ());
        mSSI->SetTemporaryFile (mIsSSITempFile);

		if (!mSSI->IsNew())
		{
			if ( (int)mSSI->GetNObjects() > GetShapeIndexFile ()->GetNumObjects() )
			{   
				// close the existing one and try again
				wchar_t *idx_file = (wchar_t*)alloca (sizeof (wchar_t) * (1 + wcslen (GetSpatialIndex ()->FileName ())));
				wcscpy (idx_file, GetSpatialIndex ()->FileName ());
				mSSI->CloseFile ();
				if (!FdoCommonFile::Delete (idx_file))
					throw FdoException::Create (NlsMsgGet(SHP_CONNECTION_FILE_MISMATCH, "The file '%1$ls' is corrupt or does not have the same number of objects as the file '%2$ls'.", 
												idx_file, (FdoString *)GetShapeIndexFile ()->FileName()));
				else
				{
					delete mSSI;
					mSSI = new ShpSpatialIndex (idx_file, mTmpDir, GetShapeIndexFile ()->GetFileShapeType (), GetShapeIndexFile ()->HasMData ());
					PopulateRTree ();
				}
			}
		}
		else 
		{
			PopulateRTree();
		}
	}
	return (mSSI);
}

void ShpFileSet::PopulateRTree ()
{
    ULONG	    offset;
    int         length;
    eShapeTypes type;
    Shape*      shape;
    BoundingBoxEx box;

    for (int i = 0; i < GetShapeIndexFile ()->GetNumObjects (); i++)
    {
        GetShapeIndexFile ()->GetObjectAt (i, offset, length);
        shape = GetShapeFile ()->GetObjectAt (offset, type);
        if (eNullShape != type)
        {
            shape->GetBoundingBoxEx (box);
            GetSpatialIndex ()->InsertObject (type, &box, i);
        }
        delete shape;
    }
}

void ShpFileSet::GetObjectAt (RowData** row, eShapeTypes& type, Shape** shape, int nRecordNumber)
{
    ULONG offset;
    int length;

    // read the attributes. The requested row might not exist.
    *row = GetDbfFile ()->GetRowAt (nRecordNumber);
    if ((*row) && !(*row)->IsDeleted () && shape )
    {
        // seek to the shape offset
        GetShapeIndexFile ()->GetObjectAt (nRecordNumber, offset, length);
        *shape = GetShapeFile ()->GetObjectAt (offset, type);
    }
}

bool ShpFileSet::AdjustExtents (Shape* shape, bool remove, bool useCopyFiles)
{
    ShapeFile* shp;
    ShapeIndex* shx;
    ShpSpatialIndex* ssi;
    int record;
    BoundingBoxEx box;
    BoundingBoxEx before;
    ULONG offset;
    int length;
    eShapeTypes type;
    Shape* old_shape;
    BoundingBoxEx old_box;
    BoundingBoxEx after;
    bool ret;

    ret = true; // assume we have to change the headers

	shp = useCopyFiles? GetShapeFileC () : GetShapeFile ();
	shx = useCopyFiles? GetShapeIndexFileC () : GetShapeIndexFile ();
	ssi = useCopyFiles? GetSpatialIndexC () : GetSpatialIndex (true);

    shape->GetBoundingBoxEx (box);
    ssi->GetSSIExtent (before);
    record = shape->GetRecordNum () - 1;
    if (record < shx->GetNumObjects ())
    {   // update or delete,
        // so delete the old bounding box from the spatial index
        shx->GetObjectAt (record, offset, length);
        old_shape = shp->GetObjectAt (offset, type);
        try
        {
            if (eNullShape != old_shape->GetShapeType ())
            {
                old_shape->GetBoundingBoxEx (old_box);
                if (remove || (box != old_box))
                    ssi->DeleteObject (&old_box, record);
                else
                    ret = false; // extents the same, no update to headers
            }
            else if (eNullShape == shape->GetShapeType ())
                ret = false; // both null, no update to headers
        }
        catch (...)
        {
            delete old_shape;
            throw;
        }
        delete old_shape;
        
        if (!remove && ret && (eNullShape != shape->GetShapeType ()))
            ssi->InsertObject (shape->GetShapeType (), &box, record);
        if (ret)
        {
            ssi->GetSSIExtent (after);
            ret = before != after;
        }
    }
    else
    {   // insert
        if (eNullShape != shape->GetShapeType ())
            ssi->InsertObject (shape->GetShapeType (), &box, record);
        ssi->GetSSIExtent (after);
        ret = before != after;
    }

    if (ret)
    {
        shp->SetBoundingBoxMinX (after.xMin);
        shp->SetBoundingBoxMaxX (after.xMax);
        shp->SetBoundingBoxMinY (after.yMin);
        shp->SetBoundingBoxMaxY (after.yMax);
        shp->SetBoundingBoxMinZ (after.zMin);
        shp->SetBoundingBoxMaxZ (after.zMax);
        shp->SetBoundingBoxMinM (after.mMin);
        shp->SetBoundingBoxMaxM (after.mMax);
        shp->SetHeaderDirty (true);
        shx->SetBoundingBoxMinX (after.xMin);
        shx->SetBoundingBoxMaxX (after.xMax);
        shx->SetBoundingBoxMinY (after.yMin);
        shx->SetBoundingBoxMaxY (after.yMax);
        shx->SetBoundingBoxMinZ (after.zMin);
        shx->SetBoundingBoxMaxZ (after.zMax);
        shx->SetBoundingBoxMinM (after.mMin);
        shx->SetBoundingBoxMaxM (after.mMax);
        shx->SetHeaderDirty (true);
    }

    return (ret);
}

// make the space 'length' long at 'offset' be 'new_length' long
// it's assumed that the contents at 'offset' of length/new_length is/will-be garbage
void ShpFileSet::MakeSpace (int nRecordNumber, ULONG offset, int length, int new_length, bool useCopyFiles)
{
    ShapeFile* shp;
    ShapeIndex* shx;
    int excess;
    int plus;
    int size;
    long read;
    char* buffer;

	shp = useCopyFiles? GetShapeFileC () : GetShapeFile ();
	shx = useCopyFiles? GetShapeIndexFileC () : GetShapeIndexFile ();

    if ((length != new_length) && ((shx->GetNumObjects () - 1) > nRecordNumber))
    {
        if (new_length > length)
        {
            // expand
            excess = new_length - length;
            offset += length;
            size = BUFFER_SIZE + excess;
            buffer = new char[size];
            shp->SetFilePointer64 ((FdoInt64)offset);
            plus = 0;
            while (shp->ReadFile (buffer + plus, size, &read) && (size == read))
            {
                shp->SetFilePointer64 ((FdoInt64)(offset + excess));
                shp->WriteFile (buffer, BUFFER_SIZE);
                memmove (buffer, buffer + BUFFER_SIZE, excess);
                offset += BUFFER_SIZE;
                size = BUFFER_SIZE;
                plus = excess;
                shp->SetFilePointer64 ((FdoInt64)(offset + excess));
            }
            if (0 != read)
            {
                shp->SetFilePointer64 ((FdoInt64)(offset + excess));
                shp->WriteFile (buffer, plus + read);
            }
            delete [] buffer;
        }
        else
        {
            // contract
            excess = length - new_length;
            offset += new_length;
            buffer = new char[BUFFER_SIZE];
            shp->SetFilePointer64 ((FdoInt64)(offset + excess));
            while (shp->ReadFile (buffer, BUFFER_SIZE, &read) && (BUFFER_SIZE == read))
            {
                shp->SetFilePointer64 ((FdoInt64)offset);
                shp->WriteFile (buffer, BUFFER_SIZE);
                offset += BUFFER_SIZE;
                shp->SetFilePointer64 ((FdoInt64)(offset + excess));
            }
            if (0 != read)
            {
                shp->SetFilePointer64 ((FdoInt64)offset);
                shp->WriteFile (buffer, read);
            }
            delete [] buffer;
        }
    }
    // else, nothing to do
}

void ShpFileSet::SetObjectAt (RowData* row, Shape* shape, bool batch, bool useCopyFiles)
{
    ShapeFile* shp;
    ShapeDBF* dbf;
    ShapeIndex* shx;
    int record;
    eShapeTypes shape_type;
    eShapeTypes file_type;
    int count;
    ULONG before;
    ULONG after;

	shp = useCopyFiles? GetShapeFileC () : GetShapeFile ();
	dbf = useCopyFiles? GetDbfFileC () : GetDbfFile ();
	shx = useCopyFiles? GetShapeIndexFileC () : GetShapeIndexFile ();

    record = shape->GetRecordNum () - 1;

    // handle conversion from multipointX to pointX:
    // at schema creation time, we assumed the user wanted multipoint,
    // but now we can set the type we are really inserting
    shape_type = shape->GetShapeType ();
    if (eNullShape != shape_type) // null shapes are always allowed
    {
        file_type = shp->GetFileShapeType ();
        if (shape_type != file_type)
        {
            if (0 == shx->GetNumObjects ())
            {
                bool ok;

                ok = false;
                switch (file_type)
                {
                    case eMultiPointShape:
                        if (shape_type == ePointShape)
                            ok = true;
                        break;
                    case eMultiPointMShape:
                        if (shape_type == ePointMShape)
                            ok = true;
                        break;
                    case eMultiPointZShape:
                        if (shape_type == ePointZShape)
                            ok = true;
                        break;
                    default:
                        break;
                }
                if (ok)
                {
                    // if we're writing the first object and it's just a non-multipoint
                    // of the shape type of the file so update the header
                    shp->SetFileShapeType (shape_type);
                    shp->SetHeaderDirty (true);
                    shx->SetFileShapeType (shape_type);
                    shx->SetHeaderDirty (true);
                }
                else
                    // otherwise complain
                    throw FdoException::Create (NlsMsgGet(SHP_INVALID_GEOMETRY_TYPE, "The geometry type ('%1$ls') of the feature conflicts with the shape file type ('%2$ls').", ShapeFileBase::ShapeTypeToString (shape_type), ShapeFileBase::ShapeTypeToString (file_type)));
            }
            else
                throw FdoException::Create (NlsMsgGet(SHP_INVALID_GEOMETRY_TYPE, "The geometry type ('%1$ls') of the feature conflicts with the shape file type ('%2$ls').", ShapeFileBase::ShapeTypeToString (shape_type), ShapeFileBase::ShapeTypeToString (file_type)));
        }
    }

    // handle the switch to M valued Z type files
    if (0 == shx->GetNumObjects ())
    {
        shp->SetHasMData (shape->HasMeasure ());
        shx->SetHasMData (shape->HasMeasure ());
    }

    dbf->SetRowAt (row, record, batch);
    count = shx->GetNumObjects ();
    if (record < count)
    {
        // update
        ULONG offset;
        int length;
        int new_length;

        AdjustExtents (shape, false, useCopyFiles);
        shx->GetObjectAt (record, offset, length);
        new_length = shape->GetContentLength ();
        new_length *= WORD_SIZE_IN_BYTES;
        MakeSpace (record, offset, length + sizeof(SHPRecordHeader), new_length + sizeof(SHPRecordHeader), useCopyFiles);
        shp->SetFilePointer64 ((FdoInt64)offset);
        shp->SetObjectAt (shape, batch);
        // update shx file to match
        #pragma message ("ToDo: more efficient shx file update")
        new_length = new_length - length;
        if (0 != new_length)
        {
            shx->GetObjectAt (record, offset, length);
            length += new_length;
            shx->SetObjectAt (record, offset, length, batch);
            for (int i = record + 1; i < count; i++)
            {
                shx->GetObjectAt (i, offset, length);
                offset += new_length;
                shx->SetObjectAt (i, offset, length, batch);
            }
        }
    }
    else
    {
        // append
        shp->SetFilePointer64 ((FdoInt64)0, FdoCommonFile::FILE_POS_END);
        before = shp->GetFileLength ();
        // do adjustment before writing the shape so SetHasMData() reports what would be true
        AdjustExtents (shape, false, useCopyFiles);
        shp->SetObjectAt (shape, batch);
        after = shp->GetFileLength ();
        shx->SetObjectAt (record, before * WORD_SIZE_IN_BYTES, ((after - before) * WORD_SIZE_IN_BYTES) - sizeof(SHPRecordHeader), batch);
    }

    if (!batch)
        Flush (useCopyFiles);
}

void ShpFileSet::DeleteObjectAt (int nRecordNumber)
{
    ULONG	    offset;
    int         length;
    eShapeTypes type;
    Shape*      shape;
    BoundingBoxEx box;
    BoundingBoxEx before;
    BoundingBoxEx after;

    // delete the attribute record
    // this marks it as deleted for all the other files
    GetDbfFile ()->DeleteRowAt (nRecordNumber);

	mHasDeletedRecords = true;

    // update the spatial index
    GetShapeIndexFile ()->GetObjectAt (nRecordNumber, offset, length);
    shape = GetShapeFile ()->GetObjectAt (offset, type);
    try
    {
        AdjustExtents (shape, true, false);
    }
    catch (...)
    {
        delete shape;
        throw;
    }
    delete shape;
}

void ShpFileSet::Flush (bool useCopyFiles)
{
    ShapeFile* shp;
    ShapeDBF* dbf;
    ShapeIndex* shx;

	shp = useCopyFiles? GetShapeFileC () : GetShapeFile ();
	dbf = useCopyFiles? GetDbfFileC () : GetDbfFile ();
	shx = useCopyFiles? GetShapeIndexFileC () : GetShapeIndexFile ();

    if (shp->IsHeaderDirty ())
        shp->PutFileHeaderDetails ();
    if (shx->IsHeaderDirty ())
        shx->PutFileHeaderDetails ();
    if (dbf->IsHeaderDirty ())
        dbf->PutFileHeaderDetails ();
}

/// <summary>Gets data in a row buffer and sends it to the dbf file.</summary>
/// <param name="connection">The connection object.</param> 
/// <param name="class_name">The class of object to be inserted.</param> 
/// <param name="values">The data values to be inserted.</param> 
/// <param name="row">The attribute values.</param> 
/// <param name="shape">The geometry.</param> 
/// <param name="batch">If true, don't write headers.</param> 
/// <remark>The feature id is taken from the shape record number.</remark> 
void ShpFileSet::PutData (ShpConnection* connection, FdoString* class_name, FdoPropertyValueCollection* values, RowData* row, Shape* shape, bool batch)
{
    ColumnInfo* info;
    int count;
    FdoPtr<FdoClassDefinition> logical;
    FdoPtr<FdoDataPropertyDefinitionCollection> identities;
    FdoPtr<FdoDataPropertyDefinition> id;
    FdoString* identity;
    FdoPtr<FdoPropertyDefinitionCollection> properties;
    FdoPtr<FdoPropertyDefinition> definition;
    FdoPtr<ShpLpPropertyDefinitionCollection> physical;
    FdoInt32 num;
    FdoPtr<FdoPropertyValue> value;
    FdoString* name;
    FdoPtr<ShpLpPropertyDefinition> property;
    const wchar_t* column;
    FdoPtr<FdoValueExpression> expression;
    bool assigned;
    eDBFColumnType type;
    FdoGeometryValue* geometry;
    FdoStringValue* string;
    FdoDecimalValue* decimal;
    FdoInt32Value* int32;
    FdoDateTimeValue* datetime;
    FdoDateTime _datetime;
    Date date;
    FdoBooleanValue* boolean;

    try
    {
        info = row->GetColumnInfo ();
        count = info->GetNumColumns ();

        // for each specified property find it and assign it
        logical = ShpSchemaUtilities::GetLogicalClassDefinition (connection, class_name, NULL);
        identities = logical->GetIdentityProperties ();
        id = identities->GetItem (0);
        identity = id->GetName ();
        properties = logical->GetProperties ();
        physical = FdoPtr<ShpLpClassDefinition> (ShpSchemaUtilities::GetLpClassDefinition (connection, class_name))->GetLpProperties ();
        num = values->GetCount ();
        for (FdoInt32 i = 0; i < num; i++)
        {
            value = values->GetItem (i);
            name = FdoPtr<FdoIdentifier> (value->GetName ())->GetName ();
            definition = properties->FindItem (name);
            if (definition == NULL)
                throw FdoException::Create (NlsMsgGet(SHP_COMMAND_PROPERTY_NOT_FOUND, "Property '%1$ls' is not part of class '%2$ls'.", name, class_name));
            if (0 == wcscmp (name, identity))
                throw FdoException::Create (NlsMsgGet(SHP_FEATID_READONLY, "The property '%1$ls' is read-only.", identity));
            switch (definition->GetPropertyType ())
            {
                case FdoPropertyType_DataProperty:
                    property = physical->FindItem (name);
                    column = property->GetPhysicalColumnName ();
                    expression = value->GetValue ();
#pragma message ("TODO: Handle NULL expressions for batch parameter values.")
                    assigned = false;
                    for (int j = 0; !assigned && (j < count); j++)
                    {
                        if (0 == wcscmp (column, info->GetColumnNameAt (j)))
                        {
                            type = info->GetColumnTypeAt (j);
                            switch (type)
                            {
                                case kColumnCharType:
                                    string = dynamic_cast<FdoStringValue*>(expression.p);
                                    if ((string == NULL) && (expression != NULL))
                                        throw FdoException::Create (NlsMsgGet(SHP_INVALID_DATA_TYPE, "The value for property '%1$ls' is not '%2$ls'.", name, FdoCommonMiscUtil::FdoDataTypeToString (ShpSchemaUtilities::DbfTypeToFdoType (kColumnCharType))));
                                    if ((string == NULL) || string->IsNull())
                                        row->SetData (j, true,  (wchar_t*)NULL);
                                    else
									{
										FdoStringP  shpCpg;
										if (this->GetCpgFile())
											shpCpg = this->GetCpgFile()->GetCodePage();
                                        row->SetData (j, false, (wchar_t*)string->GetString (), (wchar_t*)(FdoString *)shpCpg);
									}
                                    break;

                                case kColumnDecimalType:
                                    // NOTE: this could either be a decimal or int32 value
                                    decimal = dynamic_cast<FdoDecimalValue*>(expression.p);
                                    int32 = dynamic_cast<FdoInt32Value*>(expression.p);

                                    if ((decimal == NULL) && (int32 == NULL) && (expression != NULL))
                                        throw FdoException::Create (NlsMsgGet(SHP_INVALID_DATA_TYPE, "The value for property '%1$ls' is not '%2$ls'.", name, FdoCommonMiscUtil::FdoDataTypeToString (ShpSchemaUtilities::DbfTypeToFdoType (kColumnDecimalType))));

                                    if ( ((int32 == NULL) || (int32->IsNull())) && ((decimal == NULL) || (decimal->IsNull())) )
                                        row->SetData (j, true, 0.0);
                                    else if (int32 != NULL)
                                        row->SetData (j, false, (double)int32->GetInt32 ());
                                    else if (decimal != NULL)
                                        row->SetData (j, false, decimal->GetDecimal ());
                                    break;

                                case kColumnDateType:
                                    datetime = dynamic_cast<FdoDateTimeValue*>(expression.p);
                                    if ((datetime == NULL) && (expression != NULL))
                                        throw FdoException::Create (NlsMsgGet(SHP_INVALID_DATA_TYPE, "The value for property '%1$ls' is not '%2$ls'.", name, FdoCommonMiscUtil::FdoDataTypeToString (ShpSchemaUtilities::DbfTypeToFdoType (kColumnDateType))));
                                    if ((datetime == NULL) || datetime->IsNull())
                                        row->SetData(j, true, Date());
                                    else
                                    {
                                        _datetime = datetime->GetDateTime ();
                                        date.nYear = _datetime.year;
                                        date.nMonth = _datetime.month;
                                        date.nDay = _datetime.day;
                                        row->SetData (j, false, date);
                                    }
                                    break;

                                case kColumnLogicalType:
                                    boolean = dynamic_cast<FdoBooleanValue*>(expression.p);
                                    if ((boolean == NULL) && (expression != NULL))
                                        throw FdoException::Create (NlsMsgGet(SHP_INVALID_DATA_TYPE, "The value for property '%1$ls' is not '%2$ls'.", name, FdoCommonMiscUtil::FdoDataTypeToString (ShpSchemaUtilities::DbfTypeToFdoType (kColumnLogicalType))));
                                    if ((boolean == NULL) || boolean->IsNull())
                                        row->SetData (j, true, false);
                                    else
                                        row->SetData (j, false, boolean->GetBoolean ());
                                    break;

                                default:
                                    throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"UNKNOWN"));
                            }
                            assigned = true;
                        }
                    }
                    if (!assigned)
                        throw FdoException::Create (NlsMsgGet(SHP_COMMAND_PROPERTY_NOT_FOUND, "Property '%1$ls' is not part of class '%2$ls'.", name, class_name));
                    break;
                case FdoPropertyType_ObjectProperty:
                    throw FdoException::Create (NlsMsgGet(SHP_OBJECT_PROPERTIES_NOT_SUPPORTED, "Object properties are not supported."));
                    break;
                case FdoPropertyType_GeometricProperty:
                    expression = value->GetValue ();
                    if (expression == NULL)
                    {
                        Shape* sh = NullShape::NewNullShape (shape->GetRecordNum ());
                        delete shape;
                        shape = sh;
                    }
                    else
                    {
                        geometry = dynamic_cast<FdoGeometryValue*>(expression.p);
                        if (geometry == NULL)
                            throw FdoException::Create(NlsMsgGet(SHP_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", L"Geometry", name));
                        if (!geometry->IsNull ()) // geometries are allowed to be null
                        {
                            Shape* sh = ShapeFile::ShapeFromGeometry (FdoPtr<FdoByteArray>(geometry->GetGeometry ()), shape->GetRecordNum () - 1);
                            delete shape;
                            shape = sh;
                        }
                        else
                        {
                            Shape* sh = NullShape::NewNullShape (shape->GetRecordNum ());
                            delete shape;
                            shape = sh;
                        }
                    }
                    break;
                }
        }
        SetObjectAt (row, shape, batch);
    }
    catch (...)
    {
        delete row;
        delete shape;
        throw;
    }
    delete row;
    delete shape;
}

void ShpFileSet::ReopenFileset( FdoCommonFile::OpenFlags flags )
{
	if ( mFilesExist && flags & FdoCommonFile::IDF_OPEN_UPDATE )
	{
		if (this->GetDbfFile ()->IsReadOnly ()) 
			this->GetDbfFile ()->Reopen( flags );

		if (this->GetShapeFile ()->IsReadOnly ()) 
			this->GetShapeFile()->Reopen( flags );

		if (this->GetShapeIndexFile ()->IsReadOnly ()) 
			this->GetShapeIndexFile()->Reopen( flags );
		
		if (this->GetSpatialIndex (true)->IsReadOnly ()) 
			this->GetSpatialIndex()->Reopen( flags );
	}
	else if ( mFilesExist && flags & FdoCommonFile::IDF_OPEN_READ )
	{
		if (!this->GetDbfFile ()->IsReadOnly ()) 
			this->GetDbfFile ()->Reopen( flags );

		if (!this->GetShapeFile ()->IsReadOnly ())
			this->GetShapeFile()->Reopen( flags );

		if (!this->GetShapeIndexFile ()->IsReadOnly ()) 
			this->GetShapeIndexFile()->Reopen( flags );
		
		if (this->GetSpatialIndex() && !this->GetSpatialIndex ()->IsReadOnly ())
		{
			if ( !this->GetSpatialIndex ()->IsTemporaryFile() ) 
			{
				this->GetSpatialIndex()->WriteSSIHeader();
				this->GetSpatialIndex()->FlushNodeCache(TRUE);
			}
			this->GetSpatialIndex()->Reopen( flags );
		}
	}
}

void ShpFileSet::FlushFileset()
{
	FdoCommonFile::OpenFlags	flag = FdoCommonFile::IDF_OPEN_UPDATE;

	// Do flush only when the file is open for write
	if ( mFilesExist )
	{
		if (!this->GetDbfFile ()->IsReadOnly ()) 
			this->GetDbfFile ()->Reopen( flag );

		if (!this->GetShapeFile ()->IsReadOnly ())
			this->GetShapeFile()->Reopen( flag );

		if (!this->GetShapeIndexFile ()->IsReadOnly ()) 
			this->GetShapeIndexFile()->Reopen( flag );
		
		if (this->GetSpatialIndex() && !this->GetSpatialIndex ()->IsReadOnly ())
		{
			if ( !this->GetSpatialIndex ()->IsTemporaryFile() ) 
			{
				this->GetSpatialIndex()->WriteSSIHeader();
				this->GetSpatialIndex()->FlushNodeCache(TRUE);
			}
			this->GetSpatialIndex()->Reopen( flag );
		}
	}
}

void ShpFileSet::SetFilesDeleted() 
{ 
	mFilesExist = false; 
}

bool ShpFileSet::IsWritable()
{
    bool bWritable = true;

    // Note that FdoCommonFile::IsReadOnly() == true means the file was opened in read-only mode,
    // it does not imply the file is not writable by the current process.
    if (GetShapeFile()->IsReadOnly())
    {
	    try
	    {
		    ReopenFileset( FdoCommonFile::IDF_OPEN_UPDATE );
	    }
	    catch (FdoException* ex)
	    {
            ex->Release();
		    bWritable = false;
	    }

	    // Revert to read-only state:
	    ReopenFileset( FdoCommonFile::IDF_OPEN_READ );
    }

    return bWritable;
}
