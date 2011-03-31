#ifndef lint
static char const 
fdo_expression_yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define FDO_EXPRESSION_YYBYACC 1
#define FDO_EXPRESSION_YYMAJOR 1
#define FDO_EXPRESSION_YYMINOR 9
#define FDO_EXPRESSION_YYLEX fdo_expression_yylex(pParse)
#define FDO_EXPRESSION_YYEMPTY -1
#define fdo_expression_yyclearin (pParse->fdo_expression_yychar=(FDO_EXPRESSION_YYEMPTY))
#define fdo_expression_yyerrok (pParse->fdo_expression_yyerrflag=0)
#define FDO_EXPRESSION_YYRECOVERING() (pParse->fdo_expression_yyerrflag!=0)
class FdoParse; static int fdo_expression_yygrowstack(FdoParse *pParse);
#define FDO_EXPRESSION_YYPREFIX "fdo_expression_yy"
#line 36 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"

#include "Parse.h"
#pragma warning(disable:4102)	/* unreferenced labels in fdo_expression_yyExpression.cpp*/

#line 43 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
#if 0
{
	FdoIDisposable*	m_node;		/* expression or filter parse tree node*/
	FdoInt32		m_id;		/* enumerations, keywords, ...*/
	FdoInt32		m_integer;	/* integer values (Int8, Int16, Int32 )*/
	FdoInt64		m_int64;	/* 64-bit integer values*/
	double			m_double;	/* floating point values (single(float), double)*/
	FdoString*		m_string;	/* string*/
	_FdoDateTime	m_datetime;	/* date time*/
#endif
#line 33 "yyExpression.cpp"
#define FDO_EXPRESSION_YYERRCODE 256
#define FdoToken_NULL 257
#define FdoToken_TRUE 258
#define FdoToken_FALSE 259
#define FdoToken_DATE 260
#define FdoToken_TIME 261
#define FdoToken_TIMESTAMP 262
#define FdoToken_GEOMFROMTEXT 263
#define FdoToken_JOINNONE 264
#define FdoToken_JOININNER 265
#define FdoToken_JOINRIGHTOUTER 266
#define FdoToken_JOINLEFTOUTER 267
#define FdoToken_JOINFULLOUTER 268
#define FdoToken_JOINCROSS 269
#define FdoToken_AND 270
#define FdoToken_OR 271
#define FdoToken_NOT 272
#define FdoToken_LIKE 273
#define FdoToken_IN 274
#define FdoToken_SELECT 275
#define FdoToken_JOIN 276
#define FdoToken_BEYOND 277
#define FdoToken_WITHINDISTANCE 278
#define FdoToken_CONTAINS 279
#define FdoToken_COVEREDBY 280
#define FdoToken_CROSSES 281
#define FdoToken_DISJOINT 282
#define FdoToken_ENVELOPEINTERSECTS 283
#define FdoToken_EQUALS 284
#define FdoToken_INTERSECTS 285
#define FdoToken_INSIDE 286
#define FdoToken_OVERLAPS 287
#define FdoToken_TOUCHES 288
#define FdoToken_WITHIN 289
#define FdoToken_RELATE 290
#define FdoToken_IDENTIFIER 291
#define FdoToken_PARAMETER 292
#define FdoToken_STRING 293
#define FdoToken_INTEGER 294
#define FdoToken_INT64 295
#define FdoToken_INTHEX 296
#define FdoToken_INTBIN 297
#define FdoToken_DOUBLE 298
#define FdoToken_DATETIME 299
#define FdoToken_BLOB 300
#define FdoToken_CLOB 301
#define FdoToken_Add 302
#define FdoToken_Subtract 303
#define FdoToken_Multiply 304
#define FdoToken_Divide 305
#define FdoToken_Negate 306
#define FdoToken_EQ 307
#define FdoToken_NE 308
#define FdoToken_GT 309
#define FdoToken_GE 310
#define FdoToken_LT 311
#define FdoToken_LE 312
#define FdoToken_LeftParenthesis 313
#define FdoToken_RightParenthesis 314
#define FdoToken_Comma 315
#define FdoToken_Dot 316
#define FdoToken_Colon 317
#define FdoToken_AS 318
#define FdoToken_BETWEEN 319
const short fdo_expression_yylhs[] = {                                        -1,
    0,    0,    3,    3,    3,    3,    3,    3,    3,    1,
    1,    1,    1,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,   21,   21,   20,   22,   23,   24,
   25,   19,   26,    6,   11,   11,   12,   12,   12,   12,
   12,   17,   17,   17,   17,    5,    5,   10,   10,   10,
   10,    9,    9,    9,    9,    4,    4,    4,    7,    8,
   13,   13,   14,   14,   15,   15,   16,   18,   18,   18,
   18,   18,   18,
};
const short fdo_expression_yylen[] = {                                         2,
    1,    1,    3,    1,    1,    1,    1,    1,    1,    3,
    3,    3,    3,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    4,    5,    3,    5,    3,    1,    5,
    3,    6,    8,    6,    8,    1,    3,    6,    8,    8,
   10,    5,    3,    5,    3,    0,    1,    3,    4,    1,
    1,    1,    1,    1,    1,    2,    2,    1,    1,    1,
    1,    1,    1,
};
const short fdo_expression_yydefred[] = {                                      0,
    2,   24,   25,   26,    0,    0,   60,   65,   27,   28,
   29,   30,   31,   32,   33,   22,   23,    0,    0,    0,
    0,    5,   64,    0,    6,   63,    0,    9,    0,    8,
   61,   62,    4,   16,   21,   14,   17,   18,   19,   20,
   15,    0,    0,    0,   66,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   53,    0,    0,   55,   59,    0,    0,    0,
   34,    0,    0,    0,    0,    0,   52,   54,    0,    0,
    0,    0,    0,   48,    0,    0,    0,    0,   38,    0,
    0,   46,    0,    0,    0,    0,   50,    0,   49,    0,
    0,   37,    0,    0,    0,   47,    0,    0,    0,    0,
    0,   51,    0,   36,   68,   69,   70,   71,   72,   73,
    0,    0,    0,   42,    0,   44,    0,   35,    0,    0,
   43,   45,
};
const short fdo_expression_yydgoto[] = {                                      21,
   22,   23,   24,   65,   91,   25,   26,   27,   28,   29,
  105,   76,   30,   31,   32,   33,   92,  121,   34,   35,
   36,   37,   38,   39,   40,   41,
};
const short fdo_expression_yysindex[] = {                                    -38,
    0,    0,    0,    0,  -30,  -17,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   71, -287,   71,
    0,    0,    0, -277,    0,    0,   -9,    0, -279,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -249, -243, -262,    0,  -22,  -34,   71,   71,   71,
   71, -243,   71, -243,   19,   23, -247, -242, -271, -271,
 -262, -262,    0, -277,  -24,    0,    0,   98, -243, -243,
    0,   71,   71, -267,  -40,   -1,    0,    0, -277,   33,
   65, -243, -243,    0, -272, -240, -230,    0,    0,   53,
   11,    0,   17, -243, -243,  -26,    0, -163,    0, -163,
    0,    0, -243,  -41,   70,    0,   51,   92, -243, -167,
 -167,    0, -184,    0,    0,    0,    0,    0,    0,    0,
   77,   78, -243,    0, -158,    0, -158,    0,   95,   96,
    0,    0,
};
const short fdo_expression_yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  139,    0,    0,    1,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    5,    0,    0,    0,    0,    0,    0,
    0,    0,   79,    0,    0,    0,    9,    0,   22,   24,
   13,   18,    0,   84,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -12,    0,    0,    0,   85,    0,
 -222,    0,    0,    0,    0, -218,    0,  -33,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -29,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
const short fdo_expression_yygindex[] = {                                      0,
    0,    0,   59,    0,   40,    0,    0,   21,    0,   -4,
    0,    0,    0,    0,    0,    0,   43,   31,    0,  -55,
    0,    0,    0,    0,    0,    0,
};
#define FDO_EXPRESSION_YYTABLESIZE 415
const short fdo_expression_yytable[] = {                                      53,
    7,   20,  110,   90,   67,   45,   58,   41,    3,   42,
   41,   40,   12,  103,   40,   47,   71,   13,   57,   72,
    9,   10,   43,   11,   48,   49,   50,   51,   39,   93,
   53,   39,   50,   51,   48,   49,   50,   51,   54,   84,
   52,    7,   85,   55,    7,   67,   52,    7,   67,    3,
   82,   97,    3,   12,   98,   52,   12,   99,   13,   67,
  100,   13,   10,   56,   11,   10,   68,   11,   47,  129,
   69,  130,   63,   86,   66,   70,   44,   94,   46,    7,
    7,    7,    7,    3,    3,    3,    3,   95,   75,   77,
   78,  112,   96,   81,   98,    7,  115,  116,  117,  118,
  119,  120,   88,   89,   53,   87,   59,   60,   61,   62,
   20,   64,   90,  111,  101,  102,  104,  124,  126,   56,
  125,  127,   56,  108,   57,   58,   74,   57,   58,  114,
   79,   80,  113,  123,    9,  131,  132,   73,    1,  107,
  106,  122,    0,  128,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    1,    2,    3,
    4,    0,    0,    0,    5,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    6,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    7,    8,    9,   10,   11,   12,   13,   14,
   15,   16,   17,    0,    7,    0,    0,   18,   53,   53,
   53,   53,   52,   52,   52,   52,  109,   83,   19,   48,
   49,   50,   51,   54,   53,    0,    0,    0,   52,    7,
    7,    7,    7,    0,    0,   52,    0,    0,    0,    0,
    0,    0,    7,    7,    7,    7,   67,   67,   67,   67,
    3,    3,    3,    3,   12,   12,   12,   12,    7,   13,
   13,   13,   13,   10,   10,   11,   11,    2,    3,    4,
    0,    0,    0,    5,   48,   49,   50,   51,    0,    0,
    0,    0,    0,    0,    0,    6,    0,    0,    0,    0,
   52,    0,    0,    0,    2,    3,    4,    0,    0,    0,
    5,    7,    8,    9,   10,   11,   12,   13,   14,   15,
   16,   17,    6,    0,    0,    0,   18,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   19,    7,    8,
    9,   10,   11,   12,   13,   14,   15,   16,   17,    0,
    0,    0,    0,   18,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   19,
};
const short fdo_expression_yycheck[] = {                                      40,
    0,   40,   44,  276,    0,  293,   41,   41,    0,   40,
   44,   41,    0,   40,   44,   20,   41,    0,   41,   44,
  293,    0,   40,    0,  302,  303,  304,  305,   41,   85,
   40,   44,  304,  305,  302,  303,  304,  305,  318,   41,
  318,   41,   44,  293,   44,   41,  318,  291,   44,   41,
  318,   41,   44,   41,   44,  318,   44,   41,   41,   41,
   44,   44,   41,   43,   41,   44,   44,   44,   73,  125,
  318,  127,   52,   41,   54,  318,   18,  318,   20,  302,
  303,  304,  305,  302,  303,  304,  305,  318,   68,   69,
   70,   41,   40,   73,   44,  318,  264,  265,  266,  267,
  268,  269,   82,   83,   40,   41,   48,   49,   50,   51,
   40,   53,  276,   44,   94,   95,   96,   41,   41,   41,
   44,   44,   44,  103,   41,   41,   68,   44,   44,  109,
   72,   73,   41,  318,  293,   41,   41,   40,    0,  100,
   98,  111,   -1,  123,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  256,  257,  258,
  259,   -1,   -1,   -1,  263,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  275,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  291,  292,  293,  294,  295,  296,  297,  298,
  299,  300,  301,   -1,  291,   -1,   -1,  306,  302,  303,
  304,  305,  302,  303,  304,  305,  318,  318,  317,  302,
  303,  304,  305,  318,  318,   -1,   -1,   -1,  318,  302,
  303,  304,  305,   -1,   -1,  318,   -1,   -1,   -1,   -1,
   -1,   -1,  302,  303,  304,  305,  302,  303,  304,  305,
  302,  303,  304,  305,  302,  303,  304,  305,  318,  302,
  303,  304,  305,  302,  303,  302,  303,  257,  258,  259,
   -1,   -1,   -1,  263,  302,  303,  304,  305,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  275,   -1,   -1,   -1,   -1,
  318,   -1,   -1,   -1,  257,  258,  259,   -1,   -1,   -1,
  263,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  275,   -1,   -1,   -1,  306,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  317,  291,  292,
  293,  294,  295,  296,  297,  298,  299,  300,  301,   -1,
   -1,   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  317,
};
#define FDO_EXPRESSION_YYFINAL 21
#ifndef FDO_EXPRESSION_YYDEBUG
#define FDO_EXPRESSION_YYDEBUG 0
#endif
#define FDO_EXPRESSION_YYMAXTOKEN 319
#if FDO_EXPRESSION_YYDEBUG
const char * const fdo_expression_yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"FdoToken_NULL",
"FdoToken_TRUE","FdoToken_FALSE","FdoToken_DATE","FdoToken_TIME",
"FdoToken_TIMESTAMP","FdoToken_GEOMFROMTEXT","FdoToken_JOINNONE",
"FdoToken_JOININNER","FdoToken_JOINRIGHTOUTER","FdoToken_JOINLEFTOUTER",
"FdoToken_JOINFULLOUTER","FdoToken_JOINCROSS","FdoToken_AND","FdoToken_OR",
"FdoToken_NOT","FdoToken_LIKE","FdoToken_IN","FdoToken_SELECT","FdoToken_JOIN",
"FdoToken_BEYOND","FdoToken_WITHINDISTANCE","FdoToken_CONTAINS",
"FdoToken_COVEREDBY","FdoToken_CROSSES","FdoToken_DISJOINT",
"FdoToken_ENVELOPEINTERSECTS","FdoToken_EQUALS","FdoToken_INTERSECTS",
"FdoToken_INSIDE","FdoToken_OVERLAPS","FdoToken_TOUCHES","FdoToken_WITHIN",
"FdoToken_RELATE","FdoToken_IDENTIFIER","FdoToken_PARAMETER","FdoToken_STRING",
"FdoToken_INTEGER","FdoToken_INT64","FdoToken_INTHEX","FdoToken_INTBIN",
"FdoToken_DOUBLE","FdoToken_DATETIME","FdoToken_BLOB","FdoToken_CLOB",
"FdoToken_Add","FdoToken_Subtract","FdoToken_Multiply","FdoToken_Divide",
"FdoToken_Negate","FdoToken_EQ","FdoToken_NE","FdoToken_GT","FdoToken_GE",
"FdoToken_LT","FdoToken_LE","FdoToken_LeftParenthesis",
"FdoToken_RightParenthesis","FdoToken_Comma","FdoToken_Dot","FdoToken_Colon",
"FdoToken_AS","FdoToken_BETWEEN",
};
const char * const fdo_expression_yyrule[] = {
"$accept : fdo",
"fdo : Expression",
"fdo : error",
"Expression : '(' Expression ')'",
"Expression : UnaryExpression",
"Expression : BinaryExpression",
"Expression : Function",
"Expression : Identifier",
"Expression : ValueExpression",
"Expression : ComputedIdentifier",
"BinaryExpression : Expression FdoToken_Add Expression",
"BinaryExpression : Expression FdoToken_Subtract Expression",
"BinaryExpression : Expression FdoToken_Multiply Expression",
"BinaryExpression : Expression FdoToken_Divide Expression",
"DataValue : boolean",
"DataValue : datetime",
"DataValue : double",
"DataValue : integer",
"DataValue : int64",
"DataValue : inthex",
"DataValue : intbin",
"DataValue : string",
"DataValue : FdoToken_BLOB",
"DataValue : FdoToken_CLOB",
"DataValue : FdoToken_NULL",
"boolean : FdoToken_TRUE",
"boolean : FdoToken_FALSE",
"string : FdoToken_STRING",
"integer : FdoToken_INTEGER",
"int64 : FdoToken_INT64",
"inthex : FdoToken_INTHEX",
"intbin : FdoToken_INTBIN",
"double : FdoToken_DOUBLE",
"datetime : FdoToken_DATETIME",
"Function : Identifier '(' ExpressionCollection ')'",
"ComputedIdentifierOnly : '(' Identifier ')' FdoToken_AS Identifier",
"ComputedIdentifierOnly : Identifier FdoToken_AS Identifier",
"ComputedIdentifier3 : '(' Identifier ')' FdoToken_AS Identifier",
"ComputedIdentifier3 : Identifier FdoToken_AS Identifier",
"ComputedIdentifier3 : Identifier",
"ComputedIdentifier3 : '(' Expression ')' FdoToken_AS Identifier",
"ComputedIdentifier3 : Expression FdoToken_AS Identifier",
"JoinCriteria : FdoToken_JOIN '(' Identifier ',' JoinOperations ')'",
"JoinCriteria : FdoToken_JOIN '(' Identifier ',' JoinOperations ',' string ')'",
"JoinCriteria : FdoToken_JOIN '(' ComputedIdentifierOnly ',' JoinOperations ')'",
"JoinCriteria : FdoToken_JOIN '(' ComputedIdentifierOnly ',' JoinOperations ',' string ')'",
"JoinCriteriaCollection : JoinCriteria",
"JoinCriteriaCollection : JoinCriteriaCollection ',' JoinCriteria",
"SubSelectExpression : FdoToken_SELECT '(' Identifier ',' ComputedIdentifier3 ')'",
"SubSelectExpression : FdoToken_SELECT '(' Identifier ',' ComputedIdentifier3 ',' string ')'",
"SubSelectExpression : FdoToken_SELECT '(' Identifier ',' ComputedIdentifier3 ',' JoinCriteriaCollection ')'",
"SubSelectExpression : FdoToken_SELECT '(' Identifier ',' ComputedIdentifier3 ',' string ',' JoinCriteriaCollection ')'",
"ComputedIdentifier : '(' Expression ')' FdoToken_AS Identifier",
"ComputedIdentifier : Expression FdoToken_AS Identifier",
"ComputedIdentifier : '(' SubSelectExpression ')' FdoToken_AS Identifier",
"ComputedIdentifier : SubSelectExpression FdoToken_AS Identifier",
"ExpressionCollection :",
"ExpressionCollection : Expression",
"ExpressionCollection : ExpressionCollection ',' Expression",
"GeometryValue : FdoToken_GEOMFROMTEXT '(' FdoToken_STRING ')'",
"Identifier : FdoToken_IDENTIFIER",
"ValueExpression : LiteralValue",
"ValueExpression : Parameter",
"LiteralValue : GeometryValue",
"LiteralValue : DataValue",
"Parameter : FdoToken_PARAMETER",
"Parameter : FdoToken_Colon FdoToken_STRING",
"UnaryExpression : FdoToken_Negate Expression",
"JoinOperations : FdoToken_JOINNONE",
"JoinOperations : FdoToken_JOININNER",
"JoinOperations : FdoToken_JOINRIGHTOUTER",
"JoinOperations : FdoToken_JOINLEFTOUTER",
"JoinOperations : FdoToken_JOINFULLOUTER",
"JoinOperations : FdoToken_JOINCROSS",
};
#endif
#if FDO_EXPRESSION_YYDEBUG
#include <stdio.h>
#endif
#ifdef FDO_EXPRESSION_YYSTACKSIZE
#undef FDO_EXPRESSION_YYMAXDEPTH
#define FDO_EXPRESSION_YYMAXDEPTH FDO_EXPRESSION_YYSTACKSIZE
#else
#ifdef FDO_EXPRESSION_YYMAXDEPTH
#define FDO_EXPRESSION_YYSTACKSIZE FDO_EXPRESSION_YYMAXDEPTH
#else
#define FDO_EXPRESSION_YYSTACKSIZE 10000
#define FDO_EXPRESSION_YYMAXDEPTH 10000
#endif
#endif
#define FDO_EXPRESSION_YYINITSTACKSIZE 200
#line 294 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
///////////////////////////////////////////////////////////////////////

