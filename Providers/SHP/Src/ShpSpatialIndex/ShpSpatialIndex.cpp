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
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "ShpSpatialIndex.h"
#include "ShpSpatialIndexHeader.h"
#include "ShpSpatialIndexNode.h"
#include "ShpSpatialIndexHandle.h"
#include <Constants.h>
#include <FdoCommonStringUtil.h>

#ifdef _WIN32
#include <io.h>
#include <share.h>
#else
#define MAX_PATH PATH_MAX
#include <wchar.h>
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#endif

// local constant definitions
const int NODE_CACHE_SIZE = 30;                     // size of node cache
const int NODE_STACK_SIZE = 30;                     // size of node stack
const int MAX_SHP_OBJECTS = 8192;                   // SHP object cache size

//-------------------------------------------------------------------------
//
// SHPObjectInfo - Used by the search methods to store the SHP file offset
//     and bounding box of an SHP object.
//
//-------------------------------------------------------------------------

ShpSpatialIndex::ShpSpatialIndex () :
    m_ssiLength (0),
    m_ssiHeader (NULL),
    m_lruStamp (0),
    m_nodeCache (NULL),
    m_nodeStack (NULL),
    m_topOfStack (-1),
    m_searchInitialized (FALSE),
    m_traversalInitialized (FALSE),
    m_objClass (0),
    m_sortOffsets (FALSE),
    m_nSHPObjects (0),
    m_currSHPObject (0),
    m_shpObjects (NULL),
    m_shpObjectCache (NULL),
    m_nEliminatedNodes (0),
    m_eliminatedNodes (NULL),
    mTempFile (false)
{
}

//-------------------------------------------------------------------------
//
// METHOD: Constructor.
//
// PURPOSE: Open (or create) the specified spatial index file, and
//          initialize the ShpSpatialIndex object.
//
// PARAMETERS:
//
//     Input:
//
//         ssiPath   - passes the path of the SSI.
//         openFlags - passes a bitwise combination of the file-open
//                     flags defined in FdoCommonFile.h.
//         
//-------------------------------------------------------------------------
ShpSpatialIndex::ShpSpatialIndex (const wchar_t* idx, const wchar_t* tmp, eShapeTypes type, bool has_m) :
    m_ssiLength (0),
    m_ssiHeader (NULL),
    m_lruStamp (0),
    m_nodeCache (NULL),
    m_nodeStack (NULL),
    m_topOfStack (-1),
    m_searchInitialized (FALSE),
    m_traversalInitialized (FALSE),
    m_objClass (0),
    m_sortOffsets (FALSE),
    m_nSHPObjects (0),
    m_currSHPObject (0),
    m_shpObjects (NULL),
    m_shpObjectCache (NULL),
    m_nEliminatedNodes (0),
    m_eliminatedNodes (NULL),
    mTempFile (false),
    mTempDir(tmp)
{
    FdoCommonFile::OpenFlags flags;
    ErrorCode status;
    wchar_t* temp;

    flags = (FdoCommonFile::OpenFlags)(FdoCommonFile::IDF_OPEN_UPDATE | FdoCommonFile::IDF_OPEN_ALWAYS);

    if (!OpenFile (idx, flags, status))
        // check the status for a priviledge (Read-Only filesystem) problem
        if ((ACCESS_DENIED == status) || (READ_ONLY == status))
        {
            // try a temporary
            if (FdoCommonFile::GetTempFile (&temp, tmp))
            {
                if (OpenFile (temp, flags, status))
                {
                    delete[] temp;
                    mTempFile = true;
                }
                else
                {
                    delete[] temp;
                    throw FdoCommonFile::ErrorCodeToException (status, temp, flags);
                }
            }
            else
                throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::ShpSpatialIndex(GetTempFile)");
        }
        else
            throw FdoException::Create (NlsMsgGet(SHP_SI_UNABLE_TO_OPEN, "The file '%1$ls' cannot be opened.", idx));

    m_ssiHeader = new ShpSpatialIndexHeader ();
    if (IsNew ())
    {
        m_ssiHeader->SetValid_Z (ShapeHasZ (type));
        m_ssiHeader->SetValid_M (has_m);
        WriteSSIHeader ();
    }
    else
        ReadSSIHeader ();

	FdoInt64 ssiLength;
    // get the length of the SSI
    if (GetFileSize64(ssiLength))
    {
		m_ssiLength = (ULONG)ssiLength;

        // create the SHPObjectInfo cache (the cache is implemented as an array
        // of pointers to speed up sorting on SHP offset)
        m_shpObjects = new SHPObjectInfo[MAX_SHP_OBJECTS];
        m_shpObjectCache = new SHPObjectInfo *[MAX_SHP_OBJECTS];

        // finally, create the node cache, stack, and eliminated node list
        m_nodeCache = new ShpSpatialIndexNode *[NODE_CACHE_SIZE];
        for (int i = 0; i < NODE_CACHE_SIZE; i++)
            m_nodeCache[i] = new ShpSpatialIndexNode;

        m_nodeStack = new ShpSpatialIndexHandle[NODE_STACK_SIZE];

        m_eliminatedNodes = new ShpSpatialIndexNode *[NODE_CACHE_SIZE];
        for (int i = 0; i < NODE_CACHE_SIZE; i++)
            m_eliminatedNodes[i] = NULL;
    }
    else
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::ShpSpatialIndex(GetFileSize)");
} // end: Constructor


//-------------------------------------------------------------------------
//
// METHOD: Destructor.
//
// PURPOSE: Flush any modified nodes to the SSI and release all dynamically
//          allocated resources.
//
//-------------------------------------------------------------------------

ShpSpatialIndex::~ShpSpatialIndex()
{
    // if file is not read-only, write out the header and flush any modified 
    // RTree nodes back to the SSI

    if (IsOpen () && !IsReadOnly () && m_nodeCache != NULL && !mTempFile)
    {
        WriteSSIHeader();
        FlushNodeCache(TRUE);
    }
    // delete temporary file
    if (mTempFile)
    {
        wchar_t* name = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (FileName ()) + 1));
        wcscpy (name, FileName ());
        CloseFile ();
        FdoCommonFile::Delete (name);
    }

    // free up dynamically allocated memory

    delete m_ssiHeader;
    delete [] m_shpObjects;
    delete [] m_shpObjectCache;
    delete [] m_nodeStack;

    if (m_nodeCache != NULL) {
        for (int i = 0; i < NODE_CACHE_SIZE; i++) {
            if (m_nodeCache[i] != NULL)
                delete m_nodeCache[i];
        }
        delete [] m_nodeCache;
    }

    if (m_eliminatedNodes != NULL) {
        for (int i = 0; i < NODE_CACHE_SIZE; i++) {
            if (m_eliminatedNodes[i] != NULL)
                delete m_eliminatedNodes[i];
        }
        delete [] m_eliminatedNodes;
    }

} // end: Destructor


void ShpSpatialIndex::Reopen(OpenFlags openFlags)
{
    ErrorCode status;

    ATLTRACE(L"Reopening a file without recreating the object\n");

	if (openFlags & IDF_OPEN_READ)
	{
		if ( !OpenFile(FileName(), IDF_OPEN_READ, status) )
			throw FdoException::Create (NlsMsgGet(SHP_SI_UNABLE_TO_OPEN, "The file '%1$ls' cannot be opened.", FileName()));
	}
	else if ( IsReadOnly() && (openFlags & IDF_OPEN_UPDATE) ) 
	{
		OpenFlags flags = (FdoCommonFile::OpenFlags)(FdoCommonFile::IDF_OPEN_UPDATE | FdoCommonFile::IDF_OPEN_ALWAYS);
		wchar_t*  temp;

		if (!OpenFile (FileName(), flags, status))
		{
			// check the status for a priviledge (Read-Only filesystem) problem
			if ((ACCESS_DENIED == status) || (READ_ONLY == status))
			{
				// try a temporary
				if (FdoCommonFile::GetTempFile (&temp, mTempDir))
				{
					if (OpenFile (temp, flags, status))
					{
						delete[] temp;
						mTempFile = true;
					}
					else
					{
						delete[] temp;
						throw FdoCommonFile::ErrorCodeToException (status, temp, flags);
					}
				}
				else
					throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::ShpSpatialIndex(GetTempFile)");
			}
		}
	}
}

//-------------------------------------------------------------------------
//
// METHOD: GetVersionNumber().
//
// PURPOSE: Get the version number of the SSI.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: Returns the version of the SSI.
//
//-------------------------------------------------------------------------

int ShpSpatialIndex::GetVersionNumber() const
{
    return m_ssiHeader->m_version;

} // end: GetVersionNumber().


//-------------------------------------------------------------------------
//
// METHOD: GetDescription().
//
// PURPOSE: Get the SSI description string.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: Returns a pointer to the NULL terminated description string.
//
//-------------------------------------------------------------------------

LPCTSTR ShpSpatialIndex::GetDescription() const
{
     return m_ssiHeader->m_description;

} // end: GetDescription()


//-------------------------------------------------------------------------
//
// METHOD: SetDescription().
//
// PURPOSE: Set the SSI description string, and write the header back to
//          the SSI.
//
// PARAMETERS:
//
//     Input:
//
//         description - passes a pointer to the null-terminated descr-
//                       iption string.
//
//     Output:
//
//         None.
//
//
// RETURNS: Returns the SSI nStatus (see ssi.doc).
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::SetDescription(LPCTSTR description)
{
    int nChars = sizeof(m_ssiHeader->m_description) - 1;
    Safe_tcsncpy(m_ssiHeader->m_description, description, nChars);
    m_ssiHeader->m_description[nChars] = L'\0';
    WriteSSIHeader();
} // end: SetDescription()


//-------------------------------------------------------------------------
//
// METHOD: GetFloatingPointPrec().
//
// PURPOSE: Get the floating point precision for the SSI.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: Returns the floating point precision used by the SSI (32 or 64).
//
//-------------------------------------------------------------------------

int ShpSpatialIndex::GetFloatingPointPrec() const
{
     return m_ssiHeader->m_floatPrecision;

} // end: GetFloatingPointPrec()


//-------------------------------------------------------------------------
//
// METHOD: SetFloatingPointPrec().
//
// PURPOSE: Set the floating point precision for the SSI, and write the
//          header back. NOTE - this function only works for newly created 
//          SSI's (before any insertions take place).
//
// PARAMETERS:
//
//     Input:
//
//         fpPrecision - passes the floating precision (number of bits).
//
//     Output:
//
//         None.
//
//
// RETURNS: Returns the SSI nStatus (see ssi.doc).
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::SetFloatingPointPrec(int fpPrecision)
{
    // check for invalid floating point precision

    if (fpPrecision != 32 && fpPrecision != 64)
        throw FdoException::Create (NlsMsgGet(SHP_SI_INVALID_PRECISION, "Invalid floating point precision (%2$d).", fpPrecision));

    // if this is a new, empty SSI, then update the SSI header, and 
    // write it out to disk
    
    if (IsNew () && m_ssiHeader->m_rootNodeOffset == 0) {
        m_ssiHeader->m_floatPrecision = fpPrecision;
        m_ssiHeader->ComputeNodeSizes();
        WriteSSIHeader();
    }
} // end: SetFloatingPointPrec()

void ShpSpatialIndex::SetSHPFileSize(unsigned long ulFileSize)
{
    // if this is a new, empty SSI, then update the SSI header, and 
    // write it out to disk
    
    if (IsNew () && m_ssiHeader->m_rootNodeOffset == 0) {
        m_ssiHeader->m_ulSHPSize = ulFileSize;
        WriteSSIHeader();
    }
}

void ShpSpatialIndex::SetSHPWriteTime(FILETIME& ftWriteTime)
{
    // if this is a new, empty SSI, then update the SSI header, and 
    // write it out to disk
    
    if (IsNew () && m_ssiHeader->m_rootNodeOffset == 0) {
        m_ssiHeader->m_ftSHPWriteTime = ftWriteTime;
        WriteSSIHeader();
    }
}

//-------------------------------------------------------------------------
//
// METHOD: GetEntriesPerNode().
//
// PURPOSE: Get the maximum/minimum number of entries per R-Tree node.
//
// PARAMETERS:
//
//     Input:
//
//         None.
//
//     Output:
//
//         maxEntries - passes a reference to an unsigned integer; the
//                      maximum entries per node is copied to the refer-
//                      enced integer.
//         minEntries - passes a reference to an unsigned integer; the
//                      minimum entries per node is copied to the refer-
//                      enced integer.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::GetEntriesPerNode(unsigned &maxEntries, 
    unsigned &minEntries) const
{
    maxEntries = m_ssiHeader->m_maxEntriesPerNode;
    minEntries = m_ssiHeader->m_minEntriesPerNode;

} // end: GetEntriesPerNode()


