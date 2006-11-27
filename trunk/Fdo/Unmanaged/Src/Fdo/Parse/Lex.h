// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
// Note: used SED substitution to fix getenv() prototype
// if you want tracing enabled you need to set environment
// variables of these names with a value of 1 to 9
//#define FDO_FILTER_YYDEBUG		1
//#define FDO_EXPRESSION_YYDEBUG	1

#include "Parse.h"
class FdoParse;
#ifndef NOTOKEN_H
// include both 1) exposes both fdo_*_yylval unions and 2) guarantees
// that tokens align, because will get compile redefinition errors if bad
#ifdef _WIN32
#include "yyExpressionWin.h"
#include "yyFilterWin.h"
#else
#include "yyExpression.h"
#include "yyFilter.h"
#endif
#endif

#ifdef _DEBUG
//#define DEBUG_TRACELEX	1
//#define DEBUG_TRACETREE	1
#endif

typedef enum FdoToken 
{
  // bad stuff/processing...
  FdoToken_Undefined = -3,     // Token is undefined
  FdoToken_End = -2,           // End of string 
  FdoToken_Start = -1,         // Start of line 
  FdoToken_Error = 0,          // Lexical analyzer error or erroneous token

  // data values
  FdoToken_Literal = 1         // Literal

} FdoToken;

// FDO keywords
typedef struct _FdoKeyWord
{
	FdoString*	word;
	FdoInt32	token;
} FdoKeyWord;

// The FdoLex class provides lexical analysis support for Expression and
// Filter parsing, i.e. accepts data from the input stream, recognizes tokens 
// in it and outputs their type together with appropriate attributes.
class FdoLex
{
// was 256 in Object_Data2; the max text literal length in SQL is 4000 bytes
static const FdoInt32 maxCharLength = 4000;
static const FdoInt32 maxBinaryLength = 256*8;

  public:
	// must contruct class with string to parse.
	explicit    FdoLex(FdoParse *, FdoString*);  

    virtual		~FdoLex(void);

    // Read next token and its attributes
    FdoInt32		GetToken(FdoParse *);

	// find keyword in table
	FdoInt32		FindKeyWord(FdoString*, FdoKeyWord*, FdoInt32);

    //  Public data
    FdoInt32		m_token;			// Token type 
    FdoInt32		m_lastToken;		// Predecessor
	FdoDataValue*	m_data;				// token data
    FdoInt32		m_cprev;            // Start of the token
#if _DEBUG
	void			PrintToken(FdoInt32 token);
#endif

private:
    const wchar_t	nextchar		(void) const;  // Next character 
    const wchar_t	prevchar		(void) const;  // Previous character
    const wchar_t	if_getch		(FdoParse *);        // Read next character
    const wchar_t	find_nonblank	(FdoParse *pParse);        // Get next not blank symbol

    void			getword			(FdoParse *, wchar_t*, FdoInt32);   // Get keyword or identifier from input

    wchar_t*		getdigits		(FdoParse *pParse, wchar_t*);  	// Get digits 
    FdoUInt32		get_unsigned	(FdoParse *pParse);			// Get unsigned integer number
    bool			getnumber		(FdoParse *pParse, bool);			// Get number 
    bool			get_string		(FdoParse *pParse, wchar_t*&, wchar_t);	// Get character string; caller is responsible for freeing buffer

    bool			get_date		(FdoParse *pParse, FdoUInt16 *, FdoUInt16 *, FdoUInt16 *);  
    bool			get_second		(FdoParse *pParse, double *);
    bool			get_timevalue	(FdoParse *pParse, FdoUInt16 *, FdoUInt16 *, double *);
    bool			get_time		(FdoParse *pParse, FdoUInt16 *, FdoUInt16 *, double *);
    bool			get_timestamp	(FdoParse *pParse, FdoUInt16 *, FdoUInt16 *, FdoUInt16 *,	FdoUInt16 *, FdoUInt16 *, double *);
    bool			get_bitstring	(FdoParse *pParse);
    bool			get_hexstring	(FdoParse *pParse);

    // Lexical analyzer state
    FdoString*		m_line;			// Points to parsed string
    FdoInt32        m_lineLength;   // cached length of m_line, to avoid scanning string at every if_getch() call
    FdoInt32		m_cc;			// Current position in the input string
    wchar_t			m_ch;			// Current character in the input string
    FdoInt32		m_prevToken;	// Previous token type for internal use
};


