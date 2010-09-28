// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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

#ifndef PROPERTY_NAME_INDEX_H
#define PROPERTY_NAME_INDEX_H

#include <vector>
#include <algorithm>

struct NameIndexPair
{
    wchar_t* name;
    int index;

    NameIndexPair()
    {
    }

    NameIndexPair(const wchar_t* name_, int index_)
        : name((wchar_t*)name_), index(index_)
    {}

    bool operator< (const NameIndexPair& r) const
    {
        return wcscmp(name, r.name) < 0;
    }

};



//Hashtable of property names to their index in a result query
class PropertyNameIndex
{
public:

    PropertyNameIndex()
        : m_lastChar(0),
          m_lastIndex(0)
    {
    }

    void Add(const wchar_t* name, int index)
    {
        NameIndexPair p(name, index);
        m_nametable[(unsigned char)*name & 0xF].push_back(p);
    }

    int GetIndex(const wchar_t* name)
    {
        //TODO: this is not fully optimized

        unsigned char hash = (unsigned char)*name & 0xF;
        const std::vector<NameIndexPair>* sublist = &m_nametable[hash];
        size_t sz = m_vecsizes[hash];

        //start with the last returned index?
        size_t start = (m_lastChar == hash) ? m_lastIndex : 0;

        for (size_t i=start; i<sz; i++)
        {
            const NameIndexPair& p = (*sublist)[i];
            if (wcscmp(name, p.name) == 0)
            {
                m_lastChar = hash;
                m_lastIndex = i;
                return p.index;
            }
        }

        for (size_t i=0; i<start; i++)
        {
            const NameIndexPair& p = (*sublist)[i];
            if (wcscmp(name, p.name) == 0)
            {
                m_lastChar = hash;
                m_lastIndex = i;
                return p.index;
            }
        }

        return -1;
    }

    void Prepare()
    {
        for (int i=0; i<16; i++)
            m_vecsizes[i] = m_nametable[i].size();
    }

    void Clear()
    {
        for (int i=0; i<16; i++)
            m_nametable[i].clear();
    }

private:

    unsigned char m_lastChar;
    size_t m_lastIndex;
    std::vector<NameIndexPair> m_nametable[16];
    size_t m_vecsizes[16];

};

//Binary search on sorted list approach to the name-index map
//About as fast as the hashtable above when tested on 10 or so 
//properties -- the real overhead is the string compare.
class PropertyNameIndex2
{
public:

    PropertyNameIndex2()
        : m_sorted(NULL),
          m_len(0),
          m_last(0)
    {
    }

    ~PropertyNameIndex2()
    {
        Clear();
    }

    void Add(const wchar_t* name, int index)
    {
        NameIndexPair p(_wcsdup(name), index);
        m_list.push_back(p);
    }
   
    void Prepare()
    {
        m_len = (unsigned int)m_list.size() - 1;
        m_sorted = new NameIndexPair[m_list.size()];
        
        std::sort(m_list.begin(), m_list.end());

        for (size_t i=0; i<m_list.size(); i++)
        {
            m_sorted[i] = m_list[i];
        }
    }

    int GetIndex(const wchar_t* name)
    {
        NameIndexPair* list = m_sorted;
        int hi = m_len;
        int mid = m_last;
        int lo = 0;

        do
        {
            NameIndexPair* cur = &list[mid];
            int res = wcscmp(name, cur->name);

            if (res == 0)
            {
                m_last = mid;
                return cur->index;
            }
            else if (res < 0)
            {
                hi = mid - 1;
            }
            else if (res > 0)
            {
                lo = mid + 1;
            }

            mid = (hi + lo) / 2;

        } while (hi >= lo);

        return -1;
    }

    void Clear()
    {
        m_list.clear();

        if (m_sorted)
        {
            for (unsigned int i=0; i<=m_len; i++)
                free(m_sorted[i].name);

            delete[] m_sorted;
            m_sorted = NULL;
            m_len = 0;
        }
    }

private:

    unsigned int m_last;
    unsigned int m_len;
    NameIndexPair* m_sorted;

    std::vector<NameIndexPair> m_list;

};


//Suffix Trie (note: Trie, not Tree) - like approach 
class PropertyNameIndex3
{
public:

    struct Node
    {
        Node() : c(0), p(NULL)
        {}

        void InitSize()
        {
            sz = children.size();

            for (size_t i=0; i<sz; i++)
                children[i].InitSize();

            if (sz)
            {
                first = &children[0];
                last = &children[sz - 1];
            }
        }

        wchar_t c;
        NameIndexPair* p;
        std::vector<Node> children;
        size_t sz;
        Node* first;
        Node* last;
    };

    PropertyNameIndex3()
    {
    }

    ~PropertyNameIndex3()
    {
        Clear();
    }

    void Add(const wchar_t* name, int index)
    {
        NameIndexPair p(name, index);
        m_list.push_back(p);
    }
   
    //Builds the tree structure
    void Prepare()
    {
        m_root.c = (wchar_t)-1; // dummy used to not trip the Search algorithm (which trips at 0)

        for (size_t i=0; i<m_list.size(); i++)
        {
            Insert(&m_root, &m_list[i], 0);
        }

        m_root.InitSize();
    }

    int __forceinline GetIndex(const wchar_t* name)
    {
        Node* n = &m_root;
        const wchar_t* tmp = name;

        while (1)
        {
            Node* nchild = n->first;
            Node* last = n->last;

            while (nchild <= last)
            {
                if (nchild->c == *tmp)
                {
                    n = nchild;
                    goto keep_going;
                }

                nchild++;
            }

            return n->c ? -1 : n->p->index;

keep_going: tmp++;
        }
    }

    void Clear()
    {
        m_list.clear();

        m_root.p = NULL;
        m_root.c = (wchar_t)-1;
        m_root.children.clear();
    }


private:

    void Insert(Node* n, NameIndexPair* p, int cur_char_index)
    {
        wchar_t c = p->name[cur_char_index];

        //search for the current character in the list of child nodes
        Node* child = SearchChildren(n, c);

        if (!child)
        {
            //there is not yet a child which matches
            //so we create a new leaf node containing the given string
            Node newnode;
            newnode.c = c;
            newnode.p = c ? NULL : p;
            n->children.push_back(newnode);
            child = &n->children[n->children.size() - 1];
        }
        else
        {
            //there is a child which matches the current character
            //Here we have to see if our string terminates earlier than the
            //existing string which created this node

            //If the child is a leaf node, we have to split it
            /*
            if (child->p)
            {
                //reinsert the leaf data as a child of the node which we are splitting
                NameIndexPair* pold = child->p;
                child->p = NULL; //zero out the leaf pointer -- this makes it an inner node!
                Insert(child, pold, cur_char_index + 1);
            }
            */
        }

        //insert the new node as another child
        if (c)
            Insert(child, p, cur_char_index + 1);
    }

    //Helper to search a list of child nodes for the given character
    Node* SearchChildren(Node* n, wchar_t c)
    {
        size_t sz = n->children.size();
        for (size_t i=0; i<sz; i++)
        {
            Node* nchild = &n->children[i];
            if (nchild->c == c)
            {
                return nchild;
            }
        }

        return NULL;
    }

    std::vector<NameIndexPair> m_list;
    Node m_root;

};

#endif
