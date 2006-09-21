;//
;//Copyright (C) 2004-2006  Autodesk, Inc.
;//
;//This library is free software; you can redistribute it and/or
;//modify it under the terms of version 2.1 of the GNU Lesser
;//General Public License as published by the Free Software Foundation.
;//
;//This library is distributed in the hope that it will be useful,
;//but WITHOUT ANY WARRANTY; without even the implied warranty of
;//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;//Lesser General Public License for more details.
;//
;//You should have received a copy of the GNU Lesser General Public
;//License along with this library; if not, write to the Free Software
;//Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
;//


;//************************************************************************************
;//************************************************************************************
;//** GENERIC MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=1000
SymbolicName=ARCSDE_UNEXPECTED_ERROR_ARG
Language=English
Unexpected error encountered in ArcSDE Provider (%1$ls).
.
MessageId=1001
SymbolicName=ARCSDE_UNEXPECTED_ERROR
Language=English
Unexpected error encountered in ArcSDE Provider.
.
MessageId=1002
SymbolicName=ARCSDE_NULL_ARGUMENT
Language=English
A required argument was set to NULL.
.
MessageId=1003
SymbolicName=ARCSDE_MEMORY_FAILURE
Language=English
Failed to allocate memory.
.
MessageId=1004
SymbolicName=ARCSDE_INDEX_OUT_OF_RANGE
Language=English
The index '%1$d' is out of range.
.
MessageId=1005
SymbolicName=ARCSDE_RELATION_DIRECTIVES_NOT_SUPPORTED
Language=English
Relation directives are not supported.
.
MessageId=1006
SymbolicName=ARCSDE_COMMAND_TIMEOUT_NOT_SUPPORTED
Language=English
Command timeout is not supported.
.
MessageId=1007
SymbolicName=ARCSDE_UNSUPPORTED_DATATYPE_BOOLEAN
Language=English
The Boolean data type is not supported by ArcSDE.
.
MessageId=1008
SymbolicName=ARCSDE_UNSUPPORTED_DATATYPE_BYTE
Language=English
The Byte data type is not supported by ArcSDE.
.
MessageId=1009
SymbolicName=ARCSDE_UNSUPPORTED_DATATYPE_INT64
Language=English
The Int64 data type is not supported by ArcSDE.
.
MessageId=1010
SymbolicName=ARCSDE_UNSUPPORTED_DATATYPE_DECIMAL
Language=English
The Decimal data type is not supported by ArcSDE.
.
MessageId=1011
SymbolicName=ARCSDE_UNSUPPORTED_DATATYPE_CLOB
Language=English
The CLOB data type is not supported by ArcSDE.
.
MessageId=1012
SymbolicName=ARCSDE_USER_UNKNOWN
Language=English
Cannot determine current user.
.
MessageId=1013
SymbolicName=ARCSDE_COMMAND_PARAMETERS_NOT_SUPPORTED
Language=English
Command parameters are not supported.
.
MessageId=1014
SymbolicName=ARCSDE_BIND_COLUMN_FAILED
Language=English
Failed to bind column '%1$ls'.
.
MessageId=1015
SymbolicName=ARCSDE_FEATURE_CLASS_UNSPECIFIED
Language=English
Feature class name not specified.
.
MessageId=1016
SymbolicName=ARCSDE_CONFIGURATION_FILE_NOT_SUPPORTED
Language=English
ArcSDE Provider does not support configuration files.
.
MessageId=1019
SymbolicName=ARCSDE_DATATYPE_UNHANDLED
Language=English
The FDO DataType %1$d is unsupported.
.
MessageId=1020
SymbolicName=ARCSDE_COMMAND_NOT_SUPPORTED
Language=English
The command '%1$d' is not supported.
.
MessageId=1021
SymbolicName=ARCSDE_QUALIFIED_TABLENAME_FAILURE
Language=English
Failed to make a qualified table name from table name '%1$ls' and schema name '%2$ls'.
.
MessageId=1022
SymbolicName=ARCSDE_IDENTIFIER_SCOPES_UNSUPPORTED
Language=English
FDO Identifier scopes are not supported; detected scopes in identifier '%1$ls'.
.
MessageId=1023
SymbolicName=ARCSDE_OPERATION_UNSUPPORTED
Language=English
This operation is not supported.
.
MessageId=1024
SymbolicName=ARCSDE_PROPERTY_NOT_FOUND
Language=English
The property '%1$ls' was not found.
.
MessageId=1025
SymbolicName=ARCSDE_PROPERTY_UNEXPECTED_TYPE
Language=English
The property '%1$ls' was not of the expect type '%2$ls'.
.
MessageId=1026
SymbolicName=ARCSDE_GEOMETRY_CONVERSION_SHAPE_TO_FGF
Language=English
Error encountered while converting ArcSDE shape to FGF.
.
MessageId=1027
SymbolicName=ARCSDE_SQL_COMMAND_MISSING_STATEMENT
Language=English
No SQL statement specified for execution by the SQL Command.
.
MessageId=1028
SymbolicName=ARCSDE_SQL_COMMAND_PREPARE_FAILED
Language=English
The given SQL statement '%1$ls' could not be prepared by ArcSDE.
.
MessageId=1029
SymbolicName=ARCSDE_SQL_COMMAND_EXECUTE_FAILED
Language=English
The given SQL statement '%1$ls' failed to execute.
.
MessageId=1030
SymbolicName=ARCSDE_UNSUPPORTED_DATATYPE_RASTER
Language=English
Raster properties are not supported.
.
MessageId=1031
SymbolicName=ARCSDE_ERROR_FILE_LINE
Language=English
Error occurred in file %1$ls, at line %2$ld.
.
MessageId=1032
SymbolicName=ARCSDE_UNSUPPORTED_GEOMETRY_TYPE
Language=English
The geometry type '%1$d' is not supported by ArcSDE.
.