//-------------------------------------------------------------------------
//
// METHOD: SetEntriesPerNode().
//
// PURPOSE: Set the maximum/minimum number of entries per R-Tree node.
//          NOTE - this function only works for newly created SSI's (before
//          any insertions take place).
//
// PARAMETERS:
//
//     Input:
//
//         maxEntries - passes the maximum entries per node.
//         minEntries - passes the minimum entries per node.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::SetEntriesPerNode(unsigned maxEntries, 
    unsigned minEntries)
{
    // check for invalid number of entries

    if (maxEntries < minEntries)
        throw FdoException::Create (NlsMsgGet(SHP_SI_INVALID_MINIMUM_NODES, "Invalid minimum entries per node (%2$d).", minEntries));
    if (maxEntries > MAX_NODE_ENTRIES)
        throw FdoException::Create (NlsMsgGet(SHP_SI_INVALID_MAXIMUM_NODES, "Invalid maximum entries per node (%2$d).", maxEntries));

    // if this is a new SSI then update the appropriate entries of the SSI
    // header, and write it to disk

    if (IsNew () && m_ssiHeader->m_rootNodeOffset == 0) {
        m_ssiHeader->m_maxEntriesPerNode = maxEntries;
        m_ssiHeader->m_minEntriesPerNode = minEntries;
        m_ssiHeader->ComputeNodeSizes();
        WriteSSIHeader();
    }
} // end: SetEntriesPerNode()


//-------------------------------------------------------------------------
//
// METHOD: GetNObjects().
//
// PURPOSE: Get the number of objects contained in the SSI.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: Returns the number of entries in the SSI.
//
//-------------------------------------------------------------------------

unsigned long ShpSpatialIndex::GetNObjects() const
{
    return m_ssiHeader->m_totalObjects;

}

eShapeTypes ShpSpatialIndex::GetFileShapeType () const
{
    return ((eShapeTypes)m_ssiHeader->m_type);
}

FILETIME ShpSpatialIndex::GetSHPWriteTime() const
{
    return m_ssiHeader->m_ftSHPWriteTime;
}

unsigned long ShpSpatialIndex::GetSHPFileSize() const
{
    return m_ssiHeader->m_ulSHPSize;
}

//-------------------------------------------------------------------------
//
// METHOD: GetSSIExtent().
//
// PURPOSE: Get the extent of all objects in the SSI.
//
// PARAMETERS:
//
//     Input:
//
//         None.
//
//     Output:
//
//         ssiExtent - passes a reference to a BoundingBox. The extent
//                     of the SSI is copied to the referenced structure.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::GetSSIExtent (BoundingBoxEx &ssiExtent)
{
    // if SSI is not empty then get a pointer to the root node
    if (m_ssiHeader->m_rootNodeOffset != 0) {
        ShpSpatialIndexHandle rootNode;
        rootNode = GetNode(m_ssiHeader->m_rootNodeOffset, GetNodeLevel(1));
        GetNodeExtent(rootNode, &ssiExtent);
    }

    // else just set extent to all 0's
    else
        ssiExtent = BoundingBoxEx ();
}
       

//-------------------------------------------------------------------------
//
// METHOD: InsertObject().
//
// PURPOSE: Insert an object into the SSI; the object is specified by its
//          type, bounding box, and the offset of the object within the
//          associated SHP.
//
// PARAMETERS:
//
//     Input:
//
//         type      - passes the SHP object type.
//         objExtent - passes a pointer to a BoundingBox. The contents
//                     of the structure pointed to contain the coordinates
//                     of the object extent.
//         shpOffset - passes the SHP offset of the object.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::InsertObject (eShapeTypes type,
    const BoundingBoxEx *objExtent, unsigned long shpOffset)
{
    if (eNullShape != type)
    {
        if (IsNew () && (eNullShape == (eShapeTypes)m_ssiHeader->m_type))
        {
            m_ssiHeader->m_type = type;
            WriteSSIHeader ();
        }
        const BoundingBoxEx bb (*objExtent);
        // call internal insertion method instructing it to insert at the
        // leaf level
        Insert (0, &bb, shpOffset);
        m_ssiHeader->m_totalObjects++;
    }
}


//-------------------------------------------------------------------------
//
// METHOD: Insert().
//
// PURPOSE: Insert an entry into the SSI at the specified level.
//
// PARAMETERS:
//
//     Input:
//
//         level      - passes the level insertion is to take place at.
//         extent     - passes the extent of the entry.
//         fileOffset - passes the file offset (either and SSI or SHP
//                      file offset depending on the level).
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::Insert (unsigned level, const BoundingBoxEx *extent,
    unsigned long fileOffset)
{
    // check if the SSI was opened in read-only mode

    if (IsReadOnly ())
        throw FdoException::Create (NlsMsgGet(SHP_SI_READ_ONLY, "File '%1$ls' is read-only.", FileName ()));

    // get a pointer to the root node
    ShpSpatialIndexHandle rootNode;

    if (m_ssiHeader->m_rTreeHeight == 0) {
        m_ssiHeader->m_rTreeHeight = 1;
        rootNode = GetNode(GetNodeLevel(1));
        m_ssiHeader->m_rootNodeOffset = rootNode->m_ssiOffset;
    }
    else
        rootNode = GetNode(m_ssiHeader->m_rootNodeOffset, GetNodeLevel(1));

    // call recursive internal method to do the insertion

    ShpSpatialIndexNode *newNode = Insert(rootNode, level, extent, fileOffset);

    // if a split propagated back then need to grow the tree in height by 1
 
    if (newNode) {
        ShpSpatialIndexHandle oldRootNode = rootNode;

        // get a new root node

        m_ssiHeader->m_rTreeHeight++;
        rootNode = GetNode(GetNodeLevel(1));

        // assign the children of the new root node

        rootNode->m_fileOffset[0] = oldRootNode->m_ssiOffset;
        GetNodeExtent(oldRootNode, &rootNode->m_childExt[0]);
        rootNode->m_fileOffset[1] = newNode->m_ssiOffset;
        GetNodeExtent(newNode, &rootNode->m_childExt[1]);
        rootNode->m_nodeModified = TRUE;
        rootNode->m_nEntries = 2;

        // save the file offset of the new root node

        m_ssiHeader->m_rootNodeOffset = rootNode->m_ssiOffset;
    }
} // end: Insert()


//-------------------------------------------------------------------------
//
// METHOD: Insert().
//
// PURPOSE: Internal method to recursively descend into the R-Tree to 
//          locate the appropriate R-Tree node that the object will be
//          inserted into.
//
// PARAMETERS:
//
//     Input:
//
//         node       - passes a pointer to the current R-Tree node.
//         level      - passes the level insertion is to take place at.
//         extent     - passes the extent of the entry.
//         fileOffset - passes the file offset (either and SSI or SHP
//                      file offset depending on the level).
//         nStatus     - passes a reference to an integer. The SSI nStatus
//                      is copied to the referenced integer.
//
//     Output:
//
//         None.
//
// RETURNS: Returns a pointer to an ShpSpatialIndexNode if 'node' was split as a
//          result of the insertion; otherwise returns NULL.
//
//-------------------------------------------------------------------------

ShpSpatialIndexNode *ShpSpatialIndex::Insert(ShpSpatialIndexNode *node, unsigned level, 
    const BoundingBoxEx *extent, unsigned long fileOffset)
{
    // if at the level for insertion then insert the object into the node

    if (node->m_nodeLevel == level) {
        // if the node isn't full then just insert the object

        if (node->m_nEntries < m_ssiHeader->m_maxEntriesPerNode)
            AddNodeEntry(node, fileOffset, extent);
        else // need to split the node
            return SplitNode(node, extent, fileOffset);
    }

    // else recursively descend into the R-Tree ...

    else {
        // determine the best child node for insertion

        unsigned i = ChooseInsertionSubtree(node, extent);
        unsigned childLevel = node->m_nodeLevel - 1;
        ShpSpatialIndexHandle childNode = GetNode(node->m_fileOffset[i], childLevel);

        // recursively descend into the child node

        ShpSpatialIndexNode *newChildNode;
        newChildNode = Insert(childNode, level, extent, fileOffset);

        // if a split propagated back, then insert the new child node
        // into the current node

        if (newChildNode) {
            // update the extent of the insertion child since it will have
            // been altered by the node split

            GetNodeExtent(childNode, &node->m_childExt[i]);

            // determine the extent of the new child node

            BoundingBoxEx newChildExt;
            GetNodeExtent(newChildNode, &newChildExt);

            // if the node isn't full then just insert the new child

            if (node->m_nEntries < m_ssiHeader->m_maxEntriesPerNode)
                AddNodeEntry(node, newChildNode->m_ssiOffset, &newChildExt);

            // else need to split the internal node

            else {
                unsigned long ssiOffset = newChildNode->m_ssiOffset;
                return SplitNode(node, &newChildExt, ssiOffset);
            }
        }

        // else update the extent of the child node with the object extent

        else {
            node->m_childExt[i].UnionWith(extent);
            node->m_nodeModified = TRUE;
        }
    }            

    return NULL;
} // end: Insert()


//-------------------------------------------------------------------------
//
// METHOD: AddNodeEntry().
//
// PURPOSE: Add a new entry to the specified node.
//
// PARAMETERS:
//
//     Input:
//
//         node       - passes a pointer to the ShpSpatialIndexNode.
//         fileOffset - passes the file offset of the entry.
//         extent     - passes the extent of the entry.
//
//     Output:
//
//         None.
//         
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::AddNodeEntry (ShpSpatialIndexNode *node, unsigned long fileOffset,
    const BoundingBoxEx *extent)
{
    node->m_fileOffset[node->m_nEntries] = fileOffset;
    node->m_childExt[node->m_nEntries] = *extent;
    node->m_nodeModified = TRUE;
    node->m_nEntries++;

} // end: AddNodeEntry()


//-------------------------------------------------------------------------
//
// METHOD: GetNodeExtent().
//
// PURPOSE: Determine the extent (bounding box) of all children in the
//          specified node.
//
// PARAMETERS:
//
//     Input:
//
//         node   - passes a pointer to the ShpSpatialIndexNode.
//
//     Output:
//
//         extent - passes a pointer to a BoundingBox. The extent of the
//                  node is copied to the structure pointed to.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::GetNodeExtent(const ShpSpatialIndexNode *node, 
    BoundingBoxEx *nodeExtent) const
{
    *nodeExtent = node->m_childExt[0];
    for (unsigned i = 1; i < node->m_nEntries; i++) 
        nodeExtent->UnionWith(&node->m_childExt[i]);

} // end: GetNodeExtent()


//-------------------------------------------------------------------------
//
// METHOD: ChooseInsertionSubtree().
//
// PURPOSE: Choose the subtree of the specified node into which insertion
//          should take place; the subtree chosen is the one whose extent
//          will require the least amount of areal enlargement.
//
// PARAMETERS:
//
//     Input:
//
//         node      - passes a pointer to the ShpSpatialIndexNode.
//         objExtent - passes the object extent.
//
//     Output:
//
//         None.
//
// RETURNS: Returns the index of the subtree to insert into.
//
//-------------------------------------------------------------------------

unsigned ShpSpatialIndex::ChooseInsertionSubtree(const ShpSpatialIndexNode *node, 
    const BoundingBoxEx *objExtent) const
{
    unsigned subTreeIndex, i;
    double minAreaIncrease, minArea;

    // initialize ... 

    subTreeIndex = 0;     
    minAreaIncrease = minArea = HUGE_VAL;

    // check each child looking for the one whose extent will require the 
    // least amount of enlargement

    for (i = 0; i < node->m_nEntries; i++) {
        double oldArea = node->m_childExt[i].Area();
        double newArea = (max(node->m_childExt[i].xMax, objExtent->xMax) -
                          min(node->m_childExt[i].xMin, objExtent->xMin)) *
                         (max(node->m_childExt[i].yMax, objExtent->yMax) -
                          min(node->m_childExt[i].yMin, objExtent->yMin));
        double areaIncrease = newArea - oldArea;

        if (areaIncrease < minAreaIncrease || 
            (areaIncrease == minAreaIncrease && newArea < minArea)) {
            minAreaIncrease = areaIncrease;
            minArea = newArea;
            subTreeIndex = i;
        }
    }

    return subTreeIndex;

} // end: ChooseInsertionSubtree()


//-------------------------------------------------------------------------
//
// METHOD: SplitNode().
//
// PURPOSE: Split an overflowing node into two. The quadratic algorithm 
//          outlined by Guttman is used.
//
// PARAMETERS:
//
//     Input:
//
//         fullNode        - passes a pointer to the overflowing node.
//         childExtent     - passes the extent of the child object.
//         childFileOffset - passes the file offset of the child object.
//
//     Output:
//
//         nStatus          - passes a reference to an integer. The SSI
//                           nStatus is copied to the referenced integer.
//
// RETURNS: Returns a pointer to the newly allocated node.
//
//-------------------------------------------------------------------------

