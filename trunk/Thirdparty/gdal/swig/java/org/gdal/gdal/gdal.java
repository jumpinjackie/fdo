/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.31
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.gdal.gdal;

public class gdal {
  public static void Debug(String msg_class, String message) {
    gdalJNI.Debug(msg_class, message);
  }

  public static void Error(int msg_class, int err_code, String msg) {
    gdalJNI.Error(msg_class, err_code, msg);
  }

  public static int PushErrorHandler(String pszCallbackName) {
    return gdalJNI.PushErrorHandler__SWIG_0(pszCallbackName);
  }

  public static void PushErrorHandler(SWIGTYPE_p_CPLErrorHandler arg0) {
    gdalJNI.PushErrorHandler__SWIG_1(SWIGTYPE_p_CPLErrorHandler.getCPtr(arg0));
  }

  public static void PopErrorHandler() {
    gdalJNI.PopErrorHandler();
  }

  public static void ErrorReset() {
    gdalJNI.ErrorReset();
  }

  public static int GetLastErrorNo() {
    return gdalJNI.GetLastErrorNo();
  }

  public static int GetLastErrorType() {
    return gdalJNI.GetLastErrorType();
  }

  public static String GetLastErrorMsg() {
    return gdalJNI.GetLastErrorMsg();
  }

  public static void PushFinderLocation(String arg0) {
    gdalJNI.PushFinderLocation(arg0);
  }

  public static void PopFinderLocation() {
    gdalJNI.PopFinderLocation();
  }

  public static void FinderClean() {
    gdalJNI.FinderClean();
  }

  public static String FindFile(String arg0, String arg1) {
    return gdalJNI.FindFile(arg0, arg1);
  }

  public static void SetConfigOption(String arg0, String arg1) {
    gdalJNI.SetConfigOption(arg0, arg1);
  }

  public static String GetConfigOption(String arg0, String arg1) {
    return gdalJNI.GetConfigOption(arg0, arg1);
  }

  public static String CPLBinaryToHex(int nBytes, SWIGTYPE_p_GByte pabyData) {
    return gdalJNI.CPLBinaryToHex(nBytes, SWIGTYPE_p_GByte.getCPtr(pabyData));
  }

  public static SWIGTYPE_p_GByte CPLHexToBinary(String pszHex, SWIGTYPE_p_int pnBytes) {
    long cPtr = gdalJNI.CPLHexToBinary(pszHex, SWIGTYPE_p_int.getCPtr(pnBytes));
    return (cPtr == 0) ? null : new SWIGTYPE_p_GByte(cPtr, false);
  }

