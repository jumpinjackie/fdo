// 
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
//  
#include "stdafx.h"
#include <Util/FdoExpressionEngineUtilDataReader.h>
#include <FdoCommonBinaryReader.h>
#include <FdoCommonBinaryWriter.h>
#include <FdoCommonPropertyIndex.h>
#include <FdoExpressionEngine.h>
#include <FdoCommonMiscUtil.h>
#include <algorithm>

// hash _Keyval to size_t value
size_t my_hash_compare::operator()(FdoByteArray * const key) const
{
    return my_hash_compare::hashvalue(key);
}


size_t my_hash_compare::hashvalue(FdoByteArray * const key) const
{
    FdoInt32 count = key->GetCount();
    const FdoByte* keyData = key->GetData();
    size_t  hashvalue = 0;
    for (FdoInt32 i=0; i<count; i++)
    {
        hashvalue = (keyData[i] + hashvalue) % INT_MAX;
    }

    return hashvalue;
}


// Note that Visual Studio and GNU behavior of hash_map differs for this operator:
//    Visual Studio's hash_map expects "LESS THAN" behavior
//    GNU's hash_map expects "EQUALS" behavior
bool my_hash_compare::operator()(FdoByteArray * const bytearray1, FdoByteArray * const bytearray2) const
{
    if (bytearray1->GetCount() < bytearray2->GetCount())
#ifdef _WIN32
        return true;  // Visual Studio hash_map expects "LESS THAN" behavior
#else // _WIN32
        return false; // GNU hash_map expects "EQUALS" behavior
#endif
    else if (bytearray1->GetCount() > bytearray2->GetCount())
        return false;
    else // bytearray1->GetCount() == bytearray2->GetCount()
    {
        FdoInt32 count = bytearray1->GetCount();
	// Initialize bRetVal to default value if both arrays have exactly the same values:
#ifdef _WIN32
        bool bRetVal = false; // Visual Studio hash_map expects "LESS THAN" behavior
#else // _ WIN32
        bool bRetVal = true;  // GNU hash_map expects "EQUALS" behavior
#endif
        for (FdoInt32 i=0; i<count; i++)
        {
            FdoByte byte1 = bytearray1->operator[](i);
            FdoByte byte2 = bytearray2->operator[](i);
            if (byte1 < byte2)
            {
#ifdef _WIN32
	        bRetVal = true;  // Visual Studio hash_map expects "LESS THAN" behavior
#else // _WIN32
                bRetVal = false; // GNU hash_map expects "EQUALS" behavior
#endif
                break;
            }
            else if (byte1 > byte2)
            {
                bRetVal = false;
                break;
            }
        }

        return bRetVal;
    }
}




