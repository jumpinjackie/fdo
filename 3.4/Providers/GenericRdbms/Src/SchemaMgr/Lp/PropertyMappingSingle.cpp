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
#include "PropertyMappingSingle.h"
#include "ObjectPropertyClass.h"

FdoSmLpGrdPropertyMappingSingle::FdoSmLpGrdPropertyMappingSingle( 
	FdoSmLpObjectPropertyDefinition* pParent,
	FdoSmLpClassDefinition* pParentType,
    FdoRdbmsOvPropertyMappingSingle* pOverrides
) :
	FdoSmLpPropertyMappingSingle(pParent, pParentType, pOverrides)
{
    const FdoSmLpObjectPropertyDefinition* pParentPrev = 
        FdoSmLpObjectPropertyDefinition::Cast( pParent->GetPrevProperty() );
    
    const FdoSmLpPropertyMappingSingle* pParentMapping = NULL;

    if ( pParentPrev ) {
        pParentMapping = 
            FdoSmLpPropertyMappingSingle::Cast( pParentPrev->RefMappingDefinition() );
    }

    SetupOverrides( pParentMapping, pParent, pOverrides, false );

	if ( pParentType ) {
		Setup( pParent->NewClass(
                        pParent, 
                        pParentType, 
                        this, 
                        FdoSmLpPropertyMappingType_Single, 
                        NULL
                   ) 
        );
	}
}

FdoSmLpGrdPropertyMappingSingle::FdoSmLpGrdPropertyMappingSingle( 
	const FdoSmLpPropertyMappingSingle* pBase,
	FdoSmLpObjectPropertyDefinition* pParent, 
	FdoSmLpClassDefinition* pParentType,
    FdoRdbmsOvPropertyMappingSingle* pOverrides
) :
	FdoSmLpPropertyMappingSingle(pBase, pParent, pParentType, pOverrides)
{
    SetupOverrides( pBase, pParent, pOverrides, true );

    if ( pParentType ) {
		Setup( pParent->NewClass(
                        FDO_SAFE_ADDREF( (FdoSmLpObjectPropertyClass*)(pBase->RefTargetClass()) ), 
                        pParent, 
                        pParentType, 
                        this, 
                        FdoSmLpPropertyMappingType_Single, 
                        NULL
                    ) 
        );
	}
}

bool FdoSmLpGrdPropertyMappingSingle::AddSchemaMappings( FdoPhysicalPropertyMappingP propMapping, bool bIncludeDefaults ) const
{
    FdoRdbmsOvObjectPropertyDefinition* grdPropMapping = 
        dynamic_cast<FdoRdbmsOvObjectPropertyDefinition*>((FdoPhysicalPropertyMapping*)propMapping);
    bool bHasMappings = false;

    // Add the schema mappings for the particular type of mapping definition.
    if ( grdPropMapping) {
        FdoRdbmsOvPropertyMappingSingleP singleMapping =
            grdPropMapping->CreateSingleMapping( true );

        if ( singleMapping ) {
            singleMapping->SetPrefix( GetPrefix() );
            bHasMappings = true;
        }
    }
   
    return bHasMappings;
}

void FdoSmLpGrdPropertyMappingSingle::SetupOverrides( 
	const FdoSmLpPropertyMappingSingle* pBase,
	FdoSmLpObjectPropertyDefinition* pParent, 
    FdoRdbmsOvPropertyMappingSingle* pOverrides,
    bool bInherit
)
{
    FdoSmPhMgrP mgr = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoStringP prefix = GetPrefix();

    // Extract the physical overrides.
    if ( pOverrides ) {
         prefix = pOverrides->GetPrefix();

         if ( prefix.GetLength() > 0 ) {
            if ( prefix != mgr->CensorDbObjectName(prefix) )
                AddPrefixCharError(prefix);

            if ( prefix.GetLength() > mgr->DbObjectNameMaxLen() ) 
                AddPrefixLengthError(prefix, mgr->DbObjectNameMaxLen());
         }
    }

    if ( prefix.GetLength() == 0 ) {
        if ( pBase ) {
            // Get them from base definition if this one is copied or inherited.
            prefix = pBase->GetPrefix();
        }

        // Generate default column prefix from object property name.
        if ( prefix.GetLength() == 0 ) {
            prefix = mgr->CensorDbObjectName(pParent->GetName()).Mid(0,25);
        }

        if ( (pParent->GetElementState() == FdoSchemaElementState_Added) && !bInherit ) {
            FdoSmLpObjectPropertyClass* pParentClass = 
                dynamic_cast<FdoSmLpObjectPropertyClass*>( (FdoSmLpSchemaElement*)(pParent->GetParent()) );
            // Pre-pend base property prefix to keep prefix as unique as possible.
            if ( pParentClass ) {
                const FdoSmLpPropertyMappingSingle* pParentMapping = 
                    FdoSmLpGrdPropertyMappingSingle::Cast( pParentClass->RefObjectProperty()->RefMappingDefinition() );

                if ( pParentMapping ) {
                    prefix = FdoStringP(pParentMapping->GetPrefix()) + L"_" + prefix;
                }
            }
        }
    }

    SetPrefix( prefix );
}

