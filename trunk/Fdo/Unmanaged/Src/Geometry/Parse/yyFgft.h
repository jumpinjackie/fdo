#ifndef FDO_FGFT_YYERRCODE
#define FDO_FGFT_YYERRCODE 256
#endif

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
typedef union _FDO_FGFT_YYSTYPE
{
	double		m_double;		/* floating point values */
} FDO_FGFT_YYSTYPE;
