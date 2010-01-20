//
// Copyright (C) 2006 Refractions Research, Inc. 
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
#ifndef FDORDBMSPOSTGISFILTERPROCESSOR_H
#define FDORDBMSPOSTGISFILTERPROCESSOR_H

#include "../../Fdo/Filter/FdoRdbmsFilterProcessor.h"

static wchar_t* PostGisSupportedFunctions[] = {

    // Agregate functions:
    FDO_FUNCTION_COUNT,
    FDO_FUNCTION_AVG,
    FDO_FUNCTION_MAX,
    FDO_FUNCTION_MIN,
    FDO_FUNCTION_SUM,
    FDO_FUNCTION_STDDEV,
    FDO_FUNCTION_SPATIALEXTENTS,
    // FDO_FUNCTION_MEDIAN,

    // Conversion Functions:
    FDO_FUNCTION_NULLVALUE,
    FDO_FUNCTION_TODATE,
    FDO_FUNCTION_TODOUBLE,
    FDO_FUNCTION_TOFLOAT,
    FDO_FUNCTION_TOINT32,
    FDO_FUNCTION_TOINT64,
    FDO_FUNCTION_TOSTRING,

    // Date Functions:
    FDO_FUNCTION_ADDMONTHS,
    FDO_FUNCTION_CURRENTDATE,
    // FDO_FUNCTION_EXTRACT,
    FDO_FUNCTION_EXTRACTTODOUBLE,
    FDO_FUNCTION_EXTRACTTOINT,
    FDO_FUNCTION_MONTHSBETWEEN,

    // Math functions:
    //FDO_FUNCTION_ABS,        
    //FDO_FUNCTION_ACOS,       
    //FDO_FUNCTION_ASIN,       
    //FDO_FUNCTION_ATAN,       
    //FDO_FUNCTION_ATAN2,      
    //FDO_FUNCTION_COS,        
    //FDO_FUNCTION_EXP,        
    //FDO_FUNCTION_LN,         
    FDO_FUNCTION_LOG,
    FDO_FUNCTION_MOD,       
    //FDO_FUNCTION_POWER,                                  
    FDO_FUNCTION_REMAINDER,  
    //FDO_FUNCTION_SIN,        
    //FDO_FUNCTION_SQRT,       
    //FDO_FUNCTION_TAN,        

    // Numeric functions:
    //FDO_FUNCTION_CEIL,   
    //FDO_FUNCTION_FLOOR,  
    FDO_FUNCTION_ROUND,  
    //FDO_FUNCTION_SIGN,   
    FDO_FUNCTION_TRUNC,                        
  

    // String functions:
    FDO_FUNCTION_CONCAT,     
    FDO_FUNCTION_INSTR,      
    //FDO_FUNCTION_LENGTH,     
    //FDO_FUNCTION_LOWER,      
    //FDO_FUNCTION_LPAD,       
    //FDO_FUNCTION_LTRIM,      
    //FDO_FUNCTION_RPAD,       
    //FDO_FUNCTION_RTRIM,      
    //FDO_FUNCTION_SOUNDEX,    
    //FDO_FUNCTION_SUBSTR,     
    //FDO_FUNCTION_TRANSLATE,  
    //FDO_FUNCTION_TRIM,       
    //FDO_FUNCTION_UPPER,      

    // Geometry functions:
    FDO_FUNCTION_AREA2D,   
    //FDO_FUNCTION_LENGTH2D,  
    //FDO_FUNCTION_X,         
    //FDO_FUNCTION_Y,         
    //FDO_FUNCTION_Z,         
    //FDO_FUNCTION_M,         
    NULL
};

/// \brief
/// Implements tree hierarchy filter for for PostGIS provider.
/// 
/// \sa
/// FdoIFilterProcessor - top-level base class of Filter Processors
/// FdoRdbmsBaseFilterProcessor- base Filter Process class for
/// RDBMS providers
///
/// \todo TODO: mloskot - Reconsider elements of local array of aggregate
/// functions (mySqlAggregateFunctions).
/// \todo TODO: mloskot - What is SDO used in PostGIS-SDO
/// (formerly named MySQL-SDO) below?
/// 
class FdoRdbmsPostGisFilterProcessor: public FdoRdbmsFilterProcessor
{
public:

    /// Custom constructor.
    /// @param connection [in] - pointer to instance of PostGIS connection
    ///
    FdoRdbmsPostGisFilterProcessor(FdoRdbmsConnection *connection);

    /// Destructor.
    ~FdoRdbmsPostGisFilterProcessor();

protected:

    /// Processes distance condition passed in.
    /// This function processese distance condition to emit spatial
    /// query string for direct consumption by PostGIS-SDO.
    /// Currently this function processes distance query and creates an SDO
    /// query string that can be directly issued to a database using SQLPlus.
    ///
    /// @param filter [in] - distance condition.
    /// 
    /// \todo NOT IMPLEMENTED, also in MySQL provider.
    ///
    virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);

    /// Processes spatial condition passed in.
    /// This function processes spatial condition to emit spatial query
    /// string for direct consumption by PostGIS.
    ///
    /// @param filter [in] - spatial condition.
    /// 
    /// \todo
    /// TODO: The implementation of this func is temporary one used by
    /// the PostGIS (formerly MySQL) demo. It must be replaced by proper
    /// implementation when full spatial support is added to the PostGIS
    /// (MySQL)  provider. However, the following illustrates a problem
    /// that was encountered.
    /// Spatial query result sets become unreliable (sometimes they work
    /// and sometimes they return no rows when there should be some) whe
    /// the spatial area coordinates contain fractional parts.
    /// This is currently worked around by casting these coordinates to long.
    ///
    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    /// 
    /// @param currentClass [in] - 
    /// @param geomPropName [in] - 
    /// @return 
    ///
    const FdoSmLpGeometricPropertyDefinition* GetGeometricProperty(
            const FdoSmLpClassDefinition* currentClass,
            const wchar_t* geomPropName);
	
    /// 
    /// @param pGeomProp [in] - 
    /// @param bChangeFilter [in] - 
    /// @return 
    /// 
    FdoStringP GetGeometryColumnNameForProperty(
            const FdoSmLpGeometricPropertyDefinition* pGeomProp,
            bool bChangeFilter = false);

    /// 
    /// @param wFunctionName [in] - 
    /// @return 
    /// 
    virtual bool IsAggregateFunctionName(FdoString* wFunctionName) const;

    void ProcessFunction (FdoFunction &expr);

    virtual bool IsNotNativeSupportedFunction(FdoString* wFunctionName) const;
    
    virtual bool HasNativeSupportedFunctionArguments(FdoFunction& expr) const;

private:
    void ProcessSpatialDistanceCondition(FdoGeometricCondition* condition, bool isSpatial );
    void BuildSpatialFilter( FdoStringP columnName, FdoStringP geomFromText, FdoSpatialCondition *spatialFilter);
    void BuildDistanceFilter( FdoStringP columnName, FdoStringP geomFromText, FdoDistanceCondition *spatialFilter);
};

#endif // FDORDBMSPOSTGISFILTERPROCESSOR_H
