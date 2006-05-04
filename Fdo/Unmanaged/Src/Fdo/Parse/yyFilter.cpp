#ifndef lint
static char const 
fdo_filter_yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define FDO_FILTER_YYBYACC 1
#define FDO_FILTER_YYMAJOR 1
#define FDO_FILTER_YYMINOR 9
#define FDO_FILTER_YYLEX fdo_filter_yylex(pParse)
#define FDO_FILTER_YYEMPTY -1
#define fdo_filter_yyclearin (pParse->fdo_filter_yychar=(FDO_FILTER_YYEMPTY))
#define fdo_filter_yyerrok (pParse->fdo_filter_yyerrflag=0)
#define FDO_FILTER_YYRECOVERING() (pParse->fdo_filter_yyerrflag!=0)
class FdoParse;
static int fdo_filter_yygrowstack(FdoParse *pParse);
#define FDO_FILTER_YYPREFIX "fdo_filter_yy"
#line 37 "Src/Fdo/Parse/yyFilter.y"
#include "Parse.h"
#pragma warning(disable:4102)	/* unreferenced labels in fdo_filter_yyFilter.cpp*/
#line 42 "Src/Fdo/Parse/yyFilter.y"
#if 0
{
	FdoIDisposable*	m_node;		/* expression or filter parse tree node*/
	FdoInt32		m_id;		/* enumerations, keywords, ...*/
	FdoInt32		m_integer;	/* integer values (Int8, Int16, Int32, TODO:I64 )*/
	double			m_double;	/* floating point values (single(float), double)*/
	FdoString*		m_string;	/* string*/
	_FdoDateTime	m_datetime;	/* date time*/
#endif
#line 30 "yyFilter.cpp"
#define FDO_FILTER_YYERRCODE 256
#define FdoToken_NULL 257
#define FdoToken_TRUE 258
#define FdoToken_FALSE 259
#define FdoToken_DATE 260
#define FdoToken_TIME 261
#define FdoToken_TIMESTAMP 262
#define FdoToken_GEOMFROMTEXT 263
#define FdoToken_AND 264
#define FdoToken_OR 265
#define FdoToken_NOT 266
#define FdoToken_LIKE 267
#define FdoToken_IN 268
#define FdoToken_BEYOND 269
#define FdoToken_WITHINDISTANCE 270
#define FdoToken_CONTAINS 271
#define FdoToken_COVEREDBY 272
#define FdoToken_CROSSES 273
#define FdoToken_DISJOINT 274
#define FdoToken_ENVELOPEINTERSECTS 275
#define FdoToken_EQUALS 276
#define FdoToken_INTERSECTS 277
#define FdoToken_INSIDE 278
#define FdoToken_OVERLAPS 279
#define FdoToken_TOUCHES 280
#define FdoToken_WITHIN 281
#define FdoToken_RELATE 282
#define FdoToken_IDENTIFIER 283
#define FdoToken_PARAMETER 284
#define FdoToken_STRING 285
#define FdoToken_INTEGER 286
#define FdoToken_DOUBLE 287
#define FdoToken_DATETIME 288
#define FdoToken_BLOB 289
#define FdoToken_CLOB 290
#define FdoToken_Add 291
#define FdoToken_Subtract 292
#define FdoToken_Multiply 293
#define FdoToken_Divide 294
#define FdoToken_Negate 295
#define FdoToken_EQ 296
#define FdoToken_NE 297
#define FdoToken_GT 298
#define FdoToken_GE 299
#define FdoToken_LT 300
#define FdoToken_LE 301
#define FdoToken_LeftParenthesis 302
#define FdoToken_RightParenthesis 303
#define FdoToken_Comma 304
#define FdoToken_Dot 305
#define FdoToken_Colon 306
#define FdoToken_AS 307
#define FdoToken_BETWEEN 308
const short fdo_filter_yylhs[] = {                                        -1,
    0,    0,    0,    3,    3,    3,    3,    3,    3,    1,
    1,    1,    1,    2,    2,    2,    2,    2,    2,    2,
    2,   30,   30,   29,   31,   28,   32,    5,    4,    4,
    4,    6,    7,   33,   33,   34,    8,    8,    9,    9,
   10,   10,   11,   15,   15,   15,   18,   18,   16,   16,
   20,   20,   17,   19,   21,   21,   21,   21,   22,   23,
   12,   12,   13,   14,   25,   25,   25,   25,   25,   25,
   25,   26,   26,   24,   24,   24,   24,   24,   24,   24,
   24,   24,   24,   24,   27,   27,
};
const short fdo_filter_yylen[] = {                                         2,
    1,    3,    1,    3,    1,    1,    1,    1,    1,    3,
    3,    3,    3,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    4,    0,    1,
    3,    4,    1,    1,    3,    5,    1,    1,    1,    1,
    1,    2,    2,    3,    1,    1,    1,    1,    1,    1,
    1,    3,    5,    2,    1,    1,    1,    1,    3,    2,
    3,    3,    3,    4,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,
};
const short fdo_filter_yydefred[] = {                                      0,
    3,   21,   22,   23,    0,    0,   33,   41,   24,   25,
   26,   27,   19,   20,    0,    0,    0,    0,    6,   40,
    0,    7,   39,    0,    9,   37,   38,    5,   47,   56,
   49,    0,   57,   55,   45,   58,   46,   50,   48,   16,
   18,   14,   17,   15,    0,   34,    0,    0,   60,    0,
   43,    0,   42,    0,    0,   71,    0,    0,    0,    0,
   65,   66,   67,   68,   69,   70,    0,   54,    0,   72,
   73,   74,   83,   75,   76,   84,   77,   79,   78,   80,
   81,   82,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   44,    0,    0,   12,   13,    0,    0,    0,
    0,    0,    0,   61,    0,    0,   35,   32,    4,    0,
   51,    0,    0,   28,   86,   64,   85,   36,    0,   53,
    0,   52,
};
const short fdo_filter_yydgoto[] = {                                      18,
   19,   20,   21,  101,   22,   23,   52,   25,   26,   27,
   28,   29,   30,   31,   55,   33,   34,   35,   36,  112,
   37,   38,   39,   84,   67,   85,  116,   40,   41,   42,
   43,   44,   45,   46,
};
const short fdo_filter_yysindex[] = {                                    -37,
    0,    0,    0,    0,  -24,   74,    0,    0,    0,    0,
    0,    0,    0,    0,  142, -257,   74,    0,    0,    0,
 -173,    0,    0,  215,    0,    0,    0,    0,    0,    0,
    0, -224,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -10,    0, -241,   74,    0,  142,
    0,   12,    0,  119,  -41,    0,  142,  142,  142,  142,
    0,    0,    0,    0,    0,    0,  142,    0,   14,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  142,  142,  142,   74,   74,  108,    7,  166,
  -32, -261,    0, -243, -243,    0,    0, -279, -147, -279,
  -34, -279, -255,    0, -208, -224,    0,    0,    0, -226,
    0,  -23,  142,    0,    0,    0,    0,    0, -147,    0,
 -279,    0,
};
const short fdo_filter_yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  177,    0,    0,    0,    0,    0,    0,
    0,   59,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    1,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -21,    0,    0,    0,    0,    0,    0,    0,
    0, -123,    0,   17,   55,    0,    0,    6,    0,  -15,
    0,    8,    0,    0,    2,   60,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -14,    0,
};
const short fdo_filter_yygindex[] = {                                      0,
    0,    0,   18,    0,    0,    0,    5,  -95,    0,    0,
    0,    0,    0,    0,   19,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -40,    0,    0,
    0,    0,    0,  -26,
};
#define FDO_FILTER_YYTABLESIZE 496
const short fdo_filter_yytable[] = {                                      93,
    8,   62,   17,  111,   24,   63,  114,   59,  109,  113,
   24,   57,   58,   59,   60,   47,   10,  120,   32,   29,
  119,   24,   29,  122,   49,   30,   31,   53,   30,   31,
  115,   11,   51,   88,   54,   57,   58,   59,   60,   86,
   87,    8,   62,   89,    8,  110,   63,  108,   59,   59,
   60,   83,   24,   99,   11,   86,    7,   10,    1,    2,
   10,  107,  117,    0,    0,   90,    0,   91,    0,    0,
    0,    0,    0,    0,   94,   95,   96,   97,    0,    0,
    0,    0,    0,    0,   98,    0,    0,    0,    0,    0,
   24,   24,   24,   56,    0,   11,    0,    0,   11,    0,
  100,  102,  103,    0,  104,  105,  106,    0,    0,    2,
    3,    4,    0,   48,  118,    5,    0,   57,   58,   59,
   60,    0,   61,   62,   63,   64,   65,   66,    0,    0,
  121,    0,    0,    0,    0,    0,    8,    9,   10,   11,
   12,   13,   14,    4,    0,    0,    0,   17,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   16,   92,
    0,    0,    0,    0,    0,    0,    0,    4,    4,    4,
    4,    0,    4,    4,    4,    4,    4,    4,    0,    0,
    0,   50,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  109,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    8,    1,    2,
    3,    4,   86,   87,    0,    5,    0,    0,    6,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    7,    8,    9,   10,   11,
   12,   13,   14,    0,   83,    0,    0,   15,   57,   58,
   59,   60,    0,    0,    8,    8,   62,    8,   16,   63,
   63,   59,   59,    0,    0,    0,    0,    0,    0,    0,
   10,   10,    0,   10,    0,    0,    8,    8,    0,    0,
    0,    8,    8,    8,    8,    0,    8,    8,    8,    8,
    8,    8,   10,   10,    0,    0,    0,   10,   10,    0,
    0,    0,   10,   10,   10,   10,   10,   10,   11,   11,
    0,   11,    0,    0,    0,    0,    0,    0,    0,    0,
    2,    3,    4,    0,    0,    0,    5,    0,    0,    6,
   11,   11,    0,    0,    0,   11,   11,    0,    0,    0,
   11,   11,   11,   11,   11,   11,    7,    8,    9,   10,
   11,   12,   13,   14,    2,    3,    4,    0,   15,    0,
    5,    0,    0,    6,    0,    0,    0,    0,    0,   16,
    0,    0,    0,    0,    0,   56,    0,    0,    0,    0,
    7,    8,    9,   10,   11,   12,   13,   14,    2,    3,
    4,    0,   15,    0,    5,    0,    0,    0,    0,   57,
   58,   59,   60,   16,   61,   62,   63,   64,   65,   66,
    0,    0,    0,    0,    7,    8,    9,   10,   11,   12,
   13,   14,   56,    0,    0,    0,   15,    0,    0,    0,
    0,    0,    0,    8,    0,    0,    0,   16,    0,    0,
    0,    0,    0,    0,    0,    0,   57,   58,   59,   60,
    0,   61,   62,   63,   64,   65,   66,    8,    8,    8,
    8,   68,    8,    8,    8,    8,    8,    8,    0,    0,
    0,    0,   69,   70,   71,   72,   73,   74,   75,   76,
   77,   78,   79,   80,   81,   82,
};
const short fdo_filter_yycheck[] = {                                      41,
    0,    0,   40,   99,    0,    0,   41,    0,   41,   44,
    6,  291,  292,  293,  294,   40,    0,   41,    0,   41,
   44,   17,   44,  119,    6,   41,   41,  285,   44,   44,
  286,  287,   15,   44,   17,  291,  292,  293,  294,  264,
  265,   41,   41,  285,   44,  307,   41,   41,   41,  293,
  294,   40,   48,   40,    0,  264,  283,   41,    0,    0,
   44,   88,  103,   -1,   -1,   48,   -1,   50,   -1,   -1,
   -1,   -1,   -1,   -1,   57,   58,   59,   60,   -1,   -1,
   -1,   -1,   -1,   -1,   67,   -1,   -1,   -1,   -1,   -1,
   86,   87,   88,  267,   -1,   41,   -1,   -1,   44,   -1,
   83,   84,   85,   -1,   86,   87,   88,   -1,   -1,  257,
  258,  259,   -1,   40,  110,  263,   -1,  291,  292,  293,
  294,   -1,  296,  297,  298,  299,  300,  301,   -1,   -1,
  113,   -1,   -1,   -1,   -1,   -1,  284,  285,  286,  287,
  288,  289,  290,  267,   -1,   -1,   -1,   40,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  306,   41,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  291,  292,  293,
  294,   -1,  296,  297,  298,  299,  300,  301,   -1,   -1,
   -1,   40,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   41,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   41,  256,  257,
  258,  259,  264,  265,   -1,  263,   -1,   -1,  266,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  283,  284,  285,  286,  287,
  288,  289,  290,   -1,   40,   -1,   -1,  295,  291,  292,
  293,  294,   -1,   -1,  264,  265,  265,  267,  306,  264,
  265,  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  264,  265,   -1,  267,   -1,   -1,  286,  287,   -1,   -1,
   -1,  291,  292,  293,  294,   -1,  296,  297,  298,  299,
  300,  301,  286,  287,   -1,   -1,   -1,  291,  292,   -1,
   -1,   -1,  296,  297,  298,  299,  300,  301,  264,  265,
   -1,  267,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  257,  258,  259,   -1,   -1,   -1,  263,   -1,   -1,  266,
  286,  287,   -1,   -1,   -1,  291,  292,   -1,   -1,   -1,
  296,  297,  298,  299,  300,  301,  283,  284,  285,  286,
  287,  288,  289,  290,  257,  258,  259,   -1,  295,   -1,
  263,   -1,   -1,  266,   -1,   -1,   -1,   -1,   -1,  306,
   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,   -1,   -1,
  283,  284,  285,  286,  287,  288,  289,  290,  257,  258,
  259,   -1,  295,   -1,  263,   -1,   -1,   -1,   -1,  291,
  292,  293,  294,  306,  296,  297,  298,  299,  300,  301,
   -1,   -1,   -1,   -1,  283,  284,  285,  286,  287,  288,
  289,  290,  267,   -1,   -1,   -1,  295,   -1,   -1,   -1,
   -1,   -1,   -1,  267,   -1,   -1,   -1,  306,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  291,  292,  293,  294,
   -1,  296,  297,  298,  299,  300,  301,  291,  292,  293,
  294,  257,  296,  297,  298,  299,  300,  301,   -1,   -1,
   -1,   -1,  268,  269,  270,  271,  272,  273,  274,  275,
  276,  277,  278,  279,  280,  281,
};
#define FDO_FILTER_YYFINAL 18
#ifndef FDO_FILTER_YYDEBUG
#define FDO_FILTER_YYDEBUG 0
#endif
#define FDO_FILTER_YYMAXTOKEN 308
#if FDO_FILTER_YYDEBUG
const char * const fdo_filter_yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"FdoToken_NULL",
"FdoToken_TRUE","FdoToken_FALSE","FdoToken_DATE","FdoToken_TIME",
"FdoToken_TIMESTAMP","FdoToken_GEOMFROMTEXT","FdoToken_AND","FdoToken_OR",
"FdoToken_NOT","FdoToken_LIKE","FdoToken_IN","FdoToken_BEYOND",
"FdoToken_WITHINDISTANCE","FdoToken_CONTAINS","FdoToken_COVEREDBY",
"FdoToken_CROSSES","FdoToken_DISJOINT","FdoToken_ENVELOPEINTERSECTS",
"FdoToken_EQUALS","FdoToken_INTERSECTS","FdoToken_INSIDE","FdoToken_OVERLAPS",
"FdoToken_TOUCHES","FdoToken_WITHIN","FdoToken_RELATE","FdoToken_IDENTIFIER",
"FdoToken_PARAMETER","FdoToken_STRING","FdoToken_INTEGER","FdoToken_DOUBLE",
"FdoToken_DATETIME","FdoToken_BLOB","FdoToken_CLOB","FdoToken_Add",
"FdoToken_Subtract","FdoToken_Multiply","FdoToken_Divide","FdoToken_Negate",
"FdoToken_EQ","FdoToken_NE","FdoToken_GT","FdoToken_GE","FdoToken_LT",
"FdoToken_LE","FdoToken_LeftParenthesis","FdoToken_RightParenthesis",
"FdoToken_Comma","FdoToken_Dot","FdoToken_Colon","FdoToken_AS",
"FdoToken_BETWEEN",
};
const char * const fdo_filter_yyrule[] = {
"$accept : fdo",
"fdo : Filter",
"fdo : ComputedIdentifier ',' Filter",
"fdo : error",
"Expression : '(' Expression ')'",
"Expression : UnaryExpression",
"Expression : BinaryExpression",
"Expression : Function",
"Expression : Identifier",
"Expression : ValueExpression",
"BinaryExpression : Expression FdoToken_Add Expression",
"BinaryExpression : Expression FdoToken_Subtract Expression",
"BinaryExpression : Expression FdoToken_Multiply Expression",
"BinaryExpression : Expression FdoToken_Divide Expression",
"DataValue : boolean",
"DataValue : datetime",
"DataValue : double",
"DataValue : integer",
"DataValue : string",
"DataValue : FdoToken_BLOB",
"DataValue : FdoToken_CLOB",
"DataValue : FdoToken_NULL",
"boolean : FdoToken_TRUE",
"boolean : FdoToken_FALSE",
"string : FdoToken_STRING",
"integer : FdoToken_INTEGER",
"double : FdoToken_DOUBLE",
"datetime : FdoToken_DATETIME",
"Function : Identifier '(' ExpressionCollection ')'",
"ExpressionCollection :",
"ExpressionCollection : Expression",
"ExpressionCollection : ExpressionCollection ',' Expression",
"GeometryValue : FdoToken_GEOMFROMTEXT '(' FdoToken_STRING ')'",
"Identifier : FdoToken_IDENTIFIER",
"ComputedIdentifier : ComputedIdentifier2",
"ComputedIdentifier : ComputedIdentifier ',' ComputedIdentifier2",
"ComputedIdentifier2 : '(' Expression ')' FdoToken_AS Identifier",
"ValueExpression : LiteralValue",
"ValueExpression : Parameter",
"LiteralValue : GeometryValue",
"LiteralValue : DataValue",
"Parameter : FdoToken_PARAMETER",
"Parameter : FdoToken_Colon FdoToken_STRING",
"UnaryExpression : FdoToken_Negate Expression",
"Filter : '(' Filter ')'",
"Filter : LogicalOperator",
"Filter : SearchCondition",
"LogicalOperator : BinaryLogicalOperator",
"LogicalOperator : UnaryLogicalOperator",
"GeometricCondition : DistanceCondition",
"GeometricCondition : SpatialCondition",
"ValueExpressionCollection : ValueExpression",
"ValueExpressionCollection : ValueExpressionCollection ',' ValueExpression",
"InCondition : Identifier FdoToken_IN '(' ValueExpressionCollection ')'",
"NullCondition : Identifier FdoToken_NULL",
"SearchCondition : InCondition",
"SearchCondition : ComparisonCondition",
"SearchCondition : GeometricCondition",
"SearchCondition : NullCondition",
"SpatialCondition : Identifier SpatialOperations Expression",
"UnaryLogicalOperator : FdoToken_NOT Filter",
"BinaryLogicalOperator : Filter FdoToken_AND Filter",
"BinaryLogicalOperator : Filter FdoToken_OR Filter",
"ComparisonCondition : Expression ComparisonOperations Expression",
"DistanceCondition : Identifier DistanceOperations Expression distance",
"ComparisonOperations : FdoToken_EQ",
"ComparisonOperations : FdoToken_NE",
"ComparisonOperations : FdoToken_GT",
"ComparisonOperations : FdoToken_GE",
"ComparisonOperations : FdoToken_LT",
"ComparisonOperations : FdoToken_LE",
"ComparisonOperations : FdoToken_LIKE",
"DistanceOperations : FdoToken_BEYOND",
"DistanceOperations : FdoToken_WITHINDISTANCE",
"SpatialOperations : FdoToken_CONTAINS",
"SpatialOperations : FdoToken_CROSSES",
"SpatialOperations : FdoToken_DISJOINT",
"SpatialOperations : FdoToken_EQUALS",
"SpatialOperations : FdoToken_INSIDE",
"SpatialOperations : FdoToken_INTERSECTS",
"SpatialOperations : FdoToken_OVERLAPS",
"SpatialOperations : FdoToken_TOUCHES",
"SpatialOperations : FdoToken_WITHIN",
"SpatialOperations : FdoToken_COVEREDBY",
"SpatialOperations : FdoToken_ENVELOPEINTERSECTS",
"distance : double",
"distance : FdoToken_INTEGER",
};
#endif
#if FDO_FILTER_YYDEBUG
#include <stdio.h>
#endif
#ifdef FDO_FILTER_YYSTACKSIZE
#undef FDO_FILTER_YYMAXDEPTH
#define FDO_FILTER_YYMAXDEPTH FDO_FILTER_YYSTACKSIZE
#else
#ifdef FDO_FILTER_YYMAXDEPTH
#define FDO_FILTER_YYSTACKSIZE FDO_FILTER_YYMAXDEPTH
#else
#define FDO_FILTER_YYSTACKSIZE 10000
#define FDO_FILTER_YYMAXDEPTH 10000
#endif
#endif
#define FDO_FILTER_YYINITSTACKSIZE 200
#line 340 "Src/Fdo/Parse/yyFilter.y"
///////////////////////////////////////////////////////////////////////

