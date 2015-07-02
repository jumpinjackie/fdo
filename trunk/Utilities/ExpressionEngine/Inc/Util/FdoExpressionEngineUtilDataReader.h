// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
#ifndef FdoExpressionEngineUtilDataReader_H
#define FdoExpressionEngineUtilDataReader_H

class FdoCommonPropertyIndex;
class FdoCommonBinaryReader;

#include <utility>
#include "../ExpressionEngine.h"


typedef enum FdoCommonExpressionType
{
    FdoCommonExpressionType_Constant     = 1,  // one value per row, same for all rows  (e.g. "1+2")
    FdoCommonExpressionType_PerRow       = 2,  // one value per row, can change per row  (e.g. "A+B")
    FdoCommonExpressionType_Aggregate    = 3,  // one value per group of rows  (e.g. "SUM(A+B)")
};


class FdoExpressionEngineUtilDataReader;
class FdoResultsStack;

struct orderby_context
{
public:
    orderby_context(FdoExpressionEngineUtilDataReader* theParentReader, FdoByteArray* theRowData)
    {
        parentReader = theParentReader;
        rowData      = theRowData;
    }

    FdoExpressionEngineUtilDataReader*  parentReader;
    FdoByteArray*         rowData;
};


//A generic data reader, based on the FdoCommonPropertyIndex and
//FdoCommonBinaryReader helper classes. The FdoCommonPropertyIndex is used to obtain
//information about a property and the FdoCommonBinaryReader is used to get the
//value of a property.
class FdoExpressionEngineUtilDataReader : public FdoIDataReader
{
public:

    EXPRESSIONENGINE_API static FdoExpressionEngineUtilDataReader* Create (FdoFunctionDefinitionCollection *functions, 
                                                           FdoIFeatureReader* reader, 
                                                           FdoClassDefinition* originalClassDef, 
                                                           FdoIdentifierCollection* selectedIds, 
                                                           bool bDistinct, 
                                                           FdoIdentifierCollection* orderingIds, 
                                                           FdoOrderingOption eOrderingOption, 
                                                           FdoIdentifierCollection* ids, 
                                                           FdoPtr <FdoArray<FdoFunction*> > aggrIdents );

    EXPRESSIONENGINE_API FdoExpressionEngineUtilDataReader(FdoFunctionDefinitionCollection *functions, 
                                                           FdoIFeatureReader* reader, 
                                                           FdoClassDefinition* originalClassDef, 
                                                           FdoIdentifierCollection* selectedIds, 
                                                           bool bDistinct, 
                                                           FdoIdentifierCollection* orderingIds, 
                                                           FdoOrderingOption eOrderingOption, 
                                                           FdoIdentifierCollection* ids, 
                                                           FdoPtr <FdoArray<FdoFunction*> > aggrIdents );
protected:

    virtual void Dispose();
    FdoExpressionEngineUtilDataReader() {};  // dummy 0-arg constructor to please FdoPtr::operator->
    virtual ~FdoExpressionEngineUtilDataReader();
    virtual FdoCommonBinaryReader* perform_checks (FdoDataType* types, 
                                                   FdoInt16 type_count, 
                                                   FdoString* property_name, 
                                                   int* len = NULL);

public:

    //------------------------------------------------------------
    /// FdoIDataReader implementation
    //------------------------------------------------------------

    /// \brief
    /// Gets the number of propertys in the result set.
    /// 
    /// \return
    /// Returns the number of propertys.
    /// 
    EXPRESSIONENGINE_API virtual FdoInt32 GetPropertyCount();

    /// \brief
    /// Gets the name of the property at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the property.
    /// 
    /// \return
    /// Returns the property name
    /// 
    EXPRESSIONENGINE_API virtual FdoString* GetPropertyName(FdoInt32 index);

    /// \brief
    /// Gets the index of the property with the specified name.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the property index
    /// 
    EXPRESSIONENGINE_API virtual FdoInt32 GetPropertyIndex(FdoString* propertyName);

    /// \brief
    /// Gets the data type of the property with the specified name.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the type of the property.
    /// 
    EXPRESSIONENGINE_API virtual FdoDataType GetDataType(FdoString* propertyName);

    /// \brief
    /// Gets the FDO property type of a given property. This is used
    ///  to indicate if a given property is a geometric property or a data property. If the property is
    ///  a FdoPropertyType_Data, then GetDataType can be used to to find the data type of the property.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FDO property type.
    /// 
    EXPRESSIONENGINE_API virtual FdoPropertyType GetPropertyType(FdoString* propertyName);

