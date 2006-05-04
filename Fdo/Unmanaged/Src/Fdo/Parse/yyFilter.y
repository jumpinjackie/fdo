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
//	yyFilter.y - YACC grammar 
//								
// Synopsis	
//	FdoInt32  fdo_filter_yyparse(FdoParse* pParse)
//
// Description							
//	Parse a FDO filter, and build a parse tree for it.
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
	FdoInt32		m_integer;	// integer values (Int8, Int16, Int32, TODO:I64 )
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
%token FdoToken_INTEGER  FdoToken_DOUBLE FdoToken_DATETIME
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
%type <m_node>		Function GeometryValue Identifier
%type <m_node>		ValueExpression LiteralValue Parameter UnaryExpression
// filter
%type <m_node>		BinaryLogicalOperator ComparisonCondition DistanceCondition
%type <m_node>		Filter GeometricCondition InCondition
%type <m_node>		LogicalOperator NullCondition ValueExpressionCollection 
%type <m_node>		SearchCondition SpatialCondition UnaryLogicalOperator
%type <m_id>		SpatialOperations // UnaryLogicalOperations BinaryLogicalOperations 
%type <m_id>		ComparisonOperations DistanceOperations
%type <m_node>		FdoToken_IN FdoToken_AND FdoToken_OR FdoToken_LIKE FdoToken_NOT
%type <m_node>		FdoToken_BEYOND FdoToken_WITHINDISTANCE
%type <m_node>		FdoToken_CONTAINS FdoToken_CROSSES
%type <m_node>		FdoToken_DISJOINT FdoToken_ENVELOPEINTERSECTS FdoToken_EQUALS FdoToken_INTERSECTS FdoToken_INSIDE
%type <m_node>		FdoToken_OVERLAPS FdoToken_TOUCHES FdoToken_WITHIN FdoToken_COVEREDBY
%type <m_node>		FdoToken_EQ FdoToken_NE FdoToken_GT FdoToken_GE FdoToken_LT FdoToken_LE
%type <m_node>		distance
// both
%type <m_node>		double string boolean FdoToken_Negate
%type <m_node>		FdoToken_BLOB FdoToken_CLOB FdoToken_TRUE FdoToken_FALSE
%type <m_node>		FdoToken_NULL FdoToken_GEOMFROMTEXT
%type <m_node>		integer datetime
%type <m_double>	FdoToken_DOUBLE
%type <m_integer>	FdoToken_INTEGER
%type <m_string>	FdoToken_IDENTIFIER FdoToken_STRING FdoToken_PARAMETER
%type <m_datetime>	FdoToken_DATETIME

// precedence
%left FdoToken_OR
%left FdoToken_AND
%left FdoToken_EQ FdoToken_NE FdoToken_GT FdoToken_GE FdoToken_LT FdoToken_LE
%left FdoToken_Add FdoToken_Subtract
%left FdoToken_Multiply FdoToken_Divide
%left FdoToken_Negate FdoToken_NOT

%start fdo
%%

fdo : Filter	{pParse->SetRoot(Node_Copy(L"Filter", $1));	YYACCEPT;}
    | ComputedIdentifier ',' Filter {pParse->SetRoot(Node_Copy(L"Filter", $3)); YYACCEPT;}
	| error		{Node_Trace(L"ERROR!");	pParse->SetRoot((FdoIDisposable*)NULL);	YYABORT;}
	;
    
// <yyExpression.y>

Expression :
	  '(' Expression ')'	{$$=Node_Copy(L"(Expression)", $2);}
    | UnaryExpression 		{$$=Node_Copy(L"UnaryExpression", $1);}
	| BinaryExpression		{$$=Node_Copy(L"BinaryExpression", $1);}
    | Function				{$$=Node_Copy(L"Function", $1);}
    | Identifier			{$$=Node_Copy(L"Identifier", $1);}
	| ValueExpression		{$$=Node_Copy(L"ValueExpression", $1);}
	;
	