#include <stdio.h>

FdoInt32 fdo_filter_yyerror_default(char *str, FdoInt32 lineno)
{
#ifdef FDO_FDO_FILTER_YYDEBUG
	fprintf(stderr, "line %d: %s\n", lineno, str);
#endif
	return 0;
}

typedef FdoInt32 (*ERRFUNC)(char*, FdoInt32);
static ERRFUNC fdo_filter_yyerror_func = fdo_filter_yyerror_default;

FdoInt32 fdo_filter_yyerror(FdoParse *pParse, char* str)
{
	return (*fdo_filter_yyerror_func)(str, pParse->fdo_filter_yylineno);
}

ERRFUNC fdo_filter_yyerror_set(ERRFUNC func)
{
	ERRFUNC old_func = fdo_filter_yyerror_func;

	if (func != NULL)
		fdo_filter_yyerror_func = func;

	return old_func;
}

static void  print_current_token()
{
//TODO: this has not been updated, but it is unused anyway
#ifdef FDO_FDO_FILTER_YYDEBUG
	char  *token_name;
	
	if( pParse->fdo_filter_yychar <= FDO_FILTER_YYMAXTOKEN )
	{
		switch( pParse->fdo_filter_yychar )
		{
			case IDENTIFIER:
				token_name = pParse->fdo_filter_yylval.id;
				break;

			default:
				token_name = fdo_filter_yyname[pParse->fdo_filter_yychar];
				break;
		}
	}
	else
	{
		token_name = "illegal-name";
	}

	printf( "***  current token is %d (%s)  ***\n", pParse->fdo_filter_yychar, token_name );
#endif
}
#line 472 "yyFilter.cpp"
/* allocate initial stack or double stack size, up to FDO_FILTER_YYMAXDEPTH */
static int fdo_filter_yygrowstack(FdoParse *pParse)
{
    int newsize, i;
    short *newss;
    FDO_FILTER_YYSTYPE *newvs;

    if ((newsize = pParse->fdo_filter_yystacksize) == 0)
        newsize = FDO_FILTER_YYINITSTACKSIZE;
    else if (newsize >= FDO_FILTER_YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > FDO_FILTER_YYMAXDEPTH)
        newsize = FDO_FILTER_YYMAXDEPTH;
    i = pParse->fdo_filter_yyssp - pParse->fdo_filter_yyss;
    newss = pParse->fdo_filter_yyss ? (short *)realloc(pParse->fdo_filter_yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    pParse->fdo_filter_yyss = newss;
    pParse->fdo_filter_yyssp = newss + i;
    newvs = pParse->fdo_filter_yyvs ? (FDO_FILTER_YYSTYPE *)realloc(pParse->fdo_filter_yyvs, newsize * sizeof *newvs) :
      (FDO_FILTER_YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    pParse->fdo_filter_yyvs = newvs;
    pParse->fdo_filter_yyvsp = newvs + i;
    pParse->fdo_filter_yystacksize = newsize;
    pParse->fdo_filter_yysslim = pParse->fdo_filter_yyss + newsize - 1;
    return 0;
}

#define FDO_FILTER_YYABORT goto fdo_filter_yyabort
#define FDO_FILTER_YYREJECT goto fdo_filter_yyabort
#define FDO_FILTER_YYACCEPT goto fdo_filter_yyaccept
#define FDO_FILTER_YYERROR goto fdo_filter_yyerrlab

#ifndef FDO_FILTER_YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define FDO_FILTER_YYPARSE_PARAM_ARG void
#define FDO_FILTER_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define FDO_FILTER_YYPARSE_PARAM_ARG
#define FDO_FILTER_YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* FDO_FILTER_YYPARSE_PARAM */
#ifndef FDO_FILTER_YYPARSE_PARAM_TYPE
#define FDO_FILTER_YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define FDO_FILTER_YYPARSE_PARAM_ARG FDO_FILTER_YYPARSE_PARAM_TYPE FDO_FILTER_YYPARSE_PARAM
#define FDO_FILTER_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define FDO_FILTER_YYPARSE_PARAM_ARG FDO_FILTER_YYPARSE_PARAM
#define FDO_FILTER_YYPARSE_PARAM_DECL FDO_FILTER_YYPARSE_PARAM_TYPE FDO_FILTER_YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! FDO_FILTER_YYPARSE_PARAM */

int
fdo_filter_yyparse(FdoParse* pParse)
    FDO_FILTER_YYPARSE_PARAM_DECL
{
    register int fdo_filter_yym, fdo_filter_yyn, fdo_filter_yystate;
#if FDO_FILTER_YYDEBUG
    register const char *fdo_filter_yys;

    if ((fdo_filter_yys = getenv("FDO_FILTER_YYDEBUG")))
    {
        fdo_filter_yyn = *fdo_filter_yys;
        if (fdo_filter_yyn >= '0' && fdo_filter_yyn <= '9')
            pParse->fdo_filter_yydebug = fdo_filter_yyn - '0';
    }
#endif

    pParse->fdo_filter_yynerrs = 0;
    pParse->fdo_filter_yyerrflag = 0;
    pParse->fdo_filter_yychar = (-1);

    if (pParse->fdo_filter_yyss == NULL && fdo_filter_yygrowstack(pParse)) goto fdo_filter_yyoverflow;
    pParse->fdo_filter_yyssp = pParse->fdo_filter_yyss;
    pParse->fdo_filter_yyvsp = pParse->fdo_filter_yyvs;
    *pParse->fdo_filter_yyssp = fdo_filter_yystate = 0;

fdo_filter_yyloop:
    if ((fdo_filter_yyn = fdo_filter_yydefred[fdo_filter_yystate])) goto fdo_filter_yyreduce;
    if (pParse->fdo_filter_yychar < 0)
    {
        if ((pParse->fdo_filter_yychar = fdo_filter_yylex(pParse)) < 0) pParse->fdo_filter_yychar = 0;
#if FDO_FILTER_YYDEBUG
        if (pParse->fdo_filter_yydebug)
        {
            fdo_filter_yys = 0;
            if (pParse->fdo_filter_yychar <= FDO_FILTER_YYMAXTOKEN) fdo_filter_yys = fdo_filter_yyname[pParse->fdo_filter_yychar];
            if (!fdo_filter_yys) fdo_filter_yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    FDO_FILTER_YYPREFIX, fdo_filter_yystate, pParse->fdo_filter_yychar, fdo_filter_yys);
        }
#endif
    }
    if ((fdo_filter_yyn = fdo_filter_yysindex[fdo_filter_yystate]) && (fdo_filter_yyn += pParse->fdo_filter_yychar) >= 0 &&
            fdo_filter_yyn <= FDO_FILTER_YYTABLESIZE && fdo_filter_yycheck[fdo_filter_yyn] == pParse->fdo_filter_yychar)
    {
#if FDO_FILTER_YYDEBUG
        if (pParse->fdo_filter_yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    FDO_FILTER_YYPREFIX, fdo_filter_yystate, fdo_filter_yytable[fdo_filter_yyn]);
#endif
        if (pParse->fdo_filter_yyssp >= pParse->fdo_filter_yysslim && fdo_filter_yygrowstack(pParse))
        {
            goto fdo_filter_yyoverflow;
        }
        *++pParse->fdo_filter_yyssp = fdo_filter_yystate = fdo_filter_yytable[fdo_filter_yyn];
        *++pParse->fdo_filter_yyvsp = pParse->fdo_filter_yylval;
        pParse->fdo_filter_yychar = (-1);
        if (pParse->fdo_filter_yyerrflag > 0)  --pParse->fdo_filter_yyerrflag;
        goto fdo_filter_yyloop;
    }
    if ((fdo_filter_yyn = fdo_filter_yyrindex[fdo_filter_yystate]) && (fdo_filter_yyn += pParse->fdo_filter_yychar) >= 0 &&
            fdo_filter_yyn <= FDO_FILTER_YYTABLESIZE && fdo_filter_yycheck[fdo_filter_yyn] == pParse->fdo_filter_yychar)
    {
        fdo_filter_yyn = fdo_filter_yytable[fdo_filter_yyn];
        goto fdo_filter_yyreduce;
    }
    if (pParse->fdo_filter_yyerrflag) goto fdo_filter_yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto fdo_filter_yynewerror;
#endif
fdo_filter_yynewerror:
   fdo_filter_yyerror( pParse,"syntax error");
#if defined(lint) || defined(__GNUC__)
    goto fdo_filter_yyerrlab;
#endif
fdo_filter_yyerrlab:
    ++pParse->fdo_filter_yynerrs;
fdo_filter_yyinrecovery:
    if (pParse->fdo_filter_yyerrflag < 3)
    {
        pParse->fdo_filter_yyerrflag = 3;
        for (;;)
        {
            if ((fdo_filter_yyn = fdo_filter_yysindex[*pParse->fdo_filter_yyssp]) && (fdo_filter_yyn += FDO_FILTER_YYERRCODE) >= 0 &&
                    fdo_filter_yyn <= FDO_FILTER_YYTABLESIZE && fdo_filter_yycheck[fdo_filter_yyn] == FDO_FILTER_YYERRCODE)
            {
#if FDO_FILTER_YYDEBUG
                if (pParse->fdo_filter_yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", FDO_FILTER_YYPREFIX, *pParse->fdo_filter_yyssp, fdo_filter_yytable[fdo_filter_yyn]);
#endif
                if (pParse->fdo_filter_yyssp >= pParse->fdo_filter_yysslim && fdo_filter_yygrowstack(pParse))
                {
                    goto fdo_filter_yyoverflow;
                }
                *++pParse->fdo_filter_yyssp = fdo_filter_yystate = fdo_filter_yytable[fdo_filter_yyn];
                *++pParse->fdo_filter_yyvsp = pParse->fdo_filter_yylval;
                goto fdo_filter_yyloop;
            }
            else
            {
#if FDO_FILTER_YYDEBUG
                if (pParse->fdo_filter_yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            FDO_FILTER_YYPREFIX, *pParse->fdo_filter_yyssp);
#endif
                if (pParse->fdo_filter_yyssp <= pParse->fdo_filter_yyss) goto fdo_filter_yyabort;
                --pParse->fdo_filter_yyssp;
                --pParse->fdo_filter_yyvsp;
            }
        }
    }
    else
    {
        if (pParse->fdo_filter_yychar == 0) goto fdo_filter_yyabort;
#if FDO_FILTER_YYDEBUG
        if (pParse->fdo_filter_yydebug)
        {
            fdo_filter_yys = 0;
            if (pParse->fdo_filter_yychar <= FDO_FILTER_YYMAXTOKEN) fdo_filter_yys = fdo_filter_yyname[pParse->fdo_filter_yychar];
            if (!fdo_filter_yys) fdo_filter_yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    FDO_FILTER_YYPREFIX, fdo_filter_yystate, pParse->fdo_filter_yychar, fdo_filter_yys);
        }
#endif
        pParse->fdo_filter_yychar = (-1);
        goto fdo_filter_yyloop;
    }
fdo_filter_yyreduce:
#if FDO_FILTER_YYDEBUG
    if (pParse->fdo_filter_yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                FDO_FILTER_YYPREFIX, fdo_filter_yystate, fdo_filter_yyn, fdo_filter_yyrule[fdo_filter_yyn]);
#endif
    fdo_filter_yym = fdo_filter_yylen[fdo_filter_yyn];
    pParse->fdo_filter_yyval = pParse->fdo_filter_yyvsp[1-fdo_filter_yym];
    switch (fdo_filter_yyn)
    {
case 1:
#line 121 "Src/Fdo/Parse/yyFilter.y"
{pParse->SetRoot(Node_Copy(L"Filter", pParse->fdo_filter_yyvsp[0].m_node));	FDO_FILTER_YYACCEPT;}
break;
case 2:
#line 122 "Src/Fdo/Parse/yyFilter.y"
{pParse->SetRoot(Node_Copy(L"Filter", pParse->fdo_filter_yyvsp[0].m_node)); FDO_FILTER_YYACCEPT;}
break;
case 3:
#line 123 "Src/Fdo/Parse/yyFilter.y"
{Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	FDO_FILTER_YYABORT;}
break;
case 4:
#line 129 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"(Expression)", pParse->fdo_filter_yyvsp[-1].m_node);}
break;
case 5:
#line 130 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"UnaryExpression", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 6:
#line 131 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"BinaryExpression", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 7:
#line 132 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"Function", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 8:
#line 133 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"Identifier", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 9:
#line 134 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"ValueExpression", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 10:
#line 138 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Add, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 11:
#line 139 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Subtract, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 12:
#line 140 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Multiply, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 13:
#line 141 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Divide, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 14:
#line 146 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"boolean", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 15:
#line 148 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"datetime", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 16:
#line 151 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"double", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 17:
#line 153 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"integer", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 18:
#line 155 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"string", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 19:
#line 157 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"TODO:BLOB", NULL);}
break;
case 20:
#line 159 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"TODO:CLOB", NULL);}
break;
case 21:
#line 161 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
break;
case 22:
#line 165 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
break;
case 23:
#line 166 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
break;
case 24:
#line 169 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"STRING", FdoDataValue::Create(pParse->fdo_filter_yyvsp[0].m_string));}
break;
case 25:
#line 172 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"INTEGER", FdoInt32Value::Create(pParse->fdo_filter_yyvsp[0].m_integer));}
break;
case 26:
#line 175 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"DOUBLE", FdoDoubleValue::Create(pParse->fdo_filter_yyvsp[0].m_double));}
break;
case 27:
#line 178 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"DateTime", FdoDataValue::Create(pParse->fdo_filter_yyvsp[0].m_datetime));}
break;
case 28:
#line 183 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node)->GetName(), (FdoExpressionCollection*)pParse->fdo_filter_yyvsp[-1].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-3].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-1].m_node);}
break;
case 29:
#line 186 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
break;
case 30:
#line 187 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"Expression Arg 1,");FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node); }
break;
case 31:
#line 188 "Src/Fdo/Parse/yyFilter.y"
{((FdoExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"Expression Arg N,");FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node); }
break;
case 32:
#line 193 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue(pParse->fdo_filter_yyvsp[-1].m_string));}
break;
case 33:
#line 198 "Src/Fdo/Parse/yyFilter.y"
{
						    FdoComputedIdentifier* id = pParse->FindComputedIdentifier( pParse->fdo_filter_yyvsp[0].m_string );
						    if( id == NULL )
						        pParse->fdo_filter_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(pParse->fdo_filter_yyvsp[0].m_string));
						    else
						        pParse->fdo_filter_yyval.m_node = id;
						}
break;
case 34:
#line 208 "Src/Fdo/Parse/yyFilter.y"
{}
break;
case 35:
#line 209 "Src/Fdo/Parse/yyFilter.y"
{}
break;
case 36:
#line 214 "Src/Fdo/Parse/yyFilter.y"
{
                        FdoComputedIdentifier* id = FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-3].m_node);
                        Node_Add(L"ComputedIdentifier", id);
                        pParse->AddCompIdentifier(id);FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-3].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);
                        id->Release();
                    }
