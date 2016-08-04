// 
//  
//  Copyright (C) 2004-2007  Autodesk, Inc.
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
#include "SdfQueryOptimizer.h"

#include <float.h>
#include <math.h>

#include <algorithm>
#include <functional>

#include <FdoGeometry.h>
#include <FdoSpatial.h>
#include "Int64Value.h"
#include "DoubleValue.h"
#include "StringValue.h"
#include "BooleanValue.h"
#include "DataIO.h"


//callback for R-Tree searches -- cannot be a class member function
bool SearchCallback(REC_NO recno, bool contained, void* context)
{
     ((recno_list*)context)->push_back(recno);
     
     //true means keep searching
     return true;
}

SdfQueryOptimizer::SdfQueryOptimizer(SdfRTree* rtree, KeyDb* keys, FdoClassDefinition* classDef)
{
    m_rtree = rtree;
    m_keys = keys;
    m_class = classDef;

    //can't be null
    m_class->AddRef();

    //fill in key value collection with empty spots
    //we will use it to accumulate the id property values
    //when checking for a query on key value
    m_idprops = DataIO::FindIDProps(m_class);

    m_keyvals = FdoPropertyValueCollection::Create();
}

SdfQueryOptimizer::~SdfQueryOptimizer()
{    
    //make sure we free the optimized filter stack
    //in case the user didn't get a copy
    for (filter_stack::iterator iter = m_filters.begin(); iter != m_filters.end(); iter++)
        (*iter)->Release();

    //can't be null
    m_class->Release();
    m_idprops->Release();
    m_keyvals->Release();
}


void SdfQueryOptimizer::Dispose()
{
    delete this;
}

//returns the result of a query execution
//A filter must be processed with this FilterExecutor in 
//order to have a valid result value
//The caller is responsible for freeing the returned recno_list and the pointer
recno_list* SdfQueryOptimizer::GetResult()
{
    //only one feature ID list should be left on the evaluation stack after
    //procssing a spatial query
    _ASSERT(m_retvals.size() == 1);

    recno_list* ret = m_retvals[0];

    m_retvals.clear();


    if (ret == NULL)
        return ret;

    std::sort(ret->begin(), ret->end(), std::less<REC_NO>());

    return ret;
}

//WARNING: caller responsible for calling Release() on returned FdoFilter
FdoFilter* SdfQueryOptimizer::GetOptimizedFilter()
{
    //only one feature ID list should be left on the evaluation stack after
    //procssing a spatial query
    _ASSERT(m_filters.size() == 1);

    FdoFilter* ret = m_filters[0];

    m_filters.clear();

    return ret;
}