#include <stdio.h>

FdoInt32 fdo_expression_yyerror_default(char *str, FdoInt32 lineno)
{
#ifdef FDO_FDO_EXPRESSION_YYDEBUG
	fprintf(stderr, "line %d: %s\n", lineno, str);
#endif
	return 0;
}

typedef FdoInt32 (*ERRFUNC)(char*, FdoInt32);
static ERRFUNC fdo_expression_yyerror_func = fdo_expression_yyerror_default;

FdoInt32 fdo_expression_yyerror(FdoParse *pParse, char* str)
{
	return (*fdo_expression_yyerror_func)(str, pParse->fdo_expression_yylineno);
}

ERRFUNC fdo_expression_yyerror_set(ERRFUNC func)
{
	ERRFUNC old_func = fdo_expression_yyerror_func;

	if (func != NULL)
		fdo_expression_yyerror_func = func;

	return old_func;
}

static void  print_current_token()
{
//TODO: this has not been updated, but it is unused anyway
#ifdef FDO_FDO_EXPRESSION_YYDEBUG
	char  *token_name;
	
	if( pParse->fdo_expression_yychar <= FDO_EXPRESSION_YYMAXTOKEN )
	{
		switch( pParse->fdo_expression_yychar )
		{
			case IDENTIFIER:
				token_name = pParse->fdo_expression_yylval.id;
				break;

			default:
				token_name = fdo_expression_yyname[pParse->fdo_expression_yychar];
				break;
		}
	}
	else
	{
		token_name = "illegal-name";
	}

	printf( "***  current token is %d (%s)  ***\n", pParse->fdo_expression_yychar, token_name );
#endif
}

