//
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
// 
//


#include <Fdo/Schema/PropertyValueConstraintList.h>
#include "XmlContext.h"
#include "StringUtility.h"

// Constructs a default instance of a FdoPropertyValueConstraintList.
FdoPropertyValueConstraintList::FdoPropertyValueConstraintList():m_constraintList( NULL )
{

}

FdoPropertyValueConstraintList::~FdoPropertyValueConstraintList()
{
	FDO_SAFE_RELEASE( m_constraintList );
}

	
///<summary>Constructs an empty instance of an FdoPropertyValueConstraintList.</summary>
/// <returns>Returns an FdoPropertyValueConstraintList</returns> 
FdoPropertyValueConstraintList* FdoPropertyValueConstraintList::Create( )
{
	return new FdoPropertyValueConstraintList();
}

///<summary>Returns FdoPropertyValueConstraintType_Range type.</summary>
///<returns>Returns the constraint type</returns>
FdoPropertyValueConstraintType FdoPropertyValueConstraintList::GetConstraintType()
{
	return FdoPropertyValueConstraintType_List;
}

///<summary>Returns the list of allowed values for a particular property.</summary>
/// <returns>Returns na instance of FdoDataValueCollection.</returns> 
FdoDataValueCollection* FdoPropertyValueConstraintList::GetConstraintList()
{
	if( m_constraintList == NULL )
		m_constraintList =  FdoDataValueCollection::Create();

	FDO_SAFE_ADDREF( m_constraintList );

	return m_constraintList;
}

void FdoPropertyValueConstraintList::Set( FdoPropertyValueConstraint* pConstraint, FdoString* parentName, FdoSchemaMergeContext* pContext )
{
    FdoPropertyValueConstraint::Set(pConstraint, parentName, pContext);

    // Base function adds error on constraint type mismatch so silently skip if 
    // types do not match.
    if ( pConstraint->GetConstraintType() == FdoPropertyValueConstraintType_List ) {
        FdoPropertyValueConstraintList* pListConstraint = (FdoPropertyValueConstraintList*) pConstraint;
        FdoPtr<FdoDataValueCollection> myConstraintList = GetConstraintList();
        myConstraintList->Clear();
        FdoPtr<FdoDataValueCollection> valueList = pListConstraint->GetConstraintList();

        // Copy constraint values to this element.

        FdoInt32 idx;
        for ( idx = 0; idx < valueList->GetCount(); idx++ ) {
            // For the purposes of XML reading and SDF Provider ApplySchema, reusing the value
            // is fine, but is not ok in the general case. 
            // TODO: Change the following to make a copy of the data value.
            myConstraintList->Add( FdoPtr<FdoDataValue>(valueList->GetItem(idx)) );
        }
    }
}

bool FdoPropertyValueConstraintList::Equals( FdoPropertyValueConstraint* pConstraint )
{
    bool equals = false;
    FdoInt32 idx;

    // Not equal if other constraint is not a list.
    if ( pConstraint->GetConstraintType() == FdoPropertyValueConstraintType_List ) {
        FdoPropertyValueConstraintList* pListConstraint = (FdoPropertyValueConstraintList*) pConstraint;
        FdoPtr<FdoDataValueCollection> valueList = pListConstraint->GetConstraintList();

        // Equal if value lists have same unique values. Build dictionaries from lists to 
        // weed out duplicate values.

        FdoDictionaryP myValues = ValuesToDictionary( m_constraintList );
        FdoDictionaryP theirValues = ValuesToDictionary( valueList );

        // Not equal if number of unique values differ
        if ( myValues->GetCount() == theirValues->GetCount() ) {

            // Constraint lists are not order-dependent so for each value in this constraint,
            // need to check if it is in the other constraint. The lists are equal if all
            // values in one list are in the other.

            equals = true;
            for ( idx = 0; idx < myValues->GetCount(); idx++ ) {
                FdoDictionaryElementP myValue = myValues->GetItem(idx);
                if ( !theirValues->Contains(myValue->GetName()) ) {
                    equals = false;
                    break;
                }
            }
        }
    }

    return equals;
}

bool FdoPropertyValueConstraintList::Contains( FdoPropertyValueConstraint* pConstraint )
{
    bool contains = false;
    FdoInt32 idx;

    // Not equal if other constraint is not a list.
    if ( pConstraint->GetConstraintType() == FdoPropertyValueConstraintType_List ) {
        FdoPropertyValueConstraintList* pListConstraint = (FdoPropertyValueConstraintList*) pConstraint;
        FdoPtr<FdoDataValueCollection> valueList = pListConstraint->GetConstraintList();

        // Contains other list if all other list values in this list. Build dictionaries from lists to 
        // weed out duplicate values.

        FdoDictionaryP myValues = ValuesToDictionary( m_constraintList );
        FdoDictionaryP theirValues = ValuesToDictionary( valueList );

        // Can't contain other constraint list if it has more values
        if ( myValues->GetCount() >= theirValues->GetCount() ) {

            contains = true;
            for ( idx = 0; idx < theirValues->GetCount(); idx++ ) {
                FdoDictionaryElementP theirValue = theirValues->GetItem(idx);
                if ( !myValues->Contains(theirValue->GetName()) ) {
                    contains = false;
                    break;
                }
            }
        }
    }

    return contains;
}


FdoDictionaryP FdoPropertyValueConstraintList::ValuesToDictionary( FdoDataValueCollection* values ) 
{
    FdoDictionaryP valueDictionary = FdoDictionary::Create();
    FdoInt32 idx;

    for ( idx = 0; idx < values->GetCount(); idx++ ) {
        FdoString* value = FdoPtr<FdoDataValue>(values->GetItem(idx))->ToString();

        // Skip duplicate values.
        if ( !valueDictionary->Contains(value) )
            valueDictionary->Add( FdoDictionaryElementP(FdoDictionaryElement::Create(value,L"")) );
    }

    return valueDictionary;
}