;//************************************************************************************
;//************************************************************************************
;//** CONNECTION MESSAGES 
;//************************************************************************************
;//************************************************************************************


MessageId=2001
SymbolicName=ARCSDE_OPEN_FAILED
Language=English
Failed to open ArcSDE connection.
.
MessageId=2002
SymbolicName=ARCSDE_CONNECTION_NOT_ESTABLISHED
Language=English
Connection not established (NULL).
.
MessageId=2003
SymbolicName=ARCSDE_CONNECTION_INVALID
Language=English
Connection is invalid.
.
MessageId=2004
SymbolicName=ARCSDE_CONNECTION_PROPERTY_SERVER
Language=English
Server
.
MessageId=2005
SymbolicName=ARCSDE_CONNECTION_PROPERTY_INSTANCE
Language=English
Instance
.
MessageId=2006
SymbolicName=ARCSDE_CONNECTION_PROPERTY_DATASTORE
Language=English
Datastore
.
MessageId=2007
SymbolicName=ARCSDE_CONNECTION_PROPERTY_USERNAME
Language=English
Username
.
MessageId=2008
SymbolicName=ARCSDE_CONNECTION_PROPERTY_PASSWORD
Language=English
Password
.
MessageId=2009
SymbolicName=ARCSDE_CONNECTION_NOT_OPEN
Language=English
The connection is currently closed; call the Open method to establish a connection to ArcSDE before attempting any further operations.
.
MessageId=2010
SymbolicName=ARCSDE_CONNECTION_PENDING
Language=English
The connection is currently pending; call the Open method again with all required connection properties before attempting any further operations.
.
MessageId=2011
SymbolicName=ARCSDE_CONNECTION_MISSING_REQUIRED_PROPERTY
Language=English
The connection property '%1$ls' is required but wasn't specified.
.
MessageId=2013
SymbolicName=ARCSDE_CONNECTION_ALREADY_OPEN
Language=English
The connection is already open.
.
MessageId=2014
SymbolicName=ARCSDE_CONNECTION_TIMEOUT_UNSUPPORTED
Language=English
Connection timeout is not supported.
.
MessageId=2015
SymbolicName=ARCSDE_CONNECTION_STRING_NOT_SET
Language=English
Connection string is empty or was not set.
.
MessageId=2016
SymbolicName=ARCSDE_CONNECTION_REQUIRED_PROPERTY_NULL
Language=English
The required connection property '%1$ls' cannot be set to NULL.
.
MessageId=2017
SymbolicName=ARCSDE_CONNECTION_ENUM_PROPERTY_WRONG_VALUE
Language=English
The given value '%1$ls' for enumerable property '%2$ls' is not in the enumeration value collection.
.
MessageId=2018
SymbolicName=ARCSDE_CONNECTION_PROPERTY_NOT_FOUND
Language=English
The connection property '%1$ls' was not found.
.
MessageId=2019
SymbolicName=ARCSDE_RUNTIME_NOT_FOUND
Language=English
The ArcSDE runtime was not found.
.
MessageId=2020
SymbolicName=ARCSDE_RDMS_NOT_SUPPORTED
Language=English
The RDBMS '%1$ls' is not supported by this version of ArcSDE Provider.
.
MessageId=2021
SymbolicName=ARCSDE_RUNTIME_NOT_FOUND_MODULE
Language=English
The ArcSDE runtime was not found (module %1$ls).
.
MessageId=2022
SymbolicName=ARCSDE_RUNTIME_NOT_FOUND_PROC
Language=English
The ArcSDE runtime was not found (procedure %1$ls in module %2$ls).
.
MessageId=2023
SymbolicName=ARCSDE_INVALID_CONNECTION_STRING
Language=English
Invalid connection string '%1$ls'
.
MessageId=2024
SymbolicName=ARCSDE_INVALID_CONNECTION_PROPERTY_NAME
Language=English
Invalid connection property name '%1$ls'
.

;//************************************************************************************
;//************************************************************************************
;//** SCHEMA-RELATED MESSAGES 
;//************************************************************************************
;//************************************************************************************


