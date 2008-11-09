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
#include "PropertyIndex.h"
#include <vector>

#include "RTree.h"
#include "KeyDb.h"

typedef std::vector<recno_list*> qo_retval_stack;
typedef std::vector<FdoFilter*> filter_stack;

//typedef std::vector<int> idprop_indices;
//typedef std::vector<idprop_indices*> idprop_stack;


///<summary>
///Processes Filter tree and looks for spatial queries which can be evaluated
///by searching the feature class R-Tree. In the end the result is a set
///of feature record numbers that we need to consider for that query
///For example if the Filter contains a spatial condition which limits features
///to a given extent, we only need to read and execute the full query on those
///particular features
///This class also looks for conditions on feature key (identity properties) 
///and uses the KeyDb database to look up record numbers based on feature key 
///queries.
///It can produce a modified FdoFilter based on the conditions which were
///not examined by the query optimizer itself. This filter can then be applied
///to the output features of the query optimizer to complete the filtering
///operation.
///</summary>
//TODO: get rid of the multiple inheritance!!!
class SdfQueryOptimizer : public FdoIFilterProcessor, public FdoIExpressionProcessor
{

public:

    SDF_API SdfQueryOptimizer(SdfRTree* rtree, KeyDb* keys, FdoClassDefinition* classDef);
    SDF_API ~SdfQueryOptimizer();

protected:

    SDF_API virtual void Dispose();

public:

    //--------------------------------------------------------------------
    // FdoIFilterProcessor
    //--------------------------------------------------------------------

    SDF_API virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    SDF_API virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    SDF_API virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    SDF_API virtual void ProcessInCondition(FdoInCondition& filter);

    SDF_API virtual void ProcessNullCondition(FdoNullCondition& filter);

    SDF_API virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    SDF_API virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);


    //--------------------------------------------------------------------
    // FdoIExpressionProcessor
    //--------------------------------------------------------------------

    SDF_API virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

    SDF_API virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    SDF_API virtual void ProcessFunction(FdoFunction& expr);

    SDF_API virtual void ProcessIdentifier(FdoIdentifier& expr);

	SDF_API virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr) { return; }

    SDF_API virtual void ProcessParameter(FdoParameter& expr);

    SDF_API virtual void ProcessBooleanValue(FdoBooleanValue& expr);

    SDF_API virtual void ProcessByteValue(FdoByteValue& expr);

    SDF_API virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    SDF_API virtual void ProcessDecimalValue(FdoDecimalValue& expr);

    SDF_API virtual void ProcessDoubleValue(FdoDoubleValue& expr);

    SDF_API virtual void ProcessInt16Value(FdoInt16Value& expr);

    SDF_API virtual void ProcessInt32Value(FdoInt32Value& expr);

    SDF_API virtual void ProcessInt64Value(FdoInt64Value& expr);

    SDF_API virtual void ProcessSingleValue(FdoSingleValue& expr);

    SDF_API virtual void ProcessStringValue(FdoStringValue& expr);

    SDF_API virtual void ProcessBLOBValue(FdoBLOBValue& expr);

    SDF_API virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    SDF_API virtual void ProcessGeometryValue(FdoGeometryValue& expr);

public:

    //-----------------------------------------------------------------
    // SdfQueryOptimizer
    //-----------------------------------------------------------------
    
    recno_list* GetResult();
    FdoFilter* GetOptimizedFilter();

private:

    //helper functions
    recno_list* RecnoFromKey(FdoPropertyValueCollection* keyvals = NULL);
    bool IsAxisAlignedRectangle(FdoByteArray* fgf, Bounds& bounds);
    bool IsAllDataEnclosed( Bounds& bounds );
    recno_list* recno_list_union(recno_list* left, recno_list* right);
    recno_list* recno_list_intersection(recno_list* left, recno_list* right);
    bool AreEqual(double d1, double d2);

/* Data Type conversion (FdoDataValue::Create()) clone. Can be removed when data type
   conversion is added to the FDO API
*/
    FdoDataValue* SdfQueryOptimizer::ConvertDataValue(
        FdoDataType dataType,
        FdoDataValue* src, 
        FdoBoolean nullIfIncompatible,
        FdoBoolean shift,
        FdoBoolean truncate
    );
/* End Data Type conversion */

