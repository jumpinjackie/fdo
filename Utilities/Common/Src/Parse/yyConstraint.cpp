#ifndef lint
static char const 
fdo_constraint_yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define FDO_CONSTRAINT_YYBYACC 1
#define FDO_CONSTRAINT_YYMAJOR 1
#define FDO_CONSTRAINT_YYMINOR 9
#define FDO_CONSTRAINT_YYLEX fdo_constraint_yylex(pParse)
#define FDO_CONSTRAINT_YYEMPTY -1
#define fdo_constraint_yyclearin (pParse->fdo_constraint_yychar=(FDO_CONSTRAINT_YYEMPTY))
#define fdo_constraint_yyerrok (pParse->fdo_constraint_yyerrflag=0)
#define FDO_CONSTRAINT_YYRECOVERING() (pParse->fdo_constraint_yyerrflag!=0)
class FdoCommonParse;static int fdo_constraint_yygrowstack(FdoCommonParse *pParse);
#define FDO_CONSTRAINT_YYPREFIX "fdo_constraint_yy"
#line 36 "Utilities/Common/Src/Parse/yyConstraint.y"

#include <FdoStd.h>
#include <Fdo/Expression/StringValue.h>
#include <Fdo/Expression/Int32Value.h>
#include <Fdo/Expression/DoubleValue.h>
#include <Fdo/Expression/DateTimeValue.h>
#include <Fdo/Expression/BooleanValue.h>
#include <Fdo/Schema/PropertyValueConstraintRange.h>
#include <Fdo/Schema/PropertyValueConstraintList.h>
#include <Fdo/Expression/DataValueCollection.h>
#include <Parse/Types.h>
#include <Parse/Parse.h>

#pragma warning(disable:4102)	/* unreferenced labels in fdo_constraint_yyConstraint.cpp*/

