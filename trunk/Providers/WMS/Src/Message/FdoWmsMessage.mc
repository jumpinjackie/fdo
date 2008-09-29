;;//
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

MessageId=1001
SymbolicName=FDOWMS_RESAMPLE_FUNCTION_DESC
Language=English
The RESAMPLE function returns a raster with the specified extents and size.
.
MessageId=1002
SymbolicName=FDOWMS_RESAMPLE_RASTER_ARGUMENT_DESC
Language=English
An argument that identifies the raster property definition.
.
MessageId=1003
SymbolicName=FDOWMS_RESAMPLE_MINX_ARGUMENT_DESC
Language=English
The X coordinate corresponding to the lower left corner of the RESAMPLE boundary.
.
MessageId=1004
SymbolicName=FDOWMS_RESAMPLE_MINY_ARGUMENT_DESC
Language=English
The Y coordinate corresponding to the lower left corner of the RESAMPLE boundary.
.
MessageId=1005
SymbolicName=FDOWMS_RESAMPLE_MAXX_ARGUMENT_DESC
Language=English
The X coordinate corresponding to the upper right corner of the RESAMPLE boundary.
.
MessageId=1006
SymbolicName=FDOWMS_RESAMPLE_MAXY_ARGUMENT_DESC
Language=English
The Y coordinate corresponding to the upper right corner of the RESAMPLE boundary.
.
MessageId=1007
SymbolicName=FDOWMS_RESAMPLE_HEIGHT_ARGUMENT_DESC
Language=English
The HEIGHT of the output raster image.
.
MessageId=1008
SymbolicName=FDOWMS_RESAMPLE_WIDTH_ARGUMENT_DESC
Language=English
The WIDTH of the output raster image.
.
MessageId=1009
SymbolicName=FDOWMS_UNHANDLED_EXCEPTION
Language=English
Unhandled FDO WMS Provider Internal Exception in '%1$ls'.
.
MessageId=1010
SymbolicName=FDOWMS_OPERATION_NOT_SUPPORTED
Language=English
FDO Operation '%1$ls' is not supported.
.
MessageId=1011
SymbolicName=FDOWMS_SPATIALQUERY_OPERATION_NOT_SUPPORTED
Language=English
Unsupported spatial query operation.
.
MessageId=1012
SymbolicName=FDOWMS_TEMP_PATH_FAILURE
Language=English
Failed to get temp file name.
.
MessageId=1013
SymbolicName=FDOWMS_ONLY_RESAMPLE_CLIP_FUNCTION_SUPPORTED
Language=English
Only RESAMPLE() and CLIP() functions are supported.
.
MessageId=1014
SymbolicName=FDOWMS_RESAMPLE_FUNCTION_INCORRECTLY_USED
Language=English
RESAMPLE() function is incorrectly used.
.
MessageId=1015
SymbolicName=FDOWMS_CLIP_FUNCTION_DESC
Language=English
The CLIP function returns a raster with the specified extent.
.
MessageId=1016
SymbolicName=FDOWMS_CLIP_RASTER_ARGUMENT_DESC
Language=English
An argument that identifies the raster property definition.
.
MessageId=1017
SymbolicName=FDOWMS_CLIP_MINX_ARGUMENT_DESC
Language=English
The X coordinate corresponding to the lower left corner of the CLIP boundary.
.
MessageId=10018
SymbolicName=FDOWMS_CLIP_MINY_ARGUMENT_DESC
Language=English
The Y coordinate corresponding to the lower left corner of the CLIP boundary.
.
MessageId=1019
SymbolicName=FDOWMS_CLIP_MAXX_ARGUMENT_DESC
Language=English
The X coordinate corresponding to the upper right corner of the CLIP boundary.
.
MessageId=1020
SymbolicName=FDOWMS_CLIP_MAXY_ARGUMENT_DESC
Language=English
The Y coordinate corresponding to the upper right corner of the CLIP boundary.
.
MessageId=1021
SymbolicName=FDOWMS_CLIP_FUNCTION_INCORRECTLY_USED
Language=English
CLIP() function is incorrectly used.
.
MessageId=1022
SymbolicName=FDOWMS_SPATIAL_EXTENTS_FUNCTION_DESC
Language=English
The SpatialExtents function returns the spatial extents of a raster property.
.
MessageId=1023
SymbolicName=FDOWMS_SPATIAL_EXTENTS_RASTER_ARGUMENT_DESC
Language=English
An argument that identifies a raster property.
.
MessageId=1024
SymbolicName=FDOWMS_UNSUPPORTED_FUNCTION
Language=English
'%1$ls': The WMS Provider does not support this function.
.

;//************************************************************************************
;//************************************************************************************
;//** CONNECTION MESSAGES 
;//************************************************************************************
;//************************************************************************************


