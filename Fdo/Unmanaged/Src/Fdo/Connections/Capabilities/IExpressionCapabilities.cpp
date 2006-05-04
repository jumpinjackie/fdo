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
#include <Fdo/Connections/Capabilities/IExpressionCapabilities.h>

#define ADD_DBL_FUNC(parm1,parm2) \
        func = FdoFunctionDefinition::Create(parm1,parm2,FdoDataType_Double, dblparms); \
        functions->Add( func )

#define ADD_INT_FUNC(parm1,parm2) \
        func = FdoFunctionDefinition::Create(parm1,parm2,FdoDataType_Int64, intparms); \
        functions->Add( func )

#define ADD_STR_FUNC(parm1,parm2) \
        func = FdoFunctionDefinition::Create(parm1,parm2,FdoDataType_String, strparms); \
        functions->Add( func )

#define ADD_GEOM_FUNC(parm1,parm2) \
        func = FdoFunctionDefinition::Create(parm1,parm2,FdoPropertyType_GeometricProperty, (FdoDataType)-1, geomparms); \
        functions->Add( func )


// Constructs an instance of a FunctionDefinition using the specified arguments.
FdoFunctionDefinitionCollection* FdoIExpressionCapabilities::GetWellKnownFunctions( )
{
	FdoPtr<FdoFunctionDefinition> func;

    FdoPtr<FdoArgumentDefinition>doubleParm = FdoArgumentDefinition::Create(L"dblValue",FdoException::NLSGetMessage(FUNCTION_DBL_ARG, "Argument that represents a double"),FdoDataType_Double );
    FdoPtr<FdoArgumentDefinitionCollection>dblparms = FdoArgumentDefinitionCollection::Create();
    dblparms->Add( doubleParm );

    FdoPtr<FdoArgumentDefinition>intParm = FdoArgumentDefinition::Create(L"intValue",FdoException::NLSGetMessage(FUNCTION_INT_ARG,"Argument that represents an integer"),FdoDataType_Int64 );
    FdoPtr<FdoArgumentDefinitionCollection>intparms = FdoArgumentDefinitionCollection::Create();
    intparms->Add( intParm );

    FdoPtr<FdoArgumentDefinition>strParm = FdoArgumentDefinition::Create(L"strValue",FdoException::NLSGetMessage(FUNCTION_STR_ARG,"Argument that represents a string"),FdoDataType_String );
    FdoPtr<FdoArgumentDefinitionCollection>strparms = FdoArgumentDefinitionCollection::Create();
    strparms->Add( strParm );

    FdoPtr<FdoArgumentDefinition>geomParm = FdoArgumentDefinition::Create(L"geomValue",FdoException::NLSGetMessage(FUNCTION_GEOM_ARG,"Argument that represents a geometry"),FdoPropertyType_GeometricProperty, (FdoDataType)-1 );
    FdoPtr<FdoArgumentDefinitionCollection>geomparms = FdoArgumentDefinitionCollection::Create();
    geomparms->Add( geomParm );

    FdoFunctionDefinitionCollection* functions = FdoFunctionDefinitionCollection::Create();

    // Number functions:
    ADD_INT_FUNC( FDO_FUNCTION_CEIL, FdoException::NLSGetMessage(FUNCTION_CEIL, "Returns the smallest integer greater than or equal to the expression value") );
    ADD_INT_FUNC( FDO_FUNCTION_FLOOR, FdoException::NLSGetMessage(FUNCTION_FLOOR, "Returns the largest integer equal to or less than the expression value") );
    
    // Agregate functions:
    ADD_DBL_FUNC( FDO_FUNCTION_AVG, FdoException::NLSGetMessage(FUNCTION_AVG, "Returns the average value of an expression") );
    ADD_INT_FUNC( FDO_FUNCTION_COUNT, FdoException::NLSGetMessage(FUNCTION_COUNT, "Returns the number of objects in the query") );
    ADD_DBL_FUNC( FDO_FUNCTION_MAX, FdoException::NLSGetMessage(FUNCTION_MAX, "Returns the maximum value of an expression") );
    ADD_DBL_FUNC( FDO_FUNCTION_MIN, FdoException::NLSGetMessage(FUNCTION_MIN, "Returns the minimum value of an expression") );
    ADD_DBL_FUNC( FDO_FUNCTION_SUM, FdoException::NLSGetMessage(FUNCTION_SUM, "Returns the sum of values of an expression") );
    ADD_GEOM_FUNC(FDO_FUNCTION_SPATIALEXTENTS, FdoException::NLSGetMessage(FUNCTION_SPATIALEXTENTS, "Returns the spatial extents of values of a geometric expression") );

    // String functions:
    ADD_STR_FUNC( FDO_FUNCTION_LOWER, FdoException::NLSGetMessage(FUNCTION_LOWER, "returns strings with all letters lowercase") );
    ADD_STR_FUNC( FDO_FUNCTION_UPPER, FdoException::NLSGetMessage(FUNCTION_UPPER, "returns strings with all letters uppercase") );
    // Setup the parms for the concat function
    strparms = FdoArgumentDefinitionCollection::Create();
    strParm = FdoArgumentDefinition::Create(L"strValue1",FdoException::NLSGetMessage(FUNCTION_STR_ARG,"Argument that represents a string",FdoDataType_String),FdoDataType_String);
    strparms->Add( strParm );
    strParm = FdoArgumentDefinition::Create(L"strValue2",FdoException::NLSGetMessage(FUNCTION_STR_ARG,"Argument that represents a string",FdoDataType_String),FdoDataType_String);
    strparms->Add( strParm );
    ADD_STR_FUNC( FDO_FUNCTION_CONCAT, FdoException::NLSGetMessage(FUNCTION_CONCAT, "returns the string concatenation of 2 expressions") );

    return functions;
}

