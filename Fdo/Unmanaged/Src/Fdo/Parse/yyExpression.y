// 
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

///////////////////////////////////////////////////////////////////////
// Name
//	yyExpression.y - YACC grammar 
//								
// Synopsis	
//	FdoInt32  fdo_expression_yyparse(FdoParse* pParse)
//
// Description							
//	Parse a FDO expression, and build a parse tree for it.
//
// Parameters
//	pParse	pointer to context
//
// Function Value									
//	0 if successful, 1 otherwise (respect YYACCEPT and YYABORT macros).
//	If successful, the root node is set in FdoParse to be returned. 
///////////////////////////////////////////////////////////////////////

%{

#include "Parse.h"
#pragma warning(disable:4102)	// unreferenced labels in y.tab.c

%}

// Note, the unions in yyFilter.y and yyExpression.y must be identical
%union _YYSTYPE
{
	FdoIDisposable*	m_node;		// expression or filter parse tree node
	FdoInt32		m_id;		// enumerations, keywords, ...
	FdoInt32		m_integer;	// integer values (Int8, Int16, Int32 )
	FdoInt64		m_int64;	// 64-bit integer values
	double			m_double;	// floating point values (single(float), double)
	FdoString*		m_string;	// string
	_FdoDateTime	m_datetime;	// date time
}

// keywords 
// ... data values
%token FdoToken_NULL
%token FdoToken_TRUE FdoToken_FALSE
%token FdoToken_DATE FdoToken_TIME FdoToken_TIMESTAMP
%token FdoToken_GEOMFROMTEXT
// ... operators
%token FdoToken_AND FdoToken_OR FdoToken_NOT FdoToken_LIKE FdoToken_IN
// ... distance operations
%token FdoToken_BEYOND FdoToken_WITHINDISTANCE
// ... spatial operations
%token FdoToken_CONTAINS FdoToken_COVEREDBY FdoToken_CROSSES
%token FdoToken_DISJOINT FdoToken_ENVELOPEINTERSECTS FdoToken_EQUALS FdoToken_INTERSECTS FdoToken_INSIDE
%token FdoToken_OVERLAPS FdoToken_TOUCHES FdoToken_WITHIN
// ... future use
%token FdoToken_RELATE
// data types
%token FdoToken_IDENTIFIER FdoToken_PARAMETER FdoToken_STRING
%token FdoToken_INTEGER  FdoToken_INT64 FdoToken_DOUBLE FdoToken_DATETIME
%token FdoToken_BLOB FdoToken_CLOB
// operators
%token FdoToken_Add FdoToken_Subtract FdoToken_Multiply FdoToken_Divide 
%token FdoToken_Negate
%token FdoToken_EQ FdoToken_NE FdoToken_GT FdoToken_GE FdoToken_LT FdoToken_LE 
%token FdoToken_LeftParenthesis FdoToken_RightParenthesis
%token FdoToken_Comma FdoToken_Dot FdoToken_Colon FdoToken_AS

// keywords
%token FdoToken_BETWEEN

// expression
%type <m_node>		BinaryExpression DataValue Expression ExpressionCollection
%type <m_node>		Function GeometryValue Identifier ComputedIdentifier
%type <m_node>		ValueExpression LiteralValue Parameter UnaryExpression

// both
%type <m_node>		double string boolean FdoToken_Negate
%type <m_node>		FdoToken_BLOB FdoToken_CLOB FdoToken_TRUE FdoToken_FALSE
%type <m_node>		FdoToken_NULL FdoToken_GEOMFROMTEXT
%type <m_node>		integer int64 datetime
%type <m_double>	FdoToken_DOUBLE
%type <m_integer>	FdoToken_INTEGER
%type <m_int64>		FdoToken_INT64
%type <m_string>	FdoToken_IDENTIFIER FdoToken_STRING FdoToken_PARAMETER
%type <m_datetime>	FdoToken_DATETIME

// precedence
%left FdoToken_Add FdoToken_Subtract
%left FdoToken_Multiply FdoToken_Divide
%left FdoToken_Negate

%start fdo
%%

fdo : Expression	{pParse->SetRoot(Node_Copy(L"Expression", $1));	YYACCEPT;}
	| error			{Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	YYABORT;}
	;

Expression :
	  '(' Expression ')'	{$$=Node_Copy(L"(Expression)", $2);}
    | UnaryExpression 		{$$=Node_Copy(L"UnaryExpression", $1);}
	| BinaryExpression		{$$=Node_Copy(L"BinaryExpression", $1);}
    | Function				{$$=Node_Copy(L"Function", $1);}
    | Identifier			{$$=Node_Copy(L"Identifier", $1);}
	| ValueExpression		{$$=Node_Copy(L"ValueExpression", $1);}
	| ComputedIdentifier    {$$=Node_Copy(L"ComputedIdentifier", $1);}
	;
	
BinaryExpression :
	Expression FdoToken_Add Expression			{$$=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)$1, FdoBinaryOperations_Add, (FdoExpression*)$3));}
	| Expression FdoToken_Subtract Expression	{$$=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)$1, FdoBinaryOperations_Subtract, (FdoExpression*)$3));}
	| Expression FdoToken_Multiply Expression	{$$=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)$1, FdoBinaryOperations_Multiply, (FdoExpression*)$3));}
	| Expression FdoToken_Divide Expression		{$$=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)$1, FdoBinaryOperations_Divide, (FdoExpression*)$3));}
	;
	  