MessageId=2001
SymbolicName=FDOWMS_CONNECTION_TIMEOUT_UNSUPPORTED
Language=English
Connection timeout is not supported.
.
MessageId=2002
SymbolicName=FDOWMS_CONNECTION_COMMAND_NOT_SUPPORTED
Language=English
The command %1$d is not supported.
.
MessageId=2003
SymbolicName=FDOWMS_CONNECTION_CONFIGURATION_FILE_NOT_SUPPORTED
Language=English
FDO WMS Provider does not support configuration files.
.
MessageId=2004
SymbolicName=FDOWMS_CONNECTION_INVALID
Language=English
Connection is invalid.
.
MessageId=2005
SymbolicName=FDOWMS_CONNECTION_REQUIRED_PROPERTY_NULL
Language=English
The required connection property '%1$ls' cannot be set to NULL.
.
MessageId=2006
SymbolicName=FDOWMS_CONNECTION_ENUM_PROPERTY_WRONG_VALUE
Language=English
The given value '%1$ls' for enumerable property '%2$ls' is not in the enumeration value collection.
.
MessageId=2007
SymbolicName=FDOWMS_CONNECTION_PROPERTY_NOT_FOUND
Language=English
The connection property '%1$ls' was not found.
.
MessageId=2008
SymbolicName=FDOWMS_CONNECTION_ALREADY_OPEN
Language=English
The connection is already open.
.
MessageId=2009
SymbolicName=FDOWMS_CONNECTION_UNABLE_CONNECT_WMS_SERVER
Language=English
Unable to connect to WMS server '%1$ls'.
.
MessageId=2010
SymbolicName=FDOWMS_INVALID_CONNECTION_STRING
Language=English
Invalid connection string '%1$ls'
.
MessageId=2011
SymbolicName=FDOWMS_INVALID_CONNECTION_PROPERTY_NAME
Language=English
Invalid connection property name '%1$ls'
.
MessageId=2012
SymbolicName=FDOWMS_INVALID_ACCESSED_SERVER
Language=English
The HTTP request attempted to connect to a WFS server when expected a WMS server.
.
MessageId=2013
SymbolicName=FDOWMS_UNKNOWN_ACCESSED_SERVER
Language=English
The HTTP request attempted to connect to a non WMS server.
.
MessageId=2014
SymbolicName=FDOWMS_UNACCEPTABLE_RESPONSE
Language=English
The HTTP request generated an unacceptable response by the server.
.

;//************************************************************************************
;//************************************************************************************
;//** CONNECTION PROPERTIES 
;//************************************************************************************
;//************************************************************************************

MessageId=3001
SymbolicName=FDOWMS_CONNECTION_PROPERTY_FEATURESERVER
Language=English
FeatureServer
.
MessageId=3002
SymbolicName=FDOWMS_CONNECTION_PROPERTY_USERNAME
Language=English
Username
.
MessageId=3003
SymbolicName=FDOWMS_CONNECTION_PROPERTY_PASSWORD
Language=English
Password
.
MessageId=3004
SymbolicName=FDOWMS_CONNECTION_PROPERTY_DEFAULTIMAGEHEIGHT
Language=English
DefaultImageHeight
.
MessageId=3005
SymbolicName=FDOWMS_CONNECTION_CLOSED
Language=English
The Connection to the WMS provider is closed.
.
MessageId=3006
SymbolicName=FDOWMS_COPY_BASE_PROPERTIES_PARENT
Language=English
'%1$ls': Cannot copy base properties from unspecified parent (NULL).
.
MessageId=3007
SymbolicName=FDOWMS_FIND_RASTER_PROPERTIES_CLASS
Language=English
'%1$ls': Cannot find raster property for unspecified class (NULL).
.
MessageId=3008
SymbolicName=FDOWMS_VERSION_NOT_SUPPORTED
Language=English
The WMS version '%1$ls' is not supported.
.

