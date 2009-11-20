// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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
#include "SltGeomUtils.h"
#include "SltQueryTranslator.h"
#include "SltExprExtensions.h"
#include <FdoExpressionEngineCopyFilter.h>
#include <algorithm>

recno_list* recno_list_union(recno_list* left, recno_list* right)
{
    //if one of the lists is null it means it iterates over all features...
    //so return that list as the union of the two
    if (left == NULL)
        return right;

    if (right == NULL)
        return left;

    std::sort(left->begin(), left->end(), std::less<__int64>());
    std::sort(right->begin(), right->end(), std::less<__int64>());

    recno_list::iterator iter1 = left->begin();
    recno_list::iterator iter2 = right->begin();

    recno_list* ret = new recno_list;

    while (iter1 != left->end() || iter2 != right->end())
    {
        if (iter1 == left->end())
            ret->push_back(*iter2++);
        else if (iter2 == right->end())
            ret->push_back(*iter1++);
        else if (*iter2 < *iter1)
            ret->push_back(*iter2++);
        else if (*iter2 > *iter1)
            ret->push_back(*iter1++);
        else
        {
            ret->push_back(*iter1);
            iter1++;
            iter2++;
        }
    }

    //dispose of inputs and return newly allocated list
    delete left;
    delete right;

    return ret;
}

recno_list* recno_list_intersection(recno_list* left, recno_list* right)
{
    //if one of the lists is null it means it iterates over all features...
    //so return the non-null list as the intersection of the two, if one is non-null
    if (left == NULL)
        return right;

    if (right == NULL)
        return left;

    std::sort(left->begin(), left->end(), std::less<__int64>());
    std::sort(right->begin(), right->end(), std::less<__int64>());

    recno_list::iterator iter1 = left->begin();
    recno_list::iterator iter2 = right->begin();

    recno_list* ret = new recno_list;

    while(iter1 != left->end() || iter2 != right->end())
    {
        if (iter1 == left->end())
            break;
        else if (iter2 == right->end())
            break;
        else if (*iter2 < *iter1)
            iter2++;
        else if (*iter2 > *iter1)
            iter1++;
        else
        {
            ret->push_back(*iter1);
            iter1++;
            iter2++;
        }
    }

    //dispose of inputs and return newly allocated list
    delete left;
    delete right;

    return ret;
}


SltQueryTranslator::SltQueryTranslator(FdoClassDefinition* fc)
: m_refCount(1),
m_optimizedChunk(NULL),
m_restoreChunk(NULL),
m_strgeomOperations(0),
m_geomCount(0)
{
    m_fc = FDO_SAFE_ADDREF(fc);
    m_evalStack.reserve(4);
}

SltQueryTranslator::~SltQueryTranslator()
{
    for (FilterChunkList::iterator idx = m_allocatedObjects.begin(); idx < m_allocatedObjects.end(); idx++)
    {
        delete *idx;
    }
    m_allocatedObjects.clear();

    FDO_SAFE_RELEASE(m_fc);
}

