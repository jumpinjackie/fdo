// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "RTree.h"

//It is not good form to define these here, but it avoids redefinition
//errors
int NODECARD = MAXCARD;
int LEAFCARD = MAXCARD;

#define MinNodeFill (NODECARD / 2)
#define MinLeafFill (LEAFCARD / 2)

#define MAXKIDS(n) ((n).level > 0 ? NODECARD : LEAFCARD)
#define MINFILL(n) ((n).level > 0 ? MinNodeFill : MinLeafFill)


SdfRTree::SdfRTree(SQLiteDataBase* env, const char* filename, FdoString* database, bool bReadOnly) :
    m_dbname( L"RTREE:", database, true ) 
{
    SQLiteTable* db = new SQLiteTable(env);

    int res;

    m_bReadOnly = true;  // Assume it's readonly untill a save happens
    int readOnlyFlag = bReadOnly ? SQLiteDB_RDONLY : 0;

    //open or create database in the given file with record # access type
	// Note that the nested PhysName call is necessary to reproduce the behavior of the previous versions of the SDF provider.
    if (res = db->open(0, filename, (const char*)PhysName("RTREE:", (const char*)PhysName(L"", database, false), false), (const char*) m_dbname,  readOnlyFlag, 0) != 0)
    {
        //must close even if open failed
        db->close(0);
        delete db;

        if (bReadOnly)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_4_CONNECTION_IS_READONLY)));

        db = new SQLiteTable(env);

        if (res = db->open(0, filename, (const char*)PhysName("RTREE:", (const char*)PhysName(L"", database, false), false), (const char*) m_dbname, SQLiteDB_CREATE, 0) != 0)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
        else
        {
            //create record pointing to root node (bootstrap record), place in record #1
            //The bootstrap record contains a pointer to the root record, whose position
            //may change as things are inserted into the tree
            REC_NO bootnum = 1;
            REC_NO rootnum = 2;

            //create and insert empty root Node
            Node root;
            
            SQLiteData keyboot(&bootnum, sizeof(REC_NO));
            SQLiteData databoot(&rootnum, sizeof(REC_NO));

            SQLiteData keyroot(&rootnum, sizeof(REC_NO));
            SQLiteData dataroot(&root, sizeof(Node));

            if (res = db->put(0, &keyboot, &databoot, SQLiteDB_APPEND) != 0)
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));

            if (res = db->put(0, &keyroot, &dataroot, SQLiteDB_APPEND) != 0)
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
        }
    }

   
    m_db = db;
   
    //get the record number for the root node by reading the boot node
    REC_NO bootRecno = 1;
    SQLiteData keyboot(&bootRecno, sizeof(REC_NO));
    SQLiteData databoot;

    if (res = db->get(0, &keyboot, &databoot, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_19_SPATIAL_INDEX_ERROR)));

    _ASSERT(databoot.get_size() == sizeof(REC_NO));
    //TODO: endian
    m_rootRecno = *(REC_NO*)databoot.get_data();
    m_oldRootRecno = m_rootRecno;
    
    RetrieveNode(m_rootNode, m_rootRecno);

	// This method(SdfRTree::SdfRTree) gets call just opening the FdoIConnection.
	// The following closes any open cursors so inserting features would work. 
	CloseCursor();
}

SdfRTree::~SdfRTree()
{
    if ((m_rootRecno != m_oldRootRecno) && !m_bReadOnly)
    {
        //save root record number into boot record first...
        //get the record number for the root node by reading the boot node
        REC_NO bootRecno = 1;
        SQLiteData keyboot(&bootRecno, sizeof(REC_NO));
        SQLiteData databoot(&m_rootRecno, sizeof(REC_NO));

        if (m_db->put(0, &keyboot, &databoot, 0) != 0)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_19_SPATIAL_INDEX_ERROR)));
    }

    //now close db and free memory
    m_db->close(0);
    delete m_db;
    m_db = NULL;
}


void SdfRTree::RetrieveNode(Node& node, REC_NO rec)
{
    //retrieve child node we are working with
    SQLiteData dbnode;
    SQLiteData key(&rec, sizeof(REC_NO));

    if (m_db->get(0, &key, &dbnode, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_19_SPATIAL_INDEX_ERROR)));
    _ASSERT(dbnode.get_size() == sizeof(Node));
    node = *(Node*)dbnode.get_data();
}