FdoExpressionEngineUtilDataReader::FdoExpressionEngineUtilDataReader(FdoFunctionDefinitionCollection *functions, FdoIFeatureReader* reader, FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds, bool bDistinct, FdoIdentifierCollection* orderingIds, FdoOrderingOption eOrderingOption, FdoIdentifierCollection* ids, FdoPtr<FdoArray<FdoFunction*> > aggrIdents )
{
    m_results.clear();
    m_resultsIndex = -1;
    m_orderbyIds = FDO_SAFE_ADDREF(orderingIds);
    m_orderbyOption = eOrderingOption;
    m_binReader = new FdoCommonBinaryReader(NULL, 0);
    m_functions = FDO_SAFE_ADDREF(functions);

    if ((aggrIdents != NULL) && (aggrIdents->GetCount() > 0))
    {
        //get the 'aggregate' class definition. It should only contain
        //properties which are relevant to the select aggregates.
        //In our subset this means computed identifiers containing
        //functions like min, max, sum, avg, count.
        FdoPtr<FdoClassDefinition> aggrClass = GetAggregateClassDef(originalClassDef, selectedIds);
        m_propIndex = new FdoCommonPropertyIndex(aggrClass, 0);

        //compute aggregate values
        RunAggregateQuery(reader, originalClassDef, selectedIds, aggrClass, aggrIdents);
    }
    else
    {

        //get the updated class definition. It should only contain
        //properties which are relevant to the select distinct
        FdoPtr<FdoClassDefinition> readerClass = reader->GetClassDefinition();

        //Now create a temporary FdoCommonPropertyIndex structure corresponding to that class.
        //Normally we do not use temporary FdoCommonPropertyIndex structures since they are
        //used for serialization of real properties (not computed) but in this case we are going
        //to be serializing computed properties in the process of determining distinct so it's ok.
        //Use 0 as feature class ID, since this is not a fully defined class definition.
        m_propIndex = new FdoCommonPropertyIndex(readerClass, 0, ids);

        // Dump the results into m_results:
        FdoPtr<FdoCommonBinaryWriter> wrt = new FdoCommonBinaryWriter(256);
        while (reader->ReadNext())
        {
            // Serialize the row:
            wrt->Reset();
            wrt->WritePropertyValues(readerClass, m_propIndex, reader);

            // Convert the binary writer to FdoByteArray:
            int dataLen = wrt->GetDataLen();
            unsigned char* data = wrt->GetData(true);
            FdoPtr<FdoByteArray> bytes = FdoByteArray::Create( (FdoByte*)data, dataLen);
            delete[] data;  //TODO: avoid copying bytes to FdoByteArray

            // Insert FdoByteArray into m_results:
            m_results.push_back(FDO_SAFE_ADDREF(bytes.p));
        }
    }


    ///////////////////////////////////////////////////////////////////////////
    // Compute distinct rows (if requested):
    ///////////////////////////////////////////////////////////////////////////

    if (bDistinct)
    {
        //find distinct values
        PerformDistinct();
    }


    ///////////////////////////////////////////////////////////////////////////
    // Order the results (if requested):
    ///////////////////////////////////////////////////////////////////////////

    // Apply OrderBy logic (if required):
    if ((orderingIds != NULL) && (orderingIds->GetCount() > 0))
    {
        PerformOrderBy();
    }
}


FdoExpressionEngineUtilDataReader::~FdoExpressionEngineUtilDataReader()
{
    Close();
}

void FdoExpressionEngineUtilDataReader::Dispose()
{
    delete this;
}

FdoInt32 FdoExpressionEngineUtilDataReader::GetPropertyCount()
{
    return m_propIndex->GetNumProps();
}

FdoString* FdoExpressionEngineUtilDataReader::GetPropertyName(FdoInt32 index)
{
    FdoCommonPropertyStub* ps = m_propIndex->GetPropInfo(index);
   
    return ps->m_name;
}

FdoDataType FdoExpressionEngineUtilDataReader::GetDataType(FdoString* propertyName)
{
    FdoCommonPropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    _ASSERT (ps->m_dataType != FdoDataType(-1));

    return ps->m_dataType;         
}

FdoPropertyType FdoExpressionEngineUtilDataReader::GetPropertyType(FdoString* propertyName)
{
    (void)propertyName;  // to avoid compile-time warning of unreferenced parameter

    return (FdoPropertyType)0;         
}

FdoCommonBinaryReader* FdoExpressionEngineUtilDataReader::perform_checks (
    FdoDataType* types, FdoInt16 type_count, FdoString* property_name, int* len)
{
    FdoCommonPropertyStub* ps;
    bool ok;
    int length;
    FdoCommonBinaryReader* ret;

    ok = false;
    ps = m_propIndex->GetPropInfo (property_name);
	if (ps == NULL )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_74_PROPERTY_NAME_NOT_FOUND), property_name));

    for (int i = 0; i < type_count && !ok; i++)
        if (ps->m_dataType == types[i])
            ok = true;
    if (!ok)
        throw FdoCommandException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));
    ret = GetBinaryReader ();
    if (NULL == len)
        len = &length;
    *len = ret->PositionReader (ps->m_recordIndex, m_propIndex);
    if (0 == *len)
        throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_59_CANNOTFETCHNULLVALUE)));

    return (ret);
}

bool FdoExpressionEngineUtilDataReader::GetBoolean (FdoString* propertyName)
{
    FdoDataType types[] = { FdoDataType_Boolean };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName);
    return (0 != reader->ReadByte ());
}