#line 53 "Utilities/Common/Src/Parse/yyConstraint.y"
#if 0
{
	FdoIDisposable*	m_node;		/* constraint parse tree node*/
	FdoInt32		m_id;		/* enumerations, keywords, ...*/
	FdoInt32		m_integer;	/* integer values (Int8, Int16, Int32, TODO:I64 )*/
	double			m_double;	/* floating point values (single(float), double)*/
	FdoString*		m_string;	/* string*/
	_FdoDateTime	m_datetime;	/* date time*/
#endif
#line 43 "yyConstraint.cpp"
#define FDO_CONSTRAINT_YYERRCODE 256
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
#define FdoToken_LeftSquareBrackets 304
#define FdoToken_RightSquareBrackets 305
#define FdoToken_Comma 306
#define FdoToken_Dot 307
#define FdoToken_Colon 308
#define FdoToken_AS 309
#define FdoToken_BETWEEN 310
const short fdo_constraint_yylhs[] = {                                        -1,
    0,    0,    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,   10,   11,   12,   12,   12,   12,
    5,    5,    8,    8,    7,    7,    6,    6,    9,    9,
    2,    2,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,   15,   15,   14,   16,   13,   17,    3,    3,
    3,
};
const short fdo_constraint_yylen[] = {                                         2,
    1,    1,    3,    1,    1,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    5,    5,    0,    1,    3,    3,
    3,    5,    3,    5,    3,    5,    3,    5,    3,    5,
    0,    1,    3,    3,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    3,
    3,
};
const short fdo_constraint_yydefred[] = {                                      0,
    2,   49,    0,    0,    0,    0,    1,    0,    0,   18,
    8,   10,    4,    5,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   51,    3,   19,    0,   50,
    0,   42,   43,   44,   45,   46,   47,   48,   40,   41,
    0,   25,   37,   39,   35,   38,   36,   21,   27,   23,
   29,    0,    0,    0,   11,   12,   13,   14,    0,    0,
    0,    0,    0,    0,    0,    0,   32,    0,    0,    0,
    0,    0,    0,   26,   22,   28,   24,   30,   16,    0,
   34,   15,    0,    0,   33,    0,    0,
};
const short fdo_constraint_yydgoto[] = {                                       5,
   52,   78,   64,    7,    8,    9,   10,   11,   12,   13,
   14,   15,   53,   54,   55,   56,   57,
};
const short fdo_constraint_yysindex[] = {                                    -27,
    0,    0,  -19,  -18,    0, -249,    0, -259, -253,    0,
    0,    0,    0,    0, -250,  -41,   18,  -34,  -18,  -52,
   15,  -36,  -36,  -36,  -36,  -36,  -36,  -15,  -15,  -13,
  -36,  -36,  -36,  -36,  -36,    0,    0,    0,   19,    0,
  -36,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -36,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -210,  -18, -291,    0,    0,    0,    0,  -13, -234,
 -250,   22,   25,   27,   28,   30,    0,  -21,   33,  -36,
  -38,  -35,  -34,    0,    0,    0,    0,    0,    0,  -36,
    0,    0,  -36,  -36,    0,   28,   30,
};
const short fdo_constraint_yyrindex[] = {                                      2,
    0,    0,  -33,    0,    0,    0,    0,   12,   24,    0,
    0,    0,    0,    0,   70,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    4,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -33,    0,
   26, -190, -185, -184,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,
};
const short fdo_constraint_yygindex[] = {                                      0,
    5,    0,   14,   78,    0,    0,    0,    6,   29,    0,
    0,   13,    0,    0,    0,    0,    0,
};
#define FDO_CONSTRAINT_YYTABLESIZE 270
const short fdo_constraint_yytable[] = {                                      36,
   17,   17,   36,   51,   28,   36,   38,   17,   25,   26,
   29,    7,    3,    6,   30,   18,   16,   20,   21,   89,
    3,   19,   90,    9,   63,   20,   69,   58,   59,   60,
   61,   62,   39,   65,   67,   72,   73,   74,   75,   76,
   40,   17,   71,   70,   31,   77,   22,   31,   23,   24,
   25,   26,    7,   80,   41,   79,   66,   68,   37,   36,
   27,   22,   84,    4,    9,   85,   20,   86,   87,    6,
   88,    4,    4,   91,   25,    4,   81,    4,   21,   27,
   17,   83,   82,    0,   92,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   95,    0,    0,   96,   97,    0,
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
   42,   43,   44,    0,    0,    0,   21,    0,    1,    0,
   30,   17,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   45,   46,
   47,   48,   49,   50,   31,    2,   32,   33,   34,   35,
   31,   93,   94,    2,    2,   17,   17,    2,   27,    2,
};
const short fdo_constraint_yycheck[] = {                                      41,
    0,    0,   41,   40,  264,   41,   41,   41,  300,  301,
  264,    0,   40,    0,  265,    3,    3,    4,  268,   41,
   40,   40,   44,    0,   40,    0,   40,   23,   24,   25,
   26,   27,   19,   28,   29,   31,   32,   33,   34,   35,
   93,   41,   30,   30,   41,   41,  296,   44,  298,  299,
  300,  301,   41,  264,   40,   51,   28,   29,   41,   41,
  310,  296,   41,   91,   41,   41,   41,   41,   41,    0,
   41,   91,   91,   41,  265,   91,   63,   91,  264,  264,
    3,   69,   69,   -1,   80,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   90,   -1,   -1,   93,   94,   -1,
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
  257,  258,  259,   -1,   -1,   -1,  268,   -1,  256,   -1,
  265,  265,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  285,  286,
  287,  288,  289,  290,  296,  283,  298,  299,  300,  301,
  296,  300,  301,  283,  283,  265,  265,  283,  310,  283,
};
#define FDO_CONSTRAINT_YYFINAL 5
#ifndef FDO_CONSTRAINT_YYDEBUG
#define FDO_CONSTRAINT_YYDEBUG 0
#endif
#define FDO_CONSTRAINT_YYMAXTOKEN 310
#if FDO_CONSTRAINT_YYDEBUG
const char * const fdo_constraint_yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,"']'",0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"FdoToken_NULL","FdoToken_TRUE","FdoToken_FALSE","FdoToken_DATE",
"FdoToken_TIME","FdoToken_TIMESTAMP","FdoToken_GEOMFROMTEXT","FdoToken_AND",
"FdoToken_OR","FdoToken_NOT","FdoToken_LIKE","FdoToken_IN","FdoToken_BEYOND",
"FdoToken_WITHINDISTANCE","FdoToken_CONTAINS","FdoToken_COVEREDBY",
"FdoToken_CROSSES","FdoToken_DISJOINT","FdoToken_ENVELOPEINTERSECTS",
"FdoToken_EQUALS","FdoToken_INTERSECTS","FdoToken_INSIDE","FdoToken_OVERLAPS",
"FdoToken_TOUCHES","FdoToken_WITHIN","FdoToken_RELATE","FdoToken_IDENTIFIER",
"FdoToken_PARAMETER","FdoToken_STRING","FdoToken_INTEGER","FdoToken_DOUBLE",
"FdoToken_DATETIME","FdoToken_BLOB","FdoToken_CLOB","FdoToken_Add",
"FdoToken_Subtract","FdoToken_Multiply","FdoToken_Divide","FdoToken_Negate",
"FdoToken_EQ","FdoToken_NE","FdoToken_GT","FdoToken_GE","FdoToken_LT",
"FdoToken_LE","FdoToken_LeftParenthesis","FdoToken_RightParenthesis",
"FdoToken_LeftSquareBrackets","FdoToken_RightSquareBrackets","FdoToken_Comma",
"FdoToken_Dot","FdoToken_Colon","FdoToken_AS","FdoToken_BETWEEN",
};
const char * const fdo_constraint_yyrule[] = {
"$accept : fdo",
"fdo : Constraint",
"fdo : error",
"Constraint : '(' Constraint ')'",
"Constraint : BEConstraint",
"Constraint : INConstraint",
"Constraint : ORConstraint",
"Constraint : GTConstraint",
"Constraint : LTConstraint",
"Constraint : GEConstraint",
"Constraint : LEConstraint",
"Constraint : GTConstraint FdoToken_AND LTConstraint",
"Constraint : GTConstraint FdoToken_AND LEConstraint",
"Constraint : GEConstraint FdoToken_AND LTConstraint",
"Constraint : GEConstraint FdoToken_AND LEConstraint",
"BEConstraint : Identifier FdoToken_BETWEEN DataValue FdoToken_AND DataValue",
"INConstraint : Identifier FdoToken_IN '(' DataValueCollection ')'",
"ORConstraint :",
"ORConstraint : EqualConstraint",
"ORConstraint : '(' ORConstraint ')'",
"ORConstraint : ORConstraint FdoToken_OR ORConstraint",
"GTConstraint : Identifier FdoToken_GT DataValue",
"GTConstraint : '(' Identifier FdoToken_GT DataValue ')'",
"LTConstraint : Identifier FdoToken_LT DataValue",
"LTConstraint : '(' Identifier FdoToken_LT DataValue ')'",
"EqualConstraint : Identifier FdoToken_EQ DataValue",
"EqualConstraint : '(' Identifier FdoToken_EQ DataValue ')'",
"GEConstraint : Identifier FdoToken_GE DataValue",
"GEConstraint : '(' Identifier FdoToken_GE DataValue ')'",
"LEConstraint : Identifier FdoToken_LE DataValue",
"LEConstraint : '(' Identifier FdoToken_LE DataValue ')'",
"DataValueCollection :",
"DataValueCollection : DataValue",
"DataValueCollection : DataValueCollection ',' DataValue",
"DataValue : '(' DataValue ')'",
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
"Identifier : FdoToken_IDENTIFIER",
"Identifier : '[' Identifier ']'",
"Identifier : '(' Identifier ')'",
};
#endif
#if FDO_CONSTRAINT_YYDEBUG
#include <stdio.h>
#endif
#ifdef FDO_CONSTRAINT_YYSTACKSIZE
#undef FDO_CONSTRAINT_YYMAXDEPTH
#define FDO_CONSTRAINT_YYMAXDEPTH FDO_CONSTRAINT_YYSTACKSIZE
#else
#ifdef FDO_CONSTRAINT_YYMAXDEPTH
#define FDO_CONSTRAINT_YYSTACKSIZE FDO_CONSTRAINT_YYMAXDEPTH
#else
#define FDO_CONSTRAINT_YYSTACKSIZE 10000
#define FDO_CONSTRAINT_YYMAXDEPTH 10000
#endif
#endif
#define FDO_CONSTRAINT_YYINITSTACKSIZE 200
#line 410 "Utilities/Common/Src/Parse/yyConstraint.y"
///////////////////////////////////////////////////////////////////////