MessageId=3001
SymbolicName=ARCSDE_INVALID_PROPERTY_NAME
Language=English
The given property name was invalid.
.
MessageId=3002
SymbolicName=ARCSDE_UNSUPPORTED_METADATA_VALUE
Language=English
Unsupported metadata value encountered.
.
MessageId=3003
SymbolicName=ARCSDE_CANT_OVERRIDE_IDENTITY_READONLY
Language=English
The read-only override cannot be applied to identity properties.
.
MessageId=3004
SymbolicName=ARCSDE_OBJECT_PROPERTIES_NOT_SUPPORTED
Language=English
Object properties are not supported.
.
MessageId=3005
SymbolicName=ARCSDE_FEATURE_CLASS_NOT_FOUND
Language=English
Feature class '%1$ls' not found in schema.
.
MessageId=3006
SymbolicName=ARCSDE_CLASS_NAME_EMPTY
Language=English
Unexpected empty or null FDO class name.
.
MessageId=3007
SymbolicName=ARCSDE_GENERATE_TABLE_NAME_FAILED
Language=English
Failed to generate a unique table name for class '%1$ls'.
.
MessageId=3008
SymbolicName=ARCSDE_GENERATE_COLUMN_NAME_FAILED
Language=English
Failed to generate a unique column name for property '%1$ls' of class '%2$ls'.
.
MessageId=3009
SymbolicName=ARCSDE_SCHEMA_DOES_NOT_EXIST
Language=English
Schema '%1$ls' does not exist.
.
MessageId=3010
SymbolicName=ARCSDE_SCHEMA_NOT_SPECIFIED
Language=English
Schema not specified.
.
MessageId=3011
SymbolicName=ARCSDE_SCHEMA_MULTIPLE_ID_PROPERTIES
Language=English
ArcSDE Provider does not support classes with more than one identity property.
.
MessageId=3012
SymbolicName=ARCSDE_SCHEMA_FEATURECLASS_NO_ID_PROPERTIES
Language=English
A feature class cannot have zero identity properties.
.
MessageId=3013
SymbolicName=ARCSDE_SCHEMA_UNSUPPORTED_ID_TYPE
Language=English
Only data properties are supported as identity properties.
.
MessageId=3014
SymbolicName=ARCSDE_SCHEMA_UNSUPPORTED_ID_NULLABLE
Language=English
Identity properties cannot be nullable.
.
MessageId=3015
SymbolicName=ARCSDE_SCHEMA_ADD_CLASS_FAILED
Language=English
Failed to add class '%1$ls'.
.
MessageId=3016
SymbolicName=ARCSDE_SCHEMA_ADD_LAYER_FAILED
Language=English
Failed to create layer (spatial column) for class '%1$ls'.
.
MessageId=3017
SymbolicName=ARCSDE_SCHEMA_MISSING_SPATIAL_INDEX
Language=English
Missing spatial index on layer (spatial column) for class '%1$ls'.
.
MessageId=3018
SymbolicName=ARCSDE_SCHEMA_WRONG_AUTOGENERATED_ID_TYPE
Language=English
ArcSDE Provider only supports auto-generated identity properties of type Int32.
.
MessageId=3019
SymbolicName=ARCSDE_SCHEMA_DELETE_CLASS_FAILED
Language=English
Failed to delete class '%1$ls'.
.
MessageId=3020
SymbolicName=ARCSDE_SCHEMA_MODIFY_CLASS_UNSUPPORTED
Language=English
ArcSDE Provider does not support modifying a class definition.
.
MessageId=3021
SymbolicName=ARCSDE_SCHEMA_DELETED_CLASS_IN_ADDED_SCHEMA
Language=English
Conflicting schema element state: a class marked for deletion was found in a schema to be added.
.
MessageId=3022
SymbolicName=ARCSDE_SCHEMA_UNCHANGED_CLASS_IN_ADDED_SCHEMA
Language=English
Conflicting schema element state: a class marked unchanged was found in a schema to be added.
.
MessageId=3023
SymbolicName=ARCSDE_SCHEMA_UNSUPPORTED_ELEMENT_STATE
Language=English
Schema element state '%1$d' is not supported.
.
MessageId=3024
SymbolicName=ARCSDE_SCHEMA_NON_UNIQUE_CLASS_NAME
Language=English
The unqualified class name '%1$ls' was found in more than one schema.
.
MessageId=3025
SymbolicName=ARCSDE_SCHEMA_LOCKING_INCORRECT_IDENTITY
Language=English
The class '%1$ls' has locking enabled but no autogenerated identity property; this is not supported by ArcSDE.
.
MessageId=3026
SymbolicName=ARCSDE_SCHEMA_VERSIONNING_INCORRECT_IDENTITY
Language=English
The class '%1$ls' has versionning enabled but no autogenerated identity property; this is not supported by ArcSDE.
.
MessageId=3027
SymbolicName=ARCSDE_SCHEMA_LOCKING_AND_VERSIONNING
Language=English
The class '%1$ls' has both versionning and locking enabled; this is not supported by ArcSDE.
.
MessageId=3028
SymbolicName=ARCSDE_SCHEMA_UNIQUE_INDEX_CREATION_FAILED
Language=English
Failed to create unique index on class '%1$ls'.
.
MessageId=3029
SymbolicName=ARCSDE_SCHEMA_GET_INDEX_FAILED
Language=English
Failed to read index(es) on class '%1$ls'.
.




