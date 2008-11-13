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
#include <Functions/Geometry/FdoFunctionZ.h>
#include <Functions/Geometry/Util.h>

// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionZ::FdoFunctionZ ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

	is_validated        = false;
    function_definition = NULL;
	
}  //  FdoFunctionZ ()

FdoFunctionZ::~FdoFunctionZ ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionZ ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionZ *FdoFunctionZ::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionZ();

}  //  Create ()

FdoFunctionZ *FdoFunctionZ::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionZ();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionZ::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function AREA.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionZ::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function Z().
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.
    FdoDouble						area        = 0.0;
	FdoIGeometry *					geom;
    FdoPtr<FdoFgfGeometryFactory>	gf;
    FdoPtr<FdoGeometryValue>        geom_value;
	FdoIPoint *						p;
	FdoIDirectPosition *			pos;

    gf = FdoFgfGeometryFactory::GetInstance();

    // If the argument list has not been validated, execute the check next.
    // NOTE: the validation is executed only the first time the procedure is
    //       invoked and assumes that it remains the same until the function
    //       result is retrieved.
    if (!is_validated) {

        Validate(literal_values);
        return_double_value = FdoDoubleValue::Create();
        is_validated = true;

    }  //  if (!is_validated) ...
    geom_value = (FdoGeometryValue *) literal_values->GetItem(0);
    if (geom_value->IsNull())
    {
        return_double_value->SetNull();
        return FDO_SAFE_ADDREF(return_double_value.p);
    }

    // Create a geometry object
    geom = gf->CreateGeometryFromFgf(
                            FdoPtr<FdoByteArray>(geom_value->GetGeometry()));

    // Only process point geometry
	FdoGeometryType geometryType = geom->GetDerivedType();
    if (geometryType != FdoGeometryType_Point)
    {
        return_double_value->SetNull();
        return FDO_SAFE_ADDREF(return_double_value.p);
    }

	// Check dimensionality if Z exists
	if (  ! (geom->GetDimensionality() & FdoDimensionality_Z ) )
    {
        return_double_value->SetNull();
        return FDO_SAFE_ADDREF(return_double_value.p);
    }

	// Get the poisition and reurn the value
	p = static_cast<FdoIPoint *> (geom);
	pos = p->GetPosition();

    return_double_value->SetDouble(pos->GetZ());
    return FDO_SAFE_ADDREF(return_double_value.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionZ::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function Z. The
// | function definition includes the list of supported signatures. The follow-
// | ing signatures are supported:
// |
// |    Z (geometry): double
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
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

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
                                FUNCTION_Z,
                                "Determines the Z coordinate of a point geometry");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_Z,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Geometry);

}  //  CreateFunctionDefinition ()

void FdoFunctionZ::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. Z() accepts one parameter only. If the
    // number of parameters is not correct issue an exception.

    if (count != 1) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_Z));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Geometry)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_Z));

}  //  Validate ()

