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
#ifndef RTREE_H
#define RTREE_H

#include "RTreeInfrastructure.h"

//search callback function prototype
typedef bool (*SearchHitCallback)(REC_NO featureKey, bool contained, void* arg);

//
// R-Tree class. Encapsulates an R-Tree which uses a SQLiteTable as backing store.
// Based on the original R-Tree implementation by A. Guttman
// See Guttman, A. 1984: "R-Trees : A Dynamic Index Structure for Spatial Searching"
// Most of the R-Tree logic follows Guttman's code very closely, except it is hardcoded
// for 2D bounding boxes and database storage code has been inserted
//
class SdfRTree
{
public:

    SdfRTree(SQLiteDataBase* env, const char* filename, const char* database, bool bReadOnly); 

    ~SdfRTree();

    int Insert(Bounds& bounds, int level, SQLiteData& key, REC_NO recnoForNode = 0);
    int Delete(Bounds& bounds, SQLiteData& key);
    void Search(Bounds& bounds, SearchHitCallback shcb, void* userData);

    Bounds GetBounds();
    
	void CloseCursor();

	bool NeedsAFlush(bool partial=false) { return m_db->needs_a_flush(partial); }

	void Flush();

private:

    //database access helper functions
    void RetrieveNode(Node& node, REC_NO rec);
    REC_NO SaveNode(Node& node, REC_NO rec, bool isNew);
    void DeleteNode(REC_NO rec);

    //recursive traversal functions for insert/delete/search
    int InsertRect2(Bounds& bounds, Node& n, Node& newNode, int level, SQLiteData& featureKey, REC_NO recnoForNode);
    int DeleteRect2(Bounds& r, SQLiteData& featureKey, Node& n, ListNode** ee);
    int Search(Node& node, Bounds& bounds, SearchHitCallback shcb, void* userData);


    void UpdateRootNode();

    //RTree maintenance logic
    /*static*/ int AddBranch(Branch& branch, Node& node, Node& newNode);
    /*static*/ void SplitNode(Node& n, Branch& b, Node& newNode);
    /*static*/ Bounds NodeCover(Node& n);
    /*static*/ int PickBranch(Bounds& b, Node& n);
    /*static*/ void GetBranches(Node& n, Branch& b);
    /*static*/ void MethodZero(PartitionVars& p, int minfill);
    /*static*/ void InitPVars(PartitionVars& p, int maxrects, int minfill);
    /*static*/ void PickSeeds(PartitionVars& p);
    /*static*/ void Classify(int i, int group, PartitionVars& p);
    /*static*/ void LoadNodes(Node& n, Node& q, PartitionVars& p);
    /*static*/ void Pigeonhole(PartitionVars& p);
    /*static*/ void ReInsert(Node& n, ListNode** ee);

    void DisconnectBranch(Node& n, int i);

    // root node and database pointers
    SQLiteTable* m_db;
    Node m_rootNode;
    REC_NO m_rootRecno;

    // insertion/partitioning related state
    PartitionVars Partitions;
    Branch BranchBuf[MAXCARD + 1];
    int BranchCount;
    Bounds CoverSplit;

    bool m_bReadOnly;
    REC_NO m_oldRootRecno;
};


#endif //RTREE_H

