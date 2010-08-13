/*
 * 
* Copyright (C) 2004-2011  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
*/

#include <stdafx.h>
#include <FdoExpressionEngine.h>
#include <FdoExpressionEngineImp.h>

#include <FdoCommonOSUtil.h>
#include <FdoCommonMiscUtil.h>

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

#include <wctype.h>
#include <malloc.h>
#include <math.h>
#include <limits.h>

#include <FdoExpressionEngineIAggregateFunction.h>


FdoExpressionEngine* FdoExpressionEngine::Create(FdoIReader* reader, 
        FdoClassDefinition* classDef, FdoExpressionEngineFunctionCollection *userDefinedFunctions)
{
    return new FdoExpressionEngine(reader, classDef, NULL, userDefinedFunctions);
}

FdoExpressionEngine* FdoExpressionEngine::Create(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* identifiers,
        FdoExpressionEngineFunctionCollection *userDefinedFunctions)
{
    return new FdoExpressionEngine(reader, classDef, identifiers, userDefinedFunctions);
}

FdoExpressionEngine::FdoExpressionEngine(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* compIdents,
        FdoExpressionEngineFunctionCollection *userDefinedFunctions)
{
    mEngine = FdoExpressionEngineImp::Create(reader, classDef, compIdents, userDefinedFunctions);
}

FdoExpressionEngine::FdoExpressionEngine()
{
}

FdoExpressionEngine::~FdoExpressionEngine()
{
    mEngine->Release();
}

void FdoExpressionEngine::Dispose()
{
    delete this;
}

FdoLiteralValue* FdoExpressionEngine::Evaluate(FdoExpression *expression)
{
    return mEngine->Evaluate(expression);
}

FdoLiteralValue* FdoExpressionEngine::Evaluate(FdoString* name)
{
    return mEngine->Evaluate(name);
}

FdoLiteralValue* FdoExpressionEngine::Evaluate(FdoIdentifier& expr)
{
    return mEngine->Evaluate(expr);
}

FdoPropertyValueCollection* FdoExpressionEngine::RunQuery()
{
    return mEngine->RunQuery();
}

bool FdoExpressionEngine::ProcessFilter(FdoFilter *filter)
{
    return mEngine->ProcessFilter(filter);
}

FdoFunctionDefinitionCollection *FdoExpressionEngine::GetStandardFunctions()
{
    return FdoExpressionEngineImp::GetStandardFunctions();
}

FdoFunctionDefinitionCollection *FdoExpressionEngine::GetAllFunctions()
{
    return mEngine->GetAllFunctions();
}

void FdoExpressionEngine::ValidateFilter( FdoClassDefinition *cls, FdoFilter *filter, FdoIdentifierCollection *selIds, FdoIFilterCapabilities *filterCapabilities)
{
    FdoExpressionEngineImp::ValidateFilter(cls, filter, selIds, filterCapabilities);
}

FdoFilter* FdoExpressionEngine::OptimizeFilter( FdoFilter *filter )
{
    return FdoExpressionEngineImp::OptimizeFilter(filter);
}

bool FdoExpressionEngine::IsAggregateFunction(FdoFunctionDefinitionCollection *funcDefs, FdoString *name)
{
    return FdoExpressionEngineImp::IsAggregateFunction(funcDefs, name);
}

void FdoExpressionEngine::GetExpressionType(FdoFunctionDefinitionCollection *functionDefinitions, FdoClassDefinition* originalClassDef, FdoExpression *expr, FdoPropertyType &retPropType, FdoDataType &retDataType)
{
    FdoExpressionEngineImp::GetExpressionType(functionDefinitions, originalClassDef, expr, retPropType, retDataType);
}

void FdoExpressionEngine::GetExpressionType(FdoClassDefinition* originalClassDef, FdoExpression *expr, FdoPropertyType &retPropType, FdoDataType &retDataType)
{
    FdoExpressionEngineImp::GetExpressionType(originalClassDef, expr, retPropType, retDataType);
}

void FdoExpressionEngine::RegisterFunctions(FdoExpressionEngineFunctionCollection *userDefinedFunctions)
{
    FdoExpressionEngineImp::RegisterFunctions(userDefinedFunctions);
}

void FdoExpressionEngine::GetExpressionIdentifiers(FdoFunctionDefinitionCollection *functionDefinitions, FdoClassDefinition* originalClassDef, FdoExpression *expression, FdoIdentifierCollection* identifiers)
{
    FdoExpressionEngineImp::GetExpressionIdentifiers(functionDefinitions, originalClassDef, expression, identifiers);
}

void FdoExpressionEngine::GetExpressionIdentifiers(FdoClassDefinition* originalClassDef, FdoExpression *expression, FdoIdentifierCollection* identifiers)
{
    FdoExpressionEngineImp::GetExpressionIdentifiers(originalClassDef, expression, identifiers);
}