void SdfQueryOptimizer::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
   
    FdoPtr<FdoFilter> left = filter.GetLeftOperand();
    FdoPtr<FdoFilter> right = filter.GetRightOperand();

    //id property value count -- get initial number of detected identity properties
    //before evaluating arguments
    int count0 = m_keyvals->GetCount();

    //evaluate left hand side
    left->Process(this);

    //if the left expression evaluated to an id property == comparison
    //count1 will be bigger than count0. If that's not the case, 
    //we will need to reset the property collection
    int count1 = m_keyvals->GetCount();


    //get Process() results from return value stack
    recno_list* argLeft= m_retvals.back();
    m_retvals.pop_back();

    //get optimized left side Filter from the stack
    FdoFilter* fLeft = m_filters.back();
    m_filters.pop_back();

    //Optimize binary logic by looking at the value of the left operand and see
    //if we have to evaluate the right one also
    if ((argLeft == NULL) && filter.GetOperation() == FdoBinaryLogicalOperations_Or)
    {
        //case where we need to look at all features for sure
        m_retvals.push_back(NULL);
        
        //update optimized query also by pushing
        //the old filter -- it could not be optimized
        m_filters.push_back(FDO_SAFE_ADDREF(&filter));
		if( fLeft != NULL )
			fLeft->Release();
        
        return;
    }
    
	if( filter.GetOperation() == FdoBinaryLogicalOperations_And && argLeft != NULL && argLeft->size() > 0 && fLeft == NULL )
	{
		// The left side reduced the condidate list to a smaller set and optimized away the left filter
		// We only need to apply the right filter to the reduced set.
		m_filters.push_back( right.Detach() );
		m_retvals.push_back(argLeft);
        return;
	}

    //evaluate right hand side
    right->Process(this);
    
    //if the left expression evaluated to an id property == comparison
    //count2 will be bigger than count1. If that's not the case, 
    //we will need to reset the property collection
    int count2 = m_keyvals->GetCount();

    recno_list* argRight = m_retvals.back();
    m_retvals.pop_back();

    //get optimized right side Filter from the stack
    FdoFilter* fRight = m_filters.back();
    m_filters.pop_back();

    switch (filter.GetOperation())
    {
    case FdoBinaryLogicalOperations_And: 
        if (count1 <= count0 || count2 <= count1) 
            m_keyvals->Clear(); //both left and right sides must result in an identity property condition
                                //for us to continue accumulating a key

        //Note : call to recno_list_intersection will dispose of input lists
        //if they are not to be  reused
        m_retvals.push_back(recno_list_intersection(argLeft, argRight)); 
        break;
    case FdoBinaryLogicalOperations_Or : 
        m_keyvals->Clear(); //Accumulating id props using Or operation cannot result in 
                            //unique key value (need to use And for that)

        //Note : call to recno_list_union will dispose of input lists
        //if they are not to be  reused
        m_retvals.push_back(recno_list_union(argLeft, argRight));
        break;
    default: 
        //will never get here
        throw FdoException::Create(L"Invalid logical operation type");break;
    }

    //see if filters were optimized away and push appropriate
    //filter on the result stack
    if ((fLeft != NULL) ^ (fRight != NULL))
    {
        //case where one side was optimized away : other side remains
        //as the resulting filter
        if (fLeft) 
            m_filters.push_back(fLeft);
        else
            m_filters.push_back(fRight);
    }
    else //both are NULL or both are not NULL
    {
        if (fLeft)
        {
            //case where both sides remain
            m_filters.push_back(FdoBinaryLogicalOperator::Create(fLeft, filter.GetOperation(), fRight));
            fLeft->Release();
            fRight->Release();
        }
        else
        {
            //case where both sides are NULL
            m_filters.push_back(NULL);
        }
    }

}

void SdfQueryOptimizer::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter> right = filter.GetOperand();

    //evaluate right hand side
    right->Process(this);

    //get Process() results from return value stack
    recno_list* argRight = m_retvals.back();
    m_retvals.pop_back();

    //get optimized Filter from the stack
    //just push the old filter on the result stack...
    //NOT cannot be simply optimized
    FdoFilter* fRight = m_filters.back();
    m_filters.pop_back();
    fRight->Release();
    m_filters.push_back(FDO_SAFE_ADDREF(&filter));


    //TODO:
    //for now NOT will push ALL features on the stack (i.e. push NULL)
    //We could in theory subtract a given set of feaure IDs (that we are applying NOT to)
    //from the set of all features, but this is too painful, so we look at all features instead
    switch (filter.GetOperation())
    {
    case FdoUnaryLogicalOperations_Not:
            m_retvals.push_back(NULL); break;
    default: 
        //will never get here
        throw FdoException::Create(L"Invalid logical operation type");break;
    }

    delete argRight;

}

