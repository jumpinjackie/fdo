// 
//  
//  Copyright (C) 2011 Autodesk Inc.
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

#ifdef _MSC_VER
  #define ALGNW __declspec(align(16))
  #define ALGNL
  #include <emmintrin.h>
#else
  #define ALGNW
  #define ALGNL __attribute__((aligned(16)))

  #if defined(__ICC)
      #include <emmintrin.h>
  #else
      #include <xmmintrin.h>
  #endif

  #define _aligned_free free

  void* _aligned_malloc(size_t size, size_t alignment)
  {
    void* ret = 0;
    int res = posix_memalign(&ret, alignment, size);
    return ret;
  }
#endif

//The type to use for feature IDs. Must be a signed type!
typedef __int64 fid_t;

//The type to use for node IDs. Generally should match fid_t.
typedef fid_t id_t;

//Decision whether to use out-of-core file backed node storage
#ifdef _WIN32
    #ifdef _M_X64
        #define RTREE_OUTOFCORE 0
    #else
        #define RTREE_OUTOFCORE 1
    #endif
#else
    #define RTREE_OUTOFCORE 0
#endif

static const size_t OUT_OF_CORE_THRESHOLD = 16*1024*1024; //bytes

//R-Tree branching factor -- MUST be a multiple of 4!
#define MAX_BRANCH 16


#define NULL_ID ((id_t)0)

//Should be enough for a tree depth to hold the maximum expected number of items
//at the specified branching factor. So max_depth = log_base_branch_factor (num_items)
#if MAX_BRANCH == 4
#define MAX_DEPTH 16
#elif MAX_BRANCH == 8
#define MAX_DEPTH  11
#elif MAX_BRANCH == 16
#define MAX_DEPTH  8
#elif MAX_BRANCH == 32
#define MAX_DEPTH  7
#else
#error "MAX_BRANCH is not what I expect -- please define MAX_DEPTH here, or fix MAX_BRANCH"
#endif

#define CACHE_LINE 64