void SltQueryTranslator::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> left = filter.GetLeftOperand();
    FdoPtr<FdoFilter> right = filter.GetRightOperand();

    right->Process(this);
    left->Process(this);
    
    IFilterChunk* lefts = m_evalStack.back();
    m_evalStack.pop_back();

    IFilterChunk* rights = m_evalStack.back();
    m_evalStack.pop_back();

    FdoBinaryLogicalOperations op = filter.GetOperation();

    StlFilterType leftType = lefts->GetType();
    StlFilterType rightType = rights->GetType();
    ComplexFilterChunk* rVal = NULL;

    //determine logical op we want as string
    //and also perform the operation on the bounding
    //boxes associated with the two sides.
    switch (op)
    {
    case FdoBinaryLogicalOperations_And: 
        {
            // we assume the expression cannot be optimized
            if (lefts->m_canOmit || rights->m_canOmit)
            {
                // do we have already a replaceable chunk?
                // this chunk can be replaced by a spatial chunk only in case we do not have lists involved
                if (m_optimizedChunk != NULL && m_optimizedChunk->GetType() == StlFilterType_Spatial &&
                    !(lefts->m_canOmit && leftType == StlFilterType_List) && 
                    !(rights->m_canOmit && rightType == StlFilterType_List))
                {
                    double leftLen = (leftType == StlFilterType_Spatial) ? 0 : lefts->GetBounds()->GetBoundsLength();
                    double rightLen = (rightType == StlFilterType_Spatial) ? 0 : rights->GetBounds()->GetBoundsLength();
                    double actualLen = m_optimizedChunk->GetBounds()->GetBoundsLength();

                    short useExpression = -1;
                    if (lefts->m_canOmit && leftLen < actualLen)
                    {
                        actualLen = leftLen;
                        useExpression = 0;
                    }
                    if (rights->m_canOmit && rightLen < actualLen)
                        useExpression = 1;

                    rVal = CreateComplexFilterChunk(StlFilterType_Spatial);
                    rVal->m_bounds = new DBounds();
                    // do we keep the old chunk!?
                    if (useExpression == -1)
                    {
                        *rVal->m_bounds = *m_optimizedChunk->GetBounds();
                        rVal->AddToList(CreateBaseFilterChunk("(", 1));
                        rVal->AddToList(lefts);
                        rVal->AddToList(CreateBaseFilterChunk(") AND (", 7));
                        rVal->AddToList(rights);
                        rVal->AddToList(CreateBaseFilterChunk(")", 1));
                    }
                    else
                    {
                        Reset();
                        // following chunk will be reactivated in case we find a better solution
                        ComplexFilterChunk* useChunk = CreateComplexFilterChunk();
                        useChunk->AddToList(CreateBaseFilterChunk("(", 1));
                        useChunk->AddToList(lefts);
                        useChunk->AddToList(CreateBaseFilterChunk(") AND (", 7));
                        useChunk->AddToList(rights);
                        useChunk->AddToList(CreateBaseFilterChunk(")", 1));
                        m_optimizedChunk = rVal;
                        m_restoreChunk = useChunk;
                        
                        if (useExpression == 1)
                        {
                            *rVal->m_bounds = *rights->GetBounds();
                            rVal->AddToList(lefts);
                        }
                        else // useExpression == 0
                        {
                            *rVal->m_bounds = *lefts->GetBounds();
                            rVal->AddToList(rights);
                        }
                    }
                }
                else
                {
                    // in case we have a list ID=val has priority
                    if (leftType == StlFilterType_List || rightType == StlFilterType_List)
                    {
                        bool enableReset = m_optimizedChunk == NULL || m_optimizedChunk->GetType() == StlFilterType_Spatial;
                        // handle cases like ID=5 AND ID=7 result will be an empty list
                        rVal = CreateComplexFilterChunk(StlFilterType_List);
                        rVal->m_ids = recno_list_intersection(lefts->DetachIDList(), rights->DetachIDList());
                        
                        // in case we keep a geometry clean it since ID=val has priority
                        if (enableReset && (leftType == StlFilterType_Spatial || rightType == StlFilterType_Spatial))
                            Reset();

                        // in case both chunks can be skipped we need to keep one
                        if (enableReset && (lefts->m_canOmit && leftType == StlFilterType_List || 
                            rights->m_canOmit && rightType == StlFilterType_List))
                        {
                            // following chunk will be reactivated in case we find a better solution
                            ComplexFilterChunk* useChunk = CreateComplexFilterChunk();
                            useChunk->AddToList(CreateBaseFilterChunk("(", 1));
                            useChunk->AddToList(lefts);
                            useChunk->AddToList(CreateBaseFilterChunk(") AND (", 7));
                            useChunk->AddToList(rights);
                            useChunk->AddToList(CreateBaseFilterChunk(")", 1));
                            m_optimizedChunk = rVal;
                            m_restoreChunk = useChunk;
                            
                            if (lefts->m_canOmit && leftType == StlFilterType_List)
                                rVal->AddToList(rights); // we can skip the left side and we add only the right side
                            else
                                rVal->AddToList(lefts); // we can skip the left side and we add only the left side
                        }
                        else
                        {
                            // in this case we cannot replace the skip chunk since is not a list
                            // the list is coming from the other chunk
                            rVal->AddToList(CreateBaseFilterChunk("(", 1));
                            rVal->AddToList(lefts);
                            rVal->AddToList(CreateBaseFilterChunk(") AND (", 7));
                            rVal->AddToList(rights);
                            rVal->AddToList(CreateBaseFilterChunk(")", 1));
                        }
                    }
                    else if (leftType == StlFilterType_Spatial || rightType == StlFilterType_Spatial)
                    {
                        rVal = CreateComplexFilterChunk(StlFilterType_Spatial);
                        rVal->m_bounds = new DBounds();
                        if (lefts->m_canOmit && leftType == StlFilterType_Spatial)
                            *rVal->m_bounds = *lefts->GetBounds();
                        else
                            *rVal->m_bounds = *rights->GetBounds();

                        // in case both chunks can be skipped we need to keep one
                        // following chunk will be reactivated in case we find a better solution
                        ComplexFilterChunk* useChunk = CreateComplexFilterChunk();
                        useChunk->AddToList(CreateBaseFilterChunk("(", 1));
                        useChunk->AddToList(lefts);
                        useChunk->AddToList(CreateBaseFilterChunk(") AND (", 7));
                        useChunk->AddToList(rights);
                        useChunk->AddToList(CreateBaseFilterChunk(")", 1));
                        m_optimizedChunk = rVal;
                        m_restoreChunk = useChunk;
                        
                        if (lefts->m_canOmit && leftType == StlFilterType_Spatial)
                            rVal->AddToList(rights); // we can skip the left side and we add only the right side
                        else
                            rVal->AddToList(lefts); // we can skip the left side and we add only the left side
                    }
                    // else ;not possible since we can omit one 
                }
            }
            else
            {
                if (leftType == StlFilterType_List || rightType == StlFilterType_List)
                {
                    // handle cases like ID=5 AND ID=7 result will be an empty list
                    rVal = CreateComplexFilterChunk(StlFilterType_List);
                    rVal->m_ids = recno_list_intersection(lefts->DetachIDList(), rights->DetachIDList());
                    
                    // in case we keep a geometry clean it since ID=val has priority
                    if (m_optimizedChunk != NULL && m_optimizedChunk->GetType() == StlFilterType_Spatial && 
                        (leftType == StlFilterType_Spatial || rightType == StlFilterType_Spatial))
                        Reset();
                }
                else if (leftType == StlFilterType_Spatial || rightType == StlFilterType_Spatial)
                {
                    rVal = CreateComplexFilterChunk(StlFilterType_Spatial);
                    rVal->m_bounds = new DBounds();
                    // avoid doing intersect since we can have two disjoint bboxes and a feature intersecting both
                    if (leftType == StlFilterType_Spatial && rightType == StlFilterType_Spatial)
                        DBounds::Union(rVal->m_bounds, lefts->GetBounds(), rights->GetBounds());
                    else if (leftType == StlFilterType_Spatial)
                        *rVal->m_bounds = *lefts->GetBounds();
                    else
                        *rVal->m_bounds = *rights->GetBounds();
                }
                else
                    rVal = CreateComplexFilterChunk();
                
                rVal->AddToList(CreateBaseFilterChunk("(", 1));
                rVal->AddToList(lefts);
                rVal->AddToList(CreateBaseFilterChunk(") AND (", 7));
                rVal->AddToList(rights);
                rVal->AddToList(CreateBaseFilterChunk(")", 1));
            }
        }
        break;
    case FdoBinaryLogicalOperations_Or:
        {
            if (leftType == rightType)
            {
                if (leftType == StlFilterType_List)
                {
                    // OR operations can omit only when we have only OR conditions
                    rVal = CreateComplexFilterChunk(StlFilterType_List);
                    rVal->m_ids = recno_list_union(lefts->DetachIDList(), rights->DetachIDList());
                    if (lefts->m_canOmit && rights->m_canOmit)
                        rVal->m_canOmit = true;
                }
                else if (leftType == StlFilterType_Spatial)
                {
                    // OR operations cannot omit, we just use the bounds
                    rVal = CreateComplexFilterChunk(StlFilterType_Spatial);
                    rVal->m_bounds = new DBounds();
                    DBounds::Union(rVal->m_bounds, lefts->GetBounds(), rights->GetBounds());
                }
            }
            if (rVal == NULL)
            {
                Reset();
                rVal = CreateComplexFilterChunk();
            }
            rVal->AddToList(lefts);
            rVal->AddToList(CreateBaseFilterChunk(" OR ", 4));
            rVal->AddToList(rights);
        }
        break;
    }
    // minor memory release
    if (m_optimizedChunk != NULL)
    {
        if (m_optimizedChunk != lefts)
            lefts->ResetType();
        if (m_optimizedChunk != rights)
            rights->ResetType();
    }
    else
    {
        lefts->ResetType();
        rights->ResetType();
    }
    m_evalStack.push_back(rVal);
}

