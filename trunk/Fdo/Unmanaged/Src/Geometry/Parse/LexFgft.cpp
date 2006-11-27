// 
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
//  
#include "ParseFgft.h"
#include <math.h>	// for pow()
#include <limits.h>	// LONG_MAX and LONG_MIN
#ifndef _WIN32
#include <wctype.h>
#endif
#include <time.h>
#include "StringUtility.h"

#define CHR_NULL	'\0'

//////////////////////////////////////////////////////////////////////////////

static FdoKeyWord g_aFgftWords[] =
{
  L"ARC",					FdoToken_ARC,
  L"BEZIERSEGMENT",			FdoToken_BEZIERSEGMENT,
  L"CIRCLE",				FdoToken_CIRCLE,
  L"CIRCULARARCSEGMENT",	FdoToken_CIRCULARARCSEGMENT,
  L"CURVEPOLYGON",			FdoToken_CURVEPOLYGON,
  L"CURVESTRING",			FdoToken_CURVESTRING,
  L"CUSTOM",				FdoToken_CUSTOM,
  L"GEOMETRYCOLLECTION",	FdoToken_GEOMETRYCOLLECTION,
  L"GRIDTERRAIN",			FdoToken_GRIDTERRAIN,
  L"LINESTRING",			FdoToken_LINESTRING,
  L"LINESTRINGSEGMENT",		FdoToken_LINESTRINGSEGMENT,
  L"MULTICURVEPOLYGON",		FdoToken_MULTICURVEPOLYGON,
  L"MULTICURVESTRING",		FdoToken_MULTICURVESTRING,
  L"MULTILINESTRING",		FdoToken_MULTILINESTRING,
  L"MULTIPOINT",			FdoToken_MULTIPOINT,
  L"MULTIPOLYGON",			FdoToken_MULTIPOLYGON,
  L"POINT",					FdoToken_POINT,
  L"POLYGON",				FdoToken_POLYGON,
  L"RECTANGLE",				FdoToken_RECTANGLE,
  L"TINTERRAIN",			FdoToken_TINTERRAIN,
  L"XY",					FdoToken_XY,
  L"XYM",					FdoToken_XYM,
  L"XYZ",					FdoToken_XYZ,
  L"XYZM",					FdoToken_XYZM,

(FdoString*)0,  FdoToken_Undefined
};
#define FGFTKEYWORDCOUNT   (sizeof(g_aFgftWords)/sizeof(g_aFgftWords[0])-1)

#ifdef _DEBUG
void FdoLexFgft::PrintToken(FdoInt32 token)
{
	printf(" Fgft::");
	switch (token)
	{
	case FdoToken_Undefined:
		printf("Undefined");
		break;
	case FdoToken_End:
		printf("End");
		break;
	case FdoToken_Start:
		printf("Start");
		break;
	case FdoToken_Error:
		printf("!!!!Error!!!! invalid token");
		break;
	case FdoToken_Integer:
		printf("%d (FdoInt32)", m_integer);
		break;
	case FdoToken_Double:
		printf("%g (double)", m_double);
		break;
	case FdoToken_LeftParenthesis:
		printf("LeftParenthesis");
		break;
	case FdoToken_RightParenthesis:
		printf("RightParenthesis");
		break;
	case FdoToken_Comma:
		printf("Comma");
		break;

	default:
		{
			// Keywords
			FdoInt32 i;
			for (i=0; i<FGFTKEYWORDCOUNT; i++)
			{
				if (g_aFgftWords[i].token == token)
				{
					printf("%S (keyword)\n", g_aFgftWords[i].word);
					return;
				}
			}
		}
		printf("UNDEFINED TOKEN IN TEST CODE--check FdoTokens\n");
		break;
	}
	printf("\n");
}
#endif

static FdoInt32 CompareInsensitive(FdoString* pToken, FdoString* pucString)
{
	if (!pucString && !pToken)
		return 0;

	if (!pucString	&& pToken)
		return 1;

	if (pucString && !pToken)
		return -1;

	return FdoStringUtility::StringCompareNoCase(pToken, pucString);
}

