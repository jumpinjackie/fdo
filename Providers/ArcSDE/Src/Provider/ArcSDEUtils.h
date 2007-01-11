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
 */

#ifndef ARCSDEUTILS_H
#define ARCSDEUTILS_H

#include <malloc.h>



// Constants for ArcSDE Metadata:
extern const char* METADATA_CN_CLASSSCHEMA;
extern const char* METADATA_CN_CLASSNAME;
extern const char* METADATA_CN_CLASSTYPE;
extern const char* METADATA_CN_CLASSDESC;
extern const char* METADATA_CN_CLASSATTR;
extern const char* METADATA_CN_CLASSBASE;
extern const char* METADATA_CN_CLASSABSTRACT;
extern const char* METADATA_CN_PROPNAME;
extern const char* METADATA_CN_PROPTYPE;
extern const char* METADATA_CN_PROPDESC;
extern const char* METADATA_CN_PROPREADONLY;
extern const char* METADATA_CN_PROPDEFAULTVALUE;
extern const char* METADATA_CN_PROPATTR;
extern const char* METADATA_V_DATATYPE_BOOLEAN;
extern const char* METADATA_V_DATATYPE_BYTE;
extern const char* METADATA_V_DATATYPE_INT16;
extern const char* METADATA_V_DATATYPE_INT64;
extern const char* METADATA_V_DATATYPE_SINGLE;
extern const char* METADATA_V_DATATYPE_DOUBLE;
extern const char* METADATA_V_BOOL_YES;
extern const char* METADATA_V_BOOL_NO;


#define ARCSDE_MAX_CREATED_COLUMN_LEN 30  // SE_MAX_COLUMN_LEN is too big for some underlying RDBMS's

#define SCHEMANAME_SEPARATOR L"_"  // separates database name from owner name in FDO schema name

#define SPATIALCONTEXT_AUTHNAME_PREFIX L"FdoName="  // prefix for spatial reference system AUTH_NAME field
#define SPATIALCONTEXT_AUTHNAME_SUFFIX L"="  // suffix for spatial reference system AUTH_NAME field
#define SPATIALCONTEXT_DESC_SUFFIX L"==="  // suffix for spatial reference system DESCRIPTION field
#define SPATIALCONTEXT_COORDYS_WKT_UNKNOWN_SDE "UNKNOWN"  // the string SDE uses to represent unknown coordinate systems WKT
#define SPATIALCONTEXT_CORDSYS_WKT_UNKNOWN_FDO L""  // the string FDO uses to represent unknown coordinate systems WKT
#define SPATIALCONTEXT_CORDSYS_NAME_UNKNOWN_FDO L""  // the string FDO uses to represent unknown coordinate systems Name
#define SPATIALCONTEXT_MAX_DESC_LENGTH 23  // max description length due to defect 639233

// Convenient NLS macros, copied from FdoRdbmsUtil.h/cpp:
extern char *fdoarcsde_cat;  // the default message catalog filename
#define S_FDOARCSDE_1 1  // not sure what this represents, which "string subset" of the catalog to use?
#define NlsMsgGet(msg_num,default_msg) NlsMsgGetMain(msg_num, default_msg)
#define NlsMsgGet1(msg_num,default_msg,arg1) NlsMsgGetMain(msg_num, default_msg,arg1)
#define NlsMsgGet2(msg_num,default_msg,arg1,arg2) NlsMsgGetMain(msg_num, default_msg,arg1,arg2)
#define NlsMsgGet3(msg_num,default_msg,arg1,arg2,arg3) NlsMsgGetMain(msg_num, default_msg,arg1,arg2,arg3)
#define NlsMsgGet4(msg_num,default_msg,arg1,arg2,arg3,arg4) NlsMsgGetMain(msg_num, default_msg,arg1,arg2,arg3,arg4)
#define NlsMsgGet5(msg_num,default_msg,arg1,arg2,arg3,arg4,arg5) NlsMsgGetMain(msg_num, default_msg,arg1,arg2,arg3,arg4,arg5)

FdoString* NlsMsgGetMain(int msg_num, char* default_msg, ...);