void SltQueryTranslator::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    FdoSpatialOperations op = filter.GetOperation();

    //map the spatial op to the corresponding SQLite extension function name
    const char* sfunc = g_spatial_op_map[op];

    //process the geometry value -- we need it
    //to be either a constant or an identifier
    FdoPtr<FdoExpression> geom = filter.GetGeometry();
    size_t oldsz = m_evalStack.size();
    geom->Process(this);
    size_t nsz = m_evalStack.size();

    //expr was not a constant or identifier -- complain.
    if (nsz == oldsz)
        throw FdoException::Create(L"Expected a constant or identifier in spatial condition.");

    IFilterChunk* expr = m_evalStack.back();
    m_evalStack.pop_back();
    // we don't need to keep this value we delete it at the end
    m_allocatedObjects.pop_back();

    FdoPtr<FdoIdentifier> pname = filter.GetPropertyName();
    //construct the SQLite function we want to use for this spatial op
    m_sb.Reset();
    m_sb.Append(sfunc);
    m_sb.Append("(", 1);
    m_sb.Append(pname->GetName());
    m_sb.Append(",", 1);
    m_sb.Append(expr->ToString());
    m_sb.Append(")", 1);

    FilterChunk* ret = CreateFilterChunk(m_sb.Data(), m_sb.Length(), StlFilterType_Spatial);
    ret->m_bounds = new DBounds();
    //for some operations, we can speed up the query by also
    //performing a bbox check using the spatial index
    switch(op)
    {
    //If the operation is just a bbox operation, we can
    //indicate that the rewriter can completely remove
    //the operation from the where clause (if possible)
    //since it can be directly satisfied by the spatial
    //index query. Also skip all the slow code below
    //by setting the correct bbox and leaving early.
    case FdoSpatialOperations_EnvelopeIntersects:
        //TODO: another assumption here is that the
        //geometry property tested with this operation
        //is *the* geometry property of the feature class
        //and not another geometry property
        ret->m_canOmit = true;
        *ret->m_bounds = *expr->GetBounds();
        break;
    case FdoSpatialOperations_Intersects:
    case FdoSpatialOperations_Contains:
    case FdoSpatialOperations_Within:
    case FdoSpatialOperations_Inside:
    case FdoSpatialOperations_Crosses:
    case FdoSpatialOperations_Overlaps:
        *ret->m_bounds = *expr->GetBounds();
        break;
    case FdoSpatialOperations_Disjoint:
        Reset();
        ret->ResetType();
        break;
    default:
        break;
    }
    delete expr;

    m_evalStack.push_back(ret);
}


void SltQueryTranslator::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> arg = filter.GetOperand();

    arg->Process(this);

    //if we have a negation, make sure the search bounds is reset
    //since negation would, ahem, negate the search region.
    //It is possible to search for a negated region using the
    //spatial index, but the assumption is that this is a rare
    //case, se we won't.
    //also clear any specific IDs we are looking for
    Reset();
    
    IFilterChunk* expr = m_evalStack.back();
    m_evalStack.pop_back();

    // save a ToString call and keep the content
    ComplexFilterChunk* ret = CreateComplexFilterChunk();
    ret->AddToList(CreateFilterChunk("NOT (", 5));
    ret->AddToList(expr);
    ret->AddToList(CreateFilterChunk(")", 1));

    m_evalStack.push_back(ret);
}

void SltQueryTranslator::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
    //check if we are looking for a specific row ID -- we will special case that query
    FdoPtr<FdoExpression> left = filter.GetLeftExpression();
    FdoPtr<FdoExpression> right = filter.GetRightExpression();

    if (filter.GetOperation() == FdoComparisonOperations_EqualTo)
    {
        FilterChunk* ret = NULL;
        FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = m_fc->GetIdentityProperties();
        if (idpdc->GetCount() == 1)
        {
            FdoPtr<FdoDataPropertyDefinition> idpd = idpdc->GetItem(0);
            if (wcscmp(left->ToString(), idpd->GetName()) == 0)
            {
                __int64 idval = -1;
                size_t len = 0;
                int res = swscanf(right->ToString(), L"%lld%n", &idval, &len);

                if (res == 1 && len == wcslen(right->ToString()))
                {
                    ret = CreateFilterChunk(filter.ToString(), StlFilterType_List);
                    ret->m_ids = new recno_list;
                    ret->m_canOmit = true;
                    ret->m_ids->push_back(idval);
                }
            }
        }
        if (ret != NULL)
        {
            m_evalStack.push_back(ret);
            return;
        }
    }
    m_boolConvStack.push_back(StlBoolTopOperationType_IntNeeded);
    // we need to be sure we handle datetime values, e.g. DATEPROP > DATE '10-10-2000'
    left->Process(this);
    IFilterChunk* exprLeft = m_evalStack.back();
    m_evalStack.pop_back();
    
    right->Process(this);
    IFilterChunk* exprRight = m_evalStack.back();
    m_evalStack.pop_back();
    m_boolConvStack.pop_back();

    m_sb.Reset();
    m_sb.Append(exprLeft->ToString());
    switch(filter.GetOperation())
    {
    case FdoComparisonOperations_EqualTo:
        m_sb.Append("=", 1);
        break;
    case FdoComparisonOperations_NotEqualTo:
        m_sb.Append("<>", 2);
        break;
    case FdoComparisonOperations_GreaterThan:
        m_sb.Append(">", 1);
        break;
    case FdoComparisonOperations_GreaterThanOrEqualTo:
        m_sb.Append(">=", 2);
        break;
    case FdoComparisonOperations_LessThan:
        m_sb.Append("<", 1);
        break;
    case FdoComparisonOperations_LessThanOrEqualTo:
        m_sb.Append("<=", 2);
        break;
    case FdoComparisonOperations_Like:
        m_sb.Append(" LIKE ", 6);
        break;
    }
    m_sb.Append(exprRight->ToString());
    m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
}

