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

;//************************************************************************************
;//************************************************************************************
;//** CONNECTION MESSAGES 
;//************************************************************************************
;//************************************************************************************


MessageId=2001
SymbolicName=WFS_CONNECTION_TIMEOUT_UNSUPPORTED
Language=English
Connection timeout is not supported.
.
MessageId=2002
SymbolicName=WFS_CONNECTION_COMMAND_NOT_SUPPORTED
Language=English
The command %1$d is not supported.
.
MessageId=2003
SymbolicName=WFS_CONNECTION_CONFIGURATION_FILE_NOT_SUPPORTED
Language=English
WFS Provider does not support configuration files.
.
MessageId=2004
SymbolicName=WFS_CONNECTION_INVALID
Language=English
Connection is invalid.
.
MessageId=2005
SymbolicName=WFS_CONNECTION_REQUIRED_PROPERTY_NULL
Language=English
The required property '%1$ls' cannot be set to NULL.
.
MessageId=2006
SymbolicName=WFS_CONNECTION_ENUM_PROPERTY_WRONG_VALUE
Language=English
The given value '%1$ls' for enumerable property '%2$ls' is not in the enumeration value collection.
.
MessageId=2007
SymbolicName=WFS_CONNECTION_PROPERTY_NOT_FOUND
Language=English
The connection property '%1$ls' was not found.
.
MessageId=2008
SymbolicName=WFS_SCHEMA_OVERRIDES_NOT_SUPPORTED
Language=English
WFS Provider does not support schema overrides.
.
MessageId=2009
SymbolicName=WFS_INVALID_CONNECTION_STRING
Language=English
Invalid connection string '%1$ls'
.
MessageId=2010
SymbolicName=WFS_INVALID_CONNECTION_PROPERTY_NAME
Language=English
Invalid connection property name '%1$ls'.
.
MessageId=2011
SymbolicName=WFS_CONNECTION_CLOSED
Language=English
The Connection to the WFS provider is closed.
.

;//************************************************************************************
;//************************************************************************************
;//** CONNECTION PROPERTIES 
;//************************************************************************************
;//************************************************************************************

MessageId=3001
SymbolicName=WFS_CONNECTION_PROPERTY_FEATURESRVER
Language=English
FeatureServer
.
MessageId=3002
SymbolicName=WFS_CONNECTION_PROPERTY_USERNAME
Language=English
Username
.
MessageId=3003
SymbolicName=WFS_CONNECTION_PROPERTY_PASSWORD
Language=English
Password
.

;//************************************************************************************
;//************************************************************************************
;//** SCHEMA-RELATED MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=4001
SymbolicName=WFS_OBJECT_PROPERTIES_NOT_SUPPORTED
Language=English
Object properties are not supported.
.
MessageId=4002
SymbolicName=WFS_UNSUPPORTED_DATATYPE
Language=English
The '%1$ls' data type is not supported by WFS.
.
MessageId=4003
SymbolicName=WFS_NAMED_SCHEMA_NOT_FOUND
Language=English
Schema '%1$ls' was not found in the DataStore.
.
MessageId=4004
SymbolicName=WFS_NAMED_CLASS_NOT_FOUND
Language=English
Class '%1$ls' was not found.
.
MessageId=4005
SymbolicName=WFS_CANNOT_QUERY_ABSTRACT_CLASS
Language=English
Cannot query abstract class '%1$ls'.
.
MessageId=4006
SymbolicName=WFS_NAMED_FEATURETYPE_NOT_FOUND
Language=English
WFS Feature '%1$ls' was not found.
.
MessageId=4007
SymbolicName=WFS_FEATURE_PROPERTY_NOT_FOUND
Language=English
Property '%1$ls' was not found on FDO class '%2$ls'.
.
MessageId=4008
SymbolicName=WFS_FEATURE_NO_GEOMETRY_PROPERTY
Language=English
Class '%1$ls' does not contain a Geometry property.
.
MessageId=4009
SymbolicName=WFS_FEATURE_MISSING_GEOMETRY_PROPERTY
Language=English
Class '%1$ls' does not contain named Geometry property '%1$2s'.
.

;//************************************************************************************
;//************************************************************************************
;//** VERSION-RELATED MESSAGES 
;//************************************************************************************
;//************************************************************************************

