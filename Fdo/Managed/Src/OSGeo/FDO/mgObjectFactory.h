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
*
*/

#pragma once

#include "FDO\Commands\mgCommandType.h"

BEGIN_NAMESPACE_OSGEO_RUNTIME
public __gc class Disposable;
END_NAMESPACE_OSGEO_RUNTIME

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES
public __gc class IDisposableCollection;
public __gc class Provider;
public __gc class ProviderCollection;
END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFlags;
public __gc class XmlClassMapping;
public __gc class XmlClassMappingCollection;
public __gc class XmlElementMapping;
public __gc class XmlElementMappingCollection;
public __gc class XmlFeatureHandler;
public __gc class XmlFeatureReader;
public __gc class XmlFeatureWriter;
public __gc class XmlFeaturePropertyReader;
public __gc class XmlFeaturePropertyWriter;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class AssociationPropertyDefinition;
public __gc class ClassCapabilities;
public __gc class ClassCollection;
public __gc class ClassDefinition;
public __gc class DataPropertyDefinitionCollection;
public __gc class DataPropertyDefinition;
public __gc class FeatureClass;
public __gc class FeatureSchema;
public __gc class FeatureSchemaCollection;
public __gc class GeometricPropertyDefinition;
public __gc class NetworkLayerClass;
public __gc class PropertyDefinition;
public __gc class PropertyDefinitionCollection;
public __gc class ReadOnlyDataPropertyDefinitionCollection;
public __gc class ReadOnlyPropertyDefinitionCollection;
public __gc class SchemaAttributeDictionary;
public __gc class SchemaElement;
public __gc class UniqueConstraint;
public __gc class UniqueConstraintCollection;
public __gc class PropertyValueConstraint;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_RASTER
public __gc class DataValueCollection;
public __gc __interface IRaster;
public __gc __interface IRasterPropertyDictionary;
public __gc class RasterDataModel;
END_NAMESPACE_OSGEO_FDO_RASTER

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class DataValue;
public __gc class Expression;
public __gc class ExpressionCollection;
public __gc class Identifier;
public __gc class LiteralValue;
public __gc class LOBValue;
public __gc class ValueExpression;
public __gc class DataValueCollection;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
public __gc class Filter;
public __gc class ValueExpressionCollection;
END_NAMESPACE_OSGEO_FDO_FILTER

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS
public __gc __interface IConnection;
public __gc __interface IConnectionInfo;
public __gc __interface IConnectionPropertyDictionary;
public __gc __interface ITransaction;
END_NAMESPACE_OSGEO_FDO_CONNECTIONS

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES
public __gc class ArgumentDefinition;
public __gc class FunctionDefinition;
public __gc class FunctionDefinitionCollection;
public __gc __interface ICommandCapabilities;
public __gc __interface IConnectionCapabilities;
public __gc __interface IExpressionCapabilities;
public __gc __interface IFilterCapabilities;
public __gc __interface IGeometryCapabilities;
public __gc __interface IRasterCapabilities;
public __gc __interface ISchemaCapabilities;
public __gc __interface ITopologyCapabilities;
public __gc class ReadOnlyArgumentDefinitionCollection;
END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
public __gc __interface ICommand;
public __gc class BatchParameterValueCollection;
public __gc class IdentifierCollection;
public __gc class ParameterValue;
public __gc class ParameterValueCollection;
public __gc class PropertyValue;
public __gc class PropertyValueCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SQL
public __gc __interface ISQLDataReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SQL

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE
public __gc __interface IReader;
public __gc __interface IDataReader;
public __gc __interface IFeatureReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalSchemaMappingCollection;
public __gc class PhysicalSchemaMapping;
public __gc class PhysicalElementMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE
public __gc __interface IMeasureUnitReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
public __gc __interface ILockConflictReader;
public __gc __interface ILockedObjectReader;
public __gc __interface ILockOwnersReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT
public __gc __interface ISpatialContextReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_NETWORK
public __gc __interface INetworkPathReader;
public __gc __interface INetworkReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_NETWORK

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
public __gc __interface ILongTransactionCheckpointReader;
public __gc __interface ILongTransactionReader;
public __gc __interface ILongTransactionSetReader;
public __gc __interface ILongTransactionPrivilegeReader;
public __gc __interface ILongTransactionConflictDirectiveEnumerator;
END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE
public __gc __interface IDataStorePropertyDictionary;
public __gc __interface IDataStoreReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE

BEGIN_NAMESPACE_OSGEO_FDO
public __gc __interface IConnectionManager;
public __gc __interface IProviderRegistry;
public __gc class IDisposableCollection;