#include <stdio.h>

FdoInt32 fdo_constraint_yyerror_default(char *str, FdoInt32 lineno)
{
#ifdef FDO_FDO_CONSTRAINT_YYDEBUG
	fprintf(stderr, "line %d: %s\n", lineno, str);
#endif
	return 0;
}

typedef FdoInt32 (*ERRFUNC)(char*, FdoInt32);
static ERRFUNC fdo_constraint_yyerror_func = fdo_constraint_yyerror_default;

FdoInt32 fdo_constraint_yyerror(FdoCommonParse *pParse, char* str)
{
	return (*fdo_constraint_yyerror_func)(str, pParse->fdo_constraint_yylineno);
}

ERRFUNC fdo_constraint_yyerror_set(ERRFUNC func)
{
	ERRFUNC old_func = fdo_constraint_yyerror_func;

	if (func != NULL)
		fdo_constraint_yyerror_func = func;

	return old_func;
}

static void  print_current_token()
{
//TODO: this has not been updated, but it is unused anyway
#ifdef FDO_FDO_CONSTRAINT_YYDEBUG
	char  *token_name;
	
	if( pParse->fdo_constraint_yychar <= FDO_CONSTRAINT_YYMAXTOKEN )
	{
		switch( pParse->fdo_constraint_yychar )
		{
			case IDENTIFIER:
				token_name = pParse->fdo_constraint_yylval.id;
				break;

			default:
				token_name = fdo_constraint_yyname[pParse->fdo_constraint_yychar];
				break;
		}
	}
	else
	{
		token_name = "illegal-name";
	}

	printf( "***  current token is %d (%s)  ***\n", pParse->fdo_constraint_yychar, token_name );
#endif
}
#line 387 "yyConstraint.cpp"
/* allocate initial stack or double stack size, up to FDO_CONSTRAINT_YYMAXDEPTH */
static int fdo_constraint_yygrowstack(FdoCommonParse *pParse)
{
    int newsize, i;
    short *newss;
    FDO_CONSTRAINT_YYSTYPE *newvs;

    if ((newsize = pParse->fdo_constraint_yystacksize) == 0)
        newsize = FDO_CONSTRAINT_YYINITSTACKSIZE;
    else if (newsize >= FDO_CONSTRAINT_YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > FDO_CONSTRAINT_YYMAXDEPTH)
        newsize = FDO_CONSTRAINT_YYMAXDEPTH;
    i = pParse->fdo_constraint_yyssp - pParse->fdo_constraint_yyss;
    newss = pParse->fdo_constraint_yyss ? (short *)realloc(pParse->fdo_constraint_yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    pParse->fdo_constraint_yyss = newss;
    pParse->fdo_constraint_yyssp = newss + i;
    newvs = pParse->fdo_constraint_yyvs ? (FDO_CONSTRAINT_YYSTYPE *)realloc(pParse->fdo_constraint_yyvs, newsize * sizeof *newvs) :
      (FDO_CONSTRAINT_YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    pParse->fdo_constraint_yyvs = newvs;
    pParse->fdo_constraint_yyvsp = newvs + i;
    pParse->fdo_constraint_yystacksize = newsize;
    pParse->fdo_constraint_yysslim = pParse->fdo_constraint_yyss + newsize - 1;
    return 0;
}

#define FDO_CONSTRAINT_YYABORT goto fdo_constraint_yyabort
#define FDO_CONSTRAINT_YYREJECT goto fdo_constraint_yyabort
#define FDO_CONSTRAINT_YYACCEPT goto fdo_constraint_yyaccept
#define FDO_CONSTRAINT_YYERROR goto fdo_constraint_yyerrlab

#ifndef FDO_CONSTRAINT_YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define FDO_CONSTRAINT_YYPARSE_PARAM_ARG void
#define FDO_CONSTRAINT_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define FDO_CONSTRAINT_YYPARSE_PARAM_ARG
#define FDO_CONSTRAINT_YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* FDO_CONSTRAINT_YYPARSE_PARAM */
#ifndef FDO_CONSTRAINT_YYPARSE_PARAM_TYPE
#define FDO_CONSTRAINT_YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define FDO_CONSTRAINT_YYPARSE_PARAM_ARG FDO_CONSTRAINT_YYPARSE_PARAM_TYPE FDO_CONSTRAINT_YYPARSE_PARAM
#define FDO_CONSTRAINT_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define FDO_CONSTRAINT_YYPARSE_PARAM_ARG FDO_CONSTRAINT_YYPARSE_PARAM
#define FDO_CONSTRAINT_YYPARSE_PARAM_DECL FDO_CONSTRAINT_YYPARSE_PARAM_TYPE FDO_CONSTRAINT_YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! FDO_CONSTRAINT_YYPARSE_PARAM */

int
fdo_constraint_yyparse(FdoCommonParse* pParse)
    FDO_CONSTRAINT_YYPARSE_PARAM_DECL
{
    register int fdo_constraint_yym, fdo_constraint_yyn, fdo_constraint_yystate;
#if FDO_CONSTRAINT_YYDEBUG
    register const char *fdo_constraint_yys;

    if ((fdo_constraint_yys = getenv("FDO_CONSTRAINT_YYDEBUG")))
    {
        fdo_constraint_yyn = *fdo_constraint_yys;
        if (fdo_constraint_yyn >= '0' && fdo_constraint_yyn <= '9')
            pParse->fdo_constraint_yydebug = fdo_constraint_yyn - '0';
    }
#endif

    pParse->fdo_constraint_yynerrs = 0;
    pParse->fdo_constraint_yyerrflag = 0;
    pParse->fdo_constraint_yychar = (-1);

    if (pParse->fdo_constraint_yyss == NULL && fdo_constraint_yygrowstack(pParse)) goto fdo_constraint_yyoverflow;
    pParse->fdo_constraint_yyssp = pParse->fdo_constraint_yyss;
    pParse->fdo_constraint_yyvsp = pParse->fdo_constraint_yyvs;
    *pParse->fdo_constraint_yyssp = fdo_constraint_yystate = 0;

fdo_constraint_yyloop:
    if ((fdo_constraint_yyn = fdo_constraint_yydefred[fdo_constraint_yystate])) goto fdo_constraint_yyreduce;
    if (pParse->fdo_constraint_yychar < 0)
    {
        if ((pParse->fdo_constraint_yychar = fdo_constraint_yylex(pParse)) < 0) pParse->fdo_constraint_yychar = 0;
#if FDO_CONSTRAINT_YYDEBUG
        if (pParse->fdo_constraint_yydebug)
        {
            fdo_constraint_yys = 0;
            if (pParse->fdo_constraint_yychar <= FDO_CONSTRAINT_YYMAXTOKEN) fdo_constraint_yys = fdo_constraint_yyname[pParse->fdo_constraint_yychar];
            if (!fdo_constraint_yys) fdo_constraint_yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    FDO_CONSTRAINT_YYPREFIX, fdo_constraint_yystate, pParse->fdo_constraint_yychar, fdo_constraint_yys);
        }
#endif
    }
    if ((fdo_constraint_yyn = fdo_constraint_yysindex[fdo_constraint_yystate]) && (fdo_constraint_yyn += pParse->fdo_constraint_yychar) >= 0 &&
            fdo_constraint_yyn <= FDO_CONSTRAINT_YYTABLESIZE && fdo_constraint_yycheck[fdo_constraint_yyn] == pParse->fdo_constraint_yychar)
    {
#if FDO_CONSTRAINT_YYDEBUG
        if (pParse->fdo_constraint_yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    FDO_CONSTRAINT_YYPREFIX, fdo_constraint_yystate, fdo_constraint_yytable[fdo_constraint_yyn]);
#endif
        if (pParse->fdo_constraint_yyssp >= pParse->fdo_constraint_yysslim && fdo_constraint_yygrowstack(pParse))
        {
            goto fdo_constraint_yyoverflow;
        }
        *++pParse->fdo_constraint_yyssp = fdo_constraint_yystate = fdo_constraint_yytable[fdo_constraint_yyn];
        *++pParse->fdo_constraint_yyvsp = pParse->fdo_constraint_yylval;
        pParse->fdo_constraint_yychar = (-1);
        if (pParse->fdo_constraint_yyerrflag > 0)  --pParse->fdo_constraint_yyerrflag;
        goto fdo_constraint_yyloop;
    }
    if ((fdo_constraint_yyn = fdo_constraint_yyrindex[fdo_constraint_yystate]) && (fdo_constraint_yyn += pParse->fdo_constraint_yychar) >= 0 &&
            fdo_constraint_yyn <= FDO_CONSTRAINT_YYTABLESIZE && fdo_constraint_yycheck[fdo_constraint_yyn] == pParse->fdo_constraint_yychar)
    {
        fdo_constraint_yyn = fdo_constraint_yytable[fdo_constraint_yyn];
        goto fdo_constraint_yyreduce;
    }
    if (pParse->fdo_constraint_yyerrflag) goto fdo_constraint_yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto fdo_constraint_yynewerror;
#endif
fdo_constraint_yynewerror:
   fdo_constraint_yyerror( pParse,"syntax error");
#if defined(lint) || defined(__GNUC__)
    goto fdo_constraint_yyerrlab;
#endif
fdo_constraint_yyerrlab:
    ++pParse->fdo_constraint_yynerrs;
fdo_constraint_yyinrecovery:
    if (pParse->fdo_constraint_yyerrflag < 3)
    {
        pParse->fdo_constraint_yyerrflag = 3;
        for (;;)
        {
            if ((fdo_constraint_yyn = fdo_constraint_yysindex[*pParse->fdo_constraint_yyssp]) && (fdo_constraint_yyn += FDO_CONSTRAINT_YYERRCODE) >= 0 &&
                    fdo_constraint_yyn <= FDO_CONSTRAINT_YYTABLESIZE && fdo_constraint_yycheck[fdo_constraint_yyn] == FDO_CONSTRAINT_YYERRCODE)
            {
#if FDO_CONSTRAINT_YYDEBUG
                if (pParse->fdo_constraint_yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", FDO_CONSTRAINT_YYPREFIX, *pParse->fdo_constraint_yyssp, fdo_constraint_yytable[fdo_constraint_yyn]);
#endif
                if (pParse->fdo_constraint_yyssp >= pParse->fdo_constraint_yysslim && fdo_constraint_yygrowstack(pParse))
                {
                    goto fdo_constraint_yyoverflow;
                }
                *++pParse->fdo_constraint_yyssp = fdo_constraint_yystate = fdo_constraint_yytable[fdo_constraint_yyn];
                *++pParse->fdo_constraint_yyvsp = pParse->fdo_constraint_yylval;
                goto fdo_constraint_yyloop;
            }
            else
            {
#if FDO_CONSTRAINT_YYDEBUG
                if (pParse->fdo_constraint_yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            FDO_CONSTRAINT_YYPREFIX, *pParse->fdo_constraint_yyssp);
#endif
                if (pParse->fdo_constraint_yyssp <= pParse->fdo_constraint_yyss) goto fdo_constraint_yyabort;
                --pParse->fdo_constraint_yyssp;
                --pParse->fdo_constraint_yyvsp;
            }
        }
    }
    else
    {
        if (pParse->fdo_constraint_yychar == 0) goto fdo_constraint_yyabort;
#if FDO_CONSTRAINT_YYDEBUG
        if (pParse->fdo_constraint_yydebug)
        {
            fdo_constraint_yys = 0;
            if (pParse->fdo_constraint_yychar <= FDO_CONSTRAINT_YYMAXTOKEN) fdo_constraint_yys = fdo_constraint_yyname[pParse->fdo_constraint_yychar];
            if (!fdo_constraint_yys) fdo_constraint_yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    FDO_CONSTRAINT_YYPREFIX, fdo_constraint_yystate, pParse->fdo_constraint_yychar, fdo_constraint_yys);
        }
#endif
        pParse->fdo_constraint_yychar = (-1);
        goto fdo_constraint_yyloop;
    }
fdo_constraint_yyreduce:
#if FDO_CONSTRAINT_YYDEBUG
    if (pParse->fdo_constraint_yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                FDO_CONSTRAINT_YYPREFIX, fdo_constraint_yystate, fdo_constraint_yyn, fdo_constraint_yyrule[fdo_constraint_yyn]);
#endif
    fdo_constraint_yym = fdo_constraint_yylen[fdo_constraint_yyn];
    pParse->fdo_constraint_yyval = pParse->fdo_constraint_yyvsp[1-fdo_constraint_yym];
    switch (fdo_constraint_yyn)
    {
case 1:
#line 116 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->SetRoot(Node_Copy(L"Constraint", pParse->fdo_constraint_yyvsp[0].m_node));	FDO_CONSTRAINT_YYACCEPT;}
break;
case 2:
#line 117 "Utilities/Common/Src/Parse/yyConstraint.y"
{Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	FDO_CONSTRAINT_YYABORT;}
break;
case 3:
#line 122 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;
					}
break;
case 4:
#line 125 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[0].m_node;}
break;
case 5:
#line 126 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[0].m_node;}
break;
case 6:
#line 128 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintList* list = FdoPropertyValueConstraintList::Create();
						FdoDataValueCollection* dvCol = list->GetConstraintList();
						FdoDataValue* dvTemp = NULL;
						int count = ((FdoDataValueCollection*)pParse->fdo_constraint_yyvsp[0].m_node)->GetCount();
						for (int i = 0; i < count; i++)
						{
							dvTemp = ((FdoDataValueCollection*)pParse->fdo_constraint_yyvsp[0].m_node)->GetItem(i);
							dvCol->Insert(i, dvTemp);
							FDO_SAFE_RELEASE(dvTemp);
						}
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint List",list);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node); 
						FDO_SAFE_RELEASE(dvCol);
					}
break;
case 7:
#line 144 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create();
						range->SetMinValue((FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMinInclusive(false);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node); 
					}
break;
case 8:
#line 152 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create();
						range->SetMaxValue((FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMaxInclusive(false);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node); 
					}
break;
case 9:
#line 160 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create();
						range->SetMinValue((FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMinInclusive(true);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node); 
					}
break;
case 10:
#line 168 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create();
						range->SetMaxValue((FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMaxInclusive(true);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node); 
					}
break;
case 11:
#line 176 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create((FdoDataValue*)pParse->fdo_constraint_yyvsp[-2].m_node, (FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMinInclusive(false);
						range->SetMaxInclusive(false);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[-2].m_node); 
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node);
					}