ShpSpatialIndexNode *ShpSpatialIndex::SplitNode(ShpSpatialIndexNode *fullNode, 
    const BoundingBoxEx *childExtent, unsigned long childFileOffset)
{
    // get a pointer to a new node

    ShpSpatialIndexNode *newNode = GetNode(fullNode->m_nodeLevel);

    // make temporary copies of the entries

    BoundingBoxEx childExt[MAX_NODE_ENTRIES + 1];
    unsigned long fileOffset[MAX_NODE_ENTRIES + 1];
    BOOL childAssigned[MAX_NODE_ENTRIES + 1];

    for (unsigned i = 0; i < m_ssiHeader->m_maxEntriesPerNode; i++) {
        childExt[i] = fullNode->m_childExt[i];
        fileOffset[i] = fullNode->m_fileOffset[i];
        childAssigned[i] = FALSE;
    }

    childExt[m_ssiHeader->m_maxEntriesPerNode] = *childExtent;
    fileOffset[m_ssiHeader->m_maxEntriesPerNode] = childFileOffset;
    childAssigned[m_ssiHeader->m_maxEntriesPerNode] = FALSE;

    // determine the maximum number of entries a group can contain

    unsigned maxPerGroup;
    maxPerGroup = m_ssiHeader->m_maxEntriesPerNode - m_ssiHeader->m_minEntriesPerNode;

    // determine the first entry in each group

    unsigned group1[MAX_NODE_ENTRIES];
    unsigned group2[MAX_NODE_ENTRIES];

    PickSeeds(childExt, group1[0], group2[0]);

    unsigned nGroup1Entries = 1;
    unsigned nGroup2Entries = 1;

    childAssigned[group1[0]] = TRUE;
    childAssigned[group2[0]] = TRUE;

    BoundingBoxEx group1Ext = childExt[group1[0]];
    BoundingBoxEx group2Ext = childExt[group2[0]];

    // assign remaining entries

    for (unsigned i = 2; i <= m_ssiHeader->m_maxEntriesPerNode; i++) {
        // assign the next entry

        unsigned nextEntry, group;

        PickNext(childExt, &group1Ext, nGroup1Entries, &group2Ext,
            nGroup2Entries, childAssigned, nextEntry, group);

        if (group == 1) {
            group1[nGroup1Entries++] = nextEntry;
            group1Ext.UnionWith(&childExt[nextEntry]);
        }
        else {
            group2[nGroup2Entries++] = nextEntry;
            group2Ext.UnionWith(&childExt[nextEntry]);
        }

        childAssigned[nextEntry] = TRUE;

        // check if all remaining entries must be assigned to one of the groups

        if (nGroup1Entries >= maxPerGroup || nGroup2Entries >= maxPerGroup)
            break;
    }

    // make sure all entries have been assigned

    if (nGroup1Entries < m_ssiHeader->m_minEntriesPerNode) {
        for (unsigned i = 0; i <= m_ssiHeader->m_maxEntriesPerNode; i++) {
            if (!childAssigned[i])
                group1[nGroup1Entries++] = i;
        }
    }
    else if (nGroup2Entries < m_ssiHeader->m_minEntriesPerNode) {
        for (unsigned i = 0; i <= m_ssiHeader->m_maxEntriesPerNode; i++) {
            if (!childAssigned[i])
                group2[nGroup2Entries++] = i;
        }
    }

    // Now assign the entries from each group back to the R-Tree nodes

    for (unsigned i = 0; i < nGroup1Entries; i++)
        AddNodeEntry(newNode, fileOffset[group1[i]], &childExt[group1[i]]);

    ::memset(fullNode->m_fileOffset, 0xff, sizeof(fullNode->m_fileOffset));
    ::memset(fullNode->m_childExt, 0, sizeof(fullNode->m_childExt));
    fullNode->m_nEntries = 0;

    for (unsigned i = 0; i < nGroup2Entries; i++)
        AddNodeEntry(fullNode, fileOffset[group2[i]], &childExt[group2[i]]);

    // All done ...

    return newNode;

} // end: SplitNode()


//-------------------------------------------------------------------------
//
// METHOD: PickSeeds().
//
// PURPOSE: Select the two entries that will be the first elements of the
//          two groups into which a full node (plus the overflowing entry)
//          will be split. The quadratic algorithm suggested by Guttman
//          is used.
//
// PARAMETERS:
//
//     Input:
//
//         childExt   - passes an array of BoundingBoxs. The elements
//                      of the array contain the child extents.
//
//     Output:
//
//         group1Seed - passes a reference to an unsigned integer. The
//                      seed for group 1 is copied to the referenced 
//                      integer.
//         group2Seed - passes a reference to an unsigned integer. The
//                      seed for group 2 is copied to the referenced 
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::PickSeeds(const BoundingBoxEx childExt[], 
    unsigned &group1Seed, unsigned &group2Seed) const
{
    double maxWastedArea = -HUGE_VAL;

    for (unsigned i = 0; i < m_ssiHeader->m_maxEntriesPerNode; i++) {
        for (unsigned j = i + 1; j <= m_ssiHeader->m_maxEntriesPerNode; j++) {
            // compute the area of the bounding box enclosing the two 
            // children less the area of each child

            double wastedArea;
            BoundingBoxEx extent (childExt[i]);
            extent.UnionWith(&childExt[j]);
            wastedArea = extent.Area()-childExt[i].Area()-childExt[j].Area();

            // update seeds if the wasted area is greater than any previously
            // computed wasted area

            if (wastedArea > maxWastedArea) {
                maxWastedArea = wastedArea;
                group1Seed = i;
                group2Seed = j;
            }
        }
    }

} // end: PickSeeds()


//-------------------------------------------------------------------------
//
// METHOD: PickNext().
//
// PURPOSE: Select one of the remaining entries for assignment to a split
//          group. The quadratic algorithm suggested by Guttman is used.
//
// PARAMETERS:
//
//     Input:
//
//         childExt       - passes an array of BoundingBoxs. The elements
//                          of the array contain the child extents.
//         group1Ext      - passes the current extent of the entries in
//                          group 1.
//         nGroup1Entries - passes the number of entries currently in 
//                          group 1.
//         group2Ext      - passes the current extent of the entries in
//                          group 2.
//         nGroup2Entries - passes the number of entries currently in 
//                          group 2.
//         childAssigned  - passes an array of boolean values indicating
//                          which children have already been assigned to
//                          a group.
//
//     Output:
//
//         nextEntry      - passes a reference to an unsigned integer. The
//                          index next entry to assign is copied to the 
//                          referenced integer.
//         group          - passes a reference to an unsigned integer. The
//                          number of the group the entry should be placed
//                          into is copied to the referenced integer.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::PickNext(const BoundingBoxEx childExt[], 
    const BoundingBoxEx *group1Ext, unsigned nGroup1Entries,
    const BoundingBoxEx *group2Ext, unsigned nGroup2Entries,
    const BOOL childAssigned[], unsigned &nextEntry, unsigned &group) const
{
    // initialize ...

    double maxAreaDiff = -HUGE_VAL;
    double areaGroup1 = group1Ext->Area();
    double areaGroup2 = group2Ext->Area();

    // loop through the remaining unassigned entries looking for the one
    // with the greatest preference for one of the groups

    for (unsigned i = 0; i <= m_ssiHeader->m_maxEntriesPerNode; i++) {
        if (childAssigned[i])
            continue;

        BoundingBoxEx extent1 (*group1Ext);
        extent1.UnionWith(&childExt[i]);
        double areaDiff1 = extent1.Area() - areaGroup1;

        BoundingBoxEx extent2 (*group2Ext);
        extent2.UnionWith(&childExt[i]);
        double areaDiff2 = extent2.Area() - areaGroup2;

        double areaDiff = fabs(areaDiff1 - areaDiff2);

        if (areaDiff > maxAreaDiff) {
            maxAreaDiff = areaDiff;
            nextEntry = i;
            if (areaDiff1 < areaDiff2)
                group = 1;
            else if (areaDiff2 < areaDiff1)
                group = 2;
            else if (areaGroup1 < areaGroup2)
                group = 1;
            else if (areaGroup2 < areaGroup1)
                group = 2;
            else if (nGroup1Entries < nGroup2Entries)
                group = 1;
            else 
                group = 2;
        }
    }

} // end: PickNext()


//-------------------------------------------------------------------------
//
// METHOD: DeleteObject().
//
// PURPOSE: Delete the specified SHP object from the SSI.
//
// PARAMETERS:
//
//     Input:
//
//         objExtent - passes a pointer to a BoundingBox. The contents
//                     of the structure pointed to contain the coordinates
//                     of the object extent.
//         shpOffset - passes the SHP file offset of the object.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::DeleteObject(const BoundingBoxEx *objExtent,
    unsigned long shpOffset)
{
    // check if the SSI was opened in read-only mode

    if (IsReadOnly ()) 
        throw FdoException::Create (NlsMsgGet(SHP_SI_READ_ONLY, "File '%1$ls' is read-only.", FileName ()));

    // check if the SSI is emtpy

    if (m_ssiHeader->m_rTreeHeight == 0)
        throw FdoException::Create (NlsMsgGet(SHP_SI_OBJECT_NOT_FOUND, "Specified object was not found in Spatial Index."));

    // initialize for the deletion

    m_nEliminatedNodes = 0;

    // get a pointer to the root node
   
    ShpSpatialIndexHandle rootNode;
    rootNode = GetNode(m_ssiHeader->m_rootNodeOffset, GetNodeLevel(1));

    // call internal recursive method to perform the deletion
    BoundingBoxEx bb (*objExtent);
    Delete(rootNode, &bb, shpOffset);

    // reinsert any orphaned entries from underfilled nodes

    if (m_nEliminatedNodes > 0)
        ReinsertOrphanedEntries();

    // if the root node now has only one child, make the child the new
    // root node, and decrease the height of the R-Tree by 1

    if (rootNode->m_nEntries == 1 && !AtLeafLevel(rootNode->m_nodeLevel)) {
        m_ssiHeader->m_rootNodeOffset = rootNode->m_fileOffset[0];
        m_ssiHeader->m_rTreeHeight--;
        AddNodeToFreeList(rootNode);
    }

    // else if the R-Tree is now empty, then reinitialize and truncate the
    // SSI to the size of the header

    else if (rootNode->m_nEntries == 0) {
        m_ssiHeader->m_rTreeHeight = 0;
        m_ssiHeader->m_rootNodeOffset = 0;
        m_ssiHeader->m_totalObjects = 0;
        m_ssiHeader->m_type = 0;
        m_ssiHeader->m_freeLeafNode = 0;
        m_ssiHeader->m_freeInternalNode = 0;
        FlushNodeCache(FALSE);
        m_ssiLength = sizeof(ShpSpatialIndexHeader);
        if (!SetFileSize (sizeof(ShpSpatialIndexHeader)))
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::DeleteObject(SetFileSize)");
    }
} // end: DeleteObject()


//-------------------------------------------------------------------------
//
// METHOD: Delete().
//
// PURPOSE: Delete the specified SHP object from the SSI.
//
// PARAMETERS:
//
//     Input:
//
//         node      - passes a pointer to the node at the current
//                     level of recursion.
//         objExtent - passes the object extent.
//         shpOffset - passes the SHP file offset of the object.
//
//     Output:
//
//         None.
//
// RETURNS: Returns the SHP_OK if the node was deleted, SHP_SI_SEARCH_OBJECT_NOT_FOUND otherwise.
//
//-------------------------------------------------------------------------

int ShpSpatialIndex::Delete(ShpSpatialIndexNode *node, const BoundingBoxEx *objExtent,
    unsigned long shpOffset)
{
    int ret = SHP_SI_SEARCH_OBJECT_NOT_FOUND;

    // if the leaf node contains the object then remove it

    if (AtLeafLevel(node->m_nodeLevel)) {
        unsigned entryIndex = GetNodeEntryIndex(node, shpOffset);
        if (entryIndex < m_ssiHeader->m_maxEntriesPerNode) {
            RemoveNodeEntry(node, entryIndex);
            m_ssiHeader->m_totalObjects--;
            ret = SHP_OK;
        }
        else
            ret = SHP_SI_SEARCH_OBJECT_NOT_FOUND;
    }

    // else the node is not a leaf node, then descend into all children whose
    // extents contain the object extent until the object is successfully
    // found and deleted, or all possibilities are exhausted

    else {
        unsigned i = 0;
        unsigned childLevel = node->m_nodeLevel - 1;

        while (i < node->m_nEntries) {
            if (node->m_childExt[i].Contains(objExtent)) {
                // get a pointer to the current child node
     
                ShpSpatialIndexHandle childNode = GetNode(node->m_fileOffset[i], childLevel);

                // make recursive call to delete the object

                ret = Delete(childNode, objExtent, shpOffset);

                // if nStatus is SHP_OK, then the object was successfully found
                // and deleted

                if (SHP_OK == ret)
                {
                    // if the child node has fewer than the minimum number of
                    // entries, then add a copy of the child node to the
                    // eliminated node list, and add the child node to the
                    // (appropriate) free node list

                    if (childNode->m_nEntries < m_ssiHeader->m_minEntriesPerNode) {
                        RemoveNodeEntry(node, i);
                        AddEliminatedNode(childNode);
                        AddNodeToFreeList(childNode);
                    }
 
                    // else make sure that the child extent is updated to 
                    // tightly contain all entries in the child node

                    else {
                        GetNodeExtent(childNode, &node->m_childExt[i]);
                        node->m_nodeModified = TRUE;
                    }
                   
                    break; 
                }
            }

            i++;
        }
    }        

    return (ret);

} // end: Delete()


//-------------------------------------------------------------------------
//
// METHOD: ReinsertOrphanedEntries().
//
// PURPOSE: Reinsert all orphaned entries from nodes eliminated during the
//          deletion of an SHP object from the SSI.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::ReinsertOrphanedEntries()
{
    // reinsert all entries from the copies of nodes in the eliminated node
    // list - the insertions are done based on the levels of the eliminated
    // nodes to ensure the integrity of the R-Tree is maintained

    for (int i = 0; i < m_nEliminatedNodes; i++) {
        ShpSpatialIndexNode *node = m_eliminatedNodes[i];

        for (unsigned j = 0; j < node->m_nEntries; j++) {
            Insert (node->m_nodeLevel, &node->m_childExt[j],
                node->m_fileOffset[j]);
        }
    }

} // end: ReinsertOrphanedEntries()