FdoByte FdoExpressionEngineUtilDataReader::GetByte (FdoString* propertyName)
{
    FdoDataType types[] = { FdoDataType_Byte };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName);
    return (reader->ReadByte ());
}

FdoDateTime FdoExpressionEngineUtilDataReader::GetDateTime (FdoString* propertyName)
{
    FdoDataType types[] = { FdoDataType_DateTime };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName);
    return (reader->ReadDateTime ());
}

double FdoExpressionEngineUtilDataReader::GetDouble (FdoString* propertyName)
{
    FdoDataType types[] = { FdoDataType_Double, FdoDataType_Decimal };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName);
    return (reader->ReadDouble ());
}

FdoInt16 FdoExpressionEngineUtilDataReader::GetInt16 (FdoString* propertyName)
{
    FdoDataType types[] = { FdoDataType_Int16 };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName);
    return (reader->ReadInt16 ());
}

FdoInt32 FdoExpressionEngineUtilDataReader::GetInt32 (FdoString* propertyName)
{
    FdoDataType types[] = { FdoDataType_Int32 };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName);
    return (reader->ReadInt32 ());
}

FdoInt64 FdoExpressionEngineUtilDataReader::GetInt64 (FdoString* propertyName)
{
    FdoDataType types[] = { FdoDataType_Int64 };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName);
    return (reader->ReadInt64 ());
}

float FdoExpressionEngineUtilDataReader::GetSingle (FdoString* propertyName)
{
    FdoDataType types[] = { FdoDataType_Single, FdoDataType_Decimal };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName);
    return (reader->ReadSingle ());
}

FdoString* FdoExpressionEngineUtilDataReader::GetString (FdoString* propertyName)
{
    int len;
    FdoDataType types[] = { FdoDataType_String };
    FdoPtr<FdoCommonBinaryReader> reader = perform_checks (types, ELEMENTS (types), propertyName, &len);
    return (reader->ReadRawString (len));
}

FdoLOBValue* FdoExpressionEngineUtilDataReader::GetLOB(FdoString* propertyName)
{
    (void)propertyName;  // to avoid compile-time warning of unreferenced parameter

    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_72_LOB_DATA_TYPE_NOT_SUPPORTED)));
}

FdoIStreamReader* FdoExpressionEngineUtilDataReader::GetLOBStreamReader(const wchar_t* propertyName)
{
    (void)propertyName;  // to avoid compile-time warning of unreferenced parameter

    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_72_LOB_DATA_TYPE_NOT_SUPPORTED)));
}

bool FdoExpressionEngineUtilDataReader::IsNull(FdoString* propertyName)
{
    FdoCommonPropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    FdoPtr<FdoCommonBinaryReader> reader = GetBinaryReader();
    int len = reader->PositionReader(ps->m_recordIndex, m_propIndex);

    //if there is no data record for this propery, we will
    //say that it is NULL
    //TODO: I am not clear as to whether having a default value for 
    //this property in the schema would mean that the property is not null
    return (len == 0) && !ps->m_isAutoGen;
}

FdoByteArray* FdoExpressionEngineUtilDataReader::GetGeometry(FdoString* propertyName)
{
    FdoCommonPropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    if (ps->m_dataType != FdoDataType(-1))
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_62_PROPERTYVALUEFETCHTYPEMISMATCH)));

    FdoPtr<FdoCommonBinaryReader> reader = GetBinaryReader();
    int len = reader->PositionReader(ps->m_recordIndex, m_propIndex);

    _ASSERT(len != 0);

    void* ptr = reader->GetDataAtCurrentPosition();

    return FdoByteArray::Create((unsigned char*)ptr, len);
}

FdoIRaster* FdoExpressionEngineUtilDataReader::GetRaster(FdoString* propertyName)
{
    (void)propertyName;  // to avoid compile-time warning of unreferenced parameter

    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_70_PROPERTY_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoPropertyTypeToString(FdoPropertyType_RasterProperty)));
}

