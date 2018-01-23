#ifndef _PARSE_H_
#define _PARSE_H_
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
#ifdef _WIN32
#pragma once
#endif

#ifndef _WIN32
#include <stdint.h>
#endif
#include "FdoStd.h"
#ifdef _WIN32
typedef unsigned _int16	FdoUInt16;
typedef unsigned _int32	FdoUInt32;
#else
typedef uint16_t         FdoUInt16;
typedef uint32_t         FdoUInt32;
#endif
#include <Fdo/IDisposableCollection.h>
#include <Fdo/Expression/BinaryExpression.h>
#include <Fdo/Expression/DataValue.h>
#include <Fdo/Expression/Function.h>
#include <Fdo/Expression/GeometryValue.h>
#include <Fdo/Expression/Identifier.h>
#include <Fdo/Expression/Parameter.h>
#include <Fdo/Expression/UnaryExpression.h>
#include <Fdo/Expression/ExpressionException.h>
#include <Fdo/Expression/BLOBValue.h>
#include <Fdo/Expression/BooleanValue.h>
#include <Fdo/Expression/ByteValue.h>
#include <Fdo/Expression/CLOBValue.h>
#include <Fdo/Expression/DateTimeValue.h>
#include <Fdo/Expression/DecimalValue.h>
#include <Fdo/Expression/DoubleValue.h>
#include <Fdo/Expression/Int16Value.h>
#include <Fdo/Expression/Int32Value.h>
#include <Fdo/Expression/Int64Value.h>
#include <Fdo/Expression/SingleValue.h>
#include <Fdo/Expression/StringValue.h>
#include <Fdo/Expression/IntHexValue.h>
#include <Fdo/Expression/IntBinValue.h>
#include <Fdo/Expression/ComputedIdentifier.h>
#include <Fdo/Commands/IdentifierCollection.h>
#include <Fdo/Expression/SubSelectExpression.h>

#include <Fdo/Filter/BinaryLogicalOperator.h>
#include <Fdo/Filter/ComparisonCondition.h>
#include <Fdo/Filter/DistanceCondition.h>
#include <Fdo/Filter/GeometricCondition.h>
#include <Fdo/Filter/InCondition.h>
#include <Fdo/Filter/NullCondition.h>
#include <Fdo/Filter/SpatialCondition.h>
#include <Fdo/Filter/UnaryLogicalOperator.h>
#include <Fdo/Filter/FilterException.h>
#include <Fdo/Filter/Filter.h>

#include <Fdo/Schema/PropertyValueConstraint.h>
#include <Fdo/Schema/PropertyValueConstraintList.h>
#include <Fdo/Schema/PropertyValueConstraintRange.h>

// This structure should be just like FdoDateTime except without constructors.
// If a class (or structure has a constructor, it cannot be a part of a union
// such as the _YYSTYPE unions.
struct _FdoDateTime
{
    void Set(FdoDateTime tm)
    {
        year = tm.year;
        month = tm.month;
        day = tm.day;
        hour = tm.hour;
        minute = tm.minute;
        seconds = tm.seconds;
    }

    operator FdoDateTime()
    {
        FdoDateTime retVal(year, month, day, hour, minute, seconds);
        return retVal;
    }

    FdoInt16    year;       // 100-3000
    FdoInt8     month;      // 1-12
    FdoInt8     day;        // 1-31
    FdoInt8     hour;       // (0-23)
    FdoInt8     minute;     // 0-59
    float       seconds;    // 0-59.99999999
};

#include "Lex.h"
#ifdef _DEBUG
#include <stdio.h>
#endif

#ifndef FdoToken_NULL
#ifdef _WIN32
#include "yyExpressionWin.h"
#include "yyFilterWin.h"
#else
#include "yyExpression.h"
#include "yyFilter.h"
#endif
#endif

#ifdef _WIN32
// The following is from yyExpression.cpp and yyFilter.cpp

#ifdef FDO_EXPRESSION_YYSTACKSIZE
#undef FDO_EXPRESSION_YYMAXDEPTH
#define FDO_EXPRESSION_YYMAXDEPTH FDO_EXPRESSION_YYSTACKSIZE
#else
#ifdef FDO_EXPRESSION_YYMAXDEPTH
#define FDO_EXPRESSION_YYSTACKSIZE FDO_EXPRESSION_YYMAXDEPTH
#else
#define FDO_EXPRESSION_YYSTACKSIZE 500
#define FDO_EXPRESSION_YYMAXDEPTH 500
#endif
#endif

#ifdef FDO_FILTER_YYSTACKSIZE
#undef FDO_FILTER_YYMAXDEPTH
#define FDO_FILTER_YYMAXDEPTH FDO_FILTER_YYSTACKSIZE
#else
#ifdef FDO_FILTER_YYMAXDEPTH
#define FDO_FILTER_YYSTACKSIZE FDO_FILTER_YYMAXDEPTH
#else
#define FDO_FILTER_YYSTACKSIZE 500
#define FDO_FILTER_YYMAXDEPTH 500
#endif
#endif

