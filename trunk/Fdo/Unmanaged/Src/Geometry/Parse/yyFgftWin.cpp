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

/* All symbols defined below should begin with fdo_fgft_yy or FDO_FGFT_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define FDO_FGFT_YYBISON 1

/* Skeleton name.  */
#define FDO_FGFT_YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define FDO_FGFT_YYPURE 0

/* Using locations.  */
#define FDO_FGFT_YYLSP_NEEDED 0



/* Tokens.  */
#if 0 
# define FDO_FGFT_YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum fdo_fgft_yytokentype {
     FdoToken_POINT = 258,
     FdoToken_LINESTRING = 259,
     FdoToken_POLYGON = 260,
     FdoToken_MULTIPOINT = 261,
     FdoToken_MULTILINESTRING = 262,
     FdoToken_MULTIPOLYGON = 263,
     FdoToken_GEOMETRYCOLLECTION = 264,
     FdoToken_CURVESTRING = 265,
     FdoToken_CURVEPOLYGON = 266,
     FdoToken_MULTICURVESTRING = 267,
     FdoToken_MULTICURVEPOLYGON = 268,
     FdoToken_CIRCLE = 269,
     FdoToken_RECTANGLE = 270,
     FdoToken_TINTERRAIN = 271,
     FdoToken_GRIDTERRAIN = 272,
     FdoToken_CUSTOM = 273,
     FdoToken_BEZIERSEGMENT = 274,
     FdoToken_XY = 275,
     FdoToken_XYZ = 276,
     FdoToken_XYM = 277,
     FdoToken_XYZM = 278,
     FdoToken_CIRCULARARCSEGMENT = 279,
     FdoToken_LINESTRINGSEGMENT = 280,
     FdoToken_ARC = 281,
     FdoToken_DOUBLE = 282,
     FdoToken_LeftParenthesis = 283,
     FdoToken_RightParenthesis = 284,
     FdoToken_Comma = 285
   };
#endif
#define FdoToken_POINT 258
#define FdoToken_LINESTRING 259
#define FdoToken_POLYGON 260
#define FdoToken_MULTIPOINT 261
#define FdoToken_MULTILINESTRING 262
#define FdoToken_MULTIPOLYGON 263
#define FdoToken_GEOMETRYCOLLECTION 264
#define FdoToken_CURVESTRING 265
#define FdoToken_CURVEPOLYGON 266
#define FdoToken_MULTICURVESTRING 267
#define FdoToken_MULTICURVEPOLYGON 268
#define FdoToken_CIRCLE 269
#define FdoToken_RECTANGLE 270
#define FdoToken_TINTERRAIN 271
#define FdoToken_GRIDTERRAIN 272
#define FdoToken_CUSTOM 273
#define FdoToken_BEZIERSEGMENT 274
#define FdoToken_XY 275
#define FdoToken_XYZ 276
#define FdoToken_XYM 277
#define FdoToken_XYZM 278
#define FdoToken_CIRCULARARCSEGMENT 279
#define FdoToken_LINESTRINGSEGMENT 280
#define FdoToken_ARC 281
#define FdoToken_DOUBLE 282
#define FdoToken_LeftParenthesis 283
#define FdoToken_RightParenthesis 284
#define FdoToken_Comma 285




/* Copy the first part of user declarations.  */


#include "ParseFgft.h"
#pragma warning(disable:4102)	// unreferenced labels in fdo_fgft_yyFgft.cpp


/* Enabling traces.  */
#ifndef FDO_FGFT_YYDEBUG
# define FDO_FGFT_YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef FDO_FGFT_YYERROR_VERBOSE
# undef FDO_FGFT_YYERROR_VERBOSE
# define FDO_FGFT_YYERROR_VERBOSE 1
#else
# define FDO_FGFT_YYERROR_VERBOSE 0
#endif

#if ! defined (FDO_FGFT_YYSTYPE) && ! defined (FDO_FGFT_YYSTYPE_IS_DECLARED)