//-------------------------------------------------------------------------
//
// METHOD: AddEliminatedNode().
//
// PURPOSE: Add a copy of an eliminated node to the eliminated node list.
//
// PARAMETERS:
//
//     Input:
//
//         node - passes a pointer to the eliminated node.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::AddEliminatedNode(const ShpSpatialIndexNode *node)
{
    // allocate a new node in the eliminated node list if necessary

    if (m_eliminatedNodes[m_nEliminatedNodes] == NULL) 
        m_eliminatedNodes[m_nEliminatedNodes] = new ShpSpatialIndexNode;

    // copy the node

    ::memcpy(m_eliminatedNodes[m_nEliminatedNodes], node, sizeof(ShpSpatialIndexNode));

    m_nEliminatedNodes++;

} // end: AddEliminatedNode()


//-------------------------------------------------------------------------
//
// METHOD: GetNodeEntryIndex()
//
// PURPOSE: Find the index of the specified entry in the given R-Tree node.
//
// PARAMETERS:
//
//     Input:
//
//         node       - passes a pointer to the R-Tree node.
//         fileOffset - passes the file offset of the entry.
//
//     Output:
//
//         None.
//
// RETURNS: Returns the index of the entry if it was found, else returns 
//          the maximum number of entries allowed per node.
// 
//-------------------------------------------------------------------------

unsigned ShpSpatialIndex::GetNodeEntryIndex(const ShpSpatialIndexNode *node,
    unsigned long fileOffset) const
{
    // search the node looking for a match on fileOffset

    for (unsigned i = 0; i < node->m_nEntries; i++) {
        if (node->m_fileOffset[i] == fileOffset)
            return i;
    }

    // if we got here, then the node must not have contained the fileOffset

    return m_ssiHeader->m_maxEntriesPerNode;

} // end: GetNodeEntryIndex()


//-------------------------------------------------------------------------
//
// METHOD: RemoveNodeEntry().
//
// PURPOSE: Remove the specified entry from the given R-Tree Node, com-
//          pacting the remaining entries.
//
// PARAMETERS:
//
//     Input:
//
//         node       - passes a pointer to the R-Tree node the entry is
//                      to be removed from.
//         entryIndex - passes the index of the entry to remove.
//
//     Output:
//
//         None.
//
// RETURNS: None.
// 
//-------------------------------------------------------------------------

void ShpSpatialIndex::RemoveNodeEntry(ShpSpatialIndexNode *node, unsigned entryIndex)
{
    // compact the remaining entries in the node

    unsigned i;
    unsigned lastEntry = m_ssiHeader->m_maxEntriesPerNode - 1;

    for (i = entryIndex; i < lastEntry; i++)
    {
        node->m_fileOffset[i] = node->m_fileOffset[i+1];
        node->m_childExt[i] = node->m_childExt[i+1];
    }

    // make sure last entry is reinitialized

    node->m_fileOffset[lastEntry] = (unsigned long)-1L;
    ::memset(&node->m_childExt[lastEntry], 0, sizeof(node->m_childExt[lastEntry]));

    // decrement the number of entries and mark the node as modified

    node->m_nEntries--;
    node->m_nodeModified = TRUE;

} // end: RemoveNodeEntry()


//-------------------------------------------------------------------------
//
// METHOD: InitializeSearch().
//
// PURPOSE: Initialize a search of the SSI. 
//
// PARAMETERS:
//
//     Input:
//
//         searchArea    - passes a pointer to a BoundingBox. The contents
//                         of the structure pointed to contain the coordinates
//                         of the search area; only objects whose extents
//                         intersect the search area are returned.
//         objClass      - passes the class of objects to return (for e.g.,
//                         SHP_POLYLINE_OBJECTS).
//         sortSHPOffset - passes a boolean which is FALSE (the default) 
//                         if the SHP offsets should be sorted before they
//                         are returned, and TRUE if they should.
//
//     Output:
//
//         None.
//      
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::InitializeSearch(const BoundingBox *searchArea, 
    int objClass, BOOL sortSHPOffsets)
{
    // prepare the node stack

    InitializeNodeStack();

    // push the root node onto the stack

    if (m_ssiHeader->m_rootNodeOffset != 0)
        PushNode(m_ssiHeader->m_rootNodeOffset, GetNodeLevel(1));

    // initialize remaining member variables

    m_searchInitialized = TRUE;
    m_sortOffsets = sortSHPOffsets;
    m_nSHPObjects = 0;
    m_currSHPObject = 0;
    m_searchArea = *searchArea;
    m_objClass = objClass;
} // end: InitializeSearch()
       

//-------------------------------------------------------------------------
//
// METHOD: GetNextObject().
//
// PURPOSE: Return the next SHP object that meets the search criteria set
//          by the most recent call to InitializeSearch().
//
// PARAMETERS:
//
//     Input:
//
//         None.
//
//     Output:
//
//         shpOffset - passes a reference to an unsigned long. The SHP
//                     offset of the next object is copied to the refer-
//                     enced integer.
//         objExtent - passes a reference to a BoundingBox. The extent of
//                     the object is copied to the referenced structure.
//
// RETURNS: Returns SHP_OK if the output parameters have the next shape matching
// the criteria, or SHP_SI_SEARCH_COMPLETE if no more objects meet the criteria.
//
//-------------------------------------------------------------------------

int ShpSpatialIndex::GetNextObject(unsigned long &shpOffset,
    BoundingBoxEx &objExtent)
{
    int ret;

    // initialize return parameters

    shpOffset = 0;

    // make sure a search has been properly initialized

    if (!m_searchInitialized) 
        throw FdoException::Create (NlsMsgGet(SHP_SI_SEARCH_NOT_INITIALIZED, "Spatial Index search has not been initialized."));
    else
    {
        // if there are no objects in the SHPObjectInfo cache then reload the cache

        if (m_currSHPObject >= m_nSHPObjects) {
            m_currSHPObject = m_nSHPObjects = 0;

            // if R-Tree is not empty then resume the search

            if (NULL != TopNode ())
                try
                {
                    SearchRTree ();
                }
                catch (...)
                {
                    m_currSHPObject = m_nSHPObjects = 0;
                    m_searchInitialized = FALSE;
                    throw;
                }
            
            // sort SHP objects based on the SHP file offset if necessary

            if (m_nSHPObjects > 0 && m_sortOffsets)
                SortSHPObjects(0, m_nSHPObjects -1);
        }

        if (m_currSHPObject < m_nSHPObjects)
        {   // there are more SHP objects so return the next one
            shpOffset = m_shpObjectCache[m_currSHPObject]->m_shpOffset;
            objExtent = m_shpObjectCache[m_currSHPObject]->m_objExtent;
            m_currSHPObject++;
            ret = SHP_OK;
        }
        else
        {   // search is complete
            m_searchInitialized = FALSE;
            ret = SHP_SI_SEARCH_COMPLETE;
        }
    }

    return (ret);

} // end: GetNextObject()
          

//-------------------------------------------------------------------------
//
// METHOD: SearchRTree().
//
// PURPOSE: Perform a search of the R-Tree structure for objects which meet
//          the conditions established in the most recent call to Initial-
//          izeSearch().
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::SearchRTree()
{
    // while there are more nodes to search, and the SHP object cache is
    // not full ...

    ShpSpatialIndexNode *node;

    while ((node = TopNode()) && m_nSHPObjects < MAX_SHP_OBJECTS) {

        // if the current search node is a leaf, then add all entries 
        // that meet the search condition to the SHPObjectInfo cache

        if (AtLeafLevel(node->m_nodeLevel)) {
            unsigned i = node->m_currentSubtree;

            // add each SHP object that intersects the search area to the
            // SHP object cache

            while (i < node->m_nEntries)
            {
                if (node->m_childExt[i].Intersects(&m_searchArea))
                {
                    m_shpObjects[m_nSHPObjects].m_shpOffset = node->m_fileOffset[i];
                    m_shpObjects[m_nSHPObjects].m_objExtent = node->m_childExt[i];
                    m_shpObjectCache[m_nSHPObjects] = &m_shpObjects[m_nSHPObjects];
                    m_nSHPObjects++;

                    // if SHP object cache is full then break out of the loop

                    if (m_nSHPObjects >= MAX_SHP_OBJECTS) {
                        i++;
                        break;
                    }
                }

                i++;
            }

            // save the current search state

            if (i < node->m_nEntries) 
                node->m_currentSubtree = i;
            else
                PopNode();
        }

        // else descend into all child nodes whose extents intersect
        // the search area

        else {
            unsigned i = node->m_currentSubtree;
            BOOL descending = FALSE;

            while (i < node->m_nEntries && !descending) {
                if (node->m_childExt[i].Intersects(&m_searchArea)) {
                    unsigned childLevel = node->m_nodeLevel - 1;
                    try
                    {
                        PushNode(node->m_fileOffset[i], childLevel);
                    }
                    catch (...)
                    {
                        UnwindNodeStack();
                        throw;
                    }
                    descending = TRUE;
                }
          
                i++;
            }

            if (descending)
                node->m_currentSubtree = i;
            else
                PopNode();
        }
    }
} // end: SearchRTree()


//-------------------------------------------------------------------------
//
// METHOD: SortSHPobjects().
//
// PURPOSE: Sort the cache of pointers to SHP objects based on the SHP 
//          file offset; this should help speed retrieval of the objects 
//          from the SHP. The QuickSort algorithm is used and is based on
//          an implementation given in "Algorithms and Data Structures" by 
//          Niklaus Wirth (Prentice Hall 1986).
//
// PARAMETERS:
//
//     Input:
//
//         left  - passes the index of the leftmost element in the sub-
//                 array to be sorted.
//         right - passes the index of the rightmost element in the sub-
//                 array to be sorted.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::SortSHPObjects(int left, int right)
{
    int i = left;
    int j = right;

    SHPObjectInfo *x = m_shpObjectCache[(left + right) / 2];

    do {
        while (m_shpObjectCache[i]->m_shpOffset < x->m_shpOffset)
            i++;

        while (m_shpObjectCache[j]->m_shpOffset > x->m_shpOffset)
            j--;

        if (i <= j) {
            SHPObjectInfo *w = m_shpObjectCache[i];
            m_shpObjectCache[i++] = m_shpObjectCache[j];
            m_shpObjectCache[j--] = w;
        }
    } while (i <= j);

    if (left < j)
        SortSHPObjects(left, j);

    if (i < right)
        SortSHPObjects(i, right);

} // end: SortSHPObjects()


//-------------------------------------------------------------------------
//
// METHOD: InitializeTraversal().
//
// PURPOSE: Initialize a traversal of the SSI; the method GetNextNode() 
//          can then be called to access the entries in each node of the
//          R-Tree.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::InitializeTraversal()
{
    // prepare the node stack

    InitializeNodeStack();

    // push the root node onto the stack

    if (m_ssiHeader->m_rootNodeOffset != 0)
        PushNode(m_ssiHeader->m_rootNodeOffset, GetNodeLevel(1));

    // initialize remaining member variables

    m_traversalInitialized = TRUE;
    m_nSHPObjects = 0;
} // end: InitializeTraversal()
       

//-------------------------------------------------------------------------
//
// METHOD: GetNextNode().
//
// PURPOSE: Return the entries of the next node in the R-Tree.
//
// PARAMETERS:
//
//     Input:
//
//         None.
//
//     Output:
//
//         shpOffset  - passes a pointer to an array of unsigned long
//                      integers. The SHP offsets the the entries in the
//                      next node are copied to the array. 
//         objExtent  - passes a pointer to an array of BoundingBox
//                      structures. The extents of the objects are 
//                      copied to the array.
//         nEntries   - passes a reference to an unsigned integer. The
//                      number of entries in the node is copied to the
//                      reference integer.
//         nodeExtent - passes a pointer to a bounding box. The extent of
//                      all objects in the node is copied to the structure
//                      pointed to.
//
// RETURNS: Returns SHP_OK if the output parameters have the next shape
// or SHP_SI_TRAVERSAL_COMPLETE if no more objects meet the criteria.
//
//-------------------------------------------------------------------------

int ShpSpatialIndex::GetNextNode (unsigned long shpOffset[], 
    BoundingBoxEx objExtent[], unsigned &nEntries, BoundingBoxEx &nodeExtent)
{
    int ret;

    // initialize 

    nEntries = 0;

    // make sure a traversal has been properly initialized

    if (!m_traversalInitialized) 
        throw FdoException::Create (NlsMsgGet(SHP_SI_TRAVERSAL_NOT_INITIALIZED, "Spatial Index traversal has not been initialized."));

    // if there are no objects in the SHPObjectInfo cache then reload the cache
    // with the objects in the next node

    if (m_nSHPObjects == 0) {
        // if R-Tree is not empty then resume the traversal

        if (TopNode() != NULL)
        {
            try
            {
                TraverseRTree ();
            }
            catch (...)
            {
                m_nSHPObjects = 0;
                m_traversalInitialized = FALSE;
                throw;
            }
        }
    }

    if (m_nSHPObjects > 0)
    {   // there is another node, so copy the entries to the return parameters
        for (unsigned i = 0; i < m_nSHPObjects; i++) {
            shpOffset[i] = m_shpObjects[i].m_shpOffset;
            objExtent[i] = m_shpObjects[i].m_objExtent;
        }
        nodeExtent = m_shpObjects[0].m_objExtent;
        for (unsigned i = 1; i < m_nSHPObjects; i++) 
            nodeExtent.UnionWith(&m_shpObjects[i].m_objExtent);
        nEntries = m_nSHPObjects;
        m_nSHPObjects = 0;
        ret = SHP_OK;
    }
    else
    {   // traversal is complete
        m_traversalInitialized = FALSE;
        ret = SHP_SI_TRAVERSAL_COMPLETE;
    }

    return (ret);

} // end: GetNextNode()
          

