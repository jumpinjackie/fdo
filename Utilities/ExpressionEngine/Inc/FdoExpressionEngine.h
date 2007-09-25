// 
//  Copyright (C) 2004-2007  Autodesk, Inc.
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
    static FdoExpressionEngine* Create(FdoIReader* reader, FdoClassDefinition* classDef, FdoExpressionEngineFunctionCollection *userDefinedFunctions);

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
	static FdoExpressionEngine* Create(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* identifiers,
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
	virtual FdoLiteralValue* Evaluate(FdoExpression *expression);

    /// \brief
    /// Evaluates a name
    /// 
    /// \param name 
    /// Input identifier
    /// 
    /// \return
    /// Returns a literal value. This value is valid until the next Evaluate call
    /// 
    virtual FdoLiteralValue* Evaluate(FdoString* name);

    /// \brief
    /// Evaluates an identifier
    /// 
    /// \param identifier
    /// Input identifier
    /// 
    /// \return
    /// Returns a literal value. This value is valid until the next Evaluate call
    /// 
    virtual FdoLiteralValue* Evaluate(FdoIdentifier& expr);


    /// \brief
    /// Evaluates an aggregate functions
    /// 
    /// \return
    /// Returns the aggragate results
    /// 
	FdoPropertyValueCollection* RunQuery();

    /// \brief
    /// Checks if passes the filter
    /// 
    /// \param filter
    /// Input filter
    /// 
    /// \return
    /// Returns true id passes the filter, otherwise false
    /// 
    bool ProcessFilter(FdoFilter *filter);


    /// \brief
    /// Returns the default functions plus the user defined functions
    /// 
    /// \return
    /// Returns the functions
    /// 
	FdoFunctionDefinitionCollection *GetAllFunctions();


    /// \brief
    /// Returns the default functions the expression engine supports
    /// 
    /// \return
    /// Returns the functions
    /// 
	static FdoFunctionDefinitionCollection *GetStandardFunctions();

    /// \brief
    /// Checks if the filter is valid
    /// 
    /// \param cls
    /// Input class definition
    /// 
    /// \param filter
    /// Input filter
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
	static void ValidateFilter( FdoClassDefinition *cls, FdoFilter *filter, FdoIdentifierCollection *selIds = NULL, FdoIFilterCapabilities *filterCapabilities = NULL);

    /// \brief
    /// Optimizes the filter
    /// 
    /// \param filter
    /// Input the filter
    ///
    /// \return
    /// The optimized filter
    /// 
	static FdoFilter* OptimizeFilter( FdoFilter *filter );

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
   	static bool IsAggregateFunction(FdoFunctionDefinitionCollection *funcDefs, FdoString *name);


    // Returns the type of the expression

    /// \brief
    /// Returns the type of expression
    /// 
    /// \param functions
    /// Input the list of functions
    ///
    /// \param originalClassDef
    /// Input the class definition
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

	static void GetExpressionType(FdoFunctionDefinitionCollection *functionDefinitions, FdoClassDefinition* originalClassDef, FdoExpression *expr, FdoPropertyType &retPropType, FdoDataType &retDataType);

    // Returns the type of the expression

    /// \brief
    /// Returns the type of expression
    /// \param originalClassDef
    /// Input the class definition
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

	static void GetExpressionType(FdoClassDefinition* originalClassDef, FdoExpression *expr, FdoPropertyType &retPropType, FdoDataType &retDataType);


public:
    void Dispose ();

protected:
	FdoExpressionEngine(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* identifiers,
		FdoExpressionEngineFunctionCollection *userDefinedFunctions);
	FdoExpressionEngine();
	~FdoExpressionEngine();

private:
    FdoExpressionEngineImp *mEngine;
};

#endif
