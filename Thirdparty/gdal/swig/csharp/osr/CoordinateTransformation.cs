/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.31
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace OSGeo.OSR {

using System;
using System.Runtime.InteropServices;

public class CoordinateTransformation : IDisposable {
  private HandleRef swigCPtr;
  protected object swigCMemOwner;

  internal CoordinateTransformation(IntPtr cPtr, object cMemoryOwner) {
    swigCMemOwner = cMemoryOwner;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(CoordinateTransformation obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  internal static HandleRef getCPtrAndDisown(CoordinateTransformation obj, object cMemoryOwner) {
    obj.swigCMemOwner = cMemoryOwner;
    return getCPtr(obj);
  }

  ~CoordinateTransformation() {
    Dispose();
  }

  public virtual void Dispose() {
  lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwner == null) {
        swigCMemOwner = new object();
        osrPINVOKE.delete_CoordinateTransformation(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public CoordinateTransformation(SpatialReference src, SpatialReference dst) : this(osrPINVOKE.new_CoordinateTransformation(SpatialReference.getCPtr(src), SpatialReference.getCPtr(dst)), null) {
    if (osrPINVOKE.SWIGPendingException.Pending) throw osrPINVOKE.SWIGPendingException.Retrieve();
  }

  public void TransformPoint(double[] inout) {
    osrPINVOKE.CoordinateTransformation_TransformPoint__SWIG_0(swigCPtr, inout);
    if (osrPINVOKE.SWIGPendingException.Pending) throw osrPINVOKE.SWIGPendingException.Retrieve();
  }

  public void TransformPoint(double[] argout, double x, double y, double z) {
    osrPINVOKE.CoordinateTransformation_TransformPoint__SWIG_1(swigCPtr, argout, x, y, z);
    if (osrPINVOKE.SWIGPendingException.Pending) throw osrPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
