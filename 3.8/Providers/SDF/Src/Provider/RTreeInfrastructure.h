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
#define M_PI       3.14159265358979323846

#ifndef sdf_max
#define sdf_max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef sdf_min
#define sdf_min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


//page size for the R-Tree file
//#define RTREE_PAGE_SIZE 2048
//Need the same page size for all databases if we put everything in
//one big file, so use 65536
#define RTREE_PAGE_SIZE 65536

//Determine the branching factor of the R-Tree.
//We make it so that one Node fits on one database page and fills
//it as fully as possible. 

#define MAXCARD 40//(int)((RTREE_PAGE_SIZE-2*sizeof(int)-32) / sizeof(Branch))

#define PADDING 0 //12648430 //padding used for 8-byte alignment 


// RTree bounding box representation and logic
// This struct provides a default constructor so that
// Bounds are always initialized to an invalid/empty state
struct Bounds
{
public:

    double minx;
    double miny;
    double maxx;
    double maxy;

    Bounds()
    {
        //undefined rectangle -- arbitrarily define as maxx is smaller than minx
        minx = 0.0;
        maxx = -1.0;
        miny = 0.0;
        maxy = -1.0;
    }

    Bounds(double dminx, double dminy, double dmaxx, double dmaxy)
    {
        _ASSERT(dminx <= dmaxx);
        _ASSERT(dminy <= dmaxy);

        minx = dminx;
        miny = dminy;
        maxx = dmaxx;
        maxy = dmaxy;
    }


    //Adds two bounding boxes and returns the result
    static Bounds CombineBounds(Bounds& r1, Bounds& r2)
    {
        if (IsUndefined(r1))
            return r1;

        if (IsUndefined(r2))
            return r2;

        Bounds ret(
            sdf_min(r1.minx, r2.minx),
            sdf_min(r1.miny, r2.miny),
            sdf_max(r1.maxx, r2.maxx),
            sdf_max(r1.maxy, r2.maxy));

        return ret;
    }

    //returns true if the Bounds is undefined according to our 
    //arbitrary condition
    static bool IsUndefined(Bounds& r1)
    {
        return (r1.minx > r1.maxx);
    }

    
    // The exact volume of the bounding sphere for the given Rect.
    // For 2D it's just the area of a circle with radius computed 
    // from the width and height
    static double SphericalVolume(Bounds& r)
    {
        double sum1 = (r.maxx - r.minx);
        sum1 *= sum1;

        double sum2 = (r.maxy - r.miny);
        sum2 *= sum2;

        //sum1 + sum2 is the square of the bounding sphere's radius
        //luckily, for 2D we need the square, so we save a sqrt() call!
        //How cool is that?!
        return M_PI * (sum1 + sum2); //= pi * r ^ 2
    }

    /*-----------------------------------------------------------------------------
    | Decide whether two rectangles overlap.
    -----------------------------------------------------------------------------*/
    static bool Overlap(Bounds& r, Bounds& s)
    {
        return !(  r.minx > s.maxx 
                || r.miny > s.maxy 
                || s.minx > r.maxx 
                || s.miny > r.maxy);
    }

    bool Contains(Bounds& r)
    {
        return ( (minx < r.minx) 
              && (maxx > r.maxx) 
              && (miny < r.miny) 
              && (maxy > r.maxy));
    }

   
};


//RTree branch. It holds on to the record number of its child Node (or data) and the 
//bounds of the child.
struct Branch
{
    Bounds bounds;

    //DB record# of child
    REC_NO child;   
    
    //force 8 byte alignment of Rtree structures
    //MSVC was doing it by default, g++ was not
    //DO NOT RELY ON THE COMPILER FOR THIS!
    int dummy; 

    Branch()
    {
        child = 0;
        dummy = PADDING;
    }
};


//RTree Node. It holds on to an array of branches, which point to child Node records
struct Node
{
    //number of valid Branches
    int count;

    //height of Node in the tree. 0 is for leaf node.
    int level;

    //Branch array
    Branch branch[MAXCARD];

    Node()
    {
        count = 0;
        level = 0;
    }

    bool operator!=( const Node& other ) 
    {
        return ( memcmp((const void*) this, (const void*) &other, sizeof(Node)) != 0 );
    }
};


//Linked list node. Used when deleting nodes -- we build up a list of Nodes that
//need to be reinserted into the tree to maintain balance.
struct ListNode
{
    ListNode* next;
    Node node;

    ListNode()
    {
        next = NULL;
    }
};


// variables for finding a partition 
struct PartitionVars
{
    int partition[MAXCARD+1];
    int total, minfill;
    int taken[MAXCARD+1];
    int count[2];
    Bounds cover[2];
    double area[2];
};






