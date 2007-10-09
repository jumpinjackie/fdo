//
// 
// Copyright (C) 2004-2007  Autodesk, Inc.
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
// 
//

#include <stdafx.h>
#include <Functions/Geometry/FdoFunctionLength2D.h>
#include <Functions/Geometry/Util.h>

// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionLength2D::FdoFunctionLength2D (FdoBoolean computeGeodetic)

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{
    // Initialize all class variables.
	is_validated = false;
    function_definition = NULL;
	compute_geodetic = computeGeodetic;

}  //  FdoFunctionLength2D ()

FdoFunctionLength2D::~FdoFunctionLength2D ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionLength2D ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionLength2D *FdoFunctionLength2D::Create (FdoBoolean computeGeodetic)

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionLength2D(computeGeodetic);

}  //  Create ()

FdoFunctionLength2D *FdoFunctionLength2D::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionLength2D(compute_geodetic);

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionLength2D::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the geometry func-
// | tion LENGTH.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionLength2D::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the geometry function LENGTH.
// +---------------------------------------------------------------------------

{
    FdoPtr<FdoGeometryValue> geom_value;

    // If the argument list has not been validated, execute the check next.
    // NOTE: the validation is executed only the first time the procedure is
    //       invoked and assumes that it remains the same until the function
    //       result is retrieved.

    if (!is_validated) {

        Validate(literal_values);
        is_validated = true;

    }  //  if (!is_validated) ...

    geom_value = (FdoGeometryValue *) literal_values->GetItem(0);
    if (geom_value->IsNull())
		return FdoDoubleValue::Create();

	// Create a geometry object
    FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry>			geom = gf->CreateGeometryFromFgf( FdoPtr<FdoByteArray>(geom_value->GetGeometry()));
	FdoDouble						length = 0.0;

	// Compute
	FdoExpressionEngineGeometryUtil::ComputeGeometryLength( compute_geodetic, false /*3D*/, geom, &length );
	
	return FdoDoubleValue::Create(length);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionLength2D::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the geometry function
// | LENGTH. The function definition includes the list of supported signatures.
// | The following signatures are supported:
// |
// |    LENGTH (geometry)
// |
// | The function always returns a DOUBLE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                               *desc               = NULL;

    FdoStringP                              arg1_description;
    FdoStringP                              geom_arg_literal;

    FdoPtr<FdoArgumentDefinition>           geom_arg;

    FdoPtr<FdoArgumentDefinitionCollection> geom_args;

    FdoPtr<FdoSignatureDefinition>          signature;
    FdoSignatureDefinitionCollection        *signatures;

    // Get the general descriptions for the arguments.

    arg1_description = FdoException::NLSGetMessage(
                                        FUNCTION_GENERAL_ARG,
                                        "Argument to be processed");

    // The following defines the different argument definition collections.

    geom_arg_literal =
            FdoException::NLSGetMessage(FUNCTION_GEOM_ARG_LIT,
                                        "geometry property");

    geom_arg =
            FdoArgumentDefinition::Create(geom_arg_literal,
                                          arg1_description,
                                          FdoPropertyType_GeometricProperty,
                                          (FdoDataType)-1);

    geom_args = FdoArgumentDefinitionCollection::Create();
    geom_args->Add(geom_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_Double, geom_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                                FUNCTION_LENGTH2D,
                                "Determines the length of a given geometry");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_LENGTH2D,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Geometry);

}  //  CreateFunctionDefinition ()

void FdoFunctionLength2D::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. LENGTH accepts one parameter only. If the
    // number of parameters is not correct issue an exception.

    if (count != 1) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_LENGTH2D));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Geometry)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_LENGTH2D));

}  //  Validate ()

