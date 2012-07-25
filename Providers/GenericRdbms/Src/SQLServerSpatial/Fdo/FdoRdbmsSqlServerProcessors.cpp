/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License AS published by the Free Software Foundation.
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
#include "FdoRdbmsSqlServerProcessors.h"
#include "../../Fdo/Schema/FdoRdbmsSchemaUtil.h"
#include <Geometry/Fgf/Factory.h>

#include "SpatialManager/FdoRdbmsSpatialManager.h"
#include "FdoRdbmsSqlServerConnection.h"
#include "FdoCommonOSUtil.h"
#include "FdoRdbmsFunctionIsValid.h"
#include "../SchemaMgr/Ph/SpatialIndex.h"
#include "../SchemaMgr/Ph/Mgr.h"
#include <FdoExpressionEngineCopyFilter.h>
#include "FdoRdbmsSqlServerDateTimeUtils.h"
#include <ctype.h>

#ifndef _WIN32
    #define _wcsicmp wcscasecmp
#endif

#define FDO_FUNCTION_ISVALID                L"IsValid"

#define IDX_FDO_FUNCTION_COMMON             1
#define IDX_FDO_FUNCTION_ROUND              2
#define IDX_FDO_FUNCTION_SIGN               3
#define IDX_FDO_FUNCTION_TRUNC              4
#define IDX_FDO_FUNCTION_ATAN2              5
#define IDX_FDO_FUNCTION_LN                 6
#define IDX_FDO_FUNCTION_LOG                7
#define IDX_FDO_FUNCTION_MOD                8
#define IDX_FDO_FUNCTION_POWER              9
#define IDX_FDO_FUNCTION_REMAINDER          10
#define IDX_FDO_FUNCTION_AREA2D             11
#define IDX_FDO_FUNCTION_LENGTH2D           12
#define IDX_FDO_FUNCTION_X                  13
#define IDX_FDO_FUNCTION_Y                  14
#define IDX_FDO_FUNCTION_ZM                 15
#define IDX_FDO_FUNCTION_ISVALID            16
#define IDX_FDO_FUNCTION_ADDMONTHS          17
#define IDX_FDO_FUNCTION_CURRENTDATE        18
#define IDX_FDO_FUNCTION_EXTRACT            19
#define IDX_FDO_FUNCTION_EXTRACTTODOUBLE    20
#define IDX_FDO_FUNCTION_EXTRACTTOINT       21
#define IDX_FDO_FUNCTION_MONTHSBETWEEN      22
#define IDX_FDO_FUNCTION_NULLVALUE          23
#define IDX_FDO_FUNCTION_TODATE             24
#define IDX_FDO_FUNCTION_TODOUBLE           25
#define IDX_FDO_FUNCTION_TOFLOAT            26
#define IDX_FDO_FUNCTION_TOINT32            27
#define IDX_FDO_FUNCTION_TOINT64            28
#define IDX_FDO_FUNCTION_TOSTRING           29
#define IDX_FDO_FUNCTION_CONCAT             30
#define IDX_FDO_FUNCTION_INSTR              31
#define IDX_FDO_FUNCTION_LENGTH             32
#define IDX_FDO_FUNCTION_LPAD               33
#define IDX_FDO_FUNCTION_RPAD               34
#define IDX_FDO_FUNCTION_SUBSTR             35
#define IDX_FDO_FUNCTION_TRIM               36
#define IDX_FDO_FUNCTION_MEDIAN             37
#define IDX_FDO_FUNCTION_AGG                38
#define IDX_FDO_FUNCTION_SPATIALEXTENTS     39
#define IDX_FDO_FUNCTION_CEIL               40

void FdoSqlServerInvalidExpDetProcessor::ProcessFunction(FdoFunction& expr)
{
    if (mState == 0x02)
        return;

    FdoString* name = expr.GetName();
    if (_wcsicmp(name, FDO_FUNCTION_TRANSLATE) == 0 || _wcsicmp(name, FDO_FUNCTION_MEDIAN) == 0)
    {
        mExpSupported = false;
        Done();
        return;
    }

    FdoPtr<FdoExpressionCollection>col = expr.GetArguments();
    for(int i=0; i<col->GetCount(); i++)
        HandleExpr( FdoPtr<FdoExpression>(col->GetItem( i ) ) );
}

FdoRdbmsSqlServerSqlBuilder::FdoRdbmsSqlServerSqlBuilder (FdoRdbmsSqlServerConnection* conn)
    : FdoRdbmsSqlBuilder(conn)
{
    m_props = NULL;
    m_fdoConn = conn;
    m_builderStack.push_back(new pair_working_stack(2*CONV_MEM_BLOCK_SIZE));
    m_posStack = 0;
    m_lastGtype = SqlGeometryType_Geometry;
    m_lastSrid = 0;
    m_useBr = false;
    m_hasClaculations = false;
    m_aliasStartChar = L'A';
    m_aliasStartIdxNo = 0;
}

FdoRdbmsSqlServerSqlBuilder::~FdoRdbmsSqlServerSqlBuilder()
{
    for (exp_working_stack::iterator it = m_builderStack.begin(); it != m_builderStack.end(); it++)
    {
        delete *it;
    }
    for (function_sig_map::iterator it = m_fctSig.begin(); it != m_fctSig.end(); it++)
    {
        delete it->second;
    }    
}

// this will generate: A-Z, A1-Z1, ...
FdoString* FdoRdbmsSqlServerSqlBuilder::AssignTempAlias(const FdoSmLpClassDefinition* clsDef)
{
    function_map aliases;
    for (sel_class_alias::iterator it = m_usedClasses.begin(); it != m_usedClasses.end(); it++)
    {
        if (it->second.size() != 0)
            aliases[it->second.c_str()] = 1;
    }
    std::wstring gAlias(&m_aliasStartChar, 1);
    function_map::iterator itm;
    
    while((itm = aliases.find(gAlias.c_str())) != aliases.end())
    {
        m_aliasStartChar++;
        if (m_aliasStartChar > L'Z')
        {
            m_aliasStartIdxNo++;
            m_aliasStartChar = L'A';
            gAlias.clear();
            gAlias.append(&m_aliasStartChar, 1);
            swprintf(m_useConv, 50, L"%d", m_aliasStartIdxNo);
            gAlias.append(m_useConv);
        }
        else
            gAlias[0] = m_aliasStartChar;
    }
    m_tempUsedClasses.push_back(std::make_pair(clsDef, gAlias));
    return m_tempUsedClasses.back().second.c_str();
}

void FdoRdbmsSqlServerSqlBuilder::AssignAliases()
{
    function_map aliases;
    for (sel_class_alias::iterator it = m_usedClasses.begin(); it != m_usedClasses.end(); it++)
    {
        if (it->second.size() != 0)
        {
            function_map::iterator itm = aliases.find(it->second.c_str());
            if (itm != aliases.end())
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
            aliases[it->second.c_str()] = 1;
        }
    }
    if (m_usedClasses.size() == aliases.size())
        return;
    std::wstring gAlias(&m_aliasStartChar, 1);
    for (sel_class_alias::iterator it = m_usedClasses.begin(); it != m_usedClasses.end(); it++)
    {
        if (it->second.size() == 0)
        {
            function_map::iterator itm;
            while((itm = aliases.find(gAlias.c_str())) != aliases.end())
            {
                m_aliasStartChar++;
                if (m_aliasStartChar > L'Z')
                {
                    m_aliasStartIdxNo++;
                    m_aliasStartChar = L'A';
                    gAlias.clear();
                    gAlias.append(&m_aliasStartChar, 1);
                    swprintf(m_useConv, 50, L"%d", m_aliasStartIdxNo);
                    gAlias.append(m_useConv);
                }
                else
                    gAlias[0] = m_aliasStartChar;
            }
            it->second = gAlias;
            aliases[it->second.c_str()] = 1;
        }
    }
}

void FdoRdbmsSqlServerSqlBuilder::LoadFunctionDefinitions()
{
    m_functions[FDO_FUNCTION_ABS] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_ACOS] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_ASIN] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_ATAN] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_COS] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_EXP] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_SIN] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_SQRT] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_TAN] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_FLOOR] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_UPPER] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_LOWER] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_LTRIM] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_RTRIM] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_SOUNDEX] = IDX_FDO_FUNCTION_COMMON;
    m_functions[FDO_FUNCTION_ROUND] = IDX_FDO_FUNCTION_ROUND;
    m_functions[FDO_FUNCTION_CEIL] = IDX_FDO_FUNCTION_CEIL;
    m_functions[FDO_FUNCTION_SIGN] = IDX_FDO_FUNCTION_SIGN;
    m_functions[FDO_FUNCTION_TRUNC] = IDX_FDO_FUNCTION_TRUNC;
    m_functions[FDO_FUNCTION_ATAN2] = IDX_FDO_FUNCTION_ATAN2;
    m_functions[FDO_FUNCTION_LN] = IDX_FDO_FUNCTION_LN;
    m_functions[FDO_FUNCTION_LOG] = IDX_FDO_FUNCTION_LOG;
    m_functions[FDO_FUNCTION_MOD] = IDX_FDO_FUNCTION_MOD;
    m_functions[FDO_FUNCTION_POWER] = IDX_FDO_FUNCTION_POWER;
    m_functions[FDO_FUNCTION_REMAINDER] = IDX_FDO_FUNCTION_REMAINDER;
    m_functions[FDO_FUNCTION_AREA2D] = IDX_FDO_FUNCTION_AREA2D;
    m_functions[FDO_FUNCTION_LENGTH2D] = IDX_FDO_FUNCTION_LENGTH2D;
    m_functions[FDO_FUNCTION_X] = IDX_FDO_FUNCTION_X;
    m_functions[FDO_FUNCTION_Y] = IDX_FDO_FUNCTION_Y;
    m_functions[FDO_FUNCTION_Z] = IDX_FDO_FUNCTION_ZM;
    m_functions[FDO_FUNCTION_M] = IDX_FDO_FUNCTION_ZM;
    m_functions[FDO_FUNCTION_ISVALID] = IDX_FDO_FUNCTION_ISVALID;
    m_functions[FDO_FUNCTION_ADDMONTHS] = IDX_FDO_FUNCTION_ADDMONTHS;
    m_functions[FDO_FUNCTION_CURRENTDATE] = IDX_FDO_FUNCTION_CURRENTDATE;
    m_functions[FDO_FUNCTION_EXTRACT] = IDX_FDO_FUNCTION_EXTRACT;
    m_functions[FDO_FUNCTION_EXTRACTTODOUBLE] = IDX_FDO_FUNCTION_EXTRACTTODOUBLE;
    m_functions[FDO_FUNCTION_EXTRACTTOINT] = IDX_FDO_FUNCTION_EXTRACTTOINT;
    m_functions[FDO_FUNCTION_MONTHSBETWEEN] = IDX_FDO_FUNCTION_MONTHSBETWEEN;
    m_functions[FDO_FUNCTION_NULLVALUE] = IDX_FDO_FUNCTION_NULLVALUE;
    m_functions[FDO_FUNCTION_TODATE] = IDX_FDO_FUNCTION_TODATE;
    m_functions[FDO_FUNCTION_TODOUBLE] = IDX_FDO_FUNCTION_TODOUBLE;
    m_functions[FDO_FUNCTION_TOFLOAT] = IDX_FDO_FUNCTION_TOFLOAT;
    m_functions[FDO_FUNCTION_TOINT32] = IDX_FDO_FUNCTION_TOINT32;
    m_functions[FDO_FUNCTION_TOINT64] = IDX_FDO_FUNCTION_TOINT64;
    m_functions[FDO_FUNCTION_TOSTRING] = IDX_FDO_FUNCTION_TOSTRING;
    m_functions[FDO_FUNCTION_CONCAT] = IDX_FDO_FUNCTION_CONCAT;
    m_functions[FDO_FUNCTION_INSTR] = IDX_FDO_FUNCTION_INSTR;
    m_functions[FDO_FUNCTION_LENGTH] = IDX_FDO_FUNCTION_LENGTH;
    m_functions[FDO_FUNCTION_LPAD] = IDX_FDO_FUNCTION_LPAD;
    m_functions[FDO_FUNCTION_RPAD] = IDX_FDO_FUNCTION_RPAD;
    m_functions[FDO_FUNCTION_SUBSTR] = IDX_FDO_FUNCTION_SUBSTR;
    m_functions[FDO_FUNCTION_TRIM] = IDX_FDO_FUNCTION_TRIM;
    //m_functions[FDO_FUNCTION_MEDIAN] = IDX_FDO_FUNCTION_MEDIAN;
    m_functions[FDO_FUNCTION_AVG] = IDX_FDO_FUNCTION_AGG;
    m_functions[FDO_FUNCTION_COUNT] = IDX_FDO_FUNCTION_AGG;
    m_functions[FDO_FUNCTION_MAX] = IDX_FDO_FUNCTION_AGG;
    m_functions[FDO_FUNCTION_MIN] = IDX_FDO_FUNCTION_AGG;
    m_functions[FDO_FUNCTION_SUM] = IDX_FDO_FUNCTION_AGG;
    m_functions[FDO_FUNCTION_STDDEV] = IDX_FDO_FUNCTION_AGG;
    m_functions[FDO_FUNCTION_SPATIALEXTENTS] = IDX_FDO_FUNCTION_SPATIALEXTENTS;
}

