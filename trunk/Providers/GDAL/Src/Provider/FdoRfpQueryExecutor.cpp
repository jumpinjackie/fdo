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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpQueryExecutor.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpQueryExecutor.h"
#include "FdoRfpQueryResult.h"
#include "FdoRfpGlobals.h"
#include "FdoRfpFilterEvaluator.h"
#include "FdoRfpGeoRaster.h"
#include "FdoRfpClassData.h"
#include <vector>

FdoRfpQueryExecutor::FdoRfpQueryExecutor(const FdoPtr<FdoClassDefinition>& classDef, 
					const FdoPtr<FdoFilter>& filter,
					const FdoPtr<FdoIdentifierCollection>& props,
					const FdoPtr<FdoRfpClassData>& classData) :
	m_classDef(classDef), m_filter(filter), m_propsToSelect(props), m_classData(classData)
{
}

FdoPtr<FdoRfpQueryResult> FdoRfpQueryExecutor::Execute()
{
    FdoPtr<FdoRfpQueryResult> queryResult = new FdoRfpQueryResult();

    // We must find out the following information for each column during the iteration
    // 1. Clipping bounds if CLIP function(s) applied to the column
    // 2. Whether aggregated function (e.g. MOSAIC) is applied
    // 3. The property types for each column
    // 4. All valid names for each column, these names can be used to fetch data later
    if (!(m_propsToSelect == NULL || m_propsToSelect->GetCount() == 0))
    {
        FdoInt32 count = m_propsToSelect->GetCount();
        for (int i = 0; i < count; i++)
        {

            FdoPtr<FdoIdentifier> identifier = m_propsToSelect->GetItem(i);
            FdoStringP propertyQName;
            bool bMosaic = false, bClip = false, bResample = false;
            FdoSize height = 0, width = 0;
            FdoRfpRect bounds(0, 0, 0, 0);
            // try to find out whether the identifier is a computed identifier that contains
            // functions and whether MOSAIC() function and/or CLIP() function is/are specified
            // and whether they are correctly specified
            _analyseIdentifier(identifier, bMosaic, bClip, bResample, bounds, propertyQName, height, width);

            // 1. Clipping bounds if CLIP/RESAMPLE function(s) applied to the column
            if (bClip || bResample)
                queryResult->clippingBounds.push_back(new FdoRfpRect(bounds));
            else
                queryResult->clippingBounds.push_back(NULL);

            // Get the Height/Width value is RESAMPLE applied, else filled with 0. 
            queryResult->heightValues.push_back(bResample ? height : 0);
            queryResult->widthValues.push_back(bResample ? width : 0);			

            // 2. Whether aggregated function (MOSAIC) is applied
            if ((bMosaic && !queryResult->aggregated && i != 0) ||
                (!bMosaic && queryResult->aggregated))
                throw FdoCommandException::Create(NlsMsgGet(GRFP_85_MOSAIC_INCORRECTLY_USED, "MOSAIC() is incorrectly used."));
            if (bMosaic)
                queryResult->aggregated = true;

            //Verify the identifier exists in the class
            PropertyType propType;
            // first verify the qualified name which imply the feature schema name and class name
            FdoStringP propertyName;
            _verifyPropertyQName(propertyQName, propertyName);

            FdoPtr<FdoPropertyDefinitionCollection> propsDef = m_classDef->GetProperties();
            FdoPtr<FdoPropertyDefinition> prop = propsDef->FindItem(propertyName);
            if (prop == NULL)
                throw FdoCommandException::Create(NlsMsgGet2(GRFP_53_PROPERTY_NOT_EXIST, "The property '%1$ls' does not exist in class '%2$ls'.", (FdoString*)propertyName, (FdoString*)(m_classDef->GetQualifiedName())));

            // 3. The property types for each column
            if (prop->GetPropertyType() == FdoPropertyType_DataProperty)
                propType = PropertyType_ID;
            else
                propType = PropertyType_Raster;
            queryResult->propertyTypes.push_back(propType);

            //MOSAIC or CLIP function can not be applied to Id property
            if ((bMosaic || bClip || bResample) && propType == PropertyType_ID)
                throw FdoCommandException::Create(NlsMsgGet(GRFP_55_MOSAIC_CLIP_RESAMPLE_ONLY_APPLY_TO_RASTER, "MOSAIC(), CLIP() or RESAMPLE() can only applied to raster property."));

            // 4. All valid names for each column, these names can be used to fetch data later
            // find out all valid identifers for the property. e.g. "FeatId", "ClassName.FeatId" and 
            // "FeatureName:ClassName.FeatId" are all valid identifiers for property FeatureName:ClassName.FeatId.
            std::vector<FdoStringP>* ids = new std::vector<FdoStringP>();
            queryResult->identifiers.push_back(ids);

            FdoString* name = identifier->GetName();
            ids->push_back(name);
            if (!bMosaic && !bClip && !bResample)
            {
                FdoStringP qname = m_classDef->GetName();
                qname += L".";
                qname += name;
                ids->push_back(qname);
                qname = m_classDef->GetQualifiedName();
                qname += L".";
                qname += name;
                ids->push_back(qname);
            }
        }
    }
    else //properties to select not set, like "SELECT *"
    {
        FdoPtr<FdoPropertyDefinitionCollection> properties = m_classDef->GetProperties();
        FdoInt32 count = properties->GetCount();
        for (int i = 0; i < count; i++)
        {
            // 1. Clipping bounds if CLIP function(s) applied to the column
            queryResult->clippingBounds.push_back(NULL); // no clipping for sure
            queryResult->heightValues.push_back(0);
            queryResult->widthValues.push_back(0);

            // 2. Whether aggregated function (MOSAIC) is applied
            queryResult->aggregated = false;

            // 3. The property types for each column
            FdoPtr<FdoPropertyDefinition> prop = properties->GetItem(i);
            PropertyType propType = prop->GetPropertyType() == FdoPropertyType_DataProperty ? PropertyType_ID : PropertyType_Raster;
            queryResult->propertyTypes.push_back(propType);
			
            // 4. All valid names for each column, these names can be used to fetch data later
            std::vector<FdoStringP>* ids = new std::vector<FdoStringP>();
            queryResult->identifiers.push_back(ids);
            FdoString* name = prop->GetName();
            ids->push_back(name);
            FdoStringP qname = m_classDef->GetName();
            qname += L".";
            qname += name;
            ids->push_back(qname);
            qname = m_classDef->GetQualifiedName();
            qname += L".";
            qname += name;
            ids->push_back(qname);
        }
    }

    //Evaluate each record of the class
    FdoPtr<FdoRfpGeoRasterCollection> qualifiedRecs = FdoRfpGeoRasterCollection::Create();
    FdoRfpGeoRastersP queryData = m_classData->GetGeoRasters();
    FdoPtr<FdoRfpFilterEvaluator> evaluator = FdoRfpFilterEvaluator::Create();
    evaluator->SetClass(m_classDef);
    evaluator->SetFilter(m_filter);
    FdoInt32 count = queryData->GetCount();
    for (int i = 0; i < count; i++)
    {
        FdoRfpGeoRasterP geoRaster = queryData->GetItem(i);
        evaluator->SetGeoRaster(geoRaster);
        if (evaluator->Evaluate())			
            qualifiedRecs->Add(geoRaster);
    }

    // add to return value
    queryResult->resultSet->Clear();
    count = qualifiedRecs->GetCount();
    if (count > 0)
    {
        if (queryResult->aggregated)
        {
            queryResult->resultSet->Add(qualifiedRecs);
        }
        else
        {
            for (int i = 0; i < count; i++)
            {
                FdoPtr<FdoRfpGeoRasterCollection> oneRec = FdoRfpGeoRasterCollection::Create();
                FdoPtr<FdoRfpGeoRaster> geoRaster = qualifiedRecs->GetItem(i);
                oneRec->Add(geoRaster);
                queryResult->resultSet->Add(oneRec);
            }
        }
    }

    // all done
    return queryResult;

}