//-------------------------------------------------------------------------
//
// METHOD: TraverseRTree().
//
// PURPOSE: Perform a depth-first traversal of the R-Tree structure on a
//          node by node basis. At each leaf node, the traversal is temp-
//          orarily halted and the entries in the node are copied to the
//          cache normally used by the search mechanism.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::TraverseRTree()
{
    // while there are more nodes to traverse ...

    ShpSpatialIndexNode *node;

    while (node = TopNode()) {
        // if the current search node is a leaf, then add all entries 
        // to the SHPObjectInfo cache and suspsend the traversal

        if (AtLeafLevel(node->m_nodeLevel)) {
            unsigned i;

            // add each SHP object to the SHP object cache

            for (i = 0; i < node->m_nEntries; i++) {
                m_shpObjects[i].m_shpOffset = node->m_fileOffset[i];
                m_shpObjects[i].m_objExtent = node->m_childExt[i];
            }

            m_nSHPObjects = i;

            // move up one level in the R-Tree - this is where the traversal
            // will resume

            PopNode();
            break;
        }

        // else descend into the next child of the current node

        else {
            unsigned i = node->m_currentSubtree;

            // if there is a next child, then read it in and descend

            if (i < node->m_nEntries) {
                try
                {
                    PushNode(node->m_fileOffset[i], node->m_nodeLevel-1);
                }
                catch (...)
                {
                    UnwindNodeStack();
                    throw;
                }
                node->m_currentSubtree = i + 1;
            }
            else
                PopNode();
        }
    }
} // end: TraverseRTree()


//-------------------------------------------------------------------------
//
// METHOD: TestSSI().
//
// PURPOSE:  Test the integrity of an SSI, and compute the space efficiency
//           The following is tested for:
//
//           - The file offset of each node is "reasonable"
//           - The bounding box of each node is correct
//           - Each node has at least the minimum number of entries
//           - The total number of objects in the SSI matches the header
//             entry
//
// PARAMETERS:
//
//     Input:
//
//         callbackObj - passes a pointer to an ShpSpatialIndexFileCallback object.
//                       This object (if not NULL) is used to report the
//                       current level of progress, and to determine if 
//                       the operation should be cancelled.
//
//     Output:
//
//         testInfo    - passes a pointer to an SSITestInfo structure. The
//                       test information is copied to the structure pointed
//                       to.
//
// RETURNS:
//  SHP_OK if the test is successful
//  SHP_SI_TRAVERSAL_CANCELLED if the callback has cancelled the test
//  SHP_ERROR_SI_NODE_UNDERFLOW if there are fewer nodes than specified by m_minEntriesPerNode
//  SHP_ERROR_SI_BAD_NODE_EXTENT if the node extent exceeds that of the parent
//  SHP_ERROR_SI_BAD_NODE_OFFSET if there is a child node offset outside the file extents
//  SHP_ERROR_SI_BAD_OBJECT_COUNT if the number of objects does not agree with the header
//-------------------------------------------------------------------------

int ShpSpatialIndex::TestSSI(ShpSpatialIndexFileCallback *callbackObj,
    SSITestInfo *testInfo) 
{
    unsigned gcdNodeSizes;
    BoundingBoxEx nodeExt;
    int ret;

    // flush any modified nodes to disk first - this is necessary to prevent
    // a subtle problem that can result from a mistmatch of precision with
    // node extents in the cache being in double precision, but nodes written
    // to disk being in single precision; this can result in bad node extent
    // errors being reported.
    
    FlushNodeCache(TRUE);

    // initialize testInfo structure
    testInfo->spaceEfficiency = 0.0;
    testInfo->rTreeHeight = m_ssiHeader->m_rTreeHeight;
    testInfo->nInternalNodes = 0;
    testInfo->nLeafNodes = 0;
    testInfo->nSHPObjects = 0;
    testInfo->nFreeInternalNodes = 0;
    testInfo->nFreeLeafNodes = 0;
    testInfo->internalNodeSize = m_ssiHeader->m_internalNodeSize;
    testInfo->leafNodeSize = m_ssiHeader->m_leafNodeSize;

    // set the progress range on the callback object 
    if (callbackObj && m_ssiHeader->m_totalObjects > 0)
        callbackObj->SetProgressRange(0, m_ssiHeader->m_totalObjects);

    // prepare the node stack
    InitializeNodeStack();

    // setup to test the SSI
    gcdNodeSizes = GCD(m_ssiHeader->m_leafNodeSize, m_ssiHeader->m_internalNodeSize);
 
    // push the root node onto the stack, then call the internal method to test
    // the R-Tree
    if (0 != m_ssiHeader->m_rootNodeOffset)
    {
        // check that the root node offset is valid
        if (!ValidNodeOffset(m_ssiHeader->m_rootNodeOffset, gcdNodeSizes))
            ret = SHP_ERROR_SI_BAD_NODE_OFFSET;
        else
        {
            // push the root node and test the R-Tree
            PushNode(m_ssiHeader->m_rootNodeOffset, GetNodeLevel(1));
            GetNodeExtent(TopNode(), &nodeExt);
            ret = TestRTree(callbackObj, &nodeExt, gcdNodeSizes, testInfo);
            UnwindNodeStack();
        }
    }
    else
        ret = SHP_OK;

    if (SHP_OK == ret)
    {   // handle 100% completion for an empty SSI
        if (callbackObj && m_ssiHeader->m_totalObjects == 0)
            callbackObj->SetProgress(100.0);

        // check to make sure that the total number of objects matches the header
        if (testInfo->nSHPObjects != m_ssiHeader->m_totalObjects) 
            ret = SHP_ERROR_SI_BAD_OBJECT_COUNT;
        else
        {
            // determine the number of entries in the free lists
            testInfo->nFreeInternalNodes = TraverseFreeList(m_ssiHeader->m_freeInternalNode);
            testInfo->nFreeLeafNodes = TraverseFreeList(m_ssiHeader->m_freeLeafNode);

            // finally compute the space efficieny of the R-Tree
            unsigned long totalNodes = testInfo->nInternalNodes + testInfo->nLeafNodes;
            if (totalNodes > 0) 
                testInfo->spaceEfficiency = double(totalNodes+testInfo->nSHPObjects-1) /
                    double(totalNodes * m_ssiHeader->m_maxEntriesPerNode);
            else
                testInfo->spaceEfficiency = 1.0;
        }
    }

    return (ret);
} // end: TestSSI()
       

//-------------------------------------------------------------------------
//
// METHOD: TestRTree().
//
// PURPOSE: Internal recursive method to traverse and test the R-Tree.
//
// PARAMETERS:
//
//     Input:
//
//         callbackObj  - passes a pointer to an ShpSpatialIndexFileCallback object.
//                        This object (if not NULL) is used to report the
//                        current level of progress, and to determine if 
//                        the operation should be cancelled.
//         nodeExt      - passes the extent of the current node (the
//                        one recorded in the parent node).
//         gcdNodeSizes - passes the greatest common divisor of the
//                        leaf and internal node sizes.
//
//     Output:
//
//         testInfo     - passes a pointer to an SSITestInfo structure. 
//                        The information in the structure is updated 
//                        as required.
// 
// RETURNS: 
//  SHP_OK if the test is successful
//  SHP_SI_TRAVERSAL_CANCELLED if the callback has cancelled the test
//  SHP_ERROR_SI_NODE_UNDERFLOW if there are fewer nodes than specified by m_minEntriesPerNode
//  SHP_ERROR_SI_BAD_NODE_EXTENT if the node extent exceeds that of the parent
//  SHP_ERROR_SI_BAD_NODE_OFFSET if there is a child node offset outside the file extents
//
//-------------------------------------------------------------------------

int ShpSpatialIndex::TestRTree(ShpSpatialIndexFileCallback *callbackObj,
    const BoundingBoxEx *nodeExt, unsigned gcdNodeSizes, SSITestInfo *testInfo)
{
    ShpSpatialIndexNode* node;
    BoundingBoxEx extent;
    int ret;

    // check if the test operation has been cancelled
    if (callbackObj && callbackObj->GetCanceled())
        ret = SHP_SI_TRAVERSAL_CANCELLED;
    else
    {
        // get a pointer to the current search node
        node = TopNode();

        // test for the minimum number of entries
        if ( node->m_ssiOffset != m_ssiHeader->m_rootNodeOffset && 
            node->m_nEntries < m_ssiHeader->m_minEntriesPerNode)
            ret = SHP_ERROR_SI_NODE_UNDERFLOW;
        else
        {
            // update test info
            if (AtLeafLevel(node->m_nodeLevel))
            {
                testInfo->nLeafNodes++;
                testInfo->nSHPObjects += node->m_nEntries;
                if (callbackObj)
                    callbackObj->SetProgress(testInfo->nSHPObjects);
            }
            else
                testInfo->nInternalNodes++;

            // make sure that the extent of the node matches the extent the
            // parent node has
            GetNodeExtent(node, &extent);
            if ( extent.xMin != nodeExt->xMin || extent.yMin != nodeExt->yMin || 
                extent.xMax != nodeExt->xMax || extent.yMax != nodeExt->yMax)
                ret = SHP_ERROR_SI_BAD_NODE_EXTENT;
            else
            {   // if at leaf level then just return
                if (AtLeafLevel(node->m_nodeLevel))
                    ret = SHP_OK;
                else
                {   // else descend into each child node ...
                    ret = SHP_OK;
                    for (unsigned i = 0; (i < node->m_nEntries) && (SHP_OK == ret); i++) {
                        // test the file offset of the next child
                        if (!ValidNodeOffset(node->m_fileOffset[i], gcdNodeSizes))
                            ret = SHP_ERROR_SI_BAD_NODE_OFFSET;
                        else
                        {   // push the child node pointer onto the stack and recurse
                            PushNode(node->m_fileOffset[i], node->m_nodeLevel - 1);
                            ret = TestRTree(callbackObj, &node->m_childExt[i], gcdNodeSizes, 
                                testInfo);
                            PopNode();
                        }
                    }
                }
            }
        }
    }

    return (ret);
} // end: TestRTree()

//-------------------------------------------------------------------------
//
// METHOD: ValidFileOffset().
//
// PURPOSE: Tests an SSI file offset for validity. The file offset is
//          considered to be valid if there is a solution to the following
//          linear diophantine equation: ax + by = c, where a = internal
//          node size in bytes, b = leaf node size in bytes, and c =
//          fileOffset - sizeOfSifHeader. If the GCD of a and b divides c
//          then there is a solution.
//
// PARAMETERS:
//
//     Input:
//
//         ssiOffset    - passes the SSI file offset to test.
//         gcdNodeSizes - passes the greatest common divisor of the
//                        sizes of the leaf nodes and internal nodes.
//
//     Output:
//
//         None.
//
// RETURNS: Returns TRUE if the file offset is valid and FALSE otherwise.
//
//-------------------------------------------------------------------------

BOOL ShpSpatialIndex::ValidNodeOffset(unsigned long ssiOffset,
    unsigned gcdNodeSizes) const
{
    if (ssiOffset < sizeof(ShpSpatialIndexHeader) || ssiOffset > m_ssiLength)
        return FALSE;
    else
        return ((ssiOffset - sizeof(ShpSpatialIndexHeader)) % gcdNodeSizes) == 0;

} // end: ValidNodeOffset()


//-------------------------------------------------------------------------
//
// METHOD: TraverseFreeList().
//
// PURPOSE: Traverse the specified list of free nodes to determine how
//          many nodes it contains.
//
// PARAMETERS:
//
//     Input:
//
//         ssiOffset  - passes the SSI offset of the node at the head
//                      of the free list.
//
// RETURNS: The number of free nodes in the list.
//
//-------------------------------------------------------------------------

unsigned long ShpSpatialIndex::TraverseFreeList(unsigned long ssiOffset)
{
    // initialize ...
    unsigned long nFreeNodes = 0;

    while (ssiOffset != 0) {
        // seek the the free node and read the offset of the next free node

        if (!SetFilePointer64((FdoInt64)ssiOffset, FILE_POS_BEGIN))
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::TraverseFreeList(SetFilePointer64)");

        unsigned char nextFreeNode[sizeof(unsigned long)];

        if (!ReadFile(nextFreeNode, 4))
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::TraverseFreeList(ReadFreeNode)");

        ssiOffset = DecodeUI(nextFreeNode, 32);

        nFreeNodes++;
    }

    return (nFreeNodes);
} // end: TraverseFreeList()