break;
case 12:
#line 185 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create((FdoDataValue*)pParse->fdo_constraint_yyvsp[-2].m_node, (FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMinInclusive(false);
						range->SetMaxInclusive(true);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[-2].m_node); 
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node);
					}
break;
case 13:
#line 194 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create((FdoDataValue*)pParse->fdo_constraint_yyvsp[-2].m_node, (FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMinInclusive(true);
						range->SetMaxInclusive(false);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[-2].m_node); 
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node);
					}
break;
case 14:
#line 203 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create((FdoDataValue*)pParse->fdo_constraint_yyvsp[-2].m_node, (FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMinInclusive(true);
						range->SetMaxInclusive(true);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[-2].m_node); 
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node);
					}
break;
case 15:
#line 216 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintRange* range = FdoPropertyValueConstraintRange::Create((FdoDataValue*)pParse->fdo_constraint_yyvsp[-2].m_node, (FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node);
						range->SetMinInclusive(true);
						range->SetMaxInclusive(true);
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint Range", range);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[-2].m_node); 
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node);
					}
break;
case 16:
#line 228 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						FdoPropertyValueConstraintList* list = FdoPropertyValueConstraintList::Create();
						FdoDataValueCollection* dvCol = list->GetConstraintList();
						FdoDataValue* dvTemp = NULL;
						int count = ((FdoDataValueCollection*)pParse->fdo_constraint_yyvsp[-1].m_node)->GetCount();
						for (int i = 0; i < count; i++)
						{
							dvTemp = ((FdoDataValueCollection*)pParse->fdo_constraint_yyvsp[-1].m_node)->GetItem(i);
							dvCol->Insert(i, dvTemp);
							FDO_SAFE_RELEASE(dvTemp);
						}
						pParse->fdo_constraint_yyval.m_node=Node_Add(L"Constraint List",list);
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[-1].m_node); 
						FDO_SAFE_RELEASE(dvCol);
					}
