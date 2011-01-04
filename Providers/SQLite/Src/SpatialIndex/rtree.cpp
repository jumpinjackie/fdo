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

#include "stdafx.h"
#include "rtree.h"

#if _WIN32
//for file-backed node storage option
#include "MappedFile.h"
#endif

namespace bvh {

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//              Simple (and fast) RAM-based node memory manager
//              for 64 bit platforms and smaller trees on 32 bit
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

    class node_mem_mgr
    {
    public:

        node_mem_mgr()
        {
            _nodes = NULL;
            _sznodes = 0;
            _freenodes = NULL_ID;
        }

        ~node_mem_mgr()
        {
            _aligned_free(_nodes);
        }

        node* get_node(id_t i)
        {
            return &_nodes[i];
        }

        const node* get_node(id_t i) const 
        {
            return &_nodes[i];
        }

        id_t new_node()
        {
            //No nodes are free? Make some.
            if (_freenodes == NULL_ID)
                resize_nodes();

            //get a node from the head of the
            //free nodes list
            id_t ret = _freenodes;
            node* n = get_node(ret);
            _freenodes = n->children[0];
            n->children[0] = NULL_ID;
            return ret;
        }

        void free_node(id_t i)
        {
            node* n = get_node(i);
            n->children[0] = _freenodes;
            _freenodes = i;
        }

    private:

        //Note that the free list must be empty in order to call this,
        //as it currently assumes it is during the reallocation initialization of
        //_freelist
        void resize_nodes()
        {
            size_t oldsz = _sznodes;
            if (!_nodes)
            {
                _sznodes = 8;
            }
            else
            {
                _sznodes = 2 * _sznodes;
            }

            node* newarr = (node*)_aligned_malloc(sizeof(node) * _sznodes, CACHE_LINE);
            if (_nodes)
            {
                memcpy(newarr, _nodes, sizeof(node) * oldsz);
                _aligned_free(_nodes);
            }
            _nodes = newarr;

            if (_freenodes == NULL_ID)
            {
                if (oldsz == 0)
                {
                    //The 0 node is reserved for ourselves, so the first free node
                    //will be on index 1.
                    _freenodes = 1;
                }
                else
                {
                    _freenodes = oldsz;
                }

                id_t i = _freenodes;
                node* ptr = get_node(i);
                node* end = get_node((id_t)_sznodes);

                box4_soa empty(0);

                while (ptr < end)
                {
                    ptr->set_all_boxes(empty);
                    ptr->children[0] = ++i;

                    for (int i=1; i<MAX_BRANCH; i++)
                        ptr->children[i] = NULL_ID;

                    ptr++;
                }
                (ptr-1)->children[0] = NULL_ID;
            }

        }

        //Node array
        node*  _nodes; 
        size_t _sznodes;
        id_t   _freenodes;
    };

    
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//        Out of core node memory manager (using memory mapped file)
//        useful for very large data sets on 32 bit platforms
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

#ifdef _WIN32

const int next_pow_two = (1<<9);
#pragma pack(push,1)
    struct fill
    {
        node n;
        char pad[next_pow_two-sizeof(node)];
    };
#pragma pack(pop)

//if this triggers an error, it means next_pow_two is too big
//for the type of node being used. It has to be the next higher
//power of two of sizeof(node).
static int _static_assert[2*sizeof(node) - next_pow_two];

typedef MappedFile<fill, 16384> node_mf_t;

    class node_mf_mgr
    {
    public:

        node_mf_mgr()
        {
            _nodes = NULL;
            _sznodes = 0;
            _freenodes = NULL_ID;
            _mfnodes = NULL;
        }

        ~node_mf_mgr()
        {
            _aligned_free(_nodes);
            delete _mfnodes;
        }

        node* get_node(id_t i)
        {
            if (_nodes)
            {
                return &_nodes[i];
            }

            return get_mf_node(i, _mfnodes);
        }

        const node* get_node(id_t i) const 
        {
            if (_nodes)
            {
                return &_nodes[i];
            }

            return get_mf_node(i, _mfnodes);            
        }


        id_t new_node()
        {
            if (_freenodes == NULL_ID)
            {
                if (_mfnodes)
                {
                    id_t next = _mfnodes->numrecs();
                    new_mf_node(next);
                    return next;
                }
                else
                {
                    resize_nodes();
                }
            }

            id_t ret = _freenodes;
            node* n = get_node(ret);
            _freenodes = n->children[0];
            n->children[0] = NULL_ID;
            return ret;
        }

        void free_node(id_t i)
        {
            node* n = get_node(i);
            n->children[0] = _freenodes;
            _freenodes = i;
        }