REC_NO SdfRTree::SaveNode(Node& node, REC_NO rec, bool isNew)
{
    SQLiteData dbnode(&node, sizeof(Node));

	m_bReadOnly = false; // No longer assumed as a read only RTree
    if (isNew)
    {
        _ASSERT(rec == 0); //reality check

        //case where we need a new node allocated.
        //new record number will be returned in the empty dbkey
        SQLiteData dbkey;
        if (m_db->put(0, &dbkey, &dbnode, SQLiteDB_APPEND) !=0)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_19_SPATIAL_INDEX_ERROR)));

        return *(REC_NO*)dbkey.get_data(); //endian
    }
    else
    {
        //case where we are updating an already existing node.
        //overwrite existing record
        SQLiteData dbkey(&rec, sizeof(REC_NO));
        if (m_db->put(0, &dbkey, &dbnode, 0) !=0)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_19_SPATIAL_INDEX_ERROR)));

        return rec;
    }
}


void SdfRTree::DeleteNode(REC_NO rec)
{
    SQLiteData dbkey(&rec, sizeof(REC_NO));
    if (m_db->del(0, &dbkey, 0) !=0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_19_SPATIAL_INDEX_ERROR)));

	m_bReadOnly = false; // No longer assumed as a read only RTree
}


int SdfRTree::Insert(Bounds& bounds, int level, SQLiteData& featureKey, REC_NO recnoForNode)
{
    Node* root = &m_rootNode;
    REC_NO* rootRecno = &m_rootRecno;
    
    Node newroot;
    Node newnode;
    Node dummy;
    REC_NO newRecno;
    Branch b;
    int result;

    _ASSERT(level >= 0 && level <= (*root).level);

    if (InsertRect2(bounds, *root, newnode, level, featureKey, recnoForNode) != 0)  //if root split...
    {
        //grow a new root, & tree taller
        
        newroot.level = root->level + 1;
        
        b.bounds = NodeCover(*root);
        b.child = *rootRecno;

        //save changes to newly created node
        //and obtain record number for new node (that was split from current root)
        newRecno = SaveNode(newnode, 0, true);

        //initialize new root
        AddBranch(b, newroot, dummy);
        
        b.bounds = NodeCover(newnode);
        b.child = newRecno;
        AddBranch(b, newroot, dummy);

        //save changes to old root
        SaveNode(*root, *rootRecno, false);
       
        //save new root node and reinit class member pointers
        *root = newroot;

        //obtain record number for new root and add newroot to storage
        *rootRecno = SaveNode(newroot, 0, true);

        result = 1;
    }
    else
    {
        //save changes to old root
        SaveNode(*root, *rootRecno, false);

        result = 0;
    }

    return result;
}


int SdfRTree::InsertRect2(Bounds& bounds, Node& n, Node& newNode, int level, SQLiteData& featureKey, REC_NO recnoForNode)
{
        int i;
        Branch b;
        Node n2;
        REC_NO n2Recno;

        _ASSERT(level >= 0 && level <= n.level);

        //Still above level for insertion, go down tree recursively
        if (n.level > level)
        {
            i = PickBranch(bounds, n);

            //retrieve child node we are working with
            Node child;
            RetrieveNode(child, n.branch[i].child);

            if (InsertRect2(bounds, child, n2, level, featureKey, recnoForNode) == 0)
            {
                // child was not split
                n.branch[i].bounds = Bounds::CombineBounds(bounds, n.branch[i].bounds);

                //now save changes to child...
                SaveNode(child, n.branch[i].child, false);

                return 0;
            }
            else    // child was split
            {
                //now save changes to child...
                SaveNode(child, n.branch[i].child, false);

                //save newly created node
                n2Recno = SaveNode(n2, 0, true);
                
                //update local root node
                n.branch[i].bounds = NodeCover(child);
                b.child = n2Recno;
                b.bounds = NodeCover(n2);
                return AddBranch(b, n, newNode);
            }
            
        }

        // Have reached level for insertion. Add rect, split if necessary
        //
        else if (n.level == level)
        {
            b.bounds = bounds;
            
            //if we are inserting a leaf record (a feature record number)
            if (level == 0)
            {
                _ASSERT(featureKey.get_size() == sizeof(REC_NO));
                //save feature key into database...
                //TODO endian
                REC_NO recnoForKey = *(REC_NO*)featureKey.get_data();

                b.child = recnoForKey; //branch point to feature key 
            }
            else
            {
                //case where we are inserting a node back into the R-Tree
                //reinsertion happens when we delete R-Tree records and
                //it rebalances itself afterwards
                _ASSERT(featureKey.get_size() == sizeof(Node));

                //Note that input node is already saved in the db, we
                //only need to point to it from the current branch, no
                //need to save the Node in featureKey.get_data()

                b.child = recnoForNode;

            }

            // child field of leaves contains tid of data record
            return AddBranch(b, n, newNode);
        }
        else
        {
            // Not supposed to happen 
            _ASSERT (FALSE);
            return 0;
        }

        return 0;
}