void convert_wkb_to_fgf (unsigned char** output, unsigned char** wkb);
void convert_fgf_to_wkb (unsigned char** outWKB, unsigned char** inFGF);
void convert_fdo_operator_to_sde_method(FdoSpatialOperations fdoSpatialOperation, LONG &sdeSearchMethod, BOOL &sdeSearchTruth);
void convert_fgf_to_sde_shape(ArcSDEConnection *connection, FdoByteArray* fgf, SE_COORDREF coordref, SE_SHAPE& shape, bool bCropToExtents = false);
void convert_sde_shape_to_fgf(ArcSDEConnection* connection, SE_SHAPE shape, FdoByteArray*& fgf);


void DebugByteArray(unsigned char pByteArray[], long lByteCount, char *strMessage);

LONG GetCoordRefFromColumn(ArcSDEConnection* connection, const char *strTable, const char *strColumn, SE_COORDREF &coordref);
LONG SetShapeCoordRef(SE_SHAPE &shape, SE_COORDREF &coordref);


// These functions convert ArcSDE errors (basic error + extended error info) to a hierarchy of FDO exceptions of
// given type ERROR_CLASS; if there is an ArcSDE error, the FDO exception is thrown.

// The real function;
// ARGUMENTS:
//    ERROR_CLASS: the FDO exception class to use when creating the FDO exceptions (for example: FdoException,
//       FdoCommandException, etc)
//    Connection: the relevant ArcSDE connection object, if you want to retrieve extended connection error information;
//       NULL otherwise.  Only one of the Connection of Stream arguments should be non-NULL.
//    Stream: the relevant ArcSDE stream object, if you want to retrieve extended stream error information;
//       NULL otherwise.  Only one of the Connection of Stream arguments should be non-NULL.
//    rc:  the status code returned from one of ArcSDE's API functions.  If this indicates success (SE_SUCCESS or
//         SE_FINISHED) then this function does nothing.
//    wcsCustomErrorMsg (optional): a custom error message on top of ArcSDE's error message(s),
//       or NULL if none is desired.
// RETURNS:
//    n/a
// DESCRIPTION:
//    If rc represents an ArcSDE failure, an FDO exception chain of type ERROR_CLASS is thrown.
//    The FDO exception chain (i.e. exceptions containing a "cause" exception) contains the
//    following FDO exceptions in the following order:
//      1) the custom error message in wcsCustomErrorMsg, if not NULL
//      2) the basic ArcSDE error associated with the ArcSDE error code in rc
//      3) the first part (i.e. error_msg1) of the extended ArcSDE error associated with the extended ArcSDE error info
//           for the given Connection or Stream object, if any.
//      4) the second part (i.e. error_msg2) of the extended ArcSDE error associated with the extended ArcSDE error info
//           for the given Connection or Stream object, if any.
template<class ERROR_CLASS> static void _handle_sde_err (SE_CONNECTION Connection, SE_STREAM Stream, LONG rc, char* file, long line, int msg_num, char* default_msg, va_list varargs)
{
    SE_ERROR error;
    // NOTE: SE_MAX_SQL_MESSAGE_LENGTH > SE_MAX_MESSAGE_LENGTH, so we only need SE_MAX_SQL_MESSAGE_LENGTH to
    //  guarantee we can contain error strings for either kind of error.
    CHAR error_string[SE_MAX_SQL_MESSAGE_LENGTH + 15];  // extra 15 is for the error code (rc)

    // NOTE: we create the exceptions in reverse order (from most precise to most general),
    //       so that they appear in the correct order in the "exception cause" hierarchy.

    if ( (rc != SE_SUCCESS) && (rc != SE_FINISHED) )
    {
        FdoPtr<ERROR_CLASS> fdoError;
        wchar_t *wcsErrorString = NULL;


#ifdef _DEBUG
        // Create FDO error for the file name and line number:
        ////////////////////////////////////////////////////////////////////

        wchar_t* wFile = NULL;
        multibyte_to_wide(wFile, file);
        fdoError = ERROR_CLASS::Create(NlsMsgGet2(ARCSDE_ERROR_FILE_LINE, "Error occurred in file %1$ls, at line %2$ld.", wFile, line), fdoError);  // should be OK even if fdoError==NULL
#endif


        // Create FDO error for the extended SDE error info, if any:
        ////////////////////////////////////////////////////////////////////

        if ((SE_DB_IO_ERROR == rc) || (SE_INVALID_WHERE == rc))
        {
            LONG temp_rc = SE_FAILURE;

            // Get extended error info:
            if (NULL != Stream)
            {
                // Assume this is a stream error:
                temp_rc = SE_stream_get_ext_error (Stream, &error);
            }
            else if (NULL != Connection)
            {
                // Assume this is a connection error:
                temp_rc = SE_connection_get_ext_error (Connection, &error);
            }

            // If got extended error info, create an FDO error around it:
            if (SE_SUCCESS == temp_rc)
            {
                // Add error for error.err_msg2, if any:
                if (strlen(error.err_msg2)>0)
                {
                    error_string[0] = '\0';
                    sprintf(error_string, "%s (%d)", error.err_msg2, error.ext_error);
                    multibyte_to_wide(wcsErrorString, error_string);
                    fdoError = ERROR_CLASS::Create(wcsErrorString, fdoError);  // should be OK even if fdoError==NULL
                    wcsErrorString = NULL;
                }

                // Add error for error.err_msg1:
                error_string[0] = '\0';
                sprintf(error_string, "%s (%d)", error.err_msg1, error.ext_error);
                multibyte_to_wide(wcsErrorString, error_string);
                fdoError = ERROR_CLASS::Create(wcsErrorString, fdoError);  // should be OK even if fdoError==NULL
                wcsErrorString = NULL;
            }
         }


        // Create FDO error for the basic SDE error:
        ///////////////////////////////////////////////////////////////////

        // get basic error info:
        strcpy (error_string, "Unknown ArcSDE error code"); // in case SE_error_get_string() fails
        SE_error_get_string (rc, error_string);
        sprintf(error_string, "%s (%d)", error_string, rc);
        // TODO: verify copying from error_string to self is OK

        // convert basic info to FDO exception:
        multibyte_to_wide(wcsErrorString, error_string);
        fdoError = ERROR_CLASS::Create(wcsErrorString, fdoError);  // should be OK even if fdoError==NULL
        wcsErrorString = NULL;


        // Create FDO error with custom error message:
        ///////////////////////////////////////////////////////////////////

        const wchar_t* wErrorMsg = FdoException::NLSGetMessage(msg_num, default_msg, fdoarcsde_cat, varargs);
        fdoError = ERROR_CLASS::Create(wErrorMsg, fdoError);  // should be OK even if fdoError==NULL


        // Now throw the error to the caller:
        throw FDO_SAFE_ADDREF(fdoError.p);
    }
}