    private:

        static node* get_mf_node(id_t i, node_mf_t* mf)
        {
            fill* p0 = mf->load_noaddref((IndexType)i);
            return &p0->n;
        }

        void new_mf_node(id_t i)
        {
            fill* f0 = _mfnodes->load_newrec((IndexType)i);

            node* n = &f0->n;
            for (int i=0; i<MAX_BRANCH; i++)
                n->children[i] = NULL_ID;
            for (int i=0; i<MAX_BRANCH/4; i++)
                n->child_bounds[i].make_empty();

            _mfnodes->release(f0);
        }

        //Note that the free list must be empty in order to call this,
        //as it currently assumes it is during the reallocation initialization of
        //_freelist
        void resize_nodes()
        {
            size_t oldsz = _sznodes;
            if (_sznodes == 0)
            {
                _sznodes = 8;
            }
            else
            {
                _sznodes = 2 * _sznodes;
            }

            //have we reached the threshold to transition
            //to file-based storage?
            if (_sznodes * sizeof(node) > OUT_OF_CORE_THRESHOLD)
            {
                //transfer the contents of the tree to a new memory mapped file
                _mfnodes = new node_mf_t(MAX_DEPTH);
                wchar_t* fname = _wtempnam(NULL, L"rtree");
                _mfnodes->create(fname);
                free(fname);
                
                for (size_t i=0; i<oldsz; i++)
                {
                    new_mf_node(i);
                    node* n = get_mf_node(i, _mfnodes);
                    *n = _nodes[i];
                }

                new_mf_node(oldsz);
                if (oldsz == 0)
                {
                    new_mf_node(1);
                    _freenodes = 1;
                }
                else
                    _freenodes = oldsz;
                

                _aligned_free(_nodes);
                _nodes = NULL;
                _sznodes = -1;
            }
            else //threshold not reached -- do simple array resize
            {
                node* newarr = (node*)_aligned_malloc(sizeof(node) * _sznodes, CACHE_LINE);
                if (_nodes)
                {
                    memcpy(newarr, _nodes, sizeof(node) * oldsz);
                    _aligned_free(_nodes);
                }
                _nodes = newarr;

                //update the free list by adding all the extra items
                //in the new array to it
                if (_freenodes == NULL_ID)
                {
                    if (oldsz == 0)
                    {
                        //The 0 node is reserved for ourselves, so the first free node
                        //will be on index 1.
                        _freenodes = 1;
                    }
                    else
                    {
                        _freenodes = oldsz;
                    }

                    id_t i = _freenodes;
                    node* ptr = get_node(i);
                    node* end = get_node((id_t)_sznodes);

                    box4_soa empty(0);

                    while (ptr < end)
                    {
                        ptr->set_all_boxes(empty);
                        ptr->children[0] = ++i; //the linked list pointer to the next item

                        for (int i=1; i<MAX_BRANCH; i++)
                            ptr->children[i] = NULL_ID;

                        ptr++;
                    }
                    (ptr-1)->children[0] = NULL_ID;
                }
            }

        }

        //Node array
        node*  _nodes; 
        size_t _sznodes;

        node_mf_t* _mfnodes;

        //head item of free node list
        id_t   _freenodes;
    };

#endif //_WIN32

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//                 R-Tree insert/delete implementation
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\

    rtree::rtree()
    {
        _offset[0] = _offset[1] = 0;
        _nodes = new node_mgr;
        _root = _nodes->new_node();
        _root_level = 0;
    }

    rtree::~rtree()
    {
        delete _nodes;
    }

    void rtree::offset_box(box& b, const dbox& db) const
    {
        const double * offset = _offset;
        b.minx = (float)(db.minx - offset[0]);
        b.miny = (float)(db.miny - offset[1]);
        b.maxx = (float)(db.maxx - offset[0]);
        b.maxy = (float)(db.maxy - offset[1]);
    }

    void rtree::insert(const fid_t& fid, const dbox& db)
    {
        box b;
        node_mgr* nodes = _nodes;

        node* n = nodes->get_node(_root);
        if (n->children[0] == NULL_ID)
        {
            //case where the tree is completely empty still
            //take the opportunity to initialize the offset
            _offset[0] = db.minx;
            _offset[1] = db.miny;

            offset_box(b, db);
            n->set_branch(0, fid_to_id(fid), b);
            return;
        }

        offset_box(b, db);

        insert(b, INT_MAX, fid_to_id(fid));
    }


    struct stk_entry
    {
        id_t inode;
        char child;
    };