;//************************************************************************************
;//************************************************************************************
;//** VERSION-RELATED MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=4001
SymbolicName=ARCSDE_VERSION_NULL
Language=English
Version name cannot be NULL.
.
MessageId=4002
SymbolicName=ARCSDE_VERSION_INFO_ALLOC
Language=English
Cannot initialize SE_VERSIONINFO structure.
.
MessageId=4003
SymbolicName=ARCSDE_VERSION_INFO
Language=English
Version info for '%1$ls' could not be retrieved.
.
MessageId=4004
SymbolicName=ARCSDE_VERSION_INFO_ITEM
Language=English
Version info item '%1$ls' could not be set and/or retrieved.
.
MessageId=4005
SymbolicName=ARCSDE_VERSION_CREATE_FAILED
Language=English
Cannot create version '%1$ls'.
.
MessageId=4007
SymbolicName=ARCSDE_VERSION_INFO_LIST
Language=English
Version info list could not be retrieved.
.
MessageId=4008
SymbolicName=ARCSDE_NO_ACTIVE_TRANSACTION
Language=English
No long transaction is active.
.
MessageId=4009
SymbolicName=ARCSDE_COULD_NOT_FETCH_STATE
Language=English
State id could not be retrieved.
.
MessageId=4010
SymbolicName=ARCSDE_STREAM_NOT_VERSIONED
Language=English
Stream could not be set to version state.
.
MessageId=4011
SymbolicName=ARCSDE_STATE_INFO_ALLOC
Language=English
Cannot initialize SE_STATEINFO structure.
.
MessageId=4012
SymbolicName=ARCSDE_STATE_INFO
Language=English
State info for '%1$d' could not be retrieved.
.
MessageId=4013
SymbolicName=ARCSDE_STATE_CLOSE
Language=English
Cannot close state '%1$d'.
.
MessageId=4014
SymbolicName=ARCSDE_STATE_CREATE
Language=English
Cannot create state.
.
MessageId=4015
SymbolicName=ARCSDE_STATE_INFO_ITEM
Language=English
State info item '%1$ls' could not be retrieved.
.
MessageId=4016
SymbolicName=ARCSDE_STATE_OPEN
Language=English
Cannot open state '%1$d'.
.
MessageId=4017
SymbolicName=ARCSDE_VERSION_STATE
Language=English
Cannot change the version state.
.
MessageId=4018
SymbolicName=ARCSDE_STATE_DELETE
Language=English
Cannot delete state.
.
MessageId=4019
SymbolicName=ARCSDE_VERSION_EXISTS
Language=English
Version '%1$ls' already exists.
.
MessageId=4020
SymbolicName=ARCSDE_VERSION_DELETE_FAILED
Language=English
Cannot delete version '%1$ls'.
.
MessageId=4021
SymbolicName=ARCSDE_VERSION_NAME_TOO_LONG
Language=English
Version name exceeds SE_MAX_VERSION_INPUT_LEN characters.
.
MessageId=4022
SymbolicName=ARCSDE_VERSION_DESCRIPTION_TOO_LONG
Language=English
Version description exceeds SE_MAX_DESCRIPTION_LEN characters.
.
MessageId=4023
SymbolicName=ARCSDE_INVALID_VERSION_NAME
Language=English
A version name cannot contain the character '.'.
.
MessageId=4024
SymbolicName=ARCSDE_UNIQUE_VERSION_NAME
Language=English
More than one version is named '%1$ls'.
.
MessageId=4025
SymbolicName=ARCSDE_MERGE_FAILURE
Language=English
Cannot merge changes from state '%1$d' into a new state '%2$d'.
.
MessageId=4026
SymbolicName=ARCSDE_STATE_LOCK_FAILURE
Language=English
State '%1$d' for version '%2$ls' could not be locked.
.
MessageId=4027
SymbolicName=ARCSDE_STATE_UNLOCK_FAILURE
Language=English
State '%1$d' for version '%2$ls' could not be unlocked.
.
MessageId=4028
SymbolicName=ARCSDE_VERSION_STATE_CHANGE_FAILURE
Language=English
Could not set state '%1$d' for version '%2$ls'.
.
MessageId=4029
SymbolicName=ARCSDE_NOT_OWNER
Language=English
User is not the version owner.
.
MessageId=4030
SymbolicName=ARCSDE_VERSION_HAS_CHILDREN
Language=English
Cannot commit a version that has child versions.
.
MessageId=4031
SymbolicName=ARCSDE_ILLEGAL_VERSION_NAME
Language=English
'%1$ls' is an illegal version name.
.

MessageId=4032
SymbolicName=ARCSDE_STATE_INFO_LIST
Language=English
State info list could not be retrieved.
.