void SltQueryTranslator::ProcessInCondition(FdoInCondition& filter)
{
    size_t szBefore = m_evalStack.size();
    FdoPtr<FdoValueExpressionCollection> vals = filter.GetValues();
    int cnt = vals->GetCount();
    m_boolConvStack.push_back(StlBoolTopOperationType_IntNeeded);
    for(int idx = 0; idx < cnt; idx++)
    {
        FdoPtr<FdoValueExpression> exp = vals->GetItem(idx);
        exp->Process(this);
    }
    m_boolConvStack.pop_back();
    size_t szAfter = m_evalStack.size();

    m_sb.Reset();
    FdoPtr<FdoIdentifier> prop = filter.GetPropertyName();
    m_sb.AppendDQuoted(prop->GetName());
    m_sb.Append(" IN(", 4);
    
    if (szAfter > szBefore)
    {
        for(size_t idx = szBefore; idx < szAfter; idx++)
        {
            IFilterChunk* item = m_evalStack[idx];
            m_sb.Append(item->ToString());
            m_sb.Append(", ", 2);
        }
        m_sb.Data()[m_sb.Length()-2] = ')';
    }
    else
    {
        m_sb.Append(")", 1);
    }

    // clean stack
    m_evalStack.erase(m_evalStack.begin() + szBefore, m_evalStack.end());
    m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
}

void SltQueryTranslator::ProcessNullCondition(FdoNullCondition& filter)
{
    FdoPtr<FdoIdentifier> idf = filter.GetPropertyName();
    m_sb.Reset();
    m_sb.AppendDQuoted(idf->GetName());
    m_sb.Append(" IS NULL", 8);
    m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
}

void SltQueryTranslator::ProcessDistanceCondition(FdoDistanceCondition& filter)
{
    m_evalStack.push_back(CreateBaseFilterChunk(filter.ToString()));
}

//--------------------------------------------------------------------
// FdoIExpressionProcessor
//--------------------------------------------------------------------

void SltQueryTranslator::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
    FdoPtr<FdoExpression> param = expr.GetLeftExpression();
    param->Process(this);
    IFilterChunk* exprLeft = m_evalStack.back();
    m_evalStack.pop_back();
    
    param = expr.GetRightExpression();
    param->Process(this);
    IFilterChunk* exprRight = m_evalStack.back();
    m_evalStack.pop_back();

    m_sb.Reset();
    switch(expr.GetOperation())
    {
    case FdoBinaryOperations_Add:
        m_sb.Append(exprLeft->ToString());
        m_sb.Append("+", 1);
        m_sb.Append(exprRight->ToString());
        break;
    case FdoBinaryOperations_Subtract:
        m_sb.Append(exprLeft->ToString());
        m_sb.Append("-", 1);
        m_sb.Append(exprRight->ToString());
        break;
    case FdoBinaryOperations_Multiply:
        m_sb.Append("(", 1);
        m_sb.Append(exprLeft->ToString());
        m_sb.Append(")*(", 3);
        m_sb.Append(exprRight->ToString());
        m_sb.Append(")", 1);
        break;
    case FdoBinaryOperations_Divide:
        m_sb.Append("(", 1);
        m_sb.Append(exprLeft->ToString());
        m_sb.Append(")/(", 3);
        m_sb.Append(exprRight->ToString());
        m_sb.Append(")", 1);
        break;
    }
    m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
}

void SltQueryTranslator::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    FdoPtr<FdoExpression> arg = expr.GetExpression();
    arg->Process(this);

    IFilterChunk* exprRez = m_evalStack.back();
    m_evalStack.pop_back();

    m_sb.Reset();
    m_sb.Append(" (-(", 4);
    m_sb.Append(exprRez->ToString());
    m_sb.Append("))", 2);
    m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
}

void SltQueryTranslator::ProcessFunction(FdoFunction& expr)
{
    // We need to handle this because of ToDate...
    // "ToDate(DATE '10-10-2000')" which should be "ToDate('10-10-2000')"
    // otherwise query will fail, because of syntax error
    m_boolConvStack.push_back(StlBoolTopOperationType_StrNeeded);
    size_t szBefore = m_evalStack.size();
    FdoPtr<FdoExpressionCollection> vals = expr.GetArguments();
    int cnt = vals->GetCount();
    for(int idx = 0; idx < cnt; idx++)
    {
        FdoPtr<FdoExpression> exp = vals->GetItem(idx);
        exp->Process(this);
    }
    size_t szAfter = m_evalStack.size();
    if (szAfter != szBefore)
    {
        m_sb.Reset();
        m_sb.Append(expr.GetName());
        m_sb.Append("(", 1);
        for(size_t idx = szBefore; idx < szAfter; idx++)
        {
            m_sb.Append(m_evalStack[idx]->ToString());
            m_sb.Append(", ", 2);
        }
        m_sb.Data()[m_sb.Length()-2] = ')';
        // clean stack
        m_evalStack.erase(m_evalStack.begin() + szBefore, m_evalStack.end());
        m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
    }
    else
    {
        m_sb.Reset();
        m_sb.Append(expr.GetName());
        m_sb.Append("()", 2);
        m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
    }
    m_boolConvStack.pop_back();
}

