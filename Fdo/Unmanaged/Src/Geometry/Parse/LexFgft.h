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
//#define fdo_fgft_YYDEBUG	1

#ifndef NOTOKEN_H
#ifdef _WIN32
#include "yyFgftWin.h"
#else
#include "yyFgft.h"
#endif
#endif

#ifdef _DEBUG
//#define DEBUG_TRACETOKEN	1
//#define DEBUG_TRACEACTIONS	1
//#define DEBUG_TRACETREE	1
#endif

typedef enum FdoToken 
{
  // bad stuff/processing...
  FdoToken_Undefined = -3,     // Token is undefined
  FdoToken_End = -2,           // End of string 
  FdoToken_Start = -1,         // Start of line 
  FdoToken_Error = 0,          // Lexical analyzer error or erroneous token

  // data values (literals)
  FdoToken_Double = 1,
  FdoToken_Integer

} FdoToken;

// FDO keywords
typedef struct _FdoKeyWord
{
	FdoString*	word;
	FdoInt32	token;
} FdoKeyWord;

// The FdoLexFgft class provides lexical analysis support for FGFT parsing, 
// i.e. accepts data from the input stream, recognizes tokens 
// in it and outputs their type together with appropriate attributes.
class FdoLexFgft
{
static const FdoInt32 maxCharLength = 256;	// only used for keywords or numbers

  public:
	// must contruct class with string to parse.
	explicit	FdoLexFgft(FdoString*);  
    virtual		~FdoLexFgft(void);

    // Read next token and its attributes
    FdoInt32		GetToken(void);

	// find keyword in table
	FdoInt32		FindKeyWord(FdoString*, FdoKeyWord*, FdoInt32);

    //  Public data
    FdoInt32		m_token;			// Token type 
    FdoInt32		m_lastToken;		// Predecessor
	double			m_double;			// token double value
	FdoInt32		m_integer;			// token integer value
    FdoInt32		m_cprev;            // Start of the token
#if _DEBUG
	void			PrintToken(FdoInt32 token);
#endif

private:
    const wchar_t	nextchar		(void) const;  // Next character 
    const wchar_t	prevchar		(void) const;  // Previous character
    const wchar_t	if_getch		(void);        // Read next character
    const wchar_t	find_nonblank	(void);        // Get next not blank symbol

    void			getword			(wchar_t*, FdoInt32);   // Get keyword or identifier from input

    wchar_t*		getdigits		(wchar_t*);  	// Get digits 
    FdoUInt32		get_unsigned	(void);			// Get unsigned integer number
    bool			getnumber		(bool);			// Get number 

    // Lexical analyzer state
    FdoString*		m_line;			// Points to parsed string
    FdoInt32		m_cc;			// Current position in the input string
    wchar_t			m_ch;			// Current character in the input string
    FdoInt32		m_prevToken;	// Previous token type for internal use
};

