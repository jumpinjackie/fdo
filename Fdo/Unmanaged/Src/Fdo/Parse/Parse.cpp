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
#include "Parse.h"
#include <Geometry.h>

FdoParse::FdoParse()
{
	// initialize tree with empty list
	m_nodes = FdoIDisposableCollection::Create();
	m_nodesToDelete = FdoIDisposableCollection::Create();
    m_ComputedIdentifiers = FdoIdentifierCollection::Create();

	// default to invalid root node
	m_root = NULL;
	// ... no lexical analyzer
	m_lex = NULL;


    fdo_expression_yydebug = 0;
    fdo_expression_yynerrs = 0;
    fdo_expression_yyerrflag = 0;
    fdo_expression_yychar = -1;
    fdo_expression_yyssp = NULL;
    fdo_expression_yyvsp = NULL;
#ifndef _WIN32
	fdo_expression_yyss = NULL;
	fdo_expression_yysslim = NULL;
	fdo_expression_yyvs = NULL;
	fdo_expression_yystacksize = 0;
#endif
    fdo_expression_yylineno = 0;

    fdo_filter_yydebug = 0;
    fdo_filter_yynerrs = 0;
    fdo_filter_yyerrflag = 0;
    fdo_filter_yychar = -1;
    fdo_filter_yyssp = NULL;
    fdo_filter_yyvsp = NULL;
#ifndef _WIN32
	fdo_filter_yyss = NULL;
	fdo_filter_yysslim = NULL;
	fdo_filter_yyvs = NULL;
	fdo_filter_yystacksize = 0;
#endif
    fdo_filter_yylineno = 0;
}

FdoParse::~FdoParse()
{
	FDO_SAFE_RELEASE(m_nodes);
	FDO_SAFE_RELEASE(m_nodesToDelete);
    FDO_SAFE_RELEASE(m_ComputedIdentifiers);
	if (m_lex != NULL)
		delete m_lex;
}

FdoIDisposable* FdoParse::AddNode(FdoIDisposable* pNode)
{
	if (pNode != NULL)
		m_nodes->Add(pNode);
	return pNode;
}

FdoIDisposable* FdoParse::AddNodeToDelete(FdoIDisposable* pNode)
{
	if (pNode != NULL)
		m_nodesToDelete->Add(pNode);
	return pNode;
}

void FdoParse::AddCompIdentifier(FdoComputedIdentifier* pNode)
{
    if (pNode != NULL)
    {
        if( m_ComputedIdentifiers != NULL )
            m_ComputedIdentifiers->Add( pNode );
    }
}

FdoComputedIdentifier* FdoParse::FindComputedIdentifier( FdoString* name )
{
    for(int i=0; i<m_ComputedIdentifiers->GetCount(); i++ )
    {
        FdoComputedIdentifier *id = (FdoComputedIdentifier*)m_ComputedIdentifiers->GetItem(i);
        if( wcscmp( id->GetName(), name ) == 0 )
            return id;
        else
            id->Release();
    }
    return NULL;
}

//	provide debug trace of action type
FdoIDisposable* FdoParse::Add(FdoString* pDebug, FdoIDisposable *pThis)
{
#ifdef DEBUG_TRACETREE
	printf("Tree(%S)\n", pDebug);
#endif
	return AddNode(pThis);
}
FdoIDisposable* FdoParse::Copy(FdoString* pDebug, FdoIDisposable *pThis)
{
#ifdef DEBUG_TRACETREE
	printf("Tree(%S)\n", pDebug);
#endif
	return pThis;
}
void FdoParse::Trace(FdoString* pDebug)
{
#ifdef DEBUG_TRACETREE
	printf("Tree(%S)\n", pDebug);
#endif
}

// release all the allocated nodes and then Clean
void FdoParse::Abort(void)
{
	// go through collection and release nodes
	for (FdoInt32 i=0; i<m_nodes->GetCount(); i++)
	{
		FdoIDisposable* pItem = m_nodes->GetItem(i);
		pItem->Release();
		pItem->Release();
	}
	m_nodes->Clear();
}

// clean up deleted always list
void FdoParse::Clean(void)
{
	m_nodes->Clear();
	m_ComputedIdentifiers->Clear();
    m_nodesToDelete->Clear();
}

void FdoParse::SetRoot(FdoIDisposable* pNode)
{
	m_root = pNode;
}