#line 460 "yyExpression.cpp"
/* allocate initial stack or double stack size, up to FDO_EXPRESSION_YYMAXDEPTH */
static int fdo_expression_yygrowstack(FdoParse *pParse)
{
    int newsize, i;
    short *newss;
    FDO_EXPRESSION_YYSTYPE *newvs;

    if ((newsize = pParse->fdo_expression_yystacksize) == 0)
        newsize = FDO_EXPRESSION_YYINITSTACKSIZE;
    else if (newsize >= FDO_EXPRESSION_YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > FDO_EXPRESSION_YYMAXDEPTH)
        newsize = FDO_EXPRESSION_YYMAXDEPTH;
    i = pParse->fdo_expression_yyssp - pParse->fdo_expression_yyss;
    newss = pParse->fdo_expression_yyss ? (short *)realloc(pParse->fdo_expression_yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    pParse->fdo_expression_yyss = newss;
    pParse->fdo_expression_yyssp = newss + i;
    newvs = pParse->fdo_expression_yyvs ? (FDO_EXPRESSION_YYSTYPE *)realloc(pParse->fdo_expression_yyvs, newsize * sizeof *newvs) :
      (FDO_EXPRESSION_YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    pParse->fdo_expression_yyvs = newvs;
    pParse->fdo_expression_yyvsp = newvs + i;
    pParse->fdo_expression_yystacksize = newsize;
    pParse->fdo_expression_yysslim = pParse->fdo_expression_yyss + newsize - 1;
    return 0;
}

#define FDO_EXPRESSION_YYABORT goto fdo_expression_yyabort
#define FDO_EXPRESSION_YYREJECT goto fdo_expression_yyabort
#define FDO_EXPRESSION_YYACCEPT goto fdo_expression_yyaccept
#define FDO_EXPRESSION_YYERROR goto fdo_expression_yyerrlab

#ifndef FDO_EXPRESSION_YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define FDO_EXPRESSION_YYPARSE_PARAM_ARG void
#define FDO_EXPRESSION_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define FDO_EXPRESSION_YYPARSE_PARAM_ARG
#define FDO_EXPRESSION_YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* FDO_EXPRESSION_YYPARSE_PARAM */
#ifndef FDO_EXPRESSION_YYPARSE_PARAM_TYPE
#define FDO_EXPRESSION_YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define FDO_EXPRESSION_YYPARSE_PARAM_ARG FDO_EXPRESSION_YYPARSE_PARAM_TYPE FDO_EXPRESSION_YYPARSE_PARAM
#define FDO_EXPRESSION_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define FDO_EXPRESSION_YYPARSE_PARAM_ARG FDO_EXPRESSION_YYPARSE_PARAM
#define FDO_EXPRESSION_YYPARSE_PARAM_DECL FDO_EXPRESSION_YYPARSE_PARAM_TYPE FDO_EXPRESSION_YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! FDO_EXPRESSION_YYPARSE_PARAM */

int
fdo_expression_yyparse(FdoParse* pParse)
    FDO_EXPRESSION_YYPARSE_PARAM_DECL
{
    register int fdo_expression_yym, fdo_expression_yyn, fdo_expression_yystate;
#if FDO_EXPRESSION_YYDEBUG
    register const char *fdo_expression_yys;

    if ((fdo_expression_yys = getenv("FDO_EXPRESSION_YYDEBUG")))
    {
        fdo_expression_yyn = *fdo_expression_yys;
        if (fdo_expression_yyn >= '0' && fdo_expression_yyn <= '9')
            pParse->fdo_expression_yydebug = fdo_expression_yyn - '0';
    }
#endif

    pParse->fdo_expression_yynerrs = 0;
    pParse->fdo_expression_yyerrflag = 0;
    pParse->fdo_expression_yychar = (-1);

    if (pParse->fdo_expression_yyss == NULL && fdo_expression_yygrowstack(pParse)) goto fdo_expression_yyoverflow;
    pParse->fdo_expression_yyssp = pParse->fdo_expression_yyss;
    pParse->fdo_expression_yyvsp = pParse->fdo_expression_yyvs;
    *pParse->fdo_expression_yyssp = fdo_expression_yystate = 0;

fdo_expression_yyloop:
    if ((fdo_expression_yyn = fdo_expression_yydefred[fdo_expression_yystate])) goto fdo_expression_yyreduce;
    if (pParse->fdo_expression_yychar < 0)
    {
        if ((pParse->fdo_expression_yychar = fdo_expression_yylex(pParse)) < 0) pParse->fdo_expression_yychar = 0;
#if FDO_EXPRESSION_YYDEBUG
        if (pParse->fdo_expression_yydebug)
        {
            fdo_expression_yys = 0;
            if (pParse->fdo_expression_yychar <= FDO_EXPRESSION_YYMAXTOKEN) fdo_expression_yys = fdo_expression_yyname[pParse->fdo_expression_yychar];
            if (!fdo_expression_yys) fdo_expression_yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    FDO_EXPRESSION_YYPREFIX, fdo_expression_yystate, pParse->fdo_expression_yychar, fdo_expression_yys);
        }
#endif
    }
    if ((fdo_expression_yyn = fdo_expression_yysindex[fdo_expression_yystate]) && (fdo_expression_yyn += pParse->fdo_expression_yychar) >= 0 &&
            fdo_expression_yyn <= FDO_EXPRESSION_YYTABLESIZE && fdo_expression_yycheck[fdo_expression_yyn] == pParse->fdo_expression_yychar)
    {
#if FDO_EXPRESSION_YYDEBUG
        if (pParse->fdo_expression_yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    FDO_EXPRESSION_YYPREFIX, fdo_expression_yystate, fdo_expression_yytable[fdo_expression_yyn]);
#endif
        if (pParse->fdo_expression_yyssp >= pParse->fdo_expression_yysslim && fdo_expression_yygrowstack(pParse))
        {
            goto fdo_expression_yyoverflow;
        }
        *++pParse->fdo_expression_yyssp = fdo_expression_yystate = fdo_expression_yytable[fdo_expression_yyn];
        *++pParse->fdo_expression_yyvsp = pParse->fdo_expression_yylval;
        pParse->fdo_expression_yychar = (-1);
        if (pParse->fdo_expression_yyerrflag > 0)  --pParse->fdo_expression_yyerrflag;
        goto fdo_expression_yyloop;
    }
    if ((fdo_expression_yyn = fdo_expression_yyrindex[fdo_expression_yystate]) && (fdo_expression_yyn += pParse->fdo_expression_yychar) >= 0 &&
            fdo_expression_yyn <= FDO_EXPRESSION_YYTABLESIZE && fdo_expression_yycheck[fdo_expression_yyn] == pParse->fdo_expression_yychar)
    {
        fdo_expression_yyn = fdo_expression_yytable[fdo_expression_yyn];
        goto fdo_expression_yyreduce;
    }
    if (pParse->fdo_expression_yyerrflag) goto fdo_expression_yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto fdo_expression_yynewerror;
#endif
fdo_expression_yynewerror:
   fdo_expression_yyerror( pParse,"syntax error");
#if defined(lint) || defined(__GNUC__)
    goto fdo_expression_yyerrlab;
#endif
fdo_expression_yyerrlab:
    ++pParse->fdo_expression_yynerrs;
fdo_expression_yyinrecovery:
    if (pParse->fdo_expression_yyerrflag < 3)
    {
        pParse->fdo_expression_yyerrflag = 3;
        for (;;)
        {
            if ((fdo_expression_yyn = fdo_expression_yysindex[*pParse->fdo_expression_yyssp]) && (fdo_expression_yyn += FDO_EXPRESSION_YYERRCODE) >= 0 &&
                    fdo_expression_yyn <= FDO_EXPRESSION_YYTABLESIZE && fdo_expression_yycheck[fdo_expression_yyn] == FDO_EXPRESSION_YYERRCODE)
            {
#if FDO_EXPRESSION_YYDEBUG
                if (pParse->fdo_expression_yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", FDO_EXPRESSION_YYPREFIX, *pParse->fdo_expression_yyssp, fdo_expression_yytable[fdo_expression_yyn]);
#endif
                if (pParse->fdo_expression_yyssp >= pParse->fdo_expression_yysslim && fdo_expression_yygrowstack(pParse))
                {
                    goto fdo_expression_yyoverflow;
                }
                *++pParse->fdo_expression_yyssp = fdo_expression_yystate = fdo_expression_yytable[fdo_expression_yyn];
                *++pParse->fdo_expression_yyvsp = pParse->fdo_expression_yylval;
                goto fdo_expression_yyloop;
            }
            else
            {
#if FDO_EXPRESSION_YYDEBUG
                if (pParse->fdo_expression_yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            FDO_EXPRESSION_YYPREFIX, *pParse->fdo_expression_yyssp);
#endif
                if (pParse->fdo_expression_yyssp <= pParse->fdo_expression_yyss) goto fdo_expression_yyabort;
                --pParse->fdo_expression_yyssp;
                --pParse->fdo_expression_yyvsp;
            }
        }
    }
    else
    {
        if (pParse->fdo_expression_yychar == 0) goto fdo_expression_yyabort;
#if FDO_EXPRESSION_YYDEBUG
        if (pParse->fdo_expression_yydebug)
        {
            fdo_expression_yys = 0;
            if (pParse->fdo_expression_yychar <= FDO_EXPRESSION_YYMAXTOKEN) fdo_expression_yys = fdo_expression_yyname[pParse->fdo_expression_yychar];
            if (!fdo_expression_yys) fdo_expression_yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    FDO_EXPRESSION_YYPREFIX, fdo_expression_yystate, pParse->fdo_expression_yychar, fdo_expression_yys);
        }
#endif
        pParse->fdo_expression_yychar = (-1);
        goto fdo_expression_yyloop;
    }
fdo_expression_yyreduce:
#if FDO_EXPRESSION_YYDEBUG
    if (pParse->fdo_expression_yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                FDO_EXPRESSION_YYPREFIX, fdo_expression_yystate, fdo_expression_yyn, fdo_expression_yyrule[fdo_expression_yyn]);
#endif
    fdo_expression_yym = fdo_expression_yylen[fdo_expression_yyn];
    pParse->fdo_expression_yyval = pParse->fdo_expression_yyvsp[1-fdo_expression_yym];
    switch (fdo_expression_yyn)
    {
case 1:
#line 114 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->SetRoot(Node_Copy(L"Expression", pParse->fdo_expression_yyvsp[0].m_node));	FDO_EXPRESSION_YYACCEPT;}
break;
case 2:
#line 115 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	FDO_EXPRESSION_YYABORT;}
break;
case 3:
#line 119 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"(Expression)", pParse->fdo_expression_yyvsp[-1].m_node);}
break;
case 4:
#line 120 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"UnaryExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 5:
#line 121 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"BinaryExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 6:
#line 122 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Function", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 7:
#line 123 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Identifier", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 8:
#line 124 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"ValueExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 9:
#line 125 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"ComputedIdentifier", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 10:
#line 129 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Add, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 11:
#line 130 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Subtract, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 12:
#line 131 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Multiply, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 13:
#line 132 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Divide, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 14:
#line 137 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"boolean", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 15:
#line 139 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"datetime", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 16:
#line 142 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"double", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 17:
#line 144 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"integer", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 18:
#line 146 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"int64", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 19:
#line 148 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"inthex", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 20:
#line 150 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"intbin", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 21:
#line 152 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"string", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 22:
#line 154 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"TODO:BLOB", NULL);}
break;
case 23:
#line 156 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"TODO:CLOB", NULL);}
break;
case 24:
#line 158 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
break;
case 25:
#line 162 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
break;
case 26:
#line 163 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
break;
case 27:
#line 166 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"STRING", FdoStringValue::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 28:
#line 169 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INTEGER", FdoInt32Value::Create(pParse->fdo_expression_yyvsp[0].m_integer));}
break;
case 29:
#line 172 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INT64", FdoInt64Value::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
break;
case 30:
#line 175 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INTHEX", FdoIntHexValue::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
break;
case 31:
#line 178 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INTBIN", FdoIntBinValue::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
break;
case 32:
#line 181 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"DOUBLE", FdoDoubleValue::Create(pParse->fdo_expression_yyvsp[0].m_double));}
break;
case 33:
#line 184 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"DateTime", FdoDataValue::Create(pParse->fdo_expression_yyvsp[0].m_datetime));}
break;
case 34:
#line 189 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node)->GetName(), (FdoExpressionCollection*)pParse->fdo_expression_yyvsp[-1].m_node));}
break;
case 35:
#line 194 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
break;
case 36:
#line 196 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
break;
case 37:
#line 201 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
break;
case 38:
#line 203 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
break;
case 39:
#line 205 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName()));}
break;
case 40:
#line 207 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
break;
case 41:
#line 209 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
break;
case 42:
#line 214 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node, (FdoJoinType)pParse->fdo_expression_yyvsp[-1].m_id));}
break;
case 43:
#line 216 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node, (FdoJoinType)pParse->fdo_expression_yyvsp[-3].m_id, FdoPtr<FdoFilter>(FdoFilter::Parse(((FdoStringValue*)pParse->fdo_expression_yyvsp[-1].m_node)->GetString()))));}
break;
case 44:
#line 218 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create(((FdoComputedIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node)->GetName(), FdoPtr<FdoIdentifier>(static_cast<FdoIdentifier*>(((FdoComputedIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node)->GetExpression())), (FdoJoinType)pParse->fdo_expression_yyvsp[-1].m_id));}
break;
case 45:
#line 220 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create(((FdoComputedIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node)->GetName(), FdoPtr<FdoIdentifier>(static_cast<FdoIdentifier*>(((FdoComputedIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node)->GetExpression())), (FdoJoinType)pParse->fdo_expression_yyvsp[-3].m_id, FdoPtr<FdoFilter>(FdoFilter::Parse(((FdoStringValue*)pParse->fdo_expression_yyvsp[-1].m_node)->GetString()))));}
break;
case 46:
#line 225 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoJoinCriteriaCollection::Create()); ((FdoJoinCriteriaCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoJoinCriteria*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"JoinCriteria 1");}
break;
case 47:
#line 227 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{((FdoJoinCriteriaCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoJoinCriteria*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"JoinCriteria n");}
break;
case 48:
#line 232 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node, (FdoIdentifier*)pParse->fdo_expression_yyvsp[-1].m_node));}
break;
case 49:
#line 234 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node, (FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node, FdoPtr<FdoFilter>(FdoFilter::Parse(((FdoStringValue*)pParse->fdo_expression_yyvsp[-1].m_node)->GetString()))));}
break;
case 50:
#line 236 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node, (FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node, (FdoJoinCriteriaCollection*)pParse->fdo_expression_yyvsp[-1].m_node));}
break;
case 51:
#line 238 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_expression_yyvsp[-7].m_node, (FdoIdentifier*)pParse->fdo_expression_yyvsp[-5].m_node, FdoPtr<FdoFilter>(FdoFilter::Parse(((FdoStringValue*)pParse->fdo_expression_yyvsp[-3].m_node)->GetString())), (FdoJoinCriteriaCollection*)pParse->fdo_expression_yyvsp[-1].m_node));}
break;
case 52:
#line 242 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
break;
case 53:
#line 243 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
break;
case 54:
#line 244 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
break;
case 55:
#line 245 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
break;
case 56:
#line 248 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
break;
case 57:
#line 249 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"Expression Arg 1,");}
break;
case 58:
#line 250 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{((FdoExpressionCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"Expression Arg N,");}
break;
case 59:
#line 255 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue(pParse->fdo_expression_yyvsp[-1].m_string));}
break;
case 60:
#line 260 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 61:
#line 264 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"LiteralValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 62:
#line 265 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Parameter", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 63:
#line 269 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"GeometryValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 64:
#line 270 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"DataValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 65:
#line 275 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"PARAMETER", FdoParameter::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 66:
#line 277 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L":Parameter", FdoParameter::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 67:
#line 281 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 68:
#line 285 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_id=FdoJoinType_None; Node_Trace(L"NONE");}
break;
case 69:
#line 286 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_id=FdoJoinType_Inner; Node_Trace(L"INNER");}
break;
case 70:
#line 287 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_id=FdoJoinType_RightOuter; Node_Trace(L"RIGHTOUTER");}
break;
case 71:
#line 288 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_id=FdoJoinType_LeftOuter; Node_Trace(L"LEFTOUTER");}
break;
case 72:
#line 289 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_id=FdoJoinType_FullOuter; Node_Trace(L"FULLOUTER");}
break;
case 73:
#line 290 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_id=FdoJoinType_Cross; Node_Trace(L"CROSS");}
break;
#line 947 "yyExpression.cpp"
    }
    pParse->fdo_expression_yyssp -= fdo_expression_yym;
    fdo_expression_yystate = *pParse->fdo_expression_yyssp;
    pParse->fdo_expression_yyvsp -= fdo_expression_yym;
    fdo_expression_yym = fdo_expression_yylhs[fdo_expression_yyn];
    if (fdo_expression_yystate == 0 && fdo_expression_yym == 0)
    {
#if FDO_EXPRESSION_YYDEBUG
        if (pParse->fdo_expression_yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", FDO_EXPRESSION_YYPREFIX, FDO_EXPRESSION_YYFINAL);
#endif
        fdo_expression_yystate = FDO_EXPRESSION_YYFINAL;
        *++pParse->fdo_expression_yyssp = FDO_EXPRESSION_YYFINAL;
        *++pParse->fdo_expression_yyvsp = pParse->fdo_expression_yyval;
        if (pParse->fdo_expression_yychar < 0)
        {
            if ((pParse->fdo_expression_yychar = fdo_expression_yylex(pParse)) < 0) pParse->fdo_expression_yychar = 0;
#if FDO_EXPRESSION_YYDEBUG
            if (pParse->fdo_expression_yydebug)
            {
                fdo_expression_yys = 0;
                if (pParse->fdo_expression_yychar <= FDO_EXPRESSION_YYMAXTOKEN) fdo_expression_yys = fdo_expression_yyname[pParse->fdo_expression_yychar];
                if (!fdo_expression_yys) fdo_expression_yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        FDO_EXPRESSION_YYPREFIX, FDO_EXPRESSION_YYFINAL, pParse->fdo_expression_yychar, fdo_expression_yys);
            }
#endif
        }
        if (pParse->fdo_expression_yychar == 0) goto fdo_expression_yyaccept;
        goto fdo_expression_yyloop;
    }
    if ((fdo_expression_yyn = fdo_expression_yygindex[fdo_expression_yym]) && (fdo_expression_yyn += fdo_expression_yystate) >= 0 &&
            fdo_expression_yyn <= FDO_EXPRESSION_YYTABLESIZE && fdo_expression_yycheck[fdo_expression_yyn] == fdo_expression_yystate)
        fdo_expression_yystate = fdo_expression_yytable[fdo_expression_yyn];
    else
        fdo_expression_yystate = fdo_expression_yydgoto[fdo_expression_yym];
#if FDO_EXPRESSION_YYDEBUG
    if (pParse->fdo_expression_yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", FDO_EXPRESSION_YYPREFIX, *pParse->fdo_expression_yyssp, fdo_expression_yystate);
#endif
    if (pParse->fdo_expression_yyssp >= pParse->fdo_expression_yysslim && fdo_expression_yygrowstack(pParse))
    {
        goto fdo_expression_yyoverflow;
    }
    *++pParse->fdo_expression_yyssp = fdo_expression_yystate;
    *++pParse->fdo_expression_yyvsp = pParse->fdo_expression_yyval;
    goto fdo_expression_yyloop;
fdo_expression_yyoverflow:
   fdo_expression_yyerror( pParse,"yacc stack overflow");
fdo_expression_yyabort:
    return (1);
fdo_expression_yyaccept:
    return (0);
}