// Add a branch to a node.  Split the node if necessary.
// Returns 0 if node not split.  Old node updated.
// Returns 1 if node split, sets *new_node to address of new node.
// Old node updated, becomes one of two.
//
int SdfRTree::AddBranch(Branch& b, Node& n, Node& newNode)
{
    if (n.count < MAXKIDS(n))  // split won't be necessary
    {
        for (int i = 0; i < MAXKIDS(n); i++)  // find empty branch
        {
            if (n.branch[i].child == 0)
            {
                n.branch[i] = b;
                n.count++;
                break;
            }
        }
        return 0;
    }
    else
    {
        //assert(new_node);
        SplitNode(n, b, newNode);
        return 1;
    }
}



// Pick a branch.  Pick the one that will need the smallest increase
// in area to accomodate the new rectangle.  This will result in the
// least total area for the covering rectangles in the current node.
// In case of a tie, pick the one which was smaller before, to get
// the best resolution when searching.
//
int SdfRTree::PickBranch(Bounds& b, Node& n)
{
    Bounds rr;
    int i;
    bool first_time=true;
    double increase, bestIncr=-1, area, bestArea = 0;
    int best = -1;
    Bounds tmp_rect;

    for (i=0; i<MAXKIDS(n); i++)
    {
        if (n.branch[i].child != 0)
        {
            rr = n.branch[i].bounds;
            area = Bounds::SphericalVolume(rr);
            tmp_rect = Bounds::CombineBounds(b, rr);
            increase = Bounds::SphericalVolume(tmp_rect) - area;
            if (increase < bestIncr || first_time)
            {
                best = i;
                bestArea = area;
                bestIncr = increase;
                first_time = false;
            }
            else if (increase == bestIncr && area < bestArea)
            {
                best = i;
                bestArea = area;
                bestIncr = increase;
            }
        }
    }
    return best;
}



// Find the smallest rectangle that includes all rectangles in
// branches of a node.
Bounds SdfRTree::NodeCover(Node& n)
{
    bool first_time = true;
    Bounds bounds;

    for (int i = 0; i < MAXKIDS(n); i++)
    {
        if (n.branch[i].child != 0)
        {
            if (first_time)
            {
                bounds = n.branch[i].bounds;
                first_time = false;
            }
            else
                bounds = Bounds::CombineBounds(bounds, n.branch[i].bounds);
        }
    }

    return bounds;
}


void SdfRTree::Search(Bounds& bounds, SearchHitCallback shcb, void* userData)
{
    // Need to update the root node since it is possible that another connection 
    // has modified the root node. Unless we are the writter connection, in that case we
	// just use the cached root node as we are the only connection that can modify
	// the RTree. The persisted root node is stale as it is not saved yet. It evently get
	// save by the destructor.
	if( m_bReadOnly )
		UpdateRootNode();

    Search(m_rootNode, bounds, shcb, userData);
}


int SdfRTree::Search(Node& node, Bounds& bounds, SearchHitCallback shcb, void* userData)
{
    int hitCount = 0;
    int i;

    _ASSERT(node.level >= 0);

    if (node.level > 0) /* this is an internal node in the tree */
    {
        for (i=0; i<MAXCARD; i++)
        {
            if (node.branch[i].child != 0 &&
                Bounds::Overlap(bounds, node.branch[i].bounds))
            {
                //retrieve child node from SQLiteTable...
                Node child;
                RetrieveNode(child, node.branch[i].child);

                hitCount += Search(child, bounds, shcb, userData);
            }
        }
    }
    else /* this is a leaf node */
    {
        for (i=0; i<MAXCARD; i++)
            if (node.branch[i].child != 0 &&
                Bounds::Overlap(bounds, node.branch[i].bounds))
            {
                hitCount++;

                if(shcb != NULL) // call the user-provided callback
                    if( ! shcb(node.branch[i].child, bounds.Contains(node.branch[i].bounds), userData))
                        return hitCount; // callback wants to terminate search early
            }
    }
    return hitCount;

}