#ifdef FDO_CONSTRAINT_YYSTACKSIZE
#undef FDO_CONSTRAINT_YYMAXDEPTH
#define FDO_CONSTRAINT_YYMAXDEPTH FDO_CONSTRAINT_YYSTACKSIZE
#else
#ifdef FDO_CONSTRAINT_YYMAXDEPTH
#define FDO_CONSTRAINT_YYSTACKSIZE FDO_CONSTRAINT_YYMAXDEPTH
#else
#define FDO_CONSTRAINT_YYSTACKSIZE 500
#define FDO_CONSTRAINT_YYMAXDEPTH 500
#endif
#endif

#endif

class FdoParse
{
public:
	FdoParse();
	~FdoParse();

	// parse expression or filter
	// returning root node of parse tree, NULL if invalid
	FdoExpression*		ParseExpression(FdoString* pwzExpression);
	FdoFilter*			ParseFilter(FdoString* pwzFilter);
	FdoPropertyValueConstraint* ParseConstraint(FdoString* pwzConstraint);
	FdoInt32			GetLexeme(FdoParse *pParse, void* yylval);
	FdoGeometryValue*	FgftToGeometryValue(FdoString* pwzFgft);

	// parse tree operations
	FdoIDisposable* AddNode(FdoIDisposable* pNode);
	FdoIDisposable* AddNodeToDelete(FdoIDisposable* pNode);
    void  AddCompIdentifier(FdoComputedIdentifier* pNode);
    FdoComputedIdentifier* FindComputedIdentifier( FdoString* name );
	FdoIDisposable*	Add(FdoString* pDebug, FdoIDisposable *pThis);
	FdoIDisposable* Copy(FdoString* pDebug, FdoIDisposable *pThis);
	void			Trace(FdoString* pDebug);
	void			Abort(void);
	void			Clean(void);
	void			SetRoot(FdoIDisposable* pNode);

	// data members
	FdoIDisposableCollection*	m_nodes;
	FdoIDisposableCollection*	m_nodesToDelete;
    FdoIdentifierCollection*    m_ComputedIdentifiers;
	FdoIDisposable*				m_root;
	FdoLex*						m_lex;

    int                         fdo_expression_yydebug;
    int                         fdo_expression_yynerrs;
    int                         fdo_expression_yyerrflag;
    int                         fdo_expression_yychar;
    short                       *fdo_expression_yyssp;

    FDO_EXPRESSION_YYSTYPE      *fdo_expression_yyvsp;
    FDO_EXPRESSION_YYSTYPE      fdo_expression_yyval;
    FDO_EXPRESSION_YYSTYPE      fdo_expression_yylval;
#ifdef _WIN32
    short                       fdo_expression_yyss[FDO_EXPRESSION_YYSTACKSIZE];
    FDO_EXPRESSION_YYSTYPE      fdo_expression_yyvs[FDO_EXPRESSION_YYSTACKSIZE];
#else
    short                       *fdo_expression_yyss;
	short						*fdo_expression_yysslim;
    FDO_EXPRESSION_YYSTYPE      *fdo_expression_yyvs;
	int							fdo_expression_yystacksize;
#endif
    FdoInt32                    fdo_expression_yylineno;

    int                         fdo_filter_yydebug;
    int                         fdo_filter_yynerrs;
    int                         fdo_filter_yyerrflag;
    int                         fdo_filter_yychar;
    short                       *fdo_filter_yyssp;

    FDO_FILTER_YYSTYPE          *fdo_filter_yyvsp;
    FDO_FILTER_YYSTYPE          fdo_filter_yyval;
    FDO_FILTER_YYSTYPE          fdo_filter_yylval;
#if _WIN32
    short                       fdo_filter_yyss[FDO_FILTER_YYSTACKSIZE];
    FDO_FILTER_YYSTYPE          fdo_filter_yyvs[FDO_FILTER_YYSTACKSIZE];
#else
    short                       *fdo_filter_yyss;
	short						*fdo_filter_yysslim;
    FDO_FILTER_YYSTYPE      	*fdo_filter_yyvs;
	int							fdo_filter_yystacksize;
#endif
    FdoInt32		            fdo_filter_yylineno;
};

// expression YACC interface
extern FdoInt32		fdo_expression_yyerror(FdoParse *, char* str);
extern FdoInt32		fdo_expression_yyparse(FdoParse*);
extern FdoInt32		fdo_expression_yylex(FdoParse*);

// filter YACC interface
extern FdoInt32		fdo_filter_yyerror(FdoParse *, char* str);
extern FdoInt32		fdo_filter_yyparse(FdoParse*);
extern FdoInt32		fdo_filter_yylex(FdoParse*);

// tree management
#if _DEBUG
#define Node_Add(a,b)	pParse->Add(a, b)
#define Node_Copy(a,b)	pParse->Copy(a, b)
#define Node_Trace(a)	pParse->Trace(a)
#else
#define Node_Add(a,b)	pParse->Add(NULL, b)
#define Node_Copy(a,b)	pParse->Copy(NULL, b)
#define Node_Trace(a)	
#endif
#endif