;//************************************************************************************
;//************************************************************************************
;//** FILTERS & EXPRESSIONS MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=5001
SymbolicName=ARCSDE_FILTER_PARAMETERS_NOT_SUPPORTED
Language=English
Filter parameters are not supported.
.
MessageId=5002
SymbolicName=ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION
Language=English
Failed to process the given spatial condition.
.
MessageId=5003
SymbolicName=ARCSDE_SPATIAL_QUERY_ON_NONGEOMETRIC_PROPERTY
Language=English
Cannot perform a spatial query on non-geometric property '%1$ls'.
.
MessageId=5004
SymbolicName=ARCSDE_SPATIAL_DISTANCE_BUFER_NEGATIVE
Language=English
Cannot perform a spatial query with a negative distance filter.
.
MessageId=5005
SymbolicName=ARCSDE_SPATIAL_QUERY_ON_NONLITERAL_GEOMETRY
Language=English
ArcSDE cannot perform a spatial query against a non-literal geometry expression.
.
MessageId=5006
SymbolicName=ARCSDE_UNSUPPORTED_SPATIAL_OPERATION
Language=English
The given spatial operation is not supported.
.
MessageId=5007
SymbolicName=ARCSDE_UNSUPPORTED_FILTER_COMBINATION
Language=English
The filter provided cannot be handled by ArSDE Provider.  Spatial filters can only be applied in specific filter combinations.
.
MessageId=5008
SymbolicName=ARCSDE_UNSUPPORTED_BINARY_EXPRESSION
Language=English
The given binary expression is not supported.
.
MessageId=5009
SymbolicName=ARCSDE_UNSUPPORTED_UNARY_EXPRESSION
Language=English
The given unary expression is not supported.
.
MessageId=5010
SymbolicName=ARCSDE_UNSUPPORTED_BINARY_LOGICAL_OPERATOR
Language=English
The given binary logical operator is not supported.
.
MessageId=5011
SymbolicName=ARCSDE_UNSUPPORTED_UNARY_LOGICAL_OPERATOR
Language=English
The given unary logical operator is not supported.
.
MessageId=5012
SymbolicName=ARCSDE_UNSUPPORTED_COMPARISON_CONDITION
Language=English
The given comparison condition is not supported.
.
MessageId=5013
SymbolicName=ARCSDE_FILTER_BLOBS_UNSUPPORTED
Language=English
Literal BLOB expressions are not supported in filters.
.
MessageId=5014
SymbolicName=ARCSDE_FILTER_GEOMETRIES_UNSUPPORTED
Language=English
Literal geometry expressions are not supported in filters.
.
MessageId=5015
SymbolicName=ARCSDE_FUNCTION_NOT_FOUND_OR_WRONG_TYPE
Language=English
The given function '%1$ls' was not found or is of the wrong type.
.
MessageId=5016
SymbolicName=ARCSDE_FUNCTION_WRONG_NUM_ARGUMENTS
Language=English
Wrong number of arguments passed to function '%1$ls'.
.
MessageId=5017
SymbolicName=ARCSDE_FUNCTION_WRONG_ARG_TYPE
Language=English
Aggregate function argument was not of expected type '%1$ls'.
.
MessageId=5018
SymbolicName=ARCSDE_COMPUTEDID_NOT_AGGREGATE_FUNCTION
Language=English
ArcSDE Provider does not support computed identifiers which do not correspond to aggregate functions.
.
MessageId=5019
SymbolicName=ARCSDE_FUNCTION_STDDEV_DESC
Language=English
Returns the standard deviation of an expression.
.
MessageId=5020
SymbolicName=ARCSDE_FUNCTION_DOUBLE_ARGUMENT_DESC
Language=English
An argument that accepts any expression that evaluates to a double value.
.