    void rtree::insert(const box& b, int level, id_t id)
    {
        stk_entry node_stack[MAX_DEPTH];
        stk_entry* top = node_stack;
        node_mgr* nodes = _nodes;
        id_t curnode = _root;
        box4_soa bwide(b);
        int curlevel = 0;

        //assumes that the root node is not empty
        while (1)
        {
            node* n = nodes->get_node(curnode);

            //have we reached the insertion level
            if (n->is_leaf() || curlevel == level)
            {
                top->inode = curnode;
                break;
            }

            int i = n->pick_child(bwide);
            
            top->inode = curnode;
            top->child = i;
            ++top;
            ++curlevel;

            curnode = n->children[i];
        }


        //At this point, curnode/n contains the node where we want to
        //insert the new item and the stack contains the path we took
        //through the tree to get there (including n).

        id_t newnode = id;

        //add the child to the chosen node -- node will
        //split if necessary
        newnode = add_branch(curnode, newnode, b);

        if (newnode == NULL_ID)
        {
            //Shortcut case when we know we only have to 
            //propagate the new bounds up the hierarchy --
            //slightly faster than passing through the general case
            //given in the else condition, but functionally equivalent
            while(--top >= node_stack)
            {
                node* n = nodes->get_node(top->inode);
                n->add_bbox(top->child, b);
            }
        }
        else 
        {
            while(top > node_stack)
            {
                //go up one level
                --top;

                //if a node was split in the previous level,
                //we have to update the current node with the new
                //bounds of the child that was split
                if (newnode == NULL_ID)
                {
                    node* n = nodes->get_node(top->inode);
                    n->add_bbox(top->child, b);
                }
                else 
                {
                    //get the (possibly changed) bounds of previous node
                    node* n = nodes->get_node(curnode);
                    box tmp;
                    n->summed_bbox(tmp);

                    //get the parent node off the stack
                    //and update the branch that points to the previous node
                    curnode = top->inode;
                    n = nodes->get_node(curnode);
                    n->set_child_bbox(top->child, tmp);

                    //add the newnode to the current node
                    node* nn = nodes->get_node(newnode);
                    nn->summed_bbox(tmp);
                    newnode = add_branch(curnode, newnode, tmp);
                }
            }
        }

        //finally, see if we have to grow a new root
        if (newnode != NULL_ID)
        {
            //make the new root node
            id_t newroot = nodes->new_node();
            node* nr = nodes->get_node(newroot);

            //add the old root as a child
            node* n = nodes->get_node(_root);
            box tmp;
            n->summed_bbox(tmp);
            add_branch(newroot, _root, tmp);

            //add the new node as another child
            n = nodes->get_node(newnode);
            n->summed_bbox(tmp);
            add_branch(newroot, newnode, tmp);

            //update the root pointer
            _root = newroot;
        }
    }
   