typedef union FDO_FGFT_YYSTYPE _FDO_FGFT_YYSTYPE
{
	double		m_double;		// floating point values 
 
/* Line 191 of yacc.c.  */

# define fdo_fgft_yystype FDO_FGFT_YYSTYPE /* obsolescent; will be withdrawn */
# define FDO_FGFT_YYSTYPE_IS_DECLARED 1
# define FDO_FGFT_YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */


#if ! defined (fdo_fgft_yyoverflow) || FDO_FGFT_YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if FDO_FGFT_YYSTACK_USE_ALLOCA
#  define FDO_FGFT_YYSTACK_ALLOC alloca
# else
#  ifndef FDO_FGFT_YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define FDO_FGFT_YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define FDO_FGFT_YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef FDO_FGFT_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define FDO_FGFT_YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define FDO_FGFT_YYSIZE_T size_t
#  endif
#  define FDO_FGFT_YYSTACK_ALLOC malloc
#  define FDO_FGFT_YYSTACK_FREE free
# endif
#endif /* ! defined (fdo_fgft_yyoverflow) || FDO_FGFT_YYERROR_VERBOSE */


#if (! defined (fdo_fgft_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (FDO_FGFT_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union fdo_fgft_yyalloc
{
  short fdo_fgft_yyss;
  FDO_FGFT_YYSTYPE fdo_fgft_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define FDO_FGFT_YYSTACK_GAP_MAXIMUM (sizeof (union fdo_fgft_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define FDO_FGFT_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (FDO_FGFT_YYSTYPE))				\
      + FDO_FGFT_YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef FDO_FGFT_YYCOPY
#  if 1 < __GNUC__
#   define FDO_FGFT_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define FDO_FGFT_YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register FDO_FGFT_YYSIZE_T fdo_fgft_yyi;		\
	  for (fdo_fgft_yyi = 0; fdo_fgft_yyi < (Count); fdo_fgft_yyi++)	\
	    (To)[fdo_fgft_yyi] = (From)[fdo_fgft_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables FDO_FGFT_YYSIZE and FDO_FGFT_YYSTACKSIZE give the old and new number of
   elements in the stack, and FDO_FGFT_YYPTR gives the new location of the
   stack.  Advance FDO_FGFT_YYPTR to a properly aligned location for the next
   stack.  */
# define FDO_FGFT_YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	FDO_FGFT_YYSIZE_T fdo_fgft_yynewbytes;						\
	FDO_FGFT_YYCOPY (&fdo_fgft_yyptr->Stack, Stack, fdo_fgft_yysize);				\
	Stack = &fdo_fgft_yyptr->Stack;						\
	fdo_fgft_yynewbytes = fdo_fgft_yystacksize * sizeof (*Stack) + FDO_FGFT_YYSTACK_GAP_MAXIMUM; \
	fdo_fgft_yyptr += fdo_fgft_yynewbytes / sizeof (*fdo_fgft_yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char fdo_fgft_yysigned_char;
#else
   typedef short fdo_fgft_yysigned_char;
#endif

/* FDO_FGFT_YYFINAL -- State number of the termination state. */
#define FDO_FGFT_YYFINAL  26
/* FDO_FGFT_YYLAST -- Last index in FDO_FGFT_YYTABLE.  */
#define FDO_FGFT_YYLAST   104

/* FDO_FGFT_YYNTOKENS -- Number of terminals. */
#define FDO_FGFT_YYNTOKENS  34
/* FDO_FGFT_YYNNTS -- Number of nonterminals. */
#define FDO_FGFT_YYNNTS  41
/* FDO_FGFT_YYNRULES -- Number of rules. */
#define FDO_FGFT_YYNRULES  66
/* FDO_FGFT_YYNRULES -- Number of states. */
#define FDO_FGFT_YYNSTATES  129

/* FDO_FGFT_YYTRANSLATE(FDO_FGFT_YYLEX) -- Bison symbol number corresponding to FDO_FGFT_YYLEX.  */
#define FDO_FGFT_YYUNDEFTOK  2
#define FDO_FGFT_YYMAXUTOK   285

#define FDO_FGFT_YYTRANSLATE(FDO_FGFT_YYX) 						\
  ((unsigned int) (FDO_FGFT_YYX) <= FDO_FGFT_YYMAXUTOK ? fdo_fgft_yytranslate[FDO_FGFT_YYX] : FDO_FGFT_YYUNDEFTOK)

/* FDO_FGFT_YYTRANSLATE[FDO_FGFT_YYLEX] -- Bison symbol number corresponding to FDO_FGFT_YYLEX.  */
static const unsigned char fdo_fgft_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      31,    32,     2,     2,    33,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30
};

#if FDO_FGFT_YYDEBUG
/* FDO_FGFT_YYPRHS[FDO_FGFT_YYN] -- Index of the first RHS symbol of rule number FDO_FGFT_YYN in
   FDO_FGFT_YYRHS.  */
static const unsigned char fdo_fgft_yyprhs[] =
{
       0,     0,     3,     5,     7,     8,    13,    14,    19,    20,
      25,    26,    31,    32,    37,    38,    43,    44,    49,    50,
      55,    56,    61,    62,    67,    68,    72,    76,    80,    84,
      88,    92,    96,   100,   107,   111,   115,   119,   120,   122,
     124,   126,   128,   131,   135,   140,   142,   146,   148,   149,
     154,   156,   157,   162,   164,   168,   169,   177,   178,   184,
     186,   190,   192,   193,   198,   200,   201
};

/* FDO_FGFT_YYRHS -- A `-1'-separated list of the rules' RHS. */
static const fdo_fgft_yysigned_char fdo_fgft_yyrhs[] =
{
      35,     0,    -1,    36,    -1,     1,    -1,    -1,     3,    37,
      59,    48,    -1,    -1,     4,    38,    59,    49,    -1,    -1,
       5,    39,    59,    50,    -1,    -1,    10,    40,    59,    55,
      -1,    -1,    11,    41,    59,    56,    -1,    -1,     6,    42,
      59,    51,    -1,    -1,     7,    43,    59,    52,    -1,    -1,
       8,    44,    59,    53,    -1,    -1,    12,    45,    59,    57,
      -1,    -1,    13,    46,    59,    58,    -1,    -1,     9,    47,
      54,    -1,    31,    60,    32,    -1,    31,    61,    32,    -1,
      31,    62,    32,    -1,    31,    61,    32,    -1,    31,    62,
      32,    -1,    31,    64,    32,    -1,    31,    66,    32,    -1,
      31,    60,    31,    70,    32,    32,    -1,    31,    71,    32,
      -1,    31,    71,    32,    -1,    31,    73,    32,    -1,    -1,
      20,    -1,    21,    -1,    22,    -1,    23,    -1,    27,    27,
      -1,    27,    27,    27,    -1,    27,    27,    27,    27,    -1,
      60,    -1,    61,    33,    60,    -1,    49,    -1,    -1,    62,
      33,    63,    49,    -1,    50,    -1,    -1,    64,    33,    65,
      50,    -1,    36,    -1,    66,    33,    36,    -1,    -1,    24,
      68,    31,    60,    33,    60,    32,    -1,    -1,    25,    69,
      31,    61,    32,    -1,    67,    -1,    70,    33,    67,    -1,
      55,    -1,    -1,    71,    33,    72,    55,    -1,    56,    -1,
      -1,    73,    33,    74,    56,    -1
};

/* FDO_FGFT_YYRLINE[FDO_FGFT_YYN] -- source line where rule number FDO_FGFT_YYN was defined.  */
static const unsigned char fdo_fgft_yyrline[] =
{
       0,    75,    75,    76,    80,    80,    82,    82,    84,    84,
      86,    86,    88,    88,    90,    90,    92,    92,    94,    94,
      96,    96,    98,    98,   100,   100,   105,   106,   107,   108,
     109,   110,   111,   114,   115,   116,   117,   119,   120,   121,
     122,   123,   127,   129,   131,   135,   136,   138,   139,   139,
     141,   142,   142,   144,   145,   148,   148,   149,   149,   151,
     152,   154,   155,   155,   157,   158,   158
};
#endif

#if FDO_FGFT_YYDEBUG || FDO_FGFT_YYERROR_VERBOSE
/* FDO_FGFT_YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at FDO_FGFT_YYNTOKENS, nonterminals. */
static const char *const fdo_fgft_yytname[] =
{
  "$end", "error", "$undefined", "FdoToken_POINT", "FdoToken_LINESTRING", 
  "FdoToken_POLYGON", "FdoToken_MULTIPOINT", "FdoToken_MULTILINESTRING", 
  "FdoToken_MULTIPOLYGON", "FdoToken_GEOMETRYCOLLECTION", 
  "FdoToken_CURVESTRING", "FdoToken_CURVEPOLYGON", 
  "FdoToken_MULTICURVESTRING", "FdoToken_MULTICURVEPOLYGON", 
  "FdoToken_CIRCLE", "FdoToken_RECTANGLE", "FdoToken_TINTERRAIN", 
  "FdoToken_GRIDTERRAIN", "FdoToken_CUSTOM", "FdoToken_BEZIERSEGMENT", 
  "FdoToken_XY", "FdoToken_XYZ", "FdoToken_XYM", "FdoToken_XYZM", 
  "FdoToken_CIRCULARARCSEGMENT", "FdoToken_LINESTRINGSEGMENT", 
  "FdoToken_ARC", "FdoToken_DOUBLE", "FdoToken_LeftParenthesis", 
  "FdoToken_RightParenthesis", "FdoToken_Comma", "'('", "')'", "','", 
  "$accept", "fdo", "Fgft", "@1", "@2", "@3", "@4", "@5", "@6", "@7", 
  "@8", "@9", "@10", "@11", "PointEntity", "LineString", "Polygon", 
  "MultiPoint", "MultiLineString", "MultiPolygon", "GeometryCollection", 
  "CurveString", "CurvePolygon", "MultiCurveString", "MultiCurvePolygon", 
  "Dimensionality", "Point", "PointCollection", "LineStringCollection", 
  "@12", "PolygonCollection", "@13", "FgftCollection", "CurveSegment", 
  "@14", "@15", "CurveSegmentCollection", "CurveStringCollection", "@16", 
  "CurvePolygonCollection", "@17", 0
};
#endif

# ifdef FDO_FGFT_YYPRINT
/* FDO_FGFT_YYTOKNUM[FDO_FGFT_YYLEX-NUM] -- Internal token number corresponding to
   token FDO_FGFT_YYLEX-NUM.  */
static const unsigned short fdo_fgft_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,    40,    41,    44
};
# endif

/* FDO_FGFT_YYR1[FDO_FGFT_YYN] -- Symbol number of symbol that rule FDO_FGFT_YYN derives.  */
static const unsigned char fdo_fgft_yyr1[] =
{
       0,    34,    35,    35,    37,    36,    38,    36,    39,    36,
      40,    36,    41,    36,    42,    36,    43,    36,    44,    36,
      45,    36,    46,    36,    47,    36,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    59,    59,
      59,    59,    60,    60,    60,    61,    61,    62,    63,    62,
      64,    65,    64,    66,    66,    68,    67,    69,    67,    70,
      70,    71,    72,    71,    73,    74,    73
};

/* FDO_FGFT_YYR2[FDO_FGFT_YYN] -- Number of symbols composing right hand side of rule FDO_FGFT_YYN.  */
static const unsigned char fdo_fgft_yyr2[] =
{
       0,     2,     1,     1,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     3,     3,     3,     3,     3,
       3,     3,     3,     6,     3,     3,     3,     0,     1,     1,
       1,     1,     2,     3,     4,     1,     3,     1,     0,     4,
       1,     0,     4,     1,     3,     0,     7,     0,     5,     1,
       3,     1,     0,     4,     1,     0,     4
};

/* FDO_FGFT_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when FDO_FGFT_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char fdo_fgft_yydefact[] =
{
       0,     3,     4,     6,     8,    14,    16,    18,    24,    10,
      12,    20,    22,     0,     2,    37,    37,    37,    37,    37,
      37,     0,    37,    37,    37,    37,     1,    38,    39,    40,
      41,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,     0,     5,     0,     7,     0,     9,     0,
      15,     0,    17,     0,    19,    53,     0,     0,    11,     0,
      13,     0,    21,     0,    23,     0,     0,    45,     0,    47,
       0,     0,     0,    50,     0,    32,     0,     0,    61,     0,
       0,    64,     0,    42,    26,    27,     0,    28,    48,    29,
      30,    31,    51,    54,     0,    34,    62,    35,    36,    65,
      43,    46,     0,     0,    55,    57,    59,     0,     0,     0,
      44,    49,    52,     0,     0,     0,     0,    63,    66,     0,
       0,    33,    60,     0,     0,     0,    58,     0,    56
};

/* FDO_FGFT_YYDEFGOTO[NTERM-NUM]. */
static const fdo_fgft_yysigned_char fdo_fgft_yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    22,    23,    18,    19,
      20,    24,    25,    21,    44,    69,    48,    50,    52,    54,
      38,    78,    60,    62,    64,    31,    67,    68,    70,   102,
      74,   103,    56,   106,   113,   114,   107,    79,   108,    82,
     109
};

/* FDO_FGFT_YYPACT[STATE-NUM] -- Index in FDO_FGFT_YYTABLE of the portion describing
   STATE-NUM.  */
#define FDO_FGFT_YYPACT_NINF -59
static const fdo_fgft_yysigned_char fdo_fgft_yypact[] =
{
      12,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,    44,   -59,   -13,   -13,   -13,   -13,   -13,
     -13,    19,   -13,   -13,   -13,   -13,   -59,   -59,   -59,   -59,
     -59,    29,    49,    50,    52,    53,    54,    23,   -59,    55,
      56,    57,    58,    63,   -59,    63,   -59,    49,   -59,    63,
     -59,    49,   -59,    50,   -59,   -59,   -21,    63,   -59,    55,
     -59,    55,   -59,    56,   -59,    64,    60,   -59,     5,   -59,
       7,     9,    14,   -59,    16,   -59,    23,    62,   -59,    20,
      33,   -59,    35,    67,   -59,   -59,    63,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,    45,   -59,   -59,   -59,   -59,   -59,
      68,   -59,    49,    50,   -59,   -59,   -59,    42,    55,    56,
     -59,   -59,   -59,    65,    66,    69,    45,   -59,   -59,    63,
      63,   -59,   -59,    70,    46,    63,   -59,    72,   -59
};

/* FDO_FGFT_YYPGOTO[NTERM-NUM].  */
static const fdo_fgft_yysigned_char fdo_fgft_yypgoto[] =
{
     -59,   -59,   -31,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -29,   -49,   -59,   -59,   -59,
     -59,   -37,   -58,   -59,   -59,    39,   -43,   -48,    26,   -59,
     -59,   -59,   -59,   -18,   -59,   -59,   -59,    38,   -59,   -59,
     -59
};

/* FDO_FGFT_YYTABLE[FDO_FGFT_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what FDO_FGFT_YYDEFACT says.
   If FDO_FGFT_YYTABLE_NINF, syntax error.  */
#define FDO_FGFT_YYTABLE_NINF -1
static const unsigned char fdo_fgft_yytable[] =
{
      66,    71,    58,    46,    73,    81,    55,    27,    28,    29,
      30,    75,    76,     1,    77,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    85,    86,    87,
      88,    89,    86,   101,    26,    93,    90,    88,    91,    92,
      37,   118,    95,    96,   112,    32,    33,    34,    35,    36,
      43,    39,    40,    41,    42,    97,    96,    98,    99,   104,
     105,   117,   124,   111,   115,   116,   123,    72,   126,    86,
      45,    47,   127,    49,    51,    53,    57,    59,    61,    63,
      65,    83,    84,    94,   100,   110,   119,   120,   122,    80,
       0,   121,     0,   125,   128
};

static const fdo_fgft_yysigned_char fdo_fgft_yycheck[] =
{
      43,    49,    39,    32,    53,    63,    37,    20,    21,    22,
      23,    32,    33,     1,    57,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    32,    33,    32,
      33,    32,    33,    86,     0,    76,    32,    33,    32,    33,
      31,   109,    32,    33,   103,    16,    17,    18,    19,    20,
      31,    22,    23,    24,    25,    32,    33,    32,    33,    24,
      25,   108,   120,   102,    32,    33,   119,    51,    32,    33,
      31,    31,   125,    31,    31,    31,    31,    31,    31,    31,
      27,    27,    32,    31,    27,    27,    31,    31,   116,    61,
      -1,    32,    -1,    33,    32
};

/* FDO_FGFT_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char fdo_fgft_yystos[] =
{
       0,     1,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    35,    36,    37,    38,    39,    42,    43,
      44,    47,    40,    41,    45,    46,     0,    20,    21,    22,
      23,    59,    59,    59,    59,    59,    59,    31,    54,    59,
      59,    59,    59,    31,    48,    31,    49,    31,    50,    31,
      51,    31,    52,    31,    53,    36,    66,    31,    55,    31,
      56,    31,    57,    31,    58,    27,    60,    60,    61,    49,
      62,    61,    62,    50,    64,    32,    33,    60,    55,    71,
      71,    56,    73,    27,    32,    32,    33,    32,    33,    32,
      32,    32,    33,    36,    31,    32,    33,    32,    32,    33,
      27,    60,    63,    65,    24,    25,    67,    70,    72,    74,
      27,    49,    50,    68,    69,    32,    33,    55,    56,    31,
      31,    32,    67,    60,    61,    33,    32,    60,    32
};

#if ! defined (FDO_FGFT_YYSIZE_T) && defined (__SIZE_TYPE__)
# define FDO_FGFT_YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (FDO_FGFT_YYSIZE_T) && defined (size_t)
# define FDO_FGFT_YYSIZE_T size_t
#endif
#if ! defined (FDO_FGFT_YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define FDO_FGFT_YYSIZE_T size_t
# endif
#endif
#if ! defined (FDO_FGFT_YYSIZE_T)
# define FDO_FGFT_YYSIZE_T unsigned int
#endif

#define fdo_fgft_yyerrok		(fdo_fgft_yyerrstatus = 0)
#define fdo_fgft_yyclearin	(pParse->fdo_fgft_yychar = FDO_FGFT_YYEMPTY)
#define FDO_FGFT_YYEMPTY		(-2)
#define FDO_FGFT_YYEOF		0

#define FDO_FGFT_YYACCEPT	goto fdo_fgft_yyacceptlab
#define FDO_FGFT_YYABORT		goto fdo_fgft_yyabortlab
#define FDO_FGFT_YYERROR		goto fdo_fgft_yyerrlab1

/* Like FDO_FGFT_YYERROR except do call fdo_fgft_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of FDO_FGFT_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define FDO_FGFT_YYFAIL		goto fdo_fgft_yyerrlab

#define FDO_FGFT_YYRECOVERING()  (!!fdo_fgft_yyerrstatus)

#define FDO_FGFT_YYBACKUP(Token, Value)					\
do								\
  if (pParse->fdo_fgft_yychar == FDO_FGFT_YYEMPTY && fdo_fgft_yylen == 1)				\
    {								\
      pParse->fdo_fgft_yychar = (Token);						\
      pParse->fdo_fgft_yylval = (Value);						\
      fdo_fgft_yytoken = FDO_FGFT_YYTRANSLATE (pParse->fdo_fgft_yychar);				\
      FDO_FGFT_YYPOPSTACK;						\
      goto fdo_fgft_yybackup;						\
    }								\
  else								\
    { 								\
      fdo_fgft_yyerror(pParse,"syntax error: cannot back up");\
      FDO_FGFT_YYERROR;							\
    }								\
while (0)

#define FDO_FGFT_YYTERROR	1
#define FDO_FGFT_YYERRCODE	256

/* FDO_FGFT_YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef FDO_FGFT_YYLLOC_DEFAULT
# define FDO_FGFT_YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* FDO_FGFT_YYLEX -- calling `fdo_fgft_yylex' with the right arguments.  */

#ifdef FDO_FGFT_YYLEX_PARAM
# define FDO_FGFT_YYLEX fdo_fgft_yylex (FDO_FGFT_YYLEX_PARAM)
#else
# define FDO_FGFT_YYLEX fdo_fgft_yylex(pParse)
#endif

/* Enable debugging if requested.  */
#if FDO_FGFT_YYDEBUG

# ifndef FDO_FGFT_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define FDO_FGFT_YYFPRINTF fprintf
# endif

# define FDO_FGFT_YYDPRINTF(Args)			\
do {						\
  if (pParse->fdo_fgft_yydebug)					\
    FDO_FGFT_YYFPRINTF Args;				\
} while (0)

# define FDO_FGFT_YYDSYMPRINT(Args)			\
do {						\
  if (pParse->fdo_fgft_yydebug)					\
    fdo_fgft_yysymprint Args;				\
} while (0)

# define FDO_FGFT_YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (pParse->fdo_fgft_yydebug)							\
    {								\
      FDO_FGFT_YYFPRINTF (stderr, "%s ", Title);				\
      fdo_fgft_yysymprint (stderr, 					\
                  Token, Value);	\
      FDO_FGFT_YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| fdo_fgft_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_fgft_yy_stack_print (short *bottom, short *top)
#else
static void
fdo_fgft_yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  FDO_FGFT_YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    FDO_FGFT_YYFPRINTF (stderr, " %d", *bottom);
  FDO_FGFT_YYFPRINTF (stderr, "\n");
}

# define FDO_FGFT_YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (pParse->fdo_fgft_yydebug)							\
    fdo_fgft_yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the FDO_FGFT_YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_fgft_yy_reduce_print (int fdo_fgft_yyrule)
#else
static void
fdo_fgft_yy_reduce_print (fdo_fgft_yyrule)
    int fdo_fgft_yyrule;
#endif
{
  int fdo_fgft_yyi;
  unsigned int pParse->fdo_fgft_yylineno = fdo_fgft_yyrline[fdo_fgft_yyrule];
  FDO_FGFT_YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             fdo_fgft_yyrule - 1, pParse->fdo_fgft_yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (fdo_fgft_yyi = fdo_fgft_yyprhs[fdo_fgft_yyrule]; 0 <= fdo_fgft_yyrhs[fdo_fgft_yyi]; fdo_fgft_yyi++)
    FDO_FGFT_YYFPRINTF (stderr, "%s ", fdo_fgft_yytname [fdo_fgft_yyrhs[fdo_fgft_yyi]]);
  FDO_FGFT_YYFPRINTF (stderr, "-> %s\n", fdo_fgft_yytname [fdo_fgft_yyr1[fdo_fgft_yyrule]]);
}

# define FDO_FGFT_YY_REDUCE_PRINT(Rule)		\
do {					\
  if (pParse->fdo_fgft_yydebug)				\
    fdo_fgft_yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
#else /* !FDO_FGFT_YYDEBUG */
# define FDO_FGFT_YYDPRINTF(Args)
# define FDO_FGFT_YYDSYMPRINT(Args)
# define FDO_FGFT_YYDSYMPRINTF(Title, Token, Value, Location)
# define FDO_FGFT_YY_STACK_PRINT(Bottom, Top)
# define FDO_FGFT_YY_REDUCE_PRINT(Rule)
#endif /* !FDO_FGFT_YYDEBUG */


/* FDO_FGFT_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	FDO_FGFT_YYINITDEPTH
# define FDO_FGFT_YYINITDEPTH 200
#endif

/* FDO_FGFT_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < FDO_FGFT_YYSTACK_BYTES (FDO_FGFT_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if FDO_FGFT_YYMAXDEPTH == 0
# undef FDO_FGFT_YYMAXDEPTH
#endif

#ifndef FDO_FGFT_YYMAXDEPTH
# define FDO_FGFT_YYMAXDEPTH 10000
#endif



#if FDO_FGFT_YYERROR_VERBOSE

# ifndef fdo_fgft_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define fdo_fgft_yystrlen strlen
#  else
/* Return the length of FDO_FGFT_YYSTR.  */
static FDO_FGFT_YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
fdo_fgft_yystrlen (const char *fdo_fgft_yystr)
#   else
fdo_fgft_yystrlen (fdo_fgft_yystr)
     const char *fdo_fgft_yystr;
#   endif
{
  register const char *fdo_fgft_yys = fdo_fgft_yystr;

  while (*fdo_fgft_yys++ != '\0')
    continue;

  return fdo_fgft_yys - fdo_fgft_yystr - 1;
}
#  endif
# endif

# ifndef fdo_fgft_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define fdo_fgft_yystpcpy stpcpy
#  else
/* Copy FDO_FGFT_YYSRC to FDO_FGFT_YYDEST, returning the address of the terminating '\0' in
   FDO_FGFT_YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
fdo_fgft_yystpcpy (char *fdo_fgft_yydest, const char *fdo_fgft_yysrc)
#   else
fdo_fgft_yystpcpy (fdo_fgft_yydest, fdo_fgft_yysrc)
     char *fdo_fgft_yydest;
     const char *fdo_fgft_yysrc;
#   endif
{
  register char *fdo_fgft_yyd = fdo_fgft_yydest;
  register const char *fdo_fgft_yys = fdo_fgft_yysrc;

  while ((*fdo_fgft_yyd++ = *fdo_fgft_yys++) != '\0')
    continue;

  return fdo_fgft_yyd - 1;
}
#  endif
# endif

#endif /* !FDO_FGFT_YYERROR_VERBOSE */



#if FDO_FGFT_YYDEBUG
/*--------------------------------.
| Print this symbol on FDO_FGFT_YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_fgft_yysymprint (FILE *fdo_fgft_yyoutput, int fdo_fgft_yytype, FDO_FGFT_YYSTYPE *fdo_fgft_yyvaluep)
#else
static void
fdo_fgft_yysymprint (fdo_fgft_yyoutput, fdo_fgft_yytype, fdo_fgft_yyvaluep)
    FILE *fdo_fgft_yyoutput;
    int fdo_fgft_yytype;
    FDO_FGFT_YYSTYPE *fdo_fgft_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) fdo_fgft_yyvaluep;

  if (fdo_fgft_yytype < FDO_FGFT_YYNTOKENS)
    {
      FDO_FGFT_YYFPRINTF (fdo_fgft_yyoutput, "token %s (", fdo_fgft_yytname[fdo_fgft_yytype]);
# ifdef FDO_FGFT_YYPRINT
      FDO_FGFT_YYPRINT (fdo_fgft_yyoutput, fdo_fgft_yytoknum[fdo_fgft_yytype], *fdo_fgft_yyvaluep);
# endif
    }
  else
    FDO_FGFT_YYFPRINTF (fdo_fgft_yyoutput, "nterm %s (", fdo_fgft_yytname[fdo_fgft_yytype]);

  switch (fdo_fgft_yytype)
    {
      default:
        break;
    }
  FDO_FGFT_YYFPRINTF (fdo_fgft_yyoutput, ")");
}

#endif /* ! FDO_FGFT_YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
fdo_fgft_yydestruct (int fdo_fgft_yytype, FDO_FGFT_YYSTYPE *fdo_fgft_yyvaluep)
#else
static void
fdo_fgft_yydestruct (fdo_fgft_yytype, fdo_fgft_yyvaluep)
    int fdo_fgft_yytype;
    FDO_FGFT_YYSTYPE *fdo_fgft_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) fdo_fgft_yyvaluep;

  switch (fdo_fgft_yytype)
    {
      case 0:
      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef FDO_FGFT_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int fdo_fgft_yyparse (void *FDO_FGFT_YYPARSE_PARAM);
# else
int fdo_fgft_yyparse(FdoParseFgft* pParse);
# endif
#else /* ! FDO_FGFT_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int fdo_fgft_yyparse(FdoParseFgft* pParse);
#else
int fdo_fgft_yyparse(FdoParseFgft* pParse);
#endif
#endif /* ! FDO_FGFT_YYPARSE_PARAM */



/* The lookahead symbol.  */

/* The semantic value of the lookahead symbol.  */

/* Number of syntax errors so far.  */



/*----------.
| fdo_fgft_yyparse.  |
`----------*/

#ifdef FDO_FGFT_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int fdo_fgft_yyparse (void *FDO_FGFT_YYPARSE_PARAM)
# else
int fdo_fgft_yyparse (FDO_FGFT_YYPARSE_PARAM)
  void *FDO_FGFT_YYPARSE_PARAM;
# endif
#else /* ! FDO_FGFT_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
fdo_fgft_yyparse(FdoParseFgft* pParse)
#else
int
fdo_fgft_yyparse(FdoParseFgft* pParse)

#endif
#endif
{
  
  register int fdo_fgft_yystate;
  register int fdo_fgft_yyn;
  int fdo_fgft_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int fdo_fgft_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int fdo_fgft_yytoken = 0;

  /* Three stacks and their tools:
     `fdo_fgft_yyss': related to states,
     `fdo_fgft_yyvs': related to semantic values,
     `fdo_fgft_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow fdo_fgft_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short *fdo_fgft_yyss = pParse->fdo_fgft_yyss;

  /* The semantic value stack.  */
  FDO_FGFT_YYSTYPE *fdo_fgft_yyvs = pParse->fdo_fgft_yyvs;



#define FDO_FGFT_YYPOPSTACK   (pParse->fdo_fgft_yyvsp--, pParse->fdo_fgft_yyssp--)

  FDO_FGFT_YYSIZE_T fdo_fgft_yystacksize = FDO_FGFT_YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int fdo_fgft_yylen;

  FDO_FGFT_YYDPRINTF ((stderr, "Starting parse\n"));

  fdo_fgft_yystate = 0;
  fdo_fgft_yyerrstatus = 0;
  pParse->fdo_fgft_yynerrs = 0;
  pParse->fdo_fgft_yychar = FDO_FGFT_YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  pParse->fdo_fgft_yyssp = fdo_fgft_yyss;
  pParse->fdo_fgft_yyvsp = fdo_fgft_yyvs;

  goto fdo_fgft_yysetstate;

/*------------------------------------------------------------.
| fdo_fgft_yynewstate -- Push a new state, which is found in fdo_fgft_yystate.  |
`------------------------------------------------------------*/
 fdo_fgft_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  pParse->fdo_fgft_yyssp++;

 fdo_fgft_yysetstate:
  *pParse->fdo_fgft_yyssp = fdo_fgft_yystate;

  if (fdo_fgft_yyss + fdo_fgft_yystacksize - 1 <= pParse->fdo_fgft_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      FDO_FGFT_YYSIZE_T fdo_fgft_yysize = pParse->fdo_fgft_yyssp - fdo_fgft_yyss + 1;

#ifdef fdo_fgft_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	FDO_FGFT_YYSTYPE *fdo_fgft_yyvs1 = fdo_fgft_yyvs;
	short *fdo_fgft_yyss1 = fdo_fgft_yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if fdo_fgft_yyoverflow is a macro.  */
	fdo_fgft_yyoverflow ("parser stack overflow",
		    &fdo_fgft_yyss1, fdo_fgft_yysize * sizeof (*pParse->fdo_fgft_yyssp),
		    &fdo_fgft_yyvs1, fdo_fgft_yysize * sizeof (*pParse->fdo_fgft_yyvsp),

		    &fdo_fgft_yystacksize);

	fdo_fgft_yyss = fdo_fgft_yyss1;
	fdo_fgft_yyvs = fdo_fgft_yyvs1;
      }
#else /* no fdo_fgft_yyoverflow */
# ifndef FDO_FGFT_YYSTACK_RELOCATE
      goto fdo_fgft_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (FDO_FGFT_YYMAXDEPTH <= fdo_fgft_yystacksize)
	goto fdo_fgft_yyoverflowlab;
      fdo_fgft_yystacksize *= 2;
      if (FDO_FGFT_YYMAXDEPTH < fdo_fgft_yystacksize)
	fdo_fgft_yystacksize = FDO_FGFT_YYMAXDEPTH;

      {
	short *fdo_fgft_yyss1 = fdo_fgft_yyss;
	union fdo_fgft_yyalloc *fdo_fgft_yyptr =
	  (union fdo_fgft_yyalloc *) FDO_FGFT_YYSTACK_ALLOC (FDO_FGFT_YYSTACK_BYTES (fdo_fgft_yystacksize));
	if (! fdo_fgft_yyptr)
	  goto fdo_fgft_yyoverflowlab;
	FDO_FGFT_YYSTACK_RELOCATE (fdo_fgft_yyss);
	FDO_FGFT_YYSTACK_RELOCATE (fdo_fgft_yyvs);

#  undef FDO_FGFT_YYSTACK_RELOCATE
	if (fdo_fgft_yyss1 != pParse->fdo_fgft_yyss)
	  FDO_FGFT_YYSTACK_FREE (fdo_fgft_yyss1);
      }
# endif
#endif /* no fdo_fgft_yyoverflow */

      pParse->fdo_fgft_yyssp = fdo_fgft_yyss + fdo_fgft_yysize - 1;
      pParse->fdo_fgft_yyvsp = fdo_fgft_yyvs + fdo_fgft_yysize - 1;


      FDO_FGFT_YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) fdo_fgft_yystacksize));

      if (fdo_fgft_yyss + fdo_fgft_yystacksize - 1 <= pParse->fdo_fgft_yyssp)
	FDO_FGFT_YYABORT;
    }

  FDO_FGFT_YYDPRINTF ((stderr, "Entering state %d\n", fdo_fgft_yystate));

  goto fdo_fgft_yybackup;