//TODO: use quadratic split instead
//linear time split
/*-----------------------------------------------------------------------------
| Split a node.
| Divides the nodes branches and the extra one between two nodes.
| Old node is one of the new ones, and one really new one is created.
-----------------------------------------------------------------------------*/
void SdfRTree::SplitNode(Node& n, Branch& b, Node& newNode)
{
    int level;
    double area;

    // load all the branches into a buffer, clear old node
    level = n.level;
    GetBranches(n, b);

    /* find partition */

    /* Note: can't use MINFILL(n) below since n was cleared by GetBranches() */
    MethodZero(Partitions, level>0 ? MAXCARD / 2 : MAXCARD / 2);

    /* record how good the split was for statistics */
    area = Partitions.area[0] + Partitions.area[1];

    /* put branches from buffer in 2 nodes according to chosen partition */
    newNode.level = n.level = level;
    LoadNodes(n, newNode, Partitions);
    _ASSERT(n.count + newNode.count == NODECARD+1);
}



/*-----------------------------------------------------------------------------
| Load branch buffer with branches from full node plus the extra branch.
-----------------------------------------------------------------------------*/
void SdfRTree::GetBranches(Node& n, Branch& b)
{
    // load the branch buffer
    for (int i=0; i<MAXKIDS(n); i++)
    {
        _ASSERT(n.branch[i].child != 0);  //every entry should be full
        BranchBuf[i] = n.branch[i];
    }
    BranchBuf[MAXKIDS(n)] = b;
    BranchCount = MAXKIDS(n) + 1;

    // calculate rect containing all in the set
    CoverSplit = BranchBuf[0].bounds;
    for (int i=1; i<MAXKIDS(n)+1; i++)
    {
        CoverSplit = Bounds::CombineBounds(CoverSplit, BranchBuf[i].bounds);
    }

    //initialize n by copying an empty Node into its contents
    Node tmp;
    n = tmp;
    n.level = -1;
}


/*-----------------------------------------------------------------------------
| Method 0 for finding a partition:
| First find two seeds, one for each group, well separated.
| Then put other rects in whichever group will be smallest after addition.
-----------------------------------------------------------------------------*/
void SdfRTree::MethodZero(PartitionVars& p, int minfill)
{
    InitPVars(p, BranchCount, minfill);
    PickSeeds(p);
    Pigeonhole(p);
}


/*-----------------------------------------------------------------------------
| Initialize a PartitionVars structure.
-----------------------------------------------------------------------------*/
void SdfRTree::InitPVars(PartitionVars& p, int maxrects, int minfill)
{            
    //assert(p);

    p.count[0] = p.count[1] = 0;
    p.total = maxrects;
    p.minfill = minfill;
    for (int i=0; i<maxrects; i++)
    {
        p.taken[i] = 0;
        p.partition[i] = -1;
    }
}



/*-----------------------------------------------------------------------------
| Pick two rects from set to be the first elements of the two groups.
| Pick the two that are separated most along any dimension, or overlap least.
| Distance for separation or overlap is measured modulo the width of the
| space covered by the entire set along that dimension.
-----------------------------------------------------------------------------*/
void SdfRTree::PickSeeds(PartitionVars& p)
{
    Bounds r, rlowx, rhighx, rlowy, rhighy;
    double w, h, separationX, separationY;
    double width, height;
    int leastUpperX, leastUpperY;
    int greatestLowerX, greatestLowerY;
    int seed[2];

    /* find the rectangles farthest out in each direction
            * along this dimens */
    greatestLowerX = greatestLowerY = leastUpperX = leastUpperY = 0;

    for (int i=1; i<MAXCARD+1; i++)
    {
        r = BranchBuf[i].bounds;
        if (r.minx > BranchBuf[greatestLowerX].bounds.minx)
            greatestLowerX = i;

        if (r.maxx < BranchBuf[leastUpperX].bounds.maxx)
            leastUpperX = i;

        if (r.miny > BranchBuf[greatestLowerY].bounds.miny)
            greatestLowerY = i;

        if (r.maxy < BranchBuf[leastUpperY].bounds.maxy)
            leastUpperY = i;
    }

    /* find width of the whole collection along this dimension */
    width = CoverSplit.maxx - CoverSplit.minx;
    height = CoverSplit.maxy - CoverSplit.miny;

    /* pick the best separation dimension and the two seed rects */
    /* divisor for normalizing by width */
    if (width == 0)
        w = 1.0;
    else
        w = width;

    if (height == 0)
        h = 1.0;
    else
        h = height;

    rlowx = BranchBuf[leastUpperX].bounds;
    rhighx = BranchBuf[greatestLowerX].bounds;
    rlowy = BranchBuf[leastUpperY].bounds;
    rhighy = BranchBuf[greatestLowerY].bounds;

    seed[0] = leastUpperX;
    seed[1] = greatestLowerX;
    separationX = (rhighx.minx - rlowx.maxx) / w;

    separationY = (rhighy.miny - rlowy.maxy) / h;
    if (separationY > separationX)
    {
        seed[0] = leastUpperY;
        seed[1] = greatestLowerY;
    }

    if (seed[0] != seed[1])
    {
        Classify(seed[0], 0, p);
        Classify(seed[1], 1, p);
    }
}