void SltQueryTranslator::ProcessIdentifier(FdoIdentifier& expr)
{
    //If the filter contains an expression that uses column
    //values, we cannot use fast SQLite stepping since the
    //property value will be fetched by the SQLite execution engine
    //and needs to be properly null terminated, and memcopied.
    //This is only needed for strings and blobs, numeric types
    //are stored in non-volatile memory, so we can use fast
    //stepping for them.

    FdoPtr<FdoPropertyDefinitionCollection> pdc = m_fc->GetProperties();

    FdoPtr<FdoPropertyDefinition> pd = pdc->FindItem(expr.GetName());

    if (pd.p)
    {
        FdoPropertyType type = pd->GetPropertyType();

        if (type == FdoPropertyType_GeometricProperty)
            m_strgeomOperations++;
        else if (type == FdoPropertyType_DataProperty)
        {
            FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)(pd.p);

            FdoDataType dt = dpd->GetDataType();

            if (dt == FdoDataType_String || dt == FdoDataType_BLOB)
            {
                m_strgeomOperations++;
            }
            else
            {
                // do we need the string from Boolean !?
                if (dt == FdoDataType_Boolean && m_boolConvStack.size() != 0 && 
                    m_boolConvStack.back() == StlBoolTopOperationType_StrNeeded)
                {
                    m_sb.Reset();
                    m_sb.Append("booleantostring(", 16);
                    m_sb.AppendDQuoted(expr.GetName());
                    m_sb.Append(")", 1);
                    m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
                    return;
                }
            }
        }
    }

    m_sb.Reset();
    m_sb.AppendDQuoted(expr.GetName());
    m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
}

void SltQueryTranslator::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    throw FdoException::Create(L"Unsupported FDO type in filters");
}

void SltQueryTranslator::ProcessParameter(FdoParameter& expr)
{
    m_sb.Reset();
    m_sb.Append(" :", 2);
    m_sb.Append(expr.GetName());
    m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
}

