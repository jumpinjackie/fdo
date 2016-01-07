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
 * Revision Control Modification History
 *
 *         $Id: //providers_reliant_sp2/Shp/src/ShpSpatialIndex/ShpSpatialIndex.h#2 $
 *     $Author: ds $
 *   $DateTime: 2006/06/09 17:03:23 $
 *     $Change: 13170 $
 *
 */

#ifndef SHPSPATIALINDEX_H
#define SHPSPATIALINDEX_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

// constant definitions
const unsigned long SHP_ERROR_SI_MAGIC_NUMBER = 0x41534947;  // SSI identifier ("ASSI")
const int SHP_ERROR_SI_RTREE = 0;                            // R-Tree index identifier
const int MAX_NODE_ENTRIES = 20;                             // max entries per node

// Spatial Index error codes
#define SHP_ERROR_SI_BAD_NODE_EXTENT           -3001
#define SHP_ERROR_SI_BAD_OBJECT_COUNT          -3002
#define SHP_ERROR_SI_BAD_NODE_OFFSET           -3003
#define SHP_ERROR_SI_NODE_UNDERFLOW            -3004
#define SHP_ERROR_SI_CANT_REPLACE_SSI          -3005

// typedefs for DoublePoint, BoundingBox, etc
#include "ShpSpatialIndexFile.h"
#include "ShpSpatialIndexCallBack.h"
#include <DoublePoint.h>
#include <BoundingBox.h>
#include <BoundingBoxEx.h>
#include <Shape.h>

// structure returned by TestSSI()
struct SSITestInfo {
    double spaceEfficiency;            // space efficiency of the R-Tree
    unsigned rTreeHeight;              // height of the R-Tree
    unsigned long nInternalNodes;      // number of internal nodes
    unsigned long nLeafNodes;          // number of leaf nodes
    unsigned long nSHPObjects;         // number of SHP objects
    unsigned long nFreeInternalNodes;  // number of free internal nodes
    unsigned long nFreeLeafNodes;      // number of free leaf nodes
    unsigned internalNodeSize;         // size (in bytes) of internal nodes
    unsigned leafNodeSize;             // size (in bytes) of leaf nodes
};

// forward type declarations 

struct ShpSpatialIndexHeader;
struct ShpSpatialIndexNode;
struct ShpSpatialIndexHandle;
struct SHPObjectInfo;

//-------------------------------------------------------------------------
//
// PURPOSE: Declaration of the ShpSpatialIndex class. The ShpSpatialIndex
//          class implements an R-Tree based spatial indexing of shape
//          files (SHPs).
//
//-------------------------------------------------------------------------
class ShpSpatialIndex : public ShpSpatialIndexFile
{
protected:
    struct SHPObjectInfo {
        unsigned long m_shpOffset;    // SHP file offset of the object
        BoundingBoxEx m_objExtent;    // extent of the object
    };

    unsigned long m_ssiLength;         // length (in bytes) of the SSI
    ShpSpatialIndexHeader *m_ssiHeader;// SSI header block
    unsigned long m_lruStamp;          // least-recently-used "time" stamp
    ShpSpatialIndexNode **m_nodeCache; // SSI node cache
    ShpSpatialIndexHandle *m_nodeStack;// SSI node stack
    int m_topOfStack;                  // index to node at top of the stack
    BOOL m_searchInitialized;          // has a search been initialized?
    BOOL m_traversalInitialized;       // has a traversal been initialized?
    BoundingBox m_searchArea;          // current search area
    int m_objClass;                    // class of objects to return
    BOOL m_sortOffsets;                // should the SHP offsets be sorted?
    unsigned m_nSHPObjects;            // number of objects in current block
    unsigned m_currSHPObject;          // index of next SHP object to return 
    SHPObjectInfo *m_shpObjects;       // array of unsorted SHP objects
    SHPObjectInfo **m_shpObjectCache;  // cache of pointers to SHP objects
    int m_nEliminatedNodes;            // number of nodes eliminated by a deletion
    ShpSpatialIndexNode **m_eliminatedNodes;     // list of nodes eliminated by a deletion
    bool mTempFile;                    // true if this is a temporary file
    FdoStringP mTempDir;               // temp folder (L"" if not specified)
    ShpSpatialIndex();

public:
    // constructor/destructor
//    ShpSpatialIndex (const wchar_t* idx, const wchar_t* tmp, eShapeTypes type = eNullShape, int dimensions = 0);
    ShpSpatialIndex (const wchar_t* idx, const wchar_t* tmp, eShapeTypes type, bool has_m);
    ~ShpSpatialIndex();

	// Reopen the file with the specified permissions
	void Reopen(OpenFlags openFlags);

    // methods to set/get SSI descriptors

    int GetVersionNumber() const;
    int GetFloatingPointPrec() const;
    LPCTSTR GetDescription() const;
    void GetEntriesPerNode(unsigned &maxEntries, unsigned &minEntries) const;
    unsigned long GetNObjects() const;
    FILETIME GetSHPWriteTime() const;
    unsigned long GetSHPFileSize() const;
    eShapeTypes GetFileShapeType () const;

    void SetFloatingPointPrec (int fpPrecision);
    void SetDescription (LPCTSTR description);
    void SetEntriesPerNode (unsigned maxEntries, unsigned minEntries);
    void SetSHPFileSize (unsigned long ulFileSize);
    void SetSHPWriteTime (FILETIME& ftWriteTime);