    //picks the best child to traverse when inserting
    //a new box -- using a surface area heuristic.
    //The heuristic is to pick the child which will result in 
    //smallest insrease in total area. (see Guttman 1984).
    int node4::pick_child(const box4_soa& bnew) const
    {
        const box4_soa& b = child_bounds;
        vec area_before;
        vec area_after;
        vec area_increase;
            
        //find the areas of the children
        area_before.xmm = _mm_mul_ps(_mm_sub_ps(b.maxx, b.minx), 
                                 _mm_sub_ps(b.maxy, b.miny));      

        //add the new box to each child
        __m128 tot_minx = _mm_min_ps(b.minx, bnew.minx);
        __m128 tot_miny = _mm_min_ps(b.miny, bnew.miny);
        __m128 tot_maxx = _mm_max_ps(b.maxx, bnew.maxx);
        __m128 tot_maxy = _mm_max_ps(b.maxy, bnew.maxy);

        //find the resulting areas
        area_after.xmm = _mm_mul_ps(_mm_sub_ps(tot_maxx, tot_minx), 
                                _mm_sub_ps(tot_maxy, tot_miny));      

        //compute area increase (after - before)
        area_increase.xmm = _mm_sub_ps(area_after, area_before);

        //now pick the child which results in smallest increase in area
        int best = 0;
        float best_incr = area_increase.v[0];
        float best_area = area_before.v[0];
        for (int i=1; i<MAX_BRANCH; i++)
        {
            if (area_increase.v[i] < 0) //equivalent to children[i] == NULL_ID
                break;
            if (    area_increase.v[i] < best_incr 
                || (area_increase.v[i] == best_incr && area_before.v[i] < best_area) )
            {
                best = i;
                best_incr = area_increase.v[i];
                best_area = area_before.v[i];
            }
        }

        return best;

    }


/*
    //picks the best child to traverse when inserting
    //a new box -- using a surface area heuristic.
    //The heuristic is to pick the child which will result in 
    //smallest insrease in total area. (see Guttman 1984).
    int node_generic_mul4::pick_child(const box4_soa& bnew) const
    {
        vec area_before[MAX_BRANCH/4];
        vec area_after[MAX_BRANCH/4];
        vec area_increase[MAX_BRANCH/4];

        __m128 sign_bit_killer = _mm_load_ps((float*)make_positive);
        char limit = MAX_BRANCH/4;

        for (int i=0; i<MAX_BRANCH/4; i++)
        {
            if (children[i*4] == NULL_ID)
            {
                limit = i;
                break;
            }

            const box4_soa& b = child_bounds[i];

            //find the areas of the children
            area_before[i].xmm = _mm_mul_ps(_mm_sub_ps(b.maxx, b.minx), 
                                     _mm_sub_ps(b.maxy, b.miny));      

            //add the new box to each child
            __m128 tot_minx = _mm_min_ps(b.minx, bnew.minx);
            __m128 tot_miny = _mm_min_ps(b.miny, bnew.miny);
            __m128 tot_maxx = _mm_max_ps(b.maxx, bnew.maxx);
            __m128 tot_maxy = _mm_max_ps(b.maxy, bnew.maxy);

            //find the resulting areas
            area_after[i].xmm = _mm_mul_ps(_mm_sub_ps(tot_maxx, tot_minx), 
                                    _mm_sub_ps(tot_maxy, tot_miny));      

            //compute area increase (after - before)
            area_increase[i].xmm = _mm_sub_ps(area_after[i], area_before[i]);

            //Remove the sign bit of the area_increase values --
            //this has the effect of converting -Inf increase resulting
            //from empty children to +Inf, which makes them easier to weed out
            //later when looking for the minimum increase in area
            area_increase[i].xmm = _mm_and_ps(area_increase[i].xmm, sign_bit_killer);
        }

        //Find the child which resulted in smallest area increase -- first, do one
        //pass using SIMD instructions to narrow down the choice to 4 items (one in each SSE register slot)
        //then pick the best of the four remaining using regular scalar code, similar
        //to the node4 implementation above.
        //Note that in the 4-wide part we skip one part of the algorithm, which
        //does a tie-breaker in case of equal areas (see scalar part for details).
        static const int ALGNW array_indices[] ALGNL = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
        veci best_index_4;
        best_index_4.xmm = _mm_load_si128((__m128i*)&array_indices[0]);
        vec best_incr_4 = area_increase[0];
        vec best_area_4 = area_before[0];
        
        for (int i=1; i<limit; i++)
        {
            __m128i cur_index_4 = _mm_load_si128((__m128i*)&array_indices[i*4]);
            
            //area_increase[i] < best_increase ?
            __m128 area_cmp = _mm_cmplt_ps(area_increase[i], best_incr_4);
            
            //best_increase = (area_increase[i] < best_increase) ? area_increase[i] : best_increase;
            best_incr_4 = _mm_or_ps(
                              _mm_and_ps(area_cmp, area_increase[i]),
                              _mm_andnot_ps(area_cmp, best_incr_4));

            //best_area = (area_increase[i] < best_increase) ? area_before[i] : best_area;
            best_area_4 = _mm_or_ps(
                              _mm_and_ps(area_cmp, area_before[i]),
                              _mm_andnot_ps(area_cmp, best_area_4));
            
            //best_index = (area_increase[i] < best_increase) ? cur_index : best_index;
            best_index_4 = _mm_or_si128(
                              _mm_and_si128(_mm_castps_si128(area_cmp), cur_index_4),
                              _mm_andnot_si128(_mm_castps_si128(area_cmp), best_index_4));
        }

        //The scalar part -- checking the remaining SSE vector horizontally
        int best = 0;
        float best_incr = best_incr_4.v[0];
        float best_area = best_area_4.v[0];
        for (int i=1; i<4; i++)
        {
            float area_incr = best_incr_4.v[i];

            if (    area_incr < best_incr 
                || (area_incr == best_incr && (best_area_4.v[i] < best_area) )) //tie-breaker in case areas are equal
            {
                best = best_index_4.v[i];
                best_incr = area_incr;
                best_area = best_area_4.v[i];
            }
        }

        return best;
    }
*/
    //Slightly slower, more comprehensible version of the function above.
    int node_generic_mul4::pick_child(const box4_soa& bnew) const
    {
        vec area_before[MAX_BRANCH/4];
        vec area_after[MAX_BRANCH/4];
        vec area_increase[MAX_BRANCH/4];

        const __m128 sign_bit_killer = _mm_load_ps((float*)make_positive);
        int limit = MAX_BRANCH;

        for (int i=0; i<MAX_BRANCH/4; i++)
        {
            if (children[i*4] == NULL_ID)
            {
                limit = i*4;
                break;
            }

            const box4_soa& b = child_bounds[i];

            //find the areas of the children
            area_before[i].xmm = _mm_mul_ps(_mm_sub_ps(b.maxx, b.minx), 
                                     _mm_sub_ps(b.maxy, b.miny));      

            //add the new box to each child
            __m128 tot_minx = _mm_min_ps(b.minx, bnew.minx);
            __m128 tot_miny = _mm_min_ps(b.miny, bnew.miny);
            __m128 tot_maxx = _mm_max_ps(b.maxx, bnew.maxx);
            __m128 tot_maxy = _mm_max_ps(b.maxy, bnew.maxy);

            //find the resulting areas
            area_after[i].xmm = _mm_mul_ps(_mm_sub_ps(tot_maxx, tot_minx), 
                                    _mm_sub_ps(tot_maxy, tot_miny));      

            //compute area increase (after - before)
            area_increase[i].xmm = _mm_sub_ps(area_after[i], area_before[i]);

            //Remove the sign bit of the area_increase values --
            //this has the effect of converting -Inf increase resulting
            //from empty children to +Inf, which makes them easier to weed out
            //later when looking for the minimum increase in area
            area_increase[i].xmm = _mm_and_ps(area_increase[i].xmm, sign_bit_killer);
        }

        int best = 0;
        float best_incr = area_increase[0].v[0];
        float best_area = area_before[0].v[0];
        for (int i=1; i<limit; i++)
        {
            float area_incr = ((float*)&area_increase)[i];

            if (    area_incr < best_incr 
                || (area_incr == best_incr && ((float*)area_before)[i] < best_area) )
            {
                best = i;
                best_incr = area_incr;
                best_area = ((float*)area_before)[i];
            }
        }

        return best;
    }



