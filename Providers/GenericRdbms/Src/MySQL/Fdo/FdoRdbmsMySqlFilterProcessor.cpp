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
#include "FdoRdbmsMySqlFilterProcessor.h"
#include "../../Fdo/Schema/FdoRdbmsSchemaUtil.h"
#include <Geometry/Fgf/Factory.h>
#include "FdoCommonOSUtil.h"
#include <math.h>   // For floor() and ceil()

#define NUMBER_FORMAT_SIZE      (100)


// This list includes all the MySQL aggregate functions. Not all functions are officially supported
// but they are not being prevented either.
static wchar_t* mySqlAggregateFunctions[] = {

    L"AVG",  // supported
    L"BIT_AND", 
    L"BIT_OR",
    L"BIT_XOR",
    L"COUNT",  // supported
    L"GROUP_CONCAT", 
    L"MAX",   // supported
    L"MIN",   // supported
    L"STD",
    L"STDDEV_POP",
    L"STDDEV_SAMP",
    L"SUM",  // supported
    L"VAR_POP",
    L"VAR_SAMP", 
    L"VARIANCE", 
    NULL};


FdoRdbmsMySqlFilterProcessor::FdoRdbmsMySqlFilterProcessor(FdoRdbmsConnection *connection):
FdoRdbmsFilterProcessor( connection )
{
}

FdoRdbmsMySqlFilterProcessor::~FdoRdbmsMySqlFilterProcessor(void)
{
}

const FdoSmLpGeometricPropertyDefinition* FdoRdbmsMySqlFilterProcessor::GetGeometricProperty( const FdoSmLpClassDefinition* currentClass, const wchar_t *geomPropName )
{
    const FdoSmLpGeometricPropertyDefinition* geom = NULL;

	if( currentClass != NULL )
	{
        if ( geomPropName == NULL ) 
        {
            // No property name specified, default to the GeometryProperty
            const FdoSmLpFeatureClass* feat = 
                FdoSmLpFeatureClass::Cast(currentClass);

            if ( feat ) 
                geom = feat->RefGeometryProperty();
        }
        else {
            geom = FdoSmLpGeometricPropertyDefinition::Cast(currentClass->RefProperties()->RefItem(geomPropName));
        }
	}

	return geom;
}

/************************************************************************/
/* Get name of geometry column for the classname                                                                     */
/************************************************************************/
#define   NOTINDEXED_SUFFIX     L"_NIDX" 

FdoStringP FdoRdbmsMySqlFilterProcessor::GetGeometryColumnNameForProperty(const FdoSmLpGeometricPropertyDefinition* pGeomProp, bool bChangeFilter)
{
	FdoStringP retVal;

    if( pGeomProp != NULL )
    {
	    retVal = pGeomProp->GetColumnName();

   		if ( bChangeFilter) 
        {
            FdoStringP  tmpRetVal(retVal);

    	    FdoStringP suffixName = FdoStringP::Format( L"%ls",
		        	    (FdoString *) tmpRetVal.Mid( wcslen(retVal) - wcslen(NOTINDEXED_SUFFIX), wcslen(retVal)));

            // Strip the suffix from the column name
            if ( suffixName.ICompare(NOTINDEXED_SUFFIX) == 0 )
            {
                retVal = FdoStringP::Format(L"%ls", (FdoString *) tmpRetVal.Mid( 0, wcslen(retVal) - wcslen(NOTINDEXED_SUFFIX)));
            }
        }
	}

	return retVal;
}

/************************************************************************/
/* Process distance condition to emit spatial query string for direct   */
/* consumption by MySql-SDO                                            */
/* Currently this function processes distance query and creates an SDO  */
/* query string that can be directly issued to a database using SQLPlus */
/************************************************************************/
void FdoRdbmsMySqlFilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& filter)
{

}



