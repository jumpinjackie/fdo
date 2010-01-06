/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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

#include "stdafx.h"
#include "TestCommonExpressionFunctionR.h"

// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

TestCommonExpressionFunctionR::TestCommonExpressionFunctionR ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

}  //  TestCommonExpressionFunctionR ()

TestCommonExpressionFunctionR::~TestCommonExpressionFunctionR ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
}  //  ~TestCommonExpressionFunctionR ()

// ----------------------------------------------------------------------------
// --                      Alternative tests for providers that don't        --
// --                      ApplySchema. Provider must supply pre-created     --
// --                      test data.                                        --
// ----------------------------------------------------------------------------

void TestCommonExpressionFunctionR::doTestXYZMRdOnly( FdoString* className, FdoString* geomName )
{
   // Declare and initialize all necessary local vatiables.


    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: X, Y, Z, M Function Testing      \n");
    printf("========================================================== \n");
    printf("\n");

    CheckXYZMCapabilities();

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function X on a geometry value. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

        FdoPtr<FdoISelect> select = (FdoISelect*)m_connection->CreateCommand(FdoCommandType_Select); 
        FdoPtr<FdoIdentifierCollection> ids = select->GetPropertyNames();
        ids->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(geomName)));
    
        FdoPtr<FdoExpression> expr = FdoExpression::Parse(
            FdoStringP::Format(L"X(%ls)", geomName)
        );
        FdoPtr<FdoComputedIdentifier> compIdent = FdoComputedIdentifier::Create(L"X", expr );
        ids->Add(compIdent);

        expr = FdoExpression::Parse(
            FdoStringP::Format(L"Y(%ls)", geomName)
        );
        compIdent = FdoComputedIdentifier::Create(L"Y", expr );
        ids->Add(compIdent);

        expr = FdoExpression::Parse(
            FdoStringP::Format(L"Z(%ls)", geomName)
        );
        compIdent = FdoComputedIdentifier::Create(L"Z", expr );
        ids->Add(compIdent);

        expr = FdoExpression::Parse(
            FdoStringP::Format(L"M(%ls)", geomName)
        );
        compIdent = FdoComputedIdentifier::Create(L"M", expr );
        ids->Add(compIdent);

        select->SetFeatureClassName(className);

    	FdoPtr<FdoIFeatureReader> rdr = select->Execute();

        int nnullPtCount = 0;

        while ( rdr->ReadNext() ) {
            if ( !rdr->IsNull(geomName) )
            {
                FdoPtr<FdoByteArray> geomVal = rdr->GetGeometry(geomName);
                FdoPtr<FdoIGeometry> geomObj = gf->CreateGeometryFromFgf(geomVal);
                FdoGeometryType geomType = geomObj->GetDerivedType();

                if ( geomType == FdoGeometryType_Point )
                {
                    FdoIPoint* point = static_cast<FdoIPoint*>(geomObj.p);
                    FdoInt32 dim = point->GetDimensionality();
                    FdoPtr<FdoIDirectPosition> pos = point->GetPosition();
                    CheckXYZMDim( rdr, L"X", pos->GetX(), false );
                    CheckXYZMDim( rdr, L"Y", pos->GetY(), false  );
                    CheckXYZMDim( rdr, L"Z", pos->GetZ(), !(dim & FdoDimensionality_Z)  );
                    CheckXYZMDim( rdr, L"M", pos->GetM(), !(dim & FdoDimensionality_M) );
                    nnullPtCount++;
                }
                else
                {
                    if ( (!rdr->IsNull(L"X")) || (!rdr->IsNull(L"Y")) || (!rdr->IsNull(L"Z")) || (!rdr->IsNull(L"M")) )
                        throw FdoException::Create( L"XYZM must all be null for non-point geometry" );
                }
            }
            else {
                if ( (!rdr->IsNull(L"X")) || (!rdr->IsNull(L"Y")) || (!rdr->IsNull(L"Z")) || (!rdr->IsNull(L"M")) )
                    throw FdoException::Create( L"XYZM must all be null for null geometry" );
            }
        }

        if ( nnullPtCount <= 0 ) 
            throw FdoException::Create( L"Test data did not have any not-null point geometries" );

    }  //  try ...

    catch (FdoException *exp) {


        printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...
}