    //Adds a new branch to a given node. Splits the node if necessary,
    //and returns the id of the newly created node.
    id_t rtree::add_branch(id_t curn, id_t id, const box& b)
    {
        node_mgr* nodes = _nodes;
        node* n = nodes->get_node(curn);

        //does the inner node have some space to point to the new node?
        int spot = n->find_empty_spot();

        if (spot >= 0)
        {
            //there was a spot for the new branch
            n->set_branch(spot, id, b);
            return NULL_ID;
        }
        else
        {
            //we have to split the node.
            return split_node(curn, id, b);
        }
    }

    //Splits a node into two, assigns children
    //to the two resulting nodes using a (greedy, linear) surface
    //area heuristic
    //NOTE: This implements the "linear split" method described in 
    //Guttman's paper.
    id_t rtree::split_node(id_t curn, id_t id, const box& b)
    {
        //allocate the new node
        node_mgr* nodes = _nodes;
        id_t newnode = nodes->new_node();

        node* n2 = nodes->get_node(newnode); //get the new node ptr
        node* n = nodes->get_node(curn); //get the old node ptr

        //copy the data from the old node + new child into local 
        //array for easier looping
        box bbs[MAX_BRANCH+1];
        id_t ids[MAX_BRANCH+1];

        for (int i=0; i<MAX_BRANCH; i++)
        {
            n->child_bbox(i, bbs[i]);
            ids[i] = n->children[i];
        }
        bbs[MAX_BRANCH] = b;
        ids[MAX_BRANCH] = id;

        //First, pick two seeds for each node -- 
        //find the children that are the farthest apart
        //child index 4 is used to indicate the new branch passed in id/b

        //indices to the highest minx, lowest maxx, highest miny, lowest maxy...
        int max_minx = 0;
        int max_miny = 0;
        int min_maxx = 0;
        int min_maxy = 0;

        //separation measuring box
        box b_far = bbs[0];

        //find the biggest minx, smallest maxx, etc.
        for (int i=1; i<MAX_BRANCH+1; i++)
        {
            if (bbs[i].minx > b_far.minx)
            {
                max_minx = i;
                b_far.minx = bbs[i].minx;
            }

            if (bbs[i].miny > b_far.miny)
            {
                max_miny = i;
                b_far.miny = bbs[i].miny;
            }
    
            if (bbs[i].maxx < b_far.maxx)
            {
                min_maxx = i;
                b_far.maxx = bbs[i].maxx;
            }

            if (bbs[i].maxy < b_far.maxy)
            {
                min_maxy = i;
                b_far.maxy = bbs[i].maxy;
            }
        }
        
        //overall bounds of the old children + new branch
        box btotal = bbs[0];
        for (int i=1; i<MAX_BRANCH+1; i++)
            btotal.add(bbs[i]);

        //pick dimension for best separation
        float w = btotal.maxx - btotal.minx;
        float h = btotal.maxy - btotal.miny;
        if (w == 0) w = 1;
        if (h == 0) h = 1;
        float separation_x = (b_far.minx - b_far.maxx) / w;
        float separation_y = (b_far.miny - b_far.maxy) / h;

        char seed[2];
        char done[MAX_BRANCH+1];
        memset(done, 0, sizeof(done));

        //pick the two seeds
        if (separation_x > separation_y)
        {
            seed[0] = min_maxx;
            seed[1] = max_minx;
        }
        else
        {
            seed[0] = min_maxy;
            seed[1] = max_miny;
        }
        
        //if the seeds are not the same
        //assign them (one each) to the two nodes
        int count[2] = { 0, 0 };
        box total_bbs[2];
        float areas[2];

        if (seed[0] != seed[1])
        {
            n->set_branch(0, ids[seed[0]], bbs[seed[0]]);
            done[seed[0]] = 1;
            count[0]++;
            total_bbs[0] = bbs[seed[0]];
            areas[0] = total_bbs[0].area();

            n2->set_branch(0, ids[seed[1]], bbs[seed[1]]);
            done[seed[1]] = 1;
            count[1]++;
            total_bbs[1] = bbs[seed[1]];
            areas[1] = total_bbs[1].area();
        }
        else
        {
            total_bbs[0].make_empty();
            total_bbs[1].make_empty();
            areas[0] = 0;
            areas[1] = 0;
        }
       

        //Now, we have to assign the remaining branches 
        //to the appropriate node
        for (int i=0; i<MAX_BRANCH+1; i++)
        {
            if (done[i])
                continue;

            //check if one of the nodes is full already
            //and add to the other one.
            if (count[0] == MAX_BRANCH)
            {
                n2->set_branch(count[1]++, ids[i], bbs[i]);
                continue;
            }
            if (count[1] == MAX_BRANCH)
            {
                n->set_branch(count[0]++, ids[i], bbs[1]);
                continue;
            }

            //find resulting area if branch is added to each side
            box b0 = total_bbs[0];
            b0.add(bbs[i]);
            float ar0 = b0.area();
            float incr0 = ar0 - areas[0];

            box b1 = total_bbs[1];
            b1.add(bbs[i]);
            float ar1 = b1.area();
            float incr1 = ar1 - areas[1];

            //pick the node with least increase in area
            if (incr0 < incr1)
            {
                n->set_branch(count[0]++, ids[i], bbs[i]);
                total_bbs[0] = b0;
                areas[0] = ar0;
            }
            else if (incr1 < incr0)
            {
                n2->set_branch(count[1]++, ids[i], bbs[i]);
                total_bbs[1] = b1;
                areas[1] = ar1;
            }

            //in case of tie pick the node with smallest area
            else if (ar0 < ar1)
            {
                n->set_branch(count[0]++, ids[i], bbs[i]);
                total_bbs[0] = b0;
                areas[0] = ar0;
            }
            else if (ar1 < ar0)
            {
                n2->set_branch(count[1]++, ids[i], bbs[i]);
                total_bbs[1] = b1;
                areas[1] = ar1;
            }

            //in case of tie pick the node with fewer children
            else if (count[0] < count[1])
            {
                n->set_branch(count[0]++, ids[i], bbs[i]);
                total_bbs[0] = b0;
                areas[0] = ar0;
            }
            else
            {
                n2->set_branch(count[1]++, ids[i], bbs[i]);
                total_bbs[1] = b1;
                areas[1] = ar1;
            }
        }

        //split is done. Now just have to set remaining
        //branches to empty in the old node, since it may 
        //have lost some children.
        box empty(0);
        for (int i=count[0]; i<MAX_BRANCH; i++)
        {
            n->set_child_bbox(i, empty);
            n->children[i] = NULL_ID;
        }

        return newnode;
    }