    // method to get the extent of all objects in the SSI

    void GetSSIExtent(BoundingBoxEx &ssiExtent);

    // methods to insert/delete objects from the SSI

    void InsertObject (eShapeTypes type, const BoundingBoxEx *objExtent,
        unsigned long shpOffset);
    void DeleteObject(const BoundingBoxEx *objExtent, unsigned long shpOffset);

    // methods to perform a search on the SSI

    void InitializeSearch(const BoundingBox *searchArea, int objClass, 
        BOOL sortSHPOffsets = FALSE);
    int GetNextObject(unsigned long &shpOffset, BoundingBoxEx &objExtent);

    // methods to perform a depth-first traversal of the SSI

    void InitializeTraversal();
    int GetNextNode(unsigned long shpOffset[], BoundingBoxEx objExtent[],
        unsigned &nEntries, BoundingBoxEx &nodeExtent);

    // method to test the integrity of the SSI

    int TestSSI(ShpSpatialIndexFileCallback *callbackObj, SSITestInfo *testInfo);

    // method to defragment the SSI

    int Defragment(ShpSpatialIndexFileCallback *callbackObj, wchar_t* tempFileName);

    void WriteSSIHeader();

    void FlushNodeCache (BOOL writeModified);

protected:

    // methods to create, read, and write the SSI header block

    void ReadSSIHeader();

    // methods required to insert objects into the R-Tree

    void Insert (unsigned level, const BoundingBoxEx *objExtent,
        unsigned long shpOffset);
    ShpSpatialIndexNode *Insert(ShpSpatialIndexNode *node, unsigned level,
        const BoundingBoxEx *objExtent, unsigned long shpOffset);
    void AddNodeEntry(ShpSpatialIndexNode *node, unsigned long fileOffset,
        const BoundingBoxEx *extent);
    void GetNodeExtent(const ShpSpatialIndexNode *node, BoundingBoxEx *nodeExtent) const;
    unsigned ChooseInsertionSubtree(const ShpSpatialIndexNode *node, 
        const BoundingBoxEx *objExtent) const;
    ShpSpatialIndexNode *SplitNode(ShpSpatialIndexNode *fullNode, const BoundingBoxEx *childExtent,
        unsigned long childFileOffset);
    void PickSeeds(const BoundingBoxEx childExt[], unsigned &group1Seed, 
        unsigned &group2Seed) const;
    void PickNext(const BoundingBoxEx childExt[], const BoundingBoxEx *group1Ext,
        unsigned nGroup1Entries, const BoundingBoxEx *group2Ext,
        unsigned nGroup2Entries, const BOOL childAssigned[], 
        unsigned &nextEntry, unsigned &group) const;

    // methods required to delete objects from the R-Tree

    int Delete(ShpSpatialIndexNode *node, const BoundingBoxEx *objExtent, unsigned long shpOffset);
    void ReinsertOrphanedEntries ();
    void AddEliminatedNode (const ShpSpatialIndexNode *node);
    unsigned GetNodeEntryIndex (const ShpSpatialIndexNode *node, unsigned long fileOffset) const;
    void RemoveNodeEntry (ShpSpatialIndexNode *node, unsigned entryIndex);

    // methods used to search the R-Tree

    void SearchRTree ();
    void SortSHPObjects (int left, int right);

    // recursive method used to traverse the leaf nodes in the R-Tree

    void TraverseRTree ();

    // methods used to test the integrity of the R-Tree

    int TestRTree(ShpSpatialIndexFileCallback *callbackObj, const BoundingBoxEx *nodeExt,
        unsigned gcdNodeSizes, SSITestInfo *testInfo); 
    BOOL ValidNodeOffset (unsigned long ssiOffset, unsigned gcdNodeSizes) const;
    unsigned long TraverseFreeList (unsigned long ssiOffset);

    // internal method used to defragment the SSI

    int Defragment(ShpSpatialIndex* defragFile, unsigned long &ssiOffset, 
        ShpSpatialIndexFileCallback *callbackObj);

    // various utility methods to manage the node cache, etc

    BOOL AtLeafLevel (unsigned nodeLevel) const;
    unsigned GetNodeLevel (unsigned nodeHeight) const;
    ShpSpatialIndexNode *GetNode (unsigned nodeLevel);
    ShpSpatialIndexNode *GetNode (unsigned long ssiOffset, unsigned nodeLevel);
    ShpSpatialIndexNode *GetCachedNode (unsigned long ssiOffset) const;
    ShpSpatialIndexNode *GetLRUNode ();
    void UpdateLRUStamp ();
    void AllocateNode (unsigned nodeLevel, unsigned long &ssiOffset);
    void AddNodeToFreeList (ShpSpatialIndexNode *node);
    void InitializeNodeStack ();
    void PushNode (unsigned long ssiOffset, unsigned nodeLevel, BOOL checkCache = TRUE);
    ShpSpatialIndexNode* TopNode () const;
    void PopNode ();
    void UnwindNodeStack ();
    void ReadNode (unsigned long ssiOffset, unsigned nodeLevel, ShpSpatialIndexNode *node);
    void WriteNode (const ShpSpatialIndexNode *node);

    static bool ShapeHasZ (eShapeTypes type);
};    

#endif // SHPSPATIALINDEX_H