void SdfQueryOptimizer::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
    //get optimized Filter from the stack
    //just push the old filter on the result stack...
    m_filters.push_back(FDO_SAFE_ADDREF(&filter));

    //here we check if the filter contains a search on key value, in which case we can
    //use the key index table to get feature record numbers.
    //Key can be comprised of several identity properties, which complicates things.
    //In this function we check if a comparison condition is done on an identity property
    //and push appropriate indicators on the stack, to be handled by ProcessBinaryLogicalOp.
    
    //pushing NULL on return stack means no features are excluded for this 
    //search condition -- however, this will be ignored if the caller determines
    //that in here we matched an identity property and that it is had enough 
    //information to get a particular feature record number out of the 
    //identity property collection
    m_retvals.push_back(NULL);
 
    if (filter.GetOperation() != FdoComparisonOperations_EqualTo)
        return;

    FdoPtr<FdoExpression> left = filter.GetLeftExpression();
    FdoPtr<FdoExpression> right = filter.GetRightExpression();
    
    //This could be optimized by a struct similar to the
    //PropertyIndex used for the full property collection
    FdoIdentifier* ident = dynamic_cast<FdoIdentifier*>(left.p);
    FdoPtr<FdoDataValue> val = FDO_SAFE_ADDREF(dynamic_cast<FdoDataValue*>(right.p));

    if (ident && val && !val->IsNull())
    {
        //look for the identifier in the ID property collection
        //and add it to the list of accumulated key values if it is
        for (int i=0; i<m_idprops->GetCount(); i++)
        {
            FdoPtr<FdoDataPropertyDefinition> dpd = m_idprops->GetItem(i);

            if (wcscmp(dpd->GetName(), ident->GetName()) == 0)
            {
                if ( val->GetDataType() != dpd->GetDataType() )
                {
                    // KeyDb::FindRecno() will always find nothing when value types
                    // do not match property data type. Try converting  value to 
                    // property data type.

                    val = FdoDataValue::Create( dpd->GetDataType(), val, true, false, false );
                }

                if ( val && !val->IsNull() ) 
                {
                    FdoPropertyValue* pv = FdoPropertyValue::Create(ident, val);
                    m_keyvals->Add(pv);
                    pv->Release();

                    //check to see if we have enough to 
                    recno_list* recno = RecnoFromKey();
                    
                    if (recno)
                    {
                        m_retvals.pop_back();
                        m_retvals.push_back(recno);
                    }
                }
                //else: possible data type conversion failure, can't optimize.
            }
        }
    }
}

void SdfQueryOptimizer::ProcessInCondition(FdoInCondition& filter)
{
    //get optimized Filter from the stack
    //just push the old filter on the result stack...
    m_filters.push_back(FDO_SAFE_ADDREF(&filter));

    //here we check if the in condition is on key values, in which case we can
    //use the key index table to get feature record numbers.
    
    //start by pushing NULL on return stack, meaning no features are excluded for this 
    //search condition -- however, if the values in this in condition can be 
    //mapped to record numbers, then this NULL will be replaced by the features.
    m_retvals.push_back(NULL);

    //Key can be comprised of several identity properties, which complicates things.
    //For now, don't try to optimize this case.
    if ( m_idprops->GetCount() != 1 )
        return;

    // Check if in condition is on the key property. If not then can't optimize
    FdoPtr<FdoIdentifier> ident = filter.GetPropertyName();

    FdoPtr<FdoDataPropertyDefinition> dpd = m_idprops->FindItem(ident->GetName());

    if ( !dpd )
        return;


    // Try to convert all condition values to record numbers.

    recno_list* reclist = NULL;

    FdoPtr<FdoValueExpressionCollection> values = filter.GetValues();

    for ( int i = 0; i < values->GetCount(); i++ ) 
    {
        FdoPtr<FdoValueExpression> value = values->GetItem(i);

        FdoPtr<FdoDataValue> dv = FDO_SAFE_ADDREF(dynamic_cast<FdoDataValue*>(value.p));

        if (!dv ) 
        {
            // Value not a data value, can't optimize. 
            delete reclist;
            return;
        }

        if ( dv->GetDataType() != dpd->GetDataType() ) 
        {
            // KeyDb::FindRecno() will always find nothing when value types
            // do not match property data type. Try converting  value to 
            // property data type.

            dv = FdoDataValue::Create( dpd->GetDataType(), dv, true, false, false );

            if ( !dv ) 
            {
                // data type conversion failed so can't optimize.
                delete reclist;
                return;
            }
        }

        // Convert value to record number. Use our own key values list, instead
        // of m_keyvals, in order to not mess up comparison condition handling.
        // Not sure if possible to mess it up, but just in case ...
        FdoPtr<FdoPropertyValueCollection> keyvals = FdoPropertyValueCollection::Create();
        FdoPtr<FdoPropertyValue> keyval = FdoPropertyValue::Create(ident, dv);
        keyvals->Add(keyval);

        recno_list* recno = RecnoFromKey(keyvals);

        if (!recno)
        {
            // This happens when record number retrieval failed, rather then when
            // feature does not exist. Possible reason for failure is an index
            // that needs rebuilding. Cannot optimize.
            delete reclist;
            return;
        }

        reclist = reclist ? recno_list_union(reclist, recno) : recno;
    }

    // Got here so can optimize in condition. Put features on the stack.

    m_retvals.pop_back();
    m_retvals.push_back(reclist);
}

