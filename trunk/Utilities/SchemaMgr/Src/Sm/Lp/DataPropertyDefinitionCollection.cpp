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
#include <Sm/Lp/DataPropertyDefinitionCollection.h>

const FdoSmLpDataPropertyDefinition* FdoSmLpDataPropertyDefinitionCollection::ColName2Property( 
	const FdoSmLpPropertyDefinitionCollection* pProperties,
	FdoStringP columnName 
)
{
    return FdoSmLpDataPropertyDefinition::Cast( 
        FdoSmLpSimplePropertyDefinition::ColName2Property( pProperties, columnName )
    );
}

const FdoSmLpDataPropertyDefinition* FdoSmLpDataPropertyDefinitionCollection::RefFeatIDProperty( 
	const FdoSmLpPropertyDefinitionCollection* pProperties
)
{
    return (FdoSmLpDataPropertyDefinition*) FdoSmLpDataPropertyDefinitionCollection::FindFeatIDProperty(
        FDO_SAFE_ADDREF((FdoSmLpPropertyDefinitionCollection*) pProperties)
    );
}

FdoSmLpDataPropertyP FdoSmLpDataPropertyDefinitionCollection::FindFeatIDProperty( 
	FdoSmLpPropertiesP pProperties
)
{
	FdoSmLpDataPropertyP pMatchedProp;

	// Search the collection for the FeatId property
	for ( int i = 0; i < pProperties->GetCount(); i++ ) {
		FdoSmLpDataPropertyP pDataProp = pProperties->GetItem(i)->SmartCast<FdoSmLpDataPropertyDefinition>(true);

		// Only Data properties can be FeatId propertyes
		if ( pDataProp ) {
			if ( pDataProp->GetIsFeatId() ) {
/*
				if ( pMatchedProp ) 
					throw FdoSchemaException::Create( 
						NlsMsgGet2( 
							FDORDBMS_184, 
							"Property collection contains at least two Feature Id properties ( %1$ls and %2$ls )",
							pMatchedProp->GetName(),
							pDataProp->GetName()
						)
					);
				else
*/
					pMatchedProp = pDataProp;
			}
		}
	}

	return(pMatchedProp);
}
