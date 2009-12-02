/*
 * 
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
 * Revision Control Modification History
 *
 *         $Id:  $
 *     $Author:  $
 *   $DateTime:  $
 *     $Change:  $
 *
 */

#include "stdafx.h"
#include "TestCommonSchemaUtil.h"
#include "TestCommonMiscUtil.h"


// Delete a class (if bDeleteRowsOnly==false) or delete the rows of a class (if bDeleteRowsOnly==true):
void TestCommonSchemaUtil::CleanUpClass(FdoIConnection *connection, const wchar_t* schema_name, const wchar_t* class_name, bool bDeleteRowsOnly)
{
    try
    {
        FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (class_name);
	    FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        // NOTE: in case the schema_name no longer exists, we dont specify it up front because it will cause an exception:
	    //if (NULL!=schema_name)
		//    descSchema->SetSchemaName(schema_name);
	    FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();

	    bool bFound = false;
	    for (FdoInt32 iSchemaIndex = 0; iSchemaIndex<schemas->GetCount(); iSchemaIndex++)
	    {
		    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(iSchemaIndex);
		    if ((NULL==schema_name) || (0==wcscmp(schema->GetName(), schema_name)))
		    {
			    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
			    for (FdoInt32 iClassIndex = 0; iClassIndex<classes->GetCount(); iClassIndex++)
			    {
				    FdoPtr<FdoClassDefinition> classDef = classes->GetItem(iClassIndex);
				    if (0==FdoCommonOSUtil::wcsicmp(classDef->GetName(), identifier->GetName ()))
				    {
					    bFound = true;
                        // delete the rows of this class, but not the class itself:
                        FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)connection->CreateCommand (FdoCommandType_Delete);
                        deleteCmd->SetFeatureClassName(classDef->GetName());
                        deleteCmd->Execute();

                        if (!bDeleteRowsOnly)
						{
                             // delete the entire class:
					        classDef->Delete();
					        FdoPtr<FdoIApplySchema> applySchema = (FdoIApplySchema*)connection->CreateCommand(FdoCommandType_ApplySchema);
					        applySchema->SetFeatureSchema(schema);
					        applySchema->Execute();
                        }

					    break;
				    }
			    }
		    }

		    if (bFound)
			    break;
	    }

	    // recurse with default schema, if class not found:
	    if (!bFound && (NULL!=schema_name) && (wcslen(schema_name)>0))
		    CleanUpClass(connection, NULL, class_name);
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}