void SdfQueryOptimizer::ProcessNullCondition(FdoNullCondition& filter)
{
    //cannot optimize filter away, push back on filter result stack
    m_filters.push_back(FDO_SAFE_ADDREF(&filter));

    //pushing NULL on return stack means no features are excluded for this 
    //condition
    m_retvals.push_back(NULL);

}

void SdfQueryOptimizer::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    FdoPtr<FdoExpression> expr = filter.GetGeometry();

    //right now we only handle literal Geometry values
    //Any other expression will cause exception.
    FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

    if (geomval == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_46_LITERAL_GEOM_VALUES_ONLY)));

    FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();

    //check if query can be optimized by R-Tree search
    //it must be a query on the geometry property in that case
    FdoPtr<FdoGeometricPropertyDefinition> gpd = PropertyIndex::FindGeomProp(m_class);
    FdoPtr<FdoIdentifier> idname = filter.GetPropertyName();

    if (m_rtree && gpd && wcscmp(gpd->GetName(), idname->GetName()) == 0)
    {
        //is it cartesian aligned rectangle search?
        if (filter.GetOperation() == FdoSpatialOperations_EnvelopeIntersects )
        {
            //if the given geometry is axis-aligned
            //we can perform an R-Tree search
            Bounds bounds;
            if (IsAxisAlignedRectangle(fgf, bounds))
            {
                if( IsAllDataEnclosed( bounds ) )
                {
                    m_retvals.push_back(NULL);
                    m_filters.push_back(NULL);
                }
                else
                {
                    recno_list* rl = new recno_list;

                    //search the R-Tree for records whose bounds overlap
                    //the given IEnvelope
                    m_rtree->Search(bounds, (SearchHitCallback)SearchCallback, rl);

                    //no need to sort the returned list of records
                    //GetResult() will do it

                    //push the matched feature record numbers on the result stack
                    //if (rl->size() > 0)
                    m_retvals.push_back(rl);

                    //optimize away this filter, since the R-Tree handled it
                    m_filters.push_back(NULL);
                }
            }
            else 
            {
                recno_list* rl = NULL;
                if (!(bounds.maxx == bounds.maxy && bounds.minx == bounds.miny && bounds.maxx == bounds.minx && bounds.maxy == 0.0))
                {
                    recno_list* rl = new recno_list;
                    //search the R-Tree for records whose bounds overlap
                    //the given IEnvelope
                    m_rtree->Search(bounds, (SearchHitCallback)SearchCallback, rl);
                }
                //push the matched feature record numbers on the result stack
                m_retvals.push_back(rl);
                //cannot optimize filter away, push back on filter result stack
                m_filters.push_back(FDO_SAFE_ADDREF(&filter));
            }
        }
        //for Intersects and Contains/Within we will narrow down the return set
        //by returning only features whose bounding boxes intersect with the 
        //bounding box of the input. The actual query result is guaranteed
        //to be a subset of the R-Tree search.
        else if ( filter.GetOperation() == FdoSpatialOperations_Intersects 
            || filter.GetOperation() == FdoSpatialOperations_Contains
            || filter.GetOperation() == FdoSpatialOperations_Within
			|| filter.GetOperation() == FdoSpatialOperations_Inside
			|| filter.GetOperation() == FdoSpatialOperations_Crosses )
        {

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);

            Bounds bounds;
            FdoSpatialUtility::GetExtents(fgf, bounds.minx, bounds.miny, bounds.maxx, bounds.maxy);

            if (bounds.maxx >= bounds.minx && bounds.maxy >= bounds.miny )
            {
                //expand a little to catch point features that lie exactly on the bounds
                Bounds bounds2(bounds.minx - SDF_GLOBAL_TOLERANCE, 
                    bounds.miny - SDF_GLOBAL_TOLERANCE, 
                    bounds.maxx + SDF_GLOBAL_TOLERANCE, 
                    bounds.maxy + SDF_GLOBAL_TOLERANCE);

                recno_list* rl = new recno_list;

                //search the R-Tree for records whose bounds overlap
                //the given IEnvelope
                m_rtree->Search(bounds2, (SearchHitCallback)SearchCallback, rl);

                //no need to sort the returned list of records
                //GetResult() will do it

                //push the matched feature record numbers on the result stack
                m_retvals.push_back(rl);
            }

            //cannot optimize filter away even though we have narrowed it down,
            //push back on filter result stack -- but first we can tesselate
            //any curve geometries it may contain.

            //see if we need to tesselate the input geometry.
            //Doing this here makes sure the geometry is tesselated just once
            //instead of tesselating per every feature later.
            switch (fgfgeom->GetDerivedType())
            {
            case FdoGeometryType_CurveString : 
            case FdoGeometryType_CurvePolygon : 
            case FdoGeometryType_MultiCurveString :
            case FdoGeometryType_MultiCurvePolygon : 
                {
                    //tesselate if needed
                    FdoPtr<FdoIGeometry> flatgeom = FdoSpatialUtility::TesselateCurve(fgfgeom);

                    if (flatgeom.p == fgfgeom.p)
                    {
                        //case where tesselation was not needed -- just
                        //reuse the input filter
                        m_filters.push_back(FDO_SAFE_ADDREF(&filter));
                    }
                    else
                    {
                        FdoPtr<FdoByteArray> flatfgf = gf->GetFgf(flatgeom);

                        //now allocate a new spatial condition to replace the input
                        //in the optimized filter
                        FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(flatfgf);
                        gv->SetGeometry(flatfgf);

                        //push tesselated filter on the stack
                        //note anything on the stack has its refcount bumped
                        //so we just use the fact that the constructor gives a 
                        //bumped refcount
                        m_filters.push_back(FdoSpatialCondition::Create(
                            filter.GetPropertyName(), filter.GetOperation(), gv));
                    }
                }
                break;                

            default:
                m_filters.push_back(FDO_SAFE_ADDREF(&filter));
                break;
            }
        }
        else
        {
            //pushing NULL on return stack means no features are spatially excluded for this 
            //search condition
            m_retvals.push_back(NULL);

            //cannot optimize filter away, push back on filter result stack
            m_filters.push_back(FDO_SAFE_ADDREF(&filter));
        }
    }
    //we cannot optimize other operators
    else
    {
        //pushing NULL on return stack means no features are spatially excluded for this 
        //search condition
        m_retvals.push_back(NULL);

        //cannot optimize filter away, push back on filter result stack
        m_filters.push_back(FDO_SAFE_ADDREF(&filter));
    }

}

