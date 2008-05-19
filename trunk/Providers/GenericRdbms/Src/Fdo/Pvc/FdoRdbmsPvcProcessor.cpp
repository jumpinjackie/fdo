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

#include "stdafx.h"
#include "FdoRdbmsConnection.h"
#include "Fdo/Pvc/FdoRdbmsPvcProcessor.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include <Sm/Lp/PropertyMappingConcrete.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include <Sm/Lp/ObjectPropertyClass.h>

FdoRdbmsPvcProcessor::FdoRdbmsPvcProcessor(  )
{
}

FdoRdbmsPvcProcessor::FdoRdbmsPvcProcessor( FdoRdbmsConnection *connection )
{
	mFdoConnection = connection; 
	if( mFdoConnection )
		mFdoConnection->AddRef();
	mInsertHandler = new FdoRdbmsPvcInsertHandler( mFdoConnection );
	mUpdateHandler = new FdoRdbmsPvcUpdateHandler( mFdoConnection );
}

FdoRdbmsPvcProcessor::~FdoRdbmsPvcProcessor()
{
	if( mInsertHandler )
		delete mInsertHandler;  // Need to free mInsertHandler before releasing the connection as the cleanup code may use the connection
	if( mUpdateHandler )
		delete mUpdateHandler;  // Need to free mUpdateHandler before releasing the connection as the cleanup code may use the connection

	if( mFdoConnection )
		mFdoConnection->Release();
}

