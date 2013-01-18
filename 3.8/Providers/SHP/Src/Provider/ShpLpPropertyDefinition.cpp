/*
 * 
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
#include <ShpLpPropertyDefinition.h>
#include <ShpSchemaUtilities.h>
#include <wctype.h>


ShpLpPropertyDefinition::ShpLpPropertyDefinition(ShpLpClassDefinition *parentLpClass, int iPhysicalColumnIndex, FdoPropertyDefinition *configLogicalProperty, FdoShpOvPropertyDefinition* configPropertyMapping, bool bPhysicalToLogical)
:
    m_physicalColumnIndex(iPhysicalColumnIndex),
    m_parentLpClass(parentLpClass)  // weak reference
{
    if (bPhysicalToLogical)
        ConvertPhysicalToLogical(configLogicalProperty, configPropertyMapping);
    else
        ConvertLogicalToPhysical(iPhysicalColumnIndex, configLogicalProperty, configPropertyMapping);

    // Add this Lp property to its parent LpClass:
    FdoPtr<ShpLpPropertyDefinitionCollection> lpProperties = m_parentLpClass->GetLpProperties();
    if (!lpProperties->Contains (GetName ()))
        lpProperties->Add(this);
}

ShpLpPropertyDefinition::~ShpLpPropertyDefinition(void)
{
}


// Use the (Physical) column definition to populate the Logical property definition.
void ShpLpPropertyDefinition::ConvertPhysicalToLogical(FdoPropertyDefinition* configLogicalProperty, FdoShpOvPropertyDefinition* propertyMapping)
{
    ColumnInfo* info = m_parentLpClass->GetPhysicalColumnInfo ();

    // Determine logical FDO property name:
    FdoString * logicalPropertyName = L"";
    if (NULL != propertyMapping)
        logicalPropertyName = propertyMapping->GetName();
    else
        logicalPropertyName = info->GetColumnNameAt (m_physicalColumnIndex);

    // determine logical FDO property description
    FdoString * logicalPropertyDescription = L"";
    if (NULL != configLogicalProperty)
        logicalPropertyDescription = configLogicalProperty->GetDescription ();

    // Create logical property:
    // Note: physical columns are always plain data properties; identity and geometry
    //       columns are handled specially in the parent ShpLpClassDefinition.
    m_logicalProperty = FdoDataPropertyDefinition::Create(logicalPropertyName, logicalPropertyDescription);

    // Set misc stuff:
	eDBFColumnType colType = info->GetColumnTypeAt(m_physicalColumnIndex);
	int colWidth = info->GetColumnWidthAt(m_physicalColumnIndex);
    int colScale = info->GetColumnScaleAt(m_physicalColumnIndex);

    FdoDataType data_type = ShpSchemaUtilities::DbfTypeToFdoType(colType, colWidth, colScale);
    m_logicalProperty->SetDataType(data_type);
    m_logicalProperty->SetReadOnly(false);
    m_logicalProperty->SetNullable(true);
    m_logicalProperty->SetIsAutoGenerated(false);

    // Set length (Only applies to String/Blob/Clob data properties):
    if (data_type == FdoDataType_String)
        m_logicalProperty->SetLength(info->GetColumnWidthAt(m_physicalColumnIndex));

    // Set precision/scale (Only applies to Decimal properties or original Decimal properties converted
	// to Integer types - see RFC 55)
    if (data_type == FdoDataType_Decimal || colType == kColumnDecimalType)
    {
        m_logicalProperty->SetPrecision(colWidth);
        m_logicalProperty->SetScale(colScale);
    }

    // Add the logical property to its parent:
    FdoPtr<FdoClassDefinition> logicalClass = m_parentLpClass->GetLogicalClass();
    FdoPtr<FdoPropertyDefinitionCollection> logicalProperties = logicalClass->GetProperties();

	// Check for duplicates (DBF may contain truncated column names)
	if (logicalProperties->Contains (m_logicalProperty->GetName ())) 
		throw FdoException::Create(NlsMsgGet(SHP_DUPLICATE_COLUMN, "Duplicate field name '%1$ls' in '%2$ls'.", 
					GetPhysicalColumnName(), m_parentLpClass->GetPhysicalFileSet()->GetDbfFile()->FileName()));

	logicalProperties->Add(m_logicalProperty);
}


void ShpLpPropertyDefinition::ConvertLogicalToPhysical (int physicalColumnIndex, FdoPropertyDefinition* logicalProperty, FdoShpOvPropertyDefinition* configPropertyMapping)
{
    ColumnInfo* info;
    std::wstring physicalColumnName;
    FdoDataType logicalPropertyType;
    eDBFColumnType physicalColumnType;

    info = m_parentLpClass->GetPhysicalColumnInfo ();
    // Hang on to the logical property:
    if ((logicalProperty != NULL) && (logicalProperty->GetPropertyType() != FdoPropertyType_DataProperty))
        throw FdoException::Create(NlsMsgGet(SHP_UNEXPECTED_ERROR, "Unexpected error '%1$ls'.", L"logicalProperty not FdoPropertyType_DataProperty"));
    m_logicalProperty = static_cast<FdoDataPropertyDefinition*>(FDO_SAFE_ADDREF(logicalProperty));

    // Convert logical property name to physical column name, and validate it:
    if (configPropertyMapping != NULL)
    {
        FdoPtr<FdoShpOvColumnDefinition> configColumn = configPropertyMapping->GetColumn();
        physicalColumnName = configColumn->GetName();
    }
    else
    {
        physicalColumnName = logicalProperty->GetName ();
    }

    char *mbPhysicalColumnName;

    // check for invalid characters
    for(size_t idx = 0; idx < physicalColumnName.size(); idx++)
    {
        wchar_t ch = physicalColumnName.at(idx);
        if ( iswcntrl(ch) || 
            (iswspace(ch) && (ch != L' ')) || 
            (iswpunct(ch) && (ch == L':')) || 
            (iswpunct(ch) && (ch == L'.')))
        {
            physicalColumnName[idx] = L'_';
        }
    }

    // Test in advance for the field length (multibyte). 
    // Get the codepage directly from locale. The CPG file is not created yet. 
    ShapeCPG    *cpg = new ShapeCPG(); // This constructor doesn't create the file
    FdoStringP  codepage = cpg->GetCodePage();
#ifdef _WIN32
    ULONG realCodepage = cpg->ConvertCodePageWin((WCHAR *)(FdoString *)codepage);
#else
    const char* realCodepage = cpg->ConvertCodePageLinux((WCHAR *)(FdoString *)codepage);
#endif

    wide_to_multibyte_cpg (mbPhysicalColumnName, physicalColumnName.c_str(), realCodepage);

    // Make sure the name is not exceeding the maximum size in which case it will be truncated.
    size_t lenPhyCol = physicalColumnName.size();
    // we start with a max 11 Unicode characters since we must fit in the name in 11 bytes
    lenPhyCol = lenPhyCol > nDBF_COLNAME_LENGTH ? nDBF_COLNAME_LENGTH : lenPhyCol;
    std::wstring  trimmed = physicalColumnName;
    size_t propLen = strlen (mbPhysicalColumnName);
    const WCHAR** pNames = info->GetColumnNames();
    int cntCols = info->GetNumColumns();

    bool colNameIsUnique = true;
    // see if the column is unique
    if (nDBF_COLNAME_LENGTH < propLen)
    {
        for (int y = 0; y < cntCols && colNameIsUnique; y++)
            colNameIsUnique = !(trimmed == pNames[y]);
    }
    //the index used as suffix for truncated columns
    int i = 0;
    while (nDBF_COLNAME_LENGTH < propLen || !colNameIsUnique)
    {
		// Truncate the name and tack on a unique number.
        if (i == 0)
            trimmed = physicalColumnName.substr(0, lenPhyCol);
        else
            trimmed = (FdoString*)FdoStringP::Format( L"%ls%d", physicalColumnName.substr(0, lenPhyCol - ((int) log10((double)i))).c_str(), i);

        wide_to_multibyte_cpg (mbPhysicalColumnName, trimmed.c_str(), realCodepage);
        lenPhyCol--;
        propLen = strlen (mbPhysicalColumnName);
        if (nDBF_COLNAME_LENGTH >= propLen)
        {
            colNameIsUnique = true;
            // see if the column is unique
            for (int y = 0; y < cntCols && colNameIsUnique; y++)
                colNameIsUnique = !(trimmed == pNames[y]);
            // keep trim at the same count but increment index name
            if (!colNameIsUnique)
            {
                lenPhyCol++;
                i++;
            }
        }
    }

    delete cpg;

    // Assign the new name
    info->SetColumnName (physicalColumnIndex, (WCHAR*)trimmed.c_str());

    // Convert logical data type to physical column type, and validate it:
    logicalPropertyType = m_logicalProperty->GetDataType ();

	// Reset the precision to distinguish between IntXX types and IntXX types from Decimals
	if (logicalPropertyType != FdoDataType_Decimal)
	{
		int colWidth = m_logicalProperty->GetPrecision();

		if (colWidth == DEFAULT_INT16_COL_LENGTH || colWidth == DEFAULT_INT32_COL_LENGTH || colWidth == DEFAULT_INT64_COL_LENGTH )
		{
			m_logicalProperty->SetPrecision(0);
		}
	}

    physicalColumnType = ShpSchemaUtilities::FdoTypeToDbfType (logicalPropertyType);
    if (kColumnUnsupportedType == physicalColumnType)
        throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", FdoCommonMiscUtil::FdoDataTypeToString (logicalPropertyType)));
    info->SetColumnType (physicalColumnIndex, physicalColumnType);
    switch (physicalColumnType)
    {
        case kColumnDecimalType:
            // NOTE: the logicalProperty could be either an int32 or a decimal:
            if (logicalPropertyType==FdoDataType_Decimal)
            {
                info->SetColumnScale (physicalColumnIndex, m_logicalProperty->GetScale());
                info->SetColumnWidth (physicalColumnIndex, m_logicalProperty->GetPrecision());
            }
            else  // logicalPropertyType==FdoDataType_Int16, Int32 or Int64
            {
                info->SetColumnScale (physicalColumnIndex, 0);
				int colWidth = m_logicalProperty->GetPrecision();

				if (colWidth == 0)
				{
					// If not set then use defaults
					if (logicalPropertyType == FdoDataType_Int16)
						colWidth = DEFAULT_INT16_COL_LENGTH;
					else if (logicalPropertyType == FdoDataType_Int32)
						colWidth = DEFAULT_INT32_COL_LENGTH;
					else if (logicalPropertyType == FdoDataType_Int64)
						colWidth = DEFAULT_INT64_COL_LENGTH;
				}
                info->SetColumnWidth (physicalColumnIndex, colWidth);
            }
            break;
        case kColumnCharType:
            info->SetColumnWidth (physicalColumnIndex, ((FdoDataPropertyDefinition*)logicalProperty)->GetLength ());
            break;
        case kColumnDateType:
            info->SetColumnWidth (physicalColumnIndex, 8);
            break;
        case kColumnLogicalType:
            info->SetColumnWidth (physicalColumnIndex, 1);
            break;
    }


    // Add the physical column to its parent:
    // NOTE: the physical column cannot exist separately from physicalColumns,
    //       therefore it has already been added to physicalColumns.
}


// Returns the logical property name:
FdoString* ShpLpPropertyDefinition::GetName(void)
{
    return m_logicalProperty == NULL ? L"" : m_logicalProperty->GetName();
}

// Returns the physical column name:
FdoString* ShpLpPropertyDefinition::GetPhysicalColumnName(void)
{
    return m_parentLpClass->GetPhysicalColumnInfo ()->GetColumnNameAt (m_physicalColumnIndex);
}


// Generate a new copy of the schema mappings for this property, optionally including default mappings
FdoShpOvPropertyDefinition* ShpLpPropertyDefinition::GetSchemaMappings(bool bIncludeDefaults)
{
    FdoPtr<FdoShpOvPropertyDefinition> propertyMapping = FdoShpOvPropertyDefinition::Create();
    bool bHasMappings = false;

    // Set logical name:
    propertyMapping->SetName( GetName() );

    // Set column name:
    if (bIncludeDefaults || ((GetPhysicalColumnName() != NULL) && (0!=wcscmp(GetPhysicalColumnName(), GetName()))))
    {
        bHasMappings = true;

        FdoPtr<FdoShpOvColumnDefinition> colDef = FdoShpOvColumnDefinition::Create();
        colDef->SetName(GetPhysicalColumnName());
        propertyMapping->SetColumn(colDef);
    }

    return bHasMappings ? FDO_SAFE_ADDREF(propertyMapping.p) : NULL;
}

