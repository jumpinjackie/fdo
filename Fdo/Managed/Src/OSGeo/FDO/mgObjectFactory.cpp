/*
* 
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

#include "stdafx.h"
#include <Fdo.h>
#include <Fdo\Xml\NameCollectionHandler.h>
#include <Fdo\Xml\FeaturePropertyReader.h>
#include <Fdo\Xml\FeaturePropertyWriter.h>
#include <Fdo\Xml\FeatureFlags.h>
#include <Fdo\Xml\SpatialContextFlags.h>
#include <Fdo\Xml\SpatialContextReader.h>
#include <Fdo\Xml\FeatureContext.h>
#include <Fdo\Xml\FeatureReader.h>
#include <Fdo\Xml\FeatureWriter.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\PropertyValueConstraintType.h>

#include "FDO\mgObjectFactory.h"

#include "FDO\mgIDisposableCollection.h"
#include "FDO\ClientServices\mgProvider.h"
#include "FDO\ClientServices\mgProviderCollection.h"
#include "FDO\ClientServices\mgProviderNameTokens.h"
#include "FDO\Commands\mgBatchParameterValueCollection.h"
#include "FDO\Commands\mgIdentifierCollection.h"
#include "FDO\Commands\mgParameterValue.h"
#include "FDO\Commands\mgParameterValueCollection.h"
#include "FDO\Commands\mgPropertyValue.h"
#include "FDO\Commands\mgPropertyValueCollection.h"
#include "FDO\Commands\Locking\mgILockConflictReaderImp.h"
#include "FDO\Commands\Locking\mgILockedObjectReaderImp.h"
#include "FDO\Commands\Locking\mgILockOwnersReaderImp.h"
#include "FDO\Commands\UnitOfMeasure\mgIMeasureUnitReaderImp.h"
#include "FDO\Commands\SQL\mgISQLDataReaderImp.h"
#include "FDO\Commands\SpatialContext\mgISpatialContextReaderImp.h"				
#include "FDO\Commands\Schema\mgPhysicalSchemaMappingCollection.h"	
#include "FDO\Commands\LongTransaction\mgILTCheckpointReaderImp.h"	
#include "FDO\Commands\LongTransaction\mgILTConflictDirectiveEnumeratorImp.h"	
#include "FDO\Commands\LongTransaction\mgILTPrivilegeReaderImp.h"	
#include "FDO\Commands\LongTransaction\mgILongTransactionReaderImp.h"
#include "FDO\Commands\LongTransaction\mgILongTransactionSetReaderImp.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\Expression\mgExpressionCollection.h"
#include "FDO\Filter\mgIFilterProcessorImp.h"
#include "FDO\Filter\mgValueExpressionCollection.h"
#include "FDO\Xml\mgXmlClassMappingCollection.h"
#include "FDO\Xml\mgXmlElementMappingCollection.h"
#include "FDO\Xml\mgXmlFeatureHandler.h"
#include "FDO\Xml\mgXmlFeaturePropertyWriter.h"
#include "FDO\Xml\mgXmlFeatureReader.h"
#include "FDO\Xml\mgXmlFeatureWriter.h"
#include "FDO\Xml\mgXmlSpatialContextWriter.h"
#include "FDO\Schema\mgClassCapabilities.h"
#include "FDO\Schema\mgClassCollection.h"
#include "FDO\Schema\mgDataPropertyDefinitionCollection.h"
#include "FDO\Schema\mgFeatureClassCollection.h"
#include "FDO\Schema\mgFeatureSchemaCollection.h"
#include "FDO\Schema\mgPropertyDefinitionCollection.h"
#include "FDO\Schema\mgReadOnlyDataPropertyDefinitionCollection.h"
#include "FDO\Schema\mgReadOnlyPropertyDefinitionCollection.h"
#include "FDO\Schema\mgUniqueConstraint.h"
#include "FDO\Schema\mgUniqueConstraintCollection.h"
#include "FDO\Schema\mgPropertyValueConstraint.h"
#include "FDO\Schema\mgPropertyValueConstraintRange.h"
#include "FDO\Schema\mgPropertyValueConstraintList.h"
#include "FDO\Raster\mgDataValueCollection.h"
#include "FDO\Raster\mgRasterDataModel.h"
#include "FDO\Raster\mgIRasterImp.h"
#include "FDO\Raster\mgIRasterPropertyDictionaryImp.h"
#include "FDO\Connections\mgIConnectionImp.h"
#include "FDO\Connections\mgIConnectionInfoImp.h"
#include "FDO\Connections\mgIConnectionPropertyDictionaryImp.h"
#include "FDO\Connections\mgITransactionImp.h"
#include "FDO\Connections\Capabilities\mgArgumentDefinition.h"
#include "FDO\Connections\Capabilities\mgArgumentDefinitionCollection.h"
#include "FDO\Connections\Capabilities\mgFunctionDefinition.h"
#include "FDO\Connections\Capabilities\mgFunctionDefinitionCollection.h"
#include "FDO\Connections\Capabilities\mgICommandCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIConnectionCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIExpressionCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIFilterCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIGeometryCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgIRasterCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgISchemaCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgITopologyCapabilitiesImp.h"
#include "FDO\Connections\Capabilities\mgReadOnlyArgDefColl.h"
#include "FDO\Connections\Capabilities\mgSignatureDefinition.h"

#include "FDO\mgIConnectionManagerImp.h"
#include "FDO\ClientServices\mgConnectionManager.h"

#include "FDO\mgIProviderRegistryImp.h"
#include "FDO\ClientServices\mgProviderRegistry.h"

#include "FDO\Commands\mgICommandImp.h"
#include "FDO\Commands\mgIFeatureCommandImp.h"
#include "FDO\Commands\Feature\mgIUpdateImp.h"
#include "FDO\Commands\Feature\mgIInsertImp.h"
#include "FDO\Commands\Feature\mgIDeleteImp.h"
#include "FDO\Commands\Feature\mgISelectAggregatesImp.h"
#include "FDO\Commands\Feature\mgIExtendedSelectImp.h"
#include "FDO\Commands\Feature\mgISelectImp.h"
#include "FDO\Commands\Feature\mgIBaseSelectImp.h"
#include "FDO\Commands\Locking\mgIAcquireLockImp.h"
#include "FDO\Commands\Locking\mgIGetLockedObjectsImp.h"
#include "FDO\Commands\Locking\mgIGetLockInfoImp.h"
#include "FDO\Commands\Locking\mgIGetLockOwnersImp.h"
#include "FDO\Commands\Locking\mgIReleaseLockImp.h"
#include "FDO\Commands\UnitOfMeasure\mgICreateMeasureUnitImp.h"
#include "FDO\Commands\UnitOfMeasure\mgIDestroyMeasureUnitImp.h"
#include "FDO\Commands\UnitOfMeasure\mgIGetMeasureUnitsImp.h"
#include "FDO\Commands\SQL\mgISQLCommandImp.h"
#include "FDO\Commands\SpatialContext\mgIActivateSpatialContextImp.h"
#include "FDO\Commands\SpatialContext\mgICreateSpatialContextImp.h"
#include "FDO\Commands\SpatialContext\mgIDestroySpatialContextImp.h"
#include "FDO\Commands\SpatialContext\mgIGetSpatialContextsImp.h"
#include "FDO\Commands\Schema\mgIApplySchemaImp.h"
#include "FDO\Commands\Schema\mgIGetClassNamesImp.h"
#include "FDO\Commands\Schema\mgIGetSchemaNamesImp.h"
#include "FDO\Commands\Schema\mgIDescribeSchemaImp.h"
#include "FDO\Commands\Schema\mgIDescribeSchemaMappingImp.h"
#include "FDO\Commands\Schema\mgIDestroySchemaImp.h"
#include "FDO\Commands\LongTransaction\mgIActivateLongTransactionImp.h"
#include "FDO\Commands\LongTransaction\mgIActivateLTCheckpointImp.h"
#include "FDO\Commands\LongTransaction\mgIChangeLTPrivilegesImp.h"
#include "FDO\Commands\LongTransaction\mgIChangeLongTransactionSetImp.h"
#include "FDO\Commands\LongTransaction\mgICommitLongTransactionImp.h"
#include "FDO\Commands\LongTransaction\mgICreateLongTransactionImp.h"
#include "FDO\Commands\LongTransaction\mgICreateLTCheckpointImp.h"
#include "FDO\Commands\LongTransaction\mgIDeactivateLongTransactionImp.h"
#include "FDO\Commands\LongTransaction\mgIFreezeLongTransactionImp.h"
#include "FDO\Commands\LongTransaction\mgIGetLongTransactionCheckpointsImp.h"
#include "FDO\Commands\LongTransaction\mgIGetLongTransactionPrivilegesImp.h"
#include "FDO\Commands\LongTransaction\mgIGetLongTransactionsImp.h"
#include "FDO\Commands\LongTransaction\mgIGetLongTransactionsInSetImp.h"
#include "FDO\Commands\LongTransaction\mgIRollbackLongTransactionImp.h"
#include "FDO\Commands\LongTransaction\mgIRollbackLTCheckpointImp.h"
#include "FDO\Commands\DataStore\mgIDataStorePropertyDictionaryImp.h"
#include "FDO\Commands\DataStore\mgIDataStoreReaderImp.h"
#include "FDO\Commands\DataStore\mgICreateDataStoreImp.h"
#include "FDO\Commands\DataStore\mgIDestroyDataStoreImp.h"
#include "FDO\Commands\DataStore\mgIListDataStoresImp.h"

#include "FDO\Commands\mgCommandType.h"

#include "FDO\Commands\Feature\mgIReaderImp.h"
#include "FDO\Commands\Feature\mgIDataReaderImp.h"
#include "FDO\Commands\Feature\mgIFeatureReaderImp.h"
#include "FDO\Commands\Feature\mgIScrollableFeatureReaderImp.h"

#include "FDO\Commands\Schema\mgPhysicalSchemaMapping.h"
#include "FDO\Commands\Schema\mgPhysicalPropertyMapping.h"
#include "FDO\Commands\Schema\mgPhysicalClassMapping.h"
#include "FDO\Commands\Schema\mgPhysicalElementMapping.h"
#include "FDO\Commands\Schema\mgPhysicalSchemaMappingCollection.h"
#include "FDO\Xml\mgXmlClassMapping.h"
#include "FDO\Xml\mgXmlElementMapping.h"
#include "FDO\Xml\mgXmlFeaturePropertyReader.h"
#include "FDO\Xml\mgXmlDeserializable.h"
#include "FDO\Xml\mgXmlNameCollectionHandler.h"
#include "FDO\Xml\mgXmlSchemaMapping.h"
#include "FDO\Xml\mgXmlSpatialContextReader.h"
#include "FDO\Schema\mgSchemaElement.h"
#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Schema\mgClass.h"
#include "FDO\Schema\mgPropertyDefinition.h"
#include "FDO\Schema\mgAssociationPropertyDefinition.h"
#include "FDO\Schema\mgDataPropertyDefinition.h"
#include "FDO\Schema\mgFeatureClass.h"
#include "FDO\Schema\mgFeatureSchema.h"
#include "FDO\Schema\mgFeatureSchemaCollection.h"
#include "FDO\Schema\mgGeometricPropertyDefinition.h"
#include "FDO\Schema\mgNetworkClass.h"
#include "FDO\Schema\mgNetworkFeatureClass.h"
#include "FDO\Schema\mgNetworkLayerClass.h"
#include "FDO\Schema\mgNetworkLinkFeatureClass.h"
#include "FDO\Schema\mgNetworkNodeFeatureClass.h"
#include "FDO\Schema\mgObjectPropertyDefinition.h"
#include "FDO\Schema\mgRasterPropertyDefinition.h"
#include "FDO\Schema\mgTopoFeaturePropertyDefinition.h"
#include "FDO\Schema\mgTopoGeometryPropertyDefinition.h"
#include "FDO\Schema\mgTopology.h"
#include "FDO\Schema\mgSchemaAttributeDictionary.h"

#include "FDO\Expression\mgExpression.h"
#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Expression\mgBinaryExpression.h"
#include "FDO\Expression\mgValueExpression.h"
#include "FDO\Expression\mgUnaryExpression.h"
#include "FDO\Expression\mgBLOBValue.h"
#include "FDO\Expression\mgBooleanValue.h"
#include "FDO\Expression\mgByteValue.h"
#include "FDO\Expression\mgCLOBValue.h"
#include "FDO\Expression\mgComputedIdentifier.h"
#include "FDO\Expression\mgDataValue.h"
#include "FDO\Expression\mgDateTimeValue.h"
#include "FDO\Expression\mgDecimalValue.h"
#include "FDO\Expression\mgDoubleValue.h"
#include "FDO\Expression\mgFunction.h"
#include "FDO\Expression\mgGeometryValue.h"
#include "FDO\Expression\mgInt16Value.h"
#include "FDO\Expression\mgInt32Value.h"
#include "FDO\Expression\mgInt64Value.h"
#include "FDO\Expression\mgLiteralValue.h"
#include "FDO\Expression\mgLOBValue.h"
#include "FDO\Expression\mgParameter.h"
#include "FDO\Expression\mgSingleValue.h"
#include "FDO\Expression\mgStringValue.h"
#include "FDO\Expression\mgDataValueCollection.h"

#include "FDO\Filter\mgFilter.h"
#include "FDO\Filter\mgBinaryLogicalOperator.h"
#include "FDO\Filter\mgComparisonCondition.h"
#include "FDO\Filter\mgDistanceCondition.h"
#include "FDO\Filter\mgGeometricCondition.h"
#include "FDO\Filter\mgInCondition.h"
#include "FDO\Filter\mgLogicalOperator.h"
#include "FDO\Filter\mgNullCondition.h"
#include "FDO\Filter\mgSearchCondition.h"
#include "FDO\Filter\mgSpatialCondition.h"
#include "FDO\Filter\mgUnaryLogicalOperator.h"

#include "FDO\Xml\mgXmlFlags.h"
#include "FDO\Xml\mgXmlFeatureFlags.h"
#include "FDO\Xml\mgXmlSpatialContextFlags.h"
#include "FDO\Xml\mgXmlContext.h"
#include "FDO\Xml\mgXmlFeatureContext.h"

#include "FDO\Schema\mgGeometricType.h"

template <class fdoclas, class clas> 
inline NAMESPACE_OSGEO_RUNTIME::Disposable^ CHECK(FdoIDisposable* p, FdoBoolean autoDispose)
{
    return (dynamic_cast<fdoclas*>(p) ?  gcnew clas(IntPtr(p), autoDispose) : nullptr);
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIXmlSaxHandler(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"

	if (wrap = CHECK<FdoNetworkLinkFeatureClass, NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoNetworkNodeFeatureClass, NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);

	if (wrap = CHECK<FdoNetworkFeatureClass, NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoTopoGeometryPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::TopoGeometryPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);

	if (wrap = CHECK<FdoXmlClassMapping, NAMESPACE_OSGEO_FDO_XML::XmlClassMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoXmlSchemaMapping, NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoXmlClassMapping, NAMESPACE_OSGEO_FDO_XML::XmlClassMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoClass, NAMESPACE_OSGEO_FDO_SCHEMA::Class>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoAssociationPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoDataPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoFeatureClass, NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoGeometricPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoNetworkClass, NAMESPACE_OSGEO_FDO_SCHEMA::NetworkClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoNetworkLayerClass, NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoObjectPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoRasterPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoTopoFeaturePropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::TopoFeaturePropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoTopology, NAMESPACE_OSGEO_FDO_SCHEMA::Topology>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	
	if (wrap = CHECK<FdoXmlElementMapping, NAMESPACE_OSGEO_FDO_XML::XmlElementMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoPhysicalSchemaMapping, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoPhysicalPropertyMapping, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoPhysicalClassMapping, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoClassDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoFeatureSchema, NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);	
	
	if (wrap = CHECK<FdoFeatureSchemaCollection, NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^>(static_cast<System::IDisposable^>(wrap));	
	if (wrap = CHECK<FdoPhysicalSchemaMappingCollection, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^>(static_cast<System::IDisposable^>(wrap));

	if (wrap = CHECK<FdoXmlFeaturePropertyReader, NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoPhysicalElementMapping, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoXmlDeserializable, NAMESPACE_OSGEO_FDO_XML::XmlDeserializable>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoXmlNameCollectionHandler, NAMESPACE_OSGEO_FDO_XML::XmlNameCollectionHandler>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoXmlSpatialContextReader, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextReader>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoSchemaElement, NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);
	if (wrap = CHECK<FdoSchemaAttributeDictionary, NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler^>(wrap);

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(ptr, autoDispose);
}

NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlSaxContext(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"

	if (wrap = CHECK<FdoXmlContext, NAMESPACE_OSGEO_FDO_XML::XmlContext>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^>(wrap);
	if (wrap = CHECK<FdoXmlFeatureContext, NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^>(wrap);

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlSaxContext(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO::IConnectionManager^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionManager(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO::IConnectionManagerImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO::IProviderRegistry^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIProviderRegistry(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO::IProviderRegistryImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO::IDisposableCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDisposableCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO::IDisposableCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFlags(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"

	if (wrap = CHECK<FdoXmlFeatureFlags, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_XML::XmlFlags^>(wrap);
	if (wrap = CHECK<FdoXmlSpatialContextFlags, NAMESPACE_OSGEO_FDO_XML::XmlSpatialContextFlags>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_XML::XmlFlags^>(wrap);

	return gcnew NAMESPACE_OSGEO_FDO_XML::XmlFlags(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlClassMapping^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlClassMapping(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_XML::XmlClassMapping(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlClassMappingCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlClassMappingCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_XML::XmlClassMappingCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlElementMapping^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlElementMapping(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_XML::XmlElementMapping(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlElementMappingCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureHandler(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"
	
	if (wrap = CHECK<FdoXmlFeatureReader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^>(wrap);	

	return gcnew NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeatureWriter(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeaturePropertyReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateXmlFeaturePropertyWriter(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateProvider(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CLIENTSERVICES::Provider(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateProviderCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueConstraint(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr"

	if (wrap = CHECK<FdoPropertyValueConstraintRange, NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange^>(wrap);
	if (wrap = CHECK<FdoPropertyValueConstraintList, NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintList^>(wrap);

	return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateUniqueConstraint(System::IntPtr ptr, bool autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraint(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateUniqueConstraintCollection(System::IntPtr ptr, bool autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateAssociationPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoClassDefinition* p = static_cast<FdoClassDefinition*>(ptr.ToPointer());
	
	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	switch(p->GetClassType())
	{
		case FdoClassType_Class:
		{
			// Note:
			// Here we need keep dynamic_cast to decide the real type of "ptr" because these two classes will return the same class type.

			if (wrap = CHECK<FdoTopology, NAMESPACE_OSGEO_FDO_SCHEMA::Topology>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^>(wrap);

			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::Class(IntPtr(p), autoDispose);
		}
		case FdoClassType_FeatureClass:
		{
			// Note:
			// Here we need keep dynamic_cast to decide the real type of "ptr" because these two classes will return the same class type.

			if (wrap = CHECK<FdoNetworkFeatureClass, NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^>(wrap);
			
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass(IntPtr(p), autoDispose);
		}
		case FdoClassType_NetworkClass:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::NetworkClass(IntPtr(p), autoDispose);
		}
		case FdoClassType_NetworkLayerClass:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass(IntPtr(p), autoDispose);
		}
		case FdoClassType_NetworkNodeClass:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass(IntPtr(p), autoDispose);
		}
		case FdoClassType_NetworkLinkClass:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass(IntPtr(p), autoDispose);
		}
		default:
			return nullptr;
	}
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureClass(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoFeatureClass* p = static_cast<FdoFeatureClass*>(ptr.ToPointer());
	
	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	switch(p->GetClassType())
	{
		case FdoClassType_FeatureClass:
		{
			// Note:
			// Here we need keep dynamic_cast to decide the real type of "ptr" because these two classes will return the same class type.

			if (wrap = CHECK<FdoNetworkFeatureClass, NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass^>(wrap);
			
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass(IntPtr(p), autoDispose);
		}
		case FdoClassType_NetworkNodeClass:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::NetworkNodeFeatureClass(IntPtr(p), autoDispose);
		}
		case FdoClassType_NetworkLinkClass:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLinkFeatureClass(IntPtr(p), autoDispose);
		}
		default:
			return nullptr;
	}
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchema(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchemaCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateGeometricPropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
	    return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because these two classes will return the same property type.
	// So we can not use switch-case clause to avoid dynamic_cast.

	if (wrap = CHECK<FdoTopoGeometryPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::TopoGeometryPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateNetworkLayerClass(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
	    return nullptr;

	FdoPropertyDefinition* p = static_cast<FdoPropertyDefinition*>(ptr.ToPointer());

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	switch(p->GetPropertyType())
	{
		case FdoPropertyType_DataProperty:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition(IntPtr(p), autoDispose);
		}
		case FdoPropertyType_ObjectProperty:
		{	
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition(IntPtr(p), autoDispose);
		}
		case FdoPropertyType_GeometricProperty:
		{
			if (wrap = CHECK<FdoTopoGeometryPropertyDefinition, NAMESPACE_OSGEO_FDO_SCHEMA::TopoGeometryPropertyDefinition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition^>(wrap);

			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::GeometricPropertyDefinition(IntPtr(p), autoDispose);
		}
		case FdoPropertyType_AssociationProperty:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition(IntPtr(p), autoDispose);
		}
		case FdoPropertyType_RasterProperty:
		{
			return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::RasterPropertyDefinition(IntPtr(p), autoDispose);
		}
		default:
			return nullptr;
	}
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyDataPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyPropertyDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSchemaAttributeDictionary(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSchemaElement(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
	    return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

	if (dynamic_cast<FdoPropertyDefinition*>(p))	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^>(CreatePropertyDefinition(IntPtr(p), autoDispose));
	if (dynamic_cast<FdoClassDefinition*>(p))	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^>(CreateClassDefinition(IntPtr(p), autoDispose));

    if (wrap = CHECK<FdoFeatureSchema, NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_RASTER::DataValueCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValueCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_RASTER::DataValueCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_RASTER::IRaster^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIRaster(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_RASTER::IRasterImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionary^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIRasterPropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_RASTER::IRasterPropertyDictionaryImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateRasterDataModel(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_RASTER::RasterDataModel(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValue(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoDataValue* p = static_cast<FdoDataValue*>(ptr.ToPointer());

	switch(p->GetDataType())
	{
		case FdoDataType_Boolean:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_Byte:
		{	
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_DateTime:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_Decimal:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_Double:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_Int16:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value(IntPtr(p), autoDispose);
		}
		case FdoDataType_Int32:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value(IntPtr(p), autoDispose);
		}
		case FdoDataType_Int64:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value(IntPtr(p), autoDispose);
		}
		case FdoDataType_Single:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_String:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_BLOB:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::BLOBValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_CLOB:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue(IntPtr(p), autoDispose);
		}
		default:
			return nullptr;
	}
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateExpression(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

	if (wrap = CHECK<FdoGeometryValue, NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);
	if (dynamic_cast<FdoDataValue*>(p))	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(CreateDataValue(IntPtr(p), autoDispose));
	if (wrap = CHECK<FdoParameter, NAMESPACE_OSGEO_FDO_EXPRESSION::Parameter>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);
	if (wrap = CHECK<FdoLiteralValue, NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);
	if (wrap = CHECK<FdoComputedIdentifier, NAMESPACE_OSGEO_FDO_EXPRESSION::ComputedIdentifier>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);

	if (wrap = CHECK<FdoUnaryExpression, NAMESPACE_OSGEO_FDO_EXPRESSION::UnaryExpression>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);
	if (wrap = CHECK<FdoValueExpression, NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);
	if (wrap = CHECK<FdoBinaryExpression, NAMESPACE_OSGEO_FDO_EXPRESSION::BinaryExpression>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);
	if (wrap = CHECK<FdoIdentifier, NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);
	if (wrap = CHECK<FdoFunction, NAMESPACE_OSGEO_FDO_EXPRESSION::Function>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::Expression(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateExpressionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifier(System::IntPtr ptr, System::Boolean autoDispose)
{
    if (ptr == IntPtr::Zero)
	    return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

    if (wrap = CHECK<FdoComputedIdentifier, NAMESPACE_OSGEO_FDO_EXPRESSION::ComputedIdentifier>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateLiteralValue(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

	if (wrap = CHECK<FdoGeometryValue, NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue^>(wrap);
    if (dynamic_cast<FdoDataValue*>(p))	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue^>(CreateDataValue(IntPtr(p), autoDispose));

    return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateLOBValue(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoLOBValue* p = static_cast<FdoLOBValue*>(ptr.ToPointer());

	switch(p->GetDataType())
	{
		case FdoDataType_BLOB:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::BLOBValue(IntPtr(p), autoDispose);
		}
		case FdoDataType_CLOB:
		{
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue(IntPtr(p), autoDispose);
		}
		default:
			return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue(ptr, autoDispose);
	}
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateValueExpression(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

    if (wrap = CHECK<FdoParameter, NAMESPACE_OSGEO_FDO_EXPRESSION::Parameter>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(wrap);
	if (wrap = CreateLiteralValue(IntPtr(p), autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::ValueExpression(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValueCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateEDataValueCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_EXPRESSION::DataValueCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_FILTER::Filter^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFilter(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

	if (wrap = CHECK<FdoDistanceCondition, NAMESPACE_OSGEO_FDO_FILTER::DistanceCondition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);
	if (wrap = CHECK<FdoSpatialCondition, NAMESPACE_OSGEO_FDO_FILTER::SpatialCondition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);
	
	if (wrap = CHECK<FdoBinaryLogicalOperator, NAMESPACE_OSGEO_FDO_FILTER::BinaryLogicalOperator>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);
	if (wrap = CHECK<FdoComparisonCondition, NAMESPACE_OSGEO_FDO_FILTER::ComparisonCondition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);
	if (wrap = CHECK<FdoGeometricCondition, NAMESPACE_OSGEO_FDO_FILTER::GeometricCondition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);
	if (wrap = CHECK<FdoInCondition, NAMESPACE_OSGEO_FDO_FILTER::InCondition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);
	if (wrap = CHECK<FdoNullCondition, NAMESPACE_OSGEO_FDO_FILTER::NullCondition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);
	if (wrap = CHECK<FdoUnaryLogicalOperator, NAMESPACE_OSGEO_FDO_FILTER::UnaryLogicalOperator>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);

	if (wrap = CHECK<FdoSearchCondition, NAMESPACE_OSGEO_FDO_FILTER::SearchCondition>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);
	if (wrap = CHECK<FdoLogicalOperator, NAMESPACE_OSGEO_FDO_FILTER::LogicalOperator>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_FILTER::Filter^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_FILTER::Filter(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateValueExpressionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_FILTER::ValueExpressionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionInfo(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfoImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionPropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransaction^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateITransaction(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS::ITransactionImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateArgumentDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFunctionDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFunctionDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateICommandCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ICommandCapabilitiesImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIConnectionCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IConnectionCapabilitiesImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIExpressionCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilitiesImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIFilterCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IFilterCapabilitiesImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIGeometryCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IGeometryCapabilitiesImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIRasterCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IRasterCapabilitiesImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateISchemaCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ISchemaCapabilitiesImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateITopologyCapabilities(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilitiesImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyArgumentDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinition(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinition(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSignatureDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlySignatureDefinitionCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS::ICommand^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateICommand(System::IntPtr ptr, System::Boolean autoDispose, NAMESPACE_OSGEO_FDO_COMMANDS::CommandType type)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    switch(type)
    {
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_Select:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_Insert:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsertImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_Delete:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDeleteImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_Update:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IUpdateImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_DescribeSchema:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IDescribeSchemaImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_DescribeSchemaMapping:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IDescribeSchemaMappingImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ApplySchema:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IApplySchemaImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_DestroySchema:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IDestroySchemaImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ActivateSpatialContext:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IActivateSpatialContextImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_CreateSpatialContext:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ICreateSpatialContextImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_DestroySpatialContext:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IDestroySpatialContextImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetSpatialContexts:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::IGetSpatialContextsImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_CreateMeasureUnit:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::ICreateMeasureUnitImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_DestroyMeasureUnit:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IDestroyMeasureUnitImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetMeasureUnits:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IGetMeasureUnitsImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_SQLCommand:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLCommandImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_AcquireLock:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IAcquireLockImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetLockInfo:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockInfoImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetLockedObjects:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockedObjectsImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetLockOwners:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IGetLockOwnersImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ReleaseLock:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::IReleaseLockImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ActivateLongTransaction:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IActivateLongTransactionImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_DeactivateLongTransaction:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IDeactivateLongTransactionImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_CommitLongTransaction:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICommitLongTransactionImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_CreateLongTransaction:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetLongTransactions:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_FreezeLongTransaction:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IFreezeLongTransactionImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_RollbackLongTransaction:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ActivateLongTransactionCheckpoint:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IActivateLongTransactionCheckpointImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_CreateLongTransactionCheckpoint:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ICreateLongTransactionCheckpointImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetLongTransactionCheckpoints:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionCheckpointsImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_RollbackLongTransactionCheckpoint:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IRollbackLongTransactionCheckpointImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ChangeLongTransactionPrivileges:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionPrivilegesImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetLongTransactionPrivileges:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionPrivilegesImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ChangeLongTransactionSet:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IChangeLongTransactionSetImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetLongTransactionsInSet:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::IGetLongTransactionsInSetImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_SelectAggregates:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::ISelectAggregatesImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_FirstProviderCommand:
        {
            // No such command 
            return nullptr;
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_CreateDataStore:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::ICreateDataStoreImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_DestroyDataStore:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDestroyDataStoreImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ListDataStores:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IListDataStoresImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetClassNames:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IGetClassNamesImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_GetSchemaNames:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IGetSchemaNamesImp(ptr, autoDispose);
        }
        case NAMESPACE_OSGEO_FDO_COMMANDS::CommandType::CommandType_ExtendedSelect:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IExtendedSelectImp(ptr, autoDispose);
        }
        default:
        {
            return gcnew NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(ptr, autoDispose);
        }
    }
}

NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateBatchParameterValueCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIdentifierCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS::IdentifierCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValue(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValue(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateParameterValueCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS::ParameterValueCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValue(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateISQLDataReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDataReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IDataReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIFeatureReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

	if (wrap = CHECK<FdoXmlFeatureReader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIScrollableFeatureReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

	if (wrap = CHECK<FdoXmlFeatureReader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureReader>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IScrollableFeatureReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMappingCollection(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMapping(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.

	if (wrap = CHECK<FdoXmlSchemaMapping, NAMESPACE_OSGEO_FDO_XML::XmlSchemaMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalElementMapping(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	FdoIDisposable* p = (FdoIDisposable*)ptr.ToPointer();

	NAMESPACE_OSGEO_RUNTIME::Disposable^ wrap;

	// Note:
	// Here we need keep dynamic_cast to decide the real type of "ptr" because there is more than one possibility.
    
	if (wrap = CreateXmlClassMapping(IntPtr(p), autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^>(wrap);
	
    if (wrap = CHECK<FdoPhysicalClassMapping, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^>(wrap);
    if (wrap = CHECK<FdoPhysicalPropertyMapping, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping>(p, autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^>(wrap);
    if (wrap = CreatePhysicalSchemaMapping(IntPtr(p), autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^>(wrap);
    if (wrap = CreateXmlElementMapping(IntPtr(p), autoDispose)) return static_cast<NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping^>(wrap);

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIMeasureUnitReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_UNITOFMEASURE::IMeasureUnitReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILockConflictReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockConflictReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILockedObjectReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockedObjectReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILockOwnersReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LOCKING::ILockOwnersReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateISpatialContextReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT::ISpatialContextReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILongTransactionCheckpointReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionCheckpointReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILongTransactionReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionSetReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILongTransactionSetReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionSetReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILongTransactionPrivilegeReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionPrivilegeReaderImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumerator^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateILongTransactionConflictDirectiveEnumerator(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

	return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_LONGTRANSACTION::ILongTransactionConflictDirectiveEnumeratorImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionary^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDataStorePropertyDictionary(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStorePropertyDictionaryImp(ptr, autoDispose);
}

NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReader^ NAMESPACE_OSGEO_FDO::ObjectFactory::CreateIDataStoreReader(System::IntPtr ptr, System::Boolean autoDispose)
{
	if (ptr == IntPtr::Zero)
		return nullptr;

    return gcnew NAMESPACE_OSGEO_FDO_COMMANDS_DATASTORE::IDataStoreReaderImp(ptr, autoDispose);
}