    /// \brief
    /// Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the Boolean value
    /// 
    EXPRESSIONENGINE_API virtual bool GetBoolean(FdoString* propertyName);

    /// \brief
    /// Gets the byte value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Byte or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    EXPRESSIONENGINE_API virtual FdoByte GetByte(FdoString* propertyName);

    /// \brief
    /// Gets the date time value of the specified property. No conversion
    /// is performed, thus the property must be FdoDataType_DateTime or
    /// an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    EXPRESSIONENGINE_API virtual FdoDateTime GetDateTime(FdoString* propertyName);

    /// \brief
    /// Gets the double-precision floating point value of the specified property.
    /// No conversion is performed, thus the property must be of type
    /// Double or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the double value.
    /// 
    EXPRESSIONENGINE_API virtual double GetDouble(FdoString* propertyName);

    /// \brief
    /// Gets the signed 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    EXPRESSIONENGINE_API virtual FdoInt16 GetInt16(FdoString* propertyName);

    /// \brief
    /// Gets the signed 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt32 value.
    /// 
    EXPRESSIONENGINE_API virtual FdoInt32 GetInt32(FdoString* propertyName);

    /// \brief
    /// Gets the signed 64-bit integer value of the specified property. No conversion
    /// is performed, thus the property must be FdoDataType_Int64 or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    EXPRESSIONENGINE_API virtual FdoInt64 GetInt64(FdoString* propertyName);

    /// \brief
    /// Gets the single-precision floating point value of the specified property.
    /// No conversion is performed, thus the property must be FdoDataType_Single
    /// or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the single value
    /// 
    EXPRESSIONENGINE_API virtual float GetSingle(FdoString* propertyName);

    /// \brief
    /// Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the string value.
    /// 
    EXPRESSIONENGINE_API virtual FdoString* GetString(FdoString* propertyName);

    /// \brief
    /// Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
    EXPRESSIONENGINE_API virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    /// \brief
    /// Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
    EXPRESSIONENGINE_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

    /// \brief
    /// Returns true if the value of the specified property is null.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    EXPRESSIONENGINE_API virtual bool IsNull(FdoString* propertyName);

    /// \brief
    /// Gets the geometry value of the specified property as a byte array
    /// in FGF format. No conversion is performed, thus the property
    /// must be of Geometric type or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FGF byte array value.
    /// 
    EXPRESSIONENGINE_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    /// \brief
    /// Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the raster object.
    /// 
    EXPRESSIONENGINE_API virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// \brief
    /// Gets the data type of the property at the specified index position.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the type of the property.
    /// 
    EXPRESSIONENGINE_API virtual FdoDataType GetDataType(FdoInt32 index);

    /// \brief
    /// Gets the FDO property type of the property at the given index. This is used
    /// to indicate if a given property is a geometric property or a data property. 
    /// If the property is a FdoPropertyType_DataProperty, then GetDataType 
    /// can be used to to find the data type of the property.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FDO property type.
    /// 
    EXPRESSIONENGINE_API virtual FdoPropertyType GetPropertyType(FdoInt32 index);

    /// \brief
    /// Returns true if the value of the property at the specified 
    /// index is null.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    EXPRESSIONENGINE_API virtual FdoBoolean IsNull(FdoInt32 index);

    /// \brief
    /// Gets the Boolean value of the property specified at the index position. 
    /// No conversion is performed, thus the property must be FdoDataType_Boolean 
    /// or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the Boolean value.
    /// 
    EXPRESSIONENGINE_API virtual FdoBoolean GetBoolean(FdoInt32 index);

    /// \brief
    /// Gets the Byte value of the property specified at the index position. 
    /// No conversion is performed, thus the property must be FdoDataType_Byte 
    /// or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    EXPRESSIONENGINE_API virtual FdoByte GetByte(FdoInt32 index);

    /// \brief
    /// Gets the date and time value of the of the property specified at 
    /// the index position. No conversion is performed, thus the property 
    /// must be FdoDataType_DateTime or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    EXPRESSIONENGINE_API virtual FdoDateTime GetDateTime(FdoInt32 index);

    /// \brief
    /// Gets the double-precision floating point value of the property specified at 
    /// the index position. No conversion is performed, thus the property must be 
    /// FdoDataType_Double or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the double floating point value
    /// 
    EXPRESSIONENGINE_API virtual FdoDouble GetDouble(FdoInt32 index);

    /// \brief
    /// Gets the 16-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int16 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    EXPRESSIONENGINE_API virtual FdoInt16 GetInt16(FdoInt32 index);

    /// \brief
    /// Gets the 32-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int32 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt32 value
    /// 
    EXPRESSIONENGINE_API virtual FdoInt32 GetInt32(FdoInt32 index);

