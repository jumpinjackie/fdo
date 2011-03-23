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
  *
 */

#ifndef FdoCommonCommonHash___H
#define FdoCommonCommonHash___H	1

#define INIT_HASHSIZE 20
#define KNUTH_SEED (size_t)0x9E3779B1
template <class KOBJ, class DOBJ> class FdoHashIterator;

// NOTE: TODO - we need to run some performance tests to compare std:map with this hash

template <class KOBJ, class DOBJ> class FdoHash
{
    friend class FdoHashIterator <KOBJ, DOBJ>;
    typedef struct list_element
    {
	    KOBJ key;
	    DOBJ data;
        list_element* next;
    } list_element;

    template <class KOBJ, class DOBJ> class ListHash
    {
        list_element* m_root;
        list_element* m_end;
    public:
        ListHash() { m_root = m_end = NULL; }
        ~ListHash()
        {
            while(m_root != NULL)
            {
                list_element* next = m_root->next;
                delete m_root;
                m_root = next;
            }
        }
        list_element* root() { return m_root; }
        void setempty() { m_root = m_end = NULL; }
        list_element* push_back(KOBJ key, DOBJ data)
        {
            list_element* elem = new list_element();
            elem->key = key;
            elem->data = data;
            elem->next = NULL;
            push_back(elem);
            return elem;
        }        
        void push_back(list_element* elem)
        {
            if (m_end != NULL)
            {
                m_end->next = elem;
                m_end = elem;
            }
            else
                m_root = m_end = elem;
        }
        list_element* find(KOBJ key)
        {
            list_element* item = m_root;
            while(item != NULL)
            {
                if (key == item->key)
                    return item;
                item = item->next;
            }
            return NULL;
        }
        void remove(KOBJ key)
        {
            list_element* item = detach(key);
            delete item;
        }
        list_element* detach(KOBJ key)
        {
            list_element* item = m_root;
            list_element* pitem = NULL;
            while(item != NULL)
            {
                if (key == item->key)
                {
                    if (item == m_root)
                        m_root = m_root->next;
                    else
                        pitem->next = item->next;
                    if (item == m_end)
                        m_end = pitem;

                    item->next = NULL;
                    return item;
                }
                pitem = item;
                item = item->next;
            }
            return NULL;
        }
    };

    typedef typename ListHash<KOBJ, DOBJ> HashList;
public:
    template <class KOBJ, class DOBJ> struct FdoPair
    {
	    KOBJ first;
	    DOBJ second;
	    FdoPair(const KOBJ& _Val1, const DOBJ& _Val2)
            : first(_Val1), second(_Val2)
        {}
    };
    typedef typename FdoHashIterator<KOBJ, DOBJ> iterator;
    typedef typename FdoPair<KOBJ, DOBJ> pair;
private:
    size_t m_size;
    size_t m_filledSize;
    size_t m_capacity;
    HashList** m_lists;
    iterator m_end;
private:
    // Hashing function for an integer
    size_t hash_int(KOBJ keyVal)
    {
        long lkey = 0;
        short lnkTypeSz = sizeof(KOBJ);
        if (lnkTypeSz == 8)
        {
            __int64 key = (__int64)keyVal;
            key = (key << 18) - key - 1;
            key = key ^ (key >> 31);
            key = (key + (key << 2)) + (key << 4);
            key = key ^ (key >> 11);
            key = key + (key << 6);
            key = key ^ (key >> 22);

            // Knuth’s Multiplicative Method
            key = (key >> 3) * KNUTH_SEED;
		    if (key < 0)
                key += LLONG_MAX;
            lkey = (long)(key & LONG_MAX);
        }
        else
        {
            long key = (long)keyVal;
            // Robert Jenkins’ 32 bit Mix Function
            key += (key << 12);
            key ^= (key >> 22);
            key += (key << 4);
            key ^= (key >> 9);
            key += (key << 10);
            key ^= (key >> 2);
            key += (key << 7);
            key ^= (key >> 12);
            // Knuth’s Multiplicative Method
            key = (key >> 3) * KNUTH_SEED;
            lkey = key;
        }
	    if (lkey < 0)
            lkey += LONG_MAX;
        return (size_t)(lkey % m_capacity);
    }

    void resize()
    {
        m_filledSize = m_size = 0;
        size_t oldCap = m_capacity;
        m_capacity = (size_t)(m_capacity * 1.4);
        HashList** lists = new HashList*[m_capacity];
        for (size_t i = 0; i < m_capacity; i++)
            lists[i] = NULL;
        HashList** tmp = m_lists;
        m_lists = lists;
        for (int i = 0; i < oldCap; i++)
        {
            HashList* oldLst = tmp[i];
            if (oldLst != NULL)
            {
                list_element* elem = oldLst->root();
                while (elem != NULL)
                {
                    list_element* nextEle = elem->next;
                    size_t pos = hash_int(elem->key);
                    HashList* lst = m_lists[pos];
                    if (lst == NULL)
                    {
                        lst = new HashList();
                        m_lists[pos] = lst;
                    }
                    elem->next = NULL;
                    lst->push_back(elem);
                    m_size++;
                    elem = nextEle;
                }
                oldLst->setempty();
            }
        }
        for (size_t i = 0; i < oldCap; i++)
            delete tmp[i];
        delete[] tmp;
    }
public:
    FdoHash()
    {
        m_capacity = INIT_HASHSIZE;
        m_lists = new HashList*[INIT_HASHSIZE];
        for (size_t i = 0; i < m_capacity; i++)
            m_lists[i] = NULL;
        m_filledSize = m_size = 0;
    }
    ~FdoHash()
    {
        for (size_t i = 0; i < m_capacity; i++)
            delete m_lists[i];
        delete[] m_lists;
    }

    iterator insert(pair item)
    {
        // if we have 80% filled in resize
        if (m_filledSize >= (size_t)(m_capacity*0.8))
            resize();
        list_element* elem = NULL;
        size_t pos = hash_int(item.first);
        HashList* lst = m_lists[pos];
        if (lst == NULL)
        {
            lst = new HashList();
            m_lists[pos] = lst;
            m_filledSize++;
        }
        else
            elem = lst->find(item.first);
        if (elem == NULL)
        {
            elem = lst->push_back(item.first, item.second);
            m_size++;
        }
        else
            elem->data = item.second;
        return iterator(elem);
    }

    iterator find (KOBJ keyVal)
    {
        list_element* elem = NULL;
        size_t pos = hash_int(keyVal);
        HashList* lst = m_lists[pos];
        if (lst != NULL)
            elem = lst->find(keyVal);
        return iterator(elem);
    }
    void erase(KOBJ keyVal)
    {
        size_t pos = hash_int(keyVal);
        HashList* lst = m_lists[pos];
        if (lst != NULL)
            lst->remove(keyVal);
    }
    void erase(iterator item)
    {
        list_element* elem = item->element();
        if (elem == NULL)
            return;
        size_t pos = hash_int(elem->key);
        HashList* lst = m_lists[pos];
        if (lst != NULL)
            lst->remove(elem->key);
    }
    iterator end()
    {
        return m_end;
    }
    iterator begin()
    {
        return FdoHashIterator<KOBJ, DOBJ>(m_lists, m_capacity);
    }
    bool empty()
    {
        return m_size == 0;
    }
    void clear()
    {
        for (size_t i = 0; i < m_capacity; i++)
        {
            delete m_lists[i];
            m_lists[i] = NULL;
        }
        m_filledSize = m_size = 0;
    }
    bool size()
    {
        return m_size;
    }
	DOBJ& operator[](const KOBJ& key)
    {
        list_element* elem = NULL;
        size_t pos = hash_int(key);
        HashList* lst = m_lists[pos];
        if (lst == NULL)
        {
            lst = new HashList();
            m_lists[pos] = lst;
            m_filledSize++;
        }
        else
            elem = lst->find(key);
        if (elem == NULL)
        {
            elem = lst->push_back(key, NULL);
            m_size++;
        }
        return elem->data;
    }
};

