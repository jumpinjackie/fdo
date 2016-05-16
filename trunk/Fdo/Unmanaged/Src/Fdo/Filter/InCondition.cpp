// 
//  
//  Copyright (C) 2004-2011  Autodesk, Inc.
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
#include <Fdo/Filter/InCondition.h>
#include <Fdo/Filter/IFilterProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of an InCondition.
FdoInCondition* FdoInCondition::Create()
{
	return new FdoInCondition();
}

// Constructs an instance of a InCondition using the specified arguments.
FdoInCondition* FdoInCondition::Create(FdoString* propertyName, FdoString** values, FdoInt32 length)
{
	return new FdoInCondition(propertyName, values, length);
}

// Constructs an instance of a InCondition using the specified arguments.
FdoInCondition* FdoInCondition::Create(FdoIdentifier* propertyName, FdoString** values, FdoInt32 length)
{
	return new FdoInCondition(propertyName, values, length);
}

FdoInCondition* FdoInCondition::Create(FdoIdentifier* propertyName, FdoSubSelectExpression* subSel)
{
    return new FdoInCondition(propertyName, subSel);
}

// Constructs an instance of a InCondition using the specified arguments.
FdoInCondition* FdoInCondition::Create(FdoIdentifier* propertyName, FdoValueExpressionCollection* pCollection)
{
    FdoInCondition* pRet = Create();

    pRet->SetPropertyName(propertyName);

    FdoValueExpressionCollection* pColl = NULL;
    FdoValueExpression* pexpr = NULL;
    try
    {
        pColl = pRet->GetValues();
        FdoInt32 i;
        for (i = 0; i < pCollection->GetCount(); i++)
        {
            pexpr = pCollection->GetItem(i);
            pColl->Add(pexpr);
            FDO_SAFE_RELEASE(pexpr);
        }
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pexpr);
        FDO_SAFE_RELEASE(pColl);
        throw pExcept;
    }

	pColl->Release();
    return pRet;
}

// Constructs a default instance of an InCondition.
FdoInCondition::FdoInCondition()
{
	m_propertyName = NULL;
	m_values = FdoValueExpressionCollection::Create();
    m_subSel = NULL;
}

FdoInCondition::FdoInCondition(FdoIdentifier* propertyName, FdoSubSelectExpression* subSel)
{
	m_propertyName = FDO_SAFE_ADDREF(propertyName);
	m_values = FdoValueExpressionCollection::Create();
	m_subSel = FDO_SAFE_ADDREF(subSel);
}

// Constructs an instance of a InCondition using the specified arguments.
FdoInCondition::FdoInCondition(FdoString* propertyName, FdoString** values, FdoInt32 length)
{
	m_propertyName = FdoIdentifier::Create(propertyName);
	m_values = FdoValueExpressionCollection::Create(values, length);
    m_subSel = NULL;
}

// Constructs an instance of a InCondition using the specified arguments.
FdoInCondition::FdoInCondition(FdoIdentifier* propertyName, FdoString** values, FdoInt32 length)
{
	m_propertyName = FDO_SAFE_ADDREF(propertyName);
	m_values = FdoValueExpressionCollection::Create(values, length);
    m_subSel = NULL;
}

FdoInCondition::~FdoInCondition()
{
	FDO_SAFE_RELEASE(m_values);
    FDO_SAFE_RELEASE(m_propertyName);
    FDO_SAFE_RELEASE(m_subSel);
}

void FdoInCondition::Dispose()
{
    delete this;
}

// Gets the name of the data property.
FdoIdentifier* FdoInCondition::GetPropertyName()
{
    return FDO_SAFE_ADDREF(m_propertyName);
}

// Sets the name of the data property.
void FdoInCondition::SetPropertyName(FdoIdentifier* value)
{
    FDO_SAFE_RELEASE(m_propertyName);
	m_propertyName = FDO_SAFE_ADDREF(value);
}