//  Search for token in the keyword array
static FdoInt32 findtoken 
(
FdoString*			token,              // string token to find
const FdoKeyWord	words[],			// array of keyword/token pairs
FdoInt32					number_of_words     // number of patterns
)
{
    register FdoInt32 ii;             // Indexes are also needed. Sometimes ...  
    register FdoInt32 jj;
    register FdoInt32 kk;

    // Split tokens array in two parts and select appropriate part 
    ii  = 0 ;
    jj  = number_of_words-1 ;

    kk = (ii + jj) / 2;
    while (ii <= jj) 
	{
        if (CompareInsensitive(token, words[kk].word) <= 0) 
            jj = kk-1 ;
        
        if (CompareInsensitive(token, words[kk].word) >= 0) 
            ii = kk+1 ;
        
        kk = (ii + jj) / 2;
    }

    if (CompareInsensitive(token, words[kk].word) == 0) 
        return kk;
    else 
        return -1;  // Token not found
}

FdoInt32 FdoLexFgft::FindKeyWord(FdoString* word, FdoKeyWord* aKeyWords, FdoInt32 cCount)
{
    FdoInt32 ii;

    if ((ii = findtoken(word, aKeyWords, cCount)) >= 0) 
         return aKeyWords[ii].token;
    
    return FdoToken_Undefined;
}

//  return -1 if sign is true and 1 if it is false
inline FdoInt32 ifac(bool sign) 
{
   return sign ? (-1) : 1;
}

inline double make_double(FdoUInt32 n, FdoUInt32 f, FdoInt32 m)
{
	return ((double)n+(double)f*pow(10.0, -(double)m));
}

FdoLexFgft::FdoLexFgft(FdoString* str)
{
	m_line = str;
	m_prevToken = FdoToken_Start;
	m_lastToken = FdoToken_Start;
	m_token = FdoToken_Start;
	m_cprev = 0;
	m_cc = 0;
	
	m_double = 0.0;
	m_integer = 0;
	m_ch = if_getch();   // Get first character from input string
}  

FdoLexFgft::~FdoLexFgft() 
{
}

//  Return next character from the input stream.
//  Next character becomes current
const wchar_t FdoLexFgft::if_getch(void) 
{
	wchar_t	c;

	if (m_cc >= (FdoInt32)FdoStringUtility::StringLength(m_line)) 
	{
		c = '\0';
	}
	else 
	{
		c = m_line[m_cc++];
		if (c == '\n' || c == '\r') 
			c = ' ';
	}
	return c;
}

//  Get Word from input stream
void FdoLexFgft::getword(wchar_t* pstr, FdoInt32 maxCharacters)
{
	// TODO: check maximum length
	while (iswalnum(m_ch) || m_ch == '_') 
	{
		*pstr = m_ch;
		pstr++;
		m_ch = if_getch();
	}
	*pstr = '\0';
}

// Get digits from input stream.
// This outputs characters to buffer with range checking, but no NULL termination.
// TODO: range checking
wchar_t* FdoLexFgft::getdigits(wchar_t* pstr)
{
	while (iswdigit(m_ch)) 
	{
		*pstr = m_ch;
		pstr++;
		m_ch = if_getch();
	}
	return pstr; 
} 

// Get unsigned number
FdoUInt32 FdoLexFgft::get_unsigned(void)
{
    wchar_t		str[maxCharLength];		// Input char storage
	wchar_t*	pStr=str;

	// Get numbers from input stream 
	pStr = getdigits(pStr);
	*pStr = CHR_NULL;
	if (FdoStringUtility::StringLength(str) == 0)
	{
		return (FdoUInt32) -1;      // No digits in input stream 
	}
	// TODO: wtoi is NOT ANSI
#ifdef _WIN32
	return (FdoUInt32) _wtoi(str);
#else
	return (FdoUInt32) wcstol(str, NULL, 10);
#endif
}