void FdoExpressionEngineUtilDataReader::Close()
{
    for (FdoInt32 i=0; i<(FdoInt32)m_results.size(); i++)
    {
        FdoByteArray *bytes = (FdoByteArray*)m_results.at(i);
        FDO_SAFE_RELEASE(bytes);
    }
    m_results.clear();
}


FdoCommonPropertyIndex* FdoExpressionEngineUtilDataReader::GetPropertyIndex()
{
    return FDO_SAFE_ADDREF(m_propIndex.p);
}


FdoArray<FdoFunction*>* FdoExpressionEngineUtilDataReader::GetAggregateFunctions(FdoFunctionDefinitionCollection *funcDefs, FdoIdentifierCollection* selectedIds, FdoCommonExpressionType &exprType)
{
    FdoArray<FdoFunction*>* aggrIdents = NULL;

    // If no identifiers have been explicitly selected, this implies a per-row expression type with no aggregate functions:
    if (selectedIds == NULL)
    {
        exprType = FdoCommonExpressionType_PerRow;
        return NULL;
    }

    bool bContainsAggregateExpressions = false;
    bool bContainsPerRowExpressions = false;
    for (FdoInt32 i=0; i<selectedIds->GetCount(); i++)
    {
        FdoPtr<FdoIdentifier> selectedId = selectedIds->GetItem(i);
        FdoArray<FdoFunction*>* aggregates = GetAggregateFunctions(funcDefs, selectedId, exprType);
        if ((exprType == FdoCommonExpressionType_Aggregate) && (NULL!=aggregates))
        {
            bContainsAggregateExpressions = true;

            if (NULL == aggrIdents)
                aggrIdents = FdoArray<FdoFunction*>::Create();

            aggrIdents = FdoArray<FdoFunction*>::Append(aggrIdents, aggregates->GetCount(), aggregates->GetData());
        }
        FDO_SAFE_RELEASE(aggregates);
        if (exprType == FdoCommonExpressionType_PerRow)
            bContainsPerRowExpressions = true;
    }
    if (bContainsAggregateExpressions && bContainsPerRowExpressions)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_79_AGGREGATE_INVALID_MIX)));

    // Set exprType:
    if (bContainsAggregateExpressions)
        exprType = FdoCommonExpressionType_Aggregate;
    else if (bContainsPerRowExpressions)
        exprType = FdoCommonExpressionType_PerRow;
    else
        exprType = FdoCommonExpressionType_Constant;

    return aggrIdents;
}