break;
case 17:
#line 246 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node=pParse->AddNodeToDelete(FdoDataValueCollection::Create());
					}
break;
case 18:
#line 250 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node=pParse->AddNodeToDelete(FdoDataValueCollection::Create()); 
						((FdoDataValueCollection*)pParse->fdo_constraint_yyval.m_node)->Add((FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node); 
						Node_Trace(L"DataValue Arg 1,");
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node); 
					}
break;
case 19:
#line 257 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;
					}
break;
case 20:
#line 261 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node=pParse->AddNodeToDelete(FdoDataValueCollection::Create()); 
						
						FdoDataValue* dvTemp = NULL;
						
						int count = ((FdoDataValueCollection*)pParse->fdo_constraint_yyvsp[-2].m_node)->GetCount();
						for (int i = 0; i < count; i++)
						{
							dvTemp = ((FdoDataValueCollection*)pParse->fdo_constraint_yyvsp[-2].m_node)->GetItem(i);
							((FdoDataValueCollection*)pParse->fdo_constraint_yyval.m_node)->Add(dvTemp); 
							FDO_SAFE_RELEASE(dvTemp);
						}
						
						count = ((FdoDataValueCollection*)pParse->fdo_constraint_yyvsp[0].m_node)->GetCount();
						for (int i = 0; i < count; i++)
						{
							dvTemp = ((FdoDataValueCollection*)pParse->fdo_constraint_yyvsp[0].m_node)->GetItem(i);
							((FdoDataValueCollection*)pParse->fdo_constraint_yyval.m_node)->Add(dvTemp); 
							FDO_SAFE_RELEASE(dvTemp);
						}				
		
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[-2].m_node); 
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node);
					}