/*-----------.
| fdo_fgft_yybackup.  |
`-----------*/
fdo_fgft_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* fdo_fgft_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  fdo_fgft_yyn = fdo_fgft_yypact[fdo_fgft_yystate];
  if (fdo_fgft_yyn == FDO_FGFT_YYPACT_NINF)
    goto fdo_fgft_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* FDO_FGFT_YYCHAR is either FDO_FGFT_YYEMPTY or FDO_FGFT_YYEOF or a valid lookahead symbol.  */
  if (pParse->fdo_fgft_yychar == FDO_FGFT_YYEMPTY)
    {
      FDO_FGFT_YYDPRINTF ((stderr, "Reading a token: "));
      pParse->fdo_fgft_yychar = FDO_FGFT_YYLEX;
    }

  if (pParse->fdo_fgft_yychar <= FDO_FGFT_YYEOF)
    {
      pParse->fdo_fgft_yychar = fdo_fgft_yytoken = FDO_FGFT_YYEOF;
      FDO_FGFT_YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      fdo_fgft_yytoken = FDO_FGFT_YYTRANSLATE (pParse->fdo_fgft_yychar);
      FDO_FGFT_YYDSYMPRINTF ("Next token is", fdo_fgft_yytoken, &pParse->fdo_fgft_yylval, &fdo_fgft_yylloc);
    }

  /* If the proper action on seeing token FDO_FGFT_YYTOKEN is to reduce or to
     detect an error, take that action.  */
  fdo_fgft_yyn += fdo_fgft_yytoken;
  if (fdo_fgft_yyn < 0 || FDO_FGFT_YYLAST < fdo_fgft_yyn || fdo_fgft_yycheck[fdo_fgft_yyn] != fdo_fgft_yytoken)
    goto fdo_fgft_yydefault;
  fdo_fgft_yyn = fdo_fgft_yytable[fdo_fgft_yyn];
  if (fdo_fgft_yyn <= 0)
    {
      if (fdo_fgft_yyn == 0 || fdo_fgft_yyn == FDO_FGFT_YYTABLE_NINF)
	goto fdo_fgft_yyerrlab;
      fdo_fgft_yyn = -fdo_fgft_yyn;
      goto fdo_fgft_yyreduce;
    }

  if (fdo_fgft_yyn == FDO_FGFT_YYFINAL)
    FDO_FGFT_YYACCEPT;

  /* Shift the lookahead token.  */
  FDO_FGFT_YYDPRINTF ((stderr, "Shifting token %s, ", fdo_fgft_yytname[fdo_fgft_yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (pParse->fdo_fgft_yychar != FDO_FGFT_YYEOF)
    pParse->fdo_fgft_yychar = FDO_FGFT_YYEMPTY;

  *++pParse->fdo_fgft_yyvsp = pParse->fdo_fgft_yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (fdo_fgft_yyerrstatus)
    fdo_fgft_yyerrstatus--;

  fdo_fgft_yystate = fdo_fgft_yyn;
  goto fdo_fgft_yynewstate;


/*-----------------------------------------------------------.
| fdo_fgft_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
fdo_fgft_yydefault:
  fdo_fgft_yyn = fdo_fgft_yydefact[fdo_fgft_yystate];
  if (fdo_fgft_yyn == 0)
    goto fdo_fgft_yyerrlab;
  goto fdo_fgft_yyreduce;


/*-----------------------------.
| fdo_fgft_yyreduce -- Do a reduction.  |
`-----------------------------*/
fdo_fgft_yyreduce:
  /* fdo_fgft_yyn is the number of a rule to reduce with.  */
  fdo_fgft_yylen = fdo_fgft_yyr2[fdo_fgft_yyn];

  /* If FDO_FGFT_YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets FDO_FGFT_YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to FDO_FGFT_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that FDO_FGFT_YYVAL may be used uninitialized.  */
  pParse->fdo_fgft_yyval = pParse->fdo_fgft_yyvsp[1-fdo_fgft_yylen];


  FDO_FGFT_YY_REDUCE_PRINT (fdo_fgft_yyn);
  switch (fdo_fgft_yyn)
    {
        case 2:

    {pParse->Done(); FDO_FGFT_YYACCEPT;}
    break;

  case 3:

    {pParse->Error(); FDO_FGFT_YYABORT;}
    break;

  case 4:

    {pParse->SetGeometryType(FdoToken_POINT);}
    break;

  case 6:

    {pParse->SetGeometryType(FdoToken_LINESTRING);}
    break;

  case 8:

    {pParse->SetGeometryType(FdoToken_POLYGON);}
    break;

  case 10:

    {pParse->SetGeometryType(FdoToken_CURVESTRING);}
    break;

  case 12:

    {pParse->SetGeometryType(FdoToken_CURVEPOLYGON);}
    break;

  case 14:

    {pParse->SetGeometryType(FdoToken_MULTIPOINT);}
    break;

  case 16:

    {pParse->SetGeometryType(FdoToken_MULTILINESTRING);}
    break;

  case 18:

    {pParse->SetGeometryType(FdoToken_MULTIPOLYGON);}
    break;

  case 20:

    {pParse->SetGeometryType(FdoToken_MULTICURVESTRING);}
    break;

  case 22:

    {pParse->SetGeometryType(FdoToken_MULTICURVEPOLYGON);}
    break;

  case 24:

    {pParse->SetGeometryType(FdoToken_GEOMETRYCOLLECTION);}
    break;

  case 26:

    {Node_Trace(L"PointEntity\n");}
    break;

  case 27:

    {Node_Trace(L"LineString\n");}
    break;

  case 28:

    {Node_Trace(L"Polygon\n");}
    break;

  case 29:

    {Node_Trace(L"MultiPoint\n");}
    break;

  case 30:

    {Node_Trace(L"MultiLineString\n");}
    break;

  case 31:

    {Node_Trace(L"MultiPolygon\n");}
    break;

  case 32:

    {Node_Trace(L"GeometryCollection\n");}
    break;

  case 33:

    {Node_Trace(L"CurveString\n");}
    break;

  case 34:

    {Node_Trace(L"CurvePolygon\n");}
    break;

  case 35:

    {Node_Trace(L"MultiCurveString\n");}
    break;

  case 36:

    {Node_Trace(L"MultiCurvePolygon\n");}
    break;

  case 37:

    {pParse->SetDimensionality(-1);}
    break;

  case 38:

    {pParse->SetDimensionality(FdoToken_XY);}
    break;

  case 39:

    {pParse->SetDimensionality(FdoToken_XYZ);}
    break;

  case 40:

    {pParse->SetDimensionality(FdoToken_XYM);}
    break;

  case 41:

    {pParse->SetDimensionality(FdoToken_XYZM);}
    break;

  case 42:

    {pParse->AddPoint(pParse->fdo_fgft_yyvsp[-1].m_double, pParse->fdo_fgft_yyvsp[0].m_double);}
    break;

  case 43:

    {pParse->AddPoint(pParse->fdo_fgft_yyvsp[-2].m_double, pParse->fdo_fgft_yyvsp[-1].m_double, pParse->fdo_fgft_yyvsp[0].m_double);}
    break;

  case 44:

    {pParse->AddPoint(pParse->fdo_fgft_yyvsp[-3].m_double, pParse->fdo_fgft_yyvsp[-2].m_double, pParse->fdo_fgft_yyvsp[-1].m_double, pParse->fdo_fgft_yyvsp[0].m_double);}
    break;

  case 45:

    {Node_Trace(L"PointCollection[1]\n");}
    break;

  case 46:

    {Node_Trace(L"PointCollection[n]\n");}
    break;

  case 47:

    {Node_Trace(L"LineStringCollection[1]\n");}
    break;

  case 48:

    {pParse->SetBreak();}
    break;

  case 49:

    {Node_Trace(L"LineStringCollection[n]\n");}
    break;

  case 50:

    {Node_Trace(L"PolygonCollection[1]\n");}
    break;

  case 51:

    {pParse->SetOuterBreak();}
    break;

  case 52:

    {Node_Trace(L"PolygonCollection[n]\n");}
    break;

  case 53:

    {Node_Trace(L"FgftCollection[1]\n");}
    break;

  case 54:

    {Node_Trace(L"FgftCollection[n]\n");}
    break;

  case 55:

    {pParse->SetSegmentType(FdoToken_CIRCULARARCSEGMENT);}
    break;

  case 57:

    {pParse->SetSegmentType(FdoToken_LINESTRINGSEGMENT);}
    break;

  case 59:

    {Node_Trace(L"CurveSegmentCollection[1]\n");}
    break;

  case 60:

    {Node_Trace(L"CurveSegmentCollection[n]\n");}
    break;

  case 61:

    {Node_Trace(L"CurveStringCollection[1]\n");}
    break;

  case 62:

    {pParse->SetBreak();}
    break;

  case 63:

    {Node_Trace(L"CurveStringCollection[n]\n");}
    break;

  case 64:

    {Node_Trace(L"CurvePolygonCollection[1]\n");}
    break;

  case 65:

    {pParse->SetOuterBreak();}
    break;

  case 66:

    {Node_Trace(L"CurvePolygonCollection[n]\n");}
    break;


    }

/* Line 991 of yacc.c.  */


  pParse->fdo_fgft_yyvsp -= fdo_fgft_yylen;
  pParse->fdo_fgft_yyssp -= fdo_fgft_yylen;


  FDO_FGFT_YY_STACK_PRINT (fdo_fgft_yyss, pParse->fdo_fgft_yyssp);

  *++pParse->fdo_fgft_yyvsp = pParse->fdo_fgft_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  fdo_fgft_yyn = fdo_fgft_yyr1[fdo_fgft_yyn];

  fdo_fgft_yystate = fdo_fgft_yypgoto[fdo_fgft_yyn - FDO_FGFT_YYNTOKENS] + *pParse->fdo_fgft_yyssp;
  if (0 <= fdo_fgft_yystate && fdo_fgft_yystate <= FDO_FGFT_YYLAST && fdo_fgft_yycheck[fdo_fgft_yystate] == *pParse->fdo_fgft_yyssp)
    fdo_fgft_yystate = fdo_fgft_yytable[fdo_fgft_yystate];
  else
    fdo_fgft_yystate = fdo_fgft_yydefgoto[fdo_fgft_yyn - FDO_FGFT_YYNTOKENS];

  goto fdo_fgft_yynewstate;


/*------------------------------------.
| fdo_fgft_yyerrlab -- here on detecting error |
`------------------------------------*/
fdo_fgft_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!fdo_fgft_yyerrstatus)
    {
      ++pParse->fdo_fgft_yynerrs;
#if FDO_FGFT_YYERROR_VERBOSE
      fdo_fgft_yyn = fdo_fgft_yypact[fdo_fgft_yystate];

      if (FDO_FGFT_YYPACT_NINF < fdo_fgft_yyn && fdo_fgft_yyn < FDO_FGFT_YYLAST)
	{
	  FDO_FGFT_YYSIZE_T fdo_fgft_yysize = 0;
	  int fdo_fgft_yytype = FDO_FGFT_YYTRANSLATE (pParse->fdo_fgft_yychar);
	  char *fdo_fgft_yymsg;
	  int fdo_fgft_yyx, fdo_fgft_yycount;

	  fdo_fgft_yycount = 0;
	  /* Start FDO_FGFT_YYX at -FDO_FGFT_YYN if negative to avoid negative indexes in
	     FDO_FGFT_YYCHECK.  */
	  for (fdo_fgft_yyx = fdo_fgft_yyn < 0 ? -fdo_fgft_yyn : 0;
	       fdo_fgft_yyx < (int) (sizeof (fdo_fgft_yytname) / sizeof (char *)); fdo_fgft_yyx++)
	    if (fdo_fgft_yycheck[fdo_fgft_yyx + fdo_fgft_yyn] == fdo_fgft_yyx && fdo_fgft_yyx != FDO_FGFT_YYTERROR)
	      fdo_fgft_yysize += fdo_fgft_yystrlen (fdo_fgft_yytname[fdo_fgft_yyx]) + 15, fdo_fgft_yycount++;
	  fdo_fgft_yysize += fdo_fgft_yystrlen ("syntax error, unexpected ") + 1;
	  fdo_fgft_yysize += fdo_fgft_yystrlen (fdo_fgft_yytname[fdo_fgft_yytype]);
	  fdo_fgft_yymsg = (char *) FDO_FGFT_YYSTACK_ALLOC (fdo_fgft_yysize);
	  if (fdo_fgft_yymsg != 0)
	    {
	      char *fdo_fgft_yyp = fdo_fgft_yystpcpy (fdo_fgft_yymsg, "syntax error, unexpected ");
	      fdo_fgft_yyp = fdo_fgft_yystpcpy (fdo_fgft_yyp, fdo_fgft_yytname[fdo_fgft_yytype]);

	      if (fdo_fgft_yycount < 5)
		{
		  fdo_fgft_yycount = 0;
		  for (fdo_fgft_yyx = fdo_fgft_yyn < 0 ? -fdo_fgft_yyn : 0;
		       fdo_fgft_yyx < (int) (sizeof (fdo_fgft_yytname) / sizeof (char *));
		       fdo_fgft_yyx++)
		    if (fdo_fgft_yycheck[fdo_fgft_yyx + fdo_fgft_yyn] == fdo_fgft_yyx && fdo_fgft_yyx != FDO_FGFT_YYTERROR)
		      {
			const char *fdo_fgft_yyq = ! fdo_fgft_yycount ? ", expecting " : " or ";
			fdo_fgft_yyp = fdo_fgft_yystpcpy (fdo_fgft_yyp, fdo_fgft_yyq);
			fdo_fgft_yyp = fdo_fgft_yystpcpy (fdo_fgft_yyp, fdo_fgft_yytname[fdo_fgft_yyx]);
			fdo_fgft_yycount++;
		      }
		}
	      fdo_fgft_yyerror(pParse,fdo_fgft_yymsg);
	      FDO_FGFT_YYSTACK_FREE (fdo_fgft_yymsg);
	    }
	  else
	    fdo_fgft_yyerror(pParse,"syntax error; also virtual memory exhausted");
	}
      else
#endif /* FDO_FGFT_YYERROR_VERBOSE */
	fdo_fgft_yyerror(pParse,"syntax error");
    }



  if (fdo_fgft_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (pParse->fdo_fgft_yychar == FDO_FGFT_YYEOF)
        {
	  /* Pop the error token.  */
          FDO_FGFT_YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (fdo_fgft_yyss < pParse->fdo_fgft_yyssp)
	    {
	      FDO_FGFT_YYDSYMPRINTF ("Error: popping", fdo_fgft_yystos[*pParse->fdo_fgft_yyssp], pParse->fdo_fgft_yyvsp, fdo_fgft_yylsp);
	      fdo_fgft_yydestruct (fdo_fgft_yystos[*pParse->fdo_fgft_yyssp], pParse->fdo_fgft_yyvsp);
	      FDO_FGFT_YYPOPSTACK;
	    }
	  FDO_FGFT_YYABORT;
        }

      FDO_FGFT_YYDSYMPRINTF ("Error: discarding", fdo_fgft_yytoken, &pParse->fdo_fgft_yylval, &fdo_fgft_yylloc);
      fdo_fgft_yydestruct (fdo_fgft_yytoken, &pParse->fdo_fgft_yylval);
      pParse->fdo_fgft_yychar = FDO_FGFT_YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto fdo_fgft_yyerrlab2;


/*----------------------------------------------------.
| fdo_fgft_yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
fdo_fgft_yyerrlab1:

  /* Suppress GCC warning that fdo_fgft_yyerrlab1 is unused when no action
     invokes FDO_FGFT_YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__))
#endif


  goto fdo_fgft_yyerrlab2;


/*---------------------------------------------------------------.
| fdo_fgft_yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
fdo_fgft_yyerrlab2:
  fdo_fgft_yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      fdo_fgft_yyn = fdo_fgft_yypact[fdo_fgft_yystate];
      if (fdo_fgft_yyn != FDO_FGFT_YYPACT_NINF)
	{
	  fdo_fgft_yyn += FDO_FGFT_YYTERROR;
	  if (0 <= fdo_fgft_yyn && fdo_fgft_yyn <= FDO_FGFT_YYLAST && fdo_fgft_yycheck[fdo_fgft_yyn] == FDO_FGFT_YYTERROR)
	    {
	      fdo_fgft_yyn = fdo_fgft_yytable[fdo_fgft_yyn];
	      if (0 < fdo_fgft_yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (pParse->fdo_fgft_yyssp == fdo_fgft_yyss)
	FDO_FGFT_YYABORT;

      FDO_FGFT_YYDSYMPRINTF ("Error: popping", fdo_fgft_yystos[*pParse->fdo_fgft_yyssp], pParse->fdo_fgft_yyvsp, fdo_fgft_yylsp);
      fdo_fgft_yydestruct (fdo_fgft_yystos[fdo_fgft_yystate], pParse->fdo_fgft_yyvsp);
      pParse->fdo_fgft_yyvsp--;
      fdo_fgft_yystate = *--pParse->fdo_fgft_yyssp;

      FDO_FGFT_YY_STACK_PRINT (fdo_fgft_yyss, pParse->fdo_fgft_yyssp);
    }

  if (fdo_fgft_yyn == FDO_FGFT_YYFINAL)
    FDO_FGFT_YYACCEPT;

  FDO_FGFT_YYDPRINTF ((stderr, "Shifting error token, "));

  *++pParse->fdo_fgft_yyvsp = pParse->fdo_fgft_yylval;


  fdo_fgft_yystate = fdo_fgft_yyn;
  goto fdo_fgft_yynewstate;


/*-------------------------------------.
| fdo_fgft_yyacceptlab -- FDO_FGFT_YYACCEPT comes here.  |
`-------------------------------------*/
fdo_fgft_yyacceptlab:
  fdo_fgft_yyresult = 0;
  goto fdo_fgft_yyreturn;

/*-----------------------------------.
| fdo_fgft_yyabortlab -- FDO_FGFT_YYABORT comes here.  |
`-----------------------------------*/
fdo_fgft_yyabortlab:
  fdo_fgft_yyresult = 1;
  goto fdo_fgft_yyreturn;

#ifndef fdo_fgft_yyoverflow
/*----------------------------------------------.
| fdo_fgft_yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
fdo_fgft_yyoverflowlab:
  fdo_fgft_yyerror(pParse,"parser stack overflow");
  fdo_fgft_yyresult = 2;
  /* Fall through.  */
#endif

fdo_fgft_yyreturn:
#ifndef fdo_fgft_yyoverflow
  if (fdo_fgft_yyss != pParse->fdo_fgft_yyss)
    FDO_FGFT_YYSTACK_FREE (fdo_fgft_yyss);
#endif
  return fdo_fgft_yyresult;
}




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

