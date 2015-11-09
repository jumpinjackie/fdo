// 
// Copyright (C) 2008  Autodesk, Inc.
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

#include "stdafx.h"

// Function Declarations
void SelectCities();

// Application Entry Point
int _tmain(int argc, _TCHAR* argv[])
{
    SelectCities();
    return 0;
}

// Run a Select test to return names and locations of cities in Canada
void SelectCities()
{
    try
    {
        // Create a new ODBC Provider connection
        FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
        FdoPtr<FdoIConnection> connection  = manager->CreateConnection (L"OSGeo.ODBC.3.3");

        // Set the ODBC Provider connection configuration file
        FdoPtr<FdoIoStream> stream = FdoIoFileStream::Create(L"Cities.xml", L"r");
        connection->SetConfiguration(stream);

        // Set the ODBC Provider connection parematers
        connection->SetConnectionString(L"DataSourceName=Cities");

        // Open the ODBC Provider connection
        connection->Open();

        // Create a select command
        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(connection->CreateCommand(FdoCommandType_Select));

        // Set the class the query will be executed against
        selectCmd->SetFeatureClassName(L"World:Cities");

        // Retrieve the ordering property collection
        FdoPtr<FdoIdentifierCollection> orderingIds = selectCmd->GetOrdering();
        orderingIds->Clear();

        // Set the NAME property as the ordering property
        FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(L"Name");
        orderingIds->Add(id);

        // Set the ordering option
        selectCmd->SetOrderingOption(FdoOrderingOption_Ascending);

        // Get the collection of properties the select command will return
        FdoPtr<FdoIdentifierCollection> propIDs = selectCmd->GetPropertyNames();
        propIDs->Clear();

        // Restrict the set of retrieved properties
        FdoPtr<FdoIdentifier> propID1 = FdoIdentifier::Create(L"Name");
        FdoPtr<FdoIdentifier> propID2 = FdoIdentifier::Create(L"Geometry");
        propIDs->Add(propID1);
        propIDs->Add(propID2);

        // Create a comparison filter: Country == 'Canada'
        FdoPtr<FdoIdentifier> propID = FdoIdentifier::Create(L"Country");
        FdoPtr<FdoDataValue> dataValue = FdoStringValue::Create(L"Canada");
        FdoPtr<FdoComparisonCondition> compCond = FdoComparisonCondition::Create(propID, FdoComparisonOperations_EqualTo, dataValue);

        // Set the filter on the command
        selectCmd->SetFilter(compCond);

        // Execute command:
        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

        // Print out the names of the fetched cities
        printf("\nCities in Canada...\n");

        // Iterate results:
        while (reader->ReadNext())
        {
            // Retrieve the name of the city
            FdoString* sName = reader->GetString(L"Name");

            // Retrieve the location of the city
            FdoPtr<FdoByteArray> byteArray = reader->GetGeometry(L"Geometry");

            // Convert the byte array to a geometry
            FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> geometry = factory->CreateGeometryFromFgf(byteArray);

            // Initialize coordinate values
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;

            // Determine if the geometry is of type 'Point'
            FdoGeometryType geomType = geometry->GetDerivedType();
            if (geomType == FdoGeometryType_Point)
            {
                // Cast to a point
                FdoIPoint *point = (FdoIPoint *)geometry.p;

                // Retrieve the underlying position
                FdoPtr<FdoIDirectPosition> position = point->GetPosition();

                // Retrieve the Latitude/Longitude
                x = position->GetX();
                y = position->GetY();

                // Here is how you can check to see if there is a Z coordinate. 
                FdoInt32 dimensionality = position->GetDimensionality();
                if (dimensionality & FdoDimensionality_Z) {
                    z = position->GetZ();
                }
            }
    
            // List each city retrieved
            printf("\nCity: %ls;\t Latitude:%g;\t Longitude: %g", (FdoString*)sName, y, x);
        }

        // Close the ODBC Provider connection
        connection->Close();
    }
    catch (FdoException *e)
    {
        // Report any errors
        printf("\nFailure: %ls\n", e->GetExceptionMessage());
    }
}