void SdfQueryOptimizer::ProcessDistanceCondition(FdoDistanceCondition& filter)
{    
    //cannot optimize filter away, push back on filter result stack
    m_filters.push_back(FDO_SAFE_ADDREF(&filter));

    //pushing NULL on return stack means no features are excluded for this 
    //condition
    m_retvals.push_back(NULL);
}



//--------------------------------------------------------------------
// FdoIExpressionProcessor
//--------------------------------------------------------------------


void SdfQueryOptimizer::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
}

void SdfQueryOptimizer::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
}

void SdfQueryOptimizer::ProcessFunction(FdoFunction& expr)
{
}

void SdfQueryOptimizer::ProcessIdentifier(FdoIdentifier& expr)
{
}

void SdfQueryOptimizer::ProcessParameter(FdoParameter& expr)
{
}

void SdfQueryOptimizer::ProcessBooleanValue(FdoBooleanValue& expr)
{
}

void SdfQueryOptimizer::ProcessByteValue(FdoByteValue& expr)
{
}

void SdfQueryOptimizer::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
}

void SdfQueryOptimizer::ProcessDecimalValue(FdoDecimalValue& expr)
{
}

void SdfQueryOptimizer::ProcessDoubleValue(FdoDoubleValue& expr)
{
}

void SdfQueryOptimizer::ProcessInt16Value(FdoInt16Value& expr)
{
}

