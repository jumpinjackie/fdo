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
#include <Functions/Aggregate/FdoFunctionSpatialExtents.h>


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionSpatialExtents::FdoFunctionSpatialExtents ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    is_validated               = false;
    result_geometry_value_set  = false;

    function_definition        = NULL;

}  //  FdoFunctionSpatialExtents ()


FdoFunctionSpatialExtents::~FdoFunctionSpatialExtents ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionSpatialExtents ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionSpatialExtents *FdoFunctionSpatialExtents::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionSpatialExtents();

}  //  Create ()

FdoFunctionSpatialExtents *FdoFunctionSpatialExtents::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionSpatialExtents();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionSpatialExtents::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function
// | SPATIALEXTENTS.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

void FdoFunctionSpatialExtents::Process (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function SPATIALEXTENTS.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

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
    if (!geom_value->IsNull())
        ProcessRequest(geom_value);

}  //  Process ()

FdoLiteralValue *FdoFunctionSpatialExtents::GetResult ()

// +---------------------------------------------------------------------------
// | The function returns the result of a call to the function GETSPATIAL-
// | EXTENTS.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPtr<FdoByteArray>          byte_array;
    FdoPtr<FdoFgfGeometryFactory> geometry_factory;

    // Invalidate the flag indicating that the validation has been done.

    is_validated = false;

    // Return the result.

    if (!result_geometry_value_set)
        return FdoGeometryValue::Create();

    geometry_factory = FdoFgfGeometryFactory::GetInstance();
    byte_array       = geometry_factory->GetFgf(result_geometry_value);
    return FdoGeometryValue::Create(byte_array);

}  //  GetResult ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionSpatialExtents::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function SPATIAL-
// | EXTENTS. The function definition includes the list of supported signa-
// | tures. The following signatures are supported:
// |
// |    SPATIALEXTENTS (geometry)
// |
// | The function always returns a GEOMETRY.
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

    geom_arg = FdoArgumentDefinition::Create(geom_arg_literal,
                                             arg1_description,
                                             FdoPropertyType_GeometricProperty,
                                             (FdoDataType)-1);

    geom_args = FdoArgumentDefinitionCollection::Create();
    geom_args->Add(geom_arg);

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(
                                            FdoPropertyType_GeometricProperty,
                                            (FdoDataType)-1,
                                            geom_args);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(
                            FUNCTION_SPATIALEXTENTS,
                            "Determines the average value of an expression");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_SPATIALEXTENTS,
                                        desc,
                                        true,
                                        signatures,
                                        FdoFunctionCategoryType_Aggregate);

}  //  CreateFunctionDefinition ()

void FdoFunctionSpatialExtents::ProcessRequest (
                                            FdoGeometryValue *geometry_value)

