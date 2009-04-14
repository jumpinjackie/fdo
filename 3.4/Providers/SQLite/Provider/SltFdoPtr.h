//
// Copyright (C) 2008  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef _SLTFDOPTR_H_
#define _SLTFDOPTR_H_

template <class T>
inline T* SltFdoPtrAssign(T** pp, T* lp) throw()
{
    if (lp != NULL)
        lp->AddRef();
    if (*pp)
        (*pp)->Release();
    *pp = lp;
    return lp;
}

//More or less the same as FdoPtr, but skips null checks on overloaded operators
template <class T>
class SltFdoPtr
{
public:
    
    SltFdoPtr() throw()
        : p(NULL)
    {}

    //Avoids warnings on initial construction and assigment to NULL pointer
    SltFdoPtr(long ) throw()
        : p(NULL)
    {}

    //Attaches to a pointer, does not AddRef to it.
    SltFdoPtr(T* lp) throw()
        : p(lp)
    {}

    //Copy constructor. Type U must be convertible to type T.
    template <class U>
    SltFdoPtr(const SltFdoPtr<U>& lp) throw()
    {
        p = lp.p;
        if (p)
            p->AddRef();
    }


    ~SltFdoPtr() throw()
    {
        if (p)
            p->Release();
    }

         operator T*()     const throw()     { return  p; }
    T&   operator*()       const throw()     { return *p; }
    T**  operator&()             throw()     { return &p; }
    T*   operator->()      const throw()     { return  p; }
    bool operator!()       const throw()     { return !p; }
    bool operator==(T* pT) const throw()     { return  p == pT; }

    //Assignment to raw pointer -- note that the pointer must
    //be AddRef-ed before using this operator since it does not
    //AddRef
    T* operator=(T* lp) throw()
    {
        if (p)
            p->Release();
        p = lp;
        return p;
    }
    
    template<class U>
    T* operator=(const SltFdoPtr<U>& lp) throw()
    {
        return static_cast<T*>(SltFdoPtrAssign<T>(&p, lp));
    }

    //The pointer we are managing
    T* p;

private:

};
#endif