void SdfQueryOptimizer::ProcessInt32Value(FdoInt32Value& expr)
{
}

void SdfQueryOptimizer::ProcessInt64Value(FdoInt64Value& expr)
{
}

void SdfQueryOptimizer::ProcessSingleValue(FdoSingleValue& expr)
{
}

void SdfQueryOptimizer::ProcessStringValue(FdoStringValue& expr)
{
}

void SdfQueryOptimizer::ProcessBLOBValue(FdoBLOBValue& expr)
{
}

void SdfQueryOptimizer::ProcessCLOBValue(FdoCLOBValue& expr)
{
}

void SdfQueryOptimizer::ProcessGeometryValue(FdoGeometryValue& expr)
{
    _ASSERT(false);
    //will not get here, identity properties are all DataProperties
}



//----------------------------------------------------------------------------------
// Helper functions
//----------------------------------------------------------------------------------

//tries to obtain a feature record number from a given collection 
//of features -- used when checking if a query contains 
//filters on feature key values
recno_list* SdfQueryOptimizer::RecnoFromKey(FdoPropertyValueCollection* keyvals)
{
    if ( !keyvals ) 
        keyvals = m_keyvals;

    //don't have enough property values to generate a key
    if (keyvals->GetCount() < m_idprops->GetCount())
        return NULL;

    try 
    {
        //this will throw stuff if it can't find the key
        //or could not generate one from the keyvals collection
        REC_NO recno = m_keys->FindRecno(m_class, keyvals);

		// If the index is corrupt; then we should fall back on a table scan untill
		// we get a chance to re-gen the index table.
		if( m_keys->IndexNeedsRegen() )
			return NULL;

        //reset the id property value accumulation collection
        keyvals->Clear();
        
        recno_list* ret = new recno_list;
        ret->push_back(recno);
        return ret;
    }
    catch (FdoException* e)
    {
        e->Release();
    }
    
    //return empty list, meaning no feature was found that matches the 
    //given key. This will prevent the feature reader from looping over
    //features (like it would if we returned NULL instead of empty list)
    return new recno_list;
}


