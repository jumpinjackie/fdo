#ifndef lint
static char const 
fdo_fgft_yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define FDO_FGFT_YYBYACC 1
#define FDO_FGFT_YYMAJOR 1
#define FDO_FGFT_YYMINOR 9
#define FDO_FGFT_YYLEX fdo_fgft_yylex(pParse)
#define FDO_FGFT_YYEMPTY -1
#define fdo_fgft_yyclearin (pParse->fdo_fgft_yychar=(FDO_FGFT_YYEMPTY))
#define fdo_fgft_yyerrok (pParse->fdo_fgft_yyerrflag=0)
#define FDO_FGFT_YYRECOVERING() (pParse->fdo_fgft_yyerrflag!=0)
class FdoParseFgft; static int fdo_fgft_yygrowstack(FdoParseFgft *pParse);
#define FDO_FGFT_YYPREFIX "fdo_fgft_yy"
#line 37 "Src/Geometry/Parse/yyFgft.y"
#include "ParseFgft.h"
#pragma warning(disable:4102)	/* unreferenced labels in fdo_fgft_yyFgft.cpp*/
#line 41 "Src/Geometry/Parse/yyFgft.y"
#if 0
{
	double		m_double;		/* floating point values */
#endif
#line 25 "yyFgft.cpp"
#define FDO_FGFT_YYERRCODE 256
#define FdoToken_POINT 257
#define FdoToken_LINESTRING 258
#define FdoToken_POLYGON 259
#define FdoToken_MULTIPOINT 260
#define FdoToken_MULTILINESTRING 261
#define FdoToken_MULTIPOLYGON 262
#define FdoToken_GEOMETRYCOLLECTION 263
#define FdoToken_CURVESTRING 264
#define FdoToken_CURVEPOLYGON 265
#define FdoToken_MULTICURVESTRING 266
#define FdoToken_MULTICURVEPOLYGON 267
#define FdoToken_CIRCLE 268
#define FdoToken_RECTANGLE 269
#define FdoToken_TINTERRAIN 270
#define FdoToken_GRIDTERRAIN 271
#define FdoToken_CUSTOM 272
#define FdoToken_BEZIERSEGMENT 273
#define FdoToken_XY 274
#define FdoToken_XYZ 275
#define FdoToken_XYM 276
#define FdoToken_XYZM 277
#define FdoToken_CIRCULARARCSEGMENT 278
#define FdoToken_LINESTRINGSEGMENT 279
#define FdoToken_ARC 280
#define FdoToken_DOUBLE 281
#define FdoToken_LeftParenthesis 282
#define FdoToken_RightParenthesis 283
#define FdoToken_Comma 284
const short fdo_fgft_yylhs[] = {                                        -1,
    0,    0,    3,    1,    5,    1,    7,    1,    9,    1,
   11,    1,   13,    1,   15,    1,   17,    1,   19,    1,
   21,    1,   24,    1,    4,    6,    8,   14,   16,   18,
   23,   10,   12,   20,   22,    2,    2,    2,    2,    2,
   25,   25,   25,   26,   26,   27,   33,   27,   28,   34,
   28,   29,   29,   36,   35,   37,   35,   30,   30,   31,
   38,   31,   32,   39,   32,
};
const short fdo_fgft_yylen[] = {                                         2,
    1,    1,    0,    4,    0,    4,    0,    4,    0,    4,
    0,    4,    0,    4,    0,    4,    0,    4,    0,    4,
    0,    4,    0,    3,    3,    3,    3,    3,    3,    3,
    3,    6,    3,    3,    3,    0,    1,    1,    1,    1,
    2,    3,    4,    1,    3,    1,    0,    4,    1,    0,
    4,    1,    3,    0,    7,    0,    5,    1,    3,    1,
    0,    4,    1,    0,    4,
};
const short fdo_fgft_yydefred[] = {                                      0,
    2,    3,    5,    7,   13,   15,   17,   23,    9,   11,
   19,   21,    0,    1,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   37,   38,   39,   40,    0,
    0,    0,    0,    0,    0,    0,   24,    0,    0,    0,
    0,    0,    4,    0,    6,    0,    8,    0,   14,    0,
   16,    0,   18,   52,    0,    0,   10,    0,   12,    0,
   20,    0,   22,    0,    0,   44,    0,   46,    0,    0,
    0,   49,    0,   31,    0,    0,   60,    0,    0,   63,
    0,    0,   25,   26,    0,   27,   47,   28,   29,   30,
   50,   53,    0,   33,   61,   34,   35,   64,    0,   45,
    0,    0,   54,   56,    0,   58,    0,    0,   43,   48,
   51,    0,    0,    0,    0,   62,   65,    0,    0,   32,
   59,    0,    0,    0,   57,    0,   55,
};
const short fdo_fgft_yydgoto[] = {                                      13,
   14,   30,   15,   43,   16,   68,   17,   47,   22,   77,
   23,   59,   18,   49,   19,   51,   20,   53,   24,   61,
   25,   63,   37,   21,   66,   67,   69,   73,   55,  105,
   78,   81,  101,  102,  106,  112,  113,  107,  108,
};
const short fdo_fgft_yysindex[] = {                                   -241,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -236, -236, -236, -236, -236, -236,
    2, -236, -236, -236, -236,    0,    0,    0,    0,    8,
   12,   20,   47,   49,   50, -230,    0,   51,   52,   53,
   54, -269,    0, -269,    0,   12,    0, -269,    0,   12,
    0,   20,    0,    0,    3, -269,    0,   51,    0,   51,
    0,   52,    0, -186,   55,    0,    5,    0,    9,   24,
   25,    0,   26,    0, -230,   57,    0,   33,   34,    0,
   40, -183,    0,    0, -269,    0,    0,    0,    0,    0,
    0,    0, -199,    0,    0,    0,    0,    0, -182,    0,
   12,   20,    0,    0,   42,    0,   51,   52,    0,    0,
    0,   60,   61,   62, -199,    0,    0, -269, -269,    0,
    0,   58,   44, -269,    0,   63,    0,
};
const short fdo_fgft_yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    2,    2,    2,    2,    2,    2,
    0,    2,    2,    2,    2,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -33,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -31,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,
};
const short fdo_fgft_yygindex[] = {                                      0,
  -30,   39,    0,    0,    0,  -28,    0,  -48,    0,  -36,
    0,  -57,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -42,  -47,   56,    0,    0,    0,
   45,    0,    0,    0,   -8,    0,    0,    0,    0,
};
#define FDO_FGFT_YYTABLESIZE 107
const short fdo_fgft_yytable[] = {                                      65,
   70,   57,   45,   72,   80,   54,   41,   41,   42,   42,
   41,   64,   42,   76,    1,    2,    3,    4,    5,    6,
    7,    8,    9,   10,   11,   12,    2,    3,    4,    5,
    6,    7,    8,    9,   10,   11,   12,   26,   27,   28,
   29,   36,  100,   74,   92,   84,   75,   42,   85,   86,
  117,   44,   87,  111,   31,   32,   33,   34,   35,   46,
   38,   39,   40,   41,   88,   89,   90,   85,   87,   91,
  116,  123,  110,   94,   96,  122,   95,   95,  103,  104,
   97,  126,  114,   98,  125,  115,   48,   85,   50,   52,
   56,   58,   60,   62,   82,   83,   93,   99,  109,  118,
  119,  124,  120,  127,   79,   71,  121,
};
const short fdo_fgft_yycheck[] = {                                      42,
   48,   38,   31,   52,   62,   36,   40,   41,   40,   41,
   44,  281,   44,   56,  256,  257,  258,  259,  260,  261,
  262,  263,  264,  265,  266,  267,  257,  258,  259,  260,
  261,  262,  263,  264,  265,  266,  267,  274,  275,  276,
  277,   40,   85,   41,   75,   41,   44,   40,   44,   41,
  108,   40,   44,  102,   16,   17,   18,   19,   20,   40,
   22,   23,   24,   25,   41,   41,   41,   44,   44,   44,
  107,  119,  101,   41,   41,  118,   44,   44,  278,  279,
   41,  124,   41,   44,   41,   44,   40,   44,   40,   40,
   40,   40,   40,   40,  281,   41,   40,  281,  281,   40,
   40,   44,   41,   41,   60,   50,  115,
};
#define FDO_FGFT_YYFINAL 13
#ifndef FDO_FGFT_YYDEBUG
#define FDO_FGFT_YYDEBUG 0
#endif
#define FDO_FGFT_YYMAXTOKEN 284
#if FDO_FGFT_YYDEBUG
const char * const fdo_fgft_yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"FdoToken_POINT",
"FdoToken_LINESTRING","FdoToken_POLYGON","FdoToken_MULTIPOINT",
"FdoToken_MULTILINESTRING","FdoToken_MULTIPOLYGON",
"FdoToken_GEOMETRYCOLLECTION","FdoToken_CURVESTRING","FdoToken_CURVEPOLYGON",
"FdoToken_MULTICURVESTRING","FdoToken_MULTICURVEPOLYGON","FdoToken_CIRCLE",
"FdoToken_RECTANGLE","FdoToken_TINTERRAIN","FdoToken_GRIDTERRAIN",
"FdoToken_CUSTOM","FdoToken_BEZIERSEGMENT","FdoToken_XY","FdoToken_XYZ",
"FdoToken_XYM","FdoToken_XYZM","FdoToken_CIRCULARARCSEGMENT",
"FdoToken_LINESTRINGSEGMENT","FdoToken_ARC","FdoToken_DOUBLE",
"FdoToken_LeftParenthesis","FdoToken_RightParenthesis","FdoToken_Comma",
};
const char * const fdo_fgft_yyrule[] = {
"$accept : fdo",
"fdo : Fgft",
"fdo : error",
"$$1 :",
"Fgft : FdoToken_POINT $$1 Dimensionality PointEntity",
"$$2 :",
"Fgft : FdoToken_LINESTRING $$2 Dimensionality LineString",
"$$3 :",
"Fgft : FdoToken_POLYGON $$3 Dimensionality Polygon",
"$$4 :",
"Fgft : FdoToken_CURVESTRING $$4 Dimensionality CurveString",
"$$5 :",
"Fgft : FdoToken_CURVEPOLYGON $$5 Dimensionality CurvePolygon",
"$$6 :",
"Fgft : FdoToken_MULTIPOINT $$6 Dimensionality MultiPoint",
"$$7 :",
"Fgft : FdoToken_MULTILINESTRING $$7 Dimensionality MultiLineString",
"$$8 :",
"Fgft : FdoToken_MULTIPOLYGON $$8 Dimensionality MultiPolygon",
"$$9 :",
"Fgft : FdoToken_MULTICURVESTRING $$9 Dimensionality MultiCurveString",
"$$10 :",
"Fgft : FdoToken_MULTICURVEPOLYGON $$10 Dimensionality MultiCurvePolygon",
"$$11 :",
"Fgft : FdoToken_GEOMETRYCOLLECTION $$11 GeometryCollection",
"PointEntity : '(' Point ')'",
"LineString : '(' PointCollection ')'",
"Polygon : '(' LineStringCollection ')'",
"MultiPoint : '(' PointCollection ')'",
"MultiLineString : '(' LineStringCollection ')'",
"MultiPolygon : '(' PolygonCollection ')'",
"GeometryCollection : '(' FgftCollection ')'",
"CurveString : '(' Point '(' CurveSegmentCollection ')' ')'",
"CurvePolygon : '(' CurveStringCollection ')'",
"MultiCurveString : '(' CurveStringCollection ')'",
"MultiCurvePolygon : '(' CurvePolygonCollection ')'",
"Dimensionality :",
"Dimensionality : FdoToken_XY",
"Dimensionality : FdoToken_XYZ",
"Dimensionality : FdoToken_XYM",
"Dimensionality : FdoToken_XYZM",
"Point : FdoToken_DOUBLE FdoToken_DOUBLE",
"Point : FdoToken_DOUBLE FdoToken_DOUBLE FdoToken_DOUBLE",
"Point : FdoToken_DOUBLE FdoToken_DOUBLE FdoToken_DOUBLE FdoToken_DOUBLE",
"PointCollection : Point",
"PointCollection : PointCollection ',' Point",
"LineStringCollection : LineString",
"$$12 :",
"LineStringCollection : LineStringCollection ',' $$12 LineString",
"PolygonCollection : Polygon",
"$$13 :",
"PolygonCollection : PolygonCollection ',' $$13 Polygon",
"FgftCollection : Fgft",
"FgftCollection : FgftCollection ',' Fgft",
"$$14 :",
"CurveSegment : FdoToken_CIRCULARARCSEGMENT $$14 '(' Point ',' Point ')'",
"$$15 :",
"CurveSegment : FdoToken_LINESTRINGSEGMENT $$15 '(' PointCollection ')'",
"CurveSegmentCollection : CurveSegment",
"CurveSegmentCollection : CurveSegmentCollection ',' CurveSegment",
"CurveStringCollection : CurveString",
"$$16 :",
"CurveStringCollection : CurveStringCollection ',' $$16 CurveString",
"CurvePolygonCollection : CurvePolygon",
"$$17 :",
"CurvePolygonCollection : CurvePolygonCollection ',' $$17 CurvePolygon",
};
#endif
#if FDO_FGFT_YYDEBUG
#include <stdio.h>
#endif
#ifdef FDO_FGFT_YYSTACKSIZE
#undef FDO_FGFT_YYMAXDEPTH
#define FDO_FGFT_YYMAXDEPTH FDO_FGFT_YYSTACKSIZE
#else
#ifdef FDO_FGFT_YYMAXDEPTH
#define FDO_FGFT_YYSTACKSIZE FDO_FGFT_YYMAXDEPTH
#else
#define FDO_FGFT_YYSTACKSIZE 10000
#define FDO_FGFT_YYMAXDEPTH 10000
#endif
#endif
#define FDO_FGFT_YYINITSTACKSIZE 200
#line 162 "Src/Geometry/Parse/yyFgft.y"
///////////////////////////////////////////////////////////////////////