// Convenience shorthand function:
template<class ERROR_CLASS> static void handle_sde_err (LONG rc, char* file, long line, int msg_num, char* default_msg, ...)
{
    va_list varargs;
    va_start(varargs, default_msg);
    _handle_sde_err<ERROR_CLASS>(NULL, NULL, rc, file, line, msg_num, default_msg, varargs);
    va_end(varargs);
}

// Convenience shorthand function:
template<class ERROR_CLASS> static void handle_sde_err (SE_CONNECTION Connection, LONG rc, char* file, long line, int msg_num, char* default_msg, ...)
{
    va_list varargs;
    va_start(varargs, default_msg);
    _handle_sde_err<ERROR_CLASS>(Connection, NULL, rc, file, line, msg_num, default_msg, varargs);
    va_end(varargs);
}

// Convenience shorthand function:
template<class ERROR_CLASS> static void handle_sde_err (SE_STREAM Stream, LONG rc, char* file, long line, int msg_num, char* default_msg, ...)
{
    va_list varargs;
    va_start(varargs, default_msg);
    _handle_sde_err<ERROR_CLASS>(NULL, Stream, rc, file, line, msg_num, default_msg, varargs);
    va_end(varargs);
}


// Returns true if the given class contains the given property:
bool ClassContainsProperty(FdoClassDefinition *pClassDef, FdoIdentifier* pPropertyId);
bool IsIdentityProperty(FdoClassDefinition* pClassDef, FdoString* strIdPropertyName);


// Returns true if the given class contains any rows:
bool ClassContainsData(FdoIConnection *conn, FdoClassDefinition *fdoClass);

// Converts the SDE column type to/from an FDO data type:
FdoDataType SDEType2FdoType (LONG sdeType); // Does a *simplified* mapping of SDE types to FDO types, since SQL command doesn't know about FDO classes/properties/overrides
LONG FdoType2SDEType (FdoDataType type);

// Converts an SDE datetime value to/from an FDO datetime value:
FdoDateTime SdeDateTime2FdoDateTime(struct tm sdeDateTime);
struct tm FdoDateTime2SdeDateTime(FdoDateTime fdoDateTime);