namespace bvh
{

/*
static int flt_plus_inf = 0x7f800000; //+Infinity
static int flt_minus_inf = 0xff800000; //-Infinity
*/
static int ALGNW plus_inf[] ALGNL = { 0x7f800000,  0x7f800000,  0x7f800000,  0x7f800000 } ;
static int ALGNW minus_inf[] ALGNL = { 0xff800000, 0xff800000, 0xff800000, 0xff800000 } ;
static int ALGNW make_positive[] ALGNL = { 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff } ;

static inline void minss(float* a, const float* b)
{
    __m128 v1 = _mm_load_ss(a);
    __m128 v2 = _mm_load_ss(b);
    _mm_store_ss(a, _mm_min_ss(v1, v2));
}

static inline void maxss(float* a, const float* b)
{
    __m128 v1 = _mm_load_ss(a);
    __m128 v2 = _mm_load_ss(b);
    _mm_store_ss(a, _mm_max_ss(v1, v2));
}


//A single 2D bounding box
ALGNW struct box
{
    union
    {
        struct
        {
            float minx;
            float miny;
            float maxx;
            float maxy;
        };
        float v[4];
        __m128 xmm;
    };

    box()
    {
    }

    box(int dummy_to_make_empty)
    {
        make_empty();
    }

    void make_empty()
    {
        minx = miny = *(float*)plus_inf;
        maxx = maxy = *(float*)minus_inf;
    }

    float area() const
    {
        return (maxx - minx) * (maxy - miny);
    }

    inline box& operator=(const box& right)
    {
        xmm = right.xmm;
        return *this;
    }

    void add(const box& b)
    {
        __m128 mins = _mm_min_ps(xmm, b.xmm);
        __m128 maxs = _mm_max_ps(xmm, b.xmm);
        xmm = _mm_shuffle_ps(mins, maxs, _MM_SHUFFLE(3, 2, 1, 0));
    }

    inline operator __m128() { return xmm; }
    inline operator const __m128() const { return xmm; }

} ALGNL;

//A vector of 4 elements -- useful for casting to/from SSE structures
ALGNW struct vec
{
    union
    {
        float v[4];
        __m128 xmm;
    };

    inline operator __m128() { return xmm; }
    inline operator const __m128() const { return xmm; }

    inline vec& operator=(const vec& right)
    {
        xmm = right.xmm;
        return *this;
    }

    inline vec& operator=(const __m128& right)
    {
        xmm = right;
        return *this;
    }

    //returns the minimum of the 4 values
    inline float hmin() const
    {
        float r = v[0];
        minss(&r, &v[1]);
        minss(&r, &v[2]);
        minss(&r, &v[3]);
        return r;
    }

    //returns the maximim of the 4 values
    inline float hmax() const
    {
        float r = v[0];
        maxss(&r, &v[1]);
        maxss(&r, &v[2]);
        maxss(&r, &v[3]);
        return r;
    }


} ALGNL;


//A vector of 4 elements -- useful for casting to/from SSE structures
ALGNW struct veci
{
    union
    {
        int v[4];
        __m128i xmm;
    };

    inline operator __m128i() { return xmm; }
    inline operator const __m128i() const { return xmm; }

    inline veci& operator=(const veci& right)
    {
        xmm = right.xmm;
        return *this;
    }

    inline veci& operator=(const __m128i& right)
    {
        xmm = right;
        return *this;
    }
} ALGNL;


//A pack of 4 bounding boxes in Structure-of-Arrays form
ALGNW struct box4_soa
{
    vec minx;
    vec miny;
    vec maxx;
    vec maxy;

    box4_soa()
    {
    }

    box4_soa(int dummy_to_init_empty)
    {
        make_empty();
    }

    box4_soa(const box& b)
    {
        make_wide_box(b);
    }

    void make_empty()
    {
        minx.xmm = miny.xmm = *(__m128*)plus_inf;
        maxx.xmm = maxy.xmm = *(__m128*)minus_inf;
    }

    void copy(const box4_soa& bin)
    {
        minx.xmm = bin.minx.xmm;
        miny.xmm = bin.miny.xmm;
        maxx.xmm = bin.maxx.xmm;
        maxy.xmm = bin.maxy.xmm;
    }


    //initializes all 4 boxes of the structure to the
    //contents of the given box
    void make_wide_box(const box& b)
    {
        minx.xmm = _mm_shuffle_ps(b, b, 0x00);
        miny.xmm = _mm_shuffle_ps(b, b, 0x55);
        maxx.xmm = _mm_shuffle_ps(b, b, 0xaa);
        maxy.xmm = _mm_shuffle_ps(b, b, 0xff);
    }

    //returns bitmasks containing intersection/containment flags for each
    //of the 4 boxes with the given input box
    //mask[0] has bits set if the boxes are disjoint (do not intersect at all),
    //mask[1] has bits set if the input box contains this box.
    inline void overlap_mask(char* ret, const box4_soa& b) const
    {
        //do the intersection comparisons
        __m128 cmp_maxx = _mm_cmpgt_ps(b.minx, maxx);
        __m128 cmp_maxy = _mm_cmpgt_ps(b.miny, maxy);
        __m128 cmp_minx = _mm_cmplt_ps(b.maxx, minx);
        __m128 cmp_miny = _mm_cmplt_ps(b.maxy, miny);

        //OR the comparison results together to get the disjoint
        //mask -- a 1 bit will indicate no overlap!!!
        __m128 disjoint = _mm_or_ps(_mm_or_ps(cmp_minx, cmp_miny), 
                                    _mm_or_ps(cmp_maxx, cmp_maxy));

        ret[0] = (char)_mm_movemask_ps(disjoint); 

        //do the containment comparisons
        cmp_minx = _mm_cmple_ps(b.minx, minx);
        cmp_miny = _mm_cmple_ps(b.miny, miny);
        cmp_maxx = _mm_cmpge_ps(b.maxx, maxx);
        cmp_maxy = _mm_cmpge_ps(b.maxy, maxy);

        //AND the comparisons to get the containment mask --
        //a 1 bit indicates containment.
        __m128 contains = _mm_and_ps(_mm_and_ps(cmp_minx, cmp_miny), 
                                     _mm_and_ps(cmp_maxx, cmp_maxy));

        ret[1] = (char)_mm_movemask_ps(contains);
    }

    //sets one of the 4 boxes in the pack
    //to the given bounds
    void set(unsigned char i, const box& b)
    {
        minx.v[i] = b.minx;
        miny.v[i] = b.miny;
        maxx.v[i] = b.maxx;
        maxy.v[i] = b.maxy;
    }

    //Adds the given bounds to one of the
    //boxes in the pack
    void add(unsigned char i, const box& b)
    {
        minss(&minx.v[i], &b.minx);
        minss(&miny.v[i], &b.miny);

        maxss(&maxx.v[i], &b.maxx);
        maxss(&maxy.v[i], &b.maxy);
    }

    //Returns the total bounds of the 4 boxes
    void hsum(box& b) const
    {
        b.minx = minx.hmin();
        b.miny = miny.hmin();
        b.maxx = maxx.hmax();
        b.maxy = maxy.hmax();
    }

    //Extracts a box with the given index from the pack
    void extract(unsigned char i, box& b) const
    {
        b.minx = minx.v[i];
        b.miny = miny.v[i];
        b.maxx = maxx.v[i];
        b.maxy = maxy.v[i];
    }
};


static inline id_t fid_to_id(fid_t fid)
{
    return (id_t)fid | ((id_t)1<<(8*sizeof(id_t)-1)); //set the sign bit to indicate leaf node
}

static inline fid_t id_to_fid(id_t id)
{
    return id & ~((id_t)1<<(8*sizeof(id_t)-1)); //unset the sign bit to get back the fid
}

static inline bool is_leaf(id_t i)
{
    return (i < 0); //sign bit is set for leaf nodes
}

//A node with branching factor of 4 -- slightly faster than 
//the generic node implementation
ALGNW struct node4
{
    id_t children[4];
    box4_soa child_bounds;

    static const int max_branch = 4;

    inline bool is_leaf() const
    {
        return bvh::is_leaf(children[0]);
    }

    inline void set_branch(int i, id_t id, const box& b)
    {
        child_bounds.set(i, b);
        children[i] = id;
    }

    //returns the index of the first empty spot
    //in the array of child branches. Returns
    //-1 to indicate full node
    inline int find_empty_spot() const
    {
        for (int i=0; i<max_branch; i++)
            if (children[i] == NULL_ID)
                return i;

        return -1;
    }

    inline void summed_bbox(box& b) const
    {
        child_bounds.hsum(b);
    }

    inline void child_bbox(int i, box& b) const
    {
        child_bounds.extract(i, b);
    }

    inline void set_child_bbox(int i, const box& b) 
    {
        child_bounds.set(i, b);
    }

    inline void add_bbox(int i, const box& b)
    {
        child_bounds.add(i, b);
    }

    inline void set_all_boxes(const box4_soa& val)
    {
        child_bounds.copy(val);
    }

    inline void overlap_mask(char* ret, const box4_soa& b) const
    {
        return child_bounds.overlap_mask(ret, b);
    }

    int pick_child(const box4_soa& bnew) const;

} ALGNL;


//A generic implementation of a node, containing
//a multiple of 4 branches
ALGNW struct node_generic_mul4
{
    id_t children[MAX_BRANCH];
    box4_soa child_bounds[MAX_BRANCH/4];

    inline bool is_leaf() const
    {
        return bvh::is_leaf(children[0]);
    }

    inline void set_branch(int i, id_t id, const box& b)
    {
        child_bounds[i/4].set(i%4, b);
        children[i] = id;
    }

    //returns the index of the first empty spot
    //in the array of child branches. Since there
    //are no empty nodes, we return -1 to indicate
    //full node
    inline int find_empty_spot() const
    {
        for (int i=0; i<MAX_BRANCH; i++)
            if (children[i] == NULL_ID)
                return i;

        return -1;
    }

    inline void summed_bbox(box& b) const
    {
        box tmp;
        child_bounds[0].hsum(b);
        for (int i=1; i<MAX_BRANCH/4; i++)
        {
            child_bounds[i].hsum(tmp);
            b.add(tmp);
        }
    }

    inline void child_bbox(unsigned char i, box& b) const
    {
        child_bounds[i/4].extract(i%4, b);
    }

    inline void set_child_bbox(unsigned char i, const box& b) 
    {
        child_bounds[i/4].set(i%4, b);
    }

    inline void add_bbox(unsigned char i, const box& b)
    {
        child_bounds[i/4].add(i%4, b);
    }

    void set_all_boxes(const box4_soa& val)
    {
        for (int i=0; i<MAX_BRANCH/4; i++)
        {
            child_bounds[i].copy(val);
        }
    }

    inline void overlap_mask(char* ret, const box4_soa& b) const
    {
        for (int i=0; i<MAX_BRANCH/4; i++)
        {
            child_bounds[i].overlap_mask(ret, b);
            ret += 2;
        }
    }

    int pick_child(const box4_soa& bnew) const;

} ALGNL;

struct erase_data;

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//                                R-Tree API
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

//Pick which node implementation to use based on branching factor
#if (MAX_BRANCH == 4)
    typedef node4 node;
#else
    typedef node_generic_mul4 node;
#endif

class node_mem_mgr;
class node_mf_mgr;

//Pick node memory manager to use based on in-memory or out-of-core option.
#if RTREE_OUTOFCORE
typedef node_mf_mgr node_mgr;
#else
typedef node_mem_mgr node_mgr;
#endif


//A double precision bounding box, used in the rtree API calls
struct dbox
{
    union
    {
        struct
        {
            double minx;
            double miny;
            double maxx;
            double maxy;
        };
        struct
        {
            double min[2];
            double max[2];
        };
        double v[4];
    };
};


// Classic R-Tree implementation, based on:
// Guttman, A. 1984: "R-Trees : A Dynamic Index Structure for Spatial Searching"
class rtree
{

