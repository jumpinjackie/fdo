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
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/Schema.h>
#include "PropertyMappingConcrete.h"
#include "ObjectPropertyClass.h"

FdoSmLpGrdPropertyMappingConcrete::FdoSmLpGrdPropertyMappingConcrete( 
	FdoSmLpObjectPropertyDefinition* pParent,
	FdoSmLpClassDefinition* pParentType,
    FdoRdbmsOvPropertyMappingConcrete* pOverrides
) :
	FdoSmLpPropertyMappingConcrete(pParent, pParentType, pOverrides)
{
    FdoRdbmsOvPropertyMappingConcrete* pConcreteOverrides =
        dynamic_cast<FdoRdbmsOvPropertyMappingConcrete*>(pOverrides);

    const FdoSmLpObjectPropertyDefinition* pParentPrev = 
        FdoSmLpObjectPropertyDefinition::Cast( pParent->GetPrevProperty() );
    
    const FdoSmLpPropertyMappingConcrete* pParentMapping = NULL;

    if ( pParentPrev ) {
        pParentMapping = 
            FdoSmLpPropertyMappingConcrete::Cast( pParentPrev->RefMappingDefinition() );
    }

	if ( pParentType ) {
		Setup( pParent->NewClass(
                        pParent, 
                        pParentType, 
                        this, 
                        FdoSmLpPropertyMappingType_Concrete, 
                        pConcreteOverrides ? pConcreteOverrides->GetInternalClass() : NULL
                   ) 
        );
	}
}

FdoSmLpGrdPropertyMappingConcrete::FdoSmLpGrdPropertyMappingConcrete( 
	const FdoSmLpPropertyMappingConcrete* pBase,
	FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
    FdoRdbmsOvPropertyMappingConcrete* pOverrides
) :
	FdoSmLpPropertyMappingConcrete(pBase, pParent, pParentType, pOverrides)
{
    FdoRdbmsOvPropertyMappingConcrete* pConcreteOverrides =
        dynamic_cast<FdoRdbmsOvPropertyMappingConcrete*>(pOverrides);

    if ( pParentType ) {
		Setup( pParent->NewClass(
                        FDO_SAFE_ADDREF( (FdoSmLpObjectPropertyClass*)(pBase->RefTargetClass()) ), 
                        pParent, 
                        pParentType, 
                        this, 
                        FdoSmLpPropertyMappingType_Concrete, 
                        pConcreteOverrides ? pConcreteOverrides->GetInternalClass() : NULL
                    ) 
        );
	}
}

bool FdoSmLpGrdPropertyMappingConcrete::AddSchemaMappings( FdoPhysicalPropertyMappingP propMapping, bool bIncludeDefaults ) const
{
    FdoRdbmsOvObjectPropertyDefinition* grdPropMapping = 
        dynamic_cast<FdoRdbmsOvObjectPropertyDefinition*>((FdoPhysicalPropertyMapping*)propMapping);
    bool bHasMappings = false;

    // Add the schema mappings for the particular type of mapping definition.
    if ( grdPropMapping) {
        FdoRdbmsOvPropertyMappingConcreteP concreteMapping =
            grdPropMapping->CreateConcreteMapping( true );

        if ( concreteMapping ) {
            if ( RefTargetClass() ) {
                FdoRdbmsOvClassP internalClass = concreteMapping->CreateInternalClass( true );
                FdoPhysicalClassMappingP classMapping = FDO_SAFE_ADDREF((FdoRdbmsOvClassDefinition*)internalClass);
                bHasMappings = RefTargetClass()->SetSchemaMappings(classMapping, bIncludeDefaults);
            }
        }
    }
   
    return bHasMappings;
}

