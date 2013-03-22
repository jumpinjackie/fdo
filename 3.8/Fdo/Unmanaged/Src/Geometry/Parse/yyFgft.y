// 
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

///////////////////////////////////////////////////////////////////////
// Name
//	yyFgft.y - YACC grammar 
//								
// Synopsis	
//	FdoInt32  fdo_fgf_yyparse(FdoParse* pParse)
//
// Description							
//	Parse FGFT and create an FGF from it.
//
// Parameters
//	pParse		pointer to parsing context
//
// Function Value									
//	0 if successful, 1 otherwise (respect YYACCEPT and YYABORT macros).
//	If successful, the root node is set in FdoParse to be returned. 
///////////////////////////////////////////////////////////////////////

%{
#include "ParseFgft.h"
#pragma warning(disable:4102)	// unreferenced labels in y.tab.c
%}

%union _YYSTYPE
{
	double		m_double;		// floating point values 
}

// keywords
// ... geometry types (WKT)
%token FdoToken_POINT FdoToken_LINESTRING FdoToken_POLYGON
%token FdoToken_MULTIPOINT FdoToken_MULTILINESTRING FdoToken_MULTIPOLYGON
%token FdoToken_GEOMETRYCOLLECTION
// ... autodesk extensions (FGFT)
%token FdoToken_CURVESTRING FdoToken_CURVEPOLYGON
%token FdoToken_MULTICURVESTRING FdoToken_MULTICURVEPOLYGON
// ... reserved for future extensions
%token FdoToken_CIRCLE FdoToken_RECTANGLE
%token FdoToken_TINTERRAIN FdoToken_GRIDTERRAIN FdoToken_CUSTOM
%token FdoToken_BEZIERSEGMENT
// ... dimension types
%token FdoToken_XY FdoToken_XYZ FdoToken_XYM FdoToken_XYZM
// ... segment types
%token FdoToken_CIRCULARARCSEGMENT FdoToken_LINESTRINGSEGMENT
%token FdoToken_ARC // synonym for CIRCULARARCSEGMENT

// data types
%token FdoToken_DOUBLE

// operators
%token FdoToken_LeftParenthesis FdoToken_RightParenthesis FdoToken_Comma

// node types
%type <m_double> FdoToken_DOUBLE

%start fdo
%%

fdo : Fgft		{pParse->Done(); YYACCEPT;}
	| error		{pParse->Error(); YYABORT;}
	;

Fgft :
      FdoToken_POINT {pParse->SetGeometryType(FdoToken_POINT);}
			Dimensionality PointEntity
      | FdoToken_LINESTRING {pParse->SetGeometryType(FdoToken_LINESTRING);}
			Dimensionality LineString
      | FdoToken_POLYGON {pParse->SetGeometryType(FdoToken_POLYGON);}
			Dimensionality Polygon
	  | FdoToken_CURVESTRING {pParse->SetGeometryType(FdoToken_CURVESTRING);}
			Dimensionality CurveString
	  | FdoToken_CURVEPOLYGON {pParse->SetGeometryType(FdoToken_CURVEPOLYGON);}
			Dimensionality CurvePolygon
      | FdoToken_MULTIPOINT {pParse->SetGeometryType(FdoToken_MULTIPOINT);}
			Dimensionality MultiPoint
      | FdoToken_MULTILINESTRING {pParse->SetGeometryType(FdoToken_MULTILINESTRING);}
			Dimensionality MultiLineString
      | FdoToken_MULTIPOLYGON {pParse->SetGeometryType(FdoToken_MULTIPOLYGON);}
			Dimensionality MultiPolygon
	  | FdoToken_MULTICURVESTRING {pParse->SetGeometryType(FdoToken_MULTICURVESTRING);}
			Dimensionality MultiCurveString
	  | FdoToken_MULTICURVEPOLYGON {pParse->SetGeometryType(FdoToken_MULTICURVEPOLYGON);}
			Dimensionality MultiCurvePolygon
      | FdoToken_GEOMETRYCOLLECTION {pParse->SetGeometryType(FdoToken_GEOMETRYCOLLECTION);}
			GeometryCollection
      ;

// WKT geometries--linear only, but extended dimension types
PointEntity : '(' Point ')'						{Node_Trace(L"PointEntity\n");};
LineString : '(' PointCollection ')'			{Node_Trace(L"LineString\n");};
Polygon : '(' LineStringCollection ')'			{Node_Trace(L"Polygon\n");};
MultiPoint : '(' PointCollection ')'			{Node_Trace(L"MultiPoint\n");};
MultiLineString : '(' LineStringCollection ')'	{Node_Trace(L"MultiLineString\n");};
MultiPolygon : '(' PolygonCollection ')'		{Node_Trace(L"MultiPolygon\n");};
GeometryCollection : '(' FgftCollection ')'		{Node_Trace(L"GeometryCollection\n");};

