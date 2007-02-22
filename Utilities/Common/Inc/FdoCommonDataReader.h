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
#ifndef FDOCOMMONDATAREADER_H
#define FDOCOMMONDATAREADER_H

#include <utility>
#include <FdoCommonPropertyIndex.h>
#include <FdoCommonBinaryReader.h>



typedef enum FdoCommonExpressionType
{
    FdoCommonExpressionType_Constant     = 1,  // one value per row, same for all rows  (e.g. "1+2")
    FdoCommonExpressionType_PerRow       = 2,  // one value per row, can change per row  (e.g. "A+B")
    FdoCommonExpressionType_Aggregate    = 3,  // one value per group of rows  (e.g. "SUM(A+B)")
};


class my_hash_compare
{	// traits class for hash containers
private:
    std::less< FdoByteArray* > comp;	// the comparator object

public:
	enum
		{	// parameters for hash table
		bucket_size = 4,	// 0 < bucket_size
		min_buckets = 8};	// min_buckets = 2 ^^ N, 0 < N

    my_hash_compare()
		: comp()
	{	// construct with default comparator
	}

	my_hash_compare(std::less< FdoByteArray* > _Pred)
		: comp(_Pred)
	{	// construct with _Pred comparator
	}

	bool operator()(FdoByteArray * const _Keyval1, FdoByteArray * const _Keyval2) const;

	size_t operator()(FdoByteArray * const key) const;

	size_t hashvalue(FdoByteArray * const key) const;
};


#ifdef _WIN32
typedef stdext::hash_map<FdoByteArray*, FdoByteArray*, my_hash_compare >  HASHMAP;
#else // _WIN32
typedef stdext::hash_map<FdoByteArray*, FdoByteArray*, my_hash_compare, my_hash_compare >  HASHMAP;
#endif // _WIN32
typedef std::pair<FdoByteArray*, FdoByteArray*>                           HASHMAP_PAIR;
typedef HASHMAP::iterator                                 HASHMAP_ITER;


class FdoCommonDataReader;

struct orderby_context
{
public:
    orderby_context(FdoCommonDataReader* theParentReader, FdoByteArray* theRowData)
    {
        parentReader = theParentReader;
        rowData      = theRowData;
    }

    FdoCommonDataReader*  parentReader;
    FdoByteArray*         rowData;
};




//A generic data reader, based on the FdoCommonPropertyIndex and
//FdoCommonBinaryReader helper classes. The FdoCommonPropertyIndex is used to obtain
//information about a property and the FdoCommonBinaryReader is used to get the
//value of a property.
class FdoCommonDataReader : public FdoIDataReader
{
public:

    FdoCommonDataReader(FdoIConnection* conn, FdoISelect *selectCmd, FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds, bool bDistinct, FdoIdentifierCollection* orderingIds, FdoOrderingOption eOrderingOption);

protected:

    virtual void Dispose();
    FdoCommonDataReader() {};  // dummy 0-arg constructor to please FdoPtr::operator->
    virtual ~FdoCommonDataReader();
    virtual FdoCommonBinaryReader* perform_checks (
        FdoDataType* types, FdoInt16 type_count, FdoString* property_name, int* len = NULL);

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
    virtual FdoInt32 GetPropertyCount();

    /// \brief
    /// Gets the name of the property at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the property.
    /// 
    /// \return
    /// Returns the property name
    /// 
    virtual FdoString* GetPropertyName(FdoInt32 index);

    /// \brief
    /// Gets the data type of the property with the specified name.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the type of the property.
    /// 
    virtual FdoDataType GetDataType(FdoString* propertyName);

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
    virtual FdoPropertyType GetPropertyType(FdoString* propertyName);

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
    virtual bool GetBoolean(FdoString* propertyName);

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
    virtual FdoByte GetByte(FdoString* propertyName);

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
    virtual FdoDateTime GetDateTime(FdoString* propertyName);

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
    virtual double GetDouble(FdoString* propertyName);

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
    virtual FdoInt16 GetInt16(FdoString* propertyName);

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
    virtual FdoInt32 GetInt32(FdoString* propertyName);

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
    virtual FdoInt64 GetInt64(FdoString* propertyName);

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
    virtual float GetSingle(FdoString* propertyName);

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
    virtual FdoString* GetString(FdoString* propertyName);

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
    virtual FdoLOBValue* GetLOB(FdoString* propertyName);

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
    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName );

    /// \brief
    /// Returns true if the value of the specified property is null.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    virtual bool IsNull(FdoString* propertyName);

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
    virtual FdoByteArray* GetGeometry(FdoString* propertyName);

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
    virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
    virtual bool ReadNext();

    /// \brief
    /// Closes the FdoIDataReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual void Close();

    //------------------------------------------------------------
    /// FdoCommonDataReader
    //------------------------------------------------------------

    virtual FdoCommonBinaryReader* GetBinaryReader();
    FdoCommonPropertyIndex* GetPropertyIndex();

    static FdoArray<FdoFunction*>* GetAggregateFunctions(FdoIdentifierCollection* selectedIds, FdoCommonExpressionType &exprType);
    static FdoArray<FdoFunction*>* GetAggregateFunctions(FdoExpression* expr, FdoCommonExpressionType &exprType);

    FdoClassDefinition* GetAggregateClassDef(FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds);
    void RunAggregateQuery(FdoISelect* selectCmd, FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds, FdoClassDefinition* aggrClassDef, FdoArray<FdoFunction*>* aggrFunctions);
    void PerformDistinct();
    void PerformOrderBy();

    /// Retrieve ordering information:
    virtual FdoIdentifierCollection* GetOrdering();
    virtual FdoOrderingOption GetOrderingOption();
    FdoCommonBinaryReader* GetOrderingBinReader1();
    FdoCommonBinaryReader* GetOrderingBinReader2();


protected:

    FdoPtr<FdoCommonPropertyIndex> m_propIndex;
    std::vector<void*>             m_results;
    FdoInt32                       m_resultsIndex;
    FdoPtr<FdoCommonBinaryReader>  m_binReader;
    FdoPtr<FdoIConnection>         m_connection;

    /// Orderby information:
    FdoPtr<FdoIdentifierCollection> m_orderbyIds;
    FdoOrderingOption               m_orderbyOption;
    FdoPtr<FdoCommonBinaryReader>   m_orderbyBinReader1;
    FdoPtr<FdoCommonBinaryReader>   m_orderbyBinReader2;

};

#endif  // FDOCOMMONDATAREADER_H


