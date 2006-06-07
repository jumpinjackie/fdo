#ifndef _PARSEFGFT_H_
#define _PARSEFGFT_H_
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

#include <FdoCommon.h>
#ifdef _WIN32
typedef unsigned _int32	FdoUInt32;
#else
typedef uint32_t FdoUInt32;
#endif
#include <FdoGeometry.h>
#include "LexFgft.h"
#ifdef _DEBUG
#include <stdio.h>
#endif

#ifndef FdoToken_POINT 
#ifdef _WIN32
#include "yyFgftWin.h"
#else
#include "yyFgft.h"
#endif
#endif

#ifdef _WIN32

// The following is from yyFgft.cpp
#ifdef FDO_FGFT_YYSTACKSIZE
#undef FDO_FGFT_YYMAXDEPTH
#define FDO_FGFT_YYMAXDEPTH FDO_FGFT_YYSTACKSIZE
#else
#ifdef FDO_FGFT_YYMAXDEPTH
#define FDO_FGFT_YYSTACKSIZE FDO_FGFT_YYMAXDEPTH
#else
#define FDO_FGFT_YYSTACKSIZE 500
#define FDO_FGFT_YYMAXDEPTH 500
#endif
#endif

#endif

class FdoParseFgft
{
public:
	FdoParseFgft();
	~FdoParseFgft();

	enum _types
	{
		// From FdoGeometryType
		kNone = 0,
		kPoint = 1,
		kLineString = 2,
		kPolygon = 3,
		kMultiPoint = 4,
		kMultiLineString = 5,
		kMultiPolygon = 6,
		kMultiGeometry = 7,
		kCurveString = 10,
		kCurvePolygon = 11,
		kMultiCurveString = 12,
		kMultiCurvePolygon = 13,

		// From FdoGeometryComponentType
		kLinearRing = 129,
		kCircularArcSegment = 130,
		kLineStringSegment = 131,
    	kRing = 132,

		kBreak = 200
	};
	enum _dims
	{
		kXY = 0,
		kXYZ = 1,
		kXYM = 2,
		kXYZM = 3
	};

	// parse FGFT
	FdoIGeometry*	ParseFgft(FdoString* pwzFgft);
	FdoInt32		GetLexeme(void* yylval);

	// geometry library production
	void	SetGeometryType(FdoInt32 iType);
	void	SetDimensionality(FdoInt32 iType);
	void	SetSegmentType(FdoInt32 iType);
	void	SetBreak();
	void	SetOuterBreak();
	void	AddPoint(double x, double y, double zm=0.0, double m=0.0);
	void	Done(void);
	void	Error(void);
#ifdef _DEBUG
	void	Trace(FdoString*);
#endif

    int                     fdo_fgft_yydebug;
    int                     fdo_fgft_yynerrs;
    int                     fdo_fgft_yyerrflag;
    int                     fdo_fgft_yychar;
    short                   *fdo_fgft_yyssp;

    FDO_FGFT_YYSTYPE        *fdo_fgft_yyvsp;
    FDO_FGFT_YYSTYPE        fdo_fgft_yyval;
    FDO_FGFT_YYSTYPE        fdo_fgft_yylval;
#ifdef _WIN32
    short                   fdo_fgft_yyss[FDO_FGFT_YYSTACKSIZE];
    FDO_FGFT_YYSTYPE        fdo_fgft_yyvs[FDO_FGFT_YYSTACKSIZE];
#else
    short                   *fdo_fgft_yyss;
	short					*fdo_fgft_yysslim;
    FDO_FGFT_YYSTYPE        *fdo_fgft_yyvs;
	int						fdo_fgft_yystacksize;
#endif
    FdoInt32                fdo_fgft_yylineno;

protected:
	FdoInt32		DimToCount(_dims dim);
	FdoInt32		DimToDimensionality(_dims dim);
	FdoInt32		CountSame(FdoInt32 iContext, _types type);
	FdoIDirectPosition*	CreatePosition(_dims dim, double *doubles);
	FdoIGeometry*	DoGeometryCollection(FdoInt32 iContextStart, bool bCollect);
	FdoIGeometry*	DoPoint(FdoInt32& iContext, double* doubles);
	FdoIGeometry*	DoLineString(FdoInt32& iContext, double* doubles, _types type);
	FdoIGeometry*	DoPolygon(FdoInt32& iContext, double* doubles, _types type);
	FdoIGeometry*	DoMultiPoint(FdoInt32& iContext, double* doubles);
	FdoIGeometry*	DoMultiLineString(FdoInt32& iContext, double* doubles);
	FdoIGeometry*	DoMultiPolygon(FdoInt32& iContext, double* doubles);
	FdoCurveSegmentCollection*	DoCurveSegmentCollection(FdoInt32& iContext, double* doubles);
	FdoIGeometry*	DoCurveString(FdoInt32& iContext, double* doubles);
	FdoICurvePolygon*	DoCurvePolygon(FdoInt32& iContext, double* doubles);
	FdoIGeometry*	DoMultiCurveString(FdoInt32& iContext, double* doubles);
	FdoIGeometry*	DoMultiCurvePolygon(FdoInt32& iContext, double* doubles);

	FdoLexFgft*				m_lex;
	FdoIGeometry*			m_geometry;
	FdoFgfGeometryFactory*  m_gf;
	_dims					m_dim;
	_types					m_type;
	FdoDoubleArray*			m_values;
	FdoIntArray*			m_types;
	FdoIntArray*			m_dims;
	FdoIntArray*			m_starts;
	bool					m_break;

};

// FGFT YACC interface
extern FdoInt32		fdo_fgft_yyerror(FdoParseFgft* pParse, char* str);
extern FdoInt32		fdo_fgft_yyparse(FdoParseFgft*);
extern FdoInt32		fdo_fgft_yylex(FdoParseFgft*);

// tree management
#if _DEBUG
#define Node_Trace(a)	pParse->Trace(a)
#else
#define Node_Trace(a)	
#endif
#endif

