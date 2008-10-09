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

#ifndef ROWIDITERATOR_H
#define ROWIDITERATOR_H

//Provides an interface for iterating back and forth
//over a list of feature row ids. Can either iterate over
//a given set if row ids, or over the implicit full set of sorted 
//IDs.
class RowidIterator
{

public:
    RowidIterator(__int64 count, std::vector<__int64>* list = NULL)
        : m_count(count),
          m_list(list),
          m_current(0) //position one before the first item (1-based indexing)
    {
        if (m_list)
            m_count = m_list->size();
    }

    ~RowidIterator()
    {
        delete m_list;
    }

    __int64 Count() 
    { 
        return m_count; 
    }

    __int64 CurrentIndex() 
    {
        return m_current;
    }

    __int64 LastRowid() 
    { 
        return m_list ? m_list->at(m_count-1) : m_count; 
    }

    __int64 CurrentRowid()
    {
        if (m_list)
        {
            if (m_current < 1)
                return m_list->at(0) - 1;
            else if (m_current > m_count)
                return m_list->at(m_count - 1) + 1;
            else
                return m_list->at(m_current-1);
        }
        else
        {
            return m_current;
        }
    }
    
    //NOTE: 1-based indexing used here!!!
    bool MoveToIndex(__int64 index)
    {
        m_current = index;

        if (m_current < 1)
        {
            m_current = 0;
            return false;
        }
        else if (m_current > m_count + 1)
        {
            m_current = m_count + 1;
            return false;
        }

        return true;
    }

    //positions the cursor at the given row id.
    //NOTE: Incurs linear search if m_list is not empty!
    __int64 FindIndex(__int64 rowid)
    {
        if (m_list)
        {
            for (size_t i=0; i<m_list->size(); i++)
                if (rowid == m_list->at(i))
                    return i;

            return -1;
        }
        else
        {
            return rowid;   
        }
    }
    
    bool Next() 
    { 
        m_current++;

        if (m_current > m_count)
        {
            m_current = m_count + 1;
            return false;
        }

        return true;
    }

    bool Previous()
    {
        m_current--;

        if (m_current < 1)
        {
            m_current = 0;
            return false;
        }

        return true;
    }


private:
    __int64 m_current;
    __int64 m_count;
    std::vector<__int64>*  m_list;
};


#endif

