// 
//  Copyright (C) 2004-2011  Autodesk, Inc.
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

#ifndef FDOEXPRESSIONENGINE_H
#define FDOEXPRESSIONENGINE_H

#include "ExpressionEngine.h"

class FdoExpressionEngineFunctionCollection;
class FdoExpressionEngineImp;

class FdoExpressionEngine : public FdoIDisposable
{
public:
    /// \brief
    /// Constructs an instance of an FdoExpressionEngine using the specified arguments.
    /// 
    /// \param reader 
    /// Input reader
    /// 
    /// \param classDef
    /// Input class definition
    ///
    /// \param userDefinedFunctions
    /// Input user defined functions
    ///
    /// \return
    /// Returns FdoExpressionEngine
    /// 
    EXPRESSIONENGINE_API static FdoExpressionEngine* Create(FdoIReader* reader, 
                                                            FdoClassDefinition* classDef, 
                                                            FdoExpressionEngineFunctionCollection *userDefinedFunctions);

    /// \brief
    /// Constructs an instance of an FdoExpressionException using the specified arguments.
    /// 
    /// \param reader 
    /// Input reader
    /// 
    /// \param classDef
    /// Input class definition
    ///
    /// \param classDef
    /// Input identifiers
    ///
    /// \param userDefinedFunctions
    /// Input user defined functions
    ///
    /// \return
    /// Returns FdoExpressionEngine
    /// 
    EXPRESSIONENGINE_API static FdoExpressionEngine* Create(FdoIReader* reader, 
                                                            FdoClassDefinition* classDef, 
                                                            FdoIdentifierCollection* identifiers,
                                                            FdoExpressionEngineFunctionCollection *userDefinedFunctions);

    /// \brief
    /// Evaluates an expression
    /// 
    /// \param expression 
    /// Input expression
    /// 
    /// \return
    /// Returns a literal value. This value is valid until the next Evaluate call
    /// 
    EXPRESSIONENGINE_API virtual FdoLiteralValue* Evaluate(FdoExpression *expression);

    /// \brief
    /// Evaluates a name
    /// 
    /// \param name 
    /// Input identifier
    /// 
    /// \return
    /// Returns a literal value. This value is valid until the next Evaluate call
    /// 
    EXPRESSIONENGINE_API virtual FdoLiteralValue* Evaluate(FdoString* name);

    /// \brief
    /// Evaluates an identifier
    /// 
    /// \param identifier
    /// Input identifier
    /// 
    /// \return
    /// Returns a literal value. This value is valid until the next Evaluate call
    /// 
    EXPRESSIONENGINE_API virtual FdoLiteralValue* Evaluate(FdoIdentifier& expr);

    /// \brief
    /// Evaluates an aggregate functions
    /// 
    /// \return
    /// Returns the aggragate results
    /// 
    EXPRESSIONENGINE_API FdoPropertyValueCollection* RunQuery();

    /// \brief
    /// Checks if passes the filter
    /// 
    /// \param filter
    /// Input the filter to be processed
    /// 
    /// \return
    /// Returns true id passes the filter, otherwise false
    /// 
    EXPRESSIONENGINE_API bool ProcessFilter(FdoFilter *filter);

    /// \brief
    /// Returns the default functions plus the user defined functions
    /// 
    /// \return
    /// Returns the functions
    /// 
    EXPRESSIONENGINE_API FdoFunctionDefinitionCollection *GetAllFunctions();


    /// \brief
    /// Returns the default functions the expression engine supports
    /// 
    /// \return
    /// Returns the functions
    /// 
    EXPRESSIONENGINE_API static FdoFunctionDefinitionCollection *GetStandardFunctions();

    /// \brief
    /// Checks if the filter is valid
    /// 
    /// \param cls
    /// Input class definition
    /// 
    /// \param filter
    /// Input the filter to be validated
    /// 
    /// \param selIds
    /// Input identifier collection
    /// 
    /// \param filterCapabilities
    /// Input filter capabilities
    /// 
    /// \return
    /// Throws an exception is filter is not valid
    /// 
    EXPRESSIONENGINE_API static void ValidateFilter(FdoClassDefinition *cls, 
                                                    FdoFilter *filter, 
                                                    FdoIdentifierCollection *selIds = NULL, 
                                                    FdoIFilterCapabilities *filterCapabilities = NULL);

    /// \brief
    /// Optimizes the filter
    /// 
    /// \param filter
    /// Input the filter to be optimized
    ///
    /// \return
    /// The optimized filter
    /// 
    EXPRESSIONENGINE_API static FdoFilter* OptimizeFilter( FdoFilter *filter );