;//************************************************************************************
;//************************************************************************************
;//** FEATURE COMMAND MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=6001
SymbolicName=ARCSDE_PROPERTY_NOT_IN_RESULT
Language=English
The property '%1$ls' is not in the query result.
.
MessageId=6002
SymbolicName=ARCSDE_READER_CLOSED
Language=English
Reader is closed.
.
MessageId=6003
SymbolicName=ARCSDE_READER_EXHAUSTED
Language=English
Reader is exhausted.
.
MessageId=6004
SymbolicName=ARCSDE_OFFSET_TOO_LARGE
Language=English
The given stream offset '%1$d' is too large.
.
MessageId=6005
SymbolicName=ARCSDE_OFFSET_NEGATIVE
Language=English
The given offset '%1$d' is negative.
.
MessageId=6006
SymbolicName=ARCSDE_INVALID_COUNT
Language=English
The count '%1$d' is invalid.  It must be either positive or -1 to read the entire stream.
.
MessageId=6007
SymbolicName=ARCSDE_COULDNT_SET_VALUE
Language=English
Failed to set value for column %1$d (property %2$ls) on table %3$ls (class %4$ls).
.
MessageId=6008
SymbolicName=ARCSDE_INSERT_UNEXPECTED_ERROR
Language=English
Unexpected error while performing insert.
.
MessageId=6009
SymbolicName=ARCSDE_VALUE_TYPE_MISMATCH
Language=English
Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.
.
MessageId=6010
SymbolicName=ARCSDE_COLUMN_NOT_IN_RESULT
Language=English
The column '%1$ls' is not in the query result.
.
MessageId=6011
SymbolicName=ARCSDE_UNHANDLED_PROPERTY_TYPE
Language=English
The property type '%1$d' is not supported.
.
MessageId=6012
SymbolicName=ARCSDE_CANNOT_SET_READONLY_PROPERTY
Language=English
Property '%1$ls' cannot be set because it is read-only.
.
MessageId=6013
SymbolicName=ARCSDE_MISSING_DEFAULT_VALUE
Language=English
Read-only property '%1$ls' requires a default value.
.
MessageId=6014
SymbolicName=ARCSDE_CANNOT_DEFAULT_READONLY_PROPERTY
Language=English
Read-only identity property '%1$ls' cannot have a default value.
.
MessageId=6015
SymbolicName=ARCSDE_STREAM_FETCH
Language=English
Stream fetch failed.
.
MessageId=6016
SymbolicName=ARCSDE_STREAM_GET
Language=English
Stream get ('%1$ls') failed for column '%2$ls'.
.
MessageId=6017
SymbolicName=ARCSDE_READER_NOT_READY
Language=English
Must %1$ls prior to accessing reader.
.
MessageId=6018
SymbolicName=ARCSDE_SPATIAL_QUERY_ON_NONROWID_TABLE
Language=English
Cannot perform a spatial query (for insert, update, or delete) against ArcSDE table '%1$ls' since it has no rowid column.
.
MessageId=6019
SymbolicName=ARCSDE_GET_ROWID_FAILED
Language=English
Failed to retrieve the row id from the ArcSDE stream.
.
MessageId=6020
SymbolicName=ARCSDE_DELETE_BY_ID_FAILED
Language=English
Delete by id from table '%1$ls' failed.
.
MessageId=6021
SymbolicName=ARCSDE_DELETE_FAILED
Language=English
Delete from table '%1$ls' failed.
.
MessageId=6022
SymbolicName=ARCSDE_STREAM_NOT_INITIALIZED
Language=English
Stream not initialized.
.
MessageId=6023
SymbolicName=ARCSDE_STREAM_GENERAL_ERROR
Language=English
Error encountered while manipulating stream.
.
MessageId=6024
SymbolicName=ARCSDE_STREAM_GET_COLUMN_FAILED
Language=English
Failed to retrieve column information from the stream.
.
MessageId=6025
SymbolicName=ARCSDE_STREAM_GET_LAST_INSERTED_ID_FAILED
Language=English
Failed to retrieve the last inserted id from the stream.
.
MessageId=6026
SymbolicName=ARCSDE_NO_PROPERTY_VALUES
Language=English
No property values were specified for this operation.
.
MessageId=6027
SymbolicName=ARCSDE_UPDATE_BY_ID_FAILED
Language=English
Update table by ID failed.
.
MessageId=6028
SymbolicName=ARCSDE_UPDATE_FAILED
Language=English
Update table failed.
.
MessageId=6029
SymbolicName=ARCSDE_CONVERT_SHAPE_FAILED
Language=English
Failed to convert FGF geometry to ArcSDE shape.
.
MessageId=6030
SymbolicName=ARCSDE_SELECT_DISTINCT_TOO_MANY_PROPERTIES
Language=English
Select with 'distinct' option on %1$d properties failed; ArcSDE supports 'distinct' on only one property at a time.
.
MessageId=6031
SymbolicName=ARCSDE_SELECT_DISTINCT_INVALID_TYPE
Language=English
Select with 'distinct' option failed; ArcSDE doesn't support 'distinct' option with column types other than Int16, Int32, Single, Double, DateTime, and String.
.
MessageId=6032
SymbolicName=ARCSDE_SELECT_COMPUTEDIDENTIFIERS_NOT_SUPPORTED
Language=English
The ArcSDE Provider does not support computed identifiers in the select command.
.
MessageId=6033
SymbolicName=ARCSDE_SELECTAGGREGATES_NON_AGGREGATE_EXPR_NOT_SUPPORTED
Language=English
The ArcSDE Provider does not support non-aggregate functions or expresssions in the select aggregates command.
.
MessageId=6034
SymbolicName=ARCSDE_SELECTAGGREGATES_MIX_EXPR_NOT_SUPPORTED
Language=English
The ArcSDE Provider does not support combining aggregate functions with other identifiers or expresssions in the select aggregates command.
.
MessageId=6035
SymbolicName=ARCSDE_NULL_VALUE
Language=English
The value of property '%1$ls' is null.
.
MessageId=6036
SymbolicName=ARCSDE_SHAPE_CLIP_FAILED
Language=English
Failed to clip the given shape to the coordinate reference's extents.
.
MessageId=6037
SymbolicName=ARCSDE_SELECT_DISTINCT_TOO_MANY_RESULTS
Language=English
The number of distinct results exceeds the maximum imposed by ArcSDE Server; please update the value of MAXDISTINCT in the ArcSDE Server's GioMgr.defs file accordingly.
.