break;
case 21:
#line 289 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[0].m_node;
					}
break;
case 22:
#line 293 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;
					}
break;
case 23:
#line 300 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[0].m_node;
					}
break;
case 24:
#line 304 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;
					}
break;
case 25:
#line 311 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[0].m_node;
					}
break;
case 26:
#line 315 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;
					}
break;
case 27:
#line 322 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[0].m_node;
					}
break;
case 28:
#line 326 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;
					}
break;
case 29:
#line 333 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[0].m_node;
					}
break;
case 30:
#line 337 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;
					}
break;
case 31:
#line 343 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node=pParse->AddNodeToDelete(FdoDataValueCollection::Create());
					}
break;
case 32:
#line 347 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						pParse->fdo_constraint_yyval.m_node=pParse->AddNodeToDelete(FdoDataValueCollection::Create()); 
						((FdoDataValueCollection*)pParse->fdo_constraint_yyval.m_node)->Add((FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node); 
						Node_Trace(L"DataValue Arg 1,");
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node); 
					}
break;
case 33:
#line 354 "Utilities/Common/Src/Parse/yyConstraint.y"
{
						((FdoDataValueCollection*)pParse->fdo_constraint_yyval.m_node)->Add((FdoDataValue*)pParse->fdo_constraint_yyvsp[0].m_node); 
						Node_Trace(L"DataValue Arg N,");
						FDO_SAFE_RELEASE(pParse->fdo_constraint_yyvsp[0].m_node); 
					}
