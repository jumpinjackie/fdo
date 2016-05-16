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
#include "DbiConnection.h"
#include "FdoRdbmsFilterUtil.h"
#include "FdoRdbmsBaseFilterProcessor.h"
#include "FdoRdbmsSchemaUtil.h"
//
// This helper class is needed by the ConvertFilterToMainClass method
class FixFilterIdentifiers : public FdoRdbmsBaseFilterProcessor
{
private:
    const wchar_t *decoratorStr;
    bool        addDecoratedStr;

public:
    FixFilterIdentifiers(const wchar_t *decorator, bool add)
    {
        decoratorStr = decorator;
        addDecoratedStr = add;
    }

    // We only want to change the identifier name
    // All other method can have empty implementation.
    void ProcessIdentifier(FdoIdentifier& expr)
    {
        const wchar_t *name = expr.GetText();
        wchar_t *newName = NULL;

        if( addDecoratedStr )
        {
            newName = new wchar_t[wcslen(name) + wcslen(decoratorStr) + 2];
            wcscpy(newName, decoratorStr);
            wcscat(newName, L".");
            wcscat(newName, name);
        }
        else if( wcslen(name) > wcslen(decoratorStr) )
        {
            newName = new wchar_t[wcslen(name) + 1];
            wcsncpy(newName, &name[wcslen(decoratorStr)+1], wcslen(name)-wcslen(decoratorStr) );
        }

        if( newName != NULL )
        {
            expr.SetText( newName );
            delete[] newName;
        }
    }
};

FdoIdentifier *FdoRdbmsFilterUtil::ConvertFilterToMainClass( FdoIdentifier *className, FdoFilter  *filter )
{
    FdoIdentifier *newClassName = NULL;
    int  length;
    const FdoString **scopes = className->GetScope( length );
    if( length != 0 )
    {
        newClassName = FdoIdentifier::Create( scopes[0] );
        FdoStringP decorStr = "";
        for(int i=1;i<length;i++)
            decorStr += scopes[i];

        // Add the last obj property
        decorStr += className->GetName();

        FixFilterIdentifiers  fixer( decorStr, true );

        filter->Process( &fixer );
    }
    else // Nothing to do
    {
        newClassName = FdoIdentifier::Create( className->GetText() );
    }

    return newClassName;
}

void FdoRdbmsFilterUtil::ConvertFilterToObjectClass( FdoIdentifier *className, FdoFilter  *filter )
{
    int  length;
    const FdoString **scopes = className->GetScope( length );
    if( length != 0 )
    {
        FdoStringP decorStr = "";
        for(int i=1;i<length;i++)
            decorStr += scopes[i];

        // Add the last obj property
        decorStr += className->GetName();

        FixFilterIdentifiers  fixer( decorStr, false );

        filter->Process( &fixer );
    }
    //else // Nothing to do
}



