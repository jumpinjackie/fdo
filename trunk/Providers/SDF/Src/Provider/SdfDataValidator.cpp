// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SdfDataValidator.h"
#include "SdfConnection.h"

//
// This method computes a validation flag. This flag indicates the various types of validation that
// is required by a given class.
int SdfDataValidator::ValidationFlag( FdoClassDefinition* cls )
{
	int		validationFlag = SdfDataValidationType_None;

    // Check current class's properties:
	FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties();
	for (int i=0; i<properties->GetCount(); i++)
	{
		FdoPtr<FdoPropertyDefinition> prop = properties->GetItem( i );
        validationFlag |= ValidationFlag(prop);
	}

	// Check base class's properties:
	FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProperties = cls->GetBaseProperties();
	for(int i=0; i<baseProperties->GetCount(); i++ )
	{
		FdoPtr<FdoPropertyDefinition>prop = baseProperties->GetItem( i );
        validationFlag |= ValidationFlag(prop);
	}

	return validationFlag;
}


int SdfDataValidator::ValidationFlag( FdoPropertyDefinition* prop )
{
    int validationFlag = SdfDataValidationType_None;

	if( prop->GetPropertyType() == FdoPropertyType_AssociationProperty )
	{
		FdoAssociationPropertyDefinition *aprop = (FdoAssociationPropertyDefinition *) prop;
		if( ! aprop->GetIsReadOnly() )
			validationFlag |= SdfDataValidationType_Association;
	}
    else if ( prop->GetPropertyType() == FdoPropertyType_DataProperty )
    {
        FdoDataPropertyDefinition *dprop = (FdoDataPropertyDefinition*) prop;
        FdoPtr<FdoPropertyValueConstraint> dpropConstraint = dprop->GetValueConstraint();
        if (!dprop->GetNullable() || (dpropConstraint != NULL))
            validationFlag |= SdfDataValidationType_Constraint;
    }

    return validationFlag;
}

//
// This method scans the property value collection and validate its content. Exception will be thrown
// if the data contain invalid values.
void SdfDataValidator::Validate( SdfConnection* connection, FdoClassDefinition* cls, FdoPropertyValueCollection* pvc, int vFlag, bool forUpdate )
{
	// Check the class properties
	FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties();
	for(int i=0; i<properties->GetCount(); i++ )
	{
		FdoPtr<FdoPropertyDefinition>prop = properties->GetItem( i );
		if( (vFlag & SdfDataValidationType_Association ) && prop->GetPropertyType() == FdoPropertyType_AssociationProperty )
			ValidateAssociationProperty(connection, (FdoAssociationPropertyDefinition *) prop.p, pvc, forUpdate );
		else if( (vFlag & SdfDataValidationType_Constraint ) &&	prop->GetPropertyType() == FdoPropertyType_DataProperty )
			ValidateDataProperty( connection, (FdoDataPropertyDefinition *) prop.p, pvc, forUpdate);
	}

	// Check the base class properties
	FdoPtr<FdoReadOnlyPropertyDefinitionCollection> base_properties = cls->GetBaseProperties();
	for(int i=0; i<base_properties->GetCount(); i++ )
	{
		FdoPtr<FdoPropertyDefinition>prop = base_properties->GetItem( i );
		if( ( vFlag & SdfDataValidationType_Association ) && prop->GetPropertyType() == FdoPropertyType_AssociationProperty )
			ValidateAssociationProperty(connection, (FdoAssociationPropertyDefinition *) prop.p, pvc, forUpdate );
		else if( (vFlag & SdfDataValidationType_Constraint ) &&	prop->GetPropertyType() == FdoPropertyType_DataProperty )
			ValidateDataProperty( connection, (FdoDataPropertyDefinition *) prop.p, pvc, forUpdate);
	}
}