template <class KOBJ, class DOBJ> class FdoHashIterator
{
    friend class FdoHash<KOBJ, DOBJ>;
    typedef typename FdoHash<KOBJ, DOBJ>::list_element HashElement;
    typedef typename FdoHash<KOBJ, DOBJ>::ListHash<KOBJ, DOBJ> HashList;
    typedef typename FdoHashIterator<KOBJ, DOBJ> _Ty;

    size_t m_capacity;
    HashList** m_lists;
    size_t m_posList;
    HashElement* m_element;
private:
    FdoHashIterator(HashList** lists, size_t capacity)
    {
        first = 0;
        second = NULL;
        m_lists = lists;
        m_capacity = capacity;
        m_element = NULL;
        m_posList = 0;
        next();
    }
    FdoHashIterator(HashElement* element = NULL)
    {
        first = 0;
        second = NULL;
        m_lists = NULL;
        m_capacity = 0;
        m_element = element;
        m_posList = 0;
        if (element != NULL)
        {
            first = element->key;
            second = element->data;
        }
    }
    bool next()
    {
        first = 0;
        second = NULL;
        if (m_capacity == 0)
            return false;
        if (m_element != NULL)
            m_element = m_element->next;
        if (m_element == NULL)
        {
            do
            {
                if (m_posList >= m_capacity)
                    return false;
                HashList* lst = m_lists[m_posList];
                if (lst != NULL)
                    m_element = lst->root();
                m_posList++;
            }while (m_element == NULL);
        }
        first = m_element->key;
        second = m_element->data;
        return true;
    }

public:
    KOBJ first;
    DOBJ second;

    bool operator==(const _Ty& right) const
    {
        return (this->m_element == right.m_element);
    }

    bool operator!=(const _Ty& right) const
    {
        return (this->m_element != right.m_element);
    }

	_Ty& operator++()
    {
        this->next();
        return *this;
    }

    _Ty operator++(int)
    {
        _Ty tmp = *this;
        ++*this;
        return (tmp);
    }

	const _Ty *operator->() const
    {
        return (this);
    }
};


#endif // FdoCommonCommonHash___H	
