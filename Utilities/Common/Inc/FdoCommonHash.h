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

// NOTE: TODO - we need to run some performance tests to compare std:map with this hash
namespace FdoCommonTypes
{
    template<class T1, class T2> struct pair
    {
        pair()
            : first(T1()), second(T2())
        {}
        pair(const T1& val1, const T2& val2)
            : first(val1), second(val2)
	    {}
        T1 first;
        T2 second;
    };

    template<class T1, class T2> struct hash_element
    {
        hash_element()
            : key(T1()), data(T2()), next(NULL)
        {}
        hash_element(const T1& val1, const T2& val2)
            : key(val1), data(val2), next(NULL)
	    {}
        T1 key;
        T2 data;
        hash_element* next;
    };

    template<class T1, class T2> class hash_list
    {
        typedef typename hash_element<T1, T2> element;
    private:
        element* m_root;
        element* m_end;
    public:
        hash_list()
            : m_root(NULL), m_end(NULL)
        {}
        ~hash_list()
        {
            while(m_root != NULL)
            {
                element* next = m_root->next;
                delete m_root;
                m_root = next;
            }
        }
        element* root() { return m_root; }
        void setempty() { m_root = m_end = NULL; }
        element* push_back(const T1& key, const T2& data)
        {
            element* elem = new element();
            elem->key = key;
            elem->data = data;
            elem->next = NULL;
            push_back(elem);
            return elem;
        }        
        void push_back(element* elem)
        {
            if (m_end != NULL)
            {
                m_end->next = elem;
                m_end = elem;
            }
            else
                m_root = m_end = elem;
        }
        element* find(const T1& key)
        {
            element* item = m_root;
            while(item != NULL)
            {
                if (key == item->key)
                    return item;
                item = item->next;
            }
            return NULL;
        }
        void remove(const T1& key)
        {
            element* item = detach(key);
            delete item;
        }
        element* detach(const T1& key)
        {
            element* item = m_root;
            element* pitem = NULL;
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

    template<class T1, class T2> class hash;
    template<class T1, class T2> class hash_iterator
    {
        friend class hash<T1, T2>;
        typedef typename hash_list<T1, T2> list;
        typedef typename hash_element<T1, T2> element;
        typedef typename hash_iterator<T1, T2> _MyTy;
    private:
        size_t m_capacity;
        list** m_lists;
        size_t m_posList;
        element* m_element;
    private:
        hash_iterator(list** lists, size_t capacity)
        {
            first = 0;
            second = NULL;
            m_lists = lists;
            m_capacity = capacity;
            m_element = NULL;
            m_posList = 0;
            next();
        }
        hash_iterator(element* elem = NULL)
        {
            first = 0;
            second = NULL;
            m_lists = NULL;
            m_capacity = 0;
            m_element = elem;
            m_posList = 0;
            if (elem != NULL)
            {
                first = elem->key;
                second = elem->data;
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
                    list* lst = m_lists[m_posList];
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
        T1 first;
        T2 second;

        bool operator==(const _MyTy& right) const
        {
            return (this->m_element == right.m_element);
        }

        bool operator!=(const _MyTy& right) const
        {
            return (this->m_element != right.m_element);
        }

	    _MyTy& operator++()
        {
            this->next();
            return *this;
        }

        _MyTy operator++(int)
        {
            _MyTy tmp = *this;
            ++*this;
            return (tmp);
        }

	    const _MyTy *operator->() const
        {
            return (this);
        }
    };

    template <class T1, class T2> class hash
    {
        typedef typename hash_element<T1, T2> element;
        typedef typename hash_list<T1, T2> list;
    public:
        typedef typename hash_iterator<T1, T2> iterator;
    private:
        size_t m_size;
        size_t m_filledSize;
        size_t m_capacity;
        list** m_lists;
        iterator m_end;
    private:
        // Hashing function for an integer
        size_t hash_int(const T1& keyVal)
        {
            long lkey = 0;
            short lnkTypeSz = sizeof(T1);
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
            list** lists = new list*[m_capacity];
            for (size_t i = 0; i < m_capacity; i++)
                lists[i] = NULL;
            list** tmp = m_lists;
            m_lists = lists;
            for (size_t i = 0; i < oldCap; i++)
            {
                list* oldLst = tmp[i];
                if (oldLst != NULL)
                {
                    element* elem = oldLst->root();
                    while (elem != NULL)
                    {
                        element* nextEle = elem->next;
                        size_t pos = hash_int(elem->key);
                        list* lst = m_lists[pos];
                        if (lst == NULL)
                        {
                            lst = new list();
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
        hash()
        {
            m_capacity = INIT_HASHSIZE;
            m_lists = new list*[INIT_HASHSIZE];
            for (size_t i = 0; i < m_capacity; i++)
                m_lists[i] = NULL;
            m_filledSize = m_size = 0;
        }
        ~hash()
        {
            for (size_t i = 0; i < m_capacity; i++)
                delete m_lists[i];
            delete[] m_lists;
        }

        iterator insert(pair<T1, T2> item)
        {
            // if we have 80% filled in resize
            if (m_filledSize >= (size_t)(m_capacity*0.8))
                resize();
            element* elem = NULL;
            size_t pos = hash_int(item.first);
            list* lst = m_lists[pos];
            if (lst == NULL)
            {
                lst = new list();
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

        iterator find (const T1& keyVal)
        {
            element* elem = NULL;
            size_t pos = hash_int(keyVal);
            list* lst = m_lists[pos];
            if (lst != NULL)
                elem = lst->find(keyVal);
            return iterator(elem);
        }
        void erase(const T1& keyVal)
        {
            size_t pos = hash_int(keyVal);
            list* lst = m_lists[pos];
            if (lst != NULL)
                lst->remove(keyVal);
        }
        void erase(iterator item)
        {
            element* elem = item->element();
            if (elem == NULL)
                return;
            size_t pos = hash_int(elem->key);
            list* lst = m_lists[pos];
            if (lst != NULL)
                lst->remove(elem->key);
        }
        iterator end()
        {
            return m_end;
        }
        iterator begin()
        {
            return iterator(m_lists, m_capacity);
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
	    T2& operator[](const T1& key)
        {
            element* elem = NULL;
            size_t pos = hash_int(key);
            list* lst = m_lists[pos];
            if (lst == NULL)
            {
                lst = new list();
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
};

#endif // FdoCommonCommonHash___H	