;//************************************************************************************
;//************************************************************************************
;//** ARCSDE-RELATED MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=7001
SymbolicName=ARCSDE_STREAM_ALLOC
Language=English
Cannot initialize SE_STREAM structure.
.
MessageId=7002
SymbolicName=ARCSDE_STREAM_QUERY
Language=English
Stream query failed.
.
MessageId=7003
SymbolicName=ARCSDE_STREAM_EXECUTE
Language=English
Stream execute failed.
.
MessageId=7004
SymbolicName=ARCSDE_STREAM_CLOSE
Language=English
Stream close failed.
.
MessageId=7005
SymbolicName=ARCSDE_STREAM_FREE
Language=English
Stream free failed.
.
MessageId=7006
SymbolicName=ARCSDE_REGISTRATION_INFO_ITEM
Language=English
Table registration info item '%1$ls' could not be retrieved.
.
MessageId=7007
SymbolicName=ARCSDE_TABLE_NAME_TOO_LONG
Language=English
Table name '%1$ls' is too long for ArcSDE.
.
MessageId=7008
SymbolicName=ARCSDE_REGISTRATION_INFO_CREATE
Language=English
Table registration info could not be created.
.
MessageId=7009
SymbolicName=ARCSDE_REGISTRATION_INFO_GET
Language=English
Table registration info could not be retrieved.
.
MessageId=7010
SymbolicName=ARCSDE_REGISTRATION_INFO_SET_ITEM
Language=English
Table registration info item '%1$ls' could not be set.
.
MessageId=7011
SymbolicName=ARCSDE_REGISTRATION_INFO_ALTER
Language=English
Registration info could not be altered.
.
MessageId=7012
SymbolicName=ARCSDE_OWNER_INVALID
Language=English
Owner name '%1$ls' is too long.
.
MessageId=7013
SymbolicName=ARCSDE_STREAM_UPDATE_ROW_FAILED
Language=English
Preparing stream for update row failed.
.
MessageId=7014
SymbolicName=ARCSDE_METADATA_CREATE_FAILED
Language=English
Failed to create ArcSDE metadata.
.
MessageId=7015
SymbolicName=ARCSDE_METADATA_ADD_FAILED
Language=English
Failed to add ArcSDE metadata.
.
MessageId=7016
SymbolicName=ARCSDE_METADATA_MANIPULATE_FAILED
Language=English
Failed to get or set ArcSDE metadata.
.
MessageId=7017
SymbolicName=ARCSDE_METADATA_GET_FAILED
Language=English
Failed to retrieve ArcSDE metadata.
.
MessageId=7018
SymbolicName=ARCSDE_CONNECTION_GET_FAILED
Language=English
Failed to retrieve ArcSDE connection.
.
MessageId=7019
SymbolicName=ARCSDE_CREATE_OBJECT_FAILED
Language=English
Failed to create ArcSDE object '%1$ls'.
.
MessageId=7020
SymbolicName=ARCSDE_GET_OBJECT_FAILED
Language=English
Failed to retrieve ArcSDE object '%1$ls'.
.
MessageId=7021
SymbolicName=ARCSDE_GET_TABLE_DIFFERENCE_LIST
Language=English
Failed to retrieve ArcSDE table difference list between two states.
.
MessageId=7022
SymbolicName=ARCSDE_PARSE_TABLE_NAME_FAILED
Language=English
Failed to parse the qualified name '%1$ls'.
.
MessageId=7023
SymbolicName=ARCSDE_MAKE_TABLE_NAME_FAILED
Language=English
Failed to make qualified name.
.
MessageId=7024
SymbolicName=ARCSDE_STREAM_STATISTICS_FAILED
Language=English
Calculating stream statistics failed.
.
MessageId=7035
SymbolicName=ARCSDE_QUERYINFO_MANIPULATION_FAILED
Language=English
Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.
.
MessageId=7036
SymbolicName=ARCSDE_OWNER_NAME_TOO_LONG
Language=English
Owner name '%1$ls' is too long for ArcSDE.
.
MessageId=7037
SymbolicName=ARCSDE_DATABASE_NAME_TOO_LONG
Language=English
Database name '%1$ls' is too long for ArcSDE.
.




;//************************************************************************************
;//************************************************************************************
;//** LOCK-RELATED MESSAGES
;//************************************************************************************
;//************************************************************************************

MessageId=8001
SymbolicName=ARCSDE_INVALID_LOCK_TYPE
Language=English
Invalid lock type ('%1$d').
.
MessageId=8002
SymbolicName=ARCSDE_LONG_TRANSACTION_EXCLUSIVE_LOCKS_NOT_SUPPORTED
Language=English
Lock type FdoLockType_LongTransactionExclusive is not supported.
.
MessageId=8003
SymbolicName=ARCSDE_INVALID_LOCK_STRATEGY
Language=English
Invalid lock strategy ('%1$d').
.
MessageId=8004
SymbolicName=ARCSDE_STREAM_LOCK
Language=English
Cannot set row locking on the stream.
.
MessageId=8005
SymbolicName=ARCSDE_GET_ROW_LOCK_LIST_FAILED
Language=English
Failed to get the row lock list.
.
MessageId=8006
SymbolicName=ARCSDE_LOCKING_NOT_ENABLED
Language=English
Table '%1$ls' is not lock enabled.
.
MessageId=8007
SymbolicName=ARCSDE_RELEASE_UNOWNED_LOCKS
Language=English
Releasing other owners locks is not supported.
.
MessageId=8008
SymbolicName=ARCSDE_TABLE_DOES_NOT_SUPPORT_ROW_LOCKS
Language=English
Table is not row-lockable.
.
MessageId=8009
SymbolicName=ARCSDE_LOCK_OWNER_UNKNOWN
Language=English
<Unknown lock owner>
.
MessageId=8010
SymbolicName=ARCSDE_LOCK_TYPE_NOT_SET
Language=English
The lock type must be set to a valid type.
.



