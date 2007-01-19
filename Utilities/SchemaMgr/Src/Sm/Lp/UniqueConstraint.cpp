 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include <Sm/Lp/UniqueConstraint.h>

FdoSmLpUniqueConstraint::FdoSmLpUniqueConstraint()
{
	mProperties = new FdoSmLpDataPropertyDefinitionCollection();
}

FdoPtr<FdoSmLpUniqueConstraint> FdoSmLpUniqueConstraint::CreateInherited( FdoSmLpClassBase* pSubClass, FdoSmLpPropertyDefinitionCollection* pSubClassProperties )
{
    // Create the inherited constaint, making this constraint the base
    FdoPtr<FdoSmLpUniqueConstraint> inheritedConstraint = new FdoSmLpUniqueConstraint();
    inheritedConstraint->SetBaseConstraint( FDO_SAFE_ADDREF(this) );

    // Set the inherited constaint's properties to the sub-class equivalents of the base constraint
    // properties.
    FdoSmLpDataPropertiesP baseProperties = GetProperties();
    FdoSmLpDataPropertiesP subProperties = inheritedConstraint->GetProperties();

    FdoInt32 idx;
    bool propNotFound = false;

    for ( idx = 0; idx < baseProperties->GetCount(); idx++ ) {
        FdoSmLpDataPropertyP baseProperty = baseProperties->GetItem( idx );
        FdoSmLpPropertyP subProperty = pSubClassProperties->FindItem( baseProperty->GetName() );

        if ( subProperty && (subProperty->GetPropertyType() == FdoPropertyType_DataProperty) )
            subProperties->Add( (FdoSmLpDataPropertyDefinition*)(subProperty.p) );
        else
            // Property not in subclass so can't inherit constraint.
            propNotFound = true;
    }

    if ( propNotFound )
        inheritedConstraint = NULL;

    return inheritedConstraint;
}

FdoSmLpDataPropertyDefinitionCollection *FdoSmLpUniqueConstraint::GetProperties()
{
	return FDO_SAFE_ADDREF(mProperties.p);
}

const FdoSmLpDataPropertyDefinitionCollection *FdoSmLpUniqueConstraint::RefProperties() const
{
	return mProperties.p;
}

FdoPtr<FdoSmLpUniqueConstraint> FdoSmLpUniqueConstraint::GetBaseConstraint()
{
    return mBaseConstraint;
}

FdoInt32 FdoSmLpUniqueConstraint::Compare( FdoPtr<FdoSmLpUniqueConstraint> other ) const
{
    FdoInt32 result = -1;

    const FdoSmLpDataPropertyDefinitionCollection* myProps = RefProperties();
    const FdoSmLpDataPropertyDefinitionCollection* otherProps = other->RefProperties();
    
    if ( myProps->GetCount() == otherProps->GetCount() ) {
        FdoInt32 idx;
        result = 0;

        for ( idx = 0; idx < myProps->GetCount(); idx++ ) {
            const FdoSmLpDataPropertyDefinition* myProp = myProps->RefItem(idx);
            if ( !otherProps->RefItem(myProp->GetName()) ) {
                result = -1;
                break;
            }
        }
    }

    return result;
}

const FdoSmLpUniqueConstraint* FdoSmLpUniqueConstraint::RefBaseConstraint() const
{
    return ( ((FdoSmLpUniqueConstraint*) this)->GetBaseConstraint().p );
}

void FdoSmLpUniqueConstraint::SetBaseConstraint( FdoPtr<FdoSmLpUniqueConstraint> baseConstraint )
{
    mBaseConstraint = baseConstraint;
}

void FdoSmLpUniqueConstraint::Dispose()
{
	delete this;
}

void FdoSmLpUniqueConstraint::XMLSerialize( FILE* xmlFp, int ref  ) const
{

	if ( ref == 0 ) {
		fprintf( xmlFp, "<uniqueConstraint>\n" );

		for ( int i = 0; i < RefProperties()->GetCount(); i++ )
			RefProperties()->RefItem(i)->XMLSerialize(xmlFp, 1);
		fprintf( xmlFp, "</uniqueConstraint>\n" );

	}

}