void FdoRfpQueryExecutor::_verifyPropertyQName(FdoString* propertyQName, FdoStringP& propertyName)
{
	// propertyQName is a qualified name
	// so find out the feature name, class name and property name
	FdoStringP featureName, className;
	FdoRfpUtil::ParseQPropertyName(propertyQName, featureName, className, propertyName);
	// first verify feature schema name
	if (featureName.GetLength() != 0)
	{
		FdoPtr<FdoSchemaElement> featureSchema = m_classDef->GetParent();
		if (!STRCASEEQ(featureName, featureSchema->GetName()))
			throw FdoCommandException::Create(NlsMsgGet2(GRFP_53_PROPERTY_NOT_EXIST, "The property '%1$ls' does not exist in class '%2$ls'.", propertyQName, (FdoString*)(m_classDef->GetQualifiedName())));
	}
	// then verify class name
	if (className.GetLength() != 0)
	{
		if (!STRCASEEQ(className, m_classDef->GetName()))
			throw FdoCommandException::Create(NlsMsgGet2(GRFP_53_PROPERTY_NOT_EXIST, "The property '%1$ls' does not exist in class '%2$ls'.", propertyQName, (FdoString*)(m_classDef->GetQualifiedName())));
	}

}

void FdoRfpQueryExecutor::_analyseIdentifier(const FdoPtr<FdoIdentifier>& identifier, 
							bool& hasMosaic, bool& hasClip, bool& hasResample, FdoRfpRect& clippingBounds, 
							FdoStringP& propertyName, FdoSize& height, FdoSize& width)
{
	hasMosaic = false;
	hasClip = false;
	hasResample = false;
	height = 0;
	width = 0;
	propertyName = L"";
	
	FdoPtr<FdoComputedIdentifier> computedIdentifier = SP_DYNAMIC_CAST(FdoComputedIdentifier, identifier);
	if (computedIdentifier == NULL)
	{
		propertyName = identifier->GetText();
		return;
	}
	FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
	FdoPtr<FdoFunction> func = SP_DYNAMIC_CAST(FdoFunction, expr);
	while (true)
	{
		if (func == NULL || !(STREQUAL(FdoGrfpGlobals::MosaicFunctionName, func->GetName()) ||
			STREQUAL(FdoGrfpGlobals::ClipFunctionName, func->GetName()) ||
			STREQUAL(FdoGrfpGlobals::ResampleFunctionName, func->GetName()) ))
			throw FdoCommandException::Create(NlsMsgGet(GRFP_52_ONLY_MOSAIC_CLIP_RESAMPLE_SUPPORTED, "Only MOSAIC(), CLIP() and RESAMPLE() are supported."));
		
		FdoPtr<FdoExpressionCollection> args = func->GetArguments();
		FdoPtr<FdoExpression> arg0;
		if (STREQUAL(FdoGrfpGlobals::MosaicFunctionName, func->GetName())) // MOSAIC FUNCTION
		{
			if (args->GetCount() != 1 || hasMosaic == true)
				throw FdoCommandException::Create(NlsMsgGet(GRFP_85_MOSAIC_INCORRECTLY_USED, "MOSAIC() is incorrectly used."));
			
			arg0 = args->GetItem(0);
			hasMosaic = true;
		}
		else if (STREQUAL(FdoGrfpGlobals::ClipFunctionName, func->GetName())) // CLIP FUNCTION
		{
			FdoPtr<FdoExpressionCollection> args = func->GetArguments();
			if (args->GetCount() != 5)
				throw FdoCommandException::Create(NlsMsgGet(GRFP_84_CLIP_INCORRECTLY_USED, "CLIP() is incorrectly used."));
			
			arg0 = args->GetItem(0);
			
			FdoPtr<FdoExpression> arg = args->GetItem(1);
			FdoPtr<FdoDataValue> minX = SP_DYNAMIC_CAST(FdoDataValue, arg);
			arg = args->GetItem(2);
			FdoPtr<FdoDataValue> minY = SP_DYNAMIC_CAST(FdoDataValue, arg);
			arg = args->GetItem(3);
			FdoPtr<FdoDataValue> maxX = SP_DYNAMIC_CAST(FdoDataValue, arg);
			arg = args->GetItem(4);
			FdoPtr<FdoDataValue> maxY = SP_DYNAMIC_CAST(FdoDataValue, arg);
			if (minX == NULL || minY == NULL || maxX == NULL || maxY == NULL ||
				(minX->GetDataType() != FdoDataType_Double && 
                minX->GetDataType() != FdoDataType_Int16 &&
                minX->GetDataType() != FdoDataType_Int32 && 
                minX->GetDataType() != FdoDataType_Int64) ||
				(minY->GetDataType() != FdoDataType_Double && 
                minY->GetDataType() != FdoDataType_Int16 &&
                minY->GetDataType() != FdoDataType_Int32 && 
                minY->GetDataType() != FdoDataType_Int64)  ||
				(maxX->GetDataType() != FdoDataType_Double && 
                maxX->GetDataType() != FdoDataType_Int16 &&
                maxX->GetDataType() != FdoDataType_Int32 && 
                maxX->GetDataType() != FdoDataType_Int64)  ||
				(maxY->GetDataType() != FdoDataType_Double && 
                maxY->GetDataType() != FdoDataType_Int16 &&
                maxY->GetDataType() != FdoDataType_Int32 && 
                maxY->GetDataType() != FdoDataType_Int64) )
				throw FdoCommandException::Create(NlsMsgGet(GRFP_84_CLIP_INCORRECTLY_USED, "CLIP() is incorrectly used."));
			
			FdoRfpRect bounds(atof((const char*)FdoStringP(minX->ToString())),
								atof((const char*)FdoStringP(minY->ToString())),
								atof((const char*)FdoStringP(maxX->ToString())),
								atof((const char*)FdoStringP(maxY->ToString())));
			if (!hasClip)
			{
				hasClip = true;
				clippingBounds = bounds;
			}
			else // nested clip functions imply intersection of all bounds
				clippingBounds = clippingBounds.Intersect(bounds);	
		}
		else if (STREQUAL(FdoGrfpGlobals::ResampleFunctionName, func->GetName())) // RESAMPLE function
		{
			// RESAMPLE function can not be nested.
			if (hasResample)
				throw FdoCommandException::Create(NlsMsgGet(GRFP_105_RESAMPLE_INCORRECTLY_USED, "RESAMPLE() is incorrectly used."));

			FdoPtr<FdoExpressionCollection> args = func->GetArguments();
			if (args->GetCount() != 7)
				throw FdoCommandException::Create(NlsMsgGet(GRFP_105_RESAMPLE_INCORRECTLY_USED, "RESAMPLE() is incorrectly used."));
			
			arg0 = args->GetItem(0);
			
			FdoPtr<FdoExpression> arg = args->GetItem(1);
			FdoPtr<FdoDataValue> minX = SP_DYNAMIC_CAST(FdoDataValue, arg);
			arg = args->GetItem(2);
			FdoPtr<FdoDataValue> minY = SP_DYNAMIC_CAST(FdoDataValue, arg);
			arg = args->GetItem(3);
			FdoPtr<FdoDataValue> maxX = SP_DYNAMIC_CAST(FdoDataValue, arg);
			arg = args->GetItem(4);
			FdoPtr<FdoDataValue> maxY = SP_DYNAMIC_CAST(FdoDataValue, arg);
			arg = args->GetItem(5);
			FdoPtr<FdoDataValue> heightArg = SP_DYNAMIC_CAST(FdoDataValue, arg);
			arg = args->GetItem(6);
			FdoPtr<FdoDataValue> widthArg = SP_DYNAMIC_CAST(FdoDataValue, arg);
			if (minX == NULL || minY == NULL || maxX == NULL || maxY == NULL ||
				(minX->GetDataType() != FdoDataType_Double && 
                minX->GetDataType() != FdoDataType_Int16 &&
                minX->GetDataType() != FdoDataType_Int32 && 
                minX->GetDataType() != FdoDataType_Int64) ||
				(minY->GetDataType() != FdoDataType_Double && 
                minY->GetDataType() != FdoDataType_Int16 &&
                minY->GetDataType() != FdoDataType_Int32 && 
                minY->GetDataType() != FdoDataType_Int64)  ||
				(maxX->GetDataType() != FdoDataType_Double && 
                maxX->GetDataType() != FdoDataType_Int16 &&
                maxX->GetDataType() != FdoDataType_Int32 && 
                maxX->GetDataType() != FdoDataType_Int64)  ||
				(maxY->GetDataType() != FdoDataType_Double && 
                maxY->GetDataType() != FdoDataType_Int16 &&
                maxY->GetDataType() != FdoDataType_Int32 && 
                maxY->GetDataType() != FdoDataType_Int64) ||
				(heightArg->GetDataType() != FdoDataType_Double && 
				heightArg->GetDataType() != FdoDataType_Int16 &&
				heightArg->GetDataType() != FdoDataType_Int32 && 
				heightArg->GetDataType() != FdoDataType_Int64) ||
				(widthArg->GetDataType() != FdoDataType_Double && 
				widthArg->GetDataType() != FdoDataType_Int16 &&
				widthArg->GetDataType() != FdoDataType_Int32 && 
				widthArg->GetDataType() != FdoDataType_Int64) )
				throw FdoCommandException::Create(NlsMsgGet(GRFP_105_RESAMPLE_INCORRECTLY_USED, "RESAMPLE() is incorrectly used."));
			
			FdoRfpRect bounds(atof((const char*)FdoStringP(minX->ToString())),
								atof((const char*)FdoStringP(minY->ToString())),
								atof((const char*)FdoStringP(maxX->ToString())),
								atof((const char*)FdoStringP(maxY->ToString())));

			height = atoi ((const char*)FdoStringP(heightArg->ToString ()));
			width = atoi ((const char*)FdoStringP(widthArg->ToString ()));
			
			hasResample = true;

			// RESAMPLE function can be used with CLIP function.
			if (!hasClip)
			{
				clippingBounds = bounds;
			}
			else
			{
				clippingBounds = clippingBounds.Intersect (bounds);
			}			
		}

		// try to find the next nested function
		// first try function
		func = SP_DYNAMIC_CAST(FdoFunction, arg0);
		if (func == NULL)
		{
			// then try computed identifier
			FdoPtr<FdoComputedIdentifier> computedIdentifier = SP_DYNAMIC_CAST(FdoComputedIdentifier, arg0);
			if (computedIdentifier != NULL)
			{
				FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
                func = SP_DYNAMIC_CAST(FdoFunction, expr);
			}
		}
		// Ok, arg0 must be an identifier, otherwise there must be somthing wrong with user's input
		if (func == NULL)
		{
			// try general identifier
			FdoPtr<FdoIdentifier> id = SP_DYNAMIC_CAST(FdoIdentifier, arg0);
			if (id == NULL)
			{
				// throw out proper exception message
				if (STREQUAL(FdoGrfpGlobals::MosaicFunctionName, func->GetName()))
					throw FdoCommandException::Create(NlsMsgGet(GRFP_85_MOSAIC_INCORRECTLY_USED, "MOSAIC() is incorrectly used."));
				else if (STREQUAL(FdoGrfpGlobals::ClipFunctionName, func->GetName()))
					throw FdoCommandException::Create(NlsMsgGet(GRFP_84_CLIP_INCORRECTLY_USED, "CLIP() is incorrectly used."));
				else if (STREQUAL(FdoGrfpGlobals::ResampleFunctionName, func->GetName()))
					throw FdoCommandException::Create(NlsMsgGet(GRFP_105_RESAMPLE_INCORRECTLY_USED, "RESAMPLE() is incorrectly used."));
			}
			propertyName = id->GetText();
			break;
		}

	}
}