/************************************************************************/
/* Process spatial condition to emit spatial query string for direct    */
/* consumption by MySql                                            */
/************************************************************************/
void FdoRdbmsMySqlFilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
//TODO: This function's implementation is a temporary one used by the MySql demo. It must be replaced
// by a proper implementation when full spatial support is added to the MySql provider.
//
// However, the following illustrates a problem that was encountered. Spatial query result sets become
// unreliable ( sometimes they work and sometimes they return no rows when there should be some ) when
// the spatial area coordinates contain fractional parts. This is currently worked around by casting 
// these coordinates to long.
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
    if ( classDefinition == NULL ||  classDefinition->GetClassType() != FdoClassType_FeatureClass )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_230, "Spatial condition can only be used with feature classes"));

    const FdoSmLpGeometricPropertyDefinition* geomProp = GetGeometricProperty(classDefinition, FdoPtr<FdoIdentifier>(filter.GetPropertyName())->GetName());
    const FdoString* classTableName = classDefinition->GetDbObjectName();
    const FdoString* tableName = geomProp ? geomProp->GetContainingDbObjectName() : L""; // The geometry table name
    FdoStringP columnName = GetGeometryColumnNameForProperty(geomProp, true);
    FdoStringP columnName2 = GetGeometryColumnNameForProperty(geomProp, false);

    FdoStringP spatialClause;
    FdoPtr<FdoGeometryValue> geom = dynamic_cast<FdoGeometryValue*>(filter.GetGeometry());
    FdoPtr<FdoFgfGeometryFactory>  gf;
    FdoPtr<FdoByteArray> geomFgf;
    FdoPtr<FdoIGeometry> geometryObj;

    geomFgf = geom->GetGeometry();

    if (geomFgf == NULL)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_46, "No geometry value"));

    // Geometry factory
    gf = FdoFgfGeometryFactory::GetInstance();

    geometryObj = gf->CreateGeometryFromFgf(geomFgf);

    FdoPtr<FdoIEnvelope> env = geometryObj->GetEnvelope();

    double minx = env->GetMinX();
    double miny = env->GetMinY();
    double maxx = env->GetMaxX();
    double maxy = env->GetMaxY();

    wchar_t minxStr[NUMBER_FORMAT_SIZE];
    wchar_t minyStr[NUMBER_FORMAT_SIZE];
    wchar_t maxxStr[NUMBER_FORMAT_SIZE];
    wchar_t maxyStr[NUMBER_FORMAT_SIZE];

    FdoCommonStringUtil::FormatDouble(minx, minxStr, NUMBER_FORMAT_SIZE);
    FdoCommonStringUtil::FormatDouble(miny, minyStr, NUMBER_FORMAT_SIZE);
    FdoCommonStringUtil::FormatDouble(maxx, maxxStr, NUMBER_FORMAT_SIZE);
    FdoCommonStringUtil::FormatDouble(maxy, maxyStr, NUMBER_FORMAT_SIZE);

    // In MySQL 5.0, there really is no secondary filter -- the MBR family of functions
    // give the same results as the non-MBR functions, and often aren't even as selective
    // as advertised.  Plus, some clients pass in geometry conditions with extra dimensions.
    // For now, we'll take advantage of MySQL's limitations and reduce all geometry types
    // and dimensionalities to a 2D polygon with one ring, and just use the MBRIntersects operator.
    FdoStringP buf = FdoStringP::Format( L"MBRIntersects(GeomFromText('Polygon((%ls %ls,%ls %ls,%ls %ls,%ls %ls,%ls %ls))'),%ls)",
            minxStr, minyStr,
            maxxStr, minyStr,
            maxxStr, maxyStr,
            minxStr, maxyStr,
            minxStr, minyStr,
            (FdoString*) columnName);
    AppendString((const wchar_t*)buf);
}


bool FdoRdbmsMySqlFilterProcessor::IsAggregateFunctionName(FdoString* wFunctionName) const
{
    for(int i=0; mySqlAggregateFunctions[i]; i++)
        if( FdoCommonOSUtil::wcsicmp(mySqlAggregateFunctions[i], wFunctionName) == 0 )
            return true;

    return false;
}