;//************************************************************************************
;//************************************************************************************
;//** SPATIAL CONTEXT MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=9001
SymbolicName=ARCSDE_FAILED_TO_RETRIEVE_SRS
Language=English
Failed to retrieve requested ArcSDE Spatial Reference Systems.
.
MessageId=9002
SymbolicName=ARCSDE_FAILED_TO_READ_SRS
Language=English
Failed to get or set information for this ArcSDE Spatial Reference System.
.
MessageId=9003
SymbolicName=ARCSDE_SPATIALCONTEXT_NEED_EXTENT_TO_CREATE
Language=English
Cannot create a spatial context without specifying an extent.
.
MessageId=9004
SymbolicName=ARCSDE_SPATIALCONTEXT_NAME_NOT_SPECIFIED
Language=English
Spatial context name not specified.
.
MessageId=9005
SymbolicName=ARCSDE_SPATIALCONTEXT_ZTOLERANCE_ZERO
Language=English
A spatial context's Z tolerance can't be zero.
.
MessageId=9006
SymbolicName=ARCSDE_SPATIALCONTEXT_NAME_NOT_FOUND
Language=English
A spatial context of name '%1$ls' was not found.
.
MessageId=9007
SymbolicName=ARCSDE_SPATIALCONTEXT_DELETE_FAILED
Language=English
Failed to delete spatial context '%1$ls'.
.
MessageId=9008
SymbolicName=ARCSDE_SPATIALCONTEXT_UNSUPPORTED_EXTENTTYPE
Language=English
The given spatial context extent type is not supported.
.
MessageId=9009
SymbolicName=ARCSDE_SPATIALCONTEXT_NONEMPTY_CLASS
Language=English
Cannot change the spatial context of non-empty class '%1$ls'.
.
MessageId=9010
SymbolicName=ARCSDE_SPATIALCONTEXT_NAME_ALREADY_EXISTS
Language=English
A spatial context of name '%1$ls' already exists.
.
MessageId=9011
SymbolicName=ARCSDE_COORDREF_ERROR
Language=English
Unexpected error encountered while manipulating an ArcSDE coordinate reference.
.
MessageId=9012
SymbolicName=ARCSDE_SPATIALREF_ERROR
Language=English
Unexpected error encountered while manipulating an ArcSDE spatial reference system.
.
MessageId=9013
SymbolicName=ARCSDE_RETRIEVE_COORDSYS_FAILED
Language=English
Error while retrieving active coordinate reference system.
.
MessageId=9014
SymbolicName=ARCSDE_SPATIALREF_STORE_ERROR
Language=English
Failed to create or update an ArcSDE spatial reference system in the datastore.
.
MessageId=9015
SymbolicName=ARCSDE_LAYER_UPDATE_ERROR
Language=English
Failed to update the coordinate system of an ArcSDE layer (spatial column).
.
MessageId=9016
SymbolicName=ARCSDE_COORDSYS_ID_NOT_A_NUMBER
Language=English
The coordinate system '%1$ls' is not an integer that corresponds to an entry in ArSDE's Projection Engine.
.
MessageId=9017
SymbolicName=ARCSDE_SIMILAR_SPATIAL_CONTEXT_EXISTS
Language=English
The spatial context '%1$ls' being created matches the coordinate system, extents, and resolution of existing spatial context '%2$ls'; ArcSDE does not allow this.
.


;//************************************************************************************
;//************************************************************************************
;//** TRANSACTION MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=10001
SymbolicName=ARCSDE_TRANSACTION_START_FAILED
Language=English
Failed to start transaction.
.
MessageId=10002
SymbolicName=ARCSDE_TRANSACTION_SPENT
Language=English
This transaction has already been committed or rolled back.
.
MessageId=10003
SymbolicName=ARCSDE_TRANSACTION_COMMIT_FAILED
Language=English
Failed to commit this transaction.
.
MessageId=10004
SymbolicName=ARCSDE_TRANSACTION_ROLLBACK_FAILED
Language=English
Failed to rollback this transaction.
.
MessageId=10005
SymbolicName=ARCSDE_SET_TRANSACTION_NOT_SUPPORTED
Language=English
Setting the transaction is not supported.
.
MessageId=10006
SymbolicName=ARCSDE_NESTED_TRANSACTIONS_NOT_SUPPORTED
Language=English
Nested transactions are not supported.
.


;//************************************************************************************
;//************************************************************************************
;//** LOG FILE MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=11001
SymbolicName=ARCSDE_LOG_FILE_NAME
Language=English
Unable to generate log file name.
.
MessageId=11002
SymbolicName=ARCSDE_LOG_FILE_OPEN
Language=English
Unable to open log file.
.
MessageId=11003
SymbolicName=ARCSDE_LOG_FILE_ADD
Language=English
Unable to add an id to the log file.
.
MessageId=11004
SymbolicName=ARCSDE_LOG_FILE_QUERY
Language=English
Unable to query log file.
.
MessageId=11005
SymbolicName=ARCSDE_LOG_FILE_CLOSE
Language=English
Unable to close log file.
.
MessageId=11006
SymbolicName=ARCSDE_LOG_INFO_CREATE
Language=English
Unable to create LOGINFO object.
.
MessageId=11007
SymbolicName=ARCSDE_LOG_INFO_SET_ITEM
Language=English
Log info item '%1$ls' could not be set.
.
MessageId=11008
SymbolicName=ARCSDE_LOG_INFO_GET_ITEM
Language=English
Log info item '%1$ls' could not be retrieved.
.
MessageId=11009
SymbolicName=ARCSDE_LOG_INFO_GET
Language=English
Could not get log info.
.
MessageId=11010
SymbolicName=ARCSDE_LOG_SET_LOGFILE
Language=English
Could not set log file.
.
MessageId=11011
SymbolicName=ARCSDE_LOG_GET_IDS
Language=English
Could not get the list of ids from the log file.
.

;//************************************************************************************
;//************************************************************************************
;//** CONNECTION INFORMATIONS
;//************************************************************************************
;//************************************************************************************

MessageId=12001
SymbolicName=ARCSDE_PROVIDER_DISPLAY_NAME
Language=English
OSGeo FDO Provider for ArcSDE
.
MessageId=12002
SymbolicName=ARCSDE_PROVIDER_DESCRIPTION
Language=English
Read/write access to an ESRI ArcSDE-based data store, using Oracle and SQL Server.
.