break;
case 34:
#line 363 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;}
break;
case 35:
#line 365 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Copy(L"boolean", pParse->fdo_constraint_yyvsp[0].m_node);}
break;
case 36:
#line 367 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Copy(L"datetime", pParse->fdo_constraint_yyvsp[0].m_node);}
break;
case 37:
#line 370 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Copy(L"double", pParse->fdo_constraint_yyvsp[0].m_node);}
break;
case 38:
#line 372 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Copy(L"integer", pParse->fdo_constraint_yyvsp[0].m_node);}
break;
case 39:
#line 374 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Copy(L"string", pParse->fdo_constraint_yyvsp[0].m_node);}
break;
case 40:
#line 376 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Copy(L"TODO:BLOB", NULL);}
break;
case 41:
#line 378 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Copy(L"TODO:CLOB", NULL);}
break;
case 42:
#line 380 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
break;
case 43:
#line 384 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
break;
case 44:
#line 385 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
break;
case 45:
#line 388 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Add(L"STRING", FdoDataValue::Create(pParse->fdo_constraint_yyvsp[0].m_string));}
break;
case 46:
#line 391 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Add(L"INTEGER", FdoInt32Value::Create(pParse->fdo_constraint_yyvsp[0].m_integer));}
break;
case 47:
#line 394 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Add(L"DOUBLE", FdoDoubleValue::Create(pParse->fdo_constraint_yyvsp[0].m_double));}
break;
case 48:
#line 397 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node=Node_Add(L"DateTime", FdoDataValue::Create(pParse->fdo_constraint_yyvsp[0].m_datetime));}
break;
case 49:
#line 402 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(pParse->fdo_constraint_yyvsp[0].m_string));}
break;
case 50:
#line 404 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;}
break;
case 51:
#line 406 "Utilities/Common/Src/Parse/yyConstraint.y"
{pParse->fdo_constraint_yyval.m_node = pParse->fdo_constraint_yyvsp[-1].m_node;}
break;
#line 938 "yyConstraint.cpp"
    }
    pParse->fdo_constraint_yyssp -= fdo_constraint_yym;
    fdo_constraint_yystate = *pParse->fdo_constraint_yyssp;
    pParse->fdo_constraint_yyvsp -= fdo_constraint_yym;
    fdo_constraint_yym = fdo_constraint_yylhs[fdo_constraint_yyn];
    if (fdo_constraint_yystate == 0 && fdo_constraint_yym == 0)
    {
#if FDO_CONSTRAINT_YYDEBUG
        if (pParse->fdo_constraint_yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", FDO_CONSTRAINT_YYPREFIX, FDO_CONSTRAINT_YYFINAL);
#endif
        fdo_constraint_yystate = FDO_CONSTRAINT_YYFINAL;
        *++pParse->fdo_constraint_yyssp = FDO_CONSTRAINT_YYFINAL;
        *++pParse->fdo_constraint_yyvsp = pParse->fdo_constraint_yyval;
        if (pParse->fdo_constraint_yychar < 0)
        {
            if ((pParse->fdo_constraint_yychar = fdo_constraint_yylex(pParse)) < 0) pParse->fdo_constraint_yychar = 0;
#if FDO_CONSTRAINT_YYDEBUG
            if (pParse->fdo_constraint_yydebug)
            {
                fdo_constraint_yys = 0;
                if (pParse->fdo_constraint_yychar <= FDO_CONSTRAINT_YYMAXTOKEN) fdo_constraint_yys = fdo_constraint_yyname[pParse->fdo_constraint_yychar];
                if (!fdo_constraint_yys) fdo_constraint_yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        FDO_CONSTRAINT_YYPREFIX, FDO_CONSTRAINT_YYFINAL, pParse->fdo_constraint_yychar, fdo_constraint_yys);
            }
#endif
        }
        if (pParse->fdo_constraint_yychar == 0) goto fdo_constraint_yyaccept;
        goto fdo_constraint_yyloop;
    }
    if ((fdo_constraint_yyn = fdo_constraint_yygindex[fdo_constraint_yym]) && (fdo_constraint_yyn += fdo_constraint_yystate) >= 0 &&
            fdo_constraint_yyn <= FDO_CONSTRAINT_YYTABLESIZE && fdo_constraint_yycheck[fdo_constraint_yyn] == fdo_constraint_yystate)
        fdo_constraint_yystate = fdo_constraint_yytable[fdo_constraint_yyn];
    else
        fdo_constraint_yystate = fdo_constraint_yydgoto[fdo_constraint_yym];
#if FDO_CONSTRAINT_YYDEBUG
    if (pParse->fdo_constraint_yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", FDO_CONSTRAINT_YYPREFIX, *pParse->fdo_constraint_yyssp, fdo_constraint_yystate);
#endif
    if (pParse->fdo_constraint_yyssp >= pParse->fdo_constraint_yysslim && fdo_constraint_yygrowstack(pParse))
    {
        goto fdo_constraint_yyoverflow;
    }
    *++pParse->fdo_constraint_yyssp = fdo_constraint_yystate;
    *++pParse->fdo_constraint_yyvsp = pParse->fdo_constraint_yyval;
    goto fdo_constraint_yyloop;
fdo_constraint_yyoverflow:
   fdo_constraint_yyerror( pParse,"yacc stack overflow");
fdo_constraint_yyabort:
    return (1);
fdo_constraint_yyaccept:
    return (0);
}