/*-----------------------------------------------------------------------------
| Put a branch in one of the groups.
-----------------------------------------------------------------------------*/
void SdfRTree::Classify(int i, int group, PartitionVars& p)
{
    //assert(p);
    //assert(!p->taken[i]);

    p.partition[i] = group;
    p.taken[i] = 1;

    if (group == 0)
    {
        if (p.count[0] == 0)
            p.cover[0] = BranchBuf[i].bounds;
        else
            p.cover[0] = Bounds::CombineBounds(BranchBuf[i].bounds, p.cover[0]);
    
        p.area[0] = Bounds::SphericalVolume(p.cover[0]);
        p.count[0]++;
    }
    else
    {
        if (p.count[1] == 0)
            p.cover[1] = BranchBuf[i].bounds;
        else
            p.cover[1] = Bounds::CombineBounds(BranchBuf[i].bounds, p.cover[1]);
    
        p.area[1] = Bounds::SphericalVolume(p.cover[1]);
        p.count[1]++;
    }
}



/*-----------------------------------------------------------------------------
| Copy branches from the buffer into two nodes according to the partition.
-----------------------------------------------------------------------------*/
void SdfRTree::LoadNodes(Node& n, Node& q, PartitionVars& p)
{
    Node dummy;

    for (int i=0; i<MAXCARD+1; i++)
    {
        if (p.partition[i] == 0)
            AddBranch(BranchBuf[i], n, dummy);
        else if (p.partition[i] == 1)
            AddBranch(BranchBuf[i], q, dummy);
        else
            _ASSERT(FALSE);
    }
}



/*-----------------------------------------------------------------------------
| Put each rect that is not already in a group into a group.
| Process one rect at a time, using the following hierarchy of criteria.
| In case of a tie, go to the next test.
| 1) If one group already has the max number of elements that will allow
| the minimum fill for the other group, put r in the other.
| 2) Put r in the group whose cover will expand less.  This automatically
| takes care of the case where one group cover contains r.
| 3) Put r in the group whose cover will be smaller.  This takes care of the
| case where r is contained in both covers.
| 4) Put r in the group with fewer elements.
| 5) Put in group 1 (arbitrary).
|
| Also update the covers for both groups.
-----------------------------------------------------------------------------*/
void SdfRTree::Pigeonhole(PartitionVars& p)
{
    Bounds newCover[2];
    double newArea[2], increase[2];

    for (int i=0; i<MAXCARD+1; i++)
    {
        if (p.taken[i] == 0)
        {
            /* if one group too full, put rect in the other */
            if (p.count[0] >= p.total - p.minfill)
            {
                Classify(i, 1, p);
                continue;
            }
            else if (p.count[1] >= p.total - p.minfill)
            {
                Classify(i, 0, p);
                continue;
            }

            /* find areas of the two groups' old and new covers */

            if (p.count[0]>0)
                newCover[0] = Bounds::CombineBounds(BranchBuf[i].bounds, p.cover[0]);
            else
                newCover[0] = BranchBuf[i].bounds;
            newArea[0] = Bounds::SphericalVolume(newCover[0]);
            increase[0] = newArea[0]- p.area[0];

            if (p.count[1]>0)
                newCover[1] = Bounds::CombineBounds(BranchBuf[i].bounds, p.cover[1]);
            else
                newCover[1] = BranchBuf[i].bounds;
            newArea[1] = Bounds::SphericalVolume(newCover[1]);
            increase[1] = newArea[1] - p.area[1];
            
            
            /* put rect in group whose cover will expand less */
            if (increase[0] < increase[1])
                Classify(i, 0, p);
            else if (increase[1] < increase[0])
                Classify(i, 1, p);

                /* put rect in group that will have a smaller cover */
            else if (p.area[0] < p.area[1])
                Classify(i, 0, p);
            else if (p.area[1] < p.area[0])
                Classify(i, 1, p);

                /* put rect in group with fewer elements */
            else if (p.count[0] < p.count[1])
                Classify(i, 0, p);
            else
                Classify(i, 1, p);
        }
    }
    _ASSERT(p.count[0] + p.count[1] == NODECARD + 1);
}