FdoArray<FdoFunction*>* FdoExpressionEngineUtilDataReader::GetAggregateFunctions(FdoFunctionDefinitionCollection *funcDefs, FdoExpression* expr, FdoCommonExpressionType &exprType)
{
    VALIDATE_ARGUMENT(expr);

    // Initialize return values to defaults:

    FdoArray<FdoFunction*>* retAggrs = NULL;
    exprType = FdoCommonExpressionType_Constant;

    // Determine the proper return values:

    FdoUnaryExpression* unaryExpr = dynamic_cast<FdoUnaryExpression*>(expr);
    if (NULL != unaryExpr)
        return GetAggregateFunctions(funcDefs, FdoPtr<FdoExpression>(unaryExpr->GetExpression()), exprType);

    FdoBinaryExpression* binaryExpr = dynamic_cast<FdoBinaryExpression*>(expr);
    if (NULL != binaryExpr)
    {
        FdoCommonExpressionType leftExprType;
        FdoArray<FdoFunction*>* leftAggrs = GetAggregateFunctions(funcDefs, FdoPtr<FdoExpression>(binaryExpr->GetLeftExpression()), leftExprType);
        FdoCommonExpressionType rightExprType;
        FdoArray<FdoFunction*>* rightAggrs = GetAggregateFunctions(funcDefs, FdoPtr<FdoExpression>(binaryExpr->GetRightExpression()), rightExprType);

        // Check for invalid combination of expression types:
        if (   ((leftExprType == FdoCommonExpressionType_Aggregate) && (rightExprType == FdoCommonExpressionType_PerRow))
            || ((leftExprType == FdoCommonExpressionType_PerRow) && (rightExprType == FdoCommonExpressionType_Aggregate)) )
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_79_AGGREGATE_INVALID_MIX)));

        // Set exprType and retAggrs:
        if ((leftExprType==FdoCommonExpressionType_Aggregate) || (rightExprType==FdoCommonExpressionType_Aggregate))
        {
            exprType = FdoCommonExpressionType_Aggregate;

            if (NULL==retAggrs)
                retAggrs = FdoArray<FdoFunction*>::Create();
            if ((leftExprType==FdoCommonExpressionType_Aggregate) && (NULL!=leftAggrs))
                retAggrs = FdoArray<FdoFunction*>::Append(retAggrs, leftAggrs->GetCount(),  leftAggrs->GetData());
            if ((rightExprType==FdoCommonExpressionType_Aggregate) && (NULL!=rightAggrs))
                retAggrs = FdoArray<FdoFunction*>::Append(retAggrs, rightAggrs->GetCount(), rightAggrs->GetData());
            FDO_SAFE_RELEASE(leftAggrs);
            FDO_SAFE_RELEASE(rightAggrs);
        }
        else if ((leftExprType==FdoCommonExpressionType_PerRow) || (rightExprType==FdoCommonExpressionType_PerRow))
            exprType = FdoCommonExpressionType_PerRow;
        else
            exprType = FdoCommonExpressionType_Constant;

        return retAggrs;
    }

    FdoComputedIdentifier* computedId = dynamic_cast<FdoComputedIdentifier*>(expr);
    if (NULL != computedId)
        return GetAggregateFunctions(funcDefs, FdoPtr<FdoExpression>(computedId->GetExpression()), exprType);

    FdoIdentifier* noncomputedId = dynamic_cast<FdoIdentifier*>(expr);
    if (NULL != noncomputedId)
    {
        exprType = FdoCommonExpressionType_PerRow;
        return retAggrs;
    }

    FdoValueExpression* valueExpr = dynamic_cast<FdoValueExpression*>(expr);
    if (NULL != valueExpr)
    {
        exprType = FdoCommonExpressionType_Constant;
        return retAggrs;
    }

    FdoFunction* function = dynamic_cast<FdoFunction*>(expr);
    if (NULL != function)
    {
        if (FdoExpressionEngine::IsAggregateFunction(funcDefs, function->GetName()))
        {
            // Validate that each argument to the aggregate function is NOT an aggregate expression:
            FdoPtr<FdoExpressionCollection> funcArgs = function->GetArguments();
            for (FdoInt32 i=0; i<funcArgs->GetCount(); i++)
            {
                FdoPtr<FdoExpression> funcArg = funcArgs->GetItem(i);
                FdoCommonExpressionType funcArgExprType;
                FdoArray<FdoFunction*>* funcArgAggregates = GetAggregateFunctions(funcDefs, funcArg, funcArgExprType);
                FDO_SAFE_RELEASE(funcArgAggregates);
                if (funcArgExprType == FdoCommonExpressionType_Aggregate)
                    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_78_AGGREGATE_IN_AGGREGATE_FUNCTION)));
            }

            // Set exprType:
            exprType = FdoCommonExpressionType_Aggregate;

            // Add this function to the aggregate function list:
            //TODO: verify if the functions in the array need to be addref'ed (and later release'd)
            if (NULL==retAggrs)
                retAggrs = FdoArray<FdoFunction*>::Create();
            retAggrs = FdoArray<FdoFunction*>::Append(retAggrs, function);
        }
        else
        {
            bool bContainsAggregateExpressions = false;
            bool bContainsPerRowExpressions = false;

            FdoPtr<FdoExpressionCollection> funcArgs = function->GetArguments();
            exprType = FdoCommonExpressionType_Constant;
            for (FdoInt32 i=0; i<funcArgs->GetCount(); i++)
            {
                FdoPtr<FdoExpression> funcArg = funcArgs->GetItem(i);
                FdoCommonExpressionType funcArgExprType;
                FdoArray<FdoFunction*>* aggrIds = GetAggregateFunctions(funcDefs, funcArg, funcArgExprType);
                if ((funcArgExprType == FdoCommonExpressionType_Aggregate) && (NULL!=aggrIds))
                {
                    bContainsAggregateExpressions = true;
                    if (NULL==retAggrs)
                        retAggrs = FdoArray<FdoFunction*>::Create();
                    retAggrs = FdoArray<FdoFunction*>::Append(retAggrs, aggrIds->GetCount(), aggrIds->GetData());
                    FDO_SAFE_RELEASE(aggrIds);
                }
                else if (funcArgExprType == FdoCommonExpressionType_PerRow)
                    bContainsPerRowExpressions = true;
            }

            if (bContainsPerRowExpressions && bContainsAggregateExpressions)
                throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_79_AGGREGATE_INVALID_MIX)));

            // Set exprType:
            if (bContainsAggregateExpressions)
                exprType = FdoCommonExpressionType_Aggregate;
            else if (bContainsPerRowExpressions)
                exprType = FdoCommonExpressionType_PerRow;
            else
                exprType = FdoCommonExpressionType_Constant;
        }

        return retAggrs;
    }

    return retAggrs;
}


