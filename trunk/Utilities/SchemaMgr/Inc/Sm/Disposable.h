#ifndef FDOSMDISPOSABLE_H
#define FDOSMDISPOSABLE_H		1
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

#ifdef _WIN32
#pragma once
#endif

// IDisposable implementor for Schema Manager classes.
// Implements the Dispose function.
class FdoSmDisposable : public FdoIDisposable
{
public:
	FdoSmDisposable(void);
	virtual ~FdoSmDisposable(void);

    /// Convenience function for casting between smart pointers. 
    /// Typically called from a FdoPtr referencing this. 
    /// The returned object would typically be assigned to a 
    /// smart pointer of type T where T and this object's type are 
    /// different but compatible.
    //
    /// Returns NULL if this cannot be cast to type T.
    //
    /// Parameters:
    ///    wraparound: treat this as a regular pointer to be 
    ///      wrapped in and managed by a FdoPtr.
    template <class T> T* SmartCast( bool wraparound = false )
    { 
        T* obj = dynamic_cast<T*>(this);

        if ( wraparound && !obj ) {
    /// Cannot cast. Since we're wrapping, this must be
    /// released.
            FdoIDisposable* disp = this;
            FDO_SAFE_RELEASE(disp);
        }
        else {
            if ( obj && !wraparound ) 
    /// Cast succeeded and not wrapping. Need to 
    /// bump up refcount before assigning to destination
    /// FdoPtr.
                FDO_SAFE_ADDREF(this);
        }

        return obj;
    }

protected:
    virtual void Dispose();

private:
    bool mDeleted;
};

#endif