//
// This method validate the association property.
void SdfDataValidator::ValidateAssociationProperty( SdfConnection* connection, FdoAssociationPropertyDefinition* apd, FdoPropertyValueCollection* pvc, bool forUpdate )
{
	if( apd->GetIsReadOnly() )
		return;

	// If the reversemultiplicity is 1 and it's a new feature, then the association property must be initialized
	bool mustExist = ( ! forUpdate && wcscmp(apd->GetReverseMultiplicity(), L"1") == 0 );
	bool mustBeOne = ( wcscmp(apd->GetMultiplicity(), L"1") == 0 );

	FdoPtr<FdoClassDefinition>cls = apd->GetAssociatedClass();
	FdoPtr<FdoDataPropertyDefinitionCollection> idents = apd->GetIdentityProperties();
	FdoPtr<FdoDataPropertyDefinitionCollection> rev_idents = apd->GetReverseIdentityProperties();
	if( idents->GetCount() == 0 )
	{
		// Search for property values with names build from the association property name and the
		// associated class identity properties. For example if the associated class identifier is "Id" and the
		// association property name is "AssocProp", then we should search for a property value with
		// name: "AssocProp.Id". If that property value is found and set, then that means an association
		// exists between the new object(we are about to insert) and the object identified by the value
		// of the property value(AssocProp.Id)
		idents = cls->GetIdentityProperties();
	}
	FdoPtr<FdoFilter>associated_filter;
	FdoPtr<FdoFilter>owner_filter; // Only needed if mustBeOne is true; only one instance of this association
	for(int i=0; i<idents->GetCount(); i++ )
	{
		FdoPtr<FdoDataPropertyDefinition>prop = idents->GetItem( i );
		std::wstring wstr = apd->GetName();
		wstr += L".";
		wstr += prop->GetName();
		FdoPtr<FdoPropertyValue> pv = pvc->FindItem( wstr.c_str() );
        if( pv == NULL || FdoPtr<FdoValueExpression>(pv->GetValue()).p == NULL )
		{
			if( rev_idents->GetCount() != 0 )
			{
				wstr = FdoPtr<FdoDataPropertyDefinition>(rev_idents->GetItem( i ))->GetName();
				pv = pvc->FindItem( wstr.c_str() );
			}
			if( ( pv == NULL || FdoPtr<FdoValueExpression>(pv->GetValue()).p == NULL ) && mustExist )
				throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_64_ASSOCIATION_MUST_BE_SET), apd->GetName() ));
		}
		if( pv == NULL || FdoPtr<FdoValueExpression>(pv->GetValue()).p == NULL )
			break;

		FdoComparisonCondition* associated_cond = FdoComparisonCondition::Create(
					FdoPtr<FdoIdentifier>(FdoIdentifier::Create(prop->GetName()) ), 
					FdoComparisonOperations_EqualTo, FdoPtr<FdoValueExpression>( pv->GetValue() ) );
		if( associated_filter.p == NULL )
			associated_filter = associated_cond;
		else
		{
			associated_filter = FdoFilter::Combine( associated_filter, FdoBinaryLogicalOperations_And, associated_cond );
			associated_cond->Release();
		}
		if( mustBeOne )
		{
			FdoComparisonCondition* owner_cond = FdoComparisonCondition::Create(
					FdoPtr<FdoIdentifier>(FdoIdentifier::Create( wstr.c_str() ) ), 
					FdoComparisonOperations_EqualTo, FdoPtr<FdoValueExpression>( pv->GetValue() ) );
			if( owner_filter.p == NULL )
				owner_filter = owner_cond;
			else
			{
				owner_filter = FdoFilter::Combine( owner_filter, FdoBinaryLogicalOperations_And, owner_cond );
				owner_cond->Release();
			}
		}
	}

	// Check if the associated feature exists
	if( associated_filter.p != NULL && mustExist )
	{
		FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select); 

		select->SetFeatureClassName( cls->GetName() );

		select->SetFilter(associated_filter);
    
		FdoPtr<FdoIFeatureReader> rdr = select->Execute();
		if( ! rdr->ReadNext() ) // Associating to a non-existent feature
			throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_65_ASSOCIATED_NOT_FOUND), cls->GetName() ));
	}

	// If the multiplicity/cardinality is 1-1, then no other instance of this association should exist. i.e the
	// query based on the association value should not return any features.
	if( owner_filter.p != NULL )
	{
		FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select); 

		select->SetFeatureClassName( FdoPtr<FdoSchemaElement>(apd->GetParent())->GetName() );

		select->SetFilter(owner_filter);
    
		FdoPtr<FdoIFeatureReader> rdr = select->Execute();
		if( rdr->ReadNext() ) 
			throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_66_ASSOCIATION_DUPLICATE), FdoPtr<FdoSchemaElement>(apd->GetParent())->GetName(), cls->GetName() ));
	}
}