FdoClassDefinition* FdoExpressionEngineUtilDataReader::GetAggregateClassDef(FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds)
{
    // TODO: return FdoClass instead of FdoFeatureClass since no geometry property?  Does it really make a difference?
    FdoClassDefinition* clone = FdoFeatureClass::Create();

    FdoPtr<FdoPropertyDefinitionCollection> properties = clone->GetProperties();

    //add new properties corresponding to each selected identifier
    //we need this so that we can generate a serialized data record
    //of the results, for use by the standard DataReader
    for (int i=0; i<selectedIds->GetCount(); i++)
    {
        FdoPtr<FdoIdentifier> id = selectedIds->GetItem(i);
        _ASSERT(id);

        FdoPropertyType propType;
        FdoDataType dataType;
        FdoCommonMiscUtil::GetExpressionType(m_functions, originalClassDef, id, propType, dataType);

        if (propType == FdoPropertyType_DataProperty)
        {
            FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(id->GetName(), NULL);
            dpd->SetDataType( dataType );
            properties->Add(dpd);
        }
        else if (propType == FdoPropertyType_GeometricProperty)
        {
            FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(id->GetName(), NULL);
            properties->Add(gpd);
        }
        else
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_70_PROPERTY_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoPropertyTypeToString(propType)));
    }

    return clone;
}

void FdoExpressionEngineUtilDataReader::RunAggregateQuery(FdoIFeatureReader* rdr, FdoClassDefinition* originalClassDef, FdoIdentifierCollection* selectedIds, FdoClassDefinition* aggrClassDef, FdoArray<FdoFunction*>* aggrFunctions)
{

	FdoPtr<FdoExpressionEngine> engine = FdoExpressionEngine::Create(rdr, originalClassDef, selectedIds, NULL);
	FdoPtr<FdoPropertyValueCollection> pvc = engine->RunQuery();
    rdr->Close();

    if (pvc->GetCount() > 0)
    {
        //serialize query results:
        // NOTE: currently there is only one result, but if groupby is to be supported then there could be more
        FdoPtr<FdoCommonBinaryWriter> wrt = new FdoCommonBinaryWriter(64);
        wrt->WritePropertyValues(aggrClassDef, m_propIndex, pvc);

        // Store the results in m_results:
        FdoByteArray* bytes = FdoByteArray::Create(wrt->GetData(), wrt->GetDataLen());  // NO RELEASE OF bytes ON PURPOSE!
        m_results.push_back(bytes);
    }
}