//  Get number
bool FdoLexFgft::getnumber(bool sign)
{
// TODO: rewrite with my AutoSketch code
//    static char buffer [64];

    bool		dot;					// Dot flag
    double      rnum;					// Real number 
    wchar_t		str[maxCharLength];		// Input char storage
	wchar_t*	pstr = str;

	if (sign)
	{
		*pstr = '-';
		pstr++;
	}

    m_token = FdoToken_Integer;
    dot = false;

   	// Get numbers from input stream 
	pstr = getdigits(pstr);
    if (m_ch == '.') 
	{
		// Point follows: Real number 
		*pstr = m_ch;
		pstr++;
        m_ch = if_getch();
        pstr = getdigits(pstr);
        dot = true;
    } 
	else 
	{
		dot = false;
	}

    if (towupper(m_ch) == 'E') 
	{
		// E follows: Float point 
		*pstr = m_ch;
		pstr++;
        m_ch = if_getch();
        if (m_ch == '-' || m_ch =='+')  
		{
			*pstr = m_ch;
			pstr++;
            m_ch = if_getch();
        }
        if (!iswdigit(m_ch)) 
		{
            // Exponent shall contain digits
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_9_INVALIDDIGIT)));
        }
        pstr = getdigits(pstr);
		*pstr = CHR_NULL;
#ifdef _WIN32
        rnum = _wtof(str);
#else
        rnum =  wcstod(str, NULL);
#endif

    } 
	else if (dot == false) 
	{        
		// Integer number 
		*pstr = CHR_NULL;
#ifdef _WIN32
        rnum = _wtof(str);
#else
        wchar_t *end;
        rnum =  wcstod(str, &end);
#endif
        if (rnum < (double)LONG_MAX && rnum > (double)LONG_MIN) 
		{
			try  
			{
				m_integer = (FdoInt32)rnum;
	            return true;
			} catch (...) {
			}
        }
    }
	else 
	{
		*pstr = CHR_NULL;
#ifdef _WIN32
        rnum = _wtof(str);
#else
        rnum =  wcstod(str, NULL);
#endif
    }
	m_double = rnum;
	m_token = FdoToken_Double;
	return true;
}

//  Find first non-blank symbol
const wchar_t FdoLexFgft::find_nonblank(void) 
{
	// skip whitespace
	while (m_ch == ' ' || m_ch == '\t')  
	{
		m_ch = if_getch() ;
	}
	return m_ch;
}

//  Return next character after the current one
const wchar_t FdoLexFgft::nextchar(void) const 
{
	return m_line[m_cc];
}

//  Return previous character before the current one
const wchar_t FdoLexFgft::prevchar(void) const 
{
	wchar_t	c;
	return (m_cc<2) ? c = '\0': m_line[m_cc-2];
}

FdoInt32 FdoLexFgft::GetToken() 
{
	// Save start position of the previous symbol
	m_cprev = m_cc;

	// Save previous token
    m_lastToken = m_token;       

	// Get next not blank character 
    m_ch = find_nonblank();   

	// Stream is over
    if (m_ch == '\0') 
	{         
        m_token = FdoToken_End;
        m_prevToken = m_token;
        return m_token;
    }

    // Is it keyword ?
    if (iswalpha(m_ch)) 
	{
		wchar_t		word[maxCharLength];

		// Get keyword
        getword(word, sizeof(word)/sizeof(wchar_t));

        // Is it a valid keyword for this grammar?
        FdoInt32   tktmp;
        if ((tktmp = FindKeyWord(word, g_aFgftWords, FGFTKEYWORDCOUNT)) != FdoToken_Undefined)
		{
			m_token = tktmp;
        } 
		else 
		{
            m_token = FdoToken_Undefined;
        }
    }

	// Is it a numeric constant?
	else if (iswdigit(m_ch)) 
	{
		// m_token is real or integer number
        getnumber(false);
    }

	// negative values
	else if (m_ch == '-')
	{
        m_ch = if_getch();
        m_ch = find_nonblank();
		getnumber(true);
	}

	// comma
	else if (m_ch == ',')
	{
        m_token = FdoToken_Comma;
        m_ch = if_getch();

    }
	// left parenthesis
	else if (m_ch == '(')
	{
        m_token = FdoToken_LeftParenthesis;
        m_ch = if_getch();

    }
	// right parenthesis
	else if (m_ch == ')')
	{           
        m_token = FdoToken_RightParenthesis;
        m_ch = if_getch();
    }

	// something bad
	else
	{
        m_token = FdoToken_Error;
        m_ch = if_getch();
    }

    m_prevToken = m_token;
    return m_token;
}

