#ifndef FDOSMPHSYNONYM_H
#define FDOSMPHSYNONYM_H        1
/*
 * (C) Copyright 2011 by Autodesk, Inc. All Rights Reserved.
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
#ifdef _WIN32
#pragma once
#endif

class FdoSmPhRdDbObjectReader;
class FdoSmPhRdSynonymReader;

// represents a Synonym.
// Base class for RDBMS-specific implementations.
class FdoSmPhSynonym   : virtual public FdoSmPhDbObject
{
public:
    FdoSmPhSynonym(
        FdoStringP name,                            // synonym name
        FdoSmPhDbObjectP rootObject,                // the object referenced by this synonym.
                                                    // This parameter is mandatory for new synonyms
                                                    // It must be NULL for other synonyms. In this case
                                                    // the referenced object is read from the RDBMS.
        const FdoSmPhOwner* pOwner,                 // datastore containing the synonym
		FdoSchemaElementState elementState = FdoSchemaElementState_Added,
                                                    // Indicates whether synonym already exists.
        FdoSmPhRdDbObjectReader* reader = NULL      // Provides info for existing synonyms
    );
    ~FdoSmPhSynonym();

    // Get the database object referenced by this synonym
    FdoSmPhDbObjectP GetRootObject();

    // The following 4 getter functions are overridden to get these
    // components from the synonym's base object.
    // The data dictionary, for most RDBMS's, does not store direct references 
    // from synonyms to their components. 
    virtual FdoSmPhColumnsP GetColumns();

    virtual FdoSmPhColumnsP GetPkeyColumns();

    virtual FdoPtr<FdoSmPhIndexCollection> GetIndexes();

    virtual FdoSmPhFkeysP GetFkeysUp();

    /// Get the current long transaction mode
    virtual FdoLtLockModeType GetLtMode() const;

    /// Get the current locking mode
    virtual FdoLtLockModeType GetLockingMode() const;

    /// Returns an array of all types of locking supported on this Database Object.
    /// size is set to the size of the array.
    /// Returns NULL when size is 0.
	virtual const FdoLockType* GetLockTypes(FdoInt32& size) const;

    virtual bool ClassifyObjectType(FdoBoolean classifyDefaultTypes );

    // Returns true if the synonym's base object has been cached
    bool BaseLoaded();

    // Always returns true. This override prevents the Schema Manager from trying to 
    // retrieve columns for synonyms, since a synonym do not explicitly have columns
    virtual bool ColumnsLoaded();

    // Always returns true. This override prevents the Schema Manager from trying to 
    // retrieve indexes for synonyms, since a synonym would never explicitly have an index.
    virtual bool IndexesLoaded();

    // Given a synonym reader, get this synonym's base object from the reader,.
    bool CacheSynonymBaseObjects( FdoPtr<FdoSmPhRdSynonymReader> rdr );

    // No-op override to prevent Schema Manager from trying to process foreign keys
    // for synonyms, since synonyms do not have explicit foreign keys.
    virtual void LoadFkeyRefCands();

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhSynonym() {}

    // Overridden to load synonym base object instead of general dependency.
    virtual void LoadBaseObjects();

private:
    void LoadSynonym();

    bool mSynonymLoaded;

    FdoSmPhColumnsP mEmptyColumns;
    FdoSmPhColumnsP mEmptyPkeyColumns;
    FdoSmPhIndexesP mEmptyIndexes;
    FdoSmPhFkeysP mEmptyFkeys;
};

typedef FdoPtr<FdoSmPhSynonym> FdoSmPhSynonymP;

#endif