//-------------------------------------------------------------------------
//
// METHOD: Defragment().
//
// PURPOSE: Defragment an SSI by removing any free internal or leaf nodes.
//          The defragmentation process also reorders the nodes in a 
//          depth-first manner. This should have the additional benefit of
//          improving performance by minimizing the amount of seeking 
//          requred during searches.
//
// PARAMETERS:
//
//     Input:
//
//         callbackObj  - passes a pointer to an ShpSpatialIndexFileCallback object.
//                        This object (if not NULL) is used to report the
//                        current level of progress, and to determine if 
//                        the operation should be cancelled.
//
//     Output:
//
//         tempFileName - Passes either NULL or a pointer to an array of
//                        char. If the SSI is successfully defragmented to
//                        temporary file, but the original SSI cannot be 
//                        replaced by that temporary (for e.g., the SSI is
//                        in use by another process), then the name of the
//                        temporary file is copied to the location pointed
//                        to.
//
// RETURNS: Returns the SSI nStatus code (see ssi.doc).
//
//-------------------------------------------------------------------------

int ShpSpatialIndex::Defragment(ShpSpatialIndexFileCallback *callbackObj, 
    wchar_t* tempFileName)
{
    wchar_t* tempFile;
    wchar_t* thatFile;
    OpenFlags openFlags;
    wchar_t* thisFile;
    ErrorCode status;
    unsigned char nullBytes[sizeof(ShpSpatialIndexHeader)];
    int ret;

    // flush any modified nodes to disk first
    FlushNodeCache(TRUE);

    // prepare the node stack
    InitializeNodeStack();

    // create a temporary file - the defragmented version is written to the
    // this file and if the defragmentation process is successful, the old
    // SSI is removed and replaced by the temporary file
    if (!GetTempFile (&tempFile, mTempDir))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::Defragment(GetTempFile)");

    // save the temporary file name and delete the allocated memory
    thatFile = (wchar_t*)alloca (sizeof(wchar_t) * (wcslen (tempFile) + 1));
    wcscpy (thatFile, tempFile);
    delete[] tempFile;
    // remember details about this file
    if (IsReadOnly ())
        openFlags = IDF_OPEN_READ;
    else
        openFlags = (OpenFlags)(IDF_OPEN_UPDATE | IDF_OPEN_EXISTING);
    thisFile = (wchar_t*)alloca (sizeof(wchar_t) * (wcslen (FileName ()) + 1));
    wcscpy (thisFile, FileName ());

    ShpSpatialIndex defragFile (thatFile, NULL, (eShapeTypes)m_ssiHeader->m_type, m_ssiHeader->Valid_M ());

    try
    {
        // set the progress range on the callback object 
        if (callbackObj && m_ssiHeader->m_totalObjects > 0)
            callbackObj->SetProgressRange(0, m_ssiHeader->m_totalObjects);
        
        // reserve space at the beginning of the defragmented file for the header
        // block
        ::memset (nullBytes, 0, sizeof(ShpSpatialIndexHeader));
        if (!defragFile.WriteFile (nullBytes, sizeof(ShpSpatialIndexHeader)))
            throw FdoException::Create (NlsMsgGet(SHP_SI_TEMP_FILE_WRITE_ERROR, "Cannot write temporary file while defragmenting the Spatial Index."));

        // push the root node onto the stack, then call the internal method to
        // perform the defragmentation
        if (m_ssiHeader->m_rootNodeOffset != 0)
        {
            PushNode(m_ssiHeader->m_rootNodeOffset, GetNodeLevel(1), FALSE);
            ret = Defragment(&defragFile, m_ssiHeader->m_rootNodeOffset, callbackObj);
            UnwindNodeStack();
            FlushNodeCache(FALSE);
        }

        if (SHP_OK != ret)
        {
            defragFile.CloseFile();
            ShpSpatialIndexFile::Delete(tempFile);
        }
        else
        {
            if (m_ssiHeader->m_totalObjects == 0)
                callbackObj->SetProgress(100.0);

            // write out the SSI header block
            m_ssiHeader->m_freeInternalNode = 0;
            m_ssiHeader->m_freeLeafNode = 0;
            defragFile.WriteSSIHeader ();

            // replace the old SSI with the temporary file
            if (!defragFile.CloseFile())
                throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::Defragment(CloseTempFile)");
            else
            {
                if (!CloseFile())
                    throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::Defragment(CloseThisFile)");

                if (!Move (thatFile, thisFile))
                {
                    if (tempFileName == NULL)
                        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::Defragment(Move)");
                    else
                        ::wcscpy (tempFileName, thatFile);
                    ret = SHP_ERROR_SI_CANT_REPLACE_SSI;
                }
                else
                {
					if (!OpenFile (thisFile, openFlags, status))
                        throw FdoCommonFile::ErrorCodeToException (status, thisFile, openFlags);

   					FdoInt64 ssiLength64;
     
					if (!GetFileSize64(ssiLength64))
                        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::Defragment(GetFileSize)");
					else
						m_ssiLength = (ULONG)ssiLength64;
                }
            }
        }
    }
    catch (...)
    {
        defragFile.CloseFile ();
        ShpSpatialIndexFile::Delete (thatFile);
        throw;
    }

    return (ret);

} // end: Defragment()

//-------------------------------------------------------------------------
//
// METHOD: Defragment().
//
// PURPOSE: Internal method to defragment the SSI.
//
// PARAMETERS:
//
//     Input:
//
//         defragFile  - passes a pointer to the temporary file the 
//                       defragmented version of the SSI is written to.
//         callbackObj - passes a pointer to an ShpSpatialIndexFileCallback object.
//                       This object (if not NULL) is used to report the
//                       current level of progress, and to determine if 
//
//     Output:
//
//         ssiOffset   - passes a reference to an unsigned long integer. 
//                       The new SSI offset of the node at the current
//                       level of recursion is copied to the referenced
//                       integer on return.
//
// RETURNS: SHP_OK if the operation succeeded, SHP_SI_TRAVERSAL_CANCELLED otherwise.
//
//-------------------------------------------------------------------------

int ShpSpatialIndex::Defragment(ShpSpatialIndex* defragFile, unsigned long &ssiOffset,
    ShpSpatialIndexFileCallback *callbackObj)
{
    ShpSpatialIndexNode* node;
    unsigned nBytes;
    unsigned char* nullBytes;
    unsigned childLevel;
    int ret;
	FdoInt64 ssiOffset64;

    ret = SHP_OK;

    // get a pointer to the current node
    node = TopNode();
    
    // seek to the end of the file, and assign the new SSI offset of the node

    if (!defragFile->SetFilePointer64((FdoInt64)0, FILE_POS_END))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::Defragment(SetFilePointer64)");
    if (!defragFile->GetFilePointer64(ssiOffset64))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::Defragment(GetFilePointer64)");
	else
		ssiOffset = (ULONG)ssiOffset64;

    node->m_ssiOffset = ssiOffset;

    if (!AtLeafLevel(node->m_nodeLevel))
    {   // reserve space in the SSI for the internal node
        nBytes = m_ssiHeader->m_internalNodeSize;
        nullBytes = (unsigned char*)alloca (nBytes);
        ::memset(nullBytes, 0, nBytes);
        if (!defragFile->WriteFile(&nullBytes, nBytes))
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::Defragment(WriteBytes)");

        // rescursively descend into each child node
        childLevel = node->m_nodeLevel - 1;
        for (unsigned i = 0; (i < node->m_nEntries) && (SHP_OK == ret); i++)
        {
            PushNode(node->m_fileOffset[i], childLevel, FALSE);
            ret = Defragment(defragFile, node->m_fileOffset[i], callbackObj);
            PopNode();
            // check if the test operation has been cancelled
            if (callbackObj && callbackObj->GetCanceled())
                ret = SHP_SI_TRAVERSAL_CANCELLED;
        }
    }
    else
    {
        if (callbackObj)
        {
            double progress = callbackObj->GetProgress() + node->m_nEntries;
            callbackObj->SetProgress(progress);
            // check if the test operation has been cancelled
            if (callbackObj->GetCanceled())
                ret = SHP_SI_TRAVERSAL_CANCELLED;
        }
    }

    if (SHP_OK == ret)
        // write out the node
        defragFile->WriteNode(node);

    return (ret);

} // end: Defragment()


//-------------------------------------------------------------------------
//
// METHOD: ReadSSIHeader().
//
// PURPOSE: Read the SSI header from the SSI.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::ReadSSIHeader()
{
    // seek to beginning of the SSI

    if (!SetFilePointer64((FdoInt64)0, FILE_POS_BEGIN))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::ReadSSIHeader(SetFilePointer64)");

    // read in the header section of the SSI

    unsigned char buffer[sizeof(ShpSpatialIndexHeader)];

    if (!ReadFile(buffer, sizeof(ShpSpatialIndexHeader)))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::ReadSSIHeader(ReadHeader)");

    // check if the file is an SSI

    if (DecodeUI(buffer, 32) != SHP_ERROR_SI_MAGIC_NUMBER)
        throw FdoException::Create (NlsMsgGet(SHP_SI_NOT_AN_SSI, "Corrupted Spatial Index file '%1$ls'.", FileName ()));

    // now, decode the contents of the header (skip over magic number and
    // index type elements for now)

    unsigned bufferPos = 6;
    
    m_ssiHeader->m_version = unsigned(DecodeUI(&buffer[bufferPos], 16));
    bufferPos += 2;    

    // check for a version mismatch

    if (m_ssiHeader->m_version > SHP_SPATIAL_INDEX_VERSION)
        throw FdoException::Create (NlsMsgGet(SHP_SI_VERSION_MISMATCH, "Version mismatch '%1$ls', expected %2$d, found %3$d.", FileName (), SHP_SPATIAL_INDEX_VERSION, m_ssiHeader->m_version));

#ifdef _WIN32

    m_ssiHeader->m_ftSHPWriteTime.dwLowDateTime = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

    m_ssiHeader->m_ftSHPWriteTime.dwHighDateTime = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

#else

    m_ssiHeader->m_ftSHPWriteTime = (time_t)DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 8;

#endif

    m_ssiHeader->m_ulSHPSize = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

    m_ssiHeader->m_minEntriesPerNode = unsigned(DecodeUI(&buffer[bufferPos], 16));
    bufferPos += 2;    

    m_ssiHeader->m_maxEntriesPerNode = unsigned(DecodeUI(&buffer[bufferPos], 16));
    bufferPos += 2;    

    m_ssiHeader->m_rTreeHeight = unsigned(DecodeUI(&buffer[bufferPos], 16));
    bufferPos += 2;    

    m_ssiHeader->m_floatPrecision = unsigned(DecodeUI(&buffer[bufferPos], 16));
    bufferPos += 2;    

    m_ssiHeader->m_rootNodeOffset = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

    m_ssiHeader->m_freeInternalNode = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

    m_ssiHeader->m_freeLeafNode = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

    m_ssiHeader->m_type = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

    m_ssiHeader->m_dimensions = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

    m_ssiHeader->m_totalObjects = DecodeUI(&buffer[bufferPos], 32);
    bufferPos += 4;    

    DecodeString(&buffer[bufferPos], m_ssiHeader->m_description);
 
    m_ssiHeader->ComputeNodeSizes();

} // end: ReadSSIHeader()