BinaryExpression :
	Expression FdoToken_Add Expression			{$$=Node_Add(L"Add", FdoBinaryExpression::Create((FdoExpression*)$1, FdoBinaryOperations_Add, (FdoExpression*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	| Expression FdoToken_Subtract Expression	{$$=Node_Add(L"Subtract", FdoBinaryExpression::Create((FdoExpression*)$1, FdoBinaryOperations_Subtract, (FdoExpression*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	| Expression FdoToken_Multiply Expression	{$$=Node_Add(L"Multiply", FdoBinaryExpression::Create((FdoExpression*)$1, FdoBinaryOperations_Multiply, (FdoExpression*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	| Expression FdoToken_Divide Expression		{$$=Node_Add(L"Divide", FdoBinaryExpression::Create((FdoExpression*)$1, FdoBinaryOperations_Divide, (FdoExpression*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	;
 
DataValue :
	  boolean			// e.g. TRUE or FALSE
 						{$$=Node_Copy(L"boolean", $1);}
//	| byte
	| datetime			{$$=Node_Copy(L"datetime", $1);}
//	| decimal
	| double			// double and single precision
 						{$$=Node_Copy(L"double", $1);}
	| integer			// 16bit, 32bit, 64bit
 						{$$=Node_Copy(L"integer", $1);}
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
double :
	FdoToken_DOUBLE		{$$=Node_Add(L"DOUBLE", FdoDoubleValue::Create($1));}
	;
datetime :
	FdoToken_DATETIME	{$$=Node_Add(L"DateTime", FdoDataValue::Create($1));}
	;
	
Function :
	Identifier '(' ExpressionCollection ')'
					{$$=Node_Add(L"ExpressionCollection", FdoFunction::Create(((FdoIdentifier*)$1)->GetName(), (FdoExpressionCollection*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	;

ExpressionCollection :							{$$=pParse->AddNodeToDelete(FdoExpressionCollection::Create());}
	| Expression								{$$=pParse->AddNodeToDelete(FdoExpressionCollection::Create()); ((FdoExpressionCollection*)$$)->Add((FdoExpression*)$1); Node_Trace(L"Expression Arg 1,");FDO_SAFE_RELEASE($1); }
	| ExpressionCollection ',' Expression		{((FdoExpressionCollection*)$$)->Add((FdoExpression*)$3); Node_Trace(L"Expression Arg N,");FDO_SAFE_RELEASE($3); }
	;

GeometryValue :
	FdoToken_GEOMFROMTEXT '(' FdoToken_STRING ')'
			{$$=Node_Add(L"GeomFromText('FGFT')", pParse->FgftToGeometryValue($3));}
	;
	
Identifier :			// e.g. "Address.City", "Street", "City", and "Zip Code" or can be an alias of a computed identifier
	FdoToken_IDENTIFIER
						{
						    FdoComputedIdentifier* id = pParse->FindComputedIdentifier( $1 );
						    if( id == NULL )
						        $$ = Node_Add(L"IDENTIFIER", FdoIdentifier::Create($1));
						    else
						        $$ = id;
						}
 	;
 	
ComputedIdentifier:
      ComputedIdentifier2 {}
    | ComputedIdentifier ',' ComputedIdentifier2 {}
    ;
    
ComputedIdentifier2 :
     '(' Expression ')' FdoToken_AS Identifier
                    {
                        FdoComputedIdentifier* id = FdoComputedIdentifier::Create(((FdoIdentifier*)$5)->GetName(), (FdoExpression*)$2);
                        Node_Add(L"ComputedIdentifier", id);
                        pParse->AddCompIdentifier(id);FDO_SAFE_RELEASE($2); FDO_SAFE_RELEASE($5);
                        id->Release();
                    }
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
	FdoToken_Negate Expression	{$$ = Node_Add(L"UnaryExpression", FdoUnaryExpression::Create(FdoUnaryOperations_Negate, (FdoExpression*)$2));FDO_SAFE_RELEASE($2);}
	
// </yyExpression.y>

Filter :
	'(' Filter ')'		{$$=Node_Copy(L"(Filter)", $2);}
	| LogicalOperator	{$$=Node_Copy(L"LogicalOperator", $1);}
	| SearchCondition	{$$=Node_Copy(L"SearchCondition", $1);}
	;
	
LogicalOperator :
	BinaryLogicalOperator	{$$=Node_Copy(L"BinaryLogicalOperator", $1);}
	| UnaryLogicalOperator	{$$=Node_Copy(L"UnaryLogicalOperator", $1);}
	;
	
GeometricCondition :
	DistanceCondition	{$$=Node_Copy(L"DistanceCondition", $1);}
	| SpatialCondition	{$$=Node_Copy(L"SpatialCondition", $1);}
	;
	
ValueExpressionCollection :
	ValueExpression					{$$=pParse->AddNodeToDelete(FdoValueExpressionCollection::Create()); ((FdoValueExpressionCollection*)$$)->Add((FdoValueExpression*)$1); Node_Trace(L"ValueExpression 1");FDO_SAFE_RELEASE($1); }
	| ValueExpressionCollection ',' ValueExpression
										{((FdoValueExpressionCollection*)$$)->Add((FdoValueExpression*)$3); Node_Trace(L"ValueExpression n");FDO_SAFE_RELEASE($3); }
	;
	
InCondition :
	Identifier FdoToken_IN '(' ValueExpressionCollection ')'
				{$$=Node_Add(L"ValueExpressionCollection", FdoInCondition::Create((FdoIdentifier*)$1, (FdoValueExpressionCollection*)$4));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($4);}
	;
	
NullCondition :
	Identifier FdoToken_NULL	{$$=Node_Add(L"NULL", FdoNullCondition::Create((FdoIdentifier*)$1));FDO_SAFE_RELEASE($1);}
	;
	
SearchCondition :
	InCondition				{$$=Node_Copy(L"InCondition", $1);}
	| ComparisonCondition	{$$=Node_Copy(L"ComparisonCondition", $1);}
	| GeometricCondition	{$$=Node_Copy(L"GeometricCondition", $1);}
	| NullCondition			{$$=Node_Copy(L"NullCondition", $1);}
	;

SpatialCondition :
	Identifier SpatialOperations Expression	
		{$$=Node_Add(L"Id-SpatialOp-Expression", FdoSpatialCondition::Create((FdoIdentifier*)$1, (FdoSpatialOperations)$2, (FdoExpression*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	;

UnaryLogicalOperator :
	FdoToken_NOT Filter		{$$=Node_Add(L"NOT", FdoUnaryLogicalOperator::Create((FdoFilter*)$2, FdoUnaryLogicalOperations_Not));FDO_SAFE_RELEASE($2);}
	;

BinaryLogicalOperator :
	Filter FdoToken_AND Filter	{$$=Node_Add(L"AND", FdoBinaryLogicalOperator::Create((FdoFilter*)$1, FdoBinaryLogicalOperations_And, (FdoFilter*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	| Filter FdoToken_OR Filter	{$$=Node_Add(L"OR", FdoBinaryLogicalOperator::Create((FdoFilter*)$1, FdoBinaryLogicalOperations_Or, (FdoFilter*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	;

ComparisonCondition :
	Expression ComparisonOperations Expression	{$$=Node_Add(L"ComparisonOperations", FdoComparisonCondition::Create((FdoExpression*)$1, (FdoComparisonOperations)$2, (FdoExpression*)$3));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);}
	;

DistanceCondition :
	Identifier DistanceOperations Expression distance	
		{$$=Node_Add(L"Indentifier-DistanceOp-Expression", FdoDistanceCondition::Create((FdoIdentifier*)$1, (FdoDistanceOperations)$2, (FdoExpression*)$3, ((FdoDoubleValue*)$4)->GetDouble()));FDO_SAFE_RELEASE($1); FDO_SAFE_RELEASE($3);FDO_SAFE_RELEASE($4);}
	;
	
ComparisonOperations :
	FdoToken_EQ			{$$=FdoComparisonOperations_EqualTo; Node_Trace(L"EQ");}
	| FdoToken_NE		{$$=FdoComparisonOperations_NotEqualTo; Node_Trace(L"NE");}
	| FdoToken_GT		{$$=FdoComparisonOperations_GreaterThan; Node_Trace(L"GT");}
	| FdoToken_GE		{$$=FdoComparisonOperations_GreaterThanOrEqualTo; Node_Trace(L"GE");}
	| FdoToken_LT		{$$=FdoComparisonOperations_LessThan; Node_Trace(L"LT");}
	| FdoToken_LE		{$$=FdoComparisonOperations_LessThanOrEqualTo; Node_Trace(L"LE");}
	| FdoToken_LIKE		{$$=FdoComparisonOperations_Like; Node_Trace(L"LIKE");}
	;

DistanceOperations :
	FdoToken_BEYOND				{$$=FdoDistanceOperations_Beyond; Node_Trace(L"BEYOND");}
	| FdoToken_WITHINDISTANCE	{$$=FdoDistanceOperations_Within; Node_Trace(L"WITHINDISTANCE");}
	;

SpatialOperations :
	FdoToken_CONTAINS		{$$=FdoSpatialOperations_Contains; Node_Trace(L"CONTAINS");}
	| FdoToken_CROSSES		{$$=FdoSpatialOperations_Crosses; Node_Trace(L"CROSSES");}
	| FdoToken_DISJOINT		{$$=FdoSpatialOperations_Disjoint; Node_Trace(L"DISJOINT");}
	| FdoToken_EQUALS		{$$=FdoSpatialOperations_Equals; Node_Trace(L"EQUALS");}
	| FdoToken_INSIDE		{$$=FdoSpatialOperations_Inside; Node_Trace(L"INSIDE");}
	| FdoToken_INTERSECTS	{$$=FdoSpatialOperations_Intersects; Node_Trace(L"INTERSECTS");}
	| FdoToken_OVERLAPS		{$$=FdoSpatialOperations_Overlaps; Node_Trace(L"OVERLAPS");}
	| FdoToken_TOUCHES		{$$=FdoSpatialOperations_Touches; Node_Trace(L"TOUCHES");}
	| FdoToken_WITHIN		{$$=FdoSpatialOperations_Within; Node_Trace(L"WITHIN");}
	| FdoToken_COVEREDBY	{$$=FdoSpatialOperations_CoveredBy; Node_Trace(L"COVEREDBY");}
	| FdoToken_ENVELOPEINTERSECTS	{$$=FdoSpatialOperations_EnvelopeIntersects; Node_Trace(L"ENVELOPEINTERSECTS");}
	;
 
distance :
	double				{$$=Node_Copy(L"double", $1);}
	| FdoToken_INTEGER	{$$=Node_Add(L"INTEGER", FdoDoubleValue::Create((double)$1));}
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