/* Data Type conversion template clones. Can be removed when data type
   conversion is added to the FDO API
*/

    // Converts values that may need shifting to other values.
    template <class CI,             // input FdoDataValue 
              class CO,             // output FdoDataValue
              class TI,             // input FDO type - must correspond to CI
              class TO              // output FDO type - must correspond to CO
    > 
    static CO* Convert( 
        CI* obj,                            // input FdoDataValue
        TI val,                             // input scalar value for obj
        TO min,                             // minimum value for TO type
        TO max,                             // maximum value for TO type
        TI round,                           // amount for rounding 
        FdoBoolean nullIfIncompatible,      // see FdoDataValue::Create()
        FdoBoolean shift,                   // see FdoDataValue::Create()
        FdoBoolean truncate,                // see FdoDataValue::Create()
        FdoString* sTO                       // TO in string format; for exception messages.
    )
    {
        CO*         ret = NULL;
        FdoBoolean  isNull = false;
        TO          out;

        // First, truncate the value to be between output type min and max.
        Truncate<TI,TO>( val, out, isNull, min, max, nullIfIncompatible, truncate, sTO );
                
        if ( isNull ) 
        {
            // Output determined to be null
            ret = CO::Create();
        }
        else if ( (val < min) || (val > max) ) 
        {
            // value was truncated. Wrap it in FdoDataValue.
            ret = CO::Create(out);
        }
        else
        {
            // Round the value and convert it
            out = (TO) ((val < 0) ? (val - round) : (val + round));

            if ( (out != val) && !shift ) 
                ret = CO::Create();
            else
                ret = CO::Create(out);
        }

        return ret;
    };

    // Converts values that don't need shifting, but may need truncation,
    // to other values.
    template <
        class C,                    // output FdoDataValue
        class TI,                   // input FDO type 
        class TO                    // output FDO type - must correspond to CO
    > 
    static C* Convert( 
        TI val,                             // input scalar value for obj
        TO min,                             // minimum value for TO type
        TO max,                             // maximum value for TO type
        FdoBoolean nullIfIncompatible,      // see FdoDataValue::Create()
        FdoBoolean truncate,                // see FdoDataValue::Create()
        FdoString* sTO                      // TO in string format; for exception messages.
    )
    {
        C*          ret    = NULL;
        FdoBoolean  isNull = false;
        TO          out;

        // First, truncate the value to be between output type min and max.
        Truncate<TI, TO>( val, out, isNull, min, max, nullIfIncompatible, truncate, sTO );
                
        if ( isNull ) 
            // Output determined to be null
            ret = C::Create();
        else
            // Wrap output in an FdoDataValue.
            ret = C::Create( out );

        return ret;
    };

    // Truncates a value to be between a minimum and maximum value.
    template <
        class TI,                   // input FDO type 
        class TO                    // output FDO type
    > 
    static bool Truncate( 
        TI in,                      // original value
        TO& out,                    // truncated value
        bool& isNull,               // set to true if value truncated and
                                    // truncate=false and nullIfIncompatible=true
        TO min,                     // minimum value for TO type
        TO max,                     // maximum value for TO type
        bool nullIfIncompatible,    // see FdoDataValue::Create()
        bool truncate,              // see FdoDataValue::Create()
        FdoString* sTO              // TO in string format; for exception messages.
    )
    {
        bool success = true;

        // no truncation by default. However, it is up to the caller to 
        // determine if casting shifted the value.
        out = (TO) in;

        if ( in < min ) 
            success = Truncate<TI, TO>( in, out, isNull, min, nullIfIncompatible, truncate );   
        else if ( in > max ) 
            success = Truncate<TI, TO>( in, out, isNull, max, nullIfIncompatible, truncate );

        if ( !success )
            isNull = true;

        return true;
    };

    // Truncates a value to a range endpoint
    template <
        class TI,                   // input FDO type 
        class TO                    // output FDO type
    > 
    static bool Truncate( 
        TI in,                      // input value
        TO& out,                    // output value
        bool& isNull,               // set to true truncate=false and 
                                    // nullIfIncompatible=true 
        TO bound,                   // the range endpoint
        bool nullIfIncompatible,    // see FdoDataValue::Create() 
        bool truncate               // see FdoDataValue::Create()
    )
    {
        bool ret = true;
        
        if ( truncate ) 
        {
            // Truncation allowed so set output to the range endpoint
            out = bound;
        }
        else
        {
            if ( nullIfIncompatible ) 
                // Truncation not allowed so set output to null
                isNull = true;
            else
                // Truncation not allowed so raise an error
                ret = false;
        }

        return ret;
    };
/* End of Data Type conversion template clones. */

    //working set of features and key property values
    qo_retval_stack m_retvals;
    FdoPropertyValueCollection* m_keyvals;
    filter_stack m_filters;

    //FDO context
    SdfRTree* m_rtree;
    KeyDb* m_keys;
    FdoClassDefinition* m_class;
    FdoDataPropertyDefinitionCollection* m_idprops;

};

