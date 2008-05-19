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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpDescribeSchemaCommand.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpDescribeSchemaCommand.h"
#include "FdoRfpConnection.h"

FdoRfpDescribeSchemaCommand::FdoRfpDescribeSchemaCommand(FdoIConnection *connection) :
	FdoRfpCommand<FdoIDescribeSchema>(connection)
{
}

FdoRfpDescribeSchemaCommand::~FdoRfpDescribeSchemaCommand(void)
{
}

void FdoRfpDescribeSchemaCommand::SetSchemaName(const wchar_t* value)
{
	m_schemaName = value;
}

const wchar_t* FdoRfpDescribeSchemaCommand::GetSchemaName()
{
	return m_schemaName;
}

FdoFeatureSchemaCollection* FdoRfpDescribeSchemaCommand::Execute()
{
    FdoRfpConnectionP connection = static_cast<FdoRfpConnection*>(GetConnection());
    FdoFeatureSchemaCollection* ret = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoFeatureSchemaCollection> cachedSchemas = connection->GetFeatureSchemas();
    FdoInt32 count = cachedSchemas->GetCount();
    if (m_schemaName.GetLength() == 0)
    {
        for (int i = 0; i < count; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = cachedSchemas->GetItem(i);
            schema = _cloneSchema(schema);
            ret->Add(schema);
            schema->AcceptChanges();
        }
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = cachedSchemas->GetItem (i);
            if (STRCASEEQ(m_schemaName, schema->GetName ()))
            {
                schema = _cloneSchema (schema);
                ret->Add (schema);
                schema->AcceptChanges ();
                break;
            }
        }
        if (ret->GetCount() == 0)
            throw FdoCommandException::Create(NlsMsgGet1(GRFP_48_FEATURE_SCHEMA_NOT_FOUND, "Feature schema '%1$ls' not found.", (const wchar_t*)m_schemaName));
    }
    return ret;
}

