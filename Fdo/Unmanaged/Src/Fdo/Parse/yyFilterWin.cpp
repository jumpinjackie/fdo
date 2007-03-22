/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with fdo_filter_yy or FDO_FILTER_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define FDO_FILTER_YYBISON 1

/* Skeleton name.  */
#define FDO_FILTER_YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define FDO_FILTER_YYPURE 0

/* Using locations.  */
#define FDO_FILTER_YYLSP_NEEDED 0



/* Tokens.  */
#if 0 
# define FDO_FILTER_YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum fdo_filter_yytokentype {
     FdoToken_NULL = 258,
     FdoToken_TRUE = 259,
     FdoToken_FALSE = 260,
     FdoToken_DATE = 261,
     FdoToken_TIME = 262,
     FdoToken_TIMESTAMP = 263,
     FdoToken_GEOMFROMTEXT = 264,
     FdoToken_AND = 265,
     FdoToken_OR = 266,
     FdoToken_NOT = 267,
     FdoToken_LIKE = 268,
     FdoToken_IN = 269,
     FdoToken_BEYOND = 270,
     FdoToken_WITHINDISTANCE = 271,
     FdoToken_CONTAINS = 272,
     FdoToken_COVEREDBY = 273,
     FdoToken_CROSSES = 274,
     FdoToken_DISJOINT = 275,
     FdoToken_ENVELOPEINTERSECTS = 276,
     FdoToken_EQUALS = 277,
     FdoToken_INTERSECTS = 278,
     FdoToken_INSIDE = 279,
     FdoToken_OVERLAPS = 280,
     FdoToken_TOUCHES = 281,
     FdoToken_WITHIN = 282,
     FdoToken_RELATE = 283,
     FdoToken_IDENTIFIER = 284,
     FdoToken_PARAMETER = 285,
     FdoToken_STRING = 286,
     FdoToken_INTEGER = 287,
     FdoToken_INT64 = 288,
     FdoToken_DOUBLE = 289,
     FdoToken_DATETIME = 290,
     FdoToken_BLOB = 291,
     FdoToken_CLOB = 292,
     FdoToken_Add = 293,
     FdoToken_Subtract = 294,
     FdoToken_Multiply = 295,
     FdoToken_Divide = 296,
     FdoToken_Negate = 297,
     FdoToken_EQ = 298,
     FdoToken_NE = 299,
     FdoToken_GT = 300,
     FdoToken_GE = 301,
     FdoToken_LT = 302,
     FdoToken_LE = 303,
     FdoToken_LeftParenthesis = 304,
     FdoToken_RightParenthesis = 305,
     FdoToken_Comma = 306,
     FdoToken_Dot = 307,
     FdoToken_Colon = 308,
     FdoToken_AS = 309,
     FdoToken_BETWEEN = 310
   };
#endif
#define FdoToken_NULL 258
#define FdoToken_TRUE 259
#define FdoToken_FALSE 260
#define FdoToken_DATE 261
#define FdoToken_TIME 262
#define FdoToken_TIMESTAMP 263
#define FdoToken_GEOMFROMTEXT 264
#define FdoToken_AND 265
#define FdoToken_OR 266
#define FdoToken_NOT 267
#define FdoToken_LIKE 268
#define FdoToken_IN 269
#define FdoToken_BEYOND 270
#define FdoToken_WITHINDISTANCE 271
#define FdoToken_CONTAINS 272
#define FdoToken_COVEREDBY 273
#define FdoToken_CROSSES 274
#define FdoToken_DISJOINT 275
#define FdoToken_ENVELOPEINTERSECTS 276
#define FdoToken_EQUALS 277
#define FdoToken_INTERSECTS 278
#define FdoToken_INSIDE 279
#define FdoToken_OVERLAPS 280
#define FdoToken_TOUCHES 281
#define FdoToken_WITHIN 282
#define FdoToken_RELATE 283
#define FdoToken_IDENTIFIER 284
#define FdoToken_PARAMETER 285
#define FdoToken_STRING 286
#define FdoToken_INTEGER 287
#define FdoToken_INT64 288
#define FdoToken_DOUBLE 289
#define FdoToken_DATETIME 290
#define FdoToken_BLOB 291
#define FdoToken_CLOB 292
#define FdoToken_Add 293
#define FdoToken_Subtract 294
#define FdoToken_Multiply 295
#define FdoToken_Divide 296
#define FdoToken_Negate 297
#define FdoToken_EQ 298
#define FdoToken_NE 299
#define FdoToken_GT 300
#define FdoToken_GE 301
#define FdoToken_LT 302
#define FdoToken_LE 303
#define FdoToken_LeftParenthesis 304
#define FdoToken_RightParenthesis 305
#define FdoToken_Comma 306
#define FdoToken_Dot 307
#define FdoToken_Colon 308
#define FdoToken_AS 309
#define FdoToken_BETWEEN 310




/* Copy the first part of user declarations.  */


#include "Parse.h"
#pragma warning(disable:4102)	// unreferenced labels in fdo_filter_yyFilter.cpp


/* Enabling traces.  */
#ifndef FDO_FILTER_YYDEBUG
# define FDO_FILTER_YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef FDO_FILTER_YYERROR_VERBOSE
# undef FDO_FILTER_YYERROR_VERBOSE
# define FDO_FILTER_YYERROR_VERBOSE 1
#else
# define FDO_FILTER_YYERROR_VERBOSE 0
#endif

#if ! defined (FDO_FILTER_YYSTYPE) && ! defined (FDO_FILTER_YYSTYPE_IS_DECLARED)

