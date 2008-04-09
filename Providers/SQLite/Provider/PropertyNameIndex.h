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

    bool operator< (const NameIndexPair& r)
    {
        return wcscmp(name, r.name) < 0;
    }

};



//Hashtable of property names to their index in a result query
class PropertyNameIndex
{
public:

    void Add(const wchar_t* name, int index)
    {
        NameIndexPair p(name, index);
        m_nametable[(unsigned char)*name & 0xF].push_back(p);
    }

    int __forceinline GetIndex(const wchar_t* name)
    {
        //TODO: this is not fully optimized

        unsigned char hash = (unsigned char)*name & 0xF;
        const std::vector<NameIndexPair>* sublist = &m_nametable[hash];

        //start with the last returned index?
        int start = (m_lastChar == hash) ? m_lastIndex : 0;
        size_t sz = sublist->size();

        for (size_t i=start; i<sz; i++)
        {
            const NameIndexPair& p = sublist->at(i);
            if (wcscmp(name, p.name) == 0)
            {
                m_lastChar = hash;
                m_lastIndex = (int)i;
                return p.index;
            }
        }

        for (int i=0; i<start; i++)
        {
            const NameIndexPair& p = sublist->at(i);
            if (wcscmp(name, p.name) == 0)
            {
                m_lastChar = hash;
                m_lastIndex = (int)i;
                return p.index;
            }
        }

        return -1;
    }

    void Prepare()
    {
    }

    void Clear()
    {
        for (int i=0; i<16; i++)
            m_nametable[i].clear();
    }

private:

    unsigned char m_lastChar;
    int m_lastIndex;
    std::vector<NameIndexPair> m_nametable[16];

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

    int /*__forceinline*/ GetIndex(const wchar_t* name)
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

#endif