// Delete a data rectangle from an index structure.
// Pass in a pointer to a Rect, the tid of the record, ptr to ptr to root node.
// Returns 1 if record not found, 0 if success.
// RTreeDeleteRect provides for eliminating the root.
int SdfRTree::Delete(Bounds& bounds, SQLiteData& featureKey)
{
    Node* rootNode = &m_rootNode;
    REC_NO* rootRecno = &m_rootRecno;

    Node tmpNode;
    REC_NO tmpRecno = 0;

    ListNode* reInsertList = NULL;
    ListNode* e;

    if (DeleteRect2(bounds, featureKey, *rootNode, &reInsertList) == 0)
    {
        // found and deleted a data item

        //save root, one of its children may have been modified
        //in case of branch being reinserted
        SaveNode(*rootNode, *rootRecno, false);

        // reinsert any branches from eliminated nodes
        while (reInsertList != NULL)
        {
            tmpNode = reInsertList->node;

            //go over branches and reinsert non-empty ones
            for (int i = 0; i < MAXKIDS(tmpNode); i++)
            {
                if (tmpNode.branch[i].child != 0)
                {
                    if (tmpNode.level == 0)
                    {
                        //if leaf node, children point to 
                        //feature records -> insert as feature record number
                        SQLiteData nodeData(&tmpNode.branch[i].child, sizeof(REC_NO));
                        Insert(tmpNode.branch[i].bounds, 0, nodeData);
                    }
                    else
                    {
                        //if a non-leaf branch, reinsert as node.
                        //We reuse the old databse record for the child nodes
                        //so we pass on the record number as the optional
                        //argument to Insert()
                        Node branch;
                        RetrieveNode(branch, tmpNode.branch[i].child);
                        SQLiteData nodeData(&branch, sizeof(Node));
                        Insert(tmpNode.branch[i].bounds, tmpNode.level, nodeData, tmpNode.branch[i].child);
                    }
                }
            }

            e = reInsertList;
            reInsertList = reInsertList->next;

            delete e;
        }

        // check for redundant root (not leaf, 1 child) and eliminate
        if (rootNode->count == 1 && rootNode->level > 0)
        {
            for (int i = 0; i < MAXCARD; i++)
            {
                tmpRecno = rootNode->branch[i].child;
                if(tmpRecno != 0)
                    break;
            }

            RetrieveNode(tmpNode, tmpRecno);

            //delete redundant root from DB
            DeleteNode(*rootRecno);

            //set new root
            *rootNode = tmpNode;
            *rootRecno = tmpRecno;
        }
        return 0;
    }
    else
    {
        return 1;
    }
}