// +---------------------------------------------------------------------------
// | The function processes a request to the Expression Engine function
// | SPATIALEXTENTS.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    const double                  *aggregate_ordinates;

    int                           i                     = 0;

    double                        ordinates[15];

    FdoIPolygon                   *aggregate_extents    = NULL;

    FdoPtr<FdoIPolygon>           extents;
    FdoPtr<FdoIEnvelope>          envelope;
    FdoPtr<FdoIGeometry>          geometry;
    FdoPtr<FdoILinearRing>        linear_ring,
                                  updated_linear_ring;
    FdoPtr<FdoFgfGeometryFactory> geometry_factory;


    // If the provided geometry is empty, exit the function.

    if (geometry_value->IsNull())
        return;

    // Process the request.

    geometry_factory = FdoFgfGeometryFactory::GetInstance();
    geometry         = geometry_factory->CreateGeometryFromFgf(
                                    FdoPtr<FdoByteArray>(
                                            geometry_value->GetGeometry()));
    envelope         = geometry->GetEnvelope();

    if (!result_geometry_value_set) {

        ordinates[i++] = envelope->GetMinX();
        ordinates[i++] = envelope->GetMinY();
        if (geometry->GetDimensionality() & FdoDimensionality_Z)
            ordinates[i++] = envelope->GetMinZ();

        ordinates[i++] = envelope->GetMaxX();
        ordinates[i++] = envelope->GetMinY();
        if (geometry->GetDimensionality() & FdoDimensionality_Z)
            ordinates[i++] = envelope->GetMaxZ();

        ordinates[i++] = envelope->GetMaxX();
        ordinates[i++] = envelope->GetMaxY();
        if (geometry->GetDimensionality() & FdoDimensionality_Z)
            ordinates[i++] = envelope->GetMaxZ();

        ordinates[i++] = envelope->GetMinX();
        ordinates[i++] = envelope->GetMaxY();
        if (geometry->GetDimensionality() & FdoDimensionality_Z)
            ordinates[i++] = envelope->GetMinZ();

        ordinates[i++] = envelope->GetMinX();
        ordinates[i++] = envelope->GetMinY();
        if (geometry->GetDimensionality() & FdoDimensionality_Z)
            ordinates[i++] = envelope->GetMinZ();

    }  //  if (!result_geometry_value_set) ...
    else {

      aggregate_extents   = static_cast<FdoIPolygon*>(result_geometry_value.p);
      linear_ring         = aggregate_extents->GetExteriorRing();
      aggregate_ordinates = linear_ring->GetOrdinates();

      ordinates[i] = FdoCommonMin(envelope->GetMinX(), aggregate_ordinates[i]);
      i++;
      ordinates[i] = FdoCommonMin(envelope->GetMinY(), aggregate_ordinates[i]);
      i++;
      if (geometry->GetDimensionality() & FdoDimensionality_Z) {

          ordinates[i] =
                    FdoCommonMin(envelope->GetMinZ(), aggregate_ordinates[i]);
          i++;

      }  //  if (geometry->GetDimensionality() & FdoDimensionality_Z) ...

      ordinates[i] = FdoCommonMin(envelope->GetMaxX(), aggregate_ordinates[i]);
      i++;
      ordinates[i] = FdoCommonMin(envelope->GetMinY(), aggregate_ordinates[i]);
      i++;
      if (geometry->GetDimensionality() & FdoDimensionality_Z) {

          ordinates[i] =
                    FdoCommonMax(envelope->GetMaxZ(), aggregate_ordinates[i]);
          i++;

      }  //  if (geometry->GetDimensionality() & FdoDimensionality_Z) ...

      ordinates[i] = FdoCommonMax(envelope->GetMaxX(), aggregate_ordinates[i]);
      i++;
      ordinates[i] = FdoCommonMax(envelope->GetMaxY(), aggregate_ordinates[i]);
      i++;
      if (geometry->GetDimensionality() & FdoDimensionality_Z) {

          ordinates[i] =
                    FdoCommonMax(envelope->GetMaxZ(), aggregate_ordinates[i]);
          i++;

      }  //  if (geometry->GetDimensionality() & FdoDimensionality_Z) ...

      ordinates[i] = FdoCommonMax(envelope->GetMinX(), aggregate_ordinates[i]);
      i++;
      ordinates[i] = FdoCommonMax(envelope->GetMaxY(), aggregate_ordinates[i]);
      i++;
      if (geometry->GetDimensionality() & FdoDimensionality_Z) {

          ordinates[i] =
                    FdoCommonMin(envelope->GetMinZ(), aggregate_ordinates[i]);
          i++;

      }  //  if (geometry->GetDimensionality() & FdoDimensionality_Z) ...

      ordinates[i] = FdoCommonMin(envelope->GetMinX(), aggregate_ordinates[i]);
      i++;
      ordinates[i] = FdoCommonMin(envelope->GetMinY(), aggregate_ordinates[i]);
      i++;
      if (geometry->GetDimensionality() & FdoDimensionality_Z)
          ordinates[i] =
                    FdoCommonMin(envelope->GetMinZ(), aggregate_ordinates[i]);

    }  //  else ...

    updated_linear_ring =
                geometry_factory->CreateLinearRing(
                        geometry->GetDimensionality() & ~FdoDimensionality_M,
                        i,
                        ordinates);
    extents = geometry_factory->CreatePolygon(updated_linear_ring, NULL);

    result_geometry_value_set = true;
    result_geometry_value     = FDO_SAFE_ADDREF(extents.p);

}  //  ProcessRequest ()

void FdoFunctionSpatialExtents::Validate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                count           = literal_values->GetCount();

    FdoPtr<FdoLiteralValue> literal_value;

    // Check the number of arguments. SPATIALEXTENTS accepts one parameter
    // only. If the number of parameters is not correct issue an exception.

    if (count != 1) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_SPATIALEXTENTS));

    // Next, identify the data type associated with the value to be processed.
    // An exception is issued if the data type does not match any of the ones
    // the function supports

    literal_value = literal_values->GetItem(0);
    if (literal_value->GetLiteralValueType() != FdoLiteralValueType_Geometry)
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_ERROR, 
                  "Expression Engine: Invalid parameters for function '%1$ls'",
                  FDO_FUNCTION_SPATIALEXTENTS));

}  //  Validate ()

