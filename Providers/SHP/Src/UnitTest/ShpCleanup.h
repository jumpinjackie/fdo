/*
 * 
 * Copyright (C) 2018  Autodesk, Inc.
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
#ifndef SHP_CLEANUP_H
#define SHP_CLEANUP_H

#include "Fdo.h"
#include <FdoCommonFile.h>

class ShpCleanup
{
  public:
    static void CleanupClass(FdoIConnection *conn, FdoString* schemaName, FdoString *className)
    {
        try
        {
            TestCommonSchemaUtil::CleanUpClass(conn, schemaName, className);
        }
        catch (FdoException * ge)
        {
            printf(" >>> Exception in tearDown() with className=%ls : %ls\n", (FdoString*)className, ge->GetExceptionMessage());
        }
        catch (...)
        {
            printf(" >>> Exception in tearDown() with className=%ls : The tearDown failed for an unknown reason \n", (FdoString*)className);
        }

        FdoPtr<FdoIConnectionInfo> connInfo = conn->GetConnectionInfo();
        FdoPtr<FdoStringCollection> depFiles = connInfo->GetDependentFileNames();

        //Collect the files to be deleted
        FdoPtr<FdoStringCollection> toDelete = FdoStringCollection::Create();
        for (FdoInt32 i = 0; i < depFiles->GetCount(); i++)
        {
            FdoPtr<FdoStringElement> el = depFiles->GetItem(i);
            FdoStringP str = el->GetString();
            FdoStringP test = FdoStringP::Format(L"%S.", className);
            if (str.Contains(test))
            {
                toDelete->Add(str);
            }
        }

        //Close the connection first
        conn->Close();

        FdoInt32 toDeleteCount = toDelete->GetCount();
        if (toDeleteCount > 0)
        {
            printf(" >>> Incomplete cleanup of class (%S). Found the following dependent files still present\n", className);
            for (FdoInt32 i = 0; i < toDeleteCount; i++)
            {
                FdoPtr<FdoStringElement> el = toDelete->GetItem(i);
                FdoStringP fileName = el->GetString();
                if (FdoCommonFile::FileExists(fileName))
                {
                    FdoCommonFile::Delete(fileName);
                    printf("  >>> Deleted (%S)\n", (FdoString *)fileName);
                }
                else
                {
                    printf("  >>> Skipped file (%S). Closing the connection must've deleted the file in question\n", (FdoString *)fileName);
                }
            }
        }

        //Now re-open (just in case something further down is operating on the still-assumed-to-be-open connection)
        conn->Open();
    }
};

#endif