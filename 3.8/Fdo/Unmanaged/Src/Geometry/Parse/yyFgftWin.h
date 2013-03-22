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




#if ! defined (FDO_FGFT_YYSTYPE) && ! defined (FDO_FGFT_YYSTYPE_IS_DECLARED)

typedef union _FDO_FGFT_YYSTYPE
{
	double		m_double;		// floating point values 
} FDO_FGFT_YYSTYPE;
/* Line 1248 of yacc.c.  */

# define fdo_fgft_yystype FDO_FGFT_YYSTYPE /* obsolescent; will be withdrawn */
# define FDO_FGFT_YYSTYPE_IS_DECLARED 1
# define FDO_FGFT_YYSTYPE_IS_TRIVIAL 1
#endif




