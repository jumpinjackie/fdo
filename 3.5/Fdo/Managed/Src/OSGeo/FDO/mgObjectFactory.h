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
ref class Disposable;
END_NAMESPACE_OSGEO_RUNTIME

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES
ref class IDisposableCollection;
ref class Provider;
ref class ProviderCollection;
END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class XmlFlags;
ref class XmlClassMapping;
ref class XmlClassMappingCollection;
ref class XmlElementMapping;
ref class XmlElementMappingCollection;
ref class XmlFeatureHandler;
ref class XmlFeatureReader;
ref class XmlFeatureWriter;
ref class XmlFeaturePropertyReader;
ref class XmlFeaturePropertyWriter;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class AssociationPropertyDefinition;
ref class ClassCapabilities;
ref class ClassCollection;
ref class ClassDefinition;
ref class DataPropertyDefinitionCollection;
ref class DataPropertyDefinition;
ref class FeatureClass;
ref class FeatureSchema;
ref class FeatureSchemaCollection;
ref class GeometricPropertyDefinition;
ref class NetworkLayerClass;
ref class PropertyDefinition;
ref class PropertyDefinitionCollection;
ref class ReadOnlyDataPropertyDefinitionCollection;
ref class ReadOnlyPropertyDefinitionCollection;
ref class SchemaAttributeDictionary;
ref class SchemaElement;
ref class UniqueConstraint;
ref class UniqueConstraintCollection;
ref class PropertyValueConstraint;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_RASTER
ref class DataValueCollection;
interface class IRaster;
interface class IRasterPropertyDictionary;
ref class RasterDataModel;
END_NAMESPACE_OSGEO_FDO_RASTER

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class DataValue;
ref class Expression;
ref class ExpressionCollection;
ref class Identifier;
ref class LiteralValue;
ref class LOBValue;
ref class ValueExpression;
ref class DataValueCollection;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_FILTER
ref class Filter;
ref class ValueExpressionCollection;
END_NAMESPACE_OSGEO_FDO_FILTER

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS
interface class IConnection;
interface class IConnectionInfo;
interface class IConnectionPropertyDictionary;
interface class ITransaction;
END_NAMESPACE_OSGEO_FDO_CONNECTIONS

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES
ref class ArgumentDefinition;
ref class FunctionDefinition;
ref class FunctionDefinitionCollection;
interface class ICommandCapabilities;
interface class IConnectionCapabilities;
interface class IExpressionCapabilities;
interface class IFilterCapabilities;
interface class IGeometryCapabilities;
interface class IRasterCapabilities;
interface class ISchemaCapabilities;
interface class ITopologyCapabilities;
ref class ReadOnlyArgumentDefinitionCollection;
ref class SignatureDefinition;
ref class SignatureDefinitionCollection;
ref class ReadOnlySignatureDefinitionCollection;
END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
interface class ICommand;
ref class BatchParameterValueCollection;
ref class IdentifierCollection;
ref class ParameterValue;
ref class ParameterValueCollection;
ref class PropertyValue;
ref class PropertyValueCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SQL
interface class ISQLDataReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SQL

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE
interface class IReader;
interface class IDataReader;
interface class IFeatureReader;
interface class IScrollableFeatureReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
ref class PhysicalSchemaMappingCollection;
ref class PhysicalSchemaMapping;
ref class PhysicalElementMapping;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE
interface class IMeasureUnitReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING
interface class ILockConflictReader;
interface class ILockedObjectReader;
interface class ILockOwnersReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT
interface class ISpatialContextReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_NETWORK
interface class INetworkPathReader;
interface class INetworkReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_NETWORK

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION
interface class ILongTransactionCheckpointReader;
interface class ILongTransactionReader;
interface class ILongTransactionSetReader;
interface class ILongTransactionPrivilegeReader;
interface class ILongTransactionConflictDirectiveEnumerator;
END_NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE
interface class IDataStorePropertyDictionary;
interface class IDataStoreReader;
END_NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE

BEGIN_NAMESPACE_OSGEO_FDO
interface class IConnectionManager;
interface class IProviderRegistry;
ref class IDisposableCollection;

