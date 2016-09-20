//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

// =============================================================================
// | The file contains the definition of the class FdoLtmDisposable. The class
// | represents the implementor for Long Transaction Manager classes.
// =============================================================================


#ifndef FdoLtmDisposable_h
#define FdoLtmDisposable_h

#ifdef _WIN32
#pragma once
#endif

class FdoLtmDisposable : public FdoIDisposable 
{

    public:

	    FdoLtmDisposable(void);
	    ~FdoLtmDisposable(void);

        // --------------------------------------------------------------------
        // DESCRIPTION
        // Convenience function for casting between smart pointers. Typically
        // called from a FdoPtr referencing this. The returned object would
        // typically be assigned to a smart pointer of type T where T and this
        // object's type are different but compatible. Returns NULL if this
        // cannot be cast to type T.
        //
        // PARAMETERS
        //   wraparound: treat this as a regular pointer to be wrapped in and
        //   managed by a FdoPtr.
        // --------------------------------------------------------------------

        template <class T> T* SmartCast (bool wraparound = false) {

            T* obj = dynamic_cast<T*>(this);

            if ((wraparound) && (!obj)) {

                // Cannot cast. Since wrapping is turned on, this must be re-
                // leased.

                FdoIDisposable* disp = this;
                FDO_SAFE_RELEASE(disp);

            }  //  if ((wraparound) && ...
            else {

              if ((obj) && (!wraparound)) 

                  // Cast succeeded and no wrapping required. Add a reference
                  // count before returning the pointer.

                  FDO_SAFE_ADDREF(this);

            }  //  else ...

            return obj;

        }  //  template <class T> T* SmartCast ...


    protected:

        virtual void Dispose();


    private:


};  //  class FdoLtmDisposable

#endif