//-------------------------------------------------------------------------
//
// METHOD: WriteSSIHeader().
//
// PURPOSE: Write the SSI header back to the SSI.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::WriteSSIHeader()
{
    // evaluate node sizes based on header settings
    m_ssiHeader->ComputeNodeSizes ();

    // seek to beginning of the SSI
    if (!SetFilePointer64((FdoInt64)0, FILE_POS_BEGIN))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::WriteSSIHeader(SetFilePointer64)");

    // zero initialize the buffer the header will be encoded into

    unsigned char buffer[sizeof(ShpSpatialIndexHeader)];
    ::memset (buffer, 0, sizeof(ShpSpatialIndexHeader));

    // encode the SSI elements into the buffer that will get written out

    unsigned bufferPos = 0;

    EncodeUI(SHP_ERROR_SI_MAGIC_NUMBER, 32, buffer);
    bufferPos = 4;

    EncodeUI(SHP_ERROR_SI_RTREE, 16, &buffer[bufferPos]);
    bufferPos += 2;

    EncodeUI(m_ssiHeader->m_version, 16, &buffer[bufferPos]);
    bufferPos += 2;

#ifdef _WIN32
    EncodeUI(m_ssiHeader->m_ftSHPWriteTime.dwLowDateTime, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeUI(m_ssiHeader->m_ftSHPWriteTime.dwHighDateTime, 32, &buffer[bufferPos]);
    bufferPos += 4;
#else // _WIN32
    EncodeUI(m_ssiHeader->m_ftSHPWriteTime, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeUI(0, 32, &buffer[bufferPos]);
    bufferPos += 4;
#endif // _WIN32

    EncodeUI(m_ssiHeader->m_ulSHPSize, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeUI(m_ssiHeader->m_minEntriesPerNode, 16, &buffer[bufferPos]);
    bufferPos += 2;

    EncodeUI(m_ssiHeader->m_maxEntriesPerNode, 16, &buffer[bufferPos]);
    bufferPos += 2;

    EncodeUI(m_ssiHeader->m_rTreeHeight, 16, &buffer[bufferPos]);
    bufferPos += 2;

    EncodeUI(m_ssiHeader->m_floatPrecision, 16, &buffer[bufferPos]);
    bufferPos += 2;

    EncodeUI(m_ssiHeader->m_rootNodeOffset, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeUI(m_ssiHeader->m_freeInternalNode, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeUI(m_ssiHeader->m_freeLeafNode, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeUI(m_ssiHeader->m_type, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeUI(m_ssiHeader->m_dimensions, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeUI(m_ssiHeader->m_totalObjects, 32, &buffer[bufferPos]);
    bufferPos += 4;

    EncodeString(m_ssiHeader->m_description, &buffer[bufferPos]);

    if (!WriteFile(buffer, sizeof(ShpSpatialIndexHeader))) 
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::WriteSSIHeader(WriteBuffer)");
}


//-------------------------------------------------------------------------
//
// METHOD: AtLeafLevel().
//
// PURPOSE: Internal method to determine whether a node at the specified
//          level is at the leaf level of the R-Tree.
//
// PARAMETERS:
//
//     Input:
//
//         nodeLevel - passes the level of the node.
//
//     Output:
//
//         None.
//
// RETURNS: Returns TRUE if a node at the specified level is a leaf node
//          and FALSE otherwise.
//
//-------------------------------------------------------------------------

BOOL ShpSpatialIndex::AtLeafLevel(unsigned nodeLevel) const
{
    return nodeLevel == 0;

} // end: AtLeafLevel()


//-------------------------------------------------------------------------
//
// METHOD: GetNodeLevel().
//
// PURPOSE: Determine the level of a node given its height. The height of
//          a node is measured with respect to the root node, with the 
//          root being at height 1. The level of a node is measured with
//          respect to the leaf level - the leaves are level 0.
//
// PARAMETERS:
//
//     Input:
//
//         nodeHeight - passes the height of the node.
//
//     Output:
//
//         None.
//
// RETURNS: Returns the level of the node.
//
//-------------------------------------------------------------------------

unsigned ShpSpatialIndex::GetNodeLevel(unsigned nodeHeight) const
{
    return m_ssiHeader->m_rTreeHeight - nodeHeight;

} // end: GetNodeLevel()


//-------------------------------------------------------------------------
//
// METHOD: GetNode().
//
// PURPOSE: Allocate a new R-Tree node, install it in the node cache, and
//          return a pointer to it.
//
// PARAMETERS:
//
//     Input:
//
//         nodeLevel - passes the level of the node.
//
//     Output:
//
//         nStatus    - passes a reference to an integer. The SSI nStatus
//                     code is copied to the referenced integer.
//
// RETURNS: Returns a pointer to the node.
//
//-------------------------------------------------------------------------

ShpSpatialIndexNode *ShpSpatialIndex::GetNode(unsigned nodeLevel)
{
    // get the least recently used ShpSpatialIndexNode from the cache

    ShpSpatialIndexNode *node = GetLRUNode();

    // zero initialize the node

    ::memset(node, 0, sizeof(ShpSpatialIndexNode));
    ::memset(node->m_fileOffset, 0xff, sizeof(node->m_fileOffset));

    // allocate a new node from the SSI

    AllocateNode(nodeLevel, node->m_ssiOffset);

    // initialize the rest of the node structure
    node->m_refCount = 0;
    node->m_lruStamp = m_lruStamp;
    node->m_currentSubtree = 0;
    node->m_nodeLevel = nodeLevel;
    node->m_nodeModified = FALSE;
    node->m_nEntries = 0;
    return node;
} // end: GetNode()


//-------------------------------------------------------------------------
//
// METHOD: GetNode().
//
// PURPOSE: Get an R-Tree node from the SSI, and return the pointer to 
//          the cache entry it was read into.
//
// PARAMETERS:
//
//     Input:
//
//         ssiOffset - passes the SSI offset of the node.
//         nodeLevel - passes the level of the node.
//
//     Output:
//
//         nStatus    - passes a reference to an integer. The SSI nStatus
//                     code is copied to the referenced integer.
//
// RETURNS: Returns a pointer to the node.
//
//-------------------------------------------------------------------------

ShpSpatialIndexNode *ShpSpatialIndex::GetNode(unsigned long ssiOffset, 
    unsigned nodeLevel)
{
    // if the requested node is the root then update the LRU time stamp - the
    // LRU stamp is only updated when the root node is accessed, all other 
    // nodes on the path from the root receive the same LRU "time" stamp. If 
    // the LRU stamp was updated for each node access, then nodes higher up 
    // in the tree would get swapped out of the cache, as nodes lower in the 
    // tree were accessed, which is not what we want
    
    if (ssiOffset == m_ssiHeader->m_rootNodeOffset)
        UpdateLRUStamp();

    // if the node is already in the cache, then stamp it and return

    ShpSpatialIndexNode *node = GetCachedNode(ssiOffset);
    if (node != NULL) {
        node->m_lruStamp = m_lruStamp;
        node->m_currentSubtree = 0;
        return node;
    }

    // get the least recently used cache entry

    node = GetLRUNode();

    // read the specified node from the SSI

    ReadNode(ssiOffset, nodeLevel, node);

    // stamp the node and return a pointer to it
    node->m_lruStamp = m_lruStamp;
    node->m_currentSubtree = 0;
    return node;
} // end: GetNode()


//-------------------------------------------------------------------------
//
// METHOD: GetCachedNode().
//
// PURPOSE: Check the cache to see if it contains the node with the 
//          specified SSI offset, returning a pointer to the node if so.
//
// PARAMETERS:
//
//     Input:
//
//         ssiOffset - passes the SSI offset of the node.
//
//     Output:
//
//         None.
//
// RETURNS: Returns a pointer to the node or NULL if the node is not in
//          the cache.
//
//-------------------------------------------------------------------------

ShpSpatialIndexNode *ShpSpatialIndex::GetCachedNode(unsigned long ssiOffset) const
{
    for (int i = 0; i < NODE_CACHE_SIZE; i++) {
        if (m_nodeCache[i]->m_ssiOffset == ssiOffset)
            return m_nodeCache[i];
    }

    return NULL;

} // end: GetCachedNode()


//-------------------------------------------------------------------------
//
// METHOD: GetLRUNode().
//
// PURPOSE: Get the least recently used node (that is not in use) from the 
//          cache.
//
// PARAMETERS:
//
//     Input:
//
//         None.
//
//     Output:
//
//         nStatus    - passes a reference to an integer. The SSI nStatus
//                     code is copied to the referenced integer.
//
// RETURNS: Returns a pointer to the least recently used node.
//
//-------------------------------------------------------------------------

ShpSpatialIndexNode *ShpSpatialIndex::GetLRUNode()
{
    // initialize ...

    ShpSpatialIndexNode *lruNode = NULL;
    unsigned minNodeLevel = UINT_MAX;
    unsigned long minLruStamp = ULONG_MAX;

    // select the node with the smallest LRU stamp, breaking ties by 
    // selecting the node with the lowest level

    for (int i = 0; i < NODE_CACHE_SIZE; i++) {
        // skip the current node if it is in use

        if (m_nodeCache[i]->m_refCount > 0)
            continue;

        if ( (m_nodeCache[i]->m_lruStamp < minLruStamp) || 
             (m_nodeCache[i]->m_lruStamp == minLruStamp && 
              m_nodeCache[i]->m_nodeLevel < minNodeLevel)) {
            lruNode = m_nodeCache[i];
            minLruStamp = m_nodeCache[i]->m_lruStamp;
            minNodeLevel = m_nodeCache[i]->m_nodeLevel;
        }
    }

    // write out the node if it has been modified

    if (lruNode->m_nodeModified)
    {
        try
        {
            WriteNode(lruNode);
        }
        catch (...)
        {
            lruNode->m_nodeModified = FALSE;
            throw;
        }
        lruNode->m_nodeModified = FALSE;
    }

    // return a pointer to the LRU node

    return lruNode;

} // end: GetLRUNode()


//-------------------------------------------------------------------------
//
// METHOD: UpdateLRUStamp().
//
// PURPOSE: Update the LRU "time" stamp.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::UpdateLRUStamp()
{
    // handle the (extremely unlikely) case where the LRU time stamp is 
    // about to overflow

    if (m_lruStamp == ULONG_MAX) {
        unsigned long minLRUStamp = ULONG_MAX;

        int i;
        for (i = 0; i < NODE_CACHE_SIZE; i++) {
            if (m_nodeCache[i]->m_lruStamp < minLRUStamp)
                minLRUStamp = m_nodeCache[i]->m_lruStamp;
        }

        for (i = 0; i < NODE_CACHE_SIZE; i++) 
            m_nodeCache[i]->m_lruStamp -= minLRUStamp;

        m_lruStamp -= minLRUStamp;
    }

    m_lruStamp++;

} // end: UpdateLRUStamp()


//-------------------------------------------------------------------------
//
// METHOD: FlushNodeCache().
//
// PURPOSE: Write any modified nodes in the node cache back to the SSI (if
//          requested), and reinitialize the cache.
//
// PARAMETERS:
//
//     Input:
//
//         writeModified - passes a boolean indicating whether nodes marked
//                         as modified should be flushed to disk.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::FlushNodeCache(BOOL writeModified)
{
    // write out any modified nodes, and reinitialize each node 

    for (int i = 0; i < NODE_CACHE_SIZE; i++)
    {
        if (writeModified && m_nodeCache[i]->m_nodeModified)
            try
            {
                WriteNode(m_nodeCache[i]);
            }
            catch (...)
            {
                m_nodeCache[i]->m_ssiOffset = 0;
                m_nodeCache[i]->m_lruStamp = 0;
                m_nodeCache[i]->m_currentSubtree = 0;       
                m_nodeCache[i]->m_nodeLevel = 0;       
                m_nodeCache[i]->m_nodeModified = FALSE;
                m_nodeCache[i]->m_nEntries = 0;
                throw;
            }

        m_nodeCache[i]->m_ssiOffset = 0;
        m_nodeCache[i]->m_lruStamp = 0;
        m_nodeCache[i]->m_currentSubtree = 0;       
        m_nodeCache[i]->m_nodeLevel = 0;       
        m_nodeCache[i]->m_nodeModified = FALSE;
        m_nodeCache[i]->m_nEntries = 0;
    }

    // reinitialize the LRU stamp

    m_lruStamp = 0;
} // end: FlushNodeCache()


//-------------------------------------------------------------------------
//
// METHOD: InitializeNodeStack().
//
// PURPOSE: Prepare the node stack for use.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::InitializeNodeStack()
{
    if (m_topOfStack >= 0)
        UnwindNodeStack();
    m_topOfStack = -1;

} // end: InitializeNodeStack()


//-------------------------------------------------------------------------
//
// METHOD: PushNode().
//
// PURPOSE: Push the specified node onto the stack.
//
// PARAMETERS:
//
//     Input:
// 
//         ssiOffset  - passes the file offset of the node in the SSI.
//         nodeLevel  - passes the level of the node.
//         checkCache - passes a boolean (default value of TRUE) indicating
//                      whether the cache should be checked to determine
//                      if the node has already been read in, and to use
//                      the previously read in node if so.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::PushNode(unsigned long ssiOffset, unsigned nodeLevel,
    BOOL checkCache)
{
    // read the node into the slot on the top of the stack
    ShpSpatialIndexNode *node;

    // if required to check the cache, then call GetNode() internal method to 
    // get the node (it checks the cache)

    if (checkCache)
        node = GetNode(ssiOffset, nodeLevel);

    // else get the LRU node, and read the node directly into it

    else {
        node = GetLRUNode();

        // read the specified node from the SSI

        ReadNode(ssiOffset, nodeLevel, node);

        // stamp the node
        node->m_lruStamp = m_lruStamp;
    }

    // push the node onto the stack
    
    m_nodeStack[++m_topOfStack] = node;
} // end: PushNode()


//-------------------------------------------------------------------------
//
// METHOD: TopNode().
//
// PURPOSE: Return a pointer to the node on the top of the node stack.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: Returns a pointer to the node on the top of the stack (NULL
//          if the stack is empty).
//
//-------------------------------------------------------------------------

ShpSpatialIndexNode *ShpSpatialIndex::TopNode() const
{
    if (m_topOfStack >= 0)
        return m_nodeStack[m_topOfStack];
    else
        return NULL;

} // end: TopNode()


//-------------------------------------------------------------------------
//
// METHOD: PopNode().
//
// PURPOSE: Pop the node from the top of the node stack.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::PopNode()
{
    if (m_topOfStack >= 0) {
        m_nodeStack[m_topOfStack].DetachNode();
        m_topOfStack--;
    }

} // end: PopNode()


//-------------------------------------------------------------------------
//
// METHOD: UnwindNodeStack().
//
// PURPOSE: Pop all nodes from the node stack.
//
// PARAMETERS:
//
//     None.
//
// RETURNS: None
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::UnwindNodeStack()
{
    while (m_topOfStack >= 0)
        PopNode();

} // end: UnwindNodeStack()


//-------------------------------------------------------------------------
//
// METHOD: AllocateNode().
//
// PURPOSE: Allocate a new ShpSpatialIndexNode from the SSI. Whenever possible, the
//          allocated node is reclaimed from the appropriate free list.
//
// PARAMETERS:
//
//     Input:
//
//         nodeLevel - passes the level of the node.
//
//     Output:
//
//         ssiOffset - passes a reference to an unsigned long integer; the
//                     SSI offset of the newly allocated node is copied to
//                     the referenced integer.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::AllocateNode(unsigned nodeLevel, unsigned long &ssiOffset)
{
    // reclaim the node from one of the free lists if possible
    
    BOOL leafNode = AtLeafLevel(nodeLevel);

    if ( (leafNode && m_ssiHeader->m_freeLeafNode != 0) ||
         (!leafNode && m_ssiHeader->m_freeInternalNode != 0)) {
        // get the offset of the (appropriate) free node

        if (leafNode) 
            ssiOffset = m_ssiHeader->m_freeLeafNode;
        else 
            ssiOffset = m_ssiHeader->m_freeInternalNode;

        // update the free node link (the free lists are maintained as 
        // singly linked lists)

        if (!SetFilePointer64((FdoInt64)ssiOffset, FILE_POS_BEGIN))
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::AllocateNode(SetFilePointer1)");

        unsigned char nextFreeNode[sizeof(unsigned long)];

        if (!ReadFile(nextFreeNode, 4))
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::AllocateNode(ReadNextFreeNode)");

        if (leafNode) 
            m_ssiHeader->m_freeLeafNode = DecodeUI(nextFreeNode, 32);
        else
            m_ssiHeader->m_freeInternalNode = DecodeUI(nextFreeNode, 32);
    }

    // otherwise allocate the node at the end of the SSI

    else {
        // seek to the end of the file
        if (!SetFilePointer64((FdoInt64)0, FILE_POS_END))
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::AllocateNode(SetFilePointer2)");

		FdoInt64	ssiOffset64;

        if (!GetFilePointer64(ssiOffset64))
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::AllocateNode(GetFilePointer2)");
		else
			ssiOffset = (ULONG)ssiOffset64;

        // write out a buffer of zeros at the current file position so that
        // the new node is actually allocated - the number of bytes written
        // depends on whether the node is a leaf or not

        unsigned char buffer[sizeof(ShpSpatialIndexNode)];
        ::memset(buffer, 0, sizeof(ShpSpatialIndexNode));

        unsigned nodeSize;
        if (leafNode)
            nodeSize = m_ssiHeader->m_leafNodeSize;
        else
            nodeSize = m_ssiHeader->m_internalNodeSize;

        if (WriteFile(buffer, nodeSize))
            m_ssiLength += nodeSize;
        else
            throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::AllocateNode(WriteBuffer)");
    }

} // end: AllocateNode()


//-------------------------------------------------------------------------
//
// METHOD: AddNodeToFreeList().
//
// PURPOSE: Add an eliminated node to the leaf-node or internal-node free
//          list.
//
// PARAMETERS:
//
//     Input:
//
//         node - passes a reference to the eliminated node.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::AddNodeToFreeList(ShpSpatialIndexNode *node)
{
    // link the node into the appropriate free list

    if (!SetFilePointer64((FdoInt64)node->m_ssiOffset, FILE_POS_BEGIN))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::AddNodeToFreeList(SetFilePointer64)");

    unsigned char nextFreeNode[sizeof(unsigned long)];

    if (AtLeafLevel(node->m_nodeLevel)) 
        EncodeUI(m_ssiHeader->m_freeLeafNode, 32, nextFreeNode);
    else 
        EncodeUI(m_ssiHeader->m_freeInternalNode, 32, nextFreeNode);

    if (!WriteFile(nextFreeNode, 4))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::AddNodeToFreeList(WriteFreeNode)");

    if (AtLeafLevel(node->m_nodeLevel))
        m_ssiHeader->m_freeLeafNode = node->m_ssiOffset;
    else
        m_ssiHeader->m_freeInternalNode = node->m_ssiOffset;

    // reinitialize the node - NOTE - the refCount is not reset to 0 since
    // some ShpSpatialIndexHandle may still be referencing the node

    node->m_ssiOffset = 0;
    node->m_lruStamp = 0;
    node->m_currentSubtree = 0;
    node->m_nodeLevel = 0;
    node->m_nodeModified = FALSE;
    node->m_nEntries = 0;
} // end: AddNodeToFreeList()


//-------------------------------------------------------------------------
//
// METHOD: ReadNode().
//
// PURPOSE: Read an ShpSpatialIndexNode from the SSI.
//
// PARAMETERS:
//
//     Input:
//
//         ssiOffset - passes the SSI offset of the node.
//         nodeLevel - passes the level of the node.
//
//     Output:
//
//         node      - passes a pointer to an ShpSpatialIndexNode. The decoded form
//                     of the node contents is copied to the structure 
//                     pointed to.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------

void ShpSpatialIndex::ReadNode(unsigned long ssiOffset, unsigned nodeLevel,
    ShpSpatialIndexNode *node)
{
    // seek to the file position where the node is located

    if (!SetFilePointer64((FdoInt64)ssiOffset, FILE_POS_BEGIN))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::ReadNode(SetFilePointer64)");

    // read the node into an internal buffer

    unsigned char buffer[sizeof(ShpSpatialIndexNode)];
    unsigned nodeSize;
    if (AtLeafLevel(nodeLevel))
        nodeSize = m_ssiHeader->m_leafNodeSize;
    else
        nodeSize = m_ssiHeader->m_internalNodeSize;

    if (!ReadFile(buffer, nodeSize))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::ReadNode(ReadBuffer)");

    // now, decode the contents of the buffer

    unsigned i, bufferPos;

    // decode the file offsets

    node->m_nEntries = 0;
    for (i=0, bufferPos=0; i<m_ssiHeader->m_maxEntriesPerNode; i++, bufferPos+=4) {
        node->m_fileOffset[i] = DecodeUI(&buffer[bufferPos], 32);
        if (node->m_fileOffset[i] != -1L)
            node->m_nEntries++;
    }

    // decode the bounding boxes

    unsigned floatPrec = m_ssiHeader->m_floatPrecision;
    unsigned bytesPerFloat = floatPrec >> 3;

    for (i = 0; i < m_ssiHeader->m_maxEntriesPerNode; i++) {
        node->m_childExt[i].xMin = DecodeFloat(&buffer[bufferPos], floatPrec);
        bufferPos += bytesPerFloat;
        node->m_childExt[i].yMin = DecodeFloat(&buffer[bufferPos], floatPrec);
        bufferPos += bytesPerFloat;
        if (m_ssiHeader->Valid_Z ())
        {
            node->m_childExt[i].zMin = DecodeFloat(&buffer[bufferPos], floatPrec);
            bufferPos += bytesPerFloat;
        }
        if (m_ssiHeader->Valid_M ())
        {
            node->m_childExt[i].mMin = DecodeFloat(&buffer[bufferPos], floatPrec);
            bufferPos += bytesPerFloat;
        }
        node->m_childExt[i].xMax = DecodeFloat(&buffer[bufferPos], floatPrec);
        bufferPos += bytesPerFloat;
        node->m_childExt[i].yMax = DecodeFloat(&buffer[bufferPos], floatPrec);
        bufferPos += bytesPerFloat;
        if (m_ssiHeader->Valid_Z ())
        {
            node->m_childExt[i].zMax = DecodeFloat(&buffer[bufferPos], floatPrec);
            bufferPos += bytesPerFloat;
        }
        if (m_ssiHeader->Valid_M ())
        {
            node->m_childExt[i].mMax = DecodeFloat(&buffer[bufferPos], floatPrec);
            bufferPos += bytesPerFloat;
        }
    }

    // initialize the bookkeeping information on the node

    node->m_ssiOffset = ssiOffset;
    node->m_refCount = 0;
    node->m_lruStamp = 0;
    node->m_currentSubtree = 0;
    node->m_nodeLevel = nodeLevel;
    node->m_nodeModified = FALSE;
} // end: ReadNode()


//-------------------------------------------------------------------------
//
// METHOD: WriteNode().
//
// PURPOSE: Write an ShpSpatialIndexNode to the SSI.
//
// PARAMETERS:
//
//     Input:
//
//         outputFile - passes a pointer to the file the node is to
//                      be written to.
//         node       - passes a pointer to the ShpSpatialIndexNode.
//
//     Output:
//
//         None.
//
// RETURNS: None.
//
//-------------------------------------------------------------------------
void ShpSpatialIndex::WriteNode(const ShpSpatialIndexNode *node)
{
    // seek to the file position where the node is located

    if (!SetFilePointer64(node->m_ssiOffset, FILE_POS_BEGIN))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::WriteNode(SetFilePointer64)");

    // write the node into an internal buffer

    unsigned char buffer[sizeof(ShpSpatialIndexNode)];
    unsigned i, bufferPos;

    // write the file offsets to the buffer

    for (i=0, bufferPos=0; i < m_ssiHeader->m_maxEntriesPerNode; i++, bufferPos+=4) {
        if (i < node->m_nEntries)
            EncodeUI(node->m_fileOffset[i], 32, &buffer[bufferPos]); 
        else
            EncodeUI((unsigned long)-1L, 32, &buffer[bufferPos]);
    }

    // write the bounding boxes

    unsigned floatPrec = m_ssiHeader->m_floatPrecision;
    unsigned bytesPerFloat = floatPrec >> 3;

    for (i = 0; i < m_ssiHeader->m_maxEntriesPerNode; i++) {
        if (i < node->m_nEntries) {
            EncodeFloat(node->m_childExt[i].xMin, floatPrec, &buffer[bufferPos]);
            bufferPos += bytesPerFloat;
            EncodeFloat(node->m_childExt[i].yMin, floatPrec, &buffer[bufferPos]);
            bufferPos += bytesPerFloat;
            if (m_ssiHeader->Valid_Z ())
            {
                EncodeFloat(node->m_childExt[i].zMin, floatPrec, &buffer[bufferPos]);
                bufferPos += bytesPerFloat;
            }
            if (m_ssiHeader->Valid_M ())
            {
                EncodeFloat(node->m_childExt[i].mMin, floatPrec, &buffer[bufferPos]);
                bufferPos += bytesPerFloat;
            }
            EncodeFloat(node->m_childExt[i].xMax, floatPrec, &buffer[bufferPos]);
            bufferPos += bytesPerFloat;
            EncodeFloat(node->m_childExt[i].yMax, floatPrec, &buffer[bufferPos]);
            bufferPos += bytesPerFloat;
            if (m_ssiHeader->Valid_Z ())
            {
                EncodeFloat(node->m_childExt[i].zMax, floatPrec, &buffer[bufferPos]);
                bufferPos += bytesPerFloat;
            }
            if (m_ssiHeader->Valid_M ())
            {
                EncodeFloat(node->m_childExt[i].mMax, floatPrec, &buffer[bufferPos]);
                bufferPos += bytesPerFloat;
            }
        }
        else {
            EncodeFloat(0.0, floatPrec, &buffer[bufferPos]);
            bufferPos += bytesPerFloat;
            EncodeFloat(0.0, floatPrec, &buffer[bufferPos]);
            bufferPos += bytesPerFloat;
            if (m_ssiHeader->Valid_Z ())
            {
                EncodeFloat(0.0, floatPrec, &buffer[bufferPos]);
                bufferPos += bytesPerFloat;
            }
            if (m_ssiHeader->Valid_M ())
            {
                EncodeFloat(0.0, floatPrec, &buffer[bufferPos]);
                bufferPos += bytesPerFloat;
            }
            EncodeFloat(0.0, floatPrec, &buffer[bufferPos]);
            bufferPos += bytesPerFloat;
            EncodeFloat(0.0, floatPrec, &buffer[bufferPos]);
            bufferPos += bytesPerFloat;
            if (m_ssiHeader->Valid_Z ())
            {
                EncodeFloat(0.0, floatPrec, &buffer[bufferPos]);
                bufferPos += bytesPerFloat;
            }
            if (m_ssiHeader->Valid_M ())
            {
                EncodeFloat(0.0, floatPrec, &buffer[bufferPos]);
                bufferPos += bytesPerFloat;
            }
        }
    }

    // write out the node 

    unsigned nodeSize;

    if (AtLeafLevel(node->m_nodeLevel))
        nodeSize = m_ssiHeader->m_leafNodeSize;
    else
        nodeSize = m_ssiHeader->m_internalNodeSize;

    if (!WriteFile(buffer, nodeSize))
        throw FdoCommonFile::LastErrorToException (L"ShpSpatialIndex::WriteNode(WriteBuffer)");
} // end: WriteNode()

bool ShpSpatialIndex::ShapeHasZ (eShapeTypes type)
{
    bool ret;

    switch (type)
    {
        case eNullShape:
        case ePointShape:
        case ePolylineShape:
        case ePolygonShape:
        case eMultiPointShape:
            ret = false;
            break;
        case ePointZShape:
        case ePolylineZShape:
        case ePolygonZShape:
        case eMultiPointZShape:
        case ePointMShape:
        case ePolylineMShape:
        case ePolygonMShape:
        case eMultiPointMShape:
        case eMultiPatchShape:
            ret = true;
            break;
        default:
            throw FdoException::Create (NlsMsgGet(SHP_UNKNOWN_SHAPE_TYPE, "The shape type number '%1$d' is unknown.", type));
            break;
    }

    return (ret);
}