  public static double GDAL_GCP_GCPX_get(GCP h) {
    return gdalJNI.GDAL_GCP_GCPX_get(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_GCPX_set(GCP h, double val) {
    gdalJNI.GDAL_GCP_GCPX_set(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_GCPY_get(GCP h) {
    return gdalJNI.GDAL_GCP_GCPY_get(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_GCPY_set(GCP h, double val) {
    gdalJNI.GDAL_GCP_GCPY_set(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_GCPZ_get(GCP h) {
    return gdalJNI.GDAL_GCP_GCPZ_get(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_GCPZ_set(GCP h, double val) {
    gdalJNI.GDAL_GCP_GCPZ_set(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_GCPPixel_get(GCP h) {
    return gdalJNI.GDAL_GCP_GCPPixel_get(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_GCPPixel_set(GCP h, double val) {
    gdalJNI.GDAL_GCP_GCPPixel_set(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_GCPLine_get(GCP h) {
    return gdalJNI.GDAL_GCP_GCPLine_get(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_GCPLine_set(GCP h, double val) {
    gdalJNI.GDAL_GCP_GCPLine_set(GCP.getCPtr(h), h, val);
  }

  public static String GDAL_GCP_Info_get(GCP h) {
    return gdalJNI.GDAL_GCP_Info_get(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_Info_set(GCP h, String val) {
    gdalJNI.GDAL_GCP_Info_set(GCP.getCPtr(h), h, val);
  }

  public static String GDAL_GCP_Id_get(GCP h) {
    return gdalJNI.GDAL_GCP_Id_get(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_Id_set(GCP h, String val) {
    gdalJNI.GDAL_GCP_Id_set(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_get_GCPX(GCP h) {
    return gdalJNI.GDAL_GCP_get_GCPX(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_set_GCPX(GCP h, double val) {
    gdalJNI.GDAL_GCP_set_GCPX(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_get_GCPY(GCP h) {
    return gdalJNI.GDAL_GCP_get_GCPY(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_set_GCPY(GCP h, double val) {
    gdalJNI.GDAL_GCP_set_GCPY(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_get_GCPZ(GCP h) {
    return gdalJNI.GDAL_GCP_get_GCPZ(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_set_GCPZ(GCP h, double val) {
    gdalJNI.GDAL_GCP_set_GCPZ(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_get_GCPPixel(GCP h) {
    return gdalJNI.GDAL_GCP_get_GCPPixel(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_set_GCPPixel(GCP h, double val) {
    gdalJNI.GDAL_GCP_set_GCPPixel(GCP.getCPtr(h), h, val);
  }

  public static double GDAL_GCP_get_GCPLine(GCP h) {
    return gdalJNI.GDAL_GCP_get_GCPLine(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_set_GCPLine(GCP h, double val) {
    gdalJNI.GDAL_GCP_set_GCPLine(GCP.getCPtr(h), h, val);
  }

  public static String GDAL_GCP_get_Info(GCP h) {
    return gdalJNI.GDAL_GCP_get_Info(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_set_Info(GCP h, String val) {
    gdalJNI.GDAL_GCP_set_Info(GCP.getCPtr(h), h, val);
  }

  public static String GDAL_GCP_get_Id(GCP h) {
    return gdalJNI.GDAL_GCP_get_Id(GCP.getCPtr(h), h);
  }

  public static void GDAL_GCP_set_Id(GCP h, String val) {
    gdalJNI.GDAL_GCP_set_Id(GCP.getCPtr(h), h, val);
  }

  public static SWIGTYPE_p_FALSE_IS_ERR GCPsToGeoTransform(int nGCPs, GCP pGCPs, double[] argout, int bApproxOK) {
    return new SWIGTYPE_p_FALSE_IS_ERR(gdalJNI.GCPsToGeoTransform(nGCPs, GCP.getCPtr(pGCPs), pGCPs, argout, bApproxOK), true);
  }

  public static void AllRegister() {
    gdalJNI.AllRegister();
  }

  public static int GetCacheMax() {
    return gdalJNI.GetCacheMax();
  }

  public static void SetCacheMax(int nBytes) {
    gdalJNI.SetCacheMax(nBytes);
  }

  public static int GetCacheUsed() {
    return gdalJNI.GetCacheUsed();
  }

  public static int GetDataTypeSize(int arg0) {
    return gdalJNI.GetDataTypeSize(arg0);
  }

  public static int DataTypeIsComplex(int arg0) {
    return gdalJNI.DataTypeIsComplex(arg0);
  }

  public static String GetDataTypeName(int arg0) {
    return gdalJNI.GetDataTypeName(arg0);
  }

  public static int GetDataTypeByName(String arg0) {
    return gdalJNI.GetDataTypeByName(arg0);
  }

  public static String GetColorInterpretationName(int arg0) {
    return gdalJNI.GetColorInterpretationName(arg0);
  }

  public static String GetPaletteInterpretationName(int arg0) {
    return gdalJNI.GetPaletteInterpretationName(arg0);
  }

  public static String DecToDMS(double arg0, String arg1, int arg2) {
    return gdalJNI.DecToDMS(arg0, arg1, arg2);
  }

  public static double PackedDMSToDec(double arg0) {
    return gdalJNI.PackedDMSToDec(arg0);
  }

  public static double DecToPackedDMS(double arg0) {
    return gdalJNI.DecToPackedDMS(arg0);
  }

  public static SWIGTYPE_p_CPLXMLNode ParseXMLString(String arg0) {
    long cPtr = gdalJNI.ParseXMLString(arg0);
    return (cPtr == 0) ? null : new SWIGTYPE_p_CPLXMLNode(cPtr, false);
  }

  public static String SerializeXMLTree(SWIGTYPE_p_CPLXMLNode xmlnode) {
    return gdalJNI.SerializeXMLTree(SWIGTYPE_p_CPLXMLNode.getCPtr(xmlnode));
  }

  public static int GetDriverCount() {
    return gdalJNI.GetDriverCount();
  }

  public static Driver GetDriverByName(String name) {
    long cPtr = gdalJNI.GetDriverByName(name);
    return (cPtr == 0) ? null : new Driver(cPtr, false);
  }

  public static Driver GetDriver(int i) {
    long cPtr = gdalJNI.GetDriver(i);
    return (cPtr == 0) ? null : new Driver(cPtr, false);
  }

  public static Dataset Open(String name, int eAccess) {
    long cPtr = gdalJNI.Open(name, eAccess);
    return (cPtr == 0) ? null : new Dataset(cPtr, true);
  }

  public static Dataset OpenShared(String name, int eAccess) {
    long cPtr = gdalJNI.OpenShared(name, eAccess);
    return (cPtr == 0) ? null : new Dataset(cPtr, true);
  }

  public static Dataset AutoCreateWarpedVRT(Dataset src_ds, String src_wkt, String dst_wkt, int eResampleAlg, double maxerror) {
    long cPtr = gdalJNI.AutoCreateWarpedVRT(Dataset.getCPtr(src_ds), src_ds, src_wkt, dst_wkt, eResampleAlg, maxerror);
    return (cPtr == 0) ? null : new Dataset(cPtr, true);
  }

  public static java.util.Vector GeneralCmdLineProcessor(java.util.Vector papszArgv, int nOptions) {
    return gdalJNI.GeneralCmdLineProcessor(papszArgv, nOptions);
  }

}