void FdoExpressionEngineUtilDataReader::PerformDistinct()
{
    //Dump m_results to a hashmap, to weed out duplicate rows:
    HASHMAP hashmap;
    hashmap.clear();
    for (FdoInt32 i=0; i<(FdoInt32)m_results.size(); i++)
    {
        FdoByteArray *bytes = (FdoByteArray*)m_results.at(i);

        std::pair <HASHMAP_ITER, bool> testresult = hashmap.insert(HASHMAP_PAIR(bytes, NULL));
        // Do a release ONLY if its not been inserted into the hashmap:
        if (!testresult.second)
            FDO_SAFE_RELEASE(bytes);
    }

    // Dump hashmap back into m_results:
    m_results.clear();
    for (HASHMAP_ITER iter = hashmap.begin(); iter != hashmap.end(); iter++)
    {
        FdoByteArray* array = (FdoByteArray*)iter->first;
        m_results.push_back(array);
    }
}






// Sorting function:
bool vector_sort(orderby_context rowcontext1, orderby_context rowcontext2)
{
    FdoByteArray* row1bytes = rowcontext1.rowData;
    FdoByteArray* row2bytes = rowcontext2.rowData;
    FdoExpressionEngineUtilDataReader* parentReader = rowcontext1.parentReader;
    FdoPtr<FdoIdentifierCollection> orderbyIds = parentReader->GetOrdering();
    FdoPtr<FdoCommonPropertyIndex> propIndex = parentReader->GetPropertyIndex();
    FdoPtr<FdoCommonBinaryReader> orderbyBinReader1 = parentReader->GetOrderingBinReader1();
    FdoPtr<FdoCommonBinaryReader> orderbyBinReader2 = parentReader->GetOrderingBinReader2();

    orderbyBinReader1->Reset((unsigned char*)row1bytes->GetData(), row1bytes->GetCount());
    orderbyBinReader2->Reset((unsigned char*)row2bytes->GetData(), row2bytes->GetCount());

    // Iterate through orderby ids:
    bool bRet = false;
    bool bDone = false;
    for (int i=0; i<orderbyIds->GetCount() && !bDone; i++)
    {
        FdoPtr<FdoIdentifier> orderbyId = orderbyIds->GetItem(i);
        FdoString* orderbyidName = orderbyId->GetName();

        FdoCommonPropertyStub* ps = propIndex->GetPropInfo(orderbyidName);
        int len1 = orderbyBinReader1->PositionReader(ps->m_recordIndex, propIndex);
        if (len1 == 0)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_73_PROPERTY_INDEXOUTOFBOUNDS), ps->m_recordIndex));
        int len2 = orderbyBinReader2->PositionReader(ps->m_recordIndex, propIndex);
        if (len2 == 0)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_73_PROPERTY_INDEXOUTOFBOUNDS), ps->m_recordIndex));

        switch (ps->m_dataType)
        {
        case FdoDataType_String:
        {
            FdoString *str2 = orderbyBinReader2->ReadRawString(len2);
            FdoString *str1 = orderbyBinReader1->ReadRawString(len1);
            int result = wcscmp(str1,str2);
            bDone = (result != 0);
            bRet  = (result <  0);
        }
        break;

        case FdoDataType_Int16:
        {
            FdoInt16 int16_1 = orderbyBinReader1->ReadInt16();
            FdoInt16 int16_2 = orderbyBinReader2->ReadInt16();
            bDone = (int16_1 != int16_2);
            bRet  = (int16_1 <  int16_2);
        }
        break;

        case FdoDataType_Int32:
        {
            FdoInt32 int32_1 = orderbyBinReader1->ReadInt32();
            FdoInt32 int32_2 = orderbyBinReader2->ReadInt32();
            bDone = (int32_1 != int32_2);
            bRet  = (int32_1 <  int32_2);
        }
        break;

        case FdoDataType_Int64:
        {
            FdoInt64 int64_1 = orderbyBinReader1->ReadInt64();
            FdoInt64 int64_2 = orderbyBinReader2->ReadInt64();
            bDone = (int64_1 != int64_2);
            bRet  = (int64_1 <  int64_2);
        }
        break;

        case FdoDataType_Single:
        {
            float s1 = orderbyBinReader1->ReadSingle();
            float s2 = orderbyBinReader2->ReadSingle();
            bDone = (s1 != s2);
            bRet  = (s1 <  s2);
        }
        break;

        case FdoDataType_Decimal:  // Treat Decimal like Double
        case FdoDataType_Double:
        {
            double d1 = orderbyBinReader1->ReadDouble();
            double d2 = orderbyBinReader2->ReadDouble();
            bDone = (d1 != d2);
            bRet  = (d1 <  d2);
        }
        break;

        case FdoDataType_DateTime:
        {
            FdoDateTime d1 = orderbyBinReader1->ReadDateTime();
            FdoDateTime d2 = orderbyBinReader2->ReadDateTime();

            int dateComparison = FdoCommonMiscUtil::CompareDateTimes(d1, d2);

            bRet = (dateComparison<0);
            bDone = (dateComparison==0);
        }
        break;

        case FdoDataType_Byte:
        {
            FdoByte b1 = orderbyBinReader1->ReadByte();
            FdoByte b2 = orderbyBinReader2->ReadByte();
            bDone = (b1 != b2);
            bRet  = (b1 <  b2);
        }
        break;

        case FdoDataType_Boolean:
        {
            bool b1 = (0 != orderbyBinReader1->ReadByte());
            bool b2 = (0 != orderbyBinReader2->ReadByte());
            bDone = (b1 != b2);
            bRet  = ((b1==false) && (b2==true));
        }
        break;

        // Unhandled types:
        default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_77_ORDERBY_INVALID_TYPE), FdoCommonMiscUtil::FdoDataTypeToString(ps->m_dataType)));
        }
    }

    // Return result:
    if (!bDone) // if all id values are equal
        return false;

	return (parentReader->GetOrderingOption()==FdoOrderingOption_Ascending) ? bRet : !bRet;
}

