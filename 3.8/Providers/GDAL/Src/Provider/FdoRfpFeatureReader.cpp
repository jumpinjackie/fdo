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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpFeatureReader.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpFeatureReader.h"
#include "FdoCommonSchemaUtil.h"

FdoRfpFeatureReader::FdoRfpFeatureReader(const FdoClassDefinitionP& classDef, const FdoPtr<FdoRfpQueryResult>& queryResult) :
											superclass(queryResult), m_classDef(classDef)
{
}

FdoRfpFeatureReader* FdoRfpFeatureReader::Create(const FdoClassDefinitionP& classDef, const FdoPtr<FdoRfpQueryResult>& queryResult)
{
	FdoRfpReatureReaderP fr = new FdoRfpFeatureReader(classDef, queryResult);

	return FDO_SAFE_ADDREF(fr.p);
}

/// <summary>Gets the definition of the object currently being read. If the user
/// has requested only a subset of the class properties, the class 
/// definition reflects what the user has asked, rather than the full class 
/// definition.</summary>
/// <returns>Returns the class definition object.</returns> 
FdoClassDefinition* FdoRfpFeatureReader::GetClassDefinition()
{
    if (m_classDefDynamic == NULL)
    {
        m_classDefDynamic = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(m_classDef);
        FdoPtr<FdoPropertyDefinitionCollection> props = m_classDefDynamic->GetProperties();
        // Find out the raster property. One of the two properties much be raster property
        FdoPtr<FdoRasterPropertyDefinition> rasterProp;
        if (FdoPtr<FdoPropertyDefinition>(props->GetItem(0))->GetPropertyType() ==
            FdoPropertyType_RasterProperty)
            rasterProp = static_cast<FdoRasterPropertyDefinition*>(FDO_SAFE_ADDREF(props->GetItem(0)));
        else
            rasterProp = static_cast<FdoRasterPropertyDefinition*>(FDO_SAFE_ADDREF(props->GetItem(1)));

        bool bOrgRasterSelected = false;
        for (std::vector<PropertyType>::size_type i = 0;
            i < m_queryResult->propertyTypes.size(); i++)
        {
            if (m_queryResult->propertyTypes[i] != PropertyType_Raster)
                continue;
            FdoString* currentPropName = (*(m_queryResult->identifiers[i]))[0];
            if (STRCASEEQ(rasterProp->GetName(), currentPropName))
                bOrgRasterSelected = true;
            else
            {
                FdoPtr<FdoPropertyDefinition> computedRasterProp = FdoCommonSchemaUtil::DeepCopyFdoPropertyDefinition(rasterProp);
                // change the name and add to class definition
                computedRasterProp->SetName(currentPropName);
                props->Add(computedRasterProp);
                m_classDefDynamic->SetIsComputed(true);
            }
        }

        // remove the original raster property if it is not in the properties to select
        if (!bOrgRasterSelected)
            props->Remove(rasterProp);
    }
    return FDO_SAFE_ADDREF(m_classDefDynamic.p);
}

/// <summary>Gets a value indicating the depth of nesting for the current reader.
/// The depth value increases each time GetFeatureObject is called and a new 
/// reader is returned. The outermost reader has a depth of 0.</summary>
/// <returns>Returns the depth</returns> 
FdoInt32 FdoRfpFeatureReader::GetDepth()
{
	return 0;
}

/// <summary>Gets a reference to an FdoIFeatureReader to read the data contained in
/// the object or object collection property. If the property is not an
/// object property, an exception is thrown.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <returns>Returns the nested feature reader</returns> 
FdoIFeatureReader* FdoRfpFeatureReader::GetFeatureObject(FdoString* propertyName)
{
	_throwException(L"FeatureObject");
	return NULL;
}

/// <summary>Gets the geometry value of the specified property as a byte array in 
/// AGF format. Because no conversion is performed, the property must be
/// of Geometric type; otherwise, an exception is thrown. 
/// This method is a language-specific performance optimization that returns a
/// pointer to the array data, rather than to an object that encapsulates
/// the array.  The array's memory area is only guaranteed to be valid
/// until a call to ReadNext() or Close(), or the disposal of this reader
/// object.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <param name="count">Output the number of bytes in the array.</param> 
/// <returns>Returns a pointer to the byte array in AGF format.</returns> 
const FdoByte * FdoRfpFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32 * count)
{
	_throwException(L"Geometry");
	return NULL;
}
