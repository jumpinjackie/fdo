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
class FdoParse;static int fdo_filter_yygrowstack(FdoParse *pParse);
#define FDO_FILTER_YYPREFIX "fdo_filter_yy"
#line 36 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
#include "Parse.h"
#pragma warning(disable:4102)	/* unreferenced labels in fdo_filter_yyFilter.cpp*/
#line 41 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
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
#line 31 "yyFilter.cpp"
#define FDO_FILTER_YYERRCODE 256
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
const short fdo_filter_yylhs[] = {                                        -1,
    0,    0,    0,    3,    3,    3,    3,    3,    3,    1,
    1,    1,    1,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,   37,   37,   36,   38,   39,   40,
   41,   35,   42,    5,    4,    4,    4,    6,    7,   10,
   10,   43,   43,   11,   11,   12,   12,   12,   12,   12,
   13,   13,   14,   14,   15,   15,   16,   20,   20,   20,
   23,   23,   21,   21,   25,   25,    9,    9,    9,    9,
   26,   26,    8,    8,    8,    8,   22,   22,   24,   27,
   27,   27,   27,   28,   29,   17,   17,   18,   19,   31,
   31,   31,   31,   31,   31,   31,   33,   33,   33,   33,
   33,   33,   32,   32,   30,   30,   30,   30,   30,   30,
   30,   30,   30,   30,   30,   34,   34,   34,
};
const short fdo_filter_yylen[] = {                                         2,
    1,    3,    1,    3,    1,    1,    1,    1,    1,    3,
    3,    3,    3,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    4,    0,    1,    3,    4,    1,    1,
    3,    5,    3,    5,    3,    5,    3,    1,    5,    3,
    1,    1,    1,    1,    1,    2,    2,    3,    1,    1,
    1,    1,    1,    1,    1,    3,    6,    8,    6,    8,
    1,    3,    6,    8,    8,   10,    5,    5,    2,    1,
    1,    1,    1,    3,    2,    3,    3,    3,    4,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,
};
const short fdo_filter_yydefred[] = {                                      0,
    3,   24,   25,   26,    0,    0,   39,   55,   27,   28,
   29,   30,   31,   32,   33,   22,   23,    0,    0,    0,
    0,    6,   54,    0,    7,   53,    0,    0,    9,   51,
   52,    5,   61,   81,   63,    0,   82,   80,   59,   83,
   60,   64,   62,   16,   21,   14,   17,   18,   19,   20,
   15,   40,    0,    0,    0,   85,    0,   57,    0,   56,
    0,    0,   96,    0,    0,    0,    0,   90,   91,   92,
   93,   94,   95,    0,    0,   79,    0,  103,  104,  105,
  114,  106,  107,  115,  108,  110,  109,  111,  112,  113,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   58,    0,    0,   12,   13,   43,    0,    0,    0,    0,
    0,    0,    0,   41,   86,    0,   38,    4,    0,    0,
    0,   65,    0,    0,   34,  117,  118,   89,  116,   42,
    0,   78,    0,   77,    0,    0,   66,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   73,    0,    0,
   50,   47,    0,   71,    0,    0,    0,    0,    0,    0,
   74,    0,   75,   49,   46,    0,    0,    0,    0,   72,
    0,    0,    0,    0,   76,    0,   45,   97,   98,   99,
  100,  101,  102,    0,    0,    0,    0,   67,    0,   69,
   44,    0,    0,   68,   70,
};
const short fdo_filter_yydgoto[] = {                                      21,
   22,   23,   55,  110,   25,   26,   59,  121,  154,   28,
  168,  142,   29,   30,   31,   32,   33,   34,   35,   62,
   37,   38,   39,   40,  123,  156,   41,   42,   43,   92,
   75,   93,  184,  128,   44,   45,   46,   47,   48,   49,
   50,   51,   52,
};
const short fdo_filter_yysindex[] = {                                    -38,
    0,    0,    0,    0,  -22,  137,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  210, -286,  137,
    0,    0,    0,  309,    0,    0,  136,  -30,    0,    0,
    0,    0,    0,    0,    0, -227,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -269,  137,  321,    0,  210,    0,  -14,    0,
  226,  -33,    0,  210,  210,  210,  210,    0,    0,    0,
    0,    0,    0, -263,  210,    0,  -10,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  210,  210,  210,  183,  137,  137,    7,  276,   37, -262,
    0, -224, -224,    0,    0,    0, -232, -174, -232,  -25,
 -232, -236, -227,    0,    0, -205,    0,    0, -263,   48,
   73,    0,    9,  210,    0,    0,    0,    0,    0,    0,
 -263,    0,  305,    0, -232,   84,    0,  260,  210, -271,
  -40,   16,   41,   46, -263, -263,   91,    0, -184, -183,
    0,    0,   96,    0,  -35,   33, -263, -263,  -27, -139,
    0, -139,    0,    0,    0, -263,  -41,   94,   35,    0,
   98, -263, -229, -229,    0, -177,    0,    0,    0,    0,
    0,    0,    0,   59,   61, -263,  137,    0,  137,    0,
    0,  -29,  -26,    0,    0,
};
const short fdo_filter_yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  154,    0,    0,    0,
    0,    0,    0,    0,    0,  142,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    1,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   65,    0,    0,    0,    0,    0,    0,    0,    0,  334,
    0,   20,   63,    0,    0,    0,    5,    0,   88,    0,
   10,    0,  145,    0,    0,   11,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   89,    0,    0,    0,    0,    0,
  -19,    0,    0, -211,    0,    0,    0,    0, -206,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,
};
const short fdo_filter_yygindex[] = {                                      0,
    0,    0,   90,    0,    0,    0,   21,    0,  -16,    0,
    0,    0, -104,    0,    0,    0,    0,    0,    0,   17,
    0,    0,    0,    0,    0,  -12,    0,    0,    0,    0,
    0,    0,  -24,    0,   39,    0,    0,    0,    0,    0,
    0,    0,   55,
};
#define FDO_FILTER_YYTABLESIZE 646
const short fdo_filter_yytable[] = {                                      91,
    8,   20,  173,  122,   88,  161,   60,  101,  160,   84,
   87,  194,  166,   94,  195,  125,   36,   53,  124,   10,
   27,   48,   56,   97,   48,   91,   27,    7,  137,  108,
   64,   65,   66,   67,  178,  179,  180,  181,  182,  183,
   27,    8,   95,   96,    8,   88,  145,  117,   88,  134,
   84,   87,  133,   84,   87,  119,  148,  126,  127,  147,
   10,   14,   11,   10,   95,   64,   65,   66,   67,   64,
   65,   66,   67,  163,   27,  175,  162,  118,  162,   66,
   67,  149,    2,    3,    4,   91,  150,  131,    5,   24,
    8,    8,    8,    8,  106,    4,    4,    4,    4,  188,
  120,  190,  187,   11,  189,   35,   11,   58,   35,   61,
  113,  115,  116,  132,   27,   27,   27,    8,    9,   10,
   11,   12,   13,   14,   15,   16,   17,  138,   36,   37,
   54,   36,   37,  157,  158,  159,  153,  174,  176,  130,
  186,    1,   19,   98,    2,  170,   99,  169,  114,  185,
  129,  136,    0,  102,  103,  104,  105,    0,  141,  144,
    0,    0,    0,  155,  107,  151,  152,   27,    0,    0,
    0,    0,    0,    0,    0,   91,   54,  164,  165,  167,
  109,  111,  112,   24,    0,    0,  171,    0,    0,    0,
    0,    0,  177,    0,    8,    0,    0,    0,    0,    0,
    0,    0,    0,  192,    0,  193,  191,   27,    0,   27,
    0,    0,    0,  135,    0,    0,    0,    1,    2,    3,
    4,    0,   20,    0,    5,    0,    0,  140,  143,    0,
    0,    0,    0,    6,   95,   96,   95,   96,    0,    0,
   95,   96,    0,   95,   96,    0,    0,    0,    0,   57,
    0,    0,    7,    8,    9,   10,   11,   12,   13,   14,
   15,   16,   17,    7,    0,    0,  100,   18,    0,    0,
    8,    8,    0,    8,   88,   88,  172,  146,   19,   84,
   84,   87,    8,    8,    8,    8,    0,    0,    0,   10,
   10,    0,   10,    0,    8,    8,    0,    0,    8,  139,
    0,    0,    8,    8,    8,    8,    0,    8,    8,    8,
    8,    8,    8,   10,   10,    0,  118,   10,    8,    0,
    0,   10,   10,    0,    0,    0,   10,   10,   10,   10,
   10,   10,   11,   11,    0,   11,    0,   10,   64,   65,
   66,   67,   64,   65,   66,   67,    0,    2,    3,    4,
    0,    0,    0,    5,    0,    0,   11,   11,    0,    0,
   11,    0,    6,    0,   11,   11,  153,    0,    0,   11,
   11,   11,   11,   11,   11,    0,    0,    0,    0,    0,
   11,    7,    8,    9,   10,   11,   12,   13,   14,   15,
   16,   17,   76,    2,    3,    4,   18,    0,    0,    5,
    0,    0,    0,    0,    0,    0,    0,   19,    6,   77,
    0,    0,   78,   79,   80,   81,   82,   83,   84,   85,
   86,   87,   88,   89,   90,    0,    8,    7,    8,    9,
   10,   11,   12,   13,   14,   15,   16,   17,    0,    2,
    3,    4,   18,    0,    0,    5,    0,    0,    0,    0,
    0,    0,    0,   19,    6,    8,    8,    8,    8,    0,
    8,    8,    8,    8,    8,    8,    2,    3,    4,    0,
    0,    8,    5,    7,    8,    9,   10,   11,   12,   13,
   14,   15,   16,   17,    0,    0,    0,    0,   18,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   63,   19,
    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
   17,    0,    0,    0,    0,   18,    2,    3,    4,    0,
    0,    0,    5,    0,    0,    0,   19,   64,   65,   66,
   67,    0,   68,   69,   70,   71,   72,   73,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   63,    0,
    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
   17,    2,    3,    4,    0,   18,    0,    5,    0,    0,
    0,    0,    0,    0,    0,    0,   19,   64,   65,   66,
   67,   63,   68,   69,   70,   71,   72,   73,    0,    0,
    0,    0,    0,   63,    0,    0,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,    4,    0,    0,    0,
   64,   65,   66,   67,    0,   68,   69,   70,   71,   72,
   73,   19,   64,   65,   66,   67,   74,   68,   69,   70,
   71,   72,   73,    0,    0,    4,    4,    4,    4,    0,
    4,    4,    4,    4,    4,    4,
};
const short fdo_filter_yycheck[] = {                                      40,
    0,   40,   44,  108,    0,   41,  293,   41,   44,    0,
    0,   41,   40,   44,   41,   41,    0,   40,   44,    0,
    0,   41,    6,  293,   44,   40,    6,  291,  133,   40,
  302,  303,  304,  305,  264,  265,  266,  267,  268,  269,
   20,   41,  270,  271,   44,   41,  318,   41,   44,   41,
   41,   41,   44,   44,   44,  318,   41,  294,  295,   44,
   41,  298,    0,   44,  270,  302,  303,  304,  305,  302,
  303,  304,  305,   41,   54,   41,   44,   41,   44,  304,
  305,   41,  257,  258,  259,   40,   41,   40,  263,    0,
  302,  303,  304,  305,   74,  302,  303,  304,  305,   41,
  275,   41,   44,   41,   44,   41,   44,   18,   44,   20,
   94,   95,   96,   41,   94,   95,   96,  292,  293,  294,
  295,  296,  297,  298,  299,  300,  301,   44,   41,   41,
   40,   44,   44,  318,  318,   40,  276,   44,   41,  119,
  318,    0,  317,   54,    0,  162,   57,  160,   94,  174,
  112,  131,   -1,   64,   65,   66,   67,   -1,  138,  139,
   -1,   -1,   -1,  147,   75,  145,  146,  147,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   40,   40,  157,  158,  159,
   91,   92,   93,   94,   -1,   -1,  166,   -1,   -1,   -1,
   -1,   -1,  172,   -1,   41,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  187,   -1,  189,  186,  187,   -1,  189,
   -1,   -1,   -1,  124,   -1,   -1,   -1,  256,  257,  258,
  259,   -1,   40,   -1,  263,   -1,   -1,  138,  139,   -1,
   -1,   -1,   -1,  272,  270,  271,  270,  271,   -1,   -1,
  270,  271,   -1,  270,  271,   -1,   -1,   -1,   -1,   40,
   -1,   -1,  291,  292,  293,  294,  295,  296,  297,  298,
  299,  300,  301,  291,   -1,   -1,   41,  306,   -1,   -1,
  270,  271,   -1,  273,  270,  271,  318,  318,  317,  270,
  271,  271,  302,  303,  304,  305,   -1,   -1,   -1,  270,
  271,   -1,  273,   -1,  294,  295,   -1,   -1,  298,   40,
   -1,   -1,  302,  303,  304,  305,   -1,  307,  308,  309,
  310,  311,  312,  294,  295,   -1,   41,  298,  318,   -1,
   -1,  302,  303,   -1,   -1,   -1,  307,  308,  309,  310,
  311,  312,  270,  271,   -1,  273,   -1,  318,  302,  303,
  304,  305,  302,  303,  304,  305,   -1,  257,  258,  259,
   -1,   -1,   -1,  263,   -1,   -1,  294,  295,   -1,   -1,
  298,   -1,  272,   -1,  302,  303,  276,   -1,   -1,  307,
  308,  309,  310,  311,  312,   -1,   -1,   -1,   -1,   -1,
  318,  291,  292,  293,  294,  295,  296,  297,  298,  299,
  300,  301,  257,  257,  258,  259,  306,   -1,   -1,  263,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  317,  272,  274,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  286,  287,  288,  289,   -1,  273,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  301,   -1,  257,
  258,  259,  306,   -1,   -1,  263,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  317,  272,  302,  303,  304,  305,   -1,
  307,  308,  309,  310,  311,  312,  257,  258,  259,   -1,
   -1,  318,  263,  291,  292,  293,  294,  295,  296,  297,
  298,  299,  300,  301,   -1,   -1,   -1,   -1,  306,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,  317,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,   -1,   -1,   -1,   -1,  306,  257,  258,  259,   -1,
   -1,   -1,  263,   -1,   -1,   -1,  317,  302,  303,  304,
  305,   -1,  307,  308,  309,  310,  311,  312,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  273,   -1,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,  257,  258,  259,   -1,  306,   -1,  263,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  317,  302,  303,  304,
  305,  273,  307,  308,  309,  310,  311,  312,   -1,   -1,
   -1,   -1,   -1,  273,   -1,   -1,  292,  293,  294,  295,
  296,  297,  298,  299,  300,  301,  273,   -1,   -1,   -1,
  302,  303,  304,  305,   -1,  307,  308,  309,  310,  311,
  312,  317,  302,  303,  304,  305,  318,  307,  308,  309,
  310,  311,  312,   -1,   -1,  302,  303,  304,  305,   -1,
  307,  308,  309,  310,  311,  312,
};
#define FDO_FILTER_YYFINAL 21
#ifndef FDO_FILTER_YYDEBUG
#define FDO_FILTER_YYDEBUG 0
#endif
#define FDO_FILTER_YYMAXTOKEN 319
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
"ExpressionCollection :",
"ExpressionCollection : Expression",
"ExpressionCollection : ExpressionCollection ',' Expression",
"GeometryValue : FdoToken_GEOMFROMTEXT '(' FdoToken_STRING ')'",
"Identifier : FdoToken_IDENTIFIER",
"ComputedIdentifier : ComputedIdentifier2",
"ComputedIdentifier : ComputedIdentifier ',' ComputedIdentifier2",
"ComputedIdentifier2 : '(' Expression ')' FdoToken_AS Identifier",
"ComputedIdentifier2 : Expression FdoToken_AS Identifier",
"ComputedIdentifierOnly : '(' Identifier ')' FdoToken_AS Identifier",
"ComputedIdentifierOnly : Identifier FdoToken_AS Identifier",
"ComputedIdentifier3 : '(' Identifier ')' FdoToken_AS Identifier",
"ComputedIdentifier3 : Identifier FdoToken_AS Identifier",
"ComputedIdentifier3 : Identifier",
"ComputedIdentifier3 : '(' Expression ')' FdoToken_AS Identifier",
"ComputedIdentifier3 : Expression FdoToken_AS Identifier",
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
"JoinCriteria : FdoToken_JOIN '(' Identifier ',' JoinOperations ')'",
"JoinCriteria : FdoToken_JOIN '(' Identifier ',' JoinOperations ',' Filter ')'",
"JoinCriteria : FdoToken_JOIN '(' ComputedIdentifierOnly ',' JoinOperations ')'",
"JoinCriteria : FdoToken_JOIN '(' ComputedIdentifierOnly ',' JoinOperations ',' Filter ')'",
"JoinCriteriaCollection : JoinCriteria",
"JoinCriteriaCollection : JoinCriteriaCollection ',' JoinCriteria",
"SubSelectExpression : FdoToken_SELECT '(' Identifier ',' ComputedIdentifier3 ')'",
"SubSelectExpression : FdoToken_SELECT '(' Identifier ',' ComputedIdentifier3 ',' Filter ')'",
"SubSelectExpression : FdoToken_SELECT '(' Identifier ',' ComputedIdentifier3 ',' JoinCriteriaCollection ')'",
"SubSelectExpression : FdoToken_SELECT '(' Identifier ',' ComputedIdentifier3 ',' Filter ',' JoinCriteriaCollection ')'",
"InCondition : Identifier FdoToken_IN '(' ValueExpressionCollection ')'",
"InCondition : Identifier FdoToken_IN '(' SubSelectExpression ')'",
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
"JoinOperations : FdoToken_JOINNONE",
"JoinOperations : FdoToken_JOININNER",
"JoinOperations : FdoToken_JOINRIGHTOUTER",
"JoinOperations : FdoToken_JOINLEFTOUTER",
"JoinOperations : FdoToken_JOINFULLOUTER",
"JoinOperations : FdoToken_JOINCROSS",
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
"distance : FdoToken_INT64",
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
#line 425 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
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
#line 578 "yyFilter.cpp"
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
#line 126 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->SetRoot(Node_Copy(L"Filter", pParse->fdo_filter_yyvsp[0].m_node));	FDO_FILTER_YYACCEPT;}
break;
case 2:
#line 127 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->SetRoot(Node_Copy(L"Filter", pParse->fdo_filter_yyvsp[0].m_node)); FDO_FILTER_YYACCEPT;}
break;
case 3:
#line 128 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	FDO_FILTER_YYABORT;}
break;
case 4:
#line 134 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"(Expression)", pParse->fdo_filter_yyvsp[-1].m_node);}
break;
case 5:
#line 135 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"UnaryExpression", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 6:
#line 136 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"BinaryExpression", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 7:
#line 137 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"Function", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 8:
#line 138 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"Identifier", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 9:
#line 139 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"ValueExpression", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 10:
#line 143 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Add, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 11:
#line 144 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Subtract, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 12:
#line 145 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Multiply, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 13:
#line 146 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Divide, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 14:
#line 151 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"boolean", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 15:
#line 153 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"datetime", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 16:
#line 156 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"double", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 17:
#line 158 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"integer", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 18:
#line 160 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"int64", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 19:
#line 162 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"inthex", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 20:
#line 164 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"intbin", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 21:
#line 166 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"string", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 22:
#line 168 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"TODO:BLOB", NULL);}
break;
case 23:
#line 170 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"TODO:CLOB", NULL);}
break;
case 24:
#line 172 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
break;
case 25:
#line 176 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
break;
case 26:
#line 177 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
break;
case 27:
#line 180 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"STRING", FdoDataValue::Create(pParse->fdo_filter_yyvsp[0].m_string));}
break;
case 28:
#line 183 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"INTEGER", FdoInt32Value::Create(pParse->fdo_filter_yyvsp[0].m_integer));}
break;
case 29:
#line 186 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"INT64", FdoInt64Value::Create(pParse->fdo_filter_yyvsp[0].m_int64));}
break;
case 30:
#line 189 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"INTHEX", FdoIntHexValue::Create(pParse->fdo_filter_yyvsp[0].m_int64));}
break;
case 31:
#line 192 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"INTBIN", FdoIntBinValue::Create(pParse->fdo_filter_yyvsp[0].m_int64));}
break;
case 32:
#line 195 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"DOUBLE", FdoDoubleValue::Create(pParse->fdo_filter_yyvsp[0].m_double));}
break;
case 33:
#line 198 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"DateTime", FdoDataValue::Create(pParse->fdo_filter_yyvsp[0].m_datetime));}
break;
case 34:
#line 203 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node)->GetName(), (FdoExpressionCollection*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 35:
#line 206 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
break;
case 36:
#line 207 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"Expression Arg 1,");}
break;
case 37:
#line 208 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{((FdoExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"Expression Arg N,");}
break;
case 38:
#line 213 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue(pParse->fdo_filter_yyvsp[-1].m_string));}
break;
case 39:
#line 218 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{
		    FdoComputedIdentifier* id = pParse->FindComputedIdentifier( pParse->fdo_filter_yyvsp[0].m_string );
		    if( id == NULL )
		        pParse->fdo_filter_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(pParse->fdo_filter_yyvsp[0].m_string));
		    else
		        pParse->fdo_filter_yyval.m_node = id;
		}
