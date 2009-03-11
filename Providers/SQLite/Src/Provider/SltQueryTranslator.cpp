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

#include <algorithm>


recno_list* recno_list_union(recno_list* left, recno_list* right)
{
    //if one of the lists is null it means it iterates over all features...
    //so return that list as the union of the two
    if (left == NULL)
        return left;

    if (right == NULL)
        return right;

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
            return ret;
        else if (iter2 == right->end())
            return ret;
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
m_canUseFastStepping(true)
{
    m_fc = FDO_SAFE_ADDREF(fc);
}

SltQueryTranslator::~SltQueryTranslator()
{
    //just in case, delete any ID list that is on top of the stack.
    //This should not happen under normal operation.
    if (!m_evalStack.empty())
        delete m_evalStack[0].ids;

    FDO_SAFE_RELEASE(m_fc);
}

void SltQueryTranslator::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> left = filter.GetLeftOperand();
    FdoPtr<FdoFilter> right = filter.GetRightOperand();

    right->Process(this);
    left->Process(this);
    
    TCtx lefts = m_evalStack.back();
    m_evalStack.pop_back();

    TCtx rights = m_evalStack.back();
    m_evalStack.pop_back();

    FdoBinaryLogicalOperations op = filter.GetOperation();

    TCtx ret;
    std::wstring ops;

    //determine logical op we want as string
    //and also perform the operation on the bounding
    //boxes associated with the two sides.
    switch (op)
    {
    case FdoBinaryLogicalOperations_And: 
        {
            ops = L") AND ("; 
            DBounds::Intersection(&ret.bounds, &lefts.bounds, &rights.bounds);
            ret.ids = recno_list_intersection(lefts.ids, rights.ids);
        }
        break;
    case FdoBinaryLogicalOperations_Or:
        {
            ops = L") OR (";
            DBounds::Union(&ret.bounds, &lefts.bounds, &rights.bounds);
            ret.ids = recno_list_union(lefts.ids, rights.ids);
        }
        break;
    }

    //construct the output query string and
    //push onto the eval stack
    if (lefts.canOmit)
        ret.expr = rights.expr;
    else if (rights.canOmit)
        ret.expr = lefts.expr;
    else
        ret.expr = L"(" + lefts.expr + ops + rights.expr + L")";
   
    m_evalStack.push_back(ret);
}

void SltQueryTranslator::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    FdoSpatialOperations op = filter.GetOperation();

    //map the spatial op to the corresponding SQLite extension function name
    const char* sfunc = g_spatial_op_map[op];
    size_t len = strlen(sfunc) + 1;
    wchar_t* wfunc = (wchar_t*)alloca(sizeof(wchar_t)*len);
    mbstowcs(wfunc, sfunc, len);

    TCtx ret;

    //If the operation is just a bbox operation, we can
    //indicate that the rewriter can completely remove
    //the operation from the where clause (if possible)
    //since it can be directly satisfied by the spatial
    //index query. Also skip all the slow code below
    //by setting the correct bbox and leaving early.
    //TODO: this may be bad if there is a negation around the BBOX
    //filter and it needs to be restored, but let's not worry
    //about such case right now.
    if (op == FdoSpatialOperations_EnvelopeIntersects)
    {
        //TODO: another assumption here is that the
        //geometry property tested with this operation
        //is *the* geometry property of the feature class
        //and not another geometry property
        ret.canOmit = true;

        FdoPtr<FdoExpression> geom = filter.GetGeometry();

        //more bold assmuptions never hurt anybody.
        FdoGeometryValue* gv = (FdoGeometryValue*)(geom.p);
        FdoPtr<FdoByteArray> fgf = gv->GetGeometry();
        GetFgfExtents(fgf->GetData(), fgf->GetCount(), (double*)&ret.bounds);

        m_evalStack.push_back(ret);
        return;        
    }

    //process the geometry value -- we need it
    //to be either a constant or an identifier
    FdoPtr<FdoExpression> geom = filter.GetGeometry();
    size_t oldsz = m_evalStack.size();
    geom->Process(this);
    size_t nsz = m_evalStack.size();

    //expr was not a constant or identifier -- complain.
    if (nsz == oldsz)
        throw FdoException::Create(L"Expected a constant or identifier in spatial condition.");

    TCtx expr = m_evalStack.back();
    m_evalStack.pop_back();

    //construct the SQLite function we want to use for this
    //spatial op
    FdoPtr<FdoIdentifier> pname = filter.GetPropertyName();
    ret.expr = std::wstring(wfunc) + L"(" + pname->GetName() + L"," + expr.expr + L")";

    //for some operations, we can speed up the query by also
    //performing a bbox check using the spatial index
    if (   op == FdoSpatialOperations_Intersects 
        || op == FdoSpatialOperations_Contains
        || op == FdoSpatialOperations_Within
		|| op == FdoSpatialOperations_Inside
		|| op == FdoSpatialOperations_Crosses 
        || op == FdoSpatialOperations_EnvelopeIntersects)
    {
        ret.bounds = expr.bounds;
    }

    m_evalStack.push_back(ret);
}


void SltQueryTranslator::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> arg = filter.GetOperand();

    arg->Process(this);

    TCtx cxt = m_evalStack.back();
    m_evalStack.pop_back();

    //if we have a negation, make sure the search bounds is reset
    //since negation would, ahem, negate the search region.
    //It is possible to search for a negated region using the
    //spatial index, but the assumption is that this is a rare
    //case, se we won't.
    cxt.bounds.SetEmpty();

    //also clear any specific IDs we are looking for
    delete cxt.ids;
    cxt.ids = NULL;

    cxt.expr = L"NOT (" + cxt.expr + L")";

    m_evalStack.push_back(cxt);
}