// Gets the ValueCollection that contains the literal values.
FdoValueExpressionCollection* FdoInCondition::GetValues()
{
    return FDO_SAFE_ADDREF(m_values);
}

FdoSubSelectExpression* FdoInCondition::GetSubSelect()
{
    return FDO_SAFE_ADDREF(m_subSel);
}

void FdoInCondition::SetSubSelect(FdoSubSelectExpression* value)
{
    FDO_SAFE_RELEASE(m_subSel);
	m_subSel = FDO_SAFE_ADDREF(value);
    if (m_subSel != NULL)
        m_values->Clear();
}

// Overrides Expression.Process to pass the InCondition to the appropriate
// expression processor operation.
void FdoInCondition::Process(FdoIFilterProcessor* p)
{
    p->ProcessInCondition(*this); 
}

// Converts the filter expression to it's well defined text representation.
FdoString* FdoInCondition::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);

    FdoIdentifier*      pIdent = GetPropertyName();
    FdoValueExpressionCollection*  pColl = GetValues();
    if (pIdent == NULL || (pColl == NULL && m_subSel == NULL))
    {
        FDO_SAFE_RELEASE(pIdent);
        FDO_SAFE_RELEASE(pColl);
        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_4_INCOMPLETEINCONDITION)));
        throw pExcept;
    }
    if (m_subSel != NULL)
    {
        try
        {
            m_toString = FdoStringUtility::MakeString(pIdent->ToStringInternal(pIdCol), L" IN (", m_subSel->ToString(), L")");
        }
        catch (FdoException* pExcept)
        {
            FDO_SAFE_RELEASE(pIdent);
            FDO_SAFE_RELEASE(pColl);
            throw pExcept;
        }
        
        pIdent->Release();
        pColl->Release();
        return m_toString;
    }
    else
    {
        // loop thru the collection once to get the string lenght
        FdoInt32     i;
        size_t  Len = 3;    // 1 for EOS, 2 for '(' and ')'
        bool    bFirst = true;
        FdoString*  stringExpr;
        for (i = 0; i < pColl->GetCount(); i++)
        {
            FdoValueExpression* pExpr;

            pExpr = pColl->GetItem(i);
            if (pExpr == NULL)
                continue;

            stringExpr = pExpr->ToStringInternal( pIdCol );
            if (stringExpr == NULL)
                continue;

            if (bFirst)
                bFirst = false;
            else
                Len += 2;   // 2 = wcslen(", ")
            Len += FdoStringUtility::StringLength(stringExpr);
            FDO_SAFE_RELEASE(pExpr);

        }

        // then allocate that string and loop thru once more to fill it.
        wchar_t*    szValues =  new wchar_t[Len];
        FdoStringUtility::StringCopy(szValues, L"(");
        bFirst = true;
        for (i = 0; i < pColl->GetCount(); i++)
        {
            FdoValueExpression* pExpr;

            pExpr = pColl->GetItem(i);
            if (pExpr == NULL)
                continue;

            stringExpr = pExpr->ToStringInternal( pIdCol );
            if (stringExpr == NULL)
                continue;

            if (bFirst)
                bFirst = false;
            else
                FdoStringUtility::StringConcatenate(szValues, L", ");
            FdoStringUtility::StringConcatenate(szValues, stringExpr);
            FDO_SAFE_RELEASE(pExpr);
        }
        FdoStringUtility::StringConcatenate(szValues, L")");
        
        try
        {
            m_toString = FdoStringUtility::MakeString(pIdent->ToStringInternal(pIdCol), L" IN ", szValues);
        }
        catch (FdoException* pExcept)
        {
            delete [] szValues;
            FDO_SAFE_RELEASE(pIdent);
            FDO_SAFE_RELEASE(pColl);
            throw pExcept;
        }

        delete [] szValues;
        pIdent->Release();
        pColl->Release();
        return m_toString;
    }
}