;//************************************************************************************
;//************************************************************************************
;//** SCHEMA-RELATED MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=4001
SymbolicName=FDOWMS_OBJECT_PROPERTIES_NOT_SUPPORTED
Language=English
Object properties are not supported.
.
MessageId=4002
SymbolicName=FDOWMS_UNSUPPORTED_DATATYPE
Language=English
The '%1$ls' data type is not supported by FdoWms.
.
MessageId=4003
SymbolicName=FDOWMS_INVALID_XMLSCHEMA_RASTERFORMATTYPE
Language=English
'%1$ls' is not a valid WMS XML Raster Format Type.
.
MessageId=4004
SymbolicName=FDOWMS_INVALID_XMLSCHEMA_TRANSPARENCY_VALUE
Language=English
'%1$ls' is not a valid WMS XML Transparency value.
.
MessageId=4005
SymbolicName=FDOWMS_SCHEMA_FAIL_READ_FEATURE_SCHEMAS
Language=English
Failed to de-serialize the feature schemas from the configuration.
.
MessageId=4006
SymbolicName=FDOWMS_SCHEMA_FAIL_READ_SCHEMA_MAPPINGS
Language=English
Failed to de-serialize the schema mappings from the configuration.
.
MessageId=4007
SymbolicName=FDOWMS_SCHEMA_LAYER_NO_TITLE
Language=English
A WMS layer has no name or title.
.
MessageId=4008
SymbolicName=FDOWMS_SCHEMA_INVALID_ELEMENT_NAME
Language=English
Invalid schema element name.
.
MessageId=4009
SymbolicName=FDOWMS_NAMED_SCHEMA_NOT_FOUND
Language=English
FDO Schema '%1$ls' was not found.
.
MessageId=4010
SymbolicName=FDOWMS_NAMED_SCHEMACLASS_NOT_FOUND
Language=English
FDO Feature Class '%1$ls' was not found.
.
MessageId=4011
SymbolicName=FDOWMS_CANNOT_QUERY_ABSTRACT_CLASS
Language=English
FDO Query cann't be performed on abstract class '%1$ls'.
.
MessageId=4012
SymbolicName=FDOWMS_RASTERPROPERTY_HAS_NO_SPATIALCONTEXTASSOCIATION
Language=English
Raster property has no spatial context association.
.
MessageId=4013
SymbolicName=FDOWMS_SCHEMA_MAPPING_NOT_FOUND
Language=English
Physical schema mapping '%1$ls' was not found.
.
MessageId=4014
SymbolicName=FDOWMS_LOGICAL_SCHEMA_NOT_FOUND
Language=English
Physical schema mapping '%1$ls' has no associated logical schema.
.
MessageId=4015
SymbolicName=FDOWMS_PHYSICAL_CLASS_NOT_FOUND
Language=English
Physical class definition '%1$ls' was not found.
.
MessageId=4016
SymbolicName=FDOWMS_LOGICAL_CLASS_NOT_FOUND
Language=English
Physical class '%1$ls' has no associated logical class definition.
.
MessageId=4017
SymbolicName=FDOWMS_PHYSICAL_RASTER_PROPERTY_NOT_FOUND
Language=English
Logical raster property '%1$ls' was not mapped to a physical property.
.
MessageId=4018
SymbolicName=FDOWMS_PROPERTY_NOT_RASTER_PROPERTY
Language=English
Property '%1ls' is not a raster property.
.
MessageId=4019
SymbolicName=FDOWMS_PHYSICAL_SCHEMA_LAYER_HAS_NO_NAME
Language=English
A WMS layer has no name in physical shema mappings.
.
MessageId=4020
SymbolicName=FDOWMS_PHYSICAL_CLASS_DEFINITION_HAS_NO_LAYERS
Language=English
No WMS layers are found in physical class definition '%1$ls'.
.
MessageId=4021
SymbolicName=FDOWMS_INVALID_XMLSCHEMA_USETILECACHE_VALUE
Language=English
'%1$ls' is not a valid XML use tile cache value.
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
SymbolicName=FDOWMS_COMMAND_PARAMETERS_NOT_SUPPORTED
Language=English
Command parameters are not supported.
.
MessageId=7002
SymbolicName=FDOWMS_COMMAND_TIMEOUT_NOT_SUPPORTED
Language=English
Command timeout is not supported.
.
MessageId=7003
SymbolicName=FDOWMS_COMMAND_INCORRECT_PROPERTY_DATA_TYPE
Language=English
Property '%1$ls' is not the proper data type '%2$ls'.
.
MessageId=7004
SymbolicName=FDOWMS_COMMAND_INVALID_PROPERTY_NAME
Language=English
Invalid property name '%1$ls'.
.
MessageId=7005
SymbolicName=FDOWMS_NO_QUERY_PROPERTIES_SELECTED
Language=English
No properties were selected for retrieval from Feature class '%1$ls'.
.
MessageId=7006
SymbolicName=FDOWMS_QUERY_NON_FUNCTION_SPATIALEXTENTS
Language=English
Unknown WFS SelectAggreates function '%1$ls'. The WFS SelectAggreates command only supports the SpatialExtents function.
.
MessageId=7007
SymbolicName=FDOWMS_SPATIALEXTENTS_TOO_MANY_ARGUMENTS
Language=English
Too many arguments passed to the SelectAggregates function.
.
MessageId=7008
SymbolicName=FDOWMS_SELECT_NON_RASTER_PROPERTY_ON_CLASS
Language=English
Cannot select a non-raster property '%1$ls' on class '%2$ls' using the SelectAggreates SpatialExtents function.
.
MessageId=7009
SymbolicName=FDOWMS_SPATIALEXTENTS_NON_FEATURE_CLASS
Language=English
Cannot execute the SelectAggregates(SpatialExtents) command using a non feature class '%1$ls'.
.
MessageId=7010
SymbolicName=FDOWMS_AMBIGUOUS_CLASS_NAME
Language=English
Multiple classes found with name '%1$ls'. Please qualify your class with a schmea qualifier and run your command again.
.
MessageId=7011
SymbolicName=FDOWMS_SELECTAGGREGATES_INVALID_ARGUMENT_TYPE
Language=English
The SelectAggregates(SpatialExtents) command contains an argument '%1$ls' that is not the name of the Raster property on class '%2$ls'.
.
MessageId=7012
SymbolicName=FDOWMS_SELECTAGGREGATES_FILTER_NOT_SUPPORTED
Language=English
The SelectAggregates(SpatialExtents) command does not support the use of a conditional filter.
.
MessageId=7013
SymbolicName=FDOWMS_SELECTAGGREGATES_INVALID_ARGUMENT
Language=English
The SelectAggregates(SpatialExtents) command contains an invalid function argument.
.
MessageId=7014
SymbolicName=FDOWMS_QUERY_NON_COMPUTED_PROPERTIES
Language=English
Cannot query non-computed property '%1$ls' from feature class '%2$ls' using the SelectAggregates command.
.
MessageId=7015
SymbolicName=FDOWMS_FEATURE_NO_RASTER_PROPERTY
Language=English
Class '%1$ls' does not contain a Raster property.
.
MessageId=7016
SymbolicName=FDOWMS_SPATIAL_CONTEXT_INVALID_NAME
Language=English
'%1$ls': Invalid spatial context name; cannot activate.
.