//--------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------
void FdoRdbmsSqlServerSqlBuilder::ProcessMedianFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    
    // Get the class definition for the identified class.
    //DbiConnection *dbiConn = m_fdoConn->GetDbiConnection();
    //const FdoSmLpClassDefinition *classDefinition = dbiConn->GetSchemaUtil()->GetClass(L"TODO"); // TODO when select aggregates will use this
    //FdoStringP tableName = dbiConn->GetSchemaUtil()->GetDbObjectSqlName(classDefinition);
    FdoStringP tableName;

    FdoPtr<FdoExpression> exp = exprCol->GetItem(0);
    pair_working_stack* itmExp = push_stack();
    exp->Process(this);

    itm->first.append(L"select max(medianVal) FROM (select distinct top 50 percent ", 59);
    itm->first.append(itmExp->first);
    itm->first.append(L" AS medianVal FROM ", 19);
    itm->first.append((FdoString *)tableName);
    itm->first.append(L" AS ", 4);
    //itm->first.append(GetTableAlias(tableName));
    itm->first.append(L" WHERE ", 7);
    itm->first.append(itmExp->first);
    itm->first.append(L" IS NOT NULL ORDER BY ", 22);
    itm->first.append(itmExp->first);
    itm->first.append(L") AS medianTb", 13);

    pop_stack(); // itmExp
}

void FdoRdbmsSqlServerSqlBuilder::ProcessArea2dFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (exp0->GetExpressionType() != FdoExpressionItemType_Identifier)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    m_useBr = true;
    exp0->Process(this);
    m_useBr = false;
    itm->first.append(L".STArea()", 9);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessLength2dFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (exp0->GetExpressionType() != FdoExpressionItemType_Identifier)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    m_useBr = true;
    exp0->Process(this);
    m_useBr = false;
    itm->first.append(L".STLength()", 11);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessXFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (exp0->GetExpressionType() != FdoExpressionItemType_Identifier)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    m_useBr = true;
    exp0->Process(this);
    m_useBr = false;
    if (m_lastGtype == SqlGeometryType_Geometry)
        itm->first.append(L".STX", 4);
    else
        itm->first.append(L".Long", 5);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessYFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (exp0->GetExpressionType() != FdoExpressionItemType_Identifier)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    m_useBr = true;
    exp0->Process(this);
    m_useBr = false;
    if (m_lastGtype == SqlGeometryType_Geometry)
        itm->first.append(L".STY", 4);
    else
        itm->first.append(L".Lat", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessZMFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (exp0->GetExpressionType() != FdoExpressionItemType_Identifier)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    m_useBr = true;
    exp0->Process(this);
    m_useBr = false;
    itm->first.append(L".", 1);
    itm->first.append(expr.GetName());
}

void FdoRdbmsSqlServerSqlBuilder::ProcessIsValidFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (exp0->GetExpressionType() != FdoExpressionItemType_Identifier)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    // There are problems processing booleans. Use integers instead.
    itm->first.append(L"ABS(", 4);
    m_useBr = true;
    exp0->Process(this);
    m_useBr = false;
    itm->first.append(L".STIsValid())", 13);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessToStringFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (colCount == 2) // date->ToString()?
    {
        FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
        bool processed = false;
        DateTokenFormat tf;
        if (FdoExpressionItemType_DataValue == exp1->GetExpressionType())
        {
            FdoDataValue* dv = static_cast<FdoDataValue*>(exp1.p);
            if (dv != NULL && dv->GetDataType() == FdoDataType_String)
            {
                FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                if (strVal != NULL && !strVal->IsNull())
                {
                    FdoString* format = strVal->GetString();
                    tf.ProcessFormat(format);
                    processed = true;
                }
            }
        }
        FdoString* sql = NULL;
        if (processed)
        {
            pair_working_stack* itmExp0 = push_stack();
            exp0->Process(this);
            sql = tf.ToSQL(itmExp0->first.c_str());
            pop_stack(); // itmExp0
        }
        if (!processed || !sql)
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        itm->first.append(sql);
    }
    else
    {
        FdoDataType dataType = GetApproxExpressionDataType(exp0);
        if (dataType == FdoDataType_DateTime)
        {
            FdoString* sql = NULL;
            DateTokenFormat tf;
            tf.ProcessFormat(NULL);
            
            pair_working_stack* itmExp0 = push_stack();
            exp0->Process(this);
            sql = tf.ToSQL(itmExp0->first.c_str());
            pop_stack(); // itmExp0

            if (!sql)
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
            itm->first.append(sql);
        }
        else
        {
            itm->first.append(L"CONVERT(NVARCHAR,", 17);
            exp0->Process(this);
            itm->first.append(L")", 1);
        }
    }
}

void FdoRdbmsSqlServerSqlBuilder::ProcessToDateFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoExpressionItemType type = exp0->GetExpressionType();
    if (FdoExpressionItemType_DataValue == type)
    {
        FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
        if (dv == NULL || dv->IsNull() || dv->GetDataType() != FdoDataType_String)
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }

    pair_working_stack* itmExp0 = push_stack();
    exp0->Process(this);

    DateTokenFormat tf;
    bool processed = false;
    FdoString* sql = NULL;
    if (colCount == 2)
    {
        FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
        if (FdoExpressionItemType_DataValue == exp1->GetExpressionType())
        {
            FdoDataValue* dv = static_cast<FdoDataValue*>(exp1.p);
            if (dv != NULL && dv->GetDataType() == FdoDataType_String)
            {
                FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                if (strVal != NULL && !strVal->IsNull())
                {
                    tf.ProcessFormat(strVal->GetString());
                    sql = tf.ToDateTimeSQL(itmExp0->first.c_str());
                    processed = true;
                }
            }
        }
    }
    else
    {
        tf.ProcessFormat(NULL);
        sql = tf.ToDateTimeSQL(itmExp0->first.c_str());
        processed = true;
    }

    pop_stack(); // itmExp0

    if (!processed || !sql)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    itm->first.append(sql);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessToDoubleFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    itm->first.append(L"CONVERT(REAL,", 13);
    exp0->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessToFloatFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    itm->first.append(L"CONVERT(float,", 14);
    exp0->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessToInt32Function (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    itm->first.append(L"CONVERT(INT, CONVERT(REAL,", 26);
    exp0->Process(this);
    itm->first.append(L"))", 2);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessToInt64Function (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    itm->first.append(L"CONVERT(BIGINT, CONVERT(REAL,", 29);
    exp0->Process(this);
    itm->first.append(L"))", 2);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessAddMonthsFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    itm->first.append(L"DATEADD(MONTH, CONVERT(BIGINT,", 30);
    exp1->Process(this);
    itm->first.append(L"),", 2);
    exp0->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessMonthsBetweenFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    itm->first.append(L"DATEDIFF(MONTH, ");
    exp0->Process(this);
    itm->first.append(L",", 1);
    exp1->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessLnFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);

    itm->first.append(L"LOG(", 4);
    exp0->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessLogFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    itm->first.append(L"LOG(", 4);
    exp1->Process(this);
    itm->first.append(L")/LOG(");
    exp0->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessPowerFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    itm->first.append(L"POWER(", 6);
    exp0->Process(this);
    itm->first.append(L",", 1);
    exp1->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessRoundFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    itm->first.append(L"ROUND(", 6);
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (colCount == 1)
    {
        exp0->Process(this);
        itm->first.append(L",0)", 3);
    }
    else
    {
        FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
        exp0->Process(this);
        itm->first.append(L",CONVERT(INT,", 13);
        exp1->Process(this);
        itm->first.append(L"))", 2);
    }
}

void FdoRdbmsSqlServerSqlBuilder::ProcessCeilFunction (FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    itm->first.append(L"CEILING(");
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    exp0->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessCommonFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    itm->first.append(expr.GetName());
    itm->first.append(L"(", 1);
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    exp0->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessLengthFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    itm->first.append(L"LEN(", 4);
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    exp0->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessCurrentDateFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 0)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    itm->first.append(L"GETDATE()", 9);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessAtn2Function(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    itm->first.append(L"ATN2(", 5);
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    exp0->Process(this);
    itm->first.append(L",", 1);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    exp1->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessNullValueFunction(FdoFunction& expr) // ISNULL
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    itm->first.append(L"ISNULL(", 7);
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    exp0->Process(this);
    itm->first.append(L",", 1);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    exp1->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessSpatialExtentsFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp = exprCol->GetItem(0);
    if (exp->GetExpressionType() != FdoExpressionItemType_Identifier)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    pair_working_stack* itmExp = push_stack();
    exp->Process(this);

    if (m_lastGtype == SqlGeometryType_Geography)
    {
        itm->first.append(L"geometry::STGeomFromWKB(");
        itm->first.append(itmExp->first);
        itm->first.append(L".STAsBinary(), 0).STEnvelope()");
    }
    else
    {
        itm->first.append(itmExp->first);
        itm->first.append(L".STEnvelope()", 13);
    }

    pop_stack(); // itmExp
}

void FdoRdbmsSqlServerSqlBuilder::ProcessAggregateFunction(FdoFunction& expr) // AVG(), COUNT(), MAX(), MIN(), STDEV(), SUM()
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    if (colCount == 2)
    {
        bool processed = false;
        FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
        if (FdoExpressionItemType_DataValue == exp0->GetExpressionType())
        {
            FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
            if (dv != NULL && dv->GetDataType() == FdoDataType_String)
            {
                FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                if (strVal != NULL && !strVal->IsNull())
                {
                    FdoString* oper = strVal->GetString();
                    itm->first.append(expr.GetName());
                    if (_wcsicmp(oper, L"DISTINCT") == 0)
                        itm->first.append(L"(DISTINCT", 9);
                    else
                        itm->first.append(L"(", 1);
                    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
                    exp1->Process(this);
                    itm->first.append(L")", 1);
                }
            }
        }
        if (!processed)
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }
    else
    {
        itm->first.append(expr.GetName());
        itm->first.append(L"(", 1);
        FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
        exp0->Process(this);
        itm->first.append(L")", 1);
    }
}

void FdoRdbmsSqlServerSqlBuilder::ProcessSignFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp = exprCol->GetItem(0);
    pair_working_stack* itmExp = push_stack();
    exp->Process(this);

    itm->first.append(L"CASE WHEN ", 10);
    itm->first.append(itmExp->first);
    itm->first.append(L" IS NULL THEN NULL WHEN ", 24);
    itm->first.append(itmExp->first);
    itm->first.append(L">0 THEN 1 WHEN ", 15);
    itm->first.append(itmExp->first);
    itm->first.append(L"<0 THEN -1 ELSE 0 END", 21);

    pop_stack(); // itmExp
}

void FdoRdbmsSqlServerSqlBuilder::ProcessConcatFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount == 0)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    for (FdoInt32 i = 0; i < colCount; i++)
    {
        itm->first.append(L"CONVERT(NVARCHAR,", 17);
        FdoPtr<FdoExpression> exp = exprCol->GetItem(i);
        exp->Process(this);
        itm->first.append(L")+", 2);
    }
    itm->first.resize(itm->first.size()-1); // remove last '+'
}

