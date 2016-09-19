#ifndef FDO_EXPRESSION_YYERRCODE
#define FDO_EXPRESSION_YYERRCODE 256
#endif

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
typedef union _FDO_EXPRESSION_YYSTYPE
{
	FdoIDisposable*	m_node;		/* expression or filter parse tree node*/
	FdoInt32		m_id;		/* enumerations, keywords, ...*/
	FdoInt32		m_integer;	/* integer values (Int8, Int16, Int32 )*/
	FdoInt64		m_int64;	/* 64-bit integer values*/
	double			m_double;	/* floating point values (single(float), double)*/
	FdoString*		m_string;	/* string*/
	_FdoDateTime	m_datetime;	/* date time*/
} FDO_EXPRESSION_YYSTYPE;
