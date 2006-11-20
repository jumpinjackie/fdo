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
#line 36 "Src/Fdo/Parse/yyExpression.y"

#include "Parse.h"
#pragma warning(disable:4102)	/* unreferenced labels in fdo_expression_yyExpression.cpp*/

#line 43 "Src/Fdo/Parse/yyExpression.y"
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
#define FdoToken_INT64 287
#define FdoToken_DOUBLE 288
#define FdoToken_DATETIME 289
#define FdoToken_BLOB 290
#define FdoToken_CLOB 291
#define FdoToken_Add 292
#define FdoToken_Subtract 293
#define FdoToken_Multiply 294
#define FdoToken_Divide 295
#define FdoToken_Negate 296
#define FdoToken_EQ 297
#define FdoToken_NE 298
#define FdoToken_GT 299
#define FdoToken_GE 300
#define FdoToken_LT 301
#define FdoToken_LE 302
#define FdoToken_LeftParenthesis 303
#define FdoToken_RightParenthesis 304
#define FdoToken_Comma 305
#define FdoToken_Dot 306
#define FdoToken_Colon 307
#define FdoToken_AS 308
#define FdoToken_BETWEEN 309
const short fdo_expression_yylhs[] = {                                        -1,
    0,    0,    3,    3,    3,    3,    3,    3,    3,    1,
    1,    1,    1,    2,    2,    2,    2,    2,    2,    2,
    2,    2,   15,   15,   14,   16,   17,   13,   18,    5,
    8,    4,    4,    4,    6,    7,    9,    9,   10,   10,
   11,   11,   12,
};
const short fdo_expression_yylen[] = {                                         2,
    1,    1,    3,    1,    1,    1,    1,    1,    1,    3,
    3,    3,    3,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    4,
    5,    0,    1,    3,    4,    1,    1,    1,    1,    1,
    1,    2,    2,
};
const short fdo_expression_yydefred[] = {                                      0,
    2,   22,   23,   24,    0,   36,   41,   25,   26,   27,
   28,   29,   20,   21,    0,    0,    0,    0,    5,   40,
    0,    6,   39,    0,    9,    8,   37,   38,    4,   16,
   19,   14,   17,   18,   15,    0,   43,   42,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   12,   13,
    0,    0,   35,    0,   30,    0,   31,    0,
};
const short fdo_expression_yydgoto[] = {                                      18,
   19,   20,   21,   52,   22,   23,   24,   25,   26,   27,
   28,   29,   30,   31,   32,   33,   34,   35,
};
const short fdo_expression_yysindex[] = {                                    -40,
    0,    0,    0,    0,  -32,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -26, -262,  -26,    0,    0,    0,
 -277,    0,    0,  -15,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -259,    0,    0,  -21,  -26,
  -26,  -26,  -26,  -26,   -9, -275, -282, -282,    0,    0,
 -277,  -38,    0, -249,    0,  -26,    0, -277,
};
const short fdo_expression_yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   35,    0,    0,    1,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -34,    0,    5,    9,   11,    0,    0,
  -22,    0,    0,    0,    0,    0,    0,  -20,
};
const short fdo_expression_yygindex[] = {                                      0,
    0,    0,  -13,    0,    0,    0,  -18,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,
};
#define FDO_EXPRESSION_YYTABLESIZE 304
const short fdo_expression_yytable[] = {                                      17,
    7,   37,   55,   39,    3,   56,   32,   36,   10,   32,
   11,   42,   43,   17,   40,   41,   42,   43,   33,   46,
   34,   33,   38,   34,   44,   45,   47,   48,   49,   50,
   51,   53,   54,    6,    1,   57,    0,    0,    0,    0,
    0,    7,   58,    0,    7,    3,    0,    0,    3,   10,
    0,   11,   10,    0,   11,    0,    0,    0,    0,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    1,    2,    3,    4,    0,
    0,    0,    5,    0,    0,    0,    0,    0,    0,    0,
    2,    3,    4,    0,    0,    0,    5,    0,    0,    0,
    0,    0,    6,    7,    8,    9,   10,   11,   12,   13,
   14,    0,    0,    0,    0,   15,    6,    7,    8,    9,
   10,   11,   12,   13,   14,    0,   16,    0,    0,   15,
   40,   41,   42,   43,    0,    0,    0,    0,    0,    0,
   16,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    7,    7,    7,    7,    3,    3,    3,    3,
   10,   10,   11,   11,
};
const short fdo_expression_yycheck[] = {                                      40,
    0,   15,   41,   17,    0,   44,   41,   40,    0,   44,
    0,  294,  295,   40,  292,  293,  294,  295,   41,   41,
   41,   44,  285,   44,   40,  285,   40,   41,   42,   43,
   44,   41,  308,  283,    0,   54,   -1,   -1,   -1,   -1,
   -1,   41,   56,   -1,   44,   41,   -1,   -1,   44,   41,
   -1,   41,   44,   -1,   44,   -1,   -1,   -1,   -1,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  256,  257,  258,  259,   -1,
   -1,   -1,  263,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  257,  258,  259,   -1,   -1,   -1,  263,   -1,   -1,   -1,
   -1,   -1,  283,  284,  285,  286,  287,  288,  289,  290,
  291,   -1,   -1,   -1,   -1,  296,  283,  284,  285,  286,
  287,  288,  289,  290,  291,   -1,  307,   -1,   -1,  296,
  292,  293,  294,  295,   -1,   -1,   -1,   -1,   -1,   -1,
  307,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  292,  293,  294,  295,  292,  293,  294,  295,
  292,  293,  292,  293,
};
#define FDO_EXPRESSION_YYFINAL 18
#ifndef FDO_EXPRESSION_YYDEBUG
#define FDO_EXPRESSION_YYDEBUG 0
#endif
#define FDO_EXPRESSION_YYMAXTOKEN 309
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
"FdoToken_TIMESTAMP","FdoToken_GEOMFROMTEXT","FdoToken_AND","FdoToken_OR",
"FdoToken_NOT","FdoToken_LIKE","FdoToken_IN","FdoToken_BEYOND",
"FdoToken_WITHINDISTANCE","FdoToken_CONTAINS","FdoToken_COVEREDBY",
"FdoToken_CROSSES","FdoToken_DISJOINT","FdoToken_ENVELOPEINTERSECTS",
"FdoToken_EQUALS","FdoToken_INTERSECTS","FdoToken_INSIDE","FdoToken_OVERLAPS",
"FdoToken_TOUCHES","FdoToken_WITHIN","FdoToken_RELATE","FdoToken_IDENTIFIER",
"FdoToken_PARAMETER","FdoToken_STRING","FdoToken_INTEGER","FdoToken_INT64",
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
"DataValue : string",
"DataValue : FdoToken_BLOB",
"DataValue : FdoToken_CLOB",
"DataValue : FdoToken_NULL",
"boolean : FdoToken_TRUE",
"boolean : FdoToken_FALSE",
"string : FdoToken_STRING",
"integer : FdoToken_INTEGER",
"int64 : FdoToken_INT64",
"double : FdoToken_DOUBLE",
"datetime : FdoToken_DATETIME",
"Function : Identifier '(' ExpressionCollection ')'",
"ComputedIdentifier : '(' Expression ')' FdoToken_AS Identifier",
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
#line 218 "Src/Fdo/Parse/yyExpression.y"
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
#line 362 "yyExpression.cpp"
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
#line 108 "Src/Fdo/Parse/yyExpression.y"
{pParse->SetRoot(Node_Copy(L"Expression", pParse->fdo_expression_yyvsp[0].m_node));	FDO_EXPRESSION_YYACCEPT;}
break;
case 2:
#line 109 "Src/Fdo/Parse/yyExpression.y"
{Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	FDO_EXPRESSION_YYABORT;}
break;
case 3:
#line 113 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"(Expression)", pParse->fdo_expression_yyvsp[-1].m_node);}
break;
case 4:
#line 114 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"UnaryExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 5:
#line 115 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"BinaryExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 6:
#line 116 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Function", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 7:
#line 117 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Identifier", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 8:
#line 118 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"ValueExpression", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 9:
#line 119 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"ComputedIdentifier", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 10:
#line 123 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Add, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 11:
#line 124 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Subtract, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 12:
#line 125 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Multiply, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 13:
#line 126 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_expression_yyvsp[-2].m_node, FdoBinaryOperations_Divide, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[-2].m_node); FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 14:
#line 131 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"boolean", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 15:
#line 133 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"datetime", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 16:
#line 136 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"double", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 17:
#line 138 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"integer", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 18:
#line 140 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"int64", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 19:
#line 142 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"string", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 20:
#line 144 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"TODO:BLOB", NULL);}
break;
case 21:
#line 146 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"TODO:CLOB", NULL);}
break;
case 22:
#line 148 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
break;
case 23:
#line 152 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
break;
case 24:
#line 153 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
break;
case 25:
#line 156 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"STRING", FdoDataValue::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 26:
#line 159 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INTEGER", FdoInt32Value::Create(pParse->fdo_expression_yyvsp[0].m_integer));}
break;
case 27:
#line 162 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"INT64", FdoInt64Value::Create(pParse->fdo_expression_yyvsp[0].m_int64));}
break;
case 28:
#line 165 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"DOUBLE", FdoDoubleValue::Create(pParse->fdo_expression_yyvsp[0].m_double));}
break;
case 29:
#line 168 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"DateTime", FdoDataValue::Create(pParse->fdo_expression_yyvsp[0].m_datetime));}
break;
case 30:
#line 173 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[-3].m_node)->GetName(), (FdoExpressionCollection*)pParse->fdo_expression_yyvsp[-1].m_node));FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[-3].m_node); FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[-1].m_node);}
break;
case 31:
#line 178 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_expression_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_expression_yyvsp[-3].m_node));FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[-3].m_node); FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 32:
#line 181 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
break;
case 33:
#line 182 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"Expression Arg 1,");FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[0].m_node); }
break;
case 34:
#line 183 "Src/Fdo/Parse/yyExpression.y"
{((FdoExpressionCollection*)pParse->fdo_expression_yyval.m_node)->Add((FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node); Node_Trace(L"Expression Arg N,");FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[0].m_node); }
break;
case 35:
#line 188 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue(pParse->fdo_expression_yyvsp[-1].m_string));}
break;
case 36:
#line 193 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 37:
#line 197 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"LiteralValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 38:
#line 198 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"Parameter", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 39:
#line 202 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"GeometryValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 40:
#line 203 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node=Node_Copy(L"DataValue", pParse->fdo_expression_yyvsp[0].m_node);}
break;
case 41:
#line 208 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"PARAMETER", FdoParameter::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 42:
#line 210 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L":Parameter", FdoParameter::Create(pParse->fdo_expression_yyvsp[0].m_string));}
break;
case 43:
#line 214 "Src/Fdo/Parse/yyExpression.y"
{pParse->fdo_expression_yyval.m_node = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)pParse->fdo_expression_yyvsp[0].m_node));FDO_SAFE_RELEASE(pParse->fdo_expression_yyvsp[0].m_node);}
break;
#line 729 "yyExpression.cpp"
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