break;
case 37:
#line 223 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"LiteralValue", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 38:
#line 224 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"Parameter", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 39:
#line 228 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"GeometryValue", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 40:
#line 229 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"DataValue", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 41:
#line 234 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"PARAMETER", FdoParameter::Create(pParse->fdo_filter_yyvsp[0].m_string));}
break;
case 42:
#line 236 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L":Parameter", FdoParameter::Create(pParse->fdo_filter_yyvsp[0].m_string));}
break;
case 43:
#line 240 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 44:
#line 245 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"(Filter)", pParse->fdo_filter_yyvsp[-1].m_node);}
break;
case 45:
#line 246 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"LogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 46:
#line 247 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"SearchCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 47:
#line 251 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"BinaryLogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 48:
#line 252 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"UnaryLogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 49:
#line 256 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"DistanceCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 50:
#line 257 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"SpatialCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 51:
#line 261 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoValueExpressionCollection::Create()); ((FdoValueExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoValueExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"ValueExpression 1");FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node); }
break;
case 52:
#line 263 "Src/Fdo/Parse/yyFilter.y"
{((FdoValueExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoValueExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"ValueExpression n");FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node); }
break;
case 53:
#line 268 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"ValueExpressionCollection", FdoInCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-4].m_node, (FdoValueExpressionCollection*)pParse->fdo_filter_yyvsp[-1].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-4].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-1].m_node);}
break;
case 54:
#line 272 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"NULL", FdoNullCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-1].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-1].m_node);}
break;
case 55:
#line 276 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"InCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 56:
#line 277 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"ComparisonCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 57:
#line 278 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"GeometricCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 58:
#line 279 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"NullCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 59:
#line 284 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Id-SpatialOp-Expression", FdoSpatialCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-2].m_node, (FdoSpatialOperations)pParse->fdo_filter_yyvsp[-1].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 60:
#line 288 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"NOT", FdoUnaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node, FdoUnaryLogicalOperations_Not));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 61:
#line 292 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"AND", FdoBinaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryLogicalOperations_And, (FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 62:
#line 293 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"OR", FdoBinaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryLogicalOperations_Or, (FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 63:
#line 297 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"ComparisonOperations", FdoComparisonCondition::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, (FdoComparisonOperations)pParse->fdo_filter_yyvsp[-1].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 64:
#line 302 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Indentifier-DistanceOp-Expression", FdoDistanceCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node, (FdoDistanceOperations)pParse->fdo_filter_yyvsp[-2].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[-1].m_node, ((FdoDoubleValue*)pParse->fdo_filter_yyvsp[0].m_node)->GetDouble()));FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-3].m_node); FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[-1].m_node);FDO_SAFE_RELEASE(pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 65:
#line 306 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_EqualTo; Node_Trace(L"EQ");}
break;
case 66:
#line 307 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_NotEqualTo; Node_Trace(L"NE");}
break;
case 67:
#line 308 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_GreaterThan; Node_Trace(L"GT");}
break;
case 68:
#line 309 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_GreaterThanOrEqualTo; Node_Trace(L"GE");}
break;
case 69:
#line 310 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_LessThan; Node_Trace(L"LT");}
break;
case 70:
#line 311 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_LessThanOrEqualTo; Node_Trace(L"LE");}
break;
case 71:
#line 312 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_Like; Node_Trace(L"LIKE");}
break;
case 72:
#line 316 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoDistanceOperations_Beyond; Node_Trace(L"BEYOND");}
break;
case 73:
#line 317 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoDistanceOperations_Within; Node_Trace(L"WITHINDISTANCE");}
break;
case 74:
#line 321 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Contains; Node_Trace(L"CONTAINS");}
break;
case 75:
#line 322 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Crosses; Node_Trace(L"CROSSES");}
break;
case 76:
#line 323 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Disjoint; Node_Trace(L"DISJOINT");}
break;
case 77:
#line 324 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Equals; Node_Trace(L"EQUALS");}
break;
case 78:
#line 325 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Inside; Node_Trace(L"INSIDE");}
break;
case 79:
#line 326 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Intersects; Node_Trace(L"INTERSECTS");}
break;
case 80:
#line 327 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Overlaps; Node_Trace(L"OVERLAPS");}
break;
case 81:
#line 328 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Touches; Node_Trace(L"TOUCHES");}
break;
case 82:
#line 329 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Within; Node_Trace(L"WITHIN");}
break;
case 83:
#line 330 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_CoveredBy; Node_Trace(L"COVEREDBY");}
break;
case 84:
#line 331 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_EnvelopeIntersects; Node_Trace(L"ENVELOPEINTERSECTS");}
break;
case 85:
#line 335 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"double", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 86:
#line 336 "Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"INTEGER", FdoDoubleValue::Create((double)pParse->fdo_filter_yyvsp[0].m_integer));}
break;
#line 1022 "yyFilter.cpp"
    }
    pParse->fdo_filter_yyssp -= fdo_filter_yym;
    fdo_filter_yystate = *pParse->fdo_filter_yyssp;
    pParse->fdo_filter_yyvsp -= fdo_filter_yym;
    fdo_filter_yym = fdo_filter_yylhs[fdo_filter_yyn];
    if (fdo_filter_yystate == 0 && fdo_filter_yym == 0)
    {
#if FDO_FILTER_YYDEBUG
        if (pParse->fdo_filter_yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", FDO_FILTER_YYPREFIX, FDO_FILTER_YYFINAL);
#endif
        fdo_filter_yystate = FDO_FILTER_YYFINAL;
        *++pParse->fdo_filter_yyssp = FDO_FILTER_YYFINAL;
        *++pParse->fdo_filter_yyvsp = pParse->fdo_filter_yyval;
        if (pParse->fdo_filter_yychar < 0)
        {
            if ((pParse->fdo_filter_yychar = fdo_filter_yylex(pParse)) < 0) pParse->fdo_filter_yychar = 0;
#if FDO_FILTER_YYDEBUG
            if (pParse->fdo_filter_yydebug)
            {
                fdo_filter_yys = 0;
                if (pParse->fdo_filter_yychar <= FDO_FILTER_YYMAXTOKEN) fdo_filter_yys = fdo_filter_yyname[pParse->fdo_filter_yychar];
                if (!fdo_filter_yys) fdo_filter_yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        FDO_FILTER_YYPREFIX, FDO_FILTER_YYFINAL, pParse->fdo_filter_yychar, fdo_filter_yys);
            }
#endif
        }
        if (pParse->fdo_filter_yychar == 0) goto fdo_filter_yyaccept;
        goto fdo_filter_yyloop;
    }
    if ((fdo_filter_yyn = fdo_filter_yygindex[fdo_filter_yym]) && (fdo_filter_yyn += fdo_filter_yystate) >= 0 &&
            fdo_filter_yyn <= FDO_FILTER_YYTABLESIZE && fdo_filter_yycheck[fdo_filter_yyn] == fdo_filter_yystate)
        fdo_filter_yystate = fdo_filter_yytable[fdo_filter_yyn];
    else
        fdo_filter_yystate = fdo_filter_yydgoto[fdo_filter_yym];
#if FDO_FILTER_YYDEBUG
    if (pParse->fdo_filter_yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", FDO_FILTER_YYPREFIX, *pParse->fdo_filter_yyssp, fdo_filter_yystate);
#endif
    if (pParse->fdo_filter_yyssp >= pParse->fdo_filter_yysslim && fdo_filter_yygrowstack(pParse))
    {
        goto fdo_filter_yyoverflow;
    }
    *++pParse->fdo_filter_yyssp = fdo_filter_yystate;
    *++pParse->fdo_filter_yyvsp = pParse->fdo_filter_yyval;
    goto fdo_filter_yyloop;
fdo_filter_yyoverflow:
   fdo_filter_yyerror( pParse,"yacc stack overflow");
fdo_filter_yyabort:
    return (1);
fdo_filter_yyaccept:
    return (0);
}