// same lexical analyzer for filters and expressions
FdoInt32 FdoParse::GetLexeme(FdoParse *pParse, void* pyylval)
{
	// Note, this is based on the supposition that the union
	// in yyFilter.y and yyExpression.y are identical
	FDO_EXPRESSION_YYSTYPE* yylval = (FDO_EXPRESSION_YYSTYPE*) pyylval;

	FdoInt32 iToken = m_lex->GetToken(pParse);
#ifdef DEBUG_TRACELEX
	m_lex->PrintToken(iToken);
#endif
	switch (iToken)
	{
	case FdoToken_Literal:
		if (!m_lex->m_data)
		{
			return FdoToken_NULL;
		}
		else
		{
			// check if null
			if (m_lex->m_data->IsNull())
				return FdoToken_NULL;
			else
			{
				// print type and value
				switch (m_lex->m_data->GetDataType())
				{
				case FdoDataType_String:
					yylval->m_string = ((FdoStringValue*)(m_lex->m_data))->GetString();
					return FdoToken_STRING;
				case FdoDataType_Int16:
				case FdoDataType_Int32:
				case FdoDataType_Int64:
					yylval->m_integer = ((FdoInt32Value*)(m_lex->m_data))->GetInt32();
					return FdoToken_INTEGER;
				case FdoDataType_Single:
				case FdoDataType_Double:
					yylval->m_double = ((FdoDoubleValue*)(m_lex->m_data))->GetDouble();
					return FdoToken_DOUBLE;
				// date, time, timestamp
				case FdoDataType_DateTime:
				    yylval->m_datetime.Set(((FdoDateTimeValue*)(m_lex->m_data))->GetDateTime());
					return FdoToken_DATETIME;
				case FdoDataType_Boolean:
					if (((FdoBooleanValue*)(m_lex->m_data))->GetBoolean())
					{
						yylval->m_integer = 1;
						return FdoToken_TRUE;
					}
					else
					{
						yylval->m_integer = 0;
						return FdoToken_FALSE;
					}
					break;
				case FdoDataType_Byte:
				case FdoDataType_Decimal:
				case FdoDataType_BLOB:
				case FdoDataType_CLOB:
				default:
					return FdoToken_Error;
				}
			}
		}
		break;

	case FdoToken_IDENTIFIER:
		yylval->m_string = ((FdoStringValue*)(m_lex->m_data))->GetString();
		return FdoToken_IDENTIFIER;
	case FdoToken_PARAMETER:
		yylval->m_string = ((FdoStringValue*)(m_lex->m_data))->GetString();
		return FdoToken_PARAMETER;

	case FdoToken_LeftParenthesis:
		return '(';
	case FdoToken_RightParenthesis:
		return ')';
	case FdoToken_Comma:
		return ',';
//	case FdoToken_Dot:
//		return '.';
//	case FdoToken_Colon:
//		return ':';

#ifdef _DEBUG
	case FdoToken_End:
	// symbols
//	case FdoToken_LeftParenthesis:
//	case FdoToken_RightParenthesis:
//	case FdoToken_Comma:
	case FdoToken_Dot:
	case FdoToken_Colon:
	// expression operators
	case FdoToken_Add:
	case FdoToken_Subtract:
	case FdoToken_Multiply:
	case FdoToken_Divide:
	case FdoToken_Negate:
	// comparison operators
	case FdoToken_EQ:
	case FdoToken_NE:
	case FdoToken_LT:
	case FdoToken_LE:
	case FdoToken_GT:
	case FdoToken_GE:
	// keywords
	case FdoToken_AND:
	case FdoToken_AS:
	case FdoToken_BEYOND:
	case FdoToken_CONTAINS:
	case FdoToken_COVEREDBY:
	case FdoToken_CROSSES:
	case FdoToken_DATE:
	case FdoToken_DISJOINT:
	case FdoToken_ENVELOPEINTERSECTS:
	case FdoToken_EQUALS:
	case FdoToken_FALSE:
	case FdoToken_GEOMFROMTEXT:
	case FdoToken_IN:
	case FdoToken_INSIDE:
	case FdoToken_INTERSECTS:
	case FdoToken_LIKE:
	case FdoToken_NOT:
	case FdoToken_NULL:
	case FdoToken_OR:
	case FdoToken_OVERLAPS:
	case FdoToken_RELATE:
	case FdoToken_TIME:
	case FdoToken_TIMESTAMP:
	case FdoToken_TOUCHES:
	case FdoToken_TRUE:
	case FdoToken_WITHIN:
	case FdoToken_WITHINDISTANCE:
	case FdoToken_BETWEEN:
		return iToken;
	default:
		printf("!ERROR! Unknown token=%d\n", iToken);
		return FdoToken_Error;
#else
	default:
		return iToken;
#endif // _DEBUG
	}
// unreachable	return FdoToken_Error;
}