// Delete a rectangle from non-root part of an index structure.
// Called by Delete().  Descends tree recursively,
// merges branches on the way back up.
// Returns 1 if record not found, 0 if success.
//
int SdfRTree::DeleteRect2(Bounds& r, SQLiteData& featureKey, Node& n, ListNode** ee)
{
    //assert(r && n && ee);
    //assert(tid >= 0);
    //assert(n->level >= 0);

    if (n.level > 0)  // not a leaf node
    {
        for (int i = 0; i < MAXCARD; i++)
        {
            if (n.branch[i].child != 0 && Bounds::Overlap(r, n.branch[i].bounds))
            {
                Node child;
                RetrieveNode(child, n.branch[i].child);

                if (DeleteRect2(r, featureKey, child, ee) == 0)
                {
                    SaveNode(child, n.branch[i].child, false);

                    if (child.count >= MAXCARD / 2)
                        n.branch[i].bounds = NodeCover(child);
                    else
                    {
                        // not enough entries in child,
                        // eliminate child node
                        //
                        ReInsert(child, ee);
                        DisconnectBranch(n, i);
                    }
                    return 0;
                }
            }
        }
        return 1;
    }
    else  // a leaf node
    {
        for (int i = 0; i < MAXCARD; i++)
        {
            if (n.branch[i].child != 0)
            {
                //for leaf nodes, just kill the value of the feature record number. 
                //there is no R-Tree record to delete in this case
                //TODO: REC_NO endian
                if (n.branch[i].child == *(REC_NO*)featureKey.get_data())
                {
                    Branch empty;
                    n.branch[i] = empty;
                    n.count--;

                    return 0;
                }

                //old code...
                //SQLiteData data;
                //SQLiteData keyToData(&n.branch[i].child, sizeof(REC_NO));
                //if (m_db->get(0, &keyToData, &data, 0) != 0)
                //    throw FdoException::Create(L"Could not retrieve RTREE leaf node");

                //if (memcmp(featureKey.get_data(), data.get_data(), featureKey.get_size()) == 0)
                //{
                //    DisconnectBranch(n, i);
                //    return 0;
                //}
            }
        }
        return 1;
    }
}

// Disconnect a dependent node.
void SdfRTree::DisconnectBranch(Node& n, int i)
{
    _ASSERT(i>=0 && i<MAXKIDS(n));
    _ASSERT(n.branch[i].child != 0);

    DeleteNode(n.branch[i].child);

    Branch empty;
    n.branch[i] = empty;
    n.count--;
}


// Add a node to the reinsertion list.  All its branches will later
// be reinserted into the index structure.
void SdfRTree::ReInsert(Node& n, ListNode** ee)
{
    ListNode* l = new ListNode();

    l->node = n;
    l->next = *ee;
    *ee = l;
}


//Returns the overall data bounds of the RTree
//Just read it off the root record and that's it!
Bounds SdfRTree::GetBounds()
{
    Bounds ret;

    for (int i=0; i<m_rootNode.count; i++)
    {
        Bounds b1 = m_rootNode.branch[i].bounds;

        if (!Bounds::IsUndefined(b1))
        {
            if (Bounds::IsUndefined(ret))
                ret = b1;
            else
            {
                Bounds b2 = Bounds::CombineBounds(ret, b1);
                ret = b2;
            }
        }
    }

    return ret;
}


void SdfRTree::CloseCursor()
{
	m_db->close_cursor();
}

void SdfRTree::UpdateRootNode()
{
    //get the record number for the root node by reading the boot node
    REC_NO bootRecno = 1;
    SQLiteData keyboot(&bootRecno, sizeof(REC_NO));
    SQLiteData databoot;
    int res;

    if (res = this->m_db->get(0, &keyboot, &databoot, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_19_SPATIAL_INDEX_ERROR)));

    _ASSERT(databoot.get_size() == sizeof(REC_NO));
    //TODO: endian
    m_rootRecno = *(REC_NO*)databoot.get_data();
    m_oldRootRecno = m_rootRecno;
    
    RetrieveNode(m_rootNode, m_rootRecno);
}

void SdfRTree::Flush() 
{ 
	if ((m_rootRecno != m_oldRootRecno) && !m_bReadOnly)
    {
        //save root record number into boot record first...
        //get the record number for the root node by reading the boot node
        REC_NO bootRecno = 1;
        SQLiteData keyboot(&bootRecno, sizeof(REC_NO));
        SQLiteData databoot(&m_rootRecno, sizeof(REC_NO));

        if (m_db->put(0, &keyboot, &databoot, 0) != 0)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_19_SPATIAL_INDEX_ERROR)));

		m_oldRootRecno = m_rootRecno;
    }
	m_db->flush(); 
	m_bReadOnly = true; // re-start fresh again as all the RTree is flushed
}

void SdfRTree::Drop()
{
    if ( m_db->Drop() != 0 ) {
        throw FdoException::Create(
            NlsMsgGetMain(
                FDO_NLSID(SDFPROVIDER_81_DROP_TABLE),
                L"RTree",
                (FdoString*) m_dbname
            )
        );
    }
}