// FGFT geometries--circular and linear segments
CurveString : '(' Point '(' CurveSegmentCollection ')' ')'	{Node_Trace(L"CurveString\n");};
CurvePolygon : '(' CurveStringCollection ')'				{Node_Trace(L"CurvePolygon\n");};
MultiCurveString : '(' CurveStringCollection ')'			{Node_Trace(L"MultiCurveString\n");};
MultiCurvePolygon : '(' CurvePolygonCollection ')'			{Node_Trace(L"MultiCurvePolygon\n");};

Dimensionality :		{pParse->SetDimensionality(-1);}
	| FdoToken_XY		{pParse->SetDimensionality(FdoToken_XY);}
	| FdoToken_XYZ		{pParse->SetDimensionality(FdoToken_XYZ);}
	| FdoToken_XYM		{pParse->SetDimensionality(FdoToken_XYM);}
	| FdoToken_XYZM		{pParse->SetDimensionality(FdoToken_XYZM);}
	;
	
Point : 
	FdoToken_DOUBLE FdoToken_DOUBLE									
		{pParse->AddPoint($1, $2);}
	| FdoToken_DOUBLE FdoToken_DOUBLE FdoToken_DOUBLE					
		{pParse->AddPoint($1, $2, $3);}
	| FdoToken_DOUBLE FdoToken_DOUBLE FdoToken_DOUBLE FdoToken_DOUBLE	
		{pParse->AddPoint($1, $2, $3, $4);}
	;

PointCollection : Point						{Node_Trace(L"PointCollection[1]\n");}
	| PointCollection ',' Point				{Node_Trace(L"PointCollection[n]\n");}
	;
LineStringCollection : LineString			{Node_Trace(L"LineStringCollection[1]\n");}
	| LineStringCollection ',' {pParse->SetBreak();} LineString	{Node_Trace(L"LineStringCollection[n]\n");}
	;    
PolygonCollection : Polygon					{Node_Trace(L"PolygonCollection[1]\n");}
	| PolygonCollection ',' {pParse->SetOuterBreak();} Polygon			{Node_Trace(L"PolygonCollection[n]\n");}
	;    
FgftCollection : Fgft						{Node_Trace(L"FgftCollection[1]\n");}
	| FgftCollection ',' Fgft				{Node_Trace(L"FgftCollection[n]\n");}
	;

CurveSegment :	FdoToken_CIRCULARARCSEGMENT {pParse->SetSegmentType(FdoToken_CIRCULARARCSEGMENT);} '(' Point ',' Point ')'
	| FdoToken_LINESTRINGSEGMENT {pParse->SetSegmentType(FdoToken_LINESTRINGSEGMENT);} '(' PointCollection ')'
	;
CurveSegmentCollection : CurveSegment			{Node_Trace(L"CurveSegmentCollection[1]\n");}
	| CurveSegmentCollection ',' CurveSegment	{Node_Trace(L"CurveSegmentCollection[n]\n");}
	;
CurveStringCollection : CurveString				{Node_Trace(L"CurveStringCollection[1]\n");}
	| CurveStringCollection ',' {pParse->SetBreak();} CurveString	{Node_Trace(L"CurveStringCollection[n]\n");}
	;    
CurvePolygonCollection : CurvePolygon			{Node_Trace(L"CurvePolygonCollection[1]\n");}
	| CurvePolygonCollection ',' {pParse->SetOuterBreak();} CurvePolygon	{Node_Trace(L"CurvePolygonCollection[n]\n");}
	;    
%%
///////////////////////////////////////////////////////////////////////

#include <stdio.h>

FdoInt32 yyerror_default(char *str, FdoInt32 lineno)
{
#ifdef FDO_YYDEBUG
	fprintf(stderr, "line %d: %s\n", lineno, str);
#endif
	return 0;
}

typedef FdoInt32 (*ERRFUNC)(char*, FdoInt32);
static ERRFUNC yyerror_func = yyerror_default;

FdoInt32 yyerror(FdoParseFgft* pParse, char* str)
{
	return (*yyerror_func)(str, yylineno);
}

ERRFUNC yyerror_set(ERRFUNC func)
{
	ERRFUNC old_func = yyerror_func;

	if (func != NULL)
		yyerror_func = func;

	return old_func;
}

static void  print_current_token()
{
//TODO: this has not been updated, but it is unused anyway
#ifdef FDO_YYDEBUG
	char  *token_name;
	
	if( yychar <= YYMAXTOKEN )
	{
		switch( yychar )
		{
			case IDENTIFIER:
				token_name = yylval.id;
				break;

			default:
				token_name = yyname[yychar];
				break;
		}
	}
	else
	{
		token_name = "illegal-name";
	}

	printf( "***  current token is %d (%s)  ***\n", yychar, token_name );
#endif
}