;//************************************************************************************
;//************************************************************************************
;//** LOCK-RELATED MESSAGES
;//************************************************************************************
;//************************************************************************************

MessageId=8001
SymbolicName=FDOWMS_LOCKING_NOT_SUPPORTED
Language=English
Locking not supported.
.

;//************************************************************************************
;//************************************************************************************
;//** SPATIAL CONTEXT MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=9001
SymbolicName=FDOWMS_SPATIALCONTEXT_NOT_SUPPORTED
Language=English
The spatial context '%1$ls' is not supported by the server.
.

;//************************************************************************************
;//************************************************************************************
;//** TRANSACTION MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=10001
SymbolicName=FDOWMS_CONNECTION_TRANSACTIONS_NOT_SUPPORTED
Language=English
FDO WMS Provider does not support transactions.
.

;//************************************************************************************
;//************************************************************************************
;//** RASTER MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=11001
SymbolicName=FDOWMS_11001_RASTER_NULL
Language=English
Operation failed because the raster is null.
.
MessageId=11002
SymbolicName=FDOWMS_11002_DATAMODEL_NOT_SUPPORTED
Language=English
Raster data model not supported.
.
MessageId=11003
SymbolicName=FDOWMS_11003_RASTER_PROPERTY_NOT_EXIST
Language=English
Requested raster property '%1$ls' does not exist.
.
MessageId=11004
SymbolicName=FDOWMS_NULL_RASTER_IO_STREAM
Language=English
NULL Raster I/O stream.
.

;//************************************************************************************
;//************************************************************************************
;//** WMS RELATED MESSAGES 
;//************************************************************************************
;//************************************************************************************

MessageId=12001
SymbolicName=FDOWMS_12001_LAYER_NOT_EXIST
Language=English
The WMS layer '%1$ls' does not exist.
.
MessageId=12002
SymbolicName=FDOWMS_12002_LAYER_HAS_NO_CRS
Language=English
The WMS layer or its parent layers has no CRS.
.
MessageId=12003
SymbolicName=FDOWMS_12003_SPATIALCONTEXT_NOT_SUPPORTED_BY_LAYER
Language=English
The spatial context '%1$ls' is not supported by the layer '%2$ls'.
.
MessageId=12004
SymbolicName=FDOWMS_12004_STYLES_LAYERS_NOT_CORRESPONDING
Language=English
The WMS styles and layers are not corresponding.
.
MessageId=12005
SymbolicName=FDOWMS_12005_GETMAP_NOT_SUPPORTED
Language=English
The WMS GetMap request is not supported.
.
MessageId=12006
SymbolicName=FDOWMS_12006_NO_DATA_RETURNED
Language=English
The WMS Server returned no data from the GetMap request.
.

;//************************************************************************************
;//************************************************************************************
;//** CONNECTION INFORMATIONS
;//************************************************************************************
;//************************************************************************************

MessageId=13001
SymbolicName=WMS_PROVIDER_DISPLAY_NAME
Language=English
OSGeo FDO Provider for WMS
.
MessageId=13002
SymbolicName=WMS_PROVIDER_DESCRIPTION
Language=English
Read access to OGC WMS-based data store.
.