void SltQueryTranslator::ProcessBooleanValue(FdoBooleanValue& expr)
{
    if (!expr.IsNull())
    {
        if (m_boolConvStack.size() != 0 && m_boolConvStack.back() == StlBoolTopOperationType_StrNeeded)
        {
            if (expr.GetBoolean())
                m_evalStack.push_back(CreateBaseFilterChunk("'TRUE'", 6));
            else
                m_evalStack.push_back(CreateBaseFilterChunk("'FALSE'", 7));
        }
        else
        {
            if (expr.GetBoolean())
                m_evalStack.push_back(CreateBaseFilterChunk("1", 1));
            else
                m_evalStack.push_back(CreateBaseFilterChunk("0", 1));
        }
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessByteValue(FdoByteValue& expr)
{
    if (!expr.IsNull())
    {
        m_sb.Reset();

        _snprintf(m_sb.Data(), 256, "%d", (int)expr.GetByte());

        m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), strlen(m_sb.Data())));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    // the big issues here is FDO syntax add in front of the date
    // DATE/TIME/TIMESTAMP which for SQLite should be removed
    if (!expr.IsNull())
    {
        m_sb.Reset();
        char* dateStr = m_sb.Data();
        *dateStr = '\'';
        FdoDateTime dt = expr.GetDateTime();
        DateToString(&dt, dateStr+1, 31);
        size_t sz = strlen(dateStr+1);
        dateStr[sz+1] = '\'';
        m_evalStack.push_back(CreateBaseFilterChunk(dateStr, sz+2));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessDecimalValue(FdoDecimalValue& expr)
{
    if (!expr.IsNull())
    {
        m_sb.Reset();
        char* ptr = m_sb.Data();
        _snprintf(ptr, 256, "%g", expr.GetDecimal());
        EnsureNoIsLocalIndep(ptr);
        m_evalStack.push_back(CreateBaseFilterChunk(ptr, strlen(ptr)));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessDoubleValue(FdoDoubleValue& expr)
{
    if (!expr.IsNull())
    {
        m_sb.Reset();
        char* ptr = m_sb.Data();
        _snprintf(ptr, 256, "%g", expr.GetDouble());
        EnsureNoIsLocalIndep(ptr);
        m_evalStack.push_back(CreateBaseFilterChunk(ptr, strlen(ptr)));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessSingleValue(FdoSingleValue& expr)
{
    if (!expr.IsNull())
    {
        m_sb.Reset();
        char* ptr = m_sb.Data();
        _snprintf(ptr, 256, "%f", expr.GetSingle());

        EnsureNoIsLocalIndep(ptr);
        m_evalStack.push_back(CreateBaseFilterChunk(ptr, strlen(ptr)));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessInt16Value(FdoInt16Value& expr)
{
    if (!expr.IsNull())
    {
        m_sb.Reset();

        _snprintf(m_sb.Data(), 256, "%d", (int)expr.GetInt16());

        m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), strlen(m_sb.Data())));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessInt32Value(FdoInt32Value& expr)
{
    if (!expr.IsNull())
    {
        m_sb.Reset();
        _snprintf(m_sb.Data(), 256, "%d", expr.GetInt32());

        m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), strlen(m_sb.Data())));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessInt64Value(FdoInt64Value& expr)
{
    if (!expr.IsNull())
    {
        m_sb.Reset();
#ifdef _WIN32
        _i64toa_s(expr.GetInt64(), m_sb.Data(), 256, 10);
#else
        _snprintf(m_sb.Data(), 256, "%lld", (long long int)expr.GetInt64());
#endif
        m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), strlen(m_sb.Data())));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessStringValue(FdoStringValue& expr)
{
    // it's faster than expr.ToString()
    if (!expr.IsNull())
    {
        m_sb.Reset();
        m_sb.AppendSQuoted(expr.GetString());
        m_evalStack.push_back(CreateBaseFilterChunk(m_sb.Data(), m_sb.Length()));
    }
    else
        m_evalStack.push_back(CreateBaseFilterChunk("null", 4));
}

void SltQueryTranslator::ProcessBLOBValue(FdoBLOBValue& expr)
{
    throw FdoException::Create(L"Unsupported FDO type in filters");
}

void SltQueryTranslator::ProcessCLOBValue(FdoCLOBValue& expr)
{
    throw FdoException::Create(L"Unsupported FDO type in filters");
}

void SltQueryTranslator::ProcessGeometryValue(FdoGeometryValue& expr)
{
    //get BBOX of the geometry -- we will push this on the
    //result stack since some spatial ops can be optimized
    //by adding a BBOX query -- see ProcessSpatialCondition
    DBounds ext;
    FdoPtr<FdoByteArray> fgf = expr.GetGeometry();
    GetFgfExtents(fgf->GetData(), fgf->GetCount(), (double*)&ext);

    //flatten the geometry, if it has curves -- this will
    //speed up spatial operations evaluation
    int geom_type = *(int*)(fgf->GetData());

    if (geom_type == FdoGeometryType_CurveString
        || geom_type == FdoGeometryType_CurvePolygon
        || geom_type == FdoGeometryType_MultiCurveString
        || geom_type == FdoGeometryType_MultiCurvePolygon)
    {
        //tesselate if needed
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
        FdoPtr<FdoIGeometry> flatgeom = FdoSpatialUtility::TesselateCurve(fgfgeom);

        if (flatgeom.p != fgfgeom.p)
        {
            fgf = gf->GetFgf(flatgeom);
#ifndef USE_FULL_GEOM_DEF
            expr.SetGeometry(fgf);
#endif
        }
    }

#ifdef USE_FULL_GEOM_DEF
    //convert the FGF array to binhex encoding that we can
    //use in the SQL query
    static const wchar_t hexdigits[] = 
    {
      L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7',
      L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F' 
    };

    int len = fgf->GetCount();
    const unsigned char* blob = fgf->GetData();
    wchar_t* wstr = new wchar_t[2*len+4];

    for(int i=0; i<len; i++)
    {
        wstr[(i*2)+2] = hexdigits[(blob[i]>>4)&0x0F];
        wstr[(i*2)+3] = hexdigits[(blob[i])&0x0F];
    }

    wstr[(len*2)+2] = L'\'';
    wstr[(len*2)+3] = L'\0';
    wstr[0] = L'X';
    wstr[1] = L'\'';

    FilterChunk* ret = CreateFilterChunk(wstr, StlFilterType_Spatial);
    delete[] wstr;
#else
    m_geomCount++;
    m_sb.Reset();
#ifdef _WIN32
    _i64toa_s((FdoInt64)fgf.p, m_sb.Data(), 256, 10);
#else
    _snprintf(m_sb.Data(), 256, "%lld", (long long int)fgf.p);
#endif

    FilterChunk* ret = CreateFilterChunk(m_sb.Data(), strlen(m_sb.Data()), StlFilterType_Spatial);
#endif

    ret->m_bounds = new DBounds();
    *ret->m_bounds = ext;
    m_evalStack.push_back(ret);
}


void SltQueryTranslator::GetBBOX(DBounds& ext)
{
    IFilterChunk* main = (m_evalStack.size() == 0) ? NULL : m_evalStack[0];
    if (main != NULL)
    {
        if (m_optimizedChunk != NULL && m_optimizedChunk->GetType() == StlFilterType_Spatial)
            ext = *m_optimizedChunk->GetBounds();
        else if (main->GetType() == StlFilterType_Spatial)
            ext = *main->GetBounds();
        else
            ext.SetEmpty();
    }
    else
        ext.SetEmpty();
}

//caller is responsible for deleting the return value once this 
//function is called
std::vector<__int64>* SltQueryTranslator::DetachIDList()
{
    if (m_evalStack.size() > 0)
        return m_evalStack[0]->DetachIDList();
    else
        return NULL;
}

const char* SltQueryTranslator::GetFilter()
{
    //is it just a BBOX query -- then it will be fully handled by the spatial index
    IFilterChunk* main = (m_evalStack.size() == 0) ? NULL : m_evalStack[0];
    if (main == NULL || main->m_canOmit || (main->GetType() == StlFilterType_List && main->GetList()->size() == 0))
        return "";

    return m_evalStack[0]->ToString();
}

bool SltQueryTranslator::MustKeepFilterAlive()
{
    return (m_geomCount - (short)(m_optimizedChunk != NULL && m_optimizedChunk->GetType() == StlFilterType_Spatial)) > 0;
}

bool SltQueryTranslator::CanUseFastStepping()
{
    //TODO: the processor needs to be enhanced
    //to detect the cases where we cannot use
    //fast stepping -- when string or blob
    //identifiers are used in expressions. Until
    //this is done, we will just disable fast
    //stepping for any filter that is not just a BBOX filter.

    //return (m_evalStack.size() == 0 || m_evalStack[0]->m_canOmit);
    // when we do not use a geom or a string in a cond
    return (m_strgeomOperations - (short)(m_optimizedChunk != NULL && m_optimizedChunk->GetType() == StlFilterType_Spatial)) > 0;
}

void SltQueryTranslator::Reset()
{
    if (m_restoreChunk != NULL && m_optimizedChunk != NULL)
    {
        m_optimizedChunk->ReplaceContent(m_restoreChunk);
        m_restoreChunk = NULL;
        m_optimizedChunk = NULL;
    }
}

IFilterChunk* SltQueryTranslator::CreateBaseFilterChunk(const char* str, size_t len)
{
    IFilterChunk* ptr = new IFilterChunk(str, len);
    m_allocatedObjects.push_back(ptr);
    return ptr;
}

IFilterChunk* SltQueryTranslator::CreateBaseFilterChunk(const wchar_t* str)
{
    IFilterChunk* ptr = new IFilterChunk(str);
    m_allocatedObjects.push_back(ptr);
    return ptr;
}

FilterChunk* SltQueryTranslator::CreateFilterChunk(const char* str, size_t len, StlFilterType type)
{
    FilterChunk* ptr = new FilterChunk(str, len, type);
    m_allocatedObjects.push_back(ptr);
    return ptr;
}

FilterChunk* SltQueryTranslator::CreateFilterChunk(const wchar_t* str, StlFilterType type)
{
    FilterChunk* ptr = new FilterChunk(str, type);
    m_allocatedObjects.push_back(ptr);
    return ptr;
}

ComplexFilterChunk* SltQueryTranslator::CreateComplexFilterChunk(StlFilterType type)
{
    ComplexFilterChunk* ptr = new ComplexFilterChunk(type);
    m_allocatedObjects.push_back(ptr);
    return ptr;
}

void SltExpressionTranslator::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
    FdoPtr<FdoExpression> param = expr.GetLeftExpression();
    m_expr.Append("(", 1);
    HandleExpr(param);
    switch(expr.GetOperation())
    {
    case FdoBinaryOperations_Add:
        m_expr.Append("+", 1);
        break;
    case FdoBinaryOperations_Subtract:
        m_expr.Append("-", 1);
        break;
    case FdoBinaryOperations_Multiply:
        m_expr.Append("*", 1);
        break;
    case FdoBinaryOperations_Divide:
        m_expr.Append("/", 1);
        break;
    }
    param = expr.GetRightExpression();
    HandleExpr(param);
    m_expr.Append(")", 1);
}

void SltExpressionTranslator::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    m_expr.Append(" (-(", 4);
    FdoPtr<FdoExpression> param = expr.GetExpression();
    HandleExpr(param);
    m_expr.Append("))", 2);
}

void SltExpressionTranslator::ProcessFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> argColl = expr.GetArguments();
    FdoString* name = expr.GetName();
    if (argColl->GetCount() == 2)
    {
        // avoid comparing for each function kind
        // for now we are interested only for functions with two parametres
        if(_wcsicmp(name, FDO_FUNCTION_COUNT) == 0 || _wcsicmp(name, FDO_FUNCTION_AVG) == 0  ||
            _wcsicmp(name, FDO_FUNCTION_MAX) == 0 || _wcsicmp(name, FDO_FUNCTION_MIN) == 0 ||
            _wcsicmp(name, FDO_FUNCTION_SUM) == 0 || _wcsicmp(name, FDO_FUNCTION_STDDEV) == 0)
        {
            FdoPtr<FdoExpression> leftParam = argColl->GetItem(0);
            FdoPtr<FdoExpression> rightParam = argColl->GetItem(1);
            m_expr.Append(name);
            if (_wcsicmp(leftParam->ToString(), L"\'distinct\'") == 0)
                m_expr.Append("( DISTINCT ", 11);
            else
                m_expr.Append("(", 1);
            HandleExpr(rightParam);
            m_expr.Append(")", 1);
            return;
        }
    }
    // in case Boolean values/properties can be used in functions as parameters
    // other than concat we need to check the function name
    // since concat needs Boolean as text pass StlBoolTopOperationType_StrNeeded
    m_boolConvStack.push_back(StlBoolTopOperationType_StrNeeded);

    m_expr.Append(name);
    m_expr.Append("(", 1);
    int cnt = argColl->GetCount();
    if (cnt > 0)
    {
        FdoPtr<FdoExpression> param;
        for(int i = 0; i < cnt-1; i++)
        {
            param = argColl->GetItem(i);
            HandleExpr(param);
            m_expr.Append(",", 1);
        }
        param = argColl->GetItem(cnt-1);
        HandleExpr(param);
    }
    m_expr.Append(")", 1);

    m_boolConvStack.pop_back();
}

void SltExpressionTranslator::ProcessIdentifier(FdoIdentifier& expr)
{
    // most of the time m_boolConvStack.size() == 0 and this should not add any performance impact
    if (m_boolConvStack.size() != 0 && m_boolConvStack.back() == StlBoolTopOperationType_StrNeeded
        && m_fc != NULL)
    {
        FdoPtr<FdoPropertyDefinitionCollection> pdc = m_fc->GetProperties();
        FdoPtr<FdoPropertyDefinition> pd = pdc->FindItem(expr.GetName());
        if (pd.p != NULL)
        {
            if (pd->GetPropertyType() == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dpd = static_cast<FdoDataPropertyDefinition*>(pd.p);
                if (dpd->GetDataType() == FdoDataType_Boolean)
                {
                    m_expr.Append("booleantostring(", 16);
                    m_expr.AppendDQuoted(expr.GetName());
                    m_expr.Append(")", 1);
                    return;
                }
            }
        }
    }
    m_expr.AppendDQuoted(expr.GetName());
}

void SltExpressionTranslator::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    // handle cases like "TRUE as MyBool"
    m_boolConvStack.push_back(StlBoolTopOperationType_IntNeeded);
    FdoPtr<FdoExpression> param = expr.GetExpression();
    if (m_props != NULL)
    {
        // expand the expressions in case we have expressions besad on other expressions.
        FdoPtr<FdoExpression> expandedExpression = FdoExpressionEngineCopyFilter::Copy(param, m_props);
        HandleExpr(expandedExpression);
    }
    else
        HandleExpr(param);
    m_expr.Append(" AS ", 4);
    m_expr.AppendDQuoted(expr.GetName());
    m_boolConvStack.pop_back();
}

void SltExpressionTranslator::ProcessParameter(FdoParameter& expr)
{
    // we do support parameters
    m_expr.Append(" :", 2);
    m_expr.Append(expr.GetName());
}
void SltExpressionTranslator::ProcessBooleanValue(FdoBooleanValue& expr)
{
    if (!expr.IsNull())
    {
        if (m_boolConvStack.size() != 0 && m_boolConvStack.back() == StlBoolTopOperationType_StrNeeded)
        {
            if (expr.GetBoolean())
                m_expr.Append("'TRUE'", 6);
            else
                m_expr.Append("'FALSE'", 7);
        }
        else
        {
            if (expr.GetBoolean())
                m_expr.Append("1", 1);
            else
                m_expr.Append("0", 1);
        }
    }
    else
        m_expr.Append("null", 4);
}
void SltExpressionTranslator::ProcessByteValue(FdoByteValue& expr)
{
    if (!expr.IsNull())
    {
        _snprintf(m_useConv, 256, "%d", (int)expr.GetByte());
        m_expr.Append(m_useConv, strlen(m_useConv));
    }
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    if (!expr.IsNull())
    {
        FdoDateTime dt = expr.GetDateTime();
        DateToString(&dt, m_useConv, 31);
        m_expr.AppendSQuoted(m_useConv);
    }
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessDecimalValue(FdoDecimalValue& expr)
{
    if (!expr.IsNull())
    {
        _snprintf(m_useConv, 256, "%g", expr.GetDecimal());

        EnsureNoIsLocalIndep(m_useConv);
        m_expr.Append(m_useConv, strlen(m_useConv));
    }
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessDoubleValue(FdoDoubleValue& expr)
{
    if (!expr.IsNull())
    {
        _snprintf(m_useConv, 256, "%g", expr.GetDouble());

        EnsureNoIsLocalIndep(m_useConv);
        m_expr.Append(m_useConv, strlen(m_useConv));
    }
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessInt16Value(FdoInt16Value& expr)
{
    if (!expr.IsNull())
    {

        _snprintf(m_useConv, 256, "%d", (int)expr.GetInt16());
        m_expr.Append(m_useConv, strlen(m_useConv));
    }
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessInt32Value(FdoInt32Value& expr)
{
    if (!expr.IsNull())
    {

        _snprintf(m_useConv, 256, "%d", (int)expr.GetInt32());
        m_expr.Append(m_useConv, strlen(m_useConv));
    }
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessInt64Value(FdoInt64Value& expr)
{
    if (!expr.IsNull())
    {
#ifdef _WIN32
        _i64toa_s(expr.GetInt64(), m_useConv, 256, 10);
#else
        _snprintf(m_useConv, 256, "%lld", (long long int)expr.GetInt64());
#endif
        m_expr.Append(m_useConv, strlen(m_useConv));
    }
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessSingleValue(FdoSingleValue& expr)
{
    if (!expr.IsNull())
    {
        _snprintf(m_useConv, 256, "%f", expr.GetSingle());

        EnsureNoIsLocalIndep(m_useConv);
        m_expr.Append(m_useConv, strlen(m_useConv));
    }
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessStringValue(FdoStringValue& expr)
{
    if (!expr.IsNull())
        m_expr.AppendSQuoted(expr.GetString());
    else
        m_expr.Append("null", 4);
}

void SltExpressionTranslator::ProcessBLOBValue(FdoBLOBValue& expr)
{
    throw FdoException::Create(L"Unsupported FDO type in expression");
}

void SltExpressionTranslator::ProcessCLOBValue(FdoCLOBValue& expr)
{
    throw FdoException::Create(L"Unsupported FDO type in expression");
}

void SltExpressionTranslator::ProcessGeometryValue(FdoGeometryValue& expr)
{
    throw FdoException::Create(L"Unsupported FDO type in expression");
}

void SltExtractExpressionTranslator::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    // handle cases like "TRUE as MyBool"
    m_boolConvStack.push_back(StlBoolTopOperationType_IntNeeded);
    FdoPtr<FdoExpression> param = expr.GetExpression();
    if (m_props != NULL)
    {
        // expand the expressions in case we have expressions besad on other expressions.
        FdoPtr<FdoExpression> expandedExpression = FdoExpressionEngineCopyFilter::Copy(param, m_props);
        HandleExpr(expandedExpression);
    }
    else
        HandleExpr(param);
    m_boolConvStack.pop_back();
}

void SltScCHelperTranslator::ProcessFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> argColl = expr.GetArguments();
    FdoString* name = expr.GetName();
    if(_wcsicmp(name, FDO_FUNCTION_SPATIALEXTENTS) == 0)
    {
        if (argColl->GetCount() == 1)
        {
            FdoPtr<FdoExpression> arg = argColl->GetItem(0);
            size_t sz = m_stackNames.size();
            HandleExpr(arg);
            if ((sz+1) == m_stackNames.size())
            {
                FdoString* geomName = m_stackNames.back();
                if (m_fc)
                {
                    FdoPtr<FdoGeometricPropertyDefinition> geomProp = m_fc->GetGeometryProperty();
                    if (geomProp && 0==wcscmp(geomName, geomProp->GetName()))
                    {
                        m_extname = m_stackNames.at(sz-1);
                        return;
                    }
                }
            }
        }
    }
    else if(_wcsicmp(name, FDO_FUNCTION_COUNT) == 0)
    {
        m_countname = m_stackNames.back();
        return;
    }
    m_error = true;
}

void SltScCHelperTranslator::ProcessIdentifier(FdoIdentifier& expr)
{
    if (m_stackNames.size() != 0)
        m_stackNames.push_back(expr.GetName());
    else
        m_error = true;
}

void SltScCHelperTranslator::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    m_stackNames.push_back(expr.GetName());
    FdoPtr<FdoExpression> param = expr.GetExpression();
    HandleExpr(param);
}