FdoRdbmsCollection<FdoRdbmsPvcOperation>* FdoRdbmsPvcProcessor::RefactorPvc( FdoPropertyValueCollection* inputPropertyValues, const FdoSmLpClassDefinition* smLpClassDef, bool forUpdate ) const
{
    const FdoSmLpPropertyDefinition *propertyDefinition = NULL;
    const FdoSmLpObjectPropertyDefinition* objProp = NULL;
    const FdoSmLpClassDefinition* currentClass = NULL;
    const FdoSmLpClassDefinition* parentClass = NULL;
    FdoPtr<FdoPropertyValueCollection>propertyValues;
    const wchar_t *property = NULL;
    FdoRdbmsCollection<FdoRdbmsPvcOperation> *collection = new FdoRdbmsCollection<FdoRdbmsPvcOperation>;
    const FdoSmLpDataPropertyDefinitionCollection *sourceProperties;
    const FdoSmLpDataPropertyDefinitionCollection *targetProperties;
    const FdoSmLpDataPropertyDefinitionCollection *identities = NULL;
    bool  valueProperty = true;
    bool  associationProperty = false;

    FdoPtr<FdoPropertyValueCollection>fdoIdentities = FdoPropertyValueCollection::Create();
    currentClass = smLpClassDef;
    while (currentClass)
    {
        identities = currentClass->RefIdentityProperties();
        currentClass = currentClass->RefBaseClass();
    }
    if (identities)
    {
        for (int i=0; i<identities->GetCount(); i++)
        {
            const FdoSmLpDataPropertyDefinition *item = identities->RefItem(i);
            const wchar_t *identitiyName = item->GetName();

            for (int j=0; j<inputPropertyValues->GetCount(); j++)
            {
                FdoPtr<FdoPropertyValue>value = inputPropertyValues->GetItem(j);
                FdoPtr<FdoIdentifier>identifier = value->GetName();
                property = identifier->GetText();
                if (wcscmp((wchar_t *)property, identitiyName) == 0)
                {
                    fdoIdentities->Add(value);
                    break;
                }
            }
        }
    }

    //
    // Scan the property collection to find the maximum depth of the object properties:
    // For example a property collection having the following properties:
    // prop1
    // obj1.prop11
    // obj1.obj2.prop121
    // prop2
    // obj1.prop12
    // Would have a maximum depth of 3
    int     maximumDepth = 0;
    for (int i=0; i<inputPropertyValues->GetCount(); i++)
    {
        FdoPtr<FdoPropertyValue>value = inputPropertyValues->GetItem(i);
        FdoPtr<FdoIdentifier>identifier = value->GetName();
        FdoStringsP strings = FdoStringCollection::Create(identifier->GetText(), L".");
        if( strings->GetCount() > maximumDepth )
            maximumDepth = strings->GetCount();
    }
    //
    // We need to do a multi-scan of the property value collection an build the resulting property value collection one level at a
    // time starting by the root class. For example the result of processing the collection described above would be:
    // Pass1: would produce a collection that contain properties: (prop1, prop2)
    // Pass2: would produce a collection that contain properties: (prop11, prop12)
    // Pass3: would produce a collection that contain properties: (prop121)
    // Any association property, that use the same property format as object properties(assname.prop)
    // should not be handled as an object property. Association property are kept with their "." notation.
    // Also the necessary indentity properties and values will be added to the various collections.
    for (int currentDepth=1; currentDepth <= maximumDepth; currentDepth++ )
    {
        for (int i=0; i<inputPropertyValues->GetCount(); i++)
        {
            currentClass = smLpClassDef;
            FdoPtr<FdoPropertyValue>value = inputPropertyValues->GetItem(i);
            FdoPtr<FdoIdentifier>identifier = value->GetName();

            property = identifier->GetText();
            FdoStringsP strings = FdoStringCollection::Create(property, L".");
            FdoPtr<FdoValueExpression>dataValue;
            FdoPtr<FdoIStreamReader>streamReader;

            dataValue = value->GetValue();
            streamReader = value->GetStreamReader();

            valueProperty = true;
            if ( dataValue == NULL && streamReader == NULL)
                continue;

            if( strings->GetCount() != currentDepth )
                continue;

            associationProperty = false;
			int j;
            for ( j=0; j<strings->GetCount()-1; j++)
            {
                const wchar_t* objPropName = strings->GetString(j);
                propertyDefinition = currentClass->RefProperties()->RefItem( objPropName );

                // Ignore the association properties, they too use the "." notation
                if( propertyDefinition && propertyDefinition->GetPropertyType() == FdoPropertyType_AssociationProperty )
                {
                    associationProperty = true;
                    break;
                }

                if( ! propertyDefinition || propertyDefinition->GetPropertyType() != FdoPropertyType_ObjectProperty )
                    throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_24, "Expected object property"));

                bool   isSingleMapping = false;
                objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
                const FdoSmLpPropertyMappingDefinition* mappping = objProp->RefMappingDefinition();
                switch( mappping->GetType() )
                {
                    case FdoSmLpPropertyMappingType_Concrete:
                        {
                        const FdoSmLpPropertyMappingConcrete * concMapping = static_cast<const FdoSmLpPropertyMappingConcrete*>( mappping );
                        sourceProperties = concMapping->RefSourceProperties();
                        targetProperties = concMapping->RefTargetProperties();

                        parentClass = currentClass;
                        currentClass = concMapping->RefTargetClass();
                        }
                        break;

                    case FdoSmLpPropertyMappingType_Single:
                        {
                        const FdoSmLpPropertyMappingSingle * singleMapping = static_cast<const FdoSmLpPropertyMappingSingle*>( mappping );
                        sourceProperties = NULL;
                        targetProperties = NULL;

                        parentClass = currentClass;
                        currentClass = singleMapping->RefTargetClass();
                        FdoPtr<FdoRdbmsPvcOperation> utilClass;
                        FdoPtr<FdoPropertyValueCollection>props;
                        int k = 0;
                        for (k=0; k<collection->GetCount(); k++)
                        {
                            utilClass = collection->GetItem(k);
                            if (utilClass->GetClass() == parentClass)
                            {
                                props = utilClass->GetProperties();
                                break;
                            }
                        }
                        if( props == NULL ) // Should never happen
                            throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
                        utilClass = new FdoRdbmsPvcOperation((FdoSmLpClassDefinition *)currentClass, props, objPropName );
                        
						if( forUpdate )
							utilClass->SetPvcHandler( (const FdoRdbmsPvcHandler*)mUpdateHandler );
						else
							utilClass->SetPvcHandler( (const FdoRdbmsPvcHandler*)mInsertHandler );
						
						collection->Add(utilClass);
                        isSingleMapping = true;
                        }
                        break;

                    default:
                        throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type"));
                }

                // Let's see if this class already has entry
                FdoRdbmsPvcOperation *clas = NULL;
                int k = 0;
                for (k=0; k<collection->GetCount(); k++)
                {
                    clas = collection->GetItem(k);
                    if (clas->GetClass() == currentClass)
                    {
                        clas->Release();
                        break;
                    }
                    clas->Release();
                }
                if( k == collection->GetCount() )
                {
                    // We need to add a property collection for this intermediate class
                    // And then continue with the rest of the chain. For this we decrement 'i' so that we revisit
                    // this object property again to add its property.
                    i--; // Need to scan this property again
                    valueProperty = false;
                    break;
                }
            }

            const wchar_t* name  = strings->GetString(j);

            FdoPtr<FdoRdbmsPvcOperation>clas;
            for (j=0; j<collection->GetCount(); j++)
            {
                clas = collection->GetItem(j);
                if (clas->GetClass() == currentClass)
                    break;
            }

            // add an entry if neceesary
            if (j == collection->GetCount() )
            {
                // Start fix
                if( parentClass != NULL )
                {
                    identities = parentClass->RefIdentityProperties();
                    clas = NULL;
                    for (j=0; j<collection->GetCount(); j++)
                    {
                        clas = collection->GetItem(j);
                        if (clas->GetClass() == parentClass)
                            break;
                    }

                    if (identities)
                    {
                        fdoIdentities->Clear();
                        for (int i=0; i<identities->GetCount(); i++)
                        {
                            const FdoSmLpDataPropertyDefinition *item = identities->RefItem(i);
                            const wchar_t *identitiyName = item->GetName();

                            FdoPtr<FdoPropertyValueCollection>classProperty =  clas->GetProperties();
                            for (int j=0; j<classProperty->GetCount(); j++)
                            {
                                FdoPtr<FdoPropertyValue>value = classProperty->GetItem(j);
                                FdoPtr<FdoIdentifier>identifier = value->GetName();
                                property = identifier->GetText();
                                if (wcscmp((wchar_t *)property, identitiyName) == 0)
                                {
                                    fdoIdentities->Add(value);
                                    break;
                                }
                            }
                        }
                    }
                }
                // End fix
                clas = new FdoRdbmsPvcOperation((FdoSmLpClassDefinition *)currentClass);
				if( forUpdate )
					clas->SetPvcHandler( (const FdoRdbmsPvcHandler*)mUpdateHandler );
				else
					clas->SetPvcHandler( (const FdoRdbmsPvcHandler*)mInsertHandler );
                collection->Add(clas);

                if (strings->GetCount() != 1 && ! associationProperty && sourceProperties != NULL )
                {

                    propertyValues = clas->GetProperties();
                    for (j=0; j<fdoIdentities->GetCount(); j++)
                    {
                        FdoPtr<FdoPropertyValue>propValue = fdoIdentities->GetItem(j);
                        FdoPtr<FdoValueExpression>valueExpression = propValue->GetValue();
                        for (int k=0; k<sourceProperties->GetCount(); k++)
                        {
                            const FdoSmLpDataPropertyDefinition *sourceProperty = sourceProperties->RefItem(k);
                            const wchar_t *sourceName = sourceProperty->GetName();
                            FdoPtr<FdoIdentifier>id = propValue->GetName();
                            if (wcscmp(sourceName, id->GetName()) == 0)
                            {
                                const FdoSmLpDataPropertyDefinition *targetProperty =targetProperties->RefItem(k);
                                propValue = FdoPropertyValue::Create(targetProperty->GetName(), valueExpression);
                                propertyValues->Add(propValue);
                                break;
                            }
                        }
                    }
                }
            }


            if( valueProperty || associationProperty )
            {
                FdoStringP assoPropName;
                // If it's an association property, build the scope and name as <association name>.<identity prop>
                if( associationProperty )
                {
                    assoPropName = FdoStringP(strings->GetString(j)) + L"." + strings->GetString(j+1);
                    name  = (const wchar_t*)assoPropName;
                }
                else if( clas->GetScope()[0] != '\0' )
                {
                    assoPropName = FdoStringP(clas->GetScope()) + L"." + name;
                    name  = (const wchar_t*)assoPropName;
                }

                FdoPtr<FdoPropertyValue>propertyValue = FdoPropertyValue::Create(name, dataValue);
                if ( streamReader != NULL )
                    propertyValue->SetStreamReader(streamReader);

                propertyValues = clas->GetProperties();
                propertyValues->Add(propertyValue);
            }
        }
    }
    return collection;
}

void FdoRdbmsPvcProcessor::SetPvcOperationHandler( const FdoSmLpClassDefinition* smLpClassDef, FdoRdbmsPvcOperation  *pvcOp ) const
{

}