#include <stdio.h>

FdoInt32 fdo_fgft_yyerror_default(char *str, FdoInt32 lineno)
{
#ifdef FDO_FDO_FGFT_YYDEBUG
	fprintf(stderr, "line %d: %s\n", lineno, str);
#endif
	return 0;
}

typedef FdoInt32 (*ERRFUNC)(char*, FdoInt32);
static ERRFUNC fdo_fgft_yyerror_func = fdo_fgft_yyerror_default;

FdoInt32 fdo_fgft_yyerror(FdoParseFgft* pParse, char* str)
{
	return (*fdo_fgft_yyerror_func)(str, pParse->fdo_fgft_yylineno);
}

ERRFUNC fdo_fgft_yyerror_set(ERRFUNC func)
{
	ERRFUNC old_func = fdo_fgft_yyerror_func;

	if (func != NULL)
		fdo_fgft_yyerror_func = func;

	return old_func;
}

static void  print_current_token()
{
//TODO: this has not been updated, but it is unused anyway
#ifdef FDO_FDO_FGFT_YYDEBUG
	char  *token_name;
	
	if( pParse->fdo_fgft_yychar <= FDO_FGFT_YYMAXTOKEN )
	{
		switch( pParse->fdo_fgft_yychar )
		{
			case IDENTIFIER:
				token_name = pParse->fdo_fgft_yylval.id;
				break;

			default:
				token_name = fdo_fgft_yyname[pParse->fdo_fgft_yychar];
				break;
		}
	}
	else
	{
		token_name = "illegal-name";
	}

	printf( "***  current token is %d (%s)  ***\n", pParse->fdo_fgft_yychar, token_name );
#endif
}
#line 335 "yyFgft.cpp"
/* allocate initial stack or double stack size, up to FDO_FGFT_YYMAXDEPTH */
static int fdo_fgft_yygrowstack(FdoParseFgft *pParse)
{
    int newsize, i;
    short *newss;
    FDO_FGFT_YYSTYPE *newvs;

    if ((newsize = pParse->fdo_fgft_yystacksize) == 0)
        newsize = FDO_FGFT_YYINITSTACKSIZE;
    else if (newsize >= FDO_FGFT_YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > FDO_FGFT_YYMAXDEPTH)
        newsize = FDO_FGFT_YYMAXDEPTH;
    i = pParse->fdo_fgft_yyssp - pParse->fdo_fgft_yyss;
    newss = pParse->fdo_fgft_yyss ? (short *)realloc(pParse->fdo_fgft_yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    pParse->fdo_fgft_yyss = newss;
    pParse->fdo_fgft_yyssp = newss + i;
    newvs = pParse->fdo_fgft_yyvs ? (FDO_FGFT_YYSTYPE *)realloc(pParse->fdo_fgft_yyvs, newsize * sizeof *newvs) :
      (FDO_FGFT_YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    pParse->fdo_fgft_yyvs = newvs;
    pParse->fdo_fgft_yyvsp = newvs + i;
    pParse->fdo_fgft_yystacksize = newsize;
    pParse->fdo_fgft_yysslim = pParse->fdo_fgft_yyss + newsize - 1;
    return 0;
}

#define FDO_FGFT_YYABORT goto fdo_fgft_yyabort
#define FDO_FGFT_YYREJECT goto fdo_fgft_yyabort
#define FDO_FGFT_YYACCEPT goto fdo_fgft_yyaccept
#define FDO_FGFT_YYERROR goto fdo_fgft_yyerrlab

#ifndef FDO_FGFT_YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define FDO_FGFT_YYPARSE_PARAM_ARG void
#define FDO_FGFT_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define FDO_FGFT_YYPARSE_PARAM_ARG
#define FDO_FGFT_YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* FDO_FGFT_YYPARSE_PARAM */
#ifndef FDO_FGFT_YYPARSE_PARAM_TYPE
#define FDO_FGFT_YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define FDO_FGFT_YYPARSE_PARAM_ARG FDO_FGFT_YYPARSE_PARAM_TYPE FDO_FGFT_YYPARSE_PARAM
#define FDO_FGFT_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define FDO_FGFT_YYPARSE_PARAM_ARG FDO_FGFT_YYPARSE_PARAM
#define FDO_FGFT_YYPARSE_PARAM_DECL FDO_FGFT_YYPARSE_PARAM_TYPE FDO_FGFT_YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! FDO_FGFT_YYPARSE_PARAM */

int
fdo_fgft_yyparse(FdoParseFgft* pParse)
    FDO_FGFT_YYPARSE_PARAM_DECL
{
    register int fdo_fgft_yym, fdo_fgft_yyn, fdo_fgft_yystate;
#if FDO_FGFT_YYDEBUG
    register const char *fdo_fgft_yys;

    if ((fdo_fgft_yys = getenv("FDO_FGFT_YYDEBUG")))
    {
        fdo_fgft_yyn = *fdo_fgft_yys;
        if (fdo_fgft_yyn >= '0' && fdo_fgft_yyn <= '9')
            pParse->fdo_fgft_yydebug = fdo_fgft_yyn - '0';
    }
#endif

    pParse->fdo_fgft_yynerrs = 0;
    pParse->fdo_fgft_yyerrflag = 0;
    pParse->fdo_fgft_yychar = (-1);

    if (pParse->fdo_fgft_yyss == NULL && fdo_fgft_yygrowstack(pParse)) goto fdo_fgft_yyoverflow;
    pParse->fdo_fgft_yyssp = pParse->fdo_fgft_yyss;
    pParse->fdo_fgft_yyvsp = pParse->fdo_fgft_yyvs;
    *pParse->fdo_fgft_yyssp = fdo_fgft_yystate = 0;

fdo_fgft_yyloop:
    if ((fdo_fgft_yyn = fdo_fgft_yydefred[fdo_fgft_yystate])) goto fdo_fgft_yyreduce;
    if (pParse->fdo_fgft_yychar < 0)
    {
        if ((pParse->fdo_fgft_yychar = fdo_fgft_yylex(pParse)) < 0) pParse->fdo_fgft_yychar = 0;
#if FDO_FGFT_YYDEBUG
        if (pParse->fdo_fgft_yydebug)
        {
            fdo_fgft_yys = 0;
            if (pParse->fdo_fgft_yychar <= FDO_FGFT_YYMAXTOKEN) fdo_fgft_yys = fdo_fgft_yyname[pParse->fdo_fgft_yychar];
            if (!fdo_fgft_yys) fdo_fgft_yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    FDO_FGFT_YYPREFIX, fdo_fgft_yystate, pParse->fdo_fgft_yychar, fdo_fgft_yys);
        }
#endif
    }
    if ((fdo_fgft_yyn = fdo_fgft_yysindex[fdo_fgft_yystate]) && (fdo_fgft_yyn += pParse->fdo_fgft_yychar) >= 0 &&
            fdo_fgft_yyn <= FDO_FGFT_YYTABLESIZE && fdo_fgft_yycheck[fdo_fgft_yyn] == pParse->fdo_fgft_yychar)
    {
#if FDO_FGFT_YYDEBUG
        if (pParse->fdo_fgft_yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    FDO_FGFT_YYPREFIX, fdo_fgft_yystate, fdo_fgft_yytable[fdo_fgft_yyn]);
#endif
        if (pParse->fdo_fgft_yyssp >= pParse->fdo_fgft_yysslim && fdo_fgft_yygrowstack(pParse))
        {
            goto fdo_fgft_yyoverflow;
        }
        *++pParse->fdo_fgft_yyssp = fdo_fgft_yystate = fdo_fgft_yytable[fdo_fgft_yyn];
        *++pParse->fdo_fgft_yyvsp = pParse->fdo_fgft_yylval;
        pParse->fdo_fgft_yychar = (-1);
        if (pParse->fdo_fgft_yyerrflag > 0)  --pParse->fdo_fgft_yyerrflag;
        goto fdo_fgft_yyloop;
    }
    if ((fdo_fgft_yyn = fdo_fgft_yyrindex[fdo_fgft_yystate]) && (fdo_fgft_yyn += pParse->fdo_fgft_yychar) >= 0 &&
            fdo_fgft_yyn <= FDO_FGFT_YYTABLESIZE && fdo_fgft_yycheck[fdo_fgft_yyn] == pParse->fdo_fgft_yychar)
    {
        fdo_fgft_yyn = fdo_fgft_yytable[fdo_fgft_yyn];
        goto fdo_fgft_yyreduce;
    }
    if (pParse->fdo_fgft_yyerrflag) goto fdo_fgft_yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto fdo_fgft_yynewerror;
#endif
fdo_fgft_yynewerror:
   fdo_fgft_yyerror( pParse,"syntax error");
#if defined(lint) || defined(__GNUC__)
    goto fdo_fgft_yyerrlab;
#endif
fdo_fgft_yyerrlab:
    ++pParse->fdo_fgft_yynerrs;
fdo_fgft_yyinrecovery:
    if (pParse->fdo_fgft_yyerrflag < 3)
    {
        pParse->fdo_fgft_yyerrflag = 3;
        for (;;)
        {
            if ((fdo_fgft_yyn = fdo_fgft_yysindex[*pParse->fdo_fgft_yyssp]) && (fdo_fgft_yyn += FDO_FGFT_YYERRCODE) >= 0 &&
                    fdo_fgft_yyn <= FDO_FGFT_YYTABLESIZE && fdo_fgft_yycheck[fdo_fgft_yyn] == FDO_FGFT_YYERRCODE)
            {
#if FDO_FGFT_YYDEBUG
                if (pParse->fdo_fgft_yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", FDO_FGFT_YYPREFIX, *pParse->fdo_fgft_yyssp, fdo_fgft_yytable[fdo_fgft_yyn]);
#endif
                if (pParse->fdo_fgft_yyssp >= pParse->fdo_fgft_yysslim && fdo_fgft_yygrowstack(pParse))
                {
                    goto fdo_fgft_yyoverflow;
                }
                *++pParse->fdo_fgft_yyssp = fdo_fgft_yystate = fdo_fgft_yytable[fdo_fgft_yyn];
                *++pParse->fdo_fgft_yyvsp = pParse->fdo_fgft_yylval;
                goto fdo_fgft_yyloop;
            }
            else
            {
#if FDO_FGFT_YYDEBUG
                if (pParse->fdo_fgft_yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            FDO_FGFT_YYPREFIX, *pParse->fdo_fgft_yyssp);
#endif
                if (pParse->fdo_fgft_yyssp <= pParse->fdo_fgft_yyss) goto fdo_fgft_yyabort;
                --pParse->fdo_fgft_yyssp;
                --pParse->fdo_fgft_yyvsp;
            }
        }
    }
    else
    {
        if (pParse->fdo_fgft_yychar == 0) goto fdo_fgft_yyabort;
#if FDO_FGFT_YYDEBUG
        if (pParse->fdo_fgft_yydebug)
        {
            fdo_fgft_yys = 0;
            if (pParse->fdo_fgft_yychar <= FDO_FGFT_YYMAXTOKEN) fdo_fgft_yys = fdo_fgft_yyname[pParse->fdo_fgft_yychar];
            if (!fdo_fgft_yys) fdo_fgft_yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    FDO_FGFT_YYPREFIX, fdo_fgft_yystate, pParse->fdo_fgft_yychar, fdo_fgft_yys);
        }
#endif
        pParse->fdo_fgft_yychar = (-1);
        goto fdo_fgft_yyloop;
    }
fdo_fgft_yyreduce:
#if FDO_FGFT_YYDEBUG
    if (pParse->fdo_fgft_yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                FDO_FGFT_YYPREFIX, fdo_fgft_yystate, fdo_fgft_yyn, fdo_fgft_yyrule[fdo_fgft_yyn]);
#endif
    fdo_fgft_yym = fdo_fgft_yylen[fdo_fgft_yyn];
    pParse->fdo_fgft_yyval = pParse->fdo_fgft_yyvsp[1-fdo_fgft_yym];
    switch (fdo_fgft_yyn)
    {
case 1:
#line 76 "Src/Geometry/Parse/yyFgft.y"
{pParse->Done(); FDO_FGFT_YYACCEPT;}
break;
case 2:
#line 77 "Src/Geometry/Parse/yyFgft.y"
{pParse->Error(); FDO_FGFT_YYABORT;}
break;
case 3:
#line 81 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_POINT);}
break;
case 5:
#line 83 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_LINESTRING);}
break;
case 7:
#line 85 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_POLYGON);}
break;
case 9:
#line 87 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_CURVESTRING);}
break;
case 11:
#line 89 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_CURVEPOLYGON);}
break;
case 13:
#line 91 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_MULTIPOINT);}
break;
case 15:
#line 93 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_MULTILINESTRING);}
break;
case 17:
#line 95 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_MULTIPOLYGON);}
break;
case 19:
#line 97 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_MULTICURVESTRING);}
break;
case 21:
#line 99 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_MULTICURVEPOLYGON);}
break;
case 23:
#line 101 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetGeometryType(FdoToken_GEOMETRYCOLLECTION);}
break;
case 25:
#line 106 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"PointEntity\n");}
break;
case 26:
#line 107 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"LineString\n");}
break;
case 27:
#line 108 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"Polygon\n");}
break;
case 28:
#line 109 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"MultiPoint\n");}
break;
case 29:
#line 110 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"MultiLineString\n");}
break;
case 30:
#line 111 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"MultiPolygon\n");}
break;
case 31:
#line 112 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"GeometryCollection\n");}
break;
case 32:
#line 115 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"CurveString\n");}
break;
case 33:
#line 116 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"CurvePolygon\n");}
break;
case 34:
#line 117 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"MultiCurveString\n");}
break;
case 35:
#line 118 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"MultiCurvePolygon\n");}
break;
case 36:
#line 120 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetDimensionality(-1);}
break;
case 37:
#line 121 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetDimensionality(FdoToken_XY);}
break;
case 38:
#line 122 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetDimensionality(FdoToken_XYZ);}
break;
case 39:
#line 123 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetDimensionality(FdoToken_XYM);}
break;
case 40:
#line 124 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetDimensionality(FdoToken_XYZM);}
break;
case 41:
#line 129 "Src/Geometry/Parse/yyFgft.y"
{pParse->AddPoint(pParse->fdo_fgft_yyvsp[-1].m_double, pParse->fdo_fgft_yyvsp[0].m_double);}
break;
case 42:
#line 131 "Src/Geometry/Parse/yyFgft.y"
{pParse->AddPoint(pParse->fdo_fgft_yyvsp[-2].m_double, pParse->fdo_fgft_yyvsp[-1].m_double, pParse->fdo_fgft_yyvsp[0].m_double);}
break;
case 43:
#line 133 "Src/Geometry/Parse/yyFgft.y"
{pParse->AddPoint(pParse->fdo_fgft_yyvsp[-3].m_double, pParse->fdo_fgft_yyvsp[-2].m_double, pParse->fdo_fgft_yyvsp[-1].m_double, pParse->fdo_fgft_yyvsp[0].m_double);}
break;
case 44:
#line 136 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"PointCollection[1]\n");}
break;
case 45:
#line 137 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"PointCollection[n]\n");}
break;
case 46:
#line 139 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"LineStringCollection[1]\n");}
break;
case 47:
#line 140 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetBreak();}
break;
case 48:
#line 140 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"LineStringCollection[n]\n");}
break;
case 49:
#line 142 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"PolygonCollection[1]\n");}
break;
case 50:
#line 143 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetOuterBreak();}
break;
case 51:
#line 143 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"PolygonCollection[n]\n");}
break;
case 52:
#line 145 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"FgftCollection[1]\n");}
break;
case 53:
#line 146 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"FgftCollection[n]\n");}
break;
case 54:
#line 149 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetSegmentType(FdoToken_CIRCULARARCSEGMENT);}
break;
case 56:
#line 150 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetSegmentType(FdoToken_LINESTRINGSEGMENT);}
break;
case 58:
#line 152 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"CurveSegmentCollection[1]\n");}
break;
case 59:
#line 153 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"CurveSegmentCollection[n]\n");}
break;
case 60:
#line 155 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"CurveStringCollection[1]\n");}
break;
case 61:
#line 156 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetBreak();}
break;
case 62:
#line 156 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"CurveStringCollection[n]\n");}
break;
case 63:
#line 158 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"CurvePolygonCollection[1]\n");}
break;
case 64:
#line 159 "Src/Geometry/Parse/yyFgft.y"
{pParse->SetOuterBreak();}
break;
case 65:
#line 159 "Src/Geometry/Parse/yyFgft.y"
{Node_Trace(L"CurvePolygonCollection[n]\n");}
break;
#line 738 "yyFgft.cpp"
    }
    pParse->fdo_fgft_yyssp -= fdo_fgft_yym;
    fdo_fgft_yystate = *pParse->fdo_fgft_yyssp;
    pParse->fdo_fgft_yyvsp -= fdo_fgft_yym;
    fdo_fgft_yym = fdo_fgft_yylhs[fdo_fgft_yyn];
    if (fdo_fgft_yystate == 0 && fdo_fgft_yym == 0)
    {
#if FDO_FGFT_YYDEBUG
        if (pParse->fdo_fgft_yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", FDO_FGFT_YYPREFIX, FDO_FGFT_YYFINAL);
#endif
        fdo_fgft_yystate = FDO_FGFT_YYFINAL;
        *++pParse->fdo_fgft_yyssp = FDO_FGFT_YYFINAL;
        *++pParse->fdo_fgft_yyvsp = pParse->fdo_fgft_yyval;
        if (pParse->fdo_fgft_yychar < 0)
        {
            if ((pParse->fdo_fgft_yychar = fdo_fgft_yylex(pParse)) < 0) pParse->fdo_fgft_yychar = 0;
#if FDO_FGFT_YYDEBUG
            if (pParse->fdo_fgft_yydebug)
            {
                fdo_fgft_yys = 0;
                if (pParse->fdo_fgft_yychar <= FDO_FGFT_YYMAXTOKEN) fdo_fgft_yys = fdo_fgft_yyname[pParse->fdo_fgft_yychar];
                if (!fdo_fgft_yys) fdo_fgft_yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        FDO_FGFT_YYPREFIX, FDO_FGFT_YYFINAL, pParse->fdo_fgft_yychar, fdo_fgft_yys);
            }
#endif
        }
        if (pParse->fdo_fgft_yychar == 0) goto fdo_fgft_yyaccept;
        goto fdo_fgft_yyloop;
    }
    if ((fdo_fgft_yyn = fdo_fgft_yygindex[fdo_fgft_yym]) && (fdo_fgft_yyn += fdo_fgft_yystate) >= 0 &&
            fdo_fgft_yyn <= FDO_FGFT_YYTABLESIZE && fdo_fgft_yycheck[fdo_fgft_yyn] == fdo_fgft_yystate)
        fdo_fgft_yystate = fdo_fgft_yytable[fdo_fgft_yyn];
    else
        fdo_fgft_yystate = fdo_fgft_yydgoto[fdo_fgft_yym];
#if FDO_FGFT_YYDEBUG
    if (pParse->fdo_fgft_yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", FDO_FGFT_YYPREFIX, *pParse->fdo_fgft_yyssp, fdo_fgft_yystate);
#endif
    if (pParse->fdo_fgft_yyssp >= pParse->fdo_fgft_yysslim && fdo_fgft_yygrowstack(pParse))
    {
        goto fdo_fgft_yyoverflow;
    }
    *++pParse->fdo_fgft_yyssp = fdo_fgft_yystate;
    *++pParse->fdo_fgft_yyvsp = pParse->fdo_fgft_yyval;
    goto fdo_fgft_yyloop;
fdo_fgft_yyoverflow:
   fdo_fgft_yyerror( pParse,"yacc stack overflow");
fdo_fgft_yyabort:
    return (1);
fdo_fgft_yyaccept:
    return (0);
}