FdoIdentifierCollection* FdoExpressionEngineUtilDataReader::GetOrdering()
{
    return FDO_SAFE_ADDREF(m_orderbyIds.p);
}

FdoOrderingOption FdoExpressionEngineUtilDataReader::GetOrderingOption()
{
    return m_orderbyOption;
}

FdoCommonBinaryReader* FdoExpressionEngineUtilDataReader::GetOrderingBinReader1()
{
    if (m_orderbyBinReader1==NULL)
        m_orderbyBinReader1 = new FdoCommonBinaryReader(NULL, 0);
    return FDO_SAFE_ADDREF(m_orderbyBinReader1.p);
}

FdoCommonBinaryReader* FdoExpressionEngineUtilDataReader::GetOrderingBinReader2()
{
    if (m_orderbyBinReader2==NULL)
        m_orderbyBinReader2 = new FdoCommonBinaryReader(NULL, 0);
    return FDO_SAFE_ADDREF(m_orderbyBinReader2.p);
}



void FdoExpressionEngineUtilDataReader::PerformOrderBy()
{
    //Dump the results to a vector that contains context information required for sorting:
    std::vector<orderby_context> orderedResults;
    orderedResults.clear();
    for (FdoInt32 i=0; i<(FdoInt32)m_results.size(); i++)
    {
        FdoByteArray* bytes = (FdoByteArray*)m_results.at(i);
        orderby_context context(this, bytes);
        orderedResults.push_back(context);
    }

    // Now sort all the results based on order-by criteria:
    std::sort(orderedResults.begin(), orderedResults.end(), vector_sort);

    // Dump the results back into m_results:
    for (FdoInt32 i=0; i<(FdoInt32)orderedResults.size(); i++)
    {
        m_results[i] = orderedResults.at(i).rowData;
    }
}



bool FdoExpressionEngineUtilDataReader::ReadNext()
{
    m_resultsIndex++;

    if (m_resultsIndex >= (FdoInt32)m_results.size())
        return false;
    else
    {
        // Read next record:
        const FdoByteArray* byteArray = (FdoByteArray*)m_results.at(m_resultsIndex);
        m_binReader->Reset((unsigned char*)byteArray->GetData(), byteArray->GetCount());

        return true;
    }
}


FdoCommonBinaryReader* FdoExpressionEngineUtilDataReader::GetBinaryReader()
{
    return FDO_SAFE_ADDREF(m_binReader.p);
}