void SltQueryTranslator::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
    TCtx ret;

    //check if we are looking for a specific row ID -- we will special case that query
    FdoPtr<FdoExpression> left = filter.GetLeftExpression();
    FdoPtr<FdoExpression> right = filter.GetRightExpression();

    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = m_fc->GetIdentityProperties();
    FdoPtr<FdoDataPropertyDefinition> idpd = idpdc->GetItem(0);
    
    if (filter.GetOperation() == FdoComparisonOperations_EqualTo && wcscmp(left->ToString(), idpd->GetName()) == 0)
    {
        __int64 idval = -1;
        size_t len = 0;
        int res = swscanf(right->ToString(), L"%lld%n", &idval, &len);

        if (res == 1 && len == wcslen(right->ToString()))
        {
            ret.ids = new recno_list;
            ret.ids->push_back(idval);
            ret.canOmit = true;
        }
    }

    ret.expr = filter.ToString();
    m_evalStack.push_back(ret);
}

void SltQueryTranslator::ProcessInCondition(FdoInCondition& filter)
{
    TCtx ret;
    ret.expr = filter.ToString();
    m_evalStack.push_back(ret);
}

void SltQueryTranslator::ProcessNullCondition(FdoNullCondition& filter)
{
    TCtx ret;
    ret.expr = filter.ToString();
    m_evalStack.push_back(ret);
}

void SltQueryTranslator::ProcessDistanceCondition(FdoDistanceCondition& filter)
{
    TCtx ret;
    ret.expr = filter.ToString();
    m_evalStack.push_back(ret);
}


//--------------------------------------------------------------------
// FdoIExpressionProcessor
//--------------------------------------------------------------------

void SltQueryTranslator::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
}

void SltQueryTranslator::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
}

void SltQueryTranslator::ProcessFunction(FdoFunction& expr)
{
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
            m_canUseFastStepping = false;
        else if (type == FdoPropertyType_DataProperty)
        {
            FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)(pd.p);

            FdoDataType dt = dpd->GetDataType();

            if (dt == FdoDataType_String || dt == FdoDataType_BLOB)
                m_canUseFastStepping = false;
        }
    }

    TCtx ret;
    ret.expr = expr.GetName();
    m_evalStack.push_back(ret);
}

void SltQueryTranslator::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
}

void SltQueryTranslator::ProcessParameter(FdoParameter& expr)
{
}

void SltQueryTranslator::ProcessBooleanValue(FdoBooleanValue& expr)
{
}

void SltQueryTranslator::ProcessByteValue(FdoByteValue& expr)
{
}

void SltQueryTranslator::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
}

void SltQueryTranslator::ProcessDecimalValue(FdoDecimalValue& expr)
{
}

void SltQueryTranslator::ProcessDoubleValue(FdoDoubleValue& expr)
{
}

void SltQueryTranslator::ProcessInt16Value(FdoInt16Value& expr)
{
}

void SltQueryTranslator::ProcessInt32Value(FdoInt32Value& expr)
{
}

void SltQueryTranslator::ProcessInt64Value(FdoInt64Value& expr)
{
}

void SltQueryTranslator::ProcessSingleValue(FdoSingleValue& expr)
{
}

void SltQueryTranslator::ProcessStringValue(FdoStringValue& expr)
{
}

void SltQueryTranslator::ProcessBLOBValue(FdoBLOBValue& expr)
{
}

void SltQueryTranslator::ProcessCLOBValue(FdoCLOBValue& expr)
{
}

void SltQueryTranslator::ProcessGeometryValue(FdoGeometryValue& expr)
{
    TCtx ret;

    //get BBOX of the geometry -- we will push this on the
    //result stack since some spatial ops can be optimized
    //by adding a BBOX query -- see ProcessSpatialCondition
    DBounds ext;
    FdoPtr<FdoByteArray> fgf = expr.GetGeometry();
    GetFgfExtents(fgf->GetData(), fgf->GetCount(), (double*)&ext);
    ret.bounds = ext;

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
        }
    }

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

    ret.expr = wstr;
    delete[] wstr;

    m_evalStack.push_back(ret);
}


void SltQueryTranslator::GetBBOX(DBounds& ext)
{
    if (m_evalStack.size() > 0)
        ext = m_evalStack[0].bounds;
    else
        ext.SetEmpty();
}

//caller is responsible for deleting the return value once this 
//function is called
std::vector<__int64>* SltQueryTranslator::DetachIDList()
{
    if (m_evalStack.size() > 0)
    {
        std::vector<__int64>* ret = m_evalStack[0].ids;
        m_evalStack[0].ids = NULL; //clear out our pointer since caller now owns this
        return ret;
    }
    else
        return NULL;
}

const wchar_t* SltQueryTranslator::GetFilter()
{
    if (m_evalStack.size() == 0)
        return L"";

    //is it just a BBOX query -- then 
    //it will be fully handled by the 
    //spatial index
    if (m_evalStack[0].canOmit)
        return L"";

    return m_evalStack[0].expr.c_str();
}

bool SltQueryTranslator::CanUseFastStepping()
{
    //TODO: the processor needs to be enhanced
    //to detect the cases where we cannot use
    //fast stepping -- when string or blob
    //identifiers are used in expressions. Until
    //this is done, we will just disable fast
    //stepping for any filter that is not just a BBOX filter.
    //return m_canUseFastStepping;

    if (m_evalStack.empty())
        return true;

    return m_evalStack[0].expr.empty() || m_evalStack[0].canOmit;
}