FdoExpression* FdoParse::ParseExpression(FdoString* pwzExpression)
{
	m_lex = new FdoLex(this, pwzExpression);
	if (m_lex == NULL)
		return NULL;
	try
	{
		fdo_expression_yyparse(this);
	    if (m_root == NULL)
            throw FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(PARSE_4_STRINGINCORRECTLYFORMATTED)));
	}
	catch (FdoException* exception)
	{
		Abort();
		Clean();

        FdoExpressionException* exceptionOuter = FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(EXPRESSION_17_STRINGNOTEXPRESSION)), exception);
        exception->Release();
		throw exceptionOuter;
	}
	Clean();
	return (FdoExpression*) m_root;
}

// use same lexical analyzer for filters and expressions
FdoInt32 fdo_expression_yylex(FdoParse* parse)
{
	// pass in pointer to union to set LLVAL
	return parse->GetLexeme(parse, &parse->fdo_expression_yylval);
}


FdoFilter* FdoParse::ParseFilter(FdoString* pwzFilter)
{
	m_lex = new FdoLex(this, pwzFilter);
	if (m_lex == NULL)
		return NULL;
	try
	{
		fdo_filter_yyparse(this);
	    if (m_root == NULL)
		    throw FdoExpressionException::Create(FdoException::NLSGetMessage(FDO_NLSID(PARSE_4_STRINGINCORRECTLYFORMATTED)));
	}
	catch (FdoException* exception)
	{
		Abort();
		Clean();

        FdoFilterException* exceptionOuter = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_9_STRINGNOTFILTER)), exception);
        exception->Release();
		throw exceptionOuter;
	}
	Clean();
	return (FdoFilter*) m_root;
}



// use same lexical analyzer for filters and expressions
FdoInt32 fdo_filter_yylex(FdoParse* parse)
{
	return parse->GetLexeme(parse, (FDO_EXPRESSION_YYSTYPE*)&parse->fdo_filter_yylval);
}

///////////////////////////////////////////////////////////////////////
// Parse() methods from FdoFilter and FdoExpression
///////////////////////////////////////////////////////////////////////

FdoFilter* FdoFilter::Parse(FdoString* pwzFilter)
{
	FdoParse*	parse = new FdoParse();
	FdoFilter*	pRoot = NULL;
	if (parse != NULL)
	{
        try
        {
		    pRoot = parse->ParseFilter(pwzFilter);
        }
        catch (FdoException* exception)
        {
            delete parse;
            throw exception;
        }
		delete parse;
	}
	return pRoot;
}

FdoExpression* FdoExpression::Parse(FdoString* pwzExpression)
{
	FdoParse*		parse = new FdoParse();
	FdoExpression*	pRoot = NULL;
	if (parse != NULL)
	{
        try
        {
    		pRoot = parse->ParseExpression(pwzExpression);
        }
        catch (FdoException* exception)
        {
            delete parse;
            throw exception;
        }
		delete parse;
	}
	return pRoot;
}

FdoGeometryValue* FdoParse::FgftToGeometryValue(FdoString* fgft)
{
	FdoFgfGeometryFactory* gf = FdoFgfGeometryFactory::GetInstance();
	FdoIGeometry*			geometry = NULL;
	FdoByteArray*			fgf = NULL;
	FdoGeometryValue*		value = NULL;
	try
	{
		// get geometry from FGFT string
		geometry = gf->CreateGeometry(fgft);

		// convert geometry to FGF
		fgf = gf->GetFgf(geometry);

		// and now use FGF to create GeometryValue
		value = FdoGeometryValue::Create(fgf);
	}
	catch (FdoException* exception)
	{
		FDO_SAFE_RELEASE(fgf);
		FDO_SAFE_RELEASE(geometry);
		FDO_SAFE_RELEASE(gf);
		throw exception;
	}
	FDO_SAFE_RELEASE(fgf);
	FDO_SAFE_RELEASE(geometry);
	FDO_SAFE_RELEASE(gf);
	return value;
}