void FdoRdbmsSqlServerSqlBuilder::ProcessExtractFunction(FdoFunction& expr)
{
    // Note: since we cannot generate invalid date time values, we will preserve the wanted value 
    // and tuncate the others except the year (to have a valid date), and caller can get the wanted value.
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    bool processed = false;
    FdoDataType dataType = GetApproxExpressionDataType(exp1);
    if (dataType != FdoDataType_DateTime)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_183_INVALID_FUNCTION_ARG, "One or more arguments for function '%1$ls' did not match the expected argument types.", FDO_FUNCTION_EXTRACT));

    if (FdoExpressionItemType_DataValue == exp0->GetExpressionType())
    {
        FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
        if (dv != NULL && dv->GetDataType() == FdoDataType_String)
        {
            FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
            if (strVal != NULL && !strVal->IsNull())
            {
                pair_working_stack* itmExp1 = push_stack();
                exp1->Process(this);

                FdoString* oper = strVal->GetString();
                if (_wcsicmp(oper, L"YEAR") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(YEAR,", 19);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N'-01-01 00:00:00.000'", 39);
                }
                else if (_wcsicmp(oper, L"MONTH") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(YEAR,", 19);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(MONTH,", 44);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N'-01 00:00:00.000'", 36);
                }
                else if (_wcsicmp(oper, L"DAY") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(YEAR,", 19);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N'-01-' + CAST(DATEPART(DAY,", 45);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N' 00:00:00.000'", 33);
                }
                else if (_wcsicmp(oper, L"HOUR") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(YEAR,", 19);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N'-01-01 ' + CAST(DATEPART(HOUR,", 49);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N':00:00.000'", 30);
                }
                else if (_wcsicmp(oper, L"MINUTE") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(YEAR,", 19);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N'-01-01 00:' + CAST(DATEPART(MINUTE,", 54);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N':00.000'", 27);
                }
                else if (_wcsicmp(oper, L"SECOND") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(YEAR,", 19);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L") AS NVARCHAR) + N'-01-01 00:00:' + CAST((DATEPART(SECOND,", 58);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L")+DATEPART(MILLISECOND,", 23);
                    itm->first.append(itmExp1->first);
                    itm->first.append(L")/1000.0) AS NVARCHAR)", 22);
                }

                pop_stack(); // itmExp1
            }
        }
    }
    else
    {
        pair_working_stack* itmExp0 = push_stack();
        exp0->Process(this);

        pair_working_stack* itmExp1 = push_stack();
        exp1->Process(this);

        processed = true;
        itm->first.append(L"CASE WHEN ", 10);
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'YEAR' THEN CAST(DATEPART(YEAR,", 33);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N'-01-01 00:00:00.000' WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'MONTH' THEN CAST(DATEPART(YEAR,", 34);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(MONTH,", 44);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N'-01 00:00:00.000' WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'DAY' THEN CAST(DATEPART(YEAR,", 32);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N'-01-' + CAST(DATEPART(DAY,", 45);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N' 00:00:00.000' WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'HOUR' THEN CAST(DATEPART(YEAR,", 33);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N'-01-01 ' + CAST(DATEPART(HOUR,", 49);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N':00:00.000' WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'MINUTE' THEN CAST(DATEPART(YEAR,", 35);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N'-01-01 00:' + CAST(DATEPART(MINUTE,", 54);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N':00.000' WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'SECOND' THEN CAST(DATEPART(YEAR,", 35);
        itm->first.append(itmExp1->first);
        itm->first.append(L") AS NVARCHAR) + N'-01-01 00:00:' + CAST((DATEPART(SECOND,", 58);
        itm->first.append(itmExp1->first);
        itm->first.append(L")+DATEPART(MILLISECOND,", 23);
        itm->first.append(itmExp1->first);
        itm->first.append(L")/1000.0) AS NVARCHAR) END ", 22);
        
        pop_stack(); // itmExp1
        pop_stack(); // itmExp0
    }
    if (!processed)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
}

void FdoRdbmsSqlServerSqlBuilder::ProcessExtractToIntFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    bool processed = false;
    FdoDataType dataType = GetApproxExpressionDataType(exp1);
    if (dataType != FdoDataType_DateTime)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_183_INVALID_FUNCTION_ARG, "One or more arguments for function '%1$ls' did not match the expected argument types.", FDO_FUNCTION_EXTRACTTOINT));

    if (FdoExpressionItemType_DataValue == exp0->GetExpressionType())
    {
        FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
        if (dv != NULL && dv->GetDataType() == FdoDataType_String)
        {
            FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
            if (strVal != NULL && !strVal->IsNull())
            {
                FdoString* oper = strVal->GetString();
                if (_wcsicmp(oper, L"YEAR") == 0)
                {
                    processed = true;
                    itm->first.append(L"DATEPART(YEAR,", 14);
                    exp1->Process(this);
                    itm->first.append(L")", 1);
                }
                else if (_wcsicmp(oper, L"MONTH") == 0)
                {
                    processed = true;
                    itm->first.append(L"DATEPART(MONTH,", 15);
                    exp1->Process(this);
                    itm->first.append(L")", 1);
                }
                else if (_wcsicmp(oper, L"DAY") == 0)
                {
                    processed = true;
                    itm->first.append(L"DATEPART(DAY,", 13);
                    exp1->Process(this);
                    itm->first.append(L")", 1);
                }
                else if (_wcsicmp(oper, L"HOUR") == 0)
                {
                    processed = true;
                    itm->first.append(L"DATEPART(HOUR,", 14);
                    exp1->Process(this);
                    itm->first.append(L")", 1);
                }
                else if (_wcsicmp(oper, L"MINUTE") == 0)
                {
                    processed = true;
                    itm->first.append(L"DATEPART(MINUTE,", 16);
                    exp1->Process(this);
                    itm->first.append(L")", 1);
                }
                else if (_wcsicmp(oper, L"SECOND") == 0)
                {
                    processed = true;
                    itm->first.append(L"DATEPART(SECOND,", 16);
                    exp1->Process(this);
                    itm->first.append(L")", 1);
                }
            }
        }
    }
    else
    {
        pair_working_stack* itmExp0 = push_stack();
        exp0->Process(this);

        pair_working_stack* itmExp1 = push_stack();
        exp1->Process(this);

        processed = true;
        itm->first.append(L"CASE WHEN ", 10);
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'YEAR' THEN DATEPART(YEAR,", 28);
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ", 7);
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'MONTH' THEN DATEPART(MONTH,", 30);
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ", 7);
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'DAY' THEN DATEPART(DAY,", 26);
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ", 7);
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'HOUR' THEN DATEPART(HOUR,", 28);
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ", 7);
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'MINUTE' THEN DATEPART(MINUTE,", 32);
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ", 7);
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'SECOND' THEN DATEPART(SECOND,", 32);
        itm->first.append(itmExp1->first);
        itm->first.append(L") END ", 6);
        
        pop_stack(); // itmExp1
        pop_stack(); // itmExp0
    }
    if (!processed)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
}

void FdoRdbmsSqlServerSqlBuilder::ProcessExtractToDblFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    bool processed = false;
    FdoDataType dataType = GetApproxExpressionDataType(exp1);
    if (dataType != FdoDataType_DateTime)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_183_INVALID_FUNCTION_ARG, "One or more arguments for function '%1$ls' did not match the expected argument types.", FDO_FUNCTION_EXTRACTTODOUBLE));

    if (FdoExpressionItemType_DataValue == exp0->GetExpressionType())
    {
        FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
        if (dv != NULL && dv->GetDataType() == FdoDataType_String)
        {
            FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
            if (strVal != NULL && !strVal->IsNull())
            {
                FdoString* oper = strVal->GetString();
                if (_wcsicmp(oper, L"YEAR") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(YEAR,");
                    exp1->Process(this);
                    itm->first.append(L") AS REAL)");
                }
                else if (_wcsicmp(oper, L"MONTH") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(MONTH,");
                    exp1->Process(this);
                    itm->first.append(L") AS REAL)");
                }
                else if (_wcsicmp(oper, L"DAY") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(DAY,");
                    exp1->Process(this);
                    itm->first.append(L") AS REAL)");
                }
                else if (_wcsicmp(oper, L"HOUR") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(HOUR,");
                    exp1->Process(this);
                    itm->first.append(L") AS REAL)");
                }
                else if (_wcsicmp(oper, L"MINUTE") == 0)
                {
                    processed = true;
                    itm->first.append(L"CAST(DATEPART(MINUTE,");
                    exp1->Process(this);
                    itm->first.append(L") AS REAL)");
                }
                else if (_wcsicmp(oper, L"SECOND") == 0)
                {
                    pair_working_stack* itmExp1 = push_stack();
                    exp1->Process(this);

                    processed = true;
                    itm->first.append(L"(DATEPART(SECOND,");
                    itm->first.append(itmExp1->first);
                    itm->first.append(L")+DATEPART(MILLISECOND,");
                    itm->first.append(itmExp1->first);
                    itm->first.append(L")/1000.0)");

                    pop_stack(); // itmExp1
                }
            }
        }
    }
    else
    {
        pair_working_stack* itmExp0 = push_stack();
        exp0->Process(this);

        pair_working_stack* itmExp1 = push_stack();
        exp1->Process(this);

        processed = true;
        itm->first.append(L"CASE WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'YEAR' THEN DATEPART(YEAR,");
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'MONTH' THEN DATEPART(MONTH,");
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'DAY' THEN DATEPART(DAY,");
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'HOUR' THEN DATEPART(HOUR,");
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'MINUTE' THEN DATEPART(MINUTE,");
        itm->first.append(itmExp1->first);
        itm->first.append(L") WHEN ");
        itm->first.append(itmExp0->first);
        itm->first.append(L"=N'SECOND' THEN DATEPART(SECOND,");
        itm->first.append(itmExp1->first);
        itm->first.append(L")+DATEPART(MILLISECOND,");
        itm->first.append(itmExp1->first);
        itm->first.append(L")/1000.0) END ");

        pop_stack(); // itmExp1
        pop_stack(); // itmExp0
    }
    if (!processed)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
}

void FdoRdbmsSqlServerSqlBuilder::ProcessLpadFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2 && colCount != 3)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    pair_working_stack* itmExp0 = push_stack();
    exp0->Process(this);

    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    pair_working_stack* itmExp1 = push_stack();
    exp1->Process(this);

    itm->first.append(L"CASE WHEN CAST(");
    itm->first.append(itmExp1->first);
    itm->first.append(L" AS BIGINT) <= LEN(CONVERT(NVARCHAR,");
    itm->first.append(itmExp0->first);
    itm->first.append(L")) THEN LEFT(CONVERT(NVARCHAR,");
    itm->first.append(itmExp0->first);
    itm->first.append(L"),CAST(");
    itm->first.append(itmExp1->first);
    if (colCount == 2)
    {
        itm->first.append(L" AS BIGINT)) ELSE LEFT(REPLICATE(N' ',CAST(");
        itm->first.append(itmExp1->first);
        itm->first.append(L" AS BIGINT)), CAST(");
        itm->first.append(itmExp1->first);
        itm->first.append(L" AS BIGINT)-LEN(CONVERT(NVARCHAR,");
        itm->first.append(itmExp0->first);
        itm->first.append(L"))) + CONVERT(NVARCHAR,");
        itm->first.append(itmExp0->first);
        itm->first.append(L") END");
    }
    else
    {
        FdoPtr<FdoExpression> exp2 = exprCol->GetItem(2);
        pair_working_stack* itmExp2 = push_stack();
        exp2->Process(this);

        itm->first.append(L" AS BIGINT)) ELSE LEFT(REPLICATE(CONVERT(NVARCHAR,");
        itm->first.append(itmExp2->first);
        itm->first.append(L"),CAST(");
        itm->first.append(itmExp1->first);
        itm->first.append(L" AS BIGINT)), CAST(");
        itm->first.append(itmExp1->first);
        itm->first.append(L" AS BIGINT)-LEN(CONVERT(NVARCHAR,");
        itm->first.append(itmExp0->first);
        itm->first.append(L"))) + CONVERT(NVARCHAR,");
        itm->first.append(itmExp0->first);
        itm->first.append(L") END");

        pop_stack(); // itmExp2
    }

    pop_stack(); // itmExp1
    pop_stack(); // itmExp0
}

void FdoRdbmsSqlServerSqlBuilder::ProcessRpadFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2 && colCount != 3)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    pair_working_stack* itmExp0 = push_stack();
    exp0->Process(this);
    
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    pair_working_stack* itmExp1 = push_stack();
    exp1->Process(this);

    if (colCount == 2)
    {
        itm->first.append(L"LEFT(CONVERT(NVARCHAR,");
        itm->first.append(itmExp0->first);
        itm->first.append(L")+REPLICATE(N' ',CAST(");
        itm->first.append(itmExp1->first);
        itm->first.append(L" AS BIGINT)),CAST(");
        itm->first.append(itmExp1->first);
        itm->first.append(L" AS BIGINT))");
    }
    else
    {
        FdoPtr<FdoExpression> exp2 = exprCol->GetItem(2);
        pair_working_stack* itmExp2 = push_stack();
        exp2->Process(this);

        itm->first.append(L"LEFT(CONVERT(NVARCHAR,");
        itm->first.append(itmExp0->first);
        itm->first.append(L")+REPLICATE(CONVERT(NVARCHAR,");
        itm->first.append(itmExp2->first);
        itm->first.append(L"),CAST(");
        itm->first.append(itmExp1->first);
        itm->first.append(L" AS BIGINT)),CAST(");
        itm->first.append(itmExp1->first);
        itm->first.append(L" AS BIGINT))");
        
        pop_stack(); // itmExp2
    }

    pop_stack(); // itmExp1
    pop_stack(); // itmExp0
}

void FdoRdbmsSqlServerSqlBuilder::ProcessRemainderFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    pair_working_stack* itmExp0 = push_stack();
    exp0->Process(this);

    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    pair_working_stack* itmExp1 = push_stack();
    exp1->Process(this);

    itm->first.append(L"ABS(");
    itm->first.append(itmExp0->first);
    itm->first.append(L")-ABS(");
    itm->first.append(itmExp1->first);
    itm->first.append(L")*(CASE WHEN CAST(ABS(");
    itm->first.append(itmExp0->first);
    itm->first.append(L") AS REAL)/CAST(ABS(");
    itm->first.append(itmExp1->first);
    itm->first.append(L") AS REAL)-FLOOR(CAST(ABS(");
    itm->first.append(itmExp0->first);
    itm->first.append(L") AS REAL)/CAST(ABS(");
    itm->first.append(itmExp1->first);
    itm->first.append(L") AS REAL))<=0.5 THEN FLOOR(CAST(ABS(");
    itm->first.append(itmExp0->first);
    itm->first.append(L") AS REAL)/CAST(ABS(");
    itm->first.append(itmExp1->first);
    itm->first.append(L") AS REAL)) ELSE CEILING(CAST(ABS(");
    itm->first.append(itmExp0->first);
    itm->first.append(L") AS REAL)/CAST(ABS(");
    itm->first.append(itmExp1->first);
    itm->first.append(L") AS REAL)) END) *(CASE WHEN ");
    itm->first.append(itmExp0->first);
    itm->first.append(L"<0 THEN -1.0 ELSE 1.0 END)");

    pop_stack(); // itmExp1
    pop_stack(); // itmExp0
}

void FdoRdbmsSqlServerSqlBuilder::ProcessTrimFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> mainexp = exprCol->GetItem(0);
    if (colCount == 1)
    {
        itm->first.append(L"LTRIM(RTRIM(CONVERT(NVARCHAR,");
        mainexp->Process(this);
        itm->first.append(L")))");
    }
    else
    {
        bool processed = false;
        FdoPtr<FdoExpression> exp = exprCol->GetItem(0);
        mainexp = exprCol->GetItem(1);
        if (FdoExpressionItemType_DataValue == exp->GetExpressionType())
        {
            FdoDataValue* dv = static_cast<FdoDataValue*>(exp.p);
            if (dv != NULL && dv->GetDataType() == FdoDataType_String)
            {
                FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                if (strVal != NULL && !strVal->IsNull())
                {
                    pair_working_stack* itmMainExp = push_stack();
                    mainexp->Process(this);

                    FdoString* oper = strVal->GetString();
                    if (_wcsicmp(oper, L"BOTH") == 0)
                    {
                        processed = true;
                        itm->first.append(L"LTRIM(RTRIM(CONVERT(NVARCHAR,");
                        itm->first.append(itmMainExp->first);
                        itm->first.append(L")))");
                    }
                    else if (_wcsicmp(oper, L"LEADING") == 0)
                    {
                        processed = true;
                        itm->first.append(L"LTRIM(CONVERT(NVARCHAR,");
                        itm->first.append(itmMainExp->first);
                        itm->first.append(L"))");
                    }
                    else if (_wcsicmp(oper, L"TRAILING") == 0)
                    {
                        processed = true;
                        itm->first.append(L"RTRIM(CONVERT(NVARCHAR,");
                        itm->first.append(itmMainExp->first);
                        itm->first.append(L"))");
                    }

                    pop_stack(); // itmMainExp
                }
            }
        }
        else
        {
            pair_working_stack* itmMainExp = push_stack();
            mainexp->Process(this);
            pair_working_stack* itmExp = push_stack();
            exp->Process(this);

            processed = true;
            itm->first.append(L"CASE WHEN ");
            itm->first.append(itmExp->first);
            itm->first.append(L"=N'BOTH' THEN LTRIM(RTRIM(CONVERT(NVARCHAR,");
            itm->first.append(itmMainExp->first);
            itm->first.append(L"))) WHEN ");
            itm->first.append(itmExp->first);
            itm->first.append(L"=N'LEADING' THEN LTRIM(CONVERT(NVARCHAR,");
            itm->first.append(itmMainExp->first);
            itm->first.append(L")) WHEN ");
            itm->first.append(itmExp->first);
            itm->first.append(L"=N'TRAILING' THEN RTRIM(CONVERT(NVARCHAR,");
            itm->first.append(itmMainExp->first);
            itm->first.append(L")) END");

            pop_stack(); // itmExp
            pop_stack(); // itmMainExp
        }
        if (!processed)
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }
}

void FdoRdbmsSqlServerSqlBuilder::ProcessSubStrFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2 && colCount != 3)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    pair_working_stack* itmExp0 = push_stack();
    exp0->Process(this);
    
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    pair_working_stack* itmExp1 = push_stack();
    exp1->Process(this);

    itm->first.append(L"(SUBSTRING(");
    itm->first.append(itmExp0->first);
    itm->first.append(L",(CASE WHEN CAST(");
    itm->first.append(itmExp1->first);
    itm->first.append(L" AS BIGINT)=0 THEN 1 WHEN CAST(");
    itm->first.append(itmExp1->first);
    itm->first.append(L" AS BIGINT)<0 THEN (CASE WHEN LEN(CONVERT(NVARCHAR,");
    itm->first.append(itmExp0->first);
    itm->first.append(L"))+CAST(");
    itm->first.append(itmExp1->first);
    itm->first.append(L" AS BIGINT)<0 THEN LEN(CONVERT(NVARCHAR,");
    itm->first.append(itmExp0->first);
    itm->first.append(L"))+1 ELSE LEN(CONVERT(NVARCHAR,");
    itm->first.append(itmExp0->first);
    itm->first.append(L"))+1+CAST(");
    itm->first.append(itmExp1->first);
    itm->first.append(L" AS BIGINT) END) ELSE CAST(");
    itm->first.append(itmExp1->first);
    if (colCount == 3)
    {
        FdoPtr<FdoExpression> exp2 = exprCol->GetItem(2);
        pair_working_stack* itmExp2 = push_stack();
        exp2->Process(this);

        itm->first.append(L" AS BIGINT) END) ,(CASE WHEN CAST(");
        itm->first.append(itmExp2->first);
        itm->first.append(L" AS BIGINT)<0 THEN 0 ELSE CAST(");
        itm->first.append(itmExp2->first);
        itm->first.append(L" AS BIGINT) END)))");
        pop_stack(); // itmExp2
    }
    else
    {
        itm->first.append(L" AS BIGINT) END), LEN(CONVERT(NVARCHAR,");
        itm->first.append(itmExp0->first);
        itm->first.append(L"))))");
    }
    pop_stack(); // itmExp1
    pop_stack(); // itmExp0
}

void FdoRdbmsSqlServerSqlBuilder::ProcessInstrFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    itm->first.append(L"(CHARINDEX(CONVERT(NVARCHAR,");
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    exp1->Process(this);
    itm->first.append(L"),CONVERT(NVARCHAR,");
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    exp0->Process(this);
    itm->first.append(L")))");
}

void FdoRdbmsSqlServerSqlBuilder::ProcessTruncFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> mainexp = exprCol->GetItem(0);
    if (colCount == 1)
    {
        itm->first.append(L"CAST(CAST(");
        mainexp->Process(this);
        itm->first.append(L" AS BIGINT) AS REAL)");
    }
    else
    {
        FdoDataType dataType = GetApproxExpressionDataType(mainexp);
        if (dataType == FdoDataType_DateTime)
        {
            bool processed = false;
            FdoPtr<FdoExpression> exp = exprCol->GetItem(1);
            if (FdoExpressionItemType_DataValue == exp->GetExpressionType())
            {
                FdoDataValue* dv = static_cast<FdoDataValue*>(exp.p);
                if (dv != NULL && dv->GetDataType() == FdoDataType_String)
                {
                    FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                    if (strVal != NULL && !strVal->IsNull())
                    {
                        pair_working_stack* itmMainExp = push_stack();
                        mainexp->Process(this);

                        FdoString* oper = strVal->GetString();
                        if (_wcsicmp(oper, L"YEAR") == 0)
                        {
                            processed = true;
                            itm->first.append(L"CAST(DATEPART(YEAR,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N'-01-01 00:00:00.000'");
                        }
                        else if (_wcsicmp(oper, L"MONTH") == 0)
                        {
                            processed = true;
                            itm->first.append(L"CAST(DATEPART(YEAR,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(MONTH,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N'-01 00:00:00.000'");
                        }
                        else if (_wcsicmp(oper, L"DAY") == 0)
                        {
                            processed = true;
                            itm->first.append(itmMainExp->first);
                        }
                        else if (_wcsicmp(oper, L"HOUR") == 0)
                        {
                            processed = true;
                            itm->first.append(L"CAST(DATEPART(YEAR,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(MONTH,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(DAY,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N' ' + CAST(DATEPART(HOUR,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N':00:00.000'");
                        }
                        else if (_wcsicmp(oper, L"MINUTE") == 0)
                        {
                            processed = true;
                            itm->first.append(L"CAST(DATEPART(YEAR,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(MONTH,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(DAY,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N' ' + CAST(DATEPART(HOUR,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N':' + CAST(DATEPART(MINUTE,");
                            itm->first.append(itmMainExp->first);
                            itm->first.append(L") AS NVARCHAR) + N':00.000'");
                        }
                        pop_stack(); // itmMainExp
                    }
                }
            }
            else
            {
                dataType = GetApproxExpressionDataType(mainexp);
                //propType = FdoPropertyType_DataProperty;
                //dataType = FdoDataType_Int32;
                //FdoCommonMiscUtil::GetExpressionType(functions, clsDef, mainexp, propType, dataType);
                if (dataType == FdoDataType_String)
                {
                    pair_working_stack* itmMainExp = push_stack();
                    mainexp->Process(this);
                    
                    pair_working_stack* itmExp = push_stack();
                    exp->Process(this);

                    itm->first.append(L"CASE WHEN ");
                    itm->first.append(itmExp->first);
                    itm->first.append(L"=N'YEAR' THEN CAST(DATEPART(YEAR,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N'-01-01 00:00:00.000' WHEN ");
                    itm->first.append(itmExp->first);
                    itm->first.append(L"=N'MONTH' THEN CAST(DATEPART(YEAR,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(MONTH,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N'-01 00:00:00.000'  WHEN ");
                    itm->first.append(itmExp->first);
                    itm->first.append(L"=N'DAY' THEN ");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L" WHEN ");
                    itm->first.append(itmExp->first);
                    itm->first.append(L"=N'HOUR' THEN CAST(DATEPART(YEAR,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(MONTH,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(DAY,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N' ' + CAST(DATEPART(HOUR,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N':00:00.000' WHEN ");
                    itm->first.append(itmExp->first);
                    itm->first.append(L"=N'MINUTE' THEN CAST(DATEPART(YEAR,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(MONTH,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N'-' + CAST(DATEPART(DAY,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N' ' + CAST(DATEPART(HOUR,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N':' + CAST(DATEPART(MINUTE,");
                    itm->first.append(itmMainExp->first);
                    itm->first.append(L") AS NVARCHAR) + N':00.000' END ");

                    pop_stack(); // itmExp
                    pop_stack(); // itmMainExp
                }
            }
            if (!processed)
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        }
        else
        {
            itm->first.append(L"ROUND(");
            mainexp->Process(this);
            itm->first.append(L", CAST(");
            FdoPtr<FdoExpression> exp = exprCol->GetItem(1);
            exp->Process(this);
            itm->first.append(L" AS BIGINT), 1)");
        }
    }
}

void FdoRdbmsSqlServerSqlBuilder::ProcessModFunction(FdoFunction& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    pair_working_stack* itmExp0 = push_stack();
    exp0->Process(this);
    pair_working_stack* itmExp1 = push_stack();
    exp1->Process(this);

    itm->first.append(L"CAST(ABS(");
    itm->first.append(itmExp0->first);
    itm->first.append(L") AS REAL)-ABS(");
    itm->first.append(itmExp1->first);
    itm->first.append(L")*FLOOR(CAST(ABS(");
    itm->first.append(itmExp0->first);
    itm->first.append(L") AS REAL)/ABS(");
    itm->first.append(itmExp1->first);
    itm->first.append(L"))*(CASE WHEN ");
    itm->first.append(itmExp0->first);
    itm->first.append(L"<0 THEN -1.0 ELSE 1.0 END)");
    
    pop_stack(); // itmExp1
    pop_stack(); // itmExp0
}

//--------------------------------------------------------------------
// Expressions & Filters
//--------------------------------------------------------------------
void FdoRdbmsSqlServerSqlBuilder::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpression> param = expr.GetLeftExpression();
    itm->first.append(L"(", 1);
    param->Process(this);
    switch(expr.GetOperation())
    {
    case FdoBinaryOperations_Add:
        itm->first.append(L"+", 1);
        break;
    case FdoBinaryOperations_Subtract:
        itm->first.append(L"-", 1);
        break;
    case FdoBinaryOperations_Multiply:
        itm->first.append(L"*", 1);
        break;
    case FdoBinaryOperations_Divide:
        itm->first.append(L"/", 1);
        break;
    }
    param = expr.GetRightExpression();
    param->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    pair_working_stack* itm = top_stack();

    itm->first.append(L" (-(", 4);
    FdoPtr<FdoExpression> param = expr.GetExpression();
    param->Process(this);
    itm->first.append(L"))", 2);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessParameter(FdoParameter& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoParameterValue> pVal = (mParams == NULL) ? NULL : mParams->FindItem(expr.GetName());
    if( pVal == NULL )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoLiteralValue> lv = pVal->GetValue();
    mUsedParameterValues.push_back(std::make_pair(lv.p, 0));

    itm->first.append(L"?", 1);
}
void FdoRdbmsSqlServerSqlBuilder::ProcessBooleanValue(FdoBooleanValue& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {
        if (expr.GetBoolean())
            itm->first.append(L"1", 1);
        else
            itm->first.append(L"0", 1);
    }
    else
        itm->first.append(L"null", 4);
}
void FdoRdbmsSqlServerSqlBuilder::ProcessByteValue(FdoByteValue& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {
        swprintf(m_useConv, CONV_MEM_BLOCK_SIZE, L"%d", (int)expr.GetByte());
        itm->first.append(m_useConv, wcslen(m_useConv));
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {
        itm->first.append(L"\'", 1);
        itm->first.append(m_fdoConn->FdoToDbiTime(expr.GetDateTime(), m_useConv, CONV_MEM_BLOCK_SIZE));
        itm->first.append(L"\'", 1);
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessDecimalValue(FdoDecimalValue& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {
        swprintf(m_useConv, CONV_MEM_BLOCK_SIZE, L"%.16g", expr.GetDecimal());
        itm->first.append(m_useConv, wcslen(m_useConv));
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessDoubleValue(FdoDoubleValue& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {
        swprintf(m_useConv, CONV_MEM_BLOCK_SIZE, L"%.16g", expr.GetDouble());
        itm->first.append(m_useConv, wcslen(m_useConv));
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessInt16Value(FdoInt16Value& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {

        swprintf(m_useConv, CONV_MEM_BLOCK_SIZE, L"%d", (int)expr.GetInt16());
        itm->first.append(m_useConv, wcslen(m_useConv));
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessInt32Value(FdoInt32Value& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {

        swprintf(m_useConv, CONV_MEM_BLOCK_SIZE, L"%d", (int)expr.GetInt32());
        itm->first.append(m_useConv, wcslen(m_useConv));
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessInt64Value(FdoInt64Value& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {
#ifdef _WIN32
        _i64tow_s(expr.GetInt64(), m_useConv, CONV_MEM_BLOCK_SIZE, 10);
#else
        swprintf(m_useConv, CONV_MEM_BLOCK_SIZE, L"%lld", (long long int)expr.GetInt64());
#endif
        itm->first.append(m_useConv, wcslen(m_useConv));
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessSingleValue(FdoSingleValue& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {
        swprintf(m_useConv, CONV_MEM_BLOCK_SIZE, L"%.8f", expr.GetSingle());
        itm->first.append(m_useConv, wcslen(m_useConv));
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessStringValue(FdoStringValue& expr)
{
    pair_working_stack* itm = top_stack();

    if (!expr.IsNull())
    {
        itm->first.append(L"N", 1);
        itm->first.append(expr.ToString());
    }
    else
        itm->first.append(L"null", 4);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessBLOBValue(FdoBLOBValue& expr)
{
    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
}

void FdoRdbmsSqlServerSqlBuilder::ProcessCLOBValue(FdoCLOBValue& expr)
{
    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
}

void FdoRdbmsSqlServerSqlBuilder::ProcessGeometryValue(FdoGeometryValue& expr)
{
    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
}

void FdoRdbmsSqlServerSqlBuilder::ProcessDistanceCondition(FdoDistanceCondition& filter)
{
    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
}

void FdoRdbmsSqlServerSqlBuilder::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoFilter> left = filter.GetLeftOperand();
    FdoPtr<FdoFilter> right = filter.GetRightOperand();
    FdoBinaryLogicalOperations op = filter.GetOperation();
    bool addLeftBr = false;
    bool addRightBr = false;

    if (op == FdoBinaryLogicalOperations_And)
    {
        FdoBinaryLogicalOperator* pBLO = dynamic_cast<FdoBinaryLogicalOperator*>(right.p);
        if (pBLO != NULL && pBLO->GetOperation() == FdoBinaryLogicalOperations_Or)
            addRightBr = true;

        pBLO = dynamic_cast<FdoBinaryLogicalOperator*>(left.p);
        if (pBLO != NULL && pBLO->GetOperation() == FdoBinaryLogicalOperations_Or)
            addLeftBr = false;
    }
    
    if (addLeftBr)
    {
        itm->first.append(L"(", 1);
        left->Process(this);
        itm->first.append(L")", 1);
    }
    else
        left->Process(this);

    if (op == FdoBinaryLogicalOperations_And)
        itm->first.append(L" AND ", 5);
    else
        itm->first.append(L" OR ", 4);

    if (addRightBr)
    {
        itm->first.append(L"(", 1);
        right->Process(this);
        itm->first.append(L")", 1);
    }
    else
        right->Process(this);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoFilter> arg = filter.GetOperand();
    itm->first.append(L"NOT (", 5);
    arg->Process(this);
    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoExpression> left = filter.GetLeftExpression();
    FdoPtr<FdoExpression> right = filter.GetRightExpression();

    left->Process(this);
    switch(filter.GetOperation())
    {
    case FdoComparisonOperations_EqualTo:
        itm->first.append(L"=", 1);
        break;
    case FdoComparisonOperations_NotEqualTo:
        itm->first.append(L"<>", 2);
        break;
    case FdoComparisonOperations_GreaterThan:
        itm->first.append(L">", 1);
        break;
    case FdoComparisonOperations_GreaterThanOrEqualTo:
        itm->first.append(L">=", 2);
        break;
    case FdoComparisonOperations_LessThan:
        itm->first.append(L"<", 1);
        break;
    case FdoComparisonOperations_LessThanOrEqualTo:
        itm->first.append(L"<=", 2);
        break;
    case FdoComparisonOperations_Like:
        itm->first.append(L" LIKE ", 6);
        break;
    }
    right->Process(this);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessInCondition(FdoInCondition& filter)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoIdentifier> prop = filter.GetPropertyName();
    if( prop == NULL )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_248, "%1$ls is missing the property name", L"FdoInCondition"));

    prop->Process(this);
    itm->first.append(L" IN(", 4);
    size_t szBefore = itm->first.size();

    FdoPtr<FdoValueExpressionCollection> vals = filter.GetValues();
    int cnt = vals->GetCount();
    for(int idx = 0; idx < cnt; idx++)
    {
        FdoPtr<FdoValueExpression> exp = vals->GetItem(idx);
        if (exp->GetExpressionType() == FdoExpressionItemType_SubSelectExpression && cnt > 1)
            throw FdoException::Create(L"Unsupported FDO type in filters");
        exp->Process(this);
        itm->first.append(L",", 1);
    }
    if (!cnt)
    {
        FdoPtr<FdoSubSelectExpression> subsel = filter.GetSubSelect();
        subsel->Process(this);
    }
    else
        itm->first.resize(itm->first.size()-1); // remove the last ','

    if( szBefore == itm->first.size())
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_249, "FdoInCondition has an empty value list"));

    itm->first.append(L")", 1);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessNullCondition(FdoNullCondition& filter)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoIdentifier> prop = filter.GetPropertyName();
    if( prop == NULL )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_248, "%1$ls is missing the property name", L"FdoNullCondition"));

    prop->Process(this);
    itm->first.append(L" IS NULL", 8);
}

void FdoRdbmsSqlServerSqlBuilder::ProcessFunction(FdoFunction& expr)
{
    if (m_functions.size() == 0)
        LoadFunctionDefinitions();
    function_map::const_iterator val = m_functions.find(expr.GetName());
    if (val == m_functions.end())
    {
        pair_working_stack* itm = top_stack();
        itm->first.append(L"dbo.", 4);
        itm->first.append(expr.GetName());
        itm->first.append(L"(", 1);
        FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
        FdoInt32 colCount = exprCol->GetCount();
        for (int i = 0; i < colCount; i++)
        {
            FdoPtr<FdoExpression>exp = exprCol->GetItem(i);
            exp->Process(this);
            itm->first.append(L",", 1);
        }
        if (colCount)
            itm->first.resize(itm->first.size()-1); // remove last ','
        itm->first.append(L")", 1);
    }
    else // we found our function
    {
        switch(val->second)
        {
        // ABS(), ACOS(), ASIN(), ATAN(), COS(), EXP(), SIN(), SQRT(), TAN()
        // FLOOR(), CEIL(), UPPER(), LOWER(), LTRIM(), RTRIM(), SOUNDEX()
        case IDX_FDO_FUNCTION_COMMON:
            ProcessCommonFunction(expr);
            break;
        case IDX_FDO_FUNCTION_CEIL:
            ProcessCeilFunction(expr);
            break;            
        case IDX_FDO_FUNCTION_ROUND:
            ProcessRoundFunction(expr);
            break;
        case IDX_FDO_FUNCTION_SIGN:
            ProcessSignFunction(expr);
            break;
        case IDX_FDO_FUNCTION_TRUNC:
            ProcessTruncFunction(expr);
            break;
        case IDX_FDO_FUNCTION_ATAN2:
            ProcessAtn2Function(expr);
            break;
        case IDX_FDO_FUNCTION_LN:
            ProcessLnFunction(expr);
            break;
        case IDX_FDO_FUNCTION_LOG:
            ProcessLogFunction(expr);
            break;
        case IDX_FDO_FUNCTION_MOD:
            ProcessModFunction(expr);
            break;
        case IDX_FDO_FUNCTION_POWER:
            ProcessPowerFunction(expr);
            break;
        case IDX_FDO_FUNCTION_REMAINDER:
            ProcessRemainderFunction(expr);
            break;
        case IDX_FDO_FUNCTION_AREA2D:
            ProcessArea2dFunction(expr);
            break;
        case IDX_FDO_FUNCTION_LENGTH2D:
            ProcessLength2dFunction(expr);
            break;
        case IDX_FDO_FUNCTION_X:
            ProcessXFunction(expr);
            break;
        case IDX_FDO_FUNCTION_Y:
            ProcessYFunction(expr);
            break;
        case IDX_FDO_FUNCTION_ZM:
            ProcessZMFunction(expr);
            break;
        case IDX_FDO_FUNCTION_ISVALID:
            ProcessIsValidFunction(expr);
            break;
        case IDX_FDO_FUNCTION_ADDMONTHS:
            ProcessAddMonthsFunction(expr);
            break;
        case IDX_FDO_FUNCTION_CURRENTDATE:
            ProcessCurrentDateFunction(expr);
            break;
        case IDX_FDO_FUNCTION_EXTRACT:
            ProcessExtractFunction(expr);
            break;
        case IDX_FDO_FUNCTION_EXTRACTTODOUBLE:
            ProcessExtractToDblFunction(expr);
            break;
        case IDX_FDO_FUNCTION_EXTRACTTOINT:
            ProcessExtractToIntFunction(expr);
            break;
        case IDX_FDO_FUNCTION_MONTHSBETWEEN:
            ProcessMonthsBetweenFunction(expr);
            break;
        case IDX_FDO_FUNCTION_NULLVALUE:
            ProcessNullValueFunction(expr);
            break;
        case IDX_FDO_FUNCTION_TODATE:
            ProcessToDateFunction(expr);
            break;
        case IDX_FDO_FUNCTION_TODOUBLE:
            ProcessToDoubleFunction(expr);
            break;
        case IDX_FDO_FUNCTION_TOFLOAT:
            ProcessToFloatFunction(expr);
            break;
        case IDX_FDO_FUNCTION_TOINT32:
            ProcessToInt32Function(expr);
            break;
        case IDX_FDO_FUNCTION_TOINT64:
            ProcessToInt64Function(expr);
            break;
        case IDX_FDO_FUNCTION_TOSTRING:
            ProcessToStringFunction(expr);
            break;
        case IDX_FDO_FUNCTION_CONCAT:
            ProcessConcatFunction(expr);
            break;
        case IDX_FDO_FUNCTION_INSTR:
            ProcessInstrFunction(expr);
            break;
        case IDX_FDO_FUNCTION_LENGTH:
            ProcessLengthFunction(expr);
            break;
        case IDX_FDO_FUNCTION_LPAD:
            ProcessLpadFunction(expr);
            break;
        case IDX_FDO_FUNCTION_RPAD:
            ProcessRpadFunction(expr);
            break;
        case IDX_FDO_FUNCTION_SUBSTR:
            ProcessSubStrFunction(expr);
            break;
        case IDX_FDO_FUNCTION_TRIM:
            ProcessTrimFunction(expr);
            break;
        case IDX_FDO_FUNCTION_MEDIAN:
            ProcessMedianFunction(expr);
            break;
        case IDX_FDO_FUNCTION_AGG:
            ProcessAggregateFunction(expr);
            break;
        case IDX_FDO_FUNCTION_SPATIALEXTENTS:
            ProcessSpatialExtentsFunction(expr);
            break;
        }
    }
}

void FdoRdbmsSqlServerSqlBuilder::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    pair_working_stack* itm = top_stack();
    
    FdoPtr<FdoIdentifier> propName = filter.GetPropertyName();
    pair_working_stack* itmPropExp = push_stack();
    m_useBr = true;
    propName->Process(this);
    m_useBr = false;

    FdoPtr<FdoExpression> geomExpr = filter.GetGeometry();
    if (FdoExpressionItemType_GeometryValue != geomExpr->GetExpressionType())
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoGeometryValue *geom = static_cast<FdoGeometryValue*>(geomExpr.p);
    FdoSpatialOperations spatialOp = filter.GetOperation();

    itm->first.append(itmPropExp->first);
    itm->first.append(L".", 1);

    // Geography type does not have an STIsValid() function.
    if (SqlGeometryType_Geometry == m_lastGtype && spatialOp != FdoSpatialOperations_EnvelopeIntersects) 
    {
        // Skip the invalid geometries otherwise any spatial query will fail.
        // The user should run "Select * where IsValid() = 0" in order to find out the offending geometries.
        itm->first.append(L"STIsValid()=1 AND ", 18);
        itm->first.append(itmPropExp->first);
        itm->first.append(L".", 1);
    }

    if (SqlGeometryType_Geography == m_lastGtype)
    {
		// Some spatial operations are not supported by SqlServer 2008 (version 10) but
		// supported by SqlServer 2012 (version 11).
		FdoSmPhSqsMgrP sqsMgr = m_fdoConn->GetSchemaManager()->GetPhysicalSchema()->SmartCast<FdoSmPhSqsMgr>();
		bool bVersionSupported = ( sqsMgr->GetDbVersion().Left( L"." ).ToLong() > 10 );
		FdoStringP bufOp(L"");

        switch (spatialOp)
        {
            case FdoSpatialOperations_Disjoint:
                bufOp += L"STDisjoint";
                break;
            case FdoSpatialOperations_Intersects:
                bufOp += L"STIntersects";
                break;
            case FdoSpatialOperations_Inside:
				if (bVersionSupported)		
					bufOp += L"STWithin";
				break;
            case FdoSpatialOperations_Equals:
				if (bVersionSupported)
					bufOp += L"STEquals";
                break;
            case FdoSpatialOperations_Overlaps:
				if (bVersionSupported)
					bufOp += L"STOverlaps";
                break;
            case FdoSpatialOperations_Within:
				if (bVersionSupported)
					bufOp += L"STWithin";
                break;
            case FdoSpatialOperations_CoveredBy:
				if (bVersionSupported)
					bufOp += L"STOverlaps";
                break;
            case FdoSpatialOperations_Crosses:
            case FdoSpatialOperations_Touches:
            case FdoSpatialOperations_EnvelopeIntersects:
                break; // non-supported operation exception will be thrown below
            default:
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_111, "Unsupported spatial operation"));
        }

		if (bufOp.GetLength() > 0)
			itm->first.append(bufOp);
		else
            throw FdoFilterException::Create(
                NlsMsgGet2(
                    FDORDBMS_44, 
                    "Geometry property '%1$ls' has geodetic coordinate system; cannot use %2$ls spatial operator in filter",
                    (FdoString*) propName->GetText(),
                    (FdoString*) FdoCommonMiscUtil::FdoSpatialOperationsToString(spatialOp)
                )
            );
    }
    else
    {
        switch (spatialOp)
        {
            case FdoSpatialOperations_Contains:
                itm->first.append(L"STContains");
                break;
            case FdoSpatialOperations_Crosses:
                itm->first.append(L"STCrosses");
                break;
            case FdoSpatialOperations_Disjoint:
                itm->first.append(L"STDisjoint");
                break;
            case FdoSpatialOperations_Equals:
                itm->first.append(L"STEquals");
                break;
            case FdoSpatialOperations_Intersects:
                itm->first.append(L"STIntersects");
                break;
            case FdoSpatialOperations_Overlaps:
                itm->first.append(L"STOverlaps");
                break;
            case FdoSpatialOperations_Touches:
                itm->first.append(L"STTouches");
                break;
            case FdoSpatialOperations_Within:
                itm->first.append(L"STWithin");
                break;
            case FdoSpatialOperations_Inside:
                itm->first.append(L"STWithin");
                break;
            case FdoSpatialOperations_CoveredBy:
                itm->first.append(L"STOverlaps");
                break;
            case FdoSpatialOperations_EnvelopeIntersects:
                itm->first.append(L"STIntersects(convert(geometry, ?).STEnvelope())=1");
                break;
            default:
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_111, "Unsupported spatial operation"));
        }
    }
    mUsedParameterValues.push_back(std::make_pair(geom, m_lastSrid));
    if (spatialOp != FdoSpatialOperations_EnvelopeIntersects)
        itm->first.append(L"(?)=1", 5);

    pop_stack(); // itmPropExp
}

void FdoRdbmsSqlServerSqlBuilder::ProcessIdentifier(FdoIdentifier& expr)
{
    pair_working_stack* itm = top_stack();

    DbiConnection  *dbiConn = m_fdoConn->GetDbiConnection();
    const FdoSmLpPropertyDefinition* propertyDefinition = NULL;
    sel_class_alias::iterator clsInfo;

    FdoInt64 idxSchema = -1;
    FdoInt64 idxScope = -1;
    FdoString* propQName = expr.GetText();
    FdoString* tmp = propQName;
    while(*tmp != L'\0')
    {
        if (*tmp == L':')
        {
            if (idxSchema != -1)
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
            idxSchema = (tmp-propQName); // pos of ':'
        }
        else if (*tmp == L'.')
        {
            if (idxScope != -1)
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
            idxScope = (tmp-propQName); // pos of '.'
        }
        tmp++;
    }
    // we cannot have schema without scope!
    if (idxSchema != -1 && idxScope == -1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    // since we do not have schema or scope it must be the main class
    if (idxSchema == -1)
    {
        if (idxScope == -1)
        {
            // this could be a calculation
            FdoString* calcTxt = (m_hasClaculations) ? FindCalculation(propQName) : NULL;
            if (calcTxt == NULL || *calcTxt == '\0')
            {
                if (m_tempUsedClasses.size() != 0)
                {
                    clsInfo = m_tempUsedClasses.begin() + (m_tempUsedClasses.size()-1);
                    propertyDefinition = clsInfo->first->RefProperties()->RefItem(propQName);
                }
                if (propertyDefinition == NULL)
                {
                    clsInfo = m_usedClasses.begin();
                    propertyDefinition = clsInfo->first->RefProperties()->RefItem(propQName);
                }
            }
            else
            {
                itm->first.append(L"(", 1);
                itm->first.append(calcTxt);
                itm->first.append(L")", 1);
                return;
            }
        }
        else // scope can be the alias or the class name
        {
            std::wstring scopeName (propQName, (size_t)idxScope);
            std::wstring propName (propQName, (size_t)(idxScope+1), -1);
            if (m_tempUsedClasses.size())
            {
                clsInfo = m_tempUsedClasses.begin() + (m_tempUsedClasses.size()-1);
                if (clsInfo->second == scopeName)
                    propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                else if (scopeName == clsInfo->first->GetName())
                    propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
            }
            if (propertyDefinition == NULL)
            {
                for (size_t idx = 0; idx < m_usedClasses.size(); idx++)
                {
                    clsInfo = m_usedClasses.begin()+idx;
                    if (clsInfo->second == scopeName)
                    {
                        propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                        break;
                    }
                    else if (scopeName == clsInfo->first->GetName())
                    {
                        propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                        break;
                    }
                }
            }
        }
    }
    else // schema:class.property? rare case, however let's try to handle it
    {
        std::wstring clsName (propQName, (size_t)idxScope);
        std::wstring propName (propQName, (size_t)(idxScope+1), -1);
        const FdoSmLpClassDefinition* clsDef = dbiConn->GetSchemaUtil()->GetClass(clsName.c_str());
        if (clsDef != NULL)
        {
            if (m_tempUsedClasses.size())
            {
                clsInfo = m_tempUsedClasses.begin() + (m_tempUsedClasses.size()-1);
                if (clsDef == clsInfo->first)
                    propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
            }
            if (propertyDefinition == NULL)
            {
                for (size_t idx = 0; idx < m_usedClasses.size(); idx++)
                {
                    clsInfo = m_usedClasses.begin()+idx;
                    if (clsDef == clsInfo->first)
                    {
                        propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                        break;
                    }
                }
            }
        }
    }

    if (propertyDefinition == NULL)
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_28, "Property '%1$ls' is not found", expr.GetName() ));

    switch (propertyDefinition->GetPropertyType())
    {
        case FdoPropertyType_DataProperty:
            {
                const FdoSmLpDataPropertyDefinition* dataProp = static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                FdoStringP tableName = dbiConn->GetSchemaUtil()->GetDbObjectSqlName(clsInfo->first);
                if (clsInfo->second.size() != 0)
                    itm->first.append(clsInfo->second);
                else
                    itm->first.append((FdoString*)tableName);
                itm->first.append(L".", 1);
                itm->first.append((FdoString*)(dbiConn->GetSchemaUtil()->GetColumnSqlName(dataProp)));
            }
            break;
        case FdoPropertyType_GeometricProperty:
            {
                const FdoSmLpGeometricPropertyDefinition* geomProp = static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);
                FdoStringP tableName = dbiConn->GetSchemaUtil()->GetDbObjectSqlName(clsInfo->first);
                if (clsInfo->second.size() != 0)
                    itm->first.append(clsInfo->second);
                else
                    itm->first.append((FdoString*)tableName);
                itm->first.append(L".", 1);
                FdoStringP cName = dbiConn->GetSchemaUtil()->GetColumnSqlName(geomProp);
                if (m_useBr)
                {
                    itm->first.append(L"[", 1);
                    itm->first.append((FdoString*)cName, 1, cName.GetLength()-2);
                    itm->first.append(L"]", 1);
                }
                else
                    itm->first.append((FdoString*)cName);

                m_lastGtype = SqlGeometryType_Geometry;
                m_lastSrid = 0;
                const FdoSmPhColumn* geomColumn = geomProp ? geomProp->RefColumn() : (const FdoSmPhColumn*) NULL;
                if (geomColumn)
                {
                    m_lastGtype = (_wcsicmp(L"geometry", geomColumn->GetTypeName()) == 0) ? SqlGeometryType_Geometry : SqlGeometryType_Geography;
                    FdoSmPhColumnGeomP geomCol = ((FdoSmPhColumn*)geomColumn)->SmartCast<FdoSmPhColumnGeom>();
                    m_lastSrid = geomCol->GetSRID();
                }
            }
            break;
        default:
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type"));
    }
}

void FdoRdbmsSqlServerSqlBuilder::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    pair_working_stack* itm = top_stack();

    if (itm->second == SqlComputedEvalType_Unsupported)
        throw FdoException::Create(L"Unsupported FDO type in filters");

    FdoPtr<FdoExpression> exp = expr.GetExpression();
    pair_working_stack* itmExp = push_stack();
    itmExp->second = SqlComputedEvalType_Expression;
    exp->Process(this);
    itm->first.append(itmExp->first);
    pop_stack(); // itmExp

    if (itm->second == SqlComputedEvalType_Alias)
    {
        itm->first.append(L" AS \"", 5);
        itm->first.append(expr.GetName());
        itm->first.append(L"\"", 1);
    }
}

void FdoRdbmsSqlServerSqlBuilder::ProcessSubSelectExpression(FdoSubSelectExpression& expr)
{
    pair_working_stack* itm = top_stack();

    FdoPtr<FdoIdentifier> clsIdf = expr.GetFeatureClassName();
    FdoPtr<FdoIdentifier> propIdf = expr.GetPropertyName();
    FdoPtr<FdoFilter> filter = expr.GetFilter();

    // since this expression can be used in 'IN' only it needs all important parameters set.
    if (clsIdf == NULL || propIdf == NULL || filter == NULL)
        throw FdoException::Create(L"Unsupported FDO type in filters");

    FdoPtr<FdoJoinCriteriaCollection> joinCrit = expr.GetJoinCriteria();
    if (joinCrit != NULL) // for now we do not support joins into sub-selects
        throw FdoException::Create(L"Unsupported FDO type in filters");

    DbiConnection* dbiConn = m_fdoConn->GetDbiConnection();
    const FdoSmLpClassDefinition* clsDef = dbiConn->GetSchemaUtil()->GetClass(clsIdf->GetText());
    if (clsDef == NULL)
        throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_333, "Class '%1$ls' not found", clsIdf->GetText()));
    
    std::wstring aliasCls = AssignTempAlias(clsDef);

    pair_working_stack* theSelect = push_stack();
    pair_working_stack* theFilter;
    if (propIdf->GetExpressionType() == FdoExpressionItemType_ComputedIdentifier)
    {
        theSelect->second = SqlComputedEvalType_Expression;
        propIdf->Process(this);
        FdoComputedIdentifier* compIdf = static_cast<FdoComputedIdentifier*>(propIdf.p);
        m_lastSelCompIdf.push_back(std::make_pair(compIdf->GetName(), theSelect->first));
        theSelect->first.clear();
        theSelect->first.append(m_lastSelCompIdf.back().second);
        theSelect->first.append(L" AS \"");
        theSelect->first.append(m_lastSelCompIdf.back().first);
        theSelect->first.append(L"\"");
        theFilter = push_stack();
        filter->Process(this);
        m_lastSelCompIdf.pop_back();
    }
    else
    {
        theSelect->second = SqlComputedEvalType_Alias;
        propIdf->Process(this);
        theFilter = push_stack();
        filter->Process(this);
    }

    theSelect->first.append(L" FROM ");
    FdoStringP tableName = dbiConn->GetSchemaUtil()->GetDbObjectSqlName(clsDef);
    theSelect->first.append((FdoString*)tableName);
    theSelect->first.append(L" AS ");
    theSelect->first.append(aliasCls);

    itm->first.append(L"(SELECT ");
    itm->first.append(theSelect->first);
    itm->first.append(L" WHERE ");
    itm->first.append(theFilter->first); // add the filter
    itm->first.append(L")");

    pop_stack(); // theFilter
    pop_stack(); // theSelect

    m_tempUsedClasses.pop_back(); // remove the temp alias
}

// will return -1 in case is not calculation
FdoDataType FdoRdbmsSqlServerSqlBuilder::GetCalculationType(FdoString* name)
{
    FdoDataType retVal = (FdoDataType)-1;
    //
    int cntSelLst = (m_props != NULL) ? m_props->GetCount() : 0;
    for (int idx = 0; idx < cntSelLst; idx++)
    {
        // collect first all identifiers
        FdoPtr<FdoIdentifier> idf = m_props->GetItem(idx);
        FdoExpressionItemType type = idf->GetExpressionType();
        if (FdoExpressionItemType_ComputedIdentifier == type)
        {
            FdoComputedIdentifier* cidf = static_cast<FdoComputedIdentifier*>(idf.p);
            if (wcscmp(name, cidf->GetName()) == 0)
            {
                if (m_calcTypes[idx] == (FdoDataType)-1)
                {
                    FdoPtr<FdoExpression> pExp = cidf->GetExpression();
                    m_calcTypes[idx] = GetApproxExpressionDataType(pExp);
                }
                return m_calcTypes[idx];
            }
        }
    }
    return retVal;
}

FdoString* FdoRdbmsSqlServerSqlBuilder::FindCalculation(FdoString* name)
{
    if (m_lastSelCompIdf.size())
    {
        for (size_t idx = 0; idx < m_lastSelCompIdf.size(); idx++)
        {
            if (wcscmp(m_lastSelCompIdf.at(idx).first.c_str(), name) == 0)
                return m_lastSelCompIdf.at(idx).second.c_str();
        }
    }
    int cntSelLst = (m_props != NULL) ? m_props->GetCount() : 0;
    for (int idx = 0; idx < cntSelLst; idx++)
    {
        // collect first all identifiers
        FdoPtr<FdoIdentifier> idf = m_props->GetItem(idx);
        FdoExpressionItemType type = idf->GetExpressionType();
        if (FdoExpressionItemType_ComputedIdentifier == type)
        {
            FdoComputedIdentifier* cidf = static_cast<FdoComputedIdentifier*>(idf.p);
            if (wcscmp(name, cidf->GetName()) == 0)
            {
                // calculation was not processed? this can be the case for calculations
                // using other calculations or sub-selects
                if (m_selectList.at(idx).size() == 0)
                {
                    pair_working_stack* itmIdfExp = push_stack();
                    itmIdfExp->second = SqlComputedEvalType_Expression;
                    cidf->Process(this);
                    m_selectList[idx] = itmIdfExp->first;
                    pop_stack(); // itmIdfExp
                }
                return m_selectList.at(idx).c_str();
            }
        }
    }
    return NULL;
}

void FdoRdbmsSqlServerSqlBuilder::ExpandCalculations()
{
    int cntSelLst = (m_props != NULL) ? m_props->GetCount() : 0;
    for (int idx = 0; idx < cntSelLst; idx++)
    {
        // collect first all identifiers
        FdoPtr<FdoIdentifier> idf = m_props->GetItem(idx);
        FdoExpressionItemType type = idf->GetExpressionType();
        if (FdoExpressionItemType_ComputedIdentifier == type)
        {
            FdoComputedIdentifier* cidf = static_cast<FdoComputedIdentifier*>(idf.p);
            if (m_selectList.at(idx).size() == 0) // calculation was not processed?
            {
                pair_working_stack* itmIdfExp = push_stack();
                itmIdfExp->second = SqlComputedEvalType_Expression;
                cidf->Process(this);
                m_selectList[idx] = itmIdfExp->first;
                pop_stack(); // itmIdfExp
            }
        }
    }
}

FdoString* FdoRdbmsSqlServerSqlBuilder::ToSelectSqlString(FdoIdentifier* mainClass, FdoIdentifier* alias, FdoFilter* filter, FdoIdentifierCollection* selectList, 
    const std::vector<NameOrderingPair>& ordering, FdoJoinCriteriaCollection* joinCriteria)
{
    FdoPtr<FdoSqlServerInvalidExpDetProcessor> spDetector = FdoSqlServerInvalidExpDetProcessor::Create();
    if (filter != NULL)
    {
        spDetector->StartStoppable();
        filter->Process(spDetector);
        if (!spDetector->IsExpressionSupported())
            return NULL;
    }

    Reset();
    m_mainClass = FDO_SAFE_ADDREF(mainClass);
    pair_working_stack* itmSelect = top_stack();
    m_props = FDO_SAFE_ADDREF(selectList);
    DbiConnection* dbiConn = m_fdoConn->GetDbiConnection();
    m_usedClasses.clear();

    const FdoSmLpClassDefinition* clsDef = dbiConn->GetSchemaUtil()->GetClass(mainClass->GetText());
    FdoString* strAlias = (alias != NULL) ? alias->GetName() : NULL;
    m_usedClasses.push_back(std::make_pair(clsDef, (strAlias == NULL ? L"" : strAlias)));

    int cntJc = (joinCriteria) ? joinCriteria->GetCount() : 0;
    for(int idx = 0; idx < cntJc; idx++)
    {
        FdoPtr<FdoJoinCriteria> jc = joinCriteria->GetItem(idx);
        strAlias = (jc->HasAlias()) ? jc->GetAlias() : NULL;
        FdoPtr<FdoIdentifier> jcName = jc->GetJoinClass();
        clsDef = dbiConn->GetSchemaUtil()->GetClass(jcName->GetText());
        FdoPtr<FdoFilter> jcFlt = jc->GetFilter();
        if (jcFlt != NULL)
        {
            spDetector->StartStoppable();
            jcFlt->Process(spDetector);
            if (!spDetector->IsExpressionSupported())
                return NULL;
        }
        m_usedClasses.push_back(std::make_pair(clsDef, (strAlias == NULL ? L"" : strAlias)));
    }
    AssignAliases();

    sel_class_alias::iterator clsInfo = m_usedClasses.begin();
    size_t cntSelLst = (m_props != NULL) ? m_props->GetCount() : 0;
    m_hasClaculations = false;
    for (size_t idx = 0; idx < cntSelLst; idx++)
    {
        // collect first all identifiers
        FdoPtr<FdoIdentifier> idf = m_props->GetItem((int)idx);
        if (FdoExpressionItemType_Identifier == idf->GetExpressionType())
        {
            // format the select property
            pair_working_stack* itmIdfExp = push_stack();
            idf->Process(this);
            m_selectList.push_back(itmIdfExp->first);
            pop_stack(); // itmIdfExp
        }
        else // we need to do it in a different way
        {
            spDetector->StartStoppable();
            idf->Process(spDetector);
            if (!spDetector->IsExpressionSupported())
                return NULL;
            m_hasClaculations = true;
            m_selectList.push_back(L"");
        }
    }
    // since we can have calculations based on other calculations we need to 
    // expand calculations with a recursive function
    if (m_hasClaculations)
        ExpandCalculations();


    itmSelect->first.append(L"SELECT ");
    for (size_t idx = 0; idx < cntSelLst; idx++)
    {
        // collect first all identifiers
        FdoPtr<FdoIdentifier> idf = m_props->GetItem((int)idx);
        FdoExpressionItemType expType = idf->GetExpressionType();
        if (FdoExpressionItemType_Identifier == expType)
        {
            itmSelect->first.append(m_selectList.at(idx));
            itmSelect->first.append(L",", 1);
        }
        else if (FdoExpressionItemType_ComputedIdentifier == expType)
        {
            FdoComputedIdentifier* cIdf = static_cast<FdoComputedIdentifier*>(idf.p);
            itmSelect->first.append(m_selectList.at(idx));
            itmSelect->first.append(L" AS \"", 5);
            itmSelect->first.append(cIdf->GetName());
            itmSelect->first.append(L"\",", 2);
        }
        // we do not evaluate the type yet, we do it only if needed
        m_calcTypes.push_back((FdoDataType)-1);
    }
    if (cntSelLst == 0)
        itmSelect->first.append(L" * ");
    itmSelect->first.resize(itmSelect->first.size()-1);

    itmSelect->first.append(L" FROM ", 6);
    itmSelect->first.append((FdoString*)dbiConn->GetSchemaUtil()->GetDbObjectSqlName(clsInfo->first));
    if (clsInfo->second.size() != 0)
    {
        itmSelect->first.append(L" AS \"", 5);
        itmSelect->first.append(clsInfo->second);
        itmSelect->first.append(L"\"", 1);
    }

    for(int idx = 0; idx < cntJc; idx++)
    {
        FdoPtr<FdoJoinCriteria> jc = joinCriteria->GetItem(idx);
        FdoPtr<FdoIdentifier> joinCls = jc->GetJoinClass();
        FdoPtr<FdoFilter> jFilter = jc->GetFilter();
        FdoJoinType jType = jc->GetJoinType();
        switch (jType)
        {
        case FdoJoinType_Cross:
            itmSelect->first.append(L" CROSS ");
            break;
        case FdoJoinType_Inner:
            itmSelect->first.append(L" INNER ");
            break;
        case FdoJoinType_LeftOuter:
            itmSelect->first.append(L" LEFT ");
            break;
        case FdoJoinType_RightOuter:
            itmSelect->first.append(L" RIGHT ");
            break;
        case FdoJoinType_FullOuter:
            itmSelect->first.append(L" FULL ");
            break;
        default:
            throw FdoException::Create(L"Unsupported join type used in filter");
            break;
        }
        itmSelect->first.append(L" JOIN ");

        clsInfo = m_usedClasses.begin() + 1 + idx;
        itmSelect->first.append((FdoString*)dbiConn->GetSchemaUtil()->GetDbObjectSqlName(clsInfo->first));
        if (clsInfo->second.size() != 0)
        {
            itmSelect->first.append(L" AS \"", 5);
            itmSelect->first.append(clsInfo->second);
            itmSelect->first.append(L"\"", 1);
        }

        if (jType == FdoJoinType_Cross)
        {
            if (jFilter != NULL)
            {
                itmSelect->first.append(L" WHERE ");
                filter->Process(this);
            }
        }
        else
        {
            if (jFilter == NULL)
                throw FdoException::Create(L"Unsupported FDO type in filters");
            itmSelect->first.append(L" ON (");
            jFilter->Process(this);
            itmSelect->first.append(L") ");
        }
    }

    // first process and expand all select list if needed and generate the text for them.
    // replace all identifiers which are computed identifiers from select list
    if (filter)
    {
        itmSelect->first.append(L" WHERE ");
        filter->Process(this);
    }

    if (!ordering.empty())
    {
        itmSelect->first.append(L" ORDER BY ");
        for (size_t i=0; i<ordering.size(); i++)
        {
            FdoIdentifier* idfto = ordering[i].name;
            
            if (idfto->GetExpressionType() != FdoExpressionItemType_Identifier)
            {
                spDetector->StartStoppable();
                idfto->Process(spDetector);
                if (!spDetector->IsExpressionSupported())
                    return NULL;
            }
            pair_working_stack* itmOby = push_stack();
            itmOby->second = SqlComputedEvalType_Expression;
            idfto->Process(this);
            itmSelect->first.append(itmOby->first);
            pop_stack(); // itmOby

            if (ordering[i].option == FdoOrderingOption_Ascending)
                itmSelect->first.append(L" ASC,");
            else
                itmSelect->first.append(L" DESC,");
        }
        itmSelect->first.resize(itmSelect->first.size()-1);
    }
    // do not call pop_stack() for itmSelect
    return itmSelect->first.c_str();
}

FdoDataType FdoRdbmsSqlServerSqlBuilder::GetApproxExpressionDataType(FdoExpression* exp)
{
    // we use double for all numbers...
    FdoDataType retVal = FdoDataType_Double;
    switch(exp->GetExpressionType())
    {
    case FdoExpressionItemType_DataValue:
        {
            FdoDataValue* pVal = static_cast<FdoDataValue*>(exp);
            return ReduceDataType(pVal->GetDataType());
        }
        break;
    case FdoExpressionItemType_Identifier:
        {
            FdoIdentifier* df = static_cast<FdoIdentifier*>(exp);
            DbiConnection *dbiConn = m_fdoConn->GetDbiConnection();
            const FdoSmLpPropertyDefinition* propertyDefinition = NULL;
            sel_class_alias::iterator clsInfo;
            FdoInt64 idxSchema = -1;
            FdoInt64 idxScope = -1;
            FdoString* propQName = df->GetText();
            FdoString* tmp = propQName;
            while(*tmp != L'\0')
            {
                if (*tmp == L':')
                {
                    if (idxSchema != -1)
                        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
                    idxSchema = (tmp-propQName); // pos of ':'
                }
                else if (*tmp == L'.')
                {
                    if (idxScope != -1)
                        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
                    idxScope = (tmp-propQName); // pos of '.'
                }
                tmp++;
            }
            // we cannot have schema without scope!
            if (idxSchema != -1 && idxScope == -1)
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

            // since we do not have schema or scope it must be the main class
            if (idxSchema == -1)
            {
                if (idxScope == -1)
                {
                    // this could be a calculation
                    FdoDataType calcType = (m_hasClaculations) ? GetCalculationType(propQName) : (FdoDataType)-1;
                    if (calcType == (FdoDataType)-1)
                    {
                        if (m_tempUsedClasses.size() != 0)
                        {
                            clsInfo = m_tempUsedClasses.begin() + (m_tempUsedClasses.size()-1);
                            propertyDefinition = clsInfo->first->RefProperties()->RefItem(propQName);
                        }
                        if (propertyDefinition == NULL)
                        {
                            clsInfo = m_usedClasses.begin();
                            propertyDefinition = clsInfo->first->RefProperties()->RefItem(propQName);
                        }
                    }
                    else
                        return calcType;
                }
                else // scope can be the alias or the class name
                {
                    std::wstring scopeName (propQName, (size_t)idxScope);
                    std::wstring propName (propQName, (size_t)(idxScope+1), -1);
                    if (m_tempUsedClasses.size())
                    {
                        clsInfo = m_tempUsedClasses.begin() + (m_tempUsedClasses.size()-1);
                        if (clsInfo->second == scopeName)
                            propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                        else if (scopeName == clsInfo->first->GetName())
                            propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                    }
                    if (propertyDefinition == NULL)
                    {
                        for (size_t idx = 0; idx < m_usedClasses.size(); idx++)
                        {
                            clsInfo = m_usedClasses.begin()+idx;
                            if (clsInfo->second == scopeName)
                            {
                                propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                                break;
                            }
                            else if (scopeName == clsInfo->first->GetName())
                            {
                                propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                                break;
                            }
                        }
                    }
                }
            }
            else // schema:class.property? rare case, however let's try to handle it
            {
                std::wstring clsName (propQName, (size_t)idxScope);
                std::wstring propName (propQName, (size_t)(idxScope+1), -1);
                const FdoSmLpClassDefinition* clsDef = dbiConn->GetSchemaUtil()->GetClass(clsName.c_str());
                if (clsDef != NULL)
                {
                    if (m_tempUsedClasses.size())
                    {
                        clsInfo = m_tempUsedClasses.begin() + (m_tempUsedClasses.size()-1);
                        if (clsDef == clsInfo->first)
                            propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                    }
                    if (propertyDefinition == NULL)
                    {
                        for (size_t idx = 0; idx < m_usedClasses.size(); idx++)
                        {
                            clsInfo = m_usedClasses.begin()+idx;
                            if (clsDef == clsInfo->first)
                            {
                                propertyDefinition = clsInfo->first->RefProperties()->RefItem(propName.c_str());
                                break;
                            }
                        }
                    }
                }
            }

            if (propertyDefinition == NULL)
                throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_28, "Property '%1$ls' is not found", df->GetName() ));

            if (propertyDefinition && propertyDefinition->GetPropertyType() == FdoPropertyType_DataProperty)
            {
                const FdoSmLpDataPropertyDefinition* dataProp = static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                return ReduceDataType(dataProp->GetDataType());
            }
            else
                return FdoDataType_BLOB;
        }
        break;
    case FdoExpressionItemType_ComputedIdentifier:
        {
            FdoComputedIdentifier* cidf = static_cast<FdoComputedIdentifier*>(exp);
            FdoPtr<FdoExpression> expIdf = cidf->GetExpression();
            return GetApproxExpressionDataType(expIdf);
        }
        break;
    case FdoExpressionItemType_Function:
        {
            if (m_supp_functions == NULL)
            {
                FdoPtr<FdoIExpressionCapabilities> expressionCaps = m_fdoConn->GetExpressionCapabilities();
                m_supp_functions = expressionCaps->GetFunctions();
            }
            int cnt = m_supp_functions->GetCount();
            FdoFunction* pFct = static_cast<FdoFunction*>(exp);
            FdoString* fName = pFct->GetName();
            for (int i = 0; i < cnt; i++)
            {
                FdoPtr<FdoFunctionDefinition> fct = m_supp_functions->GetItem(i);
                if (_wcsicmp(fct->GetName(), fName) == 0)
                {
                    signature_map* sigdef = NULL;
                    function_sig_map::iterator itSig = m_fctSig.find(fct->GetName());
                    if (itSig == m_fctSig.end())
                    {
                        sigdef = new signature_map();
                        m_fctSig[fct->GetName()] = sigdef;
                        FdoPtr<FdoReadOnlySignatureDefinitionCollection> sigDefs = fct->GetSignatures();
                        int cntSig = (sigDefs == NULL) ? 0 : sigDefs->GetCount();
                        for (int k = 0; k < cntSig; k++)
                        {
                            std::wstring codeSig (L"");
                            FdoPtr<FdoSignatureDefinition> sig = sigDefs->GetItem(k);
                            FdoPropertyType stype = sig->GetReturnPropertyType();
                            FdoDataType retType = (FdoPropertyType_DataProperty == stype) ? ReduceDataType(sig->GetReturnType()) : FdoDataType_BLOB;

                            FdoPtr<FdoReadOnlyArgumentDefinitionCollection> argCol = sig->GetArguments();
                            int cntArg = (argCol == NULL) ? 0 : argCol->GetCount();
                            for (int y = 0; y < cntArg; y++)
                            {
                                FdoPtr<FdoArgumentDefinition> arg = argCol->GetItem(y);
                                FdoPropertyType atype = arg->GetPropertyType();
                                if (FdoPropertyType_DataProperty == atype)
                                {
                                    wchar_t strtype = ReduceDataTypeToChar(arg->GetDataType());
                                    codeSig.append(&strtype, 1);
                                }
                                else // if (FdoPropertyType_GeometricProperty == atype)
                                    codeSig.append(L"B");
                            }
                            signature_map::iterator itsg = sigdef->find(codeSig);
                            if (itsg == sigdef->end())
                                (*sigdef)[codeSig] = retType;
                        }
                    }
                    else
                        sigdef = itSig->second;

                    std::wstring codeFctSig (L"");
                    FdoPtr<FdoExpressionCollection> argColl =  pFct->GetArguments();
                    int cntArg = (argColl == NULL) ? 0 : argColl->GetCount();
                    for (int ii = 0; ii < cntArg; ii++)
                    {
                        FdoPtr<FdoExpression> expArg = argColl->GetItem(ii);
                        wchar_t strtype = ReduceDataTypeToChar(GetApproxExpressionDataType(expArg));
                        codeFctSig.append(&strtype, 1);
                    }
                    signature_map::iterator itsg = sigdef->find(codeFctSig);
                    if (itsg != sigdef->end())
                        return itsg->second;
                }
            }
        }
        break;
    case FdoExpressionItemType_SubSelectExpression:
        {
            FdoSubSelectExpression* sSel = static_cast<FdoSubSelectExpression*>(exp);
            FdoPtr<FdoIdentifier> clsIdf = sSel->GetFeatureClassName();
            FdoPtr<FdoIdentifier> propIdf = sSel->GetPropertyName();

            if (propIdf->GetExpressionType() == FdoExpressionItemType_ComputedIdentifier)
            {
                FdoComputedIdentifier* compIdf = static_cast<FdoComputedIdentifier*>(propIdf.p);
                FdoPtr<FdoExpression> expIdf = compIdf->GetExpression();
                return GetApproxExpressionDataType(expIdf);
            }
            else
                return GetApproxExpressionDataType(propIdf);
        }
        break;
    case FdoExpressionItemType_Parameter:
        {
            FdoParameter* pParam = static_cast<FdoParameter*>(exp);
            FdoString* parameName = pParam->GetName();
            int cntPar = mParams == NULL ? 0 : mParams->GetCount();
            for (int i = 0; i < cntPar; i++)
            {
                FdoPtr<FdoParameterValue> par = mParams->GetItem(i);
                if (wcscmp(parameName, par->GetName()) == 0)
                {
                    FdoPtr<FdoLiteralValue> lVal = par->GetValue();
                    return GetApproxExpressionDataType(lVal);
                }
            }            
        }
        break;
        // BinaryExpression, UnaryExpression -> double
    }
    return retVal;
}

FdoString* FdoRdbmsSqlServerSqlBuilder::ToUpdateFilterSqlString(FdoIdentifier* mainClass, FdoFilter* filter)
{
    if (filter == NULL || mainClass == NULL)
        return NULL;

    FdoPtr<FdoSqlServerInvalidExpDetProcessor> spDetector = FdoSqlServerInvalidExpDetProcessor::Create();
    spDetector->StartStoppable();
    filter->Process(spDetector);
    if (!spDetector->IsExpressionSupported())
        return NULL;

    Reset();

    m_mainClass = FDO_SAFE_ADDREF(mainClass);
    pair_working_stack* itmSelect = top_stack();
    m_props = NULL;
    DbiConnection* dbiConn = m_fdoConn->GetDbiConnection();
    m_usedClasses.clear();

    const FdoSmLpClassDefinition* clsDef = dbiConn->GetSchemaUtil()->GetClass(mainClass->GetText());
    m_usedClasses.push_back(std::make_pair(clsDef, L""));

    filter->Process(this);
    return itmSelect->first.c_str();
}