typedef union FDO_FILTER_YYSTYPE _FDO_FILTER_YYSTYPE
{
	FdoIDisposable*	m_node;		// expression or filter parse tree node
	FdoInt32		m_id;		// enumerations, keywords, ...
	FdoInt32		m_integer;	// integer values (Int8, Int16, Int32 )
	FdoInt64		m_int64;	// 64-bit integer values
	double			m_double;	// floating point values (single(float), double)
	FdoString*		m_string;	// string
	_FdoDateTime	m_datetime;	// date time
 
/* Line 191 of yacc.c.  */

# define fdo_filter_yystype FDO_FILTER_YYSTYPE /* obsolescent; will be withdrawn */
# define FDO_FILTER_YYSTYPE_IS_DECLARED 1
# define FDO_FILTER_YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */


#if ! defined (fdo_filter_yyoverflow) || FDO_FILTER_YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if FDO_FILTER_YYSTACK_USE_ALLOCA
#  define FDO_FILTER_YYSTACK_ALLOC alloca
# else
#  ifndef FDO_FILTER_YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define FDO_FILTER_YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define FDO_FILTER_YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef FDO_FILTER_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define FDO_FILTER_YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define FDO_FILTER_YYSIZE_T size_t
#  endif
#  define FDO_FILTER_YYSTACK_ALLOC malloc
#  define FDO_FILTER_YYSTACK_FREE free
# endif
#endif /* ! defined (fdo_filter_yyoverflow) || FDO_FILTER_YYERROR_VERBOSE */


#if (! defined (fdo_filter_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (FDO_FILTER_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union fdo_filter_yyalloc
{
  short fdo_filter_yyss;
  FDO_FILTER_YYSTYPE fdo_filter_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define FDO_FILTER_YYSTACK_GAP_MAXIMUM (sizeof (union fdo_filter_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define FDO_FILTER_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (FDO_FILTER_YYSTYPE))				\
      + FDO_FILTER_YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef FDO_FILTER_YYCOPY
#  if 1 < __GNUC__
#   define FDO_FILTER_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define FDO_FILTER_YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register FDO_FILTER_YYSIZE_T fdo_filter_yyi;		\
	  for (fdo_filter_yyi = 0; fdo_filter_yyi < (Count); fdo_filter_yyi++)	\
	    (To)[fdo_filter_yyi] = (From)[fdo_filter_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables FDO_FILTER_YYSIZE and FDO_FILTER_YYSTACKSIZE give the old and new number of
   elements in the stack, and FDO_FILTER_YYPTR gives the new location of the
   stack.  Advance FDO_FILTER_YYPTR to a properly aligned location for the next
   stack.  */
# define FDO_FILTER_YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	FDO_FILTER_YYSIZE_T fdo_filter_yynewbytes;						\
	FDO_FILTER_YYCOPY (&fdo_filter_yyptr->Stack, Stack, fdo_filter_yysize);				\
	Stack = &fdo_filter_yyptr->Stack;						\
	fdo_filter_yynewbytes = fdo_filter_yystacksize * sizeof (*Stack) + FDO_FILTER_YYSTACK_GAP_MAXIMUM; \
	fdo_filter_yyptr += fdo_filter_yynewbytes / sizeof (*fdo_filter_yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char fdo_filter_yysigned_char;
#else
   typedef short fdo_filter_yysigned_char;
#endif

/* FDO_FILTER_YYFINAL -- State number of the termination state. */
#define FDO_FILTER_YYFINAL  59
/* FDO_FILTER_YYLAST -- Last index in FDO_FILTER_YYTABLE.  */
#define FDO_FILTER_YYLAST   298

/* FDO_FILTER_YYNTOKENS -- Number of terminals. */
#define FDO_FILTER_YYNTOKENS  59
/* FDO_FILTER_YYNNTS -- Number of nonterminals. */
#define FDO_FILTER_YYNNTS  37
/* FDO_FILTER_YYNRULES -- Number of rules. */
#define FDO_FILTER_YYNRULES  91
/* FDO_FILTER_YYNRULES -- Number of states. */
#define FDO_FILTER_YYNSTATES  130

/* FDO_FILTER_YYTRANSLATE(FDO_FILTER_YYLEX) -- Bison symbol number corresponding to FDO_FILTER_YYLEX.  */
#define FDO_FILTER_YYUNDEFTOK  2
#define FDO_FILTER_YYMAXUTOK   310

#define FDO_FILTER_YYTRANSLATE(FDO_FILTER_YYX) 						\
  ((unsigned int) (FDO_FILTER_YYX) <= FDO_FILTER_YYMAXUTOK ? fdo_filter_yytranslate[FDO_FILTER_YYX] : FDO_FILTER_YYUNDEFTOK)

/* FDO_FILTER_YYTRANSLATE[FDO_FILTER_YYLEX] -- Bison symbol number corresponding to FDO_FILTER_YYLEX.  */
static const unsigned char fdo_filter_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      57,    58,     2,     2,    56,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if FDO_FILTER_YYDEBUG
/* FDO_FILTER_YYPRHS[FDO_FILTER_YYN] -- Index of the first RHS symbol of rule number FDO_FILTER_YYN in
   FDO_FILTER_YYRHS.  */
static const unsigned char fdo_filter_yyprhs[] =
{
       0,     0,     3,     5,     9,    11,    15,    17,    19,    21,
      23,    25,    29,    33,    37,    41,    43,    45,    47,    49,
      51,    53,    55,    57,    59,    61,    63,    65,    67,    69,
      71,    73,    78,    79,    81,    85,    90,    92,    94,    98,
     104,   108,   110,   112,   114,   116,   118,   121,   124,   128,
     130,   132,   134,   136,   138,   140,   142,   146,   152,   155,
     157,   159,   161,   163,   167,   170,   174,   178,   182,   187,
     189,   191,   193,   195,   197,   199,   201,   203,   205,   207,
     209,   211,   213,   215,   217,   219,   221,   223,   225,   227,
     229,   231
};

/* FDO_FILTER_YYRHS -- A `-1'-separated list of the rules' RHS. */
static const fdo_filter_yysigned_char fdo_filter_yyrhs[] =
{
      60,     0,    -1,    80,    -1,    74,    56,    80,    -1,     1,
      -1,    57,    61,    58,    -1,    79,    -1,    62,    -1,    70,
      -1,    73,    -1,    76,    -1,    61,    38,    61,    -1,    61,
      39,    61,    -1,    61,    40,    61,    -1,    61,    41,    61,
      -1,    64,    -1,    69,    -1,    68,    -1,    66,    -1,    67,
      -1,    65,    -1,    36,    -1,    37,    -1,     3,    -1,     4,
      -1,     5,    -1,    31,    -1,    32,    -1,    33,    -1,    34,
      -1,    35,    -1,    73,    57,    71,    58,    -1,    -1,    61,
      -1,    71,    56,    61,    -1,     9,    57,    31,    58,    -1,
      29,    -1,    75,    -1,    74,    56,    75,    -1,    57,    61,
      58,    54,    73,    -1,    61,    54,    73,    -1,    77,    -1,
      78,    -1,    72,    -1,    63,    -1,    30,    -1,    53,    31,
      -1,    42,    61,    -1,    57,    80,    58,    -1,    81,    -1,
      86,    -1,    89,    -1,    88,    -1,    91,    -1,    87,    -1,
      76,    -1,    83,    56,    76,    -1,    73,    14,    57,    83,
      58,    -1,    73,     3,    -1,    84,    -1,    90,    -1,    82,
      -1,    85,    -1,    73,    94,    61,    -1,    12,    80,    -1,
      80,    10,    80,    -1,    80,    11,    80,    -1,    61,    92,
      61,    -1,    73,    93,    61,    95,    -1,    43,    -1,    44,
      -1,    45,    -1,    46,    -1,    47,    -1,    48,    -1,    13,
      -1,    15,    -1,    16,    -1,    17,    -1,    19,    -1,    20,
      -1,    22,    -1,    24,    -1,    23,    -1,    25,    -1,    26,
      -1,    27,    -1,    18,    -1,    21,    -1,    68,    -1,    32,
      -1,    33,    -1
};

/* FDO_FILTER_YYRLINE[FDO_FILTER_YYN] -- source line where rule number FDO_FILTER_YYN was defined.  */
static const unsigned short fdo_filter_yyrline[] =
{
       0,   122,   122,   123,   124,   130,   131,   132,   133,   134,
     135,   139,   140,   141,   142,   146,   149,   151,   153,   155,
     157,   159,   161,   163,   168,   169,   172,   175,   178,   181,
     184,   188,   192,   193,   194,   198,   203,   214,   215,   219,
     225,   234,   235,   239,   240,   244,   246,   251,   256,   257,
     258,   262,   263,   267,   268,   272,   273,   278,   283,   287,
     288,   289,   290,   294,   299,   303,   304,   308,   312,   317,
     318,   319,   320,   321,   322,   323,   327,   328,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   346,
     347,   348
};
#endif

#if FDO_FILTER_YYDEBUG || FDO_FILTER_YYERROR_VERBOSE
/* FDO_FILTER_YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at FDO_FILTER_YYNTOKENS, nonterminals. */
static const char *const fdo_filter_yytname[] =
{
  "$end", "error", "$undefined", "FdoToken_NULL", "FdoToken_TRUE", 
  "FdoToken_FALSE", "FdoToken_DATE", "FdoToken_TIME", 
  "FdoToken_TIMESTAMP", "FdoToken_GEOMFROMTEXT", "FdoToken_AND", 
  "FdoToken_OR", "FdoToken_NOT", "FdoToken_LIKE", "FdoToken_IN", 
  "FdoToken_BEYOND", "FdoToken_WITHINDISTANCE", "FdoToken_CONTAINS", 
  "FdoToken_COVEREDBY", "FdoToken_CROSSES", "FdoToken_DISJOINT", 
  "FdoToken_ENVELOPEINTERSECTS", "FdoToken_EQUALS", "FdoToken_INTERSECTS", 
  "FdoToken_INSIDE", "FdoToken_OVERLAPS", "FdoToken_TOUCHES", 
  "FdoToken_WITHIN", "FdoToken_RELATE", "FdoToken_IDENTIFIER", 
  "FdoToken_PARAMETER", "FdoToken_STRING", "FdoToken_INTEGER", 
  "FdoToken_INT64", "FdoToken_DOUBLE", "FdoToken_DATETIME", 
  "FdoToken_BLOB", "FdoToken_CLOB", "FdoToken_Add", "FdoToken_Subtract", 
  "FdoToken_Multiply", "FdoToken_Divide", "FdoToken_Negate", 
  "FdoToken_EQ", "FdoToken_NE", "FdoToken_GT", "FdoToken_GE", 
  "FdoToken_LT", "FdoToken_LE", "FdoToken_LeftParenthesis", 
  "FdoToken_RightParenthesis", "FdoToken_Comma", "FdoToken_Dot", 
  "FdoToken_Colon", "FdoToken_AS", "FdoToken_BETWEEN", "','", "'('", 
  "')'", "$accept", "fdo", "Expression", "BinaryExpression", "DataValue", 
  "boolean", "string", "integer", "int64", "double", "datetime", 
  "Function", "ExpressionCollection", "GeometryValue", "Identifier", 
  "ComputedIdentifier", "ComputedIdentifier2", "ValueExpression", 
  "LiteralValue", "Parameter", "UnaryExpression", "Filter", 
  "LogicalOperator", "GeometricCondition", "ValueExpressionCollection", 
  "InCondition", "NullCondition", "SearchCondition", "SpatialCondition", 
  "UnaryLogicalOperator", "BinaryLogicalOperator", "ComparisonCondition", 
  "DistanceCondition", "ComparisonOperations", "DistanceOperations", 
  "SpatialOperations", "distance", 0
};
#endif

# ifdef FDO_FILTER_YYPRINT
/* FDO_FILTER_YYTOKNUM[FDO_FILTER_YYLEX-NUM] -- Internal token number corresponding to
   token FDO_FILTER_YYLEX-NUM.  */
static const unsigned short fdo_filter_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,    44,    40,    41
};
# endif

/* FDO_FILTER_YYR1[FDO_FILTER_YYN] -- Symbol number of symbol that rule FDO_FILTER_YYN derives.  */
static const unsigned char fdo_filter_yyr1[] =
{
       0,    59,    60,    60,    60,    61,    61,    61,    61,    61,
      61,    62,    62,    62,    62,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    64,    64,    65,    66,    67,    68,
      69,    70,    71,    71,    71,    72,    73,    74,    74,    75,
      75,    76,    76,    77,    77,    78,    78,    79,    80,    80,
      80,    81,    81,    82,    82,    83,    83,    84,    85,    86,
      86,    86,    86,    87,    88,    89,    89,    90,    91,    92,
      92,    92,    92,    92,    92,    92,    93,    93,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    95,
      95,    95
};

/* FDO_FILTER_YYR2[FDO_FILTER_YYN] -- Number of symbols composing right hand side of rule FDO_FILTER_YYN.  */
static const unsigned char fdo_filter_yyr2[] =
{
       0,     2,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     0,     1,     3,     4,     1,     1,     3,     5,
       3,     1,     1,     1,     1,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     2,     1,
       1,     1,     1,     3,     2,     3,     3,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
};

/* FDO_FILTER_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when FDO_FILTER_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char fdo_filter_yydefact[] =
{
       0,     4,    23,    24,    25,     0,     0,    36,    45,    26,
      27,    28,    29,    30,    21,    22,     0,     0,     0,     0,
       0,     7,    44,    15,    20,    18,    19,    17,    16,     8,
      43,     9,     0,    37,    10,    41,    42,     6,     2,    49,
      61,    59,    62,    50,    54,    52,    51,    60,    53,     0,
       0,     0,    64,     0,    47,     9,    46,     0,     0,     1,
      75,     0,     0,     0,     0,    69,    70,    71,    72,    73,
      74,     0,     0,    58,     0,    76,    77,    78,    87,    79,
      80,    88,    81,    83,    82,    84,    85,    86,    32,     0,
       0,     0,     0,     0,     0,     0,     0,     5,    48,    11,
      12,    13,    14,    40,    67,     0,    33,     0,     0,    63,
      38,     3,    65,    66,    35,     5,     0,    55,     0,     0,
      31,    90,    91,    89,    68,    39,     0,    57,    34,    56
};

/* FDO_FILTER_YYDEFGOTO[NTERM-NUM]. */
static const fdo_filter_yysigned_char fdo_filter_yydefgoto[] =
{
      -1,    19,    51,    21,    22,    23,    24,    25,    26,    27,
      28,    29,   107,    30,    55,    32,    33,    34,    35,    36,
      37,    58,    39,    40,   118,    41,    42,    43,    44,    45,
      46,    47,    48,    72,    89,    90,   124
};

/* FDO_FILTER_YYPACT[STATE-NUM] -- Index in FDO_FILTER_YYTABLE of the portion describing
   STATE-NUM.  */
#define FDO_FILTER_YYPACT_NINF -105
static const short fdo_filter_yypact[] =
{
      73,  -105,  -105,  -105,  -105,   -51,   108,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,   178,   -21,   108,    19,
     238,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,     9,   -39,  -105,  -105,  -105,  -105,  -105,    27,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,   -10,
     108,   250,  -105,   178,  -105,   -14,  -105,     1,    -7,  -105,
    -105,   178,   178,   178,   178,  -105,  -105,  -105,  -105,  -105,
    -105,    25,   178,  -105,     3,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,   178,   178,
     178,   143,   108,   108,     7,   214,    17,    30,  -105,    39,
      39,  -105,  -105,  -105,    29,   213,    29,   -49,    95,    29,
    -105,    27,  -105,    61,  -105,  -105,    25,  -105,   -43,   178,
    -105,  -105,  -105,  -105,  -105,  -105,   213,  -105,    29,  -105
};

/* FDO_FILTER_YYPGOTO[NTERM-NUM].  */
static const fdo_filter_yysigned_char fdo_filter_yypgoto[] =
{
    -105,  -105,     0,  -105,  -105,  -105,  -105,  -105,  -105,   -27,
    -105,  -105,  -105,  -105,     2,  -105,    -8,  -104,  -105,  -105,
    -105,     5,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105
};

/* FDO_FILTER_YYTABLE[FDO_FILTER_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what FDO_FILTER_YYDEFACT says.
   If FDO_FILTER_YYTABLE_NINF, syntax error.  */
#define FDO_FILTER_YYTABLE_NINF -1
static const unsigned char fdo_filter_yytable[] =
{
      20,   117,    31,    92,    93,    38,    49,   119,    31,   120,
      56,    52,    73,   126,    60,   127,    54,    91,    57,    59,
      31,    94,   129,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    92,    93,    61,
      62,    63,    64,    88,    65,    66,    67,    68,    69,    70,
      95,    98,    31,    96,     7,    61,    62,    63,    64,    97,
     105,    99,   100,   101,   102,   114,    88,    61,    62,    63,
      64,    92,   104,   103,     1,   115,     2,     3,     4,    63,
      64,   123,     5,   110,   116,     6,     0,     0,   106,   108,
     109,    20,     0,    31,    31,    31,   111,   112,   113,     0,
       0,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,     2,     3,     4,     0,    16,     0,     5,   125,   128,
       6,     0,     0,     0,     0,     0,    17,   121,   122,    12,
      18,     0,     0,    61,    62,    63,    64,     7,     8,     9,
      10,    11,    12,    13,    14,    15,     2,     3,     4,     0,
      16,     0,     5,     0,     0,     6,     0,     0,     0,     0,
       0,    17,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     7,     8,     9,    10,    11,    12,    13,    14,
      15,     2,     3,     4,     0,    16,     0,     5,     0,     0,
       0,     0,     0,     0,     0,     0,    17,     0,     0,     0,
      18,     0,     0,     0,     0,     0,     0,     7,     8,     9,
      10,    11,    12,    13,    14,    15,     2,     3,     4,     0,
      16,     0,     5,     0,     0,     0,     0,    60,     0,     0,
       0,    17,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    60,    61,    62,    63,    64,     0,    65,    66,    67,
      68,    69,    70,    60,     0,     0,    17,     0,     0,     0,
       0,     0,   115,     0,     0,     0,    61,    62,    63,    64,
       0,    65,    66,    67,    68,    69,    70,     0,    61,    62,
      63,    64,    71,    65,    66,    67,    68,    69,    70
};

static const fdo_filter_yysigned_char fdo_filter_yycheck[] =
{
       0,   105,     0,    10,    11,     0,    57,    56,     6,    58,
      31,     6,     3,    56,    13,    58,    16,    56,    18,     0,
      18,    31,   126,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    10,    11,    38,
      39,    40,    41,    57,    43,    44,    45,    46,    47,    48,
      50,    58,    50,    53,    29,    38,    39,    40,    41,    58,
      57,    61,    62,    63,    64,    58,    57,    38,    39,    40,
      41,    10,    72,    71,     1,    58,     3,     4,     5,    40,
      41,   108,     9,    91,    54,    12,    -1,    -1,    88,    89,
      90,    91,    -1,    91,    92,    93,    91,    92,    93,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     3,     4,     5,    -1,    42,    -1,     9,   116,   119,
      12,    -1,    -1,    -1,    -1,    -1,    53,    32,    33,    34,
      57,    -1,    -1,    38,    39,    40,    41,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     3,     4,     5,    -1,
      42,    -1,     9,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     3,     4,     5,    -1,    42,    -1,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     3,     4,     5,    -1,
      42,    -1,     9,    -1,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      37,    13,    38,    39,    40,    41,    -1,    43,    44,    45,
      46,    47,    48,    13,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    38,    39,    40,    41,
      -1,    43,    44,    45,    46,    47,    48,    -1,    38,    39,
      40,    41,    54,    43,    44,    45,    46,    47,    48
};

/* FDO_FILTER_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char fdo_filter_yystos[] =
{
       0,     1,     3,     4,     5,     9,    12,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    42,    53,    57,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    84,    85,    86,    87,    88,    89,    90,    91,    57,
      57,    61,    80,    57,    61,    73,    31,    61,    80,     0,
      13,    38,    39,    40,    41,    43,    44,    45,    46,    47,
      48,    54,    92,     3,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    57,    93,
      94,    56,    10,    11,    31,    61,    61,    58,    58,    61,
      61,    61,    61,    73,    61,    57,    61,    71,    61,    61,
      75,    80,    80,    80,    58,    58,    54,    76,    83,    56,
      58,    32,    33,    68,    95,    73,    56,    58,    61,    76
};

#if ! defined (FDO_FILTER_YYSIZE_T) && defined (__SIZE_TYPE__)
# define FDO_FILTER_YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (FDO_FILTER_YYSIZE_T) && defined (size_t)
# define FDO_FILTER_YYSIZE_T size_t
#endif
#if ! defined (FDO_FILTER_YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define FDO_FILTER_YYSIZE_T size_t
# endif
#endif
#if ! defined (FDO_FILTER_YYSIZE_T)
# define FDO_FILTER_YYSIZE_T unsigned int
#endif

#define fdo_filter_yyerrok		(fdo_filter_yyerrstatus = 0)
#define fdo_filter_yyclearin	(pParse->fdo_filter_yychar = FDO_FILTER_YYEMPTY)
#define FDO_FILTER_YYEMPTY		(-2)
#define FDO_FILTER_YYEOF		0

#define FDO_FILTER_YYACCEPT	goto fdo_filter_yyacceptlab
#define FDO_FILTER_YYABORT		goto fdo_filter_yyabortlab
#define FDO_FILTER_YYERROR		goto fdo_filter_yyerrlab1

/* Like FDO_FILTER_YYERROR except do call fdo_filter_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of FDO_FILTER_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define FDO_FILTER_YYFAIL		goto fdo_filter_yyerrlab

#define FDO_FILTER_YYRECOVERING()  (!!fdo_filter_yyerrstatus)

#define FDO_FILTER_YYBACKUP(Token, Value)					\
do								\
  if (pParse->fdo_filter_yychar == FDO_FILTER_YYEMPTY && fdo_filter_yylen == 1)				\
    {								\
      pParse->fdo_filter_yychar = (Token);						\
      pParse->fdo_filter_yylval = (Value);						\
      fdo_filter_yytoken = FDO_FILTER_YYTRANSLATE (pParse->fdo_filter_yychar);				\
      FDO_FILTER_YYPOPSTACK;						\
      goto fdo_filter_yybackup;						\
    }								\
  else								\
    { 								\
      fdo_filter_yyerror(pParse,"syntax error: cannot back up");\
      FDO_FILTER_YYERROR;							\
    }								\
while (0)

#define FDO_FILTER_YYTERROR	1
#define FDO_FILTER_YYERRCODE	256

/* FDO_FILTER_YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef FDO_FILTER_YYLLOC_DEFAULT
# define FDO_FILTER_YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* FDO_FILTER_YYLEX -- calling `fdo_filter_yylex' with the right arguments.  */

#ifdef FDO_FILTER_YYLEX_PARAM
# define FDO_FILTER_YYLEX fdo_filter_yylex (FDO_FILTER_YYLEX_PARAM)
#else
# define FDO_FILTER_YYLEX fdo_filter_yylex(pParse)
#endif

/* Enable debugging if requested.  */
#if FDO_FILTER_YYDEBUG

# ifndef FDO_FILTER_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define FDO_FILTER_YYFPRINTF fprintf
# endif

# define FDO_FILTER_YYDPRINTF(Args)			\
do {						\
  if (pParse->fdo_filter_yydebug)					\
    FDO_FILTER_YYFPRINTF Args;				\
} while (0)

# define FDO_FILTER_YYDSYMPRINT(Args)			\
do {						\
  if (pParse->fdo_filter_yydebug)					\
    fdo_filter_yysymprint Args;				\
} while (0)

# define FDO_FILTER_YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (pParse->fdo_filter_yydebug)							\
    {								\
      FDO_FILTER_YYFPRINTF (stderr, "%s ", Title);				\
      fdo_filter_yysymprint (stderr, 					\
                  Token, Value);	\
      FDO_FILTER_YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| fdo_filter_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_filter_yy_stack_print (short *bottom, short *top)
#else
static void
fdo_filter_yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  FDO_FILTER_YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    FDO_FILTER_YYFPRINTF (stderr, " %d", *bottom);
  FDO_FILTER_YYFPRINTF (stderr, "\n");
}

# define FDO_FILTER_YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (pParse->fdo_filter_yydebug)							\
    fdo_filter_yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the FDO_FILTER_YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_filter_yy_reduce_print (int fdo_filter_yyrule)
#else
static void
fdo_filter_yy_reduce_print (fdo_filter_yyrule)
    int fdo_filter_yyrule;
#endif
{
  int fdo_filter_yyi;
  unsigned int pParse->fdo_filter_yylineno = fdo_filter_yyrline[fdo_filter_yyrule];
  FDO_FILTER_YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             fdo_filter_yyrule - 1, pParse->fdo_filter_yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (fdo_filter_yyi = fdo_filter_yyprhs[fdo_filter_yyrule]; 0 <= fdo_filter_yyrhs[fdo_filter_yyi]; fdo_filter_yyi++)
    FDO_FILTER_YYFPRINTF (stderr, "%s ", fdo_filter_yytname [fdo_filter_yyrhs[fdo_filter_yyi]]);
  FDO_FILTER_YYFPRINTF (stderr, "-> %s\n", fdo_filter_yytname [fdo_filter_yyr1[fdo_filter_yyrule]]);
}

# define FDO_FILTER_YY_REDUCE_PRINT(Rule)		\
do {					\
  if (pParse->fdo_filter_yydebug)				\
    fdo_filter_yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#else /* !FDO_FILTER_YYDEBUG */
# define FDO_FILTER_YYDPRINTF(Args)
# define FDO_FILTER_YYDSYMPRINT(Args)
# define FDO_FILTER_YYDSYMPRINTF(Title, Token, Value, Location)
# define FDO_FILTER_YY_STACK_PRINT(Bottom, Top)
# define FDO_FILTER_YY_REDUCE_PRINT(Rule)
#endif /* !FDO_FILTER_YYDEBUG */


/* FDO_FILTER_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	FDO_FILTER_YYINITDEPTH
# define FDO_FILTER_YYINITDEPTH 200
#endif

/* FDO_FILTER_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < FDO_FILTER_YYSTACK_BYTES (FDO_FILTER_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if FDO_FILTER_YYMAXDEPTH == 0
# undef FDO_FILTER_YYMAXDEPTH
#endif

#ifndef FDO_FILTER_YYMAXDEPTH
# define FDO_FILTER_YYMAXDEPTH 10000
#endif



#if FDO_FILTER_YYERROR_VERBOSE

# ifndef fdo_filter_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define fdo_filter_yystrlen strlen
#  else
/* Return the length of FDO_FILTER_YYSTR.  */
static FDO_FILTER_YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
fdo_filter_yystrlen (const char *fdo_filter_yystr)
#   else
fdo_filter_yystrlen (fdo_filter_yystr)
     const char *fdo_filter_yystr;
#   endif
{
  register const char *fdo_filter_yys = fdo_filter_yystr;

  while (*fdo_filter_yys++ != '\0')
    continue;

  return fdo_filter_yys - fdo_filter_yystr - 1;
}
#  endif
# endif

# ifndef fdo_filter_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define fdo_filter_yystpcpy stpcpy
#  else
/* Copy FDO_FILTER_YYSRC to FDO_FILTER_YYDEST, returning the address of the terminating '\0' in
   FDO_FILTER_YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
fdo_filter_yystpcpy (char *fdo_filter_yydest, const char *fdo_filter_yysrc)
#   else
fdo_filter_yystpcpy (fdo_filter_yydest, fdo_filter_yysrc)
     char *fdo_filter_yydest;
     const char *fdo_filter_yysrc;
#   endif
{
  register char *fdo_filter_yyd = fdo_filter_yydest;
  register const char *fdo_filter_yys = fdo_filter_yysrc;

  while ((*fdo_filter_yyd++ = *fdo_filter_yys++) != '\0')
    continue;

  return fdo_filter_yyd - 1;
}
#  endif
# endif

#endif /* !FDO_FILTER_YYERROR_VERBOSE */



#if FDO_FILTER_YYDEBUG
/*--------------------------------.
| Print this symbol on FDO_FILTER_YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_filter_yysymprint (FILE *fdo_filter_yyoutput, int fdo_filter_yytype, FDO_FILTER_YYSTYPE *fdo_filter_yyvaluep)
#else
static void
fdo_filter_yysymprint (fdo_filter_yyoutput, fdo_filter_yytype, fdo_filter_yyvaluep)
    FILE *fdo_filter_yyoutput;
    int fdo_filter_yytype;
    FDO_FILTER_YYSTYPE *fdo_filter_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) fdo_filter_yyvaluep;

  if (fdo_filter_yytype < FDO_FILTER_YYNTOKENS)
    {
      FDO_FILTER_YYFPRINTF (fdo_filter_yyoutput, "token %s (", fdo_filter_yytname[fdo_filter_yytype]);
# ifdef FDO_FILTER_YYPRINT
      FDO_FILTER_YYPRINT (fdo_filter_yyoutput, fdo_filter_yytoknum[fdo_filter_yytype], *fdo_filter_yyvaluep);
# endif
    }
  else
    FDO_FILTER_YYFPRINTF (fdo_filter_yyoutput, "nterm %s (", fdo_filter_yytname[fdo_filter_yytype]);

  switch (fdo_filter_yytype)
    {
      default:
        break;
    }
  FDO_FILTER_YYFPRINTF (fdo_filter_yyoutput, ")");
}

#endif /* ! FDO_FILTER_YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_filter_yydestruct (int fdo_filter_yytype, FDO_FILTER_YYSTYPE *fdo_filter_yyvaluep)
#else
static void
fdo_filter_yydestruct (fdo_filter_yytype, fdo_filter_yyvaluep)
    int fdo_filter_yytype;
    FDO_FILTER_YYSTYPE *fdo_filter_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) fdo_filter_yyvaluep;

  switch (fdo_filter_yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef FDO_FILTER_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int fdo_filter_yyparse (void *FDO_FILTER_YYPARSE_PARAM);
# else
int fdo_filter_yyparse(FdoParse* pParse);
# endif
#else /* ! FDO_FILTER_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int fdo_filter_yyparse(FdoParse* pParse);
#else
int fdo_filter_yyparse(FdoParse* pParse);
#endif
#endif /* ! FDO_FILTER_YYPARSE_PARAM */



/* The lookahead symbol.  */

/* The semantic value of the lookahead symbol.  */

/* Number of syntax errors so far.  */



/*----------.
| fdo_filter_yyparse.  |
`----------*/

#ifdef FDO_FILTER_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int fdo_filter_yyparse (void *FDO_FILTER_YYPARSE_PARAM)
# else
int fdo_filter_yyparse (FDO_FILTER_YYPARSE_PARAM)
  void *FDO_FILTER_YYPARSE_PARAM;
# endif
#else /* ! FDO_FILTER_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
fdo_filter_yyparse(FdoParse* pParse)
#else
int
fdo_filter_yyparse(FdoParse* pParse)

#endif
#endif
{
  
  register int fdo_filter_yystate;
  register int fdo_filter_yyn;
  int fdo_filter_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int fdo_filter_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int fdo_filter_yytoken = 0;

  /* Three stacks and their tools:
     `fdo_filter_yyss': related to states,
     `fdo_filter_yyvs': related to semantic values,
     `fdo_filter_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow fdo_filter_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short *fdo_filter_yyss = pParse->fdo_filter_yyss;

  /* The semantic value stack.  */
  FDO_FILTER_YYSTYPE *fdo_filter_yyvs = pParse->fdo_filter_yyvs;



#define FDO_FILTER_YYPOPSTACK   (pParse->fdo_filter_yyvsp--, pParse->fdo_filter_yyssp--)

  FDO_FILTER_YYSIZE_T fdo_filter_yystacksize = FDO_FILTER_YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int fdo_filter_yylen;

  FDO_FILTER_YYDPRINTF ((stderr, "Starting parse\n"));

  fdo_filter_yystate = 0;
  fdo_filter_yyerrstatus = 0;
  pParse->fdo_filter_yynerrs = 0;
  pParse->fdo_filter_yychar = FDO_FILTER_YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  pParse->fdo_filter_yyssp = fdo_filter_yyss;
  pParse->fdo_filter_yyvsp = fdo_filter_yyvs;

  goto fdo_filter_yysetstate;

/*------------------------------------------------------------.
| fdo_filter_yynewstate -- Push a new state, which is found in fdo_filter_yystate.  |
`------------------------------------------------------------*/
 fdo_filter_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  pParse->fdo_filter_yyssp++;

 fdo_filter_yysetstate:
  *pParse->fdo_filter_yyssp = fdo_filter_yystate;

  if (fdo_filter_yyss + fdo_filter_yystacksize - 1 <= pParse->fdo_filter_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      FDO_FILTER_YYSIZE_T fdo_filter_yysize = pParse->fdo_filter_yyssp - fdo_filter_yyss + 1;

#ifdef fdo_filter_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	FDO_FILTER_YYSTYPE *fdo_filter_yyvs1 = fdo_filter_yyvs;
	short *fdo_filter_yyss1 = fdo_filter_yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if fdo_filter_yyoverflow is a macro.  */
	fdo_filter_yyoverflow ("parser stack overflow",
		    &fdo_filter_yyss1, fdo_filter_yysize * sizeof (*pParse->fdo_filter_yyssp),
		    &fdo_filter_yyvs1, fdo_filter_yysize * sizeof (*pParse->fdo_filter_yyvsp),

		    &fdo_filter_yystacksize);

	fdo_filter_yyss = fdo_filter_yyss1;
	fdo_filter_yyvs = fdo_filter_yyvs1;
      }
#else /* no fdo_filter_yyoverflow */
# ifndef FDO_FILTER_YYSTACK_RELOCATE
      goto fdo_filter_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (FDO_FILTER_YYMAXDEPTH <= fdo_filter_yystacksize)
	goto fdo_filter_yyoverflowlab;
      fdo_filter_yystacksize *= 2;
      if (FDO_FILTER_YYMAXDEPTH < fdo_filter_yystacksize)
	fdo_filter_yystacksize = FDO_FILTER_YYMAXDEPTH;

      {
	short *fdo_filter_yyss1 = fdo_filter_yyss;
	union fdo_filter_yyalloc *fdo_filter_yyptr =
	  (union fdo_filter_yyalloc *) FDO_FILTER_YYSTACK_ALLOC (FDO_FILTER_YYSTACK_BYTES (fdo_filter_yystacksize));
	if (! fdo_filter_yyptr)
	  goto fdo_filter_yyoverflowlab;
	FDO_FILTER_YYSTACK_RELOCATE (fdo_filter_yyss);
	FDO_FILTER_YYSTACK_RELOCATE (fdo_filter_yyvs);

#  undef FDO_FILTER_YYSTACK_RELOCATE
	if (fdo_filter_yyss1 != pParse->fdo_filter_yyss)
	  FDO_FILTER_YYSTACK_FREE (fdo_filter_yyss1);
      }
# endif
#endif /* no fdo_filter_yyoverflow */

      pParse->fdo_filter_yyssp = fdo_filter_yyss + fdo_filter_yysize - 1;
      pParse->fdo_filter_yyvsp = fdo_filter_yyvs + fdo_filter_yysize - 1;


      FDO_FILTER_YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) fdo_filter_yystacksize));

      if (fdo_filter_yyss + fdo_filter_yystacksize - 1 <= pParse->fdo_filter_yyssp)
	FDO_FILTER_YYABORT;
    }

  FDO_FILTER_YYDPRINTF ((stderr, "Entering state %d\n", fdo_filter_yystate));

  goto fdo_filter_yybackup;

/*-----------.
| fdo_filter_yybackup.  |
`-----------*/
fdo_filter_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* fdo_filter_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  fdo_filter_yyn = fdo_filter_yypact[fdo_filter_yystate];
  if (fdo_filter_yyn == FDO_FILTER_YYPACT_NINF)
    goto fdo_filter_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* FDO_FILTER_YYCHAR is either FDO_FILTER_YYEMPTY or FDO_FILTER_YYEOF or a valid lookahead symbol.  */
  if (pParse->fdo_filter_yychar == FDO_FILTER_YYEMPTY)
    {
      FDO_FILTER_YYDPRINTF ((stderr, "Reading a token: "));
      pParse->fdo_filter_yychar = FDO_FILTER_YYLEX;
    }

  if (pParse->fdo_filter_yychar <= FDO_FILTER_YYEOF)
    {
      pParse->fdo_filter_yychar = fdo_filter_yytoken = FDO_FILTER_YYEOF;
      FDO_FILTER_YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      fdo_filter_yytoken = FDO_FILTER_YYTRANSLATE (pParse->fdo_filter_yychar);
      FDO_FILTER_YYDSYMPRINTF ("Next token is", fdo_filter_yytoken, &pParse->fdo_filter_yylval, &fdo_filter_yylloc);
    }

  /* If the proper action on seeing token FDO_FILTER_YYTOKEN is to reduce or to
     detect an error, take that action.  */
  fdo_filter_yyn += fdo_filter_yytoken;
  if (fdo_filter_yyn < 0 || FDO_FILTER_YYLAST < fdo_filter_yyn || fdo_filter_yycheck[fdo_filter_yyn] != fdo_filter_yytoken)
    goto fdo_filter_yydefault;
  fdo_filter_yyn = fdo_filter_yytable[fdo_filter_yyn];
  if (fdo_filter_yyn <= 0)
    {
      if (fdo_filter_yyn == 0 || fdo_filter_yyn == FDO_FILTER_YYTABLE_NINF)
	goto fdo_filter_yyerrlab;
      fdo_filter_yyn = -fdo_filter_yyn;
      goto fdo_filter_yyreduce;
    }

  if (fdo_filter_yyn == FDO_FILTER_YYFINAL)
    FDO_FILTER_YYACCEPT;

  /* Shift the lookahead token.  */
  FDO_FILTER_YYDPRINTF ((stderr, "Shifting token %s, ", fdo_filter_yytname[fdo_filter_yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (pParse->fdo_filter_yychar != FDO_FILTER_YYEOF)
    pParse->fdo_filter_yychar = FDO_FILTER_YYEMPTY;

  *++pParse->fdo_filter_yyvsp = pParse->fdo_filter_yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (fdo_filter_yyerrstatus)
    fdo_filter_yyerrstatus--;

  fdo_filter_yystate = fdo_filter_yyn;
  goto fdo_filter_yynewstate;


/*-----------------------------------------------------------.
| fdo_filter_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
fdo_filter_yydefault:
  fdo_filter_yyn = fdo_filter_yydefact[fdo_filter_yystate];
  if (fdo_filter_yyn == 0)
    goto fdo_filter_yyerrlab;
  goto fdo_filter_yyreduce;


/*-----------------------------.
| fdo_filter_yyreduce -- Do a reduction.  |
`-----------------------------*/
fdo_filter_yyreduce:
  /* fdo_filter_yyn is the number of a rule to reduce with.  */
  fdo_filter_yylen = fdo_filter_yyr2[fdo_filter_yyn];

  /* If FDO_FILTER_YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets FDO_FILTER_YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to FDO_FILTER_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that FDO_FILTER_YYVAL may be used uninitialized.  */
  pParse->fdo_filter_yyval = pParse->fdo_filter_yyvsp[1-fdo_filter_yylen];


  FDO_FILTER_YY_REDUCE_PRINT (fdo_filter_yyn);
  switch (fdo_filter_yyn)
    {
        case 2:

    {pParse->SetRoot(Node_Copy(L"Filter", pParse->fdo_filter_yyvsp[0].m_node));	FDO_FILTER_YYACCEPT;}
    break;

  case 3:

    {pParse->SetRoot(Node_Copy(L"Filter", pParse->fdo_filter_yyvsp[0].m_node)); FDO_FILTER_YYACCEPT;}
    break;

  case 4:

    {Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	FDO_FILTER_YYABORT;}
    break;

  case 5:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"(Expression)", pParse->fdo_filter_yyvsp[-1].m_node);}
    break;

  case 6:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"UnaryExpression", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 7:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"BinaryExpression", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 8:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"Function", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 9:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"Identifier", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 10:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"ValueExpression", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 11:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Add, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 12:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Subtract, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 13:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Multiply, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 14:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryOperations_Divide, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 15:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"boolean", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 16:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"datetime", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 17:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"double", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 18:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"integer", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 19:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"int64", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 20:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"string", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 21:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"TODO:BLOB", NULL);}
    break;

  case 22:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"TODO:CLOB", NULL);}
    break;

  case 23:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
    break;

  case 24:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
    break;

  case 25:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
    break;

  case 26:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"STRING", FdoDataValue::Create(pParse->fdo_filter_yyvsp[0].m_string));}
    break;

  case 27:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"INTEGER", FdoInt32Value::Create(pParse->fdo_filter_yyvsp[0].m_integer));}
    break;

  case 28:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"INT64", FdoInt64Value::Create(pParse->fdo_filter_yyvsp[0].m_int64));}
    break;

  case 29:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"DOUBLE", FdoDoubleValue::Create(pParse->fdo_filter_yyvsp[0].m_double));}
    break;

  case 30:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"DateTime", FdoDataValue::Create(pParse->fdo_filter_yyvsp[0].m_datetime));}
    break;

  case 31:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node)->GetName(), (FdoExpressionCollection*)pParse->fdo_filter_yyvsp[-1].m_node));}
    break;

  case 32:

    {pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
    break;

  case 33:

    {pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"Expression Arg 1,");}
    break;

  case 34:

    {((FdoExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"Expression Arg N,");}
    break;

  case 35:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue(pParse->fdo_filter_yyvsp[-1].m_string));}
    break;

  case 36:

    {
		    FdoComputedIdentifier* id = pParse->FindComputedIdentifier( pParse->fdo_filter_yyvsp[0].m_string );
		    if( id == NULL )
		        pParse->fdo_filter_yyval.m_node = Node_Add(L"IDENTIFIER", FdoIdentifier::Create(pParse->fdo_filter_yyvsp[0].m_string));
		    else
		        pParse->fdo_filter_yyval.m_node = id;
		}
    break;

  case 37:

    {}
    break;

  case 38:

    {}
    break;

  case 39:

    {
            FdoComputedIdentifier* id = FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-3].m_node);
            Node_Add(L"ComputedIdentifier", id);
            pParse->AddCompIdentifier(id);
        }
    break;

  case 40:

    {
            FdoComputedIdentifier* id = FdoComputedIdentifier::Create(((FdoIdentifier*)pParse->fdo_filter_yyvsp[0].m_node)->GetName(), (FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node);
            Node_Add(L"ComputedIdentifier", id);
            pParse->AddCompIdentifier(id);
        }
    break;

  case 41:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"LiteralValue", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 42:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"Parameter", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 43:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"GeometryValue", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 44:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"DataValue", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 45:

    {pParse->fdo_filter_yyval.m_node = Node_Add(L"PARAMETER", FdoParameter::Create(pParse->fdo_filter_yyvsp[0].m_string));}
    break;

  case 46:

    {pParse->fdo_filter_yyval.m_node = Node_Add(L":Parameter", FdoParameter::Create(pParse->fdo_filter_yyvsp[0].m_string));}
    break;

  case 47:

    {pParse->fdo_filter_yyval.m_node = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 48:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"(Filter)", pParse->fdo_filter_yyvsp[-1].m_node);}
    break;

  case 49:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"LogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 50:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"SearchCondition", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 51:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"BinaryLogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 52:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"UnaryLogicalOperator", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 53:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"DistanceCondition", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 54:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"SpatialCondition", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 55:

    {pParse->fdo_filter_yyval.m_node=pParse->AddNodeToDelete(FdoValueExpressionCollection::Create()); ((FdoValueExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoValueExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"ValueExpression 1");}
    break;

  case 56:

    {((FdoValueExpressionCollection*)pParse->fdo_filter_yyval.m_node)->Add((FdoValueExpression*)pParse->fdo_filter_yyvsp[0].m_node); Node_Trace(L"ValueExpression n");}
    break;

  case 57:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"ValueExpressionCollection", FdoInCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-4].m_node, (FdoValueExpressionCollection*)pParse->fdo_filter_yyvsp[-1].m_node));}
    break;

  case 58:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"NULL", FdoNullCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-1].m_node));}
    break;

  case 59:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"InCondition", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 60:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"ComparisonCondition", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 61:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"GeometricCondition", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 62:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"NullCondition", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 63:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"Id-SpatialOp-Expression", FdoSpatialCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-2].m_node, (FdoSpatialOperations)pParse->fdo_filter_yyvsp[-1].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 64:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"NOT", FdoUnaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node, FdoUnaryLogicalOperations_Not));}
    break;

  case 65:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"AND", FdoBinaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryLogicalOperations_And, (FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 66:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"OR", FdoBinaryLogicalOperator::Create((FdoFilter*)pParse->fdo_filter_yyvsp[-2].m_node, FdoBinaryLogicalOperations_Or, (FdoFilter*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 67:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"ComparisonOperations", FdoComparisonCondition::Create((FdoExpression*)pParse->fdo_filter_yyvsp[-2].m_node, (FdoComparisonOperations)pParse->fdo_filter_yyvsp[-1].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[0].m_node));}
    break;

  case 68:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"Indentifier-DistanceOp-Expression", FdoDistanceCondition::Create((FdoIdentifier*)pParse->fdo_filter_yyvsp[-3].m_node, (FdoDistanceOperations)pParse->fdo_filter_yyvsp[-2].m_id, (FdoExpression*)pParse->fdo_filter_yyvsp[-1].m_node, ((FdoDoubleValue*)pParse->fdo_filter_yyvsp[0].m_node)->GetDouble()));}
    break;

  case 69:

    {pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_EqualTo; Node_Trace(L"EQ");}
    break;

  case 70:

    {pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_NotEqualTo; Node_Trace(L"NE");}
    break;

  case 71:

    {pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_GreaterThan; Node_Trace(L"GT");}
    break;

  case 72:

    {pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_GreaterThanOrEqualTo; Node_Trace(L"GE");}
    break;

  case 73:

    {pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_LessThan; Node_Trace(L"LT");}
    break;

  case 74:

    {pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_LessThanOrEqualTo; Node_Trace(L"LE");}
    break;

  case 75:

    {pParse->fdo_filter_yyval.m_id=FdoComparisonOperations_Like; Node_Trace(L"LIKE");}
    break;

  case 76:

    {pParse->fdo_filter_yyval.m_id=FdoDistanceOperations_Beyond; Node_Trace(L"BEYOND");}
    break;

  case 77:

    {pParse->fdo_filter_yyval.m_id=FdoDistanceOperations_Within; Node_Trace(L"WITHINDISTANCE");}
    break;

  case 78:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Contains; Node_Trace(L"CONTAINS");}
    break;

  case 79:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Crosses; Node_Trace(L"CROSSES");}
    break;

  case 80:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Disjoint; Node_Trace(L"DISJOINT");}
    break;

  case 81:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Equals; Node_Trace(L"EQUALS");}
    break;

  case 82:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Inside; Node_Trace(L"INSIDE");}
    break;

  case 83:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Intersects; Node_Trace(L"INTERSECTS");}
    break;

  case 84:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Overlaps; Node_Trace(L"OVERLAPS");}
    break;

  case 85:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Touches; Node_Trace(L"TOUCHES");}
    break;

  case 86:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_Within; Node_Trace(L"WITHIN");}
    break;

  case 87:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_CoveredBy; Node_Trace(L"COVEREDBY");}
    break;

  case 88:

    {pParse->fdo_filter_yyval.m_id=FdoSpatialOperations_EnvelopeIntersects; Node_Trace(L"ENVELOPEINTERSECTS");}
    break;

  case 89:

    {pParse->fdo_filter_yyval.m_node=Node_Copy(L"double", pParse->fdo_filter_yyvsp[0].m_node);}
    break;

  case 90:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"INTEGER", FdoDoubleValue::Create((double)pParse->fdo_filter_yyvsp[0].m_integer));}
    break;

  case 91:

    {pParse->fdo_filter_yyval.m_node=Node_Add(L"INT64", FdoDoubleValue::Create((double)pParse->fdo_filter_yyvsp[0].m_int64));}
    break;


    }