DataValue :
	  boolean			// e.g. TRUE or FALSE
 						{$$=Node_Copy(L"boolean", $1);}
//	| byte
	| datetime			{$$=Node_Copy(L"datetime", $1);}
//	| decimal
	| double			// double and single precision
 						{$$=Node_Copy(L"double", $1);}
	| integer			// 16bit, 32bit
 						{$$=Node_Copy(L"integer", $1);}
	| int64 			// 64bit
 						{$$=Node_Copy(L"int64", $1);}
	| string			// e.g. 'abc', "abc" (NOT!)
 						{$$=Node_Copy(L"string", $1);}
	| FdoToken_BLOB 	// '{' bytes... '}'	// e.g. BLOB { \x01 \x02 }	// TBD?
						{$$=Node_Copy(L"TODO:BLOB", NULL);}
	| FdoToken_CLOB  	// '{' bytes... '}'	// e.g. CLOB { \x01 \x02 }	// TBD?
						{$$=Node_Copy(L"TODO:CLOB", NULL);}
	| FdoToken_NULL		// e.g. null
						{$$=Node_Add(L"NULL", FdoDataValue::Create(FdoDataType_Boolean));}
	;

boolean :
	FdoToken_TRUE		{$$=Node_Add(L"TRUE", FdoBooleanValue::Create(true));}
	| FdoToken_FALSE	{$$=Node_Add(L"FALSE", FdoBooleanValue::Create(false));}
	;
string :
	FdoToken_STRING		{$$=Node_Add(L"STRING", FdoDataValue::Create($1));}
	;
integer :
	FdoToken_INTEGER	{$$=Node_Add(L"INTEGER", FdoInt32Value::Create($1));}
	;
int64 :
	FdoToken_INT64  	{$$=Node_Add(L"INT64", FdoInt64Value::Create($1));}
	;
double :
	FdoToken_DOUBLE		{$$=Node_Add(L"DOUBLE", FdoDoubleValue::Create($1));}
	;
datetime :
	FdoToken_DATETIME	{$$=Node_Add(L"DateTime", FdoDataValue::Create($1));}
	;
	
Function :
	Identifier '(' ExpressionCollection ')'
					{$$=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)$1)->GetName(), (FdoExpressionCollection*)$3));}
	;

ComputedIdentifier :
    '(' Expression ')' FdoToken_AS Identifier	{$$=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)$5)->GetName(), (FdoExpression*)$2));}
    | Expression FdoToken_AS Identifier	{$$=Node_Add(L"ComputedIdentifier", FdoComputedIdentifier::Create(((FdoIdentifier*)$3)->GetName(), (FdoExpression*)$1));}
     ;

ExpressionCollection :							{$$=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
	| Expression								{$$=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)$$)->Add((FdoExpression*)$1); Node_Trace(L"Expression Arg 1,");}
	| ExpressionCollection ',' Expression		{((FdoExpressionCollection*)$$)->Add((FdoExpression*)$3); Node_Trace(L"Expression Arg N,");}
	;

GeometryValue :
	FdoToken_GEOMFROMTEXT '(' FdoToken_STRING ')'
			{$$=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue($3));}
	;
	
Identifier :			// e.g. "Address.City", "Street", "City", and "Zip Code"
	FdoToken_IDENTIFIER
						{$$ = Node_Add(L"IDENTIFIER", FdoIdentifier::Create($1));}
 	;
 	
ValueExpression :
	  LiteralValue		{$$=Node_Copy(L"LiteralValue", $1);}
	| Parameter			{$$=Node_Copy(L"Parameter", $1);}
	;

LiteralValue :
	  GeometryValue		{$$=Node_Copy(L"GeometryValue", $1);}
 	| DataValue			{$$=Node_Copy(L"DataValue", $1);}
 	;

Parameter :	
	FdoToken_PARAMETER		// ':' string	// e.g. "Enter city"
						{$$ = Node_Add(L"PARAMETER", FdoParameter::Create($1));}
	| FdoToken_Colon FdoToken_STRING 
						{$$ = Node_Add(L":Parameter", FdoParameter::Create($2));}
	;
	
UnaryExpression :
	FdoToken_Negate Expression	{$$ = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)$2));}
	;

%%
///////////////////////////////////////////////////////////////////////

#include <stdio.h>

FdoInt32 yyerror_default(char *str, FdoInt32 lineno)
{
#ifdef FDO_YYDEBUG
	fprintf(stderr, "line %d: %s\n", lineno, str);
#endif
	return 0;
}

typedef FdoInt32 (*ERRFUNC)(char*, FdoInt32);
static ERRFUNC yyerror_func = yyerror_default;

FdoInt32 yyerror(FdoParse *pParse, char* str)
{
	return (*yyerror_func)(str, yylineno);
}

ERRFUNC yyerror_set(ERRFUNC func)
{
	ERRFUNC old_func = yyerror_func;

	if (func != NULL)
		yyerror_func = func;

	return old_func;
}

static void  print_current_token()
{
//TODO: this has not been updated, but it is unused anyway
#ifdef FDO_YYDEBUG
	char  *token_name;
	
	if( yychar <= YYMAXTOKEN )
	{
		switch( yychar )
		{
			case IDENTIFIER:
				token_name = yylval.id;
				break;

			default:
				token_name = yyname[yychar];
				break;
		}
	}
	else
	{
		token_name = "illegal-name";
	}

	printf( "***  current token is %d (%s)  ***\n", yychar, token_name );
#endif
}