    friend class rtree_iterator;

public:

    rtree();
    ~rtree();

    void insert(const fid_t& fid, const dbox& b);
    bool erase(const fid_t& fid, const dbox& b);
    void get_total_extent(dbox& db);


    void debug_dump(id_t root = NULL_ID, int level = 0);
    
private:

    static int find_empty_spot(const node* n);

    id_t add_branch(id_t curn, id_t id, const box& b);

    void disconnect_branch(node* n, int i);

    id_t split_node(id_t curn, id_t id, const box& b);

    void insert(const box& b, int level, id_t id);

    void offset_box(box& b, const dbox& db) const;

    int erase_rec(id_t nid, int level, erase_data* pdata);
    
    node_mgr* _nodes;
    id_t _root;
    int _root_level;
    double _offset[2]; //double precision offset into local space   
};




struct rt_iter_stack 
{
    id_t inode;
    char contained;
};

//R-Tree search iterator.
//Incrementally searches a given R-Tree for items intersecting
//the given bbox.
ALGNW class rtree_iterator
{
public:

    rtree_iterator(const rtree* rt, const dbox& db);
   
    fid_t next();

private:
    box4_soa _bwide;
    rt_iter_stack _stack[MAX_DEPTH*MAX_BRANCH];
    rt_iter_stack *_top;
    const node_mgr* _nodes;
} ALGNL;


}