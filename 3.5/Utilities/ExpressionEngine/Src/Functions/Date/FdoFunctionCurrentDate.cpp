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
#include <Functions/Date/FdoFunctionCurrentDate.h>

#include <time.h>

// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoFunctionCurrentDate::FdoFunctionCurrentDate ()

// +---------------------------------------------------------------------------
// | The class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize all class variables.

    function_definition = NULL;

    is_validated        = false;

}  //  FdoFunctionCurrentDate ()


FdoFunctionCurrentDate::~FdoFunctionCurrentDate ()

// +---------------------------------------------------------------------------
// | The class destructor.
// +---------------------------------------------------------------------------

{

    // Delete the function definition.

    FDO_SAFE_RELEASE(function_definition);

}  //  ~FdoFunctionCurrentDate ()


// ----------------------------------------------------------------------------
// --                            Public Class APIs                           --
// ----------------------------------------------------------------------------

FdoFunctionCurrentDate *FdoFunctionCurrentDate::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionCurrentDate();

}  //  Create ()

FdoFunctionCurrentDate *FdoFunctionCurrentDate::CreateObject ()

// +---------------------------------------------------------------------------
// | The function creates a new instance of the class.
// +---------------------------------------------------------------------------

{

    return new FdoFunctionCurrentDate();

}  //  CreateObject ()

FdoFunctionDefinition *FdoFunctionCurrentDate::GetFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The function creates the supported signature list for the function ADD-
// | MONTHS.
// +---------------------------------------------------------------------------

{

    if (function_definition == NULL)
        CreateFunctionDefinition();

    return FDO_SAFE_ADDREF(function_definition);

}  //  GetFunctionDefinition ()

FdoLiteralValue *FdoFunctionCurrentDate::Evaluate (
                                    FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function processes a call to the function CURRENTDATE.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoDateTime dt;

    struct tm   local_time;

    // If this is the first call to this function validate the arguments and
    // initialize some member variables used to process the request.

    if (!is_validated) {

        Validate(literal_values);
        result       = FdoDateTimeValue::Create();
        is_validated = true;

    }  //  if (!is_validated) ...

    // Get the local system time and copy the data into the date/time object.

    FdoCommonOSUtil::getsystime(&local_time);

    // NOTE: By default, the year will always be returned as an offset to 1900
    //       and hence this needs to be added to get the current year. Also,
    //       the month information is based on a 0-based vector and hence by
    //       default returns a value that is one month off. This needs to be
    //       corrected as well.

    dt.year    = local_time.tm_year + 1900;
    dt.month   = local_time.tm_mon + 1;
    dt.day     = local_time.tm_mday;
    dt.hour    = local_time.tm_hour;
    dt.minute  = local_time.tm_min;
    dt.seconds = (float)local_time.tm_sec;

    // Return the requested information.

    result->SetDateTime(dt);
    return FDO_SAFE_ADDREF(result.p);

}  //  Evaluate ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoFunctionCurrentDate::CreateFunctionDefinition ()

// +---------------------------------------------------------------------------
// | The procedure creates the function definition for the function CURRENT-
// | DATE. The function definition includes the list of supported signatures.
// | The following signatures are supported:
// |
// |    CURRENTDATE ()
// |
// | The function always returns a DATETIME.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoString                        *desc          = NULL;

    FdoPtr<FdoSignatureDefinition>   signature;
    FdoPtr<FdoSignatureDefinitionCollection> signatures;

    // Create the signature collection.

    signatures = FdoSignatureDefinitionCollection::Create();

    signature = FdoSignatureDefinition::Create(FdoDataType_DateTime, NULL);
    signatures->Add(signature);

    // Create the function definition.

    desc = FdoException::NLSGetMessage(FUNCTION_CURRENTDATE,
                                       "Returns the current date");
    function_definition =
                FdoFunctionDefinition::Create(
                                        FDO_FUNCTION_CURRENTDATE,
                                        desc,
                                        false,
                                        signatures,
                                        FdoFunctionCategoryType_Date);

}  //  CreateFunctionDefinition ()

void FdoFunctionCurrentDate::Validate (FdoLiteralValueCollection *literal_values)

// +---------------------------------------------------------------------------
// | The function validates the argument list that was passed in.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32 count = literal_values->GetCount();

    // Check the number of arguments. CURRENTDATE does not accept any arguments
    // and hence an exception needs to be issued if the number of parameters is
    // incorrect.

    if (count > 0) 
        throw FdoException::Create(
                FdoException::NLSGetMessage(
                  FUNCTION_PARAMETER_NUMBER_ERROR, 
                  "Expression Engine: Invalid number of parameters for function '%1$ls'",
                  FDO_FUNCTION_CURRENTDATE));

}  //  Validate ()

