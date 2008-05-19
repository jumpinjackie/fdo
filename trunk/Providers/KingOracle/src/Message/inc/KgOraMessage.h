/*
* Copyright (C) 2006  SL-King d.o.o
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

#ifdef _WIN32
#define g_KgOraMsgCat "KingOracleMessage.dll"
#else
#define g_KgOraMsgCat "KingOracleMessage.cat"
#endif

inline FdoString* NlsMsgGetKgOra(int msg_num, char* default_msg, ...)
{
    va_list varargs;
    va_start(varargs, default_msg);
    #ifdef _FDO_3_1
    FdoString* ret = GisException::NLSGetMessage(msg_num, default_msg, g_KgOraMsgCat, varargs);
    #else
    FdoString* ret = FdoException::NLSGetMessage(msg_num, default_msg, g_KgOraMsgCat, varargs);
    #endif
    va_end(varargs);

    return ret;
}

//************************************************************************************
//************************************************************************************
//** GENERIC MESSAGES 
//************************************************************************************
//************************************************************************************
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: M_KGORA_UNEXPECTED_ERROR
//
// MessageText:
//
// Unexpected error '%1$ls'.
//
#define M_KGORA_UNEXPECTED_ERROR         0x000003E9L

//
// MessageId: M_KGORA_UNDEFINED_ERROR
//
// MessageText:
//
// Error code '%1$d' returned.
//
#define M_KGORA_UNDEFINED_ERROR          0x000003EAL

//
// MessageId: M_KGORA_OUT_OF_MEMORY_ERROR
//
// MessageText:
//
// Out of memory.
//
#define M_KGORA_OUT_OF_MEMORY_ERROR      0x000003EBL

//
// MessageId: M_KGORA_NULL_PARAMETER_ERROR
//
// MessageText:
//
// Null parameter.
//
#define M_KGORA_NULL_PARAMETER_ERROR     0x000003ECL

//
// MessageId: M_KGORA_READ_FILE_ERROR
//
// MessageText:
//
// Error occured reading file '%1$ls'.
//
#define M_KGORA_READ_FILE_ERROR          0x000003EDL

//
// MessageId: M_KGORA_WRITE_FILE_ERROR
//
// MessageText:
//
// Error occured writing file '%1$ls'.
//
#define M_KGORA_WRITE_FILE_ERROR         0x000003EEL

//
// MessageId: M_KGORA_INVALID_RECORD_NUMBER_ERROR
//
// MessageText:
//
// Invalid record number %1$ld for file '%2$ls'.
//
#define M_KGORA_INVALID_RECORD_NUMBER_ERROR 0x000003EFL

//
// MessageId: M_KGORA_END_OF_FILE_ERROR
//
// MessageText:
//
// End of file occured reading shape at offset %1$ld for file '%2$ls'.
//
#define M_KGORA_END_OF_FILE_ERROR        0x000003F0L

//
// MessageId: M_KGORA_UNSUPPORTED_FILE_FORMAT_ERROR
//
// MessageText:
//
// The file '%1$ls' has an unsupported format (%2$ld).
//
#define M_KGORA_UNSUPPORTED_FILE_FORMAT_ERROR 0x000003F1L

//************************************************************************************
//************************************************************************************
//** CONNECTION MESSAGES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_CONNECTION_TIMEOUT_UNSUPPORTED
//
// MessageText:
//
// Connection timeout is not supported.
//
#define M_KGORA_CONNECTION_TIMEOUT_UNSUPPORTED 0x000007D1L

//
// MessageId: M_KGORA_CONNECTION_COMMAND_NOT_SUPPORTED
//
// MessageText:
//
// The command %1$d is not supported.
//
#define M_KGORA_CONNECTION_COMMAND_NOT_SUPPORTED 0x000007D2L

//
// MessageId: M_KGORA_CONNECTION_CONFIGURATION_FILE_NOT_SUPPORTED
//
// MessageText:
//
// King.Oracle Provider does not support configuration files.
//
#define M_KGORA_CONNECTION_CONFIGURATION_FILE_NOT_SUPPORTED 0x000007D3L

//
// MessageId: M_KGORA_CONNECTION_INVALID
//
// MessageText:
//
// Connection is invalid.
//
#define M_KGORA_CONNECTION_INVALID       0x000007D4L

//
// MessageId: M_KGORA_CONNECTION_REQUIRED_PROPERTY_NULL
//
// MessageText:
//
// The required property '%1$ls' cannot be set to NULL.
//
#define M_KGORA_CONNECTION_REQUIRED_PROPERTY_NULL 0x000007D5L

//
// MessageId: M_KGORA_CONNECTION_ENUM_PROPERTY_WRONG_VALUE
//
// MessageText:
//
// The given value '%1$ls' for enumerable property '%2$ls' is not in the enumeration value collection.
//
#define M_KGORA_CONNECTION_ENUM_PROPERTY_WRONG_VALUE 0x000007D6L

//
// MessageId: M_KGORA_CONNECTION_PROPERTY_NOT_FOUND
//
// MessageText:
//
// The connection property '%1$ls' was not found.
//
#define M_KGORA_CONNECTION_PROPERTY_NOT_FOUND 0x000007D7L

//
// MessageId: M_KGORA_CONNECTION_ALREADY_OPEN
//
// MessageText:
//
// The connection is already open.
//
#define M_KGORA_CONNECTION_ALREADY_OPEN  0x000007D8L

//
// MessageId: M_KGORA_CONNECTION_LOCATION_NOT_EXIST
//
// MessageText:
//
// The directory '%1$ls' does not exist.
//
#define M_KGORA_CONNECTION_LOCATION_NOT_EXIST 0x000007D9L

//
// MessageId: M_KGORA_CONNECTION_FILE_INVALID_VERSION
//
// MessageText:
//
// The file '%1$ls' has an invalid version number (%2$d).
//
#define M_KGORA_CONNECTION_FILE_INVALID_VERSION 0x000007DAL

//
// MessageId: M_KGORA_CONNECTION_FILE_NOT_FOUND
//
// MessageText:
//
// The file '%1$ls' was not found.
//
#define M_KGORA_CONNECTION_FILE_NOT_FOUND 0x000007DBL

//
// MessageId: M_KGORA_CONNECTION_PATH_NOT_FOUND
//
// MessageText:
//
// The path to file '%1$ls' was not found.
//
#define M_KGORA_CONNECTION_PATH_NOT_FOUND 0x000007DCL

//
// MessageId: M_KGORA_CONNECTION_TOO_MANY_OPEN_FILES
//
// MessageText:
//
// Too many open files.
//
#define M_KGORA_CONNECTION_TOO_MANY_OPEN_FILES 0x000007DDL

//
// MessageId: M_KGORA_CONNECTION_ACCESS_DENIED
//
// MessageText:
//
// Access to file '%1$ls' was denied.
//
#define M_KGORA_CONNECTION_ACCESS_DENIED 0x000007DEL

//
// MessageId: M_KGORA_CONNECTION_FILE_INVALID_BOUNDING_BOX
//
// MessageText:
//
// The file '%1$ls' has an invalid bounding box (%2$ls).
//
#define M_KGORA_CONNECTION_FILE_INVALID_BOUNDING_BOX 0x000007DFL

//
// MessageId: M_KGORA_CONNECTION_FILE_INVALID
//
// MessageText:
//
// The file '%1$ls' cannot be opened.
//
#define M_KGORA_CONNECTION_FILE_INVALID  0x000007E0L

//
// MessageId: M_KGORA_CONNECTION_FILE_MISMATCH
//
// MessageText:
//
// The file '%1$ls' is corrupt or does not have the same number of objects as the file '%2$ls'.
//
#define M_KGORA_CONNECTION_FILE_MISMATCH 0x000007E1L

//
// MessageId: M_KGORA_NO_DIRECTORY_SPECIFIED
//
// MessageText:
//
// The '%1$ls' connection property was not specified and no shape file location override was specified.
//
#define M_KGORA_NO_DIRECTORY_SPECIFIED   0x000007E2L

//
// MessageId: M_KGORA_TEMPORARY_FILE_PROBLEM
//
// MessageText:
//
// A temporary file could not be created in directory '%1$ls'.
//
#define M_KGORA_TEMPORARY_FILE_PROBLEM   0x000007E3L

//
// MessageId: M_KGORA_INVALID_CONNECTION_STRING
//
// MessageText:
//
// Invalid connection string '%1$ls'
//
#define M_KGORA_INVALID_CONNECTION_STRING 0x000007E4L

//************************************************************************************
//************************************************************************************
//** CONNECTION PROPERTIES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_CONNECTION_PROPERTY_DEFAULT_FILE_LOCATION
//
// MessageText:
//
// DefaultFileLocation2
//
#define M_KGORA_CONNECTION_PROPERTY_DEFAULT_FILE_LOCATION 0x00000BB9L

//
// MessageId: M_KGORA_CONNECTION_PROPERTY_TEMPORARY_FILE_LOCATION
//
// MessageText:
//
// TemporaryFileLocation
//
#define M_KGORA_CONNECTION_PROPERTY_TEMPORARY_FILE_LOCATION 0x00000BBAL

//
// MessageId: M_KGORA_CONNECTION_PROPERTY_SERVICE_NAME
//
// MessageText:
//
// ServiceName
//
#define M_KGORA_CONNECTION_PROPERTY_SERVICE_NAME 0x00000BBBL

//
// MessageId: M_KGORA_CONNECTION_PROPERTY_USERNAME
//
// MessageText:
//
// Username
//
#define M_KGORA_CONNECTION_PROPERTY_USERNAME 0x00000BBCL

//
// MessageId: M_KGORA_CONNECTION_PROPERTY_PASSWORD
//
// MessageText:
//
// Password
//
#define M_KGORA_CONNECTION_PROPERTY_PASSWORD 0x00000BBDL

//
// MessageId: M_KGORA_CONNECTION_PROPERTY_ORACLE_SCHEMA
//
// MessageText:
//
// OracleSchema
//
#define M_KGORA_CONNECTION_PROPERTY_ORACLE_SCHEMA 0x00000BBEL

//
// MessageId: M_KGORA_CONNECTION_PROPERTY_FDO_VIEWS_TABLE
//
// MessageText:
//
// FdoViewsTable
//
#define M_KGORA_CONNECTION_PROPERTY_FDO_VIEWS_TABLE 0x00000BBFL

//************************************************************************************
//************************************************************************************
//** SCHEMA-RELATED MESSAGES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_OBJECT_PROPERTIES_NOT_SUPPORTED
//
// MessageText:
//
// Object properties are not supported.
//
#define M_KGORA_OBJECT_PROPERTIES_NOT_SUPPORTED 0x00000FA1L

//
// MessageId: M_KGORA_UNSUPPORTED_DATATYPE
//
// MessageText:
//
// The '%1$ls' data type is not supported by King.Oracle.
//
#define M_KGORA_UNSUPPORTED_DATATYPE     0x00000FA2L

//
// MessageId: M_KGORA_UNKNOWN_COLUMN_TYPE
//
// MessageText:
//
// The '%1$d' column type is unknown.
//
#define M_KGORA_UNKNOWN_COLUMN_TYPE      0x00000FA3L

//
// MessageId: M_KGORA_UNKNOWN_SHAPE_TYPE
//
// MessageText:
//
// The shape type number '%1$d' is unknown.
//
#define M_KGORA_UNKNOWN_SHAPE_TYPE       0x00000FA4L

//
// MessageId: M_KGORA_SCHEMA_FAIL_READ_FEATURE_SCHEMAS
//
// MessageText:
//
// Failed to de-serialize the feature schemas from the configuration.
//
#define M_KGORA_SCHEMA_FAIL_READ_FEATURE_SCHEMAS 0x00000FA5L

//
// MessageId: M_KGORA_SCHEMA_FAIL_READ_SCHEMA_MAPPINGS
//
// MessageText:
//
// Failed to de-serialize the schema mappings from the configuration.
//
#define M_KGORA_SCHEMA_FAIL_READ_SCHEMA_MAPPINGS 0x00000FA6L

//
// MessageId: M_KGORA_SCHEMA_EXCESSIVE_GEOMETRY
//
// MessageText:
//
// The class '%1$ls' contains more than one geometry property.
//
#define M_KGORA_SCHEMA_EXCESSIVE_GEOMETRY 0x00000FA8L

//
// MessageId: M_KGORA_SCHEMA_CREATION_FAILED
//
// MessageText:
//
// Schema create failed.
//
#define M_KGORA_SCHEMA_CREATION_FAILED   0x00000FAAL

//
// MessageId: M_KGORA_UNSUPPORTED_SHAPE
//
// MessageText:
//
// The '%1$ls' geometry type is not supported by King.Oracle.
//
#define M_KGORA_UNSUPPORTED_SHAPE        0x00000FABL

//
// MessageId: M_KGORA_MISSING_SCHEMA_MAPPING
//
// MessageText:
//
// Missing schema mapping for feature schema in configuration file.
//
#define M_KGORA_MISSING_SCHEMA_MAPPING   0x00000FACL

//
// MessageId: M_KGORA_MISSING_PHYSICAL_SCHEMA
//
// MessageText:
//
// Missing physical schema.
//
#define M_KGORA_MISSING_PHYSICAL_SCHEMA  0x00000FADL

//
// MessageId: M_KGORA_SCHEMA_NOT_FOUND
//
// MessageText:
//
// Schema '%1$ls' not found.
//
#define M_KGORA_SCHEMA_NOT_FOUND         0x00000FAEL

//
// MessageId: M_KGORA_CLASS_IDENTIFIER_SCOPES_NOT_SUPPORTED
//
// MessageText:
//
// Fdo Class Identifiers that contain scopes are not supported.
//
#define M_KGORA_CLASS_IDENTIFIER_SCOPES_NOT_SUPPORTED 0x00000FAFL

//
// MessageId: M_KGORA_CLASS_NAME_NOT_UNIQUE
//
// MessageText:
//
// The unqualified FDO class name '%1$ls' is not unique across all FDO schemas.
//
#define M_KGORA_CLASS_NAME_NOT_UNIQUE    0x00000FB0L

//
// MessageId: M_KGORA_FEATURE_CLASS_NOT_FOUND
//
// MessageText:
//
// FDO class '%1$ls' not found in schema.
//
#define M_KGORA_FEATURE_CLASS_NOT_FOUND  0x00000FB1L

//
// MessageId: M_KGORA_TOO_MANY_IDENTITY_PROPERTIES
//
// MessageText:
//
// FDO class '%1$ls' has more than one identity property; only one identity property is supported.
//
#define M_KGORA_TOO_MANY_IDENTITY_PROPERTIES 0x00000FB2L

//
// MessageId: M_KGORA_IDENTITY_PROPERTY_INCORRECT_TYPE
//
// MessageText:
//
// FDO class '%1$ls' has an identity property of type '%2$ls'; only type 'Int32' is supported.
//
#define M_KGORA_IDENTITY_PROPERTY_INCORRECT_TYPE 0x00000FB3L

//
// MessageId: M_KGORA_SCHEMA_NOT_SPECIFIED
//
// MessageText:
//
// No schema specified for the apply schema command.
//
#define M_KGORA_SCHEMA_NOT_SPECIFIED     0x00000FB4L

//
// MessageId: M_KGORA_SCHEMA_UNSUPPORTED_ELEMENT_STATE
//
// MessageText:
//
// Schema element state '%1$d' is not supported.
//
#define M_KGORA_SCHEMA_UNSUPPORTED_ELEMENT_STATE 0x00000FB5L

//
// MessageId: M_KGORA_SCHEMA_DELETED_CLASS_IN_ADDED_SCHEMA
//
// MessageText:
//
// Conflicting schema element state: a class marked for deletion was found in a schema to be added.
//
#define M_KGORA_SCHEMA_DELETED_CLASS_IN_ADDED_SCHEMA 0x00000FB6L

//
// MessageId: M_KGORA_SCHEMA_MODIFIED_CLASS_IN_ADDED_SCHEMA
//
// MessageText:
//
// Conflicting schema element state: a class marked for modification was found in a schema to be added.
//
#define M_KGORA_SCHEMA_MODIFIED_CLASS_IN_ADDED_SCHEMA 0x00000FB7L

//
// MessageId: M_KGORA_SCHEMA_UNCHANGED_CLASS_IN_ADDED_SCHEMA
//
// MessageText:
//
// Conflicting schema element state: a class marked unchanged was found in a schema to be added.
//
#define M_KGORA_SCHEMA_UNCHANGED_CLASS_IN_ADDED_SCHEMA 0x00000FB8L

//
// MessageId: M_KGORA_OVERRIDE_FILENAME_NOT_QUALIFIED
//
// MessageText:
//
// The King.Oracle filename override '%1$ls' is not a fully-qualified path.
//
#define M_KGORA_OVERRIDE_FILENAME_NOT_QUALIFIED 0x00000FB9L

//
// MessageId: M_KGORA_UNSUPPORTED_CLASSTYPE
//
// MessageText:
//
// The '%1$ls' class type is not supported by King.Oracle.
//
#define M_KGORA_UNSUPPORTED_CLASSTYPE    0x00000FBAL

//
// MessageId: M_KGORA_COLUMN_WIDTH_INVALID
//
// MessageText:
//
// The value '%1$d' is invalid for the column width (must be between 0 and 255).
//
#define M_KGORA_COLUMN_WIDTH_INVALID     0x00000FBBL

//
// MessageId: M_KGORA_COLUMN_SCALE_INVALID
//
// MessageText:
//
// The value '%1$d' is invalid for the column scale (must be between 0 and 255).
//
#define M_KGORA_COLUMN_SCALE_INVALID     0x00000FBCL

//
// MessageId: M_KGORA_CLASS_NAME_INVALID
//
// MessageText:
//
// The class name '%1$ls' is invalid.
//
#define M_KGORA_CLASS_NAME_INVALID       0x00000FBDL

//
// MessageId: M_KGORA_APPLY_CONFIGURED
//
// MessageText:
//
// Apply schema after configuration or override is not allowed.
//
#define M_KGORA_APPLY_CONFIGURED         0x00000FBEL

//
// MessageId: M_KGORA_NOT_FOUND
//
// MessageText:
//
// King.Oracle file '%1$ls' was not found.
// 
//
#define M_KGORA_NOT_FOUND                0x00000FBFL

//
// MessageId: M_KGORA_INVALID_PHYSICAL_SCHEMA_PROVIDER_NAME
//
// MessageText:
//
// Invalid Physical Schema Mapping Provider Name.
//
#define M_KGORA_INVALID_PHYSICAL_SCHEMA_PROVIDER_NAME 0x00000FC1L

//
// MessageId: M_KGORA_INVALID_PHYSICAL_SCHEMA_PROVIDER_VERSION
//
// MessageText:
//
// Invalid Physical Schema Mapping Provider Version.
//
#define M_KGORA_INVALID_PHYSICAL_SCHEMA_PROVIDER_VERSION 0x00000FC2L

//
// MessageId: M_KGORA_DUPLICATE_COLUMN
//
// MessageText:
//
// Duplicate field name '%1$ls' in '%2$ls'.
//
#define M_KGORA_DUPLICATE_COLUMN         0x00000FC3L

//
// MessageId: M_KGORA_CANT_MODIFY_NON_EMPTY_CLASS
//
// MessageText:
//
// Can't modify class '%1$ls' because it contains data.
//
#define M_KGORA_CANT_MODIFY_NON_EMPTY_CLASS 0x00000FC4L

//
// MessageId: M_KGORA_CANT_DELETE_NON_EMPTY_CLASS
//
// MessageText:
//
// Can't delete class '%1$ls' because it contains data.
//
#define M_KGORA_CANT_DELETE_NON_EMPTY_CLASS 0x00000FC5L

//
// MessageId: M_KGORA_CANT_DELETE_NON_EMPTY_SCHEMA
//
// MessageText:
//
// Can't delete schema '%1$ls' because class '%2$ls' contains data.
//
#define M_KGORA_CANT_DELETE_NON_EMPTY_SCHEMA 0x00000FC6L

//************************************************************************************
//************************************************************************************
//** VERSION-RELATED MESSAGES 
//************************************************************************************
//************************************************************************************
//************************************************************************************
//************************************************************************************
//** FILTERS & EXPRESSIONS MESSAGES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_EXPRESSION_NULL_PARAMETER
//
// MessageText:
//
// Null parameter.
//
#define M_KGORA_EXPRESSION_NULL_PARAMETER 0x00001771L

//
// MessageId: M_KGORA_EXPRESSION_INVALID_PARAMETER_TYPE
//
// MessageText:
//
// Invalid parameter type.
//
#define M_KGORA_EXPRESSION_INVALID_PARAMETER_TYPE 0x00001772L

//************************************************************************************
//************************************************************************************
//** FEATURE COMMAND MESSAGES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_COMMAND_PARAMETERS_NOT_SUPPORTED
//
// MessageText:
//
// Command parameters are not supported.
//
#define M_KGORA_COMMAND_PARAMETERS_NOT_SUPPORTED 0x00001B59L

//
// MessageId: M_KGORA_COMMAND_TIMEOUT_NOT_SUPPORTED
//
// MessageText:
//
// Command timeout is not supported.
// 
//
#define M_KGORA_COMMAND_TIMEOUT_NOT_SUPPORTED 0x00001B5AL

//
// MessageId: M_KGORA_COMMAND_PROPERTY_NOT_FOUND
//
// MessageText:
//
// Property '%1$ls' is not part of class '%2$ls'.
//
#define M_KGORA_COMMAND_PROPERTY_NOT_FOUND 0x00001B5CL

//
// MessageId: M_KGORA_VALUE_TYPE_MISMATCH
//
// MessageText:
//
// Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.
//
#define M_KGORA_VALUE_TYPE_MISMATCH      0x00001B5DL

//
// MessageId: M_KGORA_FEATID_READONLY
//
// MessageText:
//
// The property '%1$ls' is read-only.
//
#define M_KGORA_FEATID_READONLY          0x00001B5EL

//
// MessageId: M_KGORA_INVALID_DATA_TYPE
//
// MessageText:
//
// The value for property '%1$ls' is not '%2$ls'.
//
#define M_KGORA_INVALID_DATA_TYPE        0x00001B5FL

//
// MessageId: M_KGORA_INVALID_GEOMETRY_TYPE
//
// MessageText:
//
// The geometry type ('%1$ls') of the feature conflicts with the shape file type ('%2$ls').
//
#define M_KGORA_INVALID_GEOMETRY_TYPE    0x00001B60L

//
// MessageId: M_KGORA_LITERAL_GEOM_VALUES_ONLY
//
// MessageText:
//
// Only literal geometry values supported.
//
#define M_KGORA_LITERAL_GEOM_VALUES_ONLY 0x00001B61L

//
// MessageId: M_KGORA_DELETE_SETUP_FAILED
//
// MessageText:
//
// Failed to initialize the delete operation.
//
#define M_KGORA_DELETE_SETUP_FAILED      0x00001B62L

//
// MessageId: M_KGORA_DELETE_FAILED
//
// MessageText:
//
// Delete failed.
//
#define M_KGORA_DELETE_FAILED            0x00001B63L

//
// MessageId: M_KGORA_ILLEGAL_COLUMN_ASSIGNMENT
//
// MessageText:
//
// Illegal column assignment.
//
#define M_KGORA_ILLEGAL_COLUMN_ASSIGNMENT 0x00001B64L

//
// MessageId: M_KGORA_VALUE_TOO_WIDE_FOR_COLUMN
//
// MessageText:
//
// The value '%1$ls' is too wide for column '%2$ls'.
//
#define M_KGORA_VALUE_TOO_WIDE_FOR_COLUMN 0x00001B65L

//
// MessageId: M_KGORA_UPDATE_SETUP_FAILED
//
// MessageText:
//
// Failed to initialize the update operation.
//
#define M_KGORA_UPDATE_SETUP_FAILED      0x00001B66L

//
// MessageId: M_KGORA_READ_ONLY_UPDATE
//
// MessageText:
//
// Cannot update read-only file '%1$ls'.
//
#define M_KGORA_READ_ONLY_UPDATE         0x00001B67L

//
// MessageId: M_KGORA_READ_ONLY_INSERT
//
// MessageText:
//
// Cannot insert into read-only file '%1$ls'.
//
#define M_KGORA_READ_ONLY_INSERT         0x00001B68L

//
// MessageId: M_KGORA_READ_ONLY_DELETE
//
// MessageText:
//
// Cannot delete from read-only file '%1$ls'.
//
#define M_KGORA_READ_ONLY_DELETE         0x00001B69L

//************************************************************************************
//************************************************************************************
//** LOCK-RELATED MESSAGES
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_LOCKING_NOT_SUPPORTED
//
// MessageText:
//
// Locking not supported (%1$ls).
//
#define M_KGORA_LOCKING_NOT_SUPPORTED    0x00001F41L

//************************************************************************************
//************************************************************************************
//** SPATIAL CONTEXT MESSAGES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_SPATIALCONTEXT_INVALIDCONFIGURATION
//
// MessageText:
//
// Cannot have more than one spatial context in a configuration file.
//
#define M_KGORA_SPATIALCONTEXT_INVALIDCONFIGURATION 0x00002329L

//
// MessageId: M_KGORA_SPATIALCONTEXT_INVALIDOPTION
//
// MessageText:
//
// Unsupported option.
//
#define M_KGORA_SPATIALCONTEXT_INVALIDOPTION 0x0000232AL

//************************************************************************************
//************************************************************************************
//** TRANSACTION MESSAGES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_CONNECTION_TRANSACTIONS_NOT_SUPPORTED
//
// MessageText:
//
// King.Oracle Provider does not support transactions.
//
#define M_KGORA_CONNECTION_TRANSACTIONS_NOT_SUPPORTED 0x00002711L

//************************************************************************************
//************************************************************************************
//** READER MESSAGES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_READER_NOT_READY
//
// MessageText:
//
// '%1$ls' reader not ready.
//
#define M_KGORA_READER_NOT_READY         0x00002AF9L

//
// MessageId: M_KGORA_READER_EXHAUSTED
//
// MessageText:
//
// '%1$ls' reader exhausted.
//
#define M_KGORA_READER_EXHAUSTED         0x00002AFAL

//
// MessageId: M_KGORA_READER_PROPERTY_NULL
//
// MessageText:
//
// The property '%1$ls' is NULL.
//
#define M_KGORA_READER_PROPERTY_NULL     0x00002AFBL

//************************************************************************************
//************************************************************************************
//** Spatial Index related messages 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_SI_UNABLE_TO_OPEN
//
// MessageText:
//
// The file '%1$ls' cannot be opened.
//
#define M_KGORA_SI_UNABLE_TO_OPEN        0x00002EE1L

//
// MessageId: M_KGORA_SI_NOT_AN_SSI
//
// MessageText:
//
// Corrupted Spatial Index file '%1$ls'.
//
#define M_KGORA_SI_NOT_AN_SSI            0x00002EE2L

//
// MessageId: M_KGORA_SI_READ_ERROR
//
// MessageText:
//
// Error reading the file '%1$ls'.
//
#define M_KGORA_SI_READ_ERROR            0x00002EE3L

//
// MessageId: M_KGORA_SI_WRITE_ERROR
//
// MessageText:
//
// Error writing the file '%1$ls'.
//
#define M_KGORA_SI_WRITE_ERROR           0x00002EE4L

//
// MessageId: M_KGORA_SI_VERSION_MISMATCH
//
// MessageText:
//
// Version mismatch '%1$ls', expected %2$d, found %3$d.
//
#define M_KGORA_SI_VERSION_MISMATCH      0x00002EE5L

//
// MessageId: M_KGORA_SI_READ_ONLY
//
// MessageText:
//
// File '%1$ls' is read-only.
//
#define M_KGORA_SI_READ_ONLY             0x00002EE6L

//
// MessageId: M_KGORA_SI_SEARCH_NOT_INITIALIZED
//
// MessageText:
//
// Spatial Index search has not been initialized.
//
#define M_KGORA_SI_SEARCH_NOT_INITIALIZED 0x00002EE7L

//
// MessageId: M_KGORA_SI_INVALID_PRECISION
//
// MessageText:
//
// Invalid floating point precision (%2$d).
//
#define M_KGORA_SI_INVALID_PRECISION     0x00002EE8L

//
// MessageId: M_KGORA_SI_OBJECT_NOT_FOUND
//
// MessageText:
//
// Specified object was not found in Spatial Index.
//
#define M_KGORA_SI_OBJECT_NOT_FOUND      0x00002EE9L

//
// MessageId: M_KGORA_SI_CANT_CREATE_TEMP_FILE
//
// MessageText:
//
// Cannot create temporary file while defragmenting the Spatial Index.
//
#define M_KGORA_SI_CANT_CREATE_TEMP_FILE 0x00002EEAL

//
// MessageId: M_KGORA_SI_TEMP_FILE_WRITE_ERROR
//
// MessageText:
//
// Cannot write temporary file while defragmenting the Spatial Index.
//
#define M_KGORA_SI_TEMP_FILE_WRITE_ERROR 0x00002EEBL

//
// MessageId: M_KGORA_SI_CANT_REPLACE_SSI
//
// MessageText:
//
// Cannot replace the Spatial Index.
//
#define M_KGORA_SI_CANT_REPLACE_SSI      0x00002EECL

//
// MessageId: M_KGORA_SI_OPERATOR_NOT_SUPPORTED
//
// MessageText:
//
// The spatial operation %1$ls is not supported.
//
#define M_KGORA_SI_OPERATOR_NOT_SUPPORTED 0x00002EEDL

//
// MessageId: M_KGORA_SI_INITIALIZATION_FAILED
//
// MessageText:
//
// Error #%1$d occured initializating the RTree.
//
#define M_KGORA_SI_INITIALIZATION_FAILED 0x00002EEEL

//
// MessageId: M_KGORA_SI_TRAVERSAL_NOT_INITIALIZED
//
// MessageText:
//
// Spatial Index traversal has not been initialized.
//
#define M_KGORA_SI_TRAVERSAL_NOT_INITIALIZED 0x00002EEFL

//
// MessageId: M_KGORA_SI_INVALID_MINIMUM_NODES
//
// MessageText:
//
// Invalid minimum entries per node (%2$d).
//
#define M_KGORA_SI_INVALID_MINIMUM_NODES 0x00002EF0L

//
// MessageId: M_KGORA_SI_INVALID_MAXIMUM_NODES
//
// MessageText:
//
// Invalid maximum entries per node (%2$d).
//
#define M_KGORA_SI_INVALID_MAXIMUM_NODES 0x00002EF1L

//************************************************************************************
//************************************************************************************
//** GEOMETRY MESSAGES 
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_CREATE_GEOMETRY_FAILED
//
// MessageText:
//
// Geometry creation failed for '%1$ls'.
//
#define M_KGORA_CREATE_GEOMETRY_FAILED   0x000032C9L

//************************************************************************************
//************************************************************************************
//** CONNECTION INFORMATIONS
//************************************************************************************
//************************************************************************************
//
// MessageId: M_KGORA_PROVIDER_DISPLAY_NAME
//
// MessageText:
//
// KingOracle FDO Provider for Oracle Spatial
//
#define M_KGORA_PROVIDER_DISPLAY_NAME    0x000036B1L

//
// MessageId: M_KGORA_PROVIDER_DESCRIPTION
//
// MessageText:
//
// Read/write access to spatial and attribute data in an Oracle Spatial.
//
#define M_KGORA_PROVIDER_DESCRIPTION     0x000036B2L