FdoClassDefinitionP FdoRfpDescribeSchemaCommand::_cloneClass(const FdoPtr<FdoClassDefinition>& definition)
{
    FdoClassDefinitionP ret;

    if (FdoClassType_FeatureClass == definition->GetClassType ())
        ret = FdoFeatureClass::Create (definition->GetName (), definition->GetDescription ());
    else
        ret = FdoClass::Create (definition->GetName (), definition->GetDescription ());

    FdoPtr<FdoPropertyDefinitionCollection> clone = ret->GetProperties ();
    FdoPtr<FdoPropertyDefinitionCollection> properties = definition->GetProperties ();
    for (int i = 0; i < properties->GetCount (); i++)
    {
        FdoPtr<FdoPropertyDefinition> property = properties->GetItem (i);
        FdoPtr<FdoPropertyDefinition> x;
        switch (property->GetPropertyType ())
        {
          case FdoPropertyType_DataProperty:
          {
              FdoDataPropertyDefinition* data_property = (FdoDataPropertyDefinition*)property.p;
              FdoDataPropertyDefinition* data_definition = FdoDataPropertyDefinition::Create (data_property->GetName (), data_property->GetDescription ());
              data_definition->SetDataType (data_property->GetDataType ());
              data_definition->SetLength (data_property->GetLength ());
              data_definition->SetPrecision (data_property->GetPrecision ());
              data_definition->SetScale (data_property->GetScale ());
              data_definition->SetNullable (data_property->GetNullable ());
              data_definition->SetReadOnly (data_property->GetReadOnly ());
              data_definition->SetDefaultValue (data_property->GetDefaultValue ());
              x = data_definition;
          }
          break;
          case FdoPropertyType_RasterProperty:
          {
              FdoRasterPropertyDefinition* rasterProperty = (FdoRasterPropertyDefinition*)property.p;
              FdoRasterPropertyDefinition* rasterDef = FdoRasterPropertyDefinition::Create(rasterProperty->GetName(), rasterProperty->GetDescription());
              FdoPtr<FdoRasterDataModel> dataModel = rasterProperty->GetDefaultDataModel();
              rasterDef->SetDefaultDataModel(dataModel);
              rasterDef->SetDefaultImageXSize(rasterProperty->GetDefaultImageXSize());
              rasterDef->SetDefaultImageYSize(rasterProperty->GetDefaultImageYSize());
              rasterDef->SetNullable(rasterProperty->GetNullable());
              rasterDef->SetReadOnly(rasterProperty->GetReadOnly());
              rasterDef->SetSpatialContextAssociation(rasterProperty->GetSpatialContextAssociation());
              x = rasterDef;
          }
          break;
          default:
            throw FdoException::Create(L"Unreachable exception.");
            break;
        }
        // copy the attributes
        FdoPtr<FdoSchemaAttributeDictionary> attributes = property->GetAttributes ();
        FdoPtr<FdoSchemaAttributeDictionary> y = x->GetAttributes ();
        FdoInt32 length;
        FdoString** names = attributes->GetAttributeNames (length);
        for (int i = 0; i < length; i++)
        {
            FdoString* name = names[i];
            y->Add (name, attributes->GetAttributeValue (name));
        }
        clone->Add (x);
    }
    // set identity properties
    FdoPtr<FdoDataPropertyDefinitionCollection> identities = definition->GetIdentityProperties ();
    FdoPtr<FdoDataPropertyDefinitionCollection> z = ret->GetIdentityProperties ();
    for (int i = 0; i < identities->GetCount (); i++)
    {
        FdoPtr<FdoDataPropertyDefinition> q = (FdoDataPropertyDefinition*)identities->GetItem (i);
        FdoPtr<FdoDataPropertyDefinition> m = (FdoDataPropertyDefinition*)clone->GetItem (q->GetName ());
        z->Add (m);
    }
    // set geometry property
    if (FdoClassType_FeatureClass == definition->GetClassType ())
    {
        FdoFeatureClassP r = SP_STATIC_CAST(FdoFeatureClass, definition);
        FdoFeatureClassP s = SP_STATIC_CAST(FdoFeatureClass, ret);
        FdoPtr<FdoGeometricPropertyDefinition> t = r->GetGeometryProperty ();
        if (t != NULL)
        {
            FdoPtr<FdoGeometricPropertyDefinition> n = (FdoGeometricPropertyDefinition*)clone->GetItem (t->GetName ());
            s->SetGeometryProperty (n);
        }
    }
    // copy the attributes
    FdoPtr<FdoSchemaAttributeDictionary> u = definition->GetAttributes ();
    FdoPtr<FdoSchemaAttributeDictionary> v = ret->GetAttributes ();
    FdoInt32 length;
    FdoString** names = u->GetAttributeNames (length);
    for (int i = 0; i < length; i++)
    {
        FdoString* name = names[i];
        v->Add (name, u->GetAttributeValue (name));
    }

    return (ret);

}
FdoPtr<FdoFeatureSchema> FdoRfpDescribeSchemaCommand::_cloneSchema(const FdoPtr<FdoFeatureSchema>& schema)
{
    FdoPtr<FdoFeatureSchema> ret = FdoFeatureSchema::Create (schema->GetName (), schema->GetDescription ());

    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    FdoPtr<FdoClassCollection> clone = ret->GetClasses ();
    for (int i = 0; i < classes->GetCount (); i++)
    {
        FdoPtr<FdoClassDefinition> definition = classes->GetItem (i);
        definition = _cloneClass (definition);
        clone->Add (definition);
    }
    // copy the attributes
    FdoPtr<FdoSchemaAttributeDictionary> u = schema->GetAttributes ();
    FdoPtr<FdoSchemaAttributeDictionary> v = ret->GetAttributes ();
    FdoInt32 length;
    FdoString** names = u->GetAttributeNames (length);
    for (int i = 0; i < length; i++)
    {
        FdoString* name = names[i];
        v->Add (name, u->GetAttributeValue (name));
    }
    // mark the schema as existing
    ret->AcceptChanges ();

    return (ret);
}