//
// This method validate the data property.
void SdfDataValidator::ValidateDataProperty( SdfConnection* connection, FdoDataPropertyDefinition* dprop, FdoPropertyValueCollection* pvc, bool forUpdate )
{
    FdoPtr<FdoPropertyValue> propValue = pvc->FindItem(dprop->GetName());

    // NOTE: null values are always acceptable regardless of range/list constraints,
    //       but null values are not acceptable if the property is not nullable.

    if (propValue == NULL)
    {
        // validate null value:
        if (!dprop->GetNullable() && !dprop->GetIsAutoGenerated() && !dprop->GetReadOnly() && !forUpdate)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_110_PROPERTY_NOT_NULLABLE), dprop->GetName()));
    }
    else
    {
        FdoPtr<FdoValueExpression> valueExpr = propValue->GetValue();

        //NOTE: dynamic_cast is risky since it sometimes fails at runtime on linux;
        // (static_cast<> is also risky since FDO users could theoretically put any kind of FdoValueExpression here,
        // not just FdoDataValue, even if it is wrong to do so).
        // Ideally the FdoExpression class would have a virtual GetExpressionType() method that indicated the expression type.
        FdoDataValue* dataValue = dynamic_cast<FdoDataValue*>(valueExpr.p);

        if (dataValue == NULL)
        {
            // validate null value:
            if (!dprop->GetNullable() && !dprop->GetIsAutoGenerated() && !dprop->GetReadOnly())
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_110_PROPERTY_NOT_NULLABLE), dprop->GetName()));
        }
        else
        {
            if (dataValue->IsNull())
            {
                // validate null value:
                if (!dprop->GetNullable() && !dprop->GetIsAutoGenerated() && !dprop->GetReadOnly())
                    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_110_PROPERTY_NOT_NULLABLE), dprop->GetName()));
            }
            else
            {
                // Validate value constraint (if any):
                FdoPtr<FdoPropertyValueConstraint> valueConstraint = dprop->GetValueConstraint();
                if (valueConstraint != NULL)
                {
                    switch (valueConstraint->GetConstraintType())
                    {
                        case FdoPropertyValueConstraintType_Range:
                        {
                            FdoPtr<FdoPropertyValueConstraintRange> rangeConstraint = FDO_SAFE_ADDREF(static_cast<FdoPropertyValueConstraintRange*>(valueConstraint.p));
                            FdoPtr<FdoDataValue> maxValue = rangeConstraint->GetMaxValue();
                            if (maxValue != NULL)
                            {
                                int iComparison = FdoCommonMiscUtil::CompareDataValues(dataValue, maxValue);
                                if ((iComparison > 0) || (iComparison==0 && !rangeConstraint->GetMaxInclusive()))
                                    FdoCommonMiscUtil::ThrowPropertyConstraintException(dprop, dataValue);
                            }
                            FdoPtr<FdoDataValue> minValue = rangeConstraint->GetMinValue();
                            if (minValue != NULL)
                            {
                                int iComparison = FdoCommonMiscUtil::CompareDataValues(dataValue, minValue);
                                if ((iComparison < 0) || (iComparison==0 && !rangeConstraint->GetMinInclusive()))
                                    FdoCommonMiscUtil::ThrowPropertyConstraintException(dprop, dataValue);
                            }
                        }
                        break;

                        case FdoPropertyValueConstraintType_List:
                        {
                            FdoPtr<FdoPropertyValueConstraintList> listConstraint = FDO_SAFE_ADDREF(static_cast<FdoPropertyValueConstraintList*>(valueConstraint.p));
                            FdoPtr<FdoDataValueCollection> allowedDataValues = listConstraint->GetConstraintList();
                            bool bFoundMatch = false;
                            for (int i=0; i<allowedDataValues->GetCount() && !bFoundMatch; i++)
                            {
                                FdoPtr<FdoDataValue> allowedDataValue = allowedDataValues->GetItem(i);
                                if (0==FdoCommonMiscUtil::CompareDataValues(dataValue, allowedDataValue))
                                    bFoundMatch = true;
                            }
                            if (!bFoundMatch)
                                FdoCommonMiscUtil::ThrowPropertyConstraintException(dprop, dataValue);
                        }
                        break;
                    }
                }
            }
        }
    }
}