    void rtree::get_total_extent(dbox& db)
    {
        box b;
        node* n = _nodes->get_node(_root);
        n->summed_bbox(b);

        db.minx = b.minx + _offset[0];
        db.miny = b.miny + _offset[1];
        db.maxx = b.maxx + _offset[0];
        db.maxy = b.maxy + _offset[1];
    }


    struct erase_data
    {
        id_t id;
        box b;
        id_t reinsert_list[MAX_DEPTH];
        int reinsert_level[MAX_DEPTH];
        int reinsert_count;

        erase_data()
        {
            memset(this, 0, sizeof(erase_data));
        }
    };


    bool rtree::erase(const fid_t& fid, const dbox& b)
    {
        node_mgr* nodes = _nodes;
        erase_data data;
        data.id = fid_to_id(fid);
        offset_box(data.b, b);

        //recursively descend the tree to find and
        //delete the item
        if (erase_rec(_root, 0, &data) == -1)
            return false;
        
        //We are done recursing. Now, we have to add back
        //the contents of nodes which got too light on content
        //during the recursive delete.
        for (int i=0; i<data.reinsert_count; i++)
        {
            node* n = nodes->get_node(data.reinsert_list[i]);

            for (int j=0; j<MAX_BRANCH; j++)
            {
                if (n->children[j] == NULL_ID)
                    break;
                
                box b;
                n->child_bbox(j, b);
                insert(b, /*n->is_leaf() ? INT_MAX :*/ data.reinsert_level[i], n->children[j]);
            }

            //free the node, once its children are back into the tree
            nodes->free_node(data.reinsert_list[i]);
        }

        //Finally, check for redundant root -- a non-leaf root
        //with one child
        node* nr = nodes->get_node(_root);
        if (nr->children[1] == NULL_ID //node has at most one child
            && !nr->is_leaf() //the one child is not a feature id
            && !(nr->children[0] == NULL_ID) //the node does not have zero children (empty root only)
            )
        {
            id_t newroot = nr->children[0];
            nodes->free_node(_root);
            _root = newroot;
        }

        return true;
    }