public __gc __sealed class ObjectFactory
{
public:
    static NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* CreateIXmlSaxHandler(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* CreateXmlSaxContext(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary* CreateIDataStorePropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReader* CreateIDataStoreReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReader* CreateILongTransactionCheckpointReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader* CreateILongTransactionReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionSetReader* CreateILongTransactionSetReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReader* CreateILongTransactionPrivilegeReader(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumerator* CreateILongTransactionConflictDirectiveEnumerator(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReader* CreateISpatialContextReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader* CreateILockConflictReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReader* CreateILockedObjectReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReader* CreateILockOwnersReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReader* CreateIMeasureUnitReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection* CreatePhysicalSchemaMappingCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* CreatePhysicalSchemaMapping(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping* CreatePhysicalElementMapping(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS::ICommand* CreateICommand(System::IntPtr ptr, System::Boolean autoDispose, NAMESPACE_OSGEO_FDO_COMMANDS::CommandType type);
    static NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection* CreateBatchParameterValueCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection* CreateIdentifierCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue* CreateParameterValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection* CreateParameterValueCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* CreatePropertyValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection* CreatePropertyValueCollection(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReader* CreateISQLDataReader(System::IntPtr ptr, System::Boolean autoDispose);
    
    static NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReader* CreateIDataReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader* CreateIFeatureReader(System::IntPtr ptr, System::Boolean autoDispose);
   
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition* CreateArgumentDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition* CreateFunctionDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection* CreateFunctionDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilities* CreateICommandCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities* CreateIConnectionCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities* CreateIExpressionCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities* CreateIFilterCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities* CreateIGeometryCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilities* CreateIRasterCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities* CreateISchemaCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities* CreateITopologyCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection* CreateReadOnlyArgumentDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection* CreateIConnection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo* CreateIConnectionInfo(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary* CreateIConnectionPropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction* CreateITransaction(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_FILTER::Filter* CreateFilter(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection* CreateValueExpressionCollection(System::IntPtr ptr, System::Boolean autoDispose);
	
    static NAMESPACE_OSGEO_FDO::IConnectionManager* CreateIConnectionManager(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO::IProviderRegistry* CreateIProviderRegistry(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO::IDisposableCollection* CreateIDisposableCollection(System::IntPtr ptr, System::Boolean autoDispose);
    
    static NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider* CreateProvider(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection* CreateProviderCollection(System::IntPtr ptr, System::Boolean autoDispose);
    
    static NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader* CreateXmlFeaturePropertyReader(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter* CreateXmlFeaturePropertyWriter(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlClassMapping* CreateXmlClassMapping(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlClassMappingCollection* CreateXmlClassMappingCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlElementMapping* CreateXmlElementMapping(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection* CreateXmlElementMappingCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler* CreateXmlFeatureHandler(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader* CreateXmlFeatureReader(System::IntPtr ptr, System::Boolean autoDispose);	
	static NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter* CreateXmlFeatureWriter(System::IntPtr ptr, System::Boolean autoDispose);	
    static NAMESPACE_OSGEO_FDO_XML::XmlFlags* CreateXmlFlags(System::IntPtr ptr, System::Boolean autoDispose);
    
    static NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* CreateAssociationPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities* CreateClassCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection* CreateClassCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* CreateClassDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection* CreateDataPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* CreateDataPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass* CreateFeatureClass(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema* CreateFeatureSchema(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection* CreateFeatureSchemaCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition* CreateGeometricPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass* CreateNetworkLayerClass(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition* CreatePropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection* CreatePropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection* CreateReadOnlyDataPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection* CreateReadOnlyPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary* CreateSchemaAttributeDictionary(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement* CreateSchemaElement(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint* CreateUniqueConstraint(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection* CreateUniqueConstraintCollection(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint* CreatePropertyValueConstraint(System::IntPtr ptr, System::Boolean autoDispose);	

    static NAMESPACE_OSGEO_FDO_RASTER::DataValueCollection* CreateDataValueCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_RASTER::IRaster* CreateIRaster(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionary* CreateIRasterPropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel* CreateRasterDataModel(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue* CreateDataValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::Expression* CreateExpression(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionCollection* CreateExpressionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* CreateIdentifier(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue* CreateLiteralValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue* CreateLOBValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression* CreateValueExpression(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_EXPRESSION::DataValueCollection* CreateEDataValueCollection(System::IntPtr ptr, System::Boolean autoDispose);

private:
	ObjectFactory(){}
};

END_NAMESPACE_OSGEO_FDO