    /// \brief
    /// Gets the 64-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int64 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    EXPRESSIONENGINE_API virtual FdoInt64 GetInt64(FdoInt32 index);

    /// \brief
    /// Gets the Single floating point value of the property specified at 
    /// the index position. No conversion is performed, thus the property 
    /// must be FdoDataType_Single or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the single value
    /// 
    EXPRESSIONENGINE_API virtual FdoFloat GetSingle(FdoInt32 index);

    /// \brief
    /// Gets the string value of the property specified at the index
    /// position. No conversion is performed, thus the property must
    /// be FdoDataType_String or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the string value
    /// 
    EXPRESSIONENGINE_API virtual FdoString* GetString(FdoInt32 index);

    /// \brief
    /// Gets a LOBValue reference to the property specified at the index
    /// position. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be 
    /// FdoDataType_BLOB or FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
    EXPRESSIONENGINE_API virtual FdoLOBValue* GetLOB(FdoInt32 index);

    /// \brief
    /// Gets a reference to the specified LOB property, specified at the index
    /// position. The reference is returned as an FdoBLOBStreamReader or an 
    /// FdoCLOBStreamReader, to allow reading in blocks of data. Because 
    /// no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type) Cast the FdoIStreamReader 
    /// to the appropiate LOB Stream Reader.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
    EXPRESSIONENGINE_API virtual FdoIStreamReader* GetLOBStreamReader(FdoInt32 index);

    /// \brief
    /// Gets the geometry value of the property, at the specified index, as  
    /// a byte array in FGF format. Because no conversion is performed, the 
    /// property must be of Geometric type; otherwise, an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the byte array in FGF format.
    /// 
    EXPRESSIONENGINE_API virtual FdoByteArray* GetGeometry(FdoInt32 index);

    /// \brief
    /// Gets the raster object of the property at the specified index.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the raster object.
    /// 
    EXPRESSIONENGINE_API virtual FdoIRaster* GetRaster(FdoInt32 index);

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
    EXPRESSIONENGINE_API virtual bool ReadNext();

    /// \brief
    /// Closes the FdoIDataReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
    EXPRESSIONENGINE_API virtual void Close();

    //------------------------------------------------------------
    /// FdoExpressionEngineUtilDataReader
    //------------------------------------------------------------

    EXPRESSIONENGINE_API virtual FdoCommonBinaryReader* GetBinaryReader();
    EXPRESSIONENGINE_API FdoCommonPropertyIndex* GetPropertyIndex();

    EXPRESSIONENGINE_API static FdoArray<FdoFunction*>* GetAggregateFunctions(FdoFunctionDefinitionCollection *funcDefs, FdoIdentifierCollection* selectedIds, FdoCommonExpressionType &exprType);
    EXPRESSIONENGINE_API static FdoArray<FdoFunction*>* GetAggregateFunctions(FdoFunctionDefinitionCollection *funcDefs, FdoExpression* expr, FdoCommonExpressionType &exprType);

    EXPRESSIONENGINE_API FdoClassDefinition* GetAggregateClassDef(FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds);
    EXPRESSIONENGINE_API void RunAggregateQuery(FdoIFeatureReader* reader, FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds, FdoClassDefinition* aggrClassDef, FdoArray<FdoFunction*>* aggrFunctions);
    EXPRESSIONENGINE_API void PerformDistinct();
    EXPRESSIONENGINE_API void PerformOrderBy();

    /// Retrieve ordering information:
    EXPRESSIONENGINE_API virtual FdoIdentifierCollection* GetOrdering();
    EXPRESSIONENGINE_API virtual FdoOrderingOption GetOrderingOption();
    EXPRESSIONENGINE_API FdoCommonBinaryReader* GetOrderingBinReader1();
    EXPRESSIONENGINE_API FdoCommonBinaryReader* GetOrderingBinReader2();


protected:

    FdoResultsStack*               m_resultsStack;
    FdoCommonPropertyIndex*        m_propIndex;
    FdoInt32                       m_resultsIndex;
    FdoCommonBinaryReader*         m_binReader;
    FdoPtr<FdoFunctionDefinitionCollection>         m_functions;

    /// Orderby information:
    FdoPtr<FdoIdentifierCollection> m_orderbyIds;
    FdoOrderingOption               m_orderbyOption;
    FdoCommonBinaryReader*          m_orderbyBinReader1;
    FdoCommonBinaryReader*          m_orderbyBinReader2;
    FdoPtr<FdoClassDefinition>      m_usedClass;

};

#endif  // FdoExpressionEngineUtilDataReader_H


