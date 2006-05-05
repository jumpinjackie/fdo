/******************************************************************************
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
  *
 ******************************************************************************/

// =============================================================================
// | The file contains the definition of the class FdoLkDisposable. The class
// | represents the implementor for Lock Manager classes.
// =============================================================================


#ifndef FdoLkDisposable_h
#define FdoLkDisposable_h

#ifdef _WIN32
#pragma once
#endif

class FdoLkDisposable : public FdoIDisposable 
{

    public:

	    FdoLkDisposable  ();
	    ~FdoLkDisposable ();

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


};  //  class FdoLkDisposable

#endif