    int rtree::erase_rec(id_t nid, int level, erase_data* pdata)
    {
        node_mgr* nodes = _nodes;
        node* n = nodes->get_node(nid);
        
        if (n->is_leaf())
        {
            //look for the id of the item in the current node's children
            int i;
            for (i=0; i<MAX_BRANCH; i++)
            {
                if (n->children[i] == NULL_ID) //reached last child, but did not find item? Bail out.
                    return -1;

                if (n->children[i] == pdata->id) //found the item.
                {
                    disconnect_branch(n, i);
                    return i;
                }
            }
        }
        else //inner nodes
        {
            char mask[2*MAX_BRANCH/4];
            n->overlap_mask(mask, pdata->b);

            char* maskptr = mask;
            for (int i=0; i<MAX_BRANCH; i++)
            {
                if (!(maskptr[0] & 1))
                {
                    //recurse...
                    int res = erase_rec(n->children[i], level+1, pdata);
                   
                    n = nodes->get_node(nid); //update the n pointer -- necessary for out of core node storage

                    if (res != -1) //did the recursive call succeed in deleting the item?
                    {
                        node* child = nodes->get_node(n->children[i]);

                        //see if the child that contained the item
                        //is now less than half full, by checking if its
                        //middle child is NULL
                        if (child->children[MAX_BRANCH/2-1] != NULL_ID)
                        {
                            //child still more than half full, so
                            //update child's bbox in current node's list
                            box tmp;
                            child->summed_bbox(tmp);
                            n->set_child_bbox(i, tmp);
                        }
                        else
                        {
                            //child is now less than half full --
                            //remove it and mark it for reinsertion
                            pdata->reinsert_list[pdata->reinsert_count] = n->children[i];
                            pdata->reinsert_level[pdata->reinsert_count] = level+1;
                            pdata->reinsert_count++;
                            disconnect_branch(n, i);
                        }

                        return i;
                    }
                }

                maskptr[0] = maskptr[0] >> 1;
                maskptr[1] = maskptr[1] >> 1;

                if ((i+1)%4 == 0)
                    maskptr += 2;
            }                
        }

        return -1;
    }

     void rtree::disconnect_branch(node* n, int i)
    {
        //erase the item by moving
        //all children after it one spot to the left
        while (++i < MAX_BRANCH) 
        {
            box tmp;
            n->child_bbox(i, tmp);
            n->set_branch(i-1, n->children[i], tmp);
            
            if (n->children[i] == NULL_ID)
                break;
        }

        //zero out the last item if we got to it
        if (i == MAX_BRANCH)
        {
            box empty(1);
            n->set_branch(MAX_BRANCH-1, NULL_ID, empty);
        }
    }


