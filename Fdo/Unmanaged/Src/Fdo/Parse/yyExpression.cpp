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
    2,    2,    2,    2,   15,   15,   14,   16,   17,   18,
   19,   13,   20,    5,    8,    8,    4,    4,    4,    6,
    7,    9,    9,   10,   10,   11,   11,   12,
};
const short fdo_expression_yylen[] = {                                         2,
    1,    1,    3,    1,    1,    1,    1,    1,    1,    3,
    3,    3,    3,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    4,    5,    3,    0,    1,    3,    4,
    1,    1,    1,    1,    1,    1,    2,    2,
};
const short fdo_expression_yydefred[] = {                                      0,
    2,   24,   25,   26,    0,   41,   46,   27,   28,   29,
   30,   31,   32,   33,   22,   23,    0,    0,    0,    0,
    5,   45,    0,    6,   44,    0,    9,    8,   42,   43,
    4,   16,   21,   14,   17,   18,   19,   20,   15,    0,
    0,   47,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   36,    0,    0,   40,    0,
   34,    0,   35,    0,
};
const short fdo_expression_yydgoto[] = {                                      20,
   21,   22,   23,   58,   24,   25,   26,   27,   28,   29,
   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
};
const short fdo_expression_yysindex[] = {                                    -40,
    0,    0,    0,    0,  -28,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   37, -283,   37,    0,
    0,    0, -277,    0,    0,  -17,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -258,
 -281,    0,  -35,   37,   37,   37,   37, -253,   37,   -2,
 -278, -297, -297, -281, -281,    0, -277,  -30,    0, -253,
    0,   37,    0, -277,
};
const short fdo_expression_yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   43,    0,    0,    1,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    5,    0,    0,    0,    0,    0,    0,    0,  -25,    0,
    9,   22,   24,   13,   18,    0,  -24,    0,    0,    0,
    0,    0,    0,   -8,
};
const short fdo_expression_yygindex[] = {                                      0,
    0,    0,  -15,    0,    0,    0,  -45,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
#define FDO_EXPRESSION_YYTABLESIZE 354
const short fdo_expression_yytable[] = {                                      19,
    7,   41,   56,   43,   48,   51,   46,   47,    3,   42,
   61,   40,   12,   62,   63,   37,   38,   13,   37,   38,
   48,   10,   49,   11,   44,   45,   46,   47,   52,   53,
   54,   55,   39,   57,   50,   39,   48,    6,   59,   60,
   48,    7,    1,    0,    7,   48,   64,    0,   48,    3,
    0,    0,    3,   12,    0,    0,   12,    0,   13,    0,
    0,   13,   10,    0,   11,   10,    0,   11,    0,    0,
    0,    0,    0,    0,    0,    0,   19,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    1,    2,    3,    4,    0,
    0,    0,    5,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
   16,    0,    0,    0,    0,   17,   44,   45,   46,   47,
    0,    0,    0,    0,    0,    0,   18,    0,    0,    0,
    0,    0,   48,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    2,    3,    4,    0,    0,    0,    5,
    0,    0,    7,    7,    7,    7,   48,   48,   48,   48,
    3,    3,    3,    3,   12,   12,   12,   12,    7,   13,
   13,   13,   13,   10,   10,   11,   11,    6,    7,    8,
    9,   10,   11,   12,   13,   14,   15,   16,    0,    0,
    0,    0,   17,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   18,
};
const short fdo_expression_yycheck[] = {                                      40,
    0,   17,   48,   19,    0,   41,  304,  305,    0,  293,
   41,   40,    0,   44,   60,   41,   41,    0,   44,   44,
  318,    0,   40,    0,  302,  303,  304,  305,   44,   45,
   46,   47,   41,   49,  293,   44,  318,  291,   41,  318,
  318,   41,    0,   -1,   44,   41,   62,   -1,   44,   41,
   -1,   -1,   44,   41,   -1,   -1,   44,   -1,   41,   -1,
   -1,   44,   41,   -1,   41,   44,   -1,   44,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   40,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  256,  257,  258,  259,   -1,
   -1,   -1,  263,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
  301,   -1,   -1,   -1,   -1,  306,  302,  303,  304,  305,
   -1,   -1,   -1,   -1,   -1,   -1,  317,   -1,   -1,   -1,
   -1,   -1,  318,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  257,  258,  259,   -1,   -1,   -1,  263,
   -1,   -1,  302,  303,  304,  305,  302,  303,  304,  305,
  302,  303,  304,  305,  302,  303,  304,  305,  318,  302,
  303,  304,  305,  302,  303,  302,  303,  291,  292,  293,
  294,  295,  296,  297,  298,  299,  300,  301,   -1,   -1,
   -1,   -1,  306,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  317,
};
#define FDO_EXPRESSION_YYFINAL 20
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
"ComputedIdentifier : '(' Expression ')' FdoToken_AS Identifier",
"ComputedIdentifier : Expression FdoToken_AS Identifier",
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
#line 231 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
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
#line 393 "yyExpression.cpp"
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
#line 111 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->SetRoot(Node_Copy(L"Expression", pParse->fdo_expression_yyvsp[0].m_node));	FDO_EXPRESSION_YYACCEPT;}
break;
case 2:
#line 112 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	FDO_EXPRESSION_YYABORT;}
break;
case 3:
#line 116 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"(Expression)", pParse->fdo_expression_yyvsp[-1].m_node);}
break;
case 4:
#line 117 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"UnaryExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 5:
#line 118 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"BinaryExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 6:
#line 119 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Function", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 7:
#line 120 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Identifier", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 8:
#line 121 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"ValueExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 9:
#line 122 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"ComputedIdentifier", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 10:
#line 126 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Add, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 11:
#line 127 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Subtract, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 12:
#line 128 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Multiply, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 13:
#line 129 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Divide, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
case 14:
#line 134 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"boolean", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 15:
#line 136 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"datetime", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 16:
#line 139 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"double", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 17:
#line 141 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"integer", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 18:
#line 143 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"int64", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 19:
#line 145 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"inthex", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 20:
#line 147 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"intbin", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 21:
#line 149 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"string", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 22:
#line 151 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"TODO:BLOB", NULL);}
break;
case 23:
#line 153 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"TODO:CLOB", NULL);}
break;
case 24:
#line 155 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
break;
case 25:
#line 159 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
break;
case 26:
#line 160 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
break;
case 27:
#line 163 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"STRING", FdoDataValue::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 28:
#line 166 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INTEGER", FdoInt32Value::Create(pParse->fdo_expression_yyvsp[0].m_integer));}
break;
case 29:
#line 169 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INT64", FdoInt64Value::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
break;
case 30:
#line 172 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INTHEX", FdoIntHexValue::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
break;
case 31:
#line 175 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INTBIN", FdoIntBinValue::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
break;
case 32:
#line 178 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"DOUBLE", FdoDoubleValue::Create(pParse->fdo_expression_yyvsp[0].m_double));}
break;
case 33:
#line 181 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"DateTime", FdoDataValue::Create(pParse->fdo_expression_yyvsp[0].m_datetime));}
break;
case 34:
#line 186 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node)->GetName(), (FdoExpressionCollection*)pParse->fdo_expression_yyvsp[-1].m_node));}
break;
case 35:
#line 190 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));}
break;
case 36:
#line 191 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node));}
break;
case 37:
#line 194 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
break;
case 38:
#line 195 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"Expression Arg 1,");}
break;
case 39:
#line 196 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{((FdoExpressionCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"Expression Arg N,");}
break;
case 40:
#line 201 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue(pParse->fdo_expression_yyvsp[-1].m_string));}
break;
case 41:
#line 206 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 42:
#line 210 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"LiteralValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 43:
#line 211 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Parameter", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 44:
#line 215 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"GeometryValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 45:
#line 216 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"DataValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 46:
#line 221 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"PARAMETER", FdoParameter::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 47:
#line 223 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L":Parameter", FdoParameter::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 48:
#line 227 "Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));}
break;
#line 780 "yyExpression.cpp"
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