// Generates a unique name based on the given names inuse and maximum name length;
// The generatedName must have been allocated by the caller and have a length >= lMaxLen.
// The suggestedName can be any length.
// The lMaxLen argument represents the maximum number of characters (*including NULL terminator*) that 
// the generatedName can have.
void generateUniqueName(ArcSDEConnection *conn, const std::vector<std::string> namesInUse, long lMaxLen, const CHAR* suggestedName, bool bStrict, CHAR* dbName, CHAR* ownerName, CHAR *generatedName);


// Returns true if-and-only-if the given collection contains an item with the given name 
FdoPropertyValue* GetItemNoThrow(FdoPropertyValueCollection *coll, FdoString* name);



// Caller is responsible for freeing memory associated with whereClause and spatialFilters
void GetFilterInfo(ArcSDEConnection *connection, FdoFilter* filter, FdoClassDefinition* definition, CHAR* &whereClause, SHORT &spatialFilterCount, SE_FILTER* &spatialFilters);

// Applies given filter info to given stream:
void ApplyFilterInfoToStream(ArcSDEConnection *connection, SE_STREAM queryStream, const CHAR* table, CHAR* whereClause, const int columnCount, const CHAR** columnNames, const SHORT spatialFilterCount, SE_FILTER* spatialFilters, FdoOrderingOption orderingOption = FdoOrderingOption_Ascending, FdoIdentifierCollection* orderingIds = NULL);

// Add one or more spatial filters that represent the given spatial operation:
void AddSpatialFilters(ArcSDEConnection* conn, const FdoSpatialOperations fdoSpatialOperation, const char* sdeColumnName, SE_SHAPE &shape, std::vector<SE_FILTER> &spatialFilters);

// Apply the given info to the given SE_QUERYINFO object:
void ApplyFilterInfoToQueryInfo(ArcSDEConnection *connection, SE_QUERYINFO query_info, const CHAR *table, const CHAR *whereClause, FdoInt32 numProperties, const CHAR **columnNames, FdoOrderingOption orderingOption, FdoIdentifierCollection* ids);

// Gets the endpoints of the given shape as an array of new SE_SHAPE objects;
// The returned SE_SHAPE array needs to be freed by the caller using delete[]:
SE_SHAPE* GetEndPointsAsShapes(ArcSDEConnection *connection, SE_SHAPE shape, long &lEndpointCount);



///////////////////////////////////////////
// Operating System-dependent functionality:
///////////////////////////////////////////

#ifdef _WIN32

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ARCSDEPROVIDER_EXPORTS
// symbol defined.  The ARCSDEPROVIDER_EXPORTS symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ARCSDEPROVIDER_EXPORTS functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ARCSDEPROVIDER_EXPORTS
#define FDOSDE_API __declspec(dllexport)
#else
#define FDOSDE_API __declspec(dllimport)
#endif

#else // _WIN32

#define FDOSDE_API

#endif // _WIN32


// Need to export a few things for white box testing
#ifdef _DEBUG
#define FDOSDE_TEST FDOSDE_API
#else
#define FDOSDE_TEST
#endif 

template <typename T> T* CheckMemory(T* p)
{
    if (p == NULL)
        throw FdoException::Create(NlsMsgGet(ARCSDE_MEMORY_FAILURE, "Failed to allocate memory."));
    return p;
}


// Return true IFF the given function name is a supported aggregate function:
bool IsAggregateFunction(const wchar_t* functionName);

// Get the SDE type that the given function will evaluate to:
LONG GetFunctionSdeType(FdoIConnection *conn, FdoString *fdoFunctionName);

// Get the SDE statistic type mask for the given aggregate function name:
LONG GetAggregateSdeStatsMask(FdoString* aggrFunctionName);

// Get the value of an aggregate function that has return type "double":
double GetAggregateValueDouble(SE_STATS* stats, FdoString* aggrFunctionName);

// Get the value of an aggregate function that has return type "FdoInt64":
FdoInt64 GetAggregateValueInt64(SE_STATS* stats, FdoString* aggrFunctionName);

// Get the function name of the given identifier, if it is a computed idenfitier; otherwise throw an exception:
FdoString* GetAggregateFunctionName(FdoIdentifier *id);

// Get the property name the given aggregate function operates on:
FdoString* GetAggregateFunctionPropertyName(FdoFunction *fdoFunction);

FdoString* RdbmsToString(long lRdbmsID);


#endif // ARCSDEUTILS_H