public ref class ObjectFactory sealed
{
public:
    static NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ CreateIXmlSaxHandler(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ CreateXmlSaxContext(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary^ CreateIDataStorePropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReader^ CreateIDataStoreReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReader^ CreateILongTransactionCheckpointReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader^ CreateILongTransactionReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionSetReader^ CreateILongTransactionSetReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReader^ CreateILongTransactionPrivilegeReader(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumerator^ CreateILongTransactionConflictDirectiveEnumerator(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReader^ CreateISpatialContextReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ CreateILockConflictReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReader^ CreateILockedObjectReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReader^ CreateILockOwnersReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReader^ CreateIMeasureUnitReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ CreatePhysicalSchemaMappingCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ CreatePhysicalSchemaMapping(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ CreatePhysicalElementMapping(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS::ICommand^ CreateICommand(System::IntPtr ptr, System::Boolean autoDispose, NAMESPACE_OSGEO_FDO_COMMANDS::CommandType type);
    static NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection^ CreateBatchParameterValueCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection^ CreateIdentifierCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue^ CreateParameterValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection^ CreateParameterValueCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ CreatePropertyValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ CreatePropertyValueCollection(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReader^ CreateISQLDataReader(System::IntPtr ptr, System::Boolean autoDispose);
    
    static NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReader^ CreateIDataReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ CreateIFeatureReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReader^ CreateIScrollableFeatureReader(System::IntPtr ptr, System::Boolean autoDispose);
   
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition^ CreateArgumentDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^ CreateFunctionDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection^ CreateFunctionDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilities^ CreateICommandCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities^ CreateIConnectionCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities^ CreateIExpressionCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities^ CreateIFilterCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities^ CreateIGeometryCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilities^ CreateIRasterCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities^ CreateISchemaCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities^ CreateITopologyCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection^ CreateReadOnlyArgumentDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ CreateSignatureDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection^ CreateSignatureDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection^ CreateReadOnlySignatureDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection^ CreateIConnection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo^ CreateIConnectionInfo(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary^ CreateIConnectionPropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction^ CreateITransaction(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_FILTER::Filter^ CreateFilter(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection^ CreateValueExpressionCollection(System::IntPtr ptr, System::Boolean autoDispose);
	
    static NAMESPACE_OSGEO_FDO::IConnectionManager^ CreateIConnectionManager(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO::IProviderRegistry^ CreateIProviderRegistry(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO::IDisposableCollection^ CreateIDisposableCollection(System::IntPtr ptr, System::Boolean autoDispose);
    
    static NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider^ CreateProvider(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection^ CreateProviderCollection(System::IntPtr ptr, System::Boolean autoDispose);
    
    static NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader^ CreateXmlFeaturePropertyReader(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter^ CreateXmlFeaturePropertyWriter(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlClassMapping^ CreateXmlClassMapping(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlClassMappingCollection^ CreateXmlClassMappingCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlElementMapping^ CreateXmlElementMapping(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection^ CreateXmlElementMappingCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ CreateXmlFeatureHandler(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader^ CreateXmlFeatureReader(System::IntPtr ptr, System::Boolean autoDispose);	
	static NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter^ CreateXmlFeatureWriter(System::IntPtr ptr, System::Boolean autoDispose);	
    static NAMESPACE_OSGEO_FDO_XML::XmlFlags^ CreateXmlFlags(System::IntPtr ptr, System::Boolean autoDispose);
    
    static NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ CreateAssociationPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities^ CreateClassCapabilities(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection^ CreateClassCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ CreateClassDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection^ CreateDataPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ CreateDataPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass^ CreateFeatureClass(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ CreateFeatureSchema(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection^ CreateFeatureSchemaCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ CreateGeometricPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass^ CreateNetworkLayerClass(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition^ CreatePropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection^ CreatePropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection^ CreateReadOnlyDataPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection^ CreateReadOnlyPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary^ CreateSchemaAttributeDictionary(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ CreateSchemaElement(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^ CreateUniqueConstraint(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection^ CreateUniqueConstraintCollection(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint^ CreatePropertyValueConstraint(System::IntPtr ptr, System::Boolean autoDispose);	

    static NAMESPACE_OSGEO_FDO_RASTER::DataValueCollection^ CreateDataValueCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_RASTER::IRaster^ CreateIRaster(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionary^ CreateIRasterPropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ CreateRasterDataModel(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ CreateDataValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ CreateExpression(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionCollection^ CreateExpressionCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ CreateIdentifier(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue^ CreateLiteralValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ CreateLOBValue(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ CreateValueExpression(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_FDO_EXPRESSION::DataValueCollection^ CreateEDataValueCollection(System::IntPtr ptr, System::Boolean autoDispose);

private:
	ObjectFactory(){}
};

END_NAMESPACE_OSGEO_FDO