     void rtree::debug_dump(id_t root, int tabs)
     {
         if (root == NULL_ID)
             root = _root;

         static const char* tab = "    ";

         node* n = _nodes->get_node(root);

         if (n->is_leaf())
         {
             for (int i=0; i<tabs; i++)
                 fprintf(stderr, "%s", tab);
             fprintf(stderr, "L %d: ", (int)root);

             for (int i=0; i<MAX_BRANCH; i++)
             {
                 if (n->children[i] == NULL_ID)
                     break;
                 fprintf(stderr, "fid %d, ", (int)n->children[i]);                 
             }

             fprintf(stderr, "\n");
         }
         else
         {
             for (int i=0; i<tabs; i++)
                 fprintf(stderr, "%s", tab);
             fprintf(stderr, "N %d: ", (int)root);

             for (int i=0; i<MAX_BRANCH; i++)
             {
                 if (n->children[i] == NULL_ID)
                     break;
                 fprintf(stderr, "%d, ", (int)n->children[i]);                 
             }

             fprintf(stderr, "\n");

             for (int i=0; i<MAX_BRANCH; i++)
             {
                 if (n->children[i] == NULL_ID)
                     break;
                 
                 debug_dump(n->children[i], tabs+1);
                 n = _nodes->get_node(root);
             }
         }
     }


//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//                R-Tree incremental search structure
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


    rtree_iterator::rtree_iterator(const rtree* rt, const dbox& db)
    {
        _nodes = rt->_nodes;

        box b;
        rt->offset_box(b, db);
        _bwide.make_wide_box(b);

        //push the root node onto the stack
        //to start with
        rt_iter_stack* top = _stack;
        top->inode = rt->_root;
        top->contained = false;
        _top = top+1;
    }
   
    fid_t rtree_iterator::next()
    {
        //cache some member pointers into registers
        const node_mgr* nodes = _nodes;
        rt_iter_stack* top = _top;
        rt_iter_stack* stack = _stack;
 
        //is stack empty -- traversal is already finished
        if (top <= stack)
        {
            _top = top;
            return 0;
        }

        //pop an item off the traversal stack
        --top;

        //current item is a leaf -- return its feature id
        id_t tid = top->inode;
        if (is_leaf(tid))
        {
            _top = top;
            return id_to_fid(tid);
        }

        //We have passed the obvious cases, now we have to drill down
        //into the tree 

        //cache the search box locally -- things seem to go faster like that
        box4_soa bwide;
        bwide.copy(_bwide);

        while (1)
        {

            //not leaf -- get the node and check for overlap
            const node* n = nodes->get_node(top->inode);

            //have we previously determined that this subtree is 
            //fully contained by the search? If yes, we bypass
            //the intersection checking and just add the children to the stack
            if (top->contained)
            {
                const id_t* ch = n->children;

                for (int i=0; i<MAX_BRANCH; i++)
                {
                    if (ch[i] == NULL_ID)
                        break;

                    top->inode = ch[i];
                    top->contained = true;
                    top++;
                }
            }
            else
            {
                char mask[2*MAX_BRANCH/4]; //mask[i] => disjoint boxes mask, mask[i+1] => containment mask
                n->overlap_mask(mask, bwide);
#if MAX_BRANCH != 4
                char* maskptr = mask;
                for (int i=0; i<MAX_BRANCH; i++)
                {
                    if (!(maskptr[0] & 1)) //is disjoint bit set? if not, we have intersection.
                    {
                        top->inode = n->children[i];
                        top->contained = maskptr[1] & 1;
                        top++;
                    }

                    maskptr[0] = maskptr[0] >> 1;
                    maskptr[1] = maskptr[1] >> 1;

                    if ((i+1)%4 == 0)
                    {
                        maskptr += 2;
                    }
                }
#else

                //check the intersection flag for each child
                //and if it intersects push it on the stack.
                //Do this in reverse order so that when popping
                //we get them back in forward order.
                if (!(mask[0] & (1<<3) ))
                {
                    top->inode = n->children[3];
                    top->contained = mask[1] & (1<<3);
                    top++;
                }
                if (!(mask[0] & (1<<2)))
                {
                    top->inode = n->children[2];
                    top->contained = mask[1] & (1<<2);
                    top++;
                }
                if (!(mask[0] & (1<<1)))
                {
                    top->inode = n->children[1];
                    top->contained = mask[1] & (1<<1);
                    top++;
                }
                if (!(mask[0] & (1<<0)))
                {
                    top->inode = n->children[0];
                    top->contained = mask[1] & (1<<0);
                    top++;
                }
#endif
            }

            //Now prepare to loop back around:
            //This section is very similar to the beginning
            //of the function

            //is the stack empty?
            if (top == stack)
            {
                _top = top;
                return 0;
            }

            //pop a new item off the stack
            --top;

            //item is a leaf -- return its feature id
            id_t tid = top->inode;
            if (is_leaf(tid))
            {
                _top = top;
                return id_to_fid(tid);
            }
        }
    }



}