;//************************************************************************************
;//************************************************************************************
;//** FILTERS & EXPRESSIONS MESSAGES 
;//************************************************************************************
;//************************************************************************************

;//************************************************************************************
;//************************************************************************************
;//** FEATURE COMMAND MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=7001
SymbolicName=WFS_COMMAND_PARAMETERS_NOT_SUPPORTED
Language=English
Command parameters are not supported.
.
MessageId=7002
SymbolicName=WFS_COMMAND_TIMEOUT_NOT_SUPPORTED
Language=English
Command timeout is not supported.
.
MessageId=7003
SymbolicName=WFS_COMMAND_CLASS_NOT_RECOGNIZED
Language=English
Feature class '%1$ls' is not recognized by WFS server.
.
MessageId=7004
SymbolicName=WFS_NO_QUERY_PROPERTIES_SELECTED
Language=English
No properties were selected for retrieval from Feature class '%1$ls'.
.
MessageId=7005
SymbolicName=WFS_QUERY_NON_COMPUTED_PROPERTIES
Language=English
Cannot query non-computed property '%1$ls' from feature class '%2$ls' using the SelectAggregates command.
.
MessageId=7006
SymbolicName=WFS_QUERY_NON_FUNCTION_SPATIALEXTENTS
Language=English
Unknown WFS SelectAggreates function '%1$ls'. The WFS SelectAggreates command only supports the SpatialExtents function.
.
MessageId=7007
SymbolicName=WFS_SPATIALEXTENTS_TOO_MANY_ARGUMENTS
Language=English
Too many arguments passed to the SelectAggregates function.
.
MessageId=7008
SymbolicName=WFS_SELECT_NON_GEOMETRIC_PROPERTY_ON_CLASS
Language=English
Cannot select a non-geometric property '%1$ls' on class '%2$ls' using the SelectAggreates SpatialExtents function.
.
MessageId=7009
SymbolicName=WFS_SPATIALEXTENTS_NON_FEATURE_CLASS
Language=English
Cannot execute the SelectAggregates(SpatialExtents) command using a non feature class '%1$ls'.
.
MessageId=7010
SymbolicName=WFS_AMBIGUOUS_CLASS_NAME
Language=English
Multiple classes found with name '%1$ls'. Please qualify your class with a schmea qualifier and run your command again.
.
MessageId=7011
SymbolicName=WFS_SELECTAGGREGATES_INVALID_ARGUMENT_TYPE
Language=English
The SelectAggregates(SpatialExtents) command contains an argument '%1$ls' that is not the name of the Geometry property on class '%2$ls'.
.
MessageId=7012
SymbolicName=WFS_SELECTAGGREGATES_FILTER_NOT_SUPPORTED
Language=English
The SelectAggregates(SpatialExtents) command does not support the use of a conditional filter.
.
MessageId=7013
SymbolicName=WFS_SELECTAGGREGATES_INVALID_ARGUMENT
Language=English
The SelectAggregates(SpatialExtents) command contains an invalid function argument.
.

;//************************************************************************************
;//************************************************************************************
;//** LOCK-RELATED MESSAGES
;//************************************************************************************
;//************************************************************************************

MessageId=8001
SymbolicName=WFS_LOCKING_NOT_SUPPORTED
Language=English
Locking not supported.
.

;//************************************************************************************
;//************************************************************************************
;//** SPATIAL CONTEXT MESSAGES 
;//************************************************************************************
;//************************************************************************************

;//************************************************************************************
;//************************************************************************************
;//** TRANSACTION MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=10001
SymbolicName=WFS_CONNECTION_TRANSACTIONS_NOT_SUPPORTED
Language=English
WFS Provider does not support transactions.
.

;//************************************************************************************
;//************************************************************************************
;//** CONNECTION INFORMATIONS
;//************************************************************************************
;//************************************************************************************

MessageId=11001
SymbolicName=WFS_PROVIDER_DISPLAY_NAME
Language=English
OSGeo FDO Provider for WFS
.
MessageId=11002
SymbolicName=WFS_PROVIDER_DESCRIPTION
Language=English
Read access to OGC WFS-based data store.
.