break;
case 40:
#line 228 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{}
break;
case 41:
#line 229 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{}
break;
case 42:
#line 234 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{
            FdoComputedIdentifier* id = FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-3].m_node);
            Node_Add(L"ComputedIdentifier", id);
            pParse->AddCompIdentifier(id);
        }
break;
case 43:
#line 240 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{
            FdoComputedIdentifier* id = FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node);
            Node_Add(L"ComputedIdentifier", id);
            pParse->AddCompIdentifier(id);
        }
break;
case 44:
#line 248 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-3].m_node));}
break;
case 45:
#line 250 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node));}
break;
case 46:
#line 255 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-3].m_node));}
break;
case 47:
#line 257 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node));}
break;
case 48:
#line 259 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName()));}
break;
case 49:
#line 261 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-3].m_node));}
break;
case 50:
#line 263 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node));}
break;
case 51:
#line 267 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"LiteralValue", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 52:
#line 268 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"Parameter", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 53:
#line 272 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"GeometryValue", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 54:
#line 273 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"DataValue", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 55:
#line 278 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"PARAMETER", FdoParameter::Create(pParse->fdo_filter_yyvsp[0].m_string));}
break;
case 56:
#line 280 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L":Parameter", FdoParameter::Create(pParse->fdo_filter_yyvsp[0].m_string));}
break;
case 57:
#line 284 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 58:
#line 289 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"(Filter)", pParse->fdo_filter_yyvsp[-1].m_node);}
break;
case 59:
#line 290 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"LogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 60:
#line 291 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"SearchCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 61:
#line 295 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"BinaryLogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 62:
#line 296 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"UnaryLogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 63:
#line 300 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"DistanceCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 64:
#line 301 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"SpatialCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 65:
#line 305 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoValueExpressionCollection::Create()); ((FdoValueExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoValueExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"ValueExpression 1");}
break;
case 66:
#line 307 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{((FdoValueExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoValueExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"ValueExpression n");}
break;
case 67:
#line 312 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node, (FdoJoinType)pParse->fdo_filter_yyvsp[-1].m_id));}
break;
case 68:
#line 314 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-5].m_node, (FdoJoinType)pParse->fdo_filter_yyvsp[-3].m_id, (FdoFilter*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 69:
#line 316 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create(((FdoComputedIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node)->GetName(), FdoPtr<FdoIdentifier>(static_cast<FdoIdentifier*>(((FdoComputedIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node)->GetExpression())), (FdoJoinType)pParse->fdo_filter_yyvsp[-1].m_id));}
break;
case 70:
#line 318 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"JoinCriteria", FdoJoinCriteria::Create(((FdoComputedIdentifier*)pParse->fdo_filter_yyvsp[-5].m_node)->GetName(), FdoPtr<FdoIdentifier>(static_cast<FdoIdentifier*>(((FdoComputedIdentifier*)pParse->fdo_filter_yyvsp[-5].m_node)->GetExpression())), (FdoJoinType)pParse->fdo_filter_yyvsp[-3].m_id, (FdoFilter*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 71:
#line 323 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoJoinCriteriaCollection::Create()); ((FdoJoinCriteriaCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoJoinCriteria*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"JoinCriteria 1");}
break;
case 72:
#line 325 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{((FdoJoinCriteriaCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoJoinCriteria*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"JoinCriteria n");}
break;
case 73:
#line 330 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"JoinCriteria", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node, (FdoIdentifier*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 74:
#line 332 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-5].m_node, (FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node, (FdoFilter*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 75:
#line 334 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-5].m_node, (FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node, (FdoJoinCriteriaCollection*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 76:
#line 336 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"SubSelectExpression", FdoSubSelectExpression::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-7].m_node, (FdoIdentifier*)pParse->fdo_filter_yyvsp[-5].m_node, (FdoFilter*)pParse->fdo_filter_yyvsp[-3].m_node, (FdoJoinCriteriaCollection*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 77:
#line 341 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"ValueExpressionCollection", FdoInCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-4].m_node, (FdoValueExpressionCollection*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 78:
#line 343 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"SubSelectExpression", FdoInCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-4].m_node, (FdoSubSelectExpression*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 79:
#line 347 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"NULL", FdoNullCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-1].m_node));}
break;
case 80:
#line 351 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"InCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 81:
#line 352 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"ComparisonCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 82:
#line 353 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"GeometricCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 83:
#line 354 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"NullCondition", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 84:
#line 359 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Id-SpatialOp-Expression", FdoSpatialCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-2].m_node, (FdoSpatialOperations)pParse->fdo_filter_yyvsp[-1].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 85:
#line 363 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"NOT", FdoUnaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node, FdoUnaryLogicalOperations_Not));}
break;
case 86:
#line 367 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"AND", FdoBinaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryLogicalOperations_And, (FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 87:
#line 368 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"OR", FdoBinaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryLogicalOperations_Or, (FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 88:
#line 372 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"ComparisonOperations", FdoComparisonCondition::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, (FdoComparisonOperations)pParse->fdo_filter_yyvsp[-1].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
break;
case 89:
#line 377 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"Indentifier-DistanceOp-Expression", FdoDistanceCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node, (FdoDistanceOperations)pParse->fdo_filter_yyvsp[-2].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[-1].m_node, ((FdoDoubleValue*)pParse->fdo_filter_yyvsp[0].m_node)->GetDouble()));}
break;
case 90:
#line 381 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_EqualTo; Node_Trace(L"EQ");}
break;
case 91:
#line 382 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_NotEqualTo; Node_Trace(L"NE");}
break;
case 92:
#line 383 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_GreaterThan; Node_Trace(L"GT");}
break;
case 93:
#line 384 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_GreaterThanOrEqualTo; Node_Trace(L"GE");}
break;
case 94:
#line 385 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_LessThan; Node_Trace(L"LT");}
break;
case 95:
#line 386 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_LessThanOrEqualTo; Node_Trace(L"LE");}
break;
case 96:
#line 387 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_Like; Node_Trace(L"LIKE");}
break;
case 97:
#line 391 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoJoinType_None; Node_Trace(L"NONE");}
break;
case 98:
#line 392 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoJoinType_Inner; Node_Trace(L"INNER");}
break;
case 99:
#line 393 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoJoinType_RightOuter; Node_Trace(L"RIGHTOUTER");}
break;
case 100:
#line 394 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoJoinType_LeftOuter; Node_Trace(L"LEFTOUTER");}
break;
case 101:
#line 395 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoJoinType_FullOuter; Node_Trace(L"FULLOUTER");}
break;
case 102:
#line 396 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoJoinType_Cross; Node_Trace(L"CROSS");}
break;
case 103:
#line 400 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoDistanceOperations_Beyond; Node_Trace(L"BEYOND");}
break;
case 104:
#line 401 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoDistanceOperations_Within; Node_Trace(L"WITHINDISTANCE");}
break;
case 105:
#line 405 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Contains; Node_Trace(L"CONTAINS");}
break;
case 106:
#line 406 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Crosses; Node_Trace(L"CROSSES");}
break;
case 107:
#line 407 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Disjoint; Node_Trace(L"DISJOINT");}
break;
case 108:
#line 408 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Equals; Node_Trace(L"EQUALS");}
break;
case 109:
#line 409 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Inside; Node_Trace(L"INSIDE");}
break;
case 110:
#line 410 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Intersects; Node_Trace(L"INTERSECTS");}
break;
case 111:
#line 411 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Overlaps; Node_Trace(L"OVERLAPS");}
break;
case 112:
#line 412 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Touches; Node_Trace(L"TOUCHES");}
break;
case 113:
#line 413 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Within; Node_Trace(L"WITHIN");}
break;
case 114:
#line 414 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_CoveredBy; Node_Trace(L"COVEREDBY");}
break;
case 115:
#line 415 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_EnvelopeIntersects; Node_Trace(L"ENVELOPEINTERSECTS");}
break;
case 116:
#line 419 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Copy(L"double", pParse->fdo_filter_yyvsp[0].m_node);}
break;
case 117:
#line 420 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"INTEGER", FdoDoubleValue::Create((double)pParse->fdo_filter_yyvsp[0].m_integer));}
break;
case 118:
#line 421 "Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y"
{pParse->fdo_filter_yyval.m_node=Node_Add(L"INT64", FdoDoubleValue::Create((double)pParse->fdo_filter_yyvsp[0].m_int64));}
break;
#line 1259 "yyFilter.cpp"
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