//determines whether the given FGF stream is an axis-aligned rectangle
//which can be used for R-Tree searches
bool SdfQueryOptimizer::IsAxisAlignedRectangle(FdoByteArray* fgf, Bounds& bounds)
{
    bounds.minx = bounds.miny = bounds.maxx = bounds.maxy = 0.0;

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);

    //TODO
    //TODO
    //TODO
    //Check if all this is fast with the new faster FDO geometry

    FdoIPolygon* box = NULL;
    if( fgfgeom->GetDerivedType() == FdoGeometryType_Polygon )
        box = (FdoIPolygon*)(fgfgeom.p);

    //it has to be a polygon
    if (box == NULL)
        return false;

    if (box->GetInteriorRingCount() > 0)
        return false;

    FdoPtr<FdoILinearRing> outer = box->GetExteriorRing();

    //technically the ring must always be closed so it will have 5 points
    //but let's be nicer and check if it is a rectangle even if the user 
    //didn't bother to close it.
    if (outer->GetCount() < 4 || outer->GetCount() > 5)
        return false;

    FdoPtr<FdoDirectPositionCollection> pts = outer->GetPositions();

    FdoPtr<FdoIDirectPosition> p0 = pts->GetItem(0);
    FdoPtr<FdoIDirectPosition> p1 = pts->GetItem(1);
    FdoPtr<FdoIDirectPosition> p2 = pts->GetItem(2);
    FdoPtr<FdoIDirectPosition> p3 = pts->GetItem(3);

    bool isRect = false;
    bool firstSegHorizontal = false;

    //determine if it is axis aligned rectangle -- I know code below it's ugly!
    if (   (   AreEqual(p0->GetX(), p1->GetX()) //case of first segment is vertical
            && AreEqual(p1->GetY(), p2->GetY()) 
            && AreEqual(p2->GetX(), p3->GetX())
            && AreEqual(p3->GetY(), p0->GetY())  ))
    {
        isRect = true;
    }

    if (!isRect && AreEqual(p0->GetY(), p1->GetY()) //case of first segment is horizontal
                && AreEqual(p1->GetX(), p2->GetX())
                && AreEqual(p2->GetY(), p3->GetY())
                && AreEqual(p3->GetX(), p0->GetX()) )
    {
        isRect = true;
        firstSegHorizontal = true;
    }

    if (!isRect)
    {
        FdoPtr<FdoIEnvelope> env = fgfgeom->GetEnvelope();
        bounds.minx = env->GetMinX();
        bounds.miny = env->GetMinY();
        bounds.maxx = env->GetMaxX();
        bounds.maxy = env->GetMaxY();
        return false;
    }
    if (firstSegHorizontal)
    {
        bounds.minx = min(p0->GetX(), p1->GetX());
        bounds.miny = min(p1->GetY(), p2->GetY());
        bounds.maxx = max(p0->GetX(), p1->GetX()); 
        bounds.maxy = max(p1->GetY(), p2->GetY());
    }
    else
    {
        bounds.minx = min(p1->GetX(), p2->GetX());
        bounds.miny = min(p2->GetY(), p3->GetY());
        bounds.maxx = max(p1->GetX(), p2->GetX()); 
        bounds.maxy = max(p2->GetY(), p3->GetY());
    }

    //expand a little to catch point features that lie exactly on the bounds
    bounds.minx -= SDF_GLOBAL_TOLERANCE;
    bounds.miny -= SDF_GLOBAL_TOLERANCE;
    bounds.maxx += SDF_GLOBAL_TOLERANCE;
    bounds.maxy += SDF_GLOBAL_TOLERANCE;    
           
    return true;
}

bool SdfQueryOptimizer::IsAllDataEnclosed( Bounds& bounds )
{
    Bounds dataBounds = m_rtree->GetBounds();

    return( bounds.minx <= dataBounds.minx &&
            bounds.miny <= dataBounds.miny &&
            bounds.maxx >= dataBounds.maxx &&
            bounds.maxy >= dataBounds.maxy );
}


recno_list* SdfQueryOptimizer::recno_list_union(recno_list* left, recno_list* right)
{
    //if one of the lists is null it means it iterates over all features...
    //so return that list as the union of the two
    if (left == NULL)
        return left;

    if (right == NULL)
        return right;

    std::sort(left->begin(), left->end(), std::less<REC_NO>());
    std::sort(right->begin(), right->end(), std::less<REC_NO>());

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

recno_list* SdfQueryOptimizer::recno_list_intersection(recno_list* left, recno_list* right)
{
    //if one of the lists is null it means it iterates over all features...
    //so return the non-null list as the intersection of the two, if one is non-null
    if (left == NULL)
        return right;

    if (right == NULL)
        return left;

    std::sort(left->begin(), left->end(), std::less<REC_NO>());
    std::sort(right->begin(), right->end(), std::less<REC_NO>());

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

bool SdfQueryOptimizer::AreEqual(double/*&*/ d1, double/*&*/ d2)
{
    if (fabs(d1 - d2) <= SDF_GLOBAL_TOLERANCE)
        return true;

    return false;
}