/* Line 991 of yacc.c.  */


  pParse->fdo_filter_yyvsp -= fdo_filter_yylen;
  pParse->fdo_filter_yyssp -= fdo_filter_yylen;


  FDO_FILTER_YY_STACK_PRINT (fdo_filter_yyss, pParse->fdo_filter_yyssp);

  *++pParse->fdo_filter_yyvsp = pParse->fdo_filter_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  fdo_filter_yyn = fdo_filter_yyr1[fdo_filter_yyn];

  fdo_filter_yystate = fdo_filter_yypgoto[fdo_filter_yyn - FDO_FILTER_YYNTOKENS] + *pParse->fdo_filter_yyssp;
  if (0 <= fdo_filter_yystate && fdo_filter_yystate <= FDO_FILTER_YYLAST && fdo_filter_yycheck[fdo_filter_yystate] == *pParse->fdo_filter_yyssp)
    fdo_filter_yystate = fdo_filter_yytable[fdo_filter_yystate];
  else
    fdo_filter_yystate = fdo_filter_yydefgoto[fdo_filter_yyn - FDO_FILTER_YYNTOKENS];

  goto fdo_filter_yynewstate;


/*------------------------------------.
| fdo_filter_yyerrlab -- here on detecting error |
`------------------------------------*/
fdo_filter_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!fdo_filter_yyerrstatus)
    {
      ++pParse->fdo_filter_yynerrs;
#if FDO_FILTER_YYERROR_VERBOSE
      fdo_filter_yyn = fdo_filter_yypact[fdo_filter_yystate];

      if (FDO_FILTER_YYPACT_NINF < fdo_filter_yyn && fdo_filter_yyn < FDO_FILTER_YYLAST)
	{
	  FDO_FILTER_YYSIZE_T fdo_filter_yysize = 0;
	  int fdo_filter_yytype = FDO_FILTER_YYTRANSLATE (pParse->fdo_filter_yychar);
	  char *fdo_filter_yymsg;
	  int fdo_filter_yyx, fdo_filter_yycount;

	  fdo_filter_yycount = 0;
	  /* Start FDO_FILTER_YYX at -FDO_FILTER_YYN if negative to avoid negative indexes in
	     FDO_FILTER_YYCHECK.  */
	  for (fdo_filter_yyx = fdo_filter_yyn < 0 ? -fdo_filter_yyn : 0;
	       fdo_filter_yyx < (int) (sizeof (fdo_filter_yytname) / sizeof (char *)); fdo_filter_yyx++)
	    if (fdo_filter_yycheck[fdo_filter_yyx + fdo_filter_yyn] == fdo_filter_yyx && fdo_filter_yyx != FDO_FILTER_YYTERROR)
	      fdo_filter_yysize += fdo_filter_yystrlen (fdo_filter_yytname[fdo_filter_yyx]) + 15, fdo_filter_yycount++;
	  fdo_filter_yysize += fdo_filter_yystrlen ("syntax error, unexpected ") + 1;
	  fdo_filter_yysize += fdo_filter_yystrlen (fdo_filter_yytname[fdo_filter_yytype]);
	  fdo_filter_yymsg = (char *) FDO_FILTER_YYSTACK_ALLOC (fdo_filter_yysize);
	  if (fdo_filter_yymsg != 0)
	    {
	      char *fdo_filter_yyp = fdo_filter_yystpcpy (fdo_filter_yymsg, "syntax error, unexpected ");
	      fdo_filter_yyp = fdo_filter_yystpcpy (fdo_filter_yyp, fdo_filter_yytname[fdo_filter_yytype]);

	      if (fdo_filter_yycount < 5)
		{
		  fdo_filter_yycount = 0;
		  for (fdo_filter_yyx = fdo_filter_yyn < 0 ? -fdo_filter_yyn : 0;
		       fdo_filter_yyx < (int) (sizeof (fdo_filter_yytname) / sizeof (char *));
		       fdo_filter_yyx++)
		    if (fdo_filter_yycheck[fdo_filter_yyx + fdo_filter_yyn] == fdo_filter_yyx && fdo_filter_yyx != FDO_FILTER_YYTERROR)
		      {
			const char *fdo_filter_yyq = ! fdo_filter_yycount ? ", expecting " : " or ";
			fdo_filter_yyp = fdo_filter_yystpcpy (fdo_filter_yyp, fdo_filter_yyq);
			fdo_filter_yyp = fdo_filter_yystpcpy (fdo_filter_yyp, fdo_filter_yytname[fdo_filter_yyx]);
			fdo_filter_yycount++;
		      }
		}
	      fdo_filter_yyerror(pParse,fdo_filter_yymsg);
	      FDO_FILTER_YYSTACK_FREE (fdo_filter_yymsg);
	    }
	  else
	    fdo_filter_yyerror(pParse,"syntax error; also virtual memory exhausted");
	}
      else
#endif /* FDO_FILTER_YYERROR_VERBOSE */
	fdo_filter_yyerror(pParse,"syntax error");
    }



  if (fdo_filter_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (pParse->fdo_filter_yychar == FDO_FILTER_YYEOF)
        {
	  /* Pop the error token.  */
          FDO_FILTER_YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (fdo_filter_yyss < pParse->fdo_filter_yyssp)
	    {
	      FDO_FILTER_YYDSYMPRINTF ("Error: popping", fdo_filter_yystos[*pParse->fdo_filter_yyssp], pParse->fdo_filter_yyvsp, fdo_filter_yylsp);
	      fdo_filter_yydestruct (fdo_filter_yystos[*pParse->fdo_filter_yyssp], pParse->fdo_filter_yyvsp);
	      FDO_FILTER_YYPOPSTACK;
	    }
	  FDO_FILTER_YYABORT;
        }

      FDO_FILTER_YYDSYMPRINTF ("Error: discarding", fdo_filter_yytoken, &pParse->fdo_filter_yylval, &fdo_filter_yylloc);
      fdo_filter_yydestruct (fdo_filter_yytoken, &pParse->fdo_filter_yylval);
      pParse->fdo_filter_yychar = FDO_FILTER_YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto fdo_filter_yyerrlab2;


/*----------------------------------------------------.
| fdo_filter_yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
fdo_filter_yyerrlab1:

  /* Suppress GCC warning that fdo_filter_yyerrlab1 is unused when no action
     invokes FDO_FILTER_YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__))
#endif


  goto fdo_filter_yyerrlab2;


/*---------------------------------------------------------------.
| fdo_filter_yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
fdo_filter_yyerrlab2:
  fdo_filter_yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      fdo_filter_yyn = fdo_filter_yypact[fdo_filter_yystate];
      if (fdo_filter_yyn != FDO_FILTER_YYPACT_NINF)
	{
	  fdo_filter_yyn += FDO_FILTER_YYTERROR;
	  if (0 <= fdo_filter_yyn && fdo_filter_yyn <= FDO_FILTER_YYLAST && fdo_filter_yycheck[fdo_filter_yyn] == FDO_FILTER_YYTERROR)
	    {
	      fdo_filter_yyn = fdo_filter_yytable[fdo_filter_yyn];
	      if (0 < fdo_filter_yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (pParse->fdo_filter_yyssp == fdo_filter_yyss)
	FDO_FILTER_YYABORT;

      FDO_FILTER_YYDSYMPRINTF ("Error: popping", fdo_filter_yystos[*pParse->fdo_filter_yyssp], pParse->fdo_filter_yyvsp, fdo_filter_yylsp);
      fdo_filter_yydestruct (fdo_filter_yystos[fdo_filter_yystate], pParse->fdo_filter_yyvsp);
      pParse->fdo_filter_yyvsp--;
      fdo_filter_yystate = *--pParse->fdo_filter_yyssp;

      FDO_FILTER_YY_STACK_PRINT (fdo_filter_yyss, pParse->fdo_filter_yyssp);
    }

  if (fdo_filter_yyn == FDO_FILTER_YYFINAL)
    FDO_FILTER_YYACCEPT;

  FDO_FILTER_YYDPRINTF ((stderr, "Shifting error token, "));

  *++pParse->fdo_filter_yyvsp = pParse->fdo_filter_yylval;


  fdo_filter_yystate = fdo_filter_yyn;
  goto fdo_filter_yynewstate;


/*-------------------------------------.
| fdo_filter_yyacceptlab -- FDO_FILTER_YYACCEPT comes here.  |
`-------------------------------------*/
fdo_filter_yyacceptlab:
  fdo_filter_yyresult = 0;
  goto fdo_filter_yyreturn;

/*-----------------------------------.
| fdo_filter_yyabortlab -- FDO_FILTER_YYABORT comes here.  |
`-----------------------------------*/
fdo_filter_yyabortlab:
  fdo_filter_yyresult = 1;
  goto fdo_filter_yyreturn;

#ifndef fdo_filter_yyoverflow
/*----------------------------------------------.
| fdo_filter_yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
fdo_filter_yyoverflowlab:
  fdo_filter_yyerror(pParse,"parser stack overflow");
  fdo_filter_yyresult = 2;
  /* Fall through.  */
#endif

fdo_filter_yyreturn:
#ifndef fdo_filter_yyoverflow
  if (fdo_filter_yyss != pParse->fdo_filter_yyss)
    FDO_FILTER_YYSTACK_FREE (fdo_filter_yyss);
#endif
  return fdo_filter_yyresult;
}




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