    /// \brief
    /// Checks if the function name is a aggregate function
    /// 
    /// \param funcDefs
    /// Input the list of functions
    ///
    /// \param name
    /// Input the function name
    ///
    /// \return
    /// True if the function is an aggregate function otherwise false
    /// 
    EXPRESSIONENGINE_API static bool IsAggregateFunction(FdoFunctionDefinitionCollection *funcDefs, FdoString *name);

    /// \brief
    /// Returns the type of expression
    /// 
    /// \param functions
    /// Input the list of functions
    ///
    /// \param originalClassDef
    /// Input the class definition
    ///
    /// \param expr
    /// Input the expression to be evaluated
    ///
    /// \param propType
    /// Output the property type
    ///
    /// \param dataType
    /// Output the data type
    ///
    /// \return
    /// Returns nothing
    /// 
    EXPRESSIONENGINE_API static void GetExpressionType(FdoFunctionDefinitionCollection *functionDefinitions, 
                                                       FdoClassDefinition* originalClassDef, 
                                                       FdoExpression *expr, 
                                                       FdoPropertyType &retPropType, 
                                                       FdoDataType &retDataType);

    /// \brief
    /// Returns the type of expression
    ///
    /// \param originalClassDef
    /// Input the class definition
    ///
    /// \param expr
    /// Input the expression to be evaluated
    ///
    /// \param propType
    /// Output the property type
    ///
    /// \param dataType
    /// Output the data type
    ///
    /// \return
    /// Returns nothing
    /// 
    EXPRESSIONENGINE_API static void GetExpressionType(FdoClassDefinition *originalClassDef, 
                                                       FdoExpression *expr, 
                                                       FdoPropertyType &retPropType, 
                                                       FdoDataType &retDataType);

    /// \brief
    /// Registers the user-defined functions
    ///
    /// \param userDefinedFunctions
    /// Input the user-defined functions
    ///
    /// \return
    /// Returns nothing
    /// 
    EXPRESSIONENGINE_API static void RegisterFunctions(FdoExpressionEngineFunctionCollection *userDefinedFunctions);

    /// \brief
    /// Returns the list of class identifiers referenced by an expression
    /// 
    /// \param functions
    /// Input a list of functions to be used when processing function type expressions
    ///
    /// \param originalClassDef
    /// Input the class definition applicable to the expression being evaluated
    ///
    /// \param expression
    /// Input the expression to be evaluated
    ///
    /// \param identifiers
    /// Output the IdentifierCollection that will be populated with the list of class 
    /// identifiers referenced by the expression
    ///
    /// \return
    /// Returns nothing
    /// 
    EXPRESSIONENGINE_API static void GetExpressionIdentifiers(FdoFunctionDefinitionCollection *functionDefinitions, 
                                                              FdoClassDefinition* originalClassDef, 
                                                              FdoExpression *expression,
                                                              FdoIdentifierCollection* identifiers);

    /// \brief
    /// Returns the list of class identifiers referenced by an expression
    ///
    /// \param originalClassDef
    /// Input the class definition applicable to the expression being evaluated
    ///
    /// \param expression
    /// Input the expression to be evaluated
    ///
    /// \param identifiers
    /// Output the IdentifierCollection that will be populated with the list of class 
    /// identifiers referenced by the expression
    ///
    /// \return
    /// Returns nothing
    /// 
    EXPRESSIONENGINE_API static void GetExpressionIdentifiers(FdoClassDefinition* originalClassDef, 
                                                              FdoExpression *expression,
                                                              FdoIdentifierCollection* identifiers);

    /// \brief
    /// Unregisters the user-defined functions added by RegisterFunctions
    ///
    /// \param userDefinedFunctions
    /// Input the user-defined functions to Unregister
    ///
    /// \return
    /// Returns nothing
    /// 
    EXPRESSIONENGINE_API static void UnRegisterFunctions(FdoExpressionEngineFunctionCollection *userDefinedFunctions);

protected:
    EXPRESSIONENGINE_API void Dispose ();

    EXPRESSIONENGINE_API FdoExpressionEngine(FdoIReader* reader, 
                                             FdoClassDefinition* classDef, 
                                             FdoIdentifierCollection* identifiers,
                                             FdoExpressionEngineFunctionCollection *userDefinedFunctions);

    EXPRESSIONENGINE_API FdoExpressionEngine();
    EXPRESSIONENGINE_API ~FdoExpressionEngine();

private:
    FdoExpressionEngineImp *mEngine;
};

#endif