bool FdoRdbmsFilterUtil::FeatIdsFromFilter( const wchar_t *featIdName, FdoFilter  *filter, int **FeatIdArray, int *count )
{
    *FeatIdArray = NULL;
    *count = 0;

    if( filter == NULL )
        return false;

    class FindIdsHelper : public FdoRdbmsBaseFilterProcessor
    {
    public:
        int   *idArray;
        int   Count;
        const wchar_t *featIdName;

        FindIdsHelper( const wchar_t *idName )
        {
            idArray = NULL;
            Count = 0;
            featIdName = idName;
        }
        virtual void ProcessComparisonCondition(FdoComparisonCondition& filter)
        {
            FdoComparisonCondition *compCnd = &filter;
            if( compCnd->GetOperation() != FdoComparisonOperations_EqualTo )
                return;

            FdoPtr<FdoExpression> lfExp = compCnd->GetLeftExpression();
            FdoPtr<FdoExpression> rtExp = compCnd->GetRightExpression();
            if( dynamic_cast<FdoIdentifier *>( lfExp.p ) == NULL )
                return;
            FdoIdentifier  *ident = static_cast<FdoIdentifier *>( (FdoExpression*)lfExp );
            if( wcscmp(ident->GetName(), featIdName ) != 0 )
                return;

            if( dynamic_cast<FdoDataValue *>( rtExp.p ) == NULL  )
                return;

            FdoDataValue *dVal = static_cast<FdoDataValue *>((FdoExpression*)rtExp);
            int featId = -1;
            switch( dVal->GetDataType() )
            {
                case FdoDataType_Int16:
                    featId = (int) (dynamic_cast<FdoInt16Value*>(dVal))->GetInt16();
                    break;

                case FdoDataType_Int32:
                    featId = (int) (dynamic_cast<FdoInt32Value*>(dVal))->GetInt32();
                    break;

                case FdoDataType_Int64:
                    featId = (int) (dynamic_cast<FdoInt64Value*>(dVal))->GetInt64();
                    break;
            }
            if( featId != -1 )
            {
                idArray = new int[1];
                idArray[0] = featId;
                Count = 1;
            }
        }
        virtual void ProcessInCondition(FdoInCondition& filter)
        {
            FdoInCondition *inCdnt = &filter;
            FdoPtr<FdoIdentifier>id = inCdnt->GetPropertyName();
            if( wcscmp(id->GetName(), featIdName ) != 0 )
                return ;

            FdoPtr<FdoValueExpressionCollection> idValues = inCdnt->GetValues();
            if( idValues == NULL )
                return ;

            idArray = new int[idValues->GetCount()];
            int i;
            for(i=0; i < idValues->GetCount(); i++ )
            {
                FdoPtr<FdoExpression>val = idValues->GetItem(i);
                if( dynamic_cast<FdoDataValue *>( val.p ) == NULL )
                {
                    delete[] idArray;
                    return;
                }
                FdoDataValue *dVal = static_cast<FdoDataValue *>((FdoExpression*)val);
                switch( dVal->GetDataType() )
                {
                    case FdoDataType_Int16:
                        idArray[i] = (int) (dynamic_cast<FdoInt16Value*>(dVal))->GetInt16();
                        break;

                    case FdoDataType_Int32:
                        idArray[i] = (int) (dynamic_cast<FdoInt32Value*>(dVal))->GetInt32();
                        break;

                    case FdoDataType_Int64:
                        idArray[i] = (int) (dynamic_cast<FdoInt64Value*>(dVal))->GetInt64();
                        break;

                    default:
                        idArray[i] = -1;
                }
            }
            Count = i;
        }
    };

    FindIdsHelper  helper(featIdName);
    filter->Process( &helper );
    if( helper.Count != 0 )
    {
        *count = helper.Count;
        *FeatIdArray = helper.idArray;
        return true;
    }
    return false;
}

FdoIdentifierCollection* FdoRdbmsFilterUtil::GetIdentPropList( const FdoSmLpClassDefinition* classDefinition )
{
	bool isFeatId = false;
	FdoStringP colName;
	FdoIdentifierCollection* selProps = NULL;
	FdoSmLpDataPropertiesP pIdentProps = ((FdoSmLpClassBase *)classDefinition)->GetIdentityProperties();
	selProps = FdoIdentifierCollection::Create();
	bool isFeatClass = (classDefinition->GetClassType() == FdoClassType_FeatureClass);
	if (isFeatClass)
    {
        const FdoSmLpDataPropertyDefinition* pFeatIdProp = classDefinition->RefFeatIdProperty();

        if ( pFeatIdProp && pFeatIdProp->RefColumn() )
        {
			colName = pFeatIdProp->GetName();
            FdoPtr<FdoIdentifier> selProp = FdoIdentifier::Create( colName );
            selProps->Add( selProp );
			isFeatId = true;
        }
    }
	for (int i=0; i<pIdentProps->GetCount(); i++)
	{
		if (isFeatId)	// Avoid adding FeatId property again
		{ 
            if (FdoStringP(FdoSmLpDataPropertyP(pIdentProps->GetItem(i))->GetName()).ICompare(colName) == 0)
				continue;
		}
		FdoPtr<FdoIdentifier> selProp = FdoIdentifier::Create( FdoSmLpDataPropertyP(pIdentProps->GetItem(i))->GetName());
		selProps->Add( selProp );
	}

	return (selProps);
}



