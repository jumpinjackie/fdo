/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.31
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace OSGeo.GDAL {

using System;
using System.Runtime.InteropServices;

public class ColorEntry : IDisposable {
  private HandleRef swigCPtr;
  protected object swigCMemOwner;

  internal ColorEntry(IntPtr cPtr, object cMemoryOwner) {
    swigCMemOwner = cMemoryOwner;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(ColorEntry obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  internal static HandleRef getCPtrAndDisown(ColorEntry obj, object cMemoryOwner) {
    obj.swigCMemOwner = cMemoryOwner;
    return getCPtr(obj);
  }

  ~ColorEntry() {
    Dispose();
  }

  public virtual void Dispose() {
  lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwner == null) {
        swigCMemOwner = new object();
        gdalPINVOKE.delete_ColorEntry(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public short c1 {
    set {
      gdalPINVOKE.ColorEntry_c1_set(swigCPtr, value);
      if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      short ret = gdalPINVOKE.ColorEntry_c1_get(swigCPtr);
      if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public short c2 {
    set {
      gdalPINVOKE.ColorEntry_c2_set(swigCPtr, value);
      if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      short ret = gdalPINVOKE.ColorEntry_c2_get(swigCPtr);
      if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public short c3 {
    set {
      gdalPINVOKE.ColorEntry_c3_set(swigCPtr, value);
      if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      short ret = gdalPINVOKE.ColorEntry_c3_get(swigCPtr);
      if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public short c4 {
    set {
      gdalPINVOKE.ColorEntry_c4_set(swigCPtr, value);
      if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      short ret = gdalPINVOKE.ColorEntry_c4_get(swigCPtr);
      if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public ColorEntry() : this(gdalPINVOKE.new_ColorEntry(), null) {
    if (gdalPINVOKE.SWIGPendingException.Pending) throw gdalPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
