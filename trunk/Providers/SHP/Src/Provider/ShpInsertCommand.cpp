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
 */

#include "stdafx.h"
#include <FdoCommonFile.h>

ShpInsertCommand::ShpInsertCommand (FdoIConnection *connection) :
    FdoCommonFeatureCommand<FdoIInsert, ShpConnection> (connection)
{
//	m_LastFileSet = NULL;
}

/** Do not implement the copy constructor. **/
//ShpInsertCommand::ShpInsertCommand (const ShpInsertCommand &right) { }

ShpInsertCommand::~ShpInsertCommand (void)
{
	FdoPtr<ShpConnection> connection = (ShpConnection *)GetConnection();
	if ( connection->GetLastEditedFileSet() && (connection->GetConnectionState() == FdoConnectionState_Open))
		connection->GetLastEditedFileSet()->ReopenFileset( FdoCommonFile::IDF_OPEN_READ );
}


/// <summary>Gets the FdoBatchParameterValueCollection that can be used for optimized
/// batch inserts of multiple features with a single insert command. Batch
/// inserts can be performed by using Parameters for each of the property
/// values, then adding collections of parameter values to the
/// FdoBatchParameterValueCollection. Each FdoParameterValueCollection in the
/// FdoBatchParameterValueCollection should contain one FdoParameterValue for each
/// of the parameters specified for property values.</summary>
/// <returns>Returns FdoBatchParameterValueCollection</returns>
FdoBatchParameterValueCollection* ShpInsertCommand::GetBatchParameterValues ()
{
    if (mBatchParameters == NULL)
        mBatchParameters = FdoBatchParameterValueCollection::Create();

    return (FDO_SAFE_ADDREF(mBatchParameters.p));
}


/// <summary>Executes the insert command and returns a reference to an
/// FdoIFeatureReader. Some feature providers can generate automatic identity
/// values for features. This will happen automatically as the features are
/// inserted. The returned FdoIFeatureReader allows the client to obtain the
/// automatic identity property value(s) of newly inserted object(s). The
/// returned feature reader at a minimum will read the unique identity
/// properties of the objects just inserted. Multiple objects will be
/// returned through the reader in the case of a batch insert.</summary>
/// <returns>Returns an FdoIFeatureReader</returns> 
FdoIFeatureReader* ShpInsertCommand::Execute ()
{
    FdoPtr<ShpConnection> connection;
    FdoString* class_name;
    FdoPtr<FdoIdentifierCollection> id;
    FdoStringP name;
    FdoPtr<FdoIdentifier> featid;
    ShpFileSet* fileset;
	ShpFileSet* lastFileset;
    int first;
    int last;
    FdoPtr<FdoFilter> filter;
    FdoPtr<ShpFeatureReader> ret;

    connection = (ShpConnection*)GetConnection ();
    class_name = FdoPtr<FdoIdentifier> (GetFeatureClassName ())->GetName ();

    // create a collection of identity property identifiers
    name = ShpSchemaUtilities::GetIdentityPropertyName (connection, class_name);
    featid = FdoIdentifier::Create (name);
    id = FdoIdentifierCollection::Create ();
    id->Add (featid);
    // get the next feature id
    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition (connection, class_name);
    FdoPtr<FdoClassDefinition> logicalClass = lpClass->GetLogicalClass();
    fileset = lpClass->GetPhysicalFileSet ();
	lastFileset = connection->GetLastEditedFileSet();

	// Attempt to open the fileset in write mode. It fails in case other process is doing update.
	try
	{
		// Give up the write permissions if the fileset changed
		if ( lastFileset && ( lastFileset != fileset ) )
			lastFileset->ReopenFileset( FdoCommonFile::IDF_OPEN_READ );

		fileset->ReopenFileset( FdoCommonFile::IDF_OPEN_UPDATE );
	}
	catch (FdoException* ex)
	{
		// Revert to read-only state the ones changed above
		fileset->ReopenFileset( FdoCommonFile::IDF_OPEN_READ );
		throw ex;
	}

	connection->SetLastEditedFileSet(fileset);
	first = fileset->GetNumRecords ();

    // perform the actual insert
    if ((mBatchParameters == NULL) || (mBatchParameters->GetCount()==0))
    {
        FdoPtr<FdoPropertyValueCollection> values;
        values = GetPropertyValues ();

        FdoCommonMiscUtil::HandleReadOnlyAndDefaultValues(logicalClass, values, true);

        // ESRI numbers their shapes starting at 1
        Shape* shape = NullShape::NewNullShape (first + 1);
        RowData* rowData = fileset->GetDbfFile ()->NewRow ();

        fileset->PutData (FdoPtr<ShpConnection>((ShpConnection*)GetConnection()), FdoPtr<FdoIdentifier> (GetFeatureClassName ())->GetName(), 
            values, rowData, shape, false);
    }
    else
    {
        FdoPtr<FdoPropertyValueCollection>  propertyValues = FdoPropertyValueCollection::Create();
        FdoPtr<FdoPropertyValue>            propertyValue;
        FdoPtr<FdoPropertyValue>            propertyToParamValue;
        FdoPtr<FdoParameterValueCollection> parameterValues;
        FdoPtr<FdoParameterValue>           parameterValue;

        // insert all the rows of data
        for (int row = 0; row < mBatchParameters->GetCount (); row++)
        {
            parameterValues = mBatchParameters->GetItem(row);

            // skip empty parameter collections:
            if (parameterValues->GetCount() == 0)
                continue;

            // map properties to parameter values, storing the mapping in a FdoParameterValueCollection:
            propertyValues->Clear();
            FdoInt32 values = mValues->GetCount ();
            for (int propValIndex=0; propValIndex < values; propValIndex++)
            {
                propertyToParamValue = mValues->GetItem(propValIndex);

                FdoPtr<FdoIdentifier> identifier = propertyToParamValue->GetName ();
                FdoPtr<FdoParameter> paramExpression = dynamic_cast<FdoParameter*>(propertyToParamValue->GetValue ());
                if (paramExpression == NULL)
                    throw FdoException::Create (NlsMsgGet(SHP_EXPRESSION_INVALID_PARAMETER_TYPE, "Invalid parameter type."));
                parameterValue = parameterValues->FindItem (paramExpression->GetName ());
                if (parameterValue == NULL)
                    propertyValue = FdoPropertyValue::Create (identifier, NULL);
                else
                {
                    FdoPtr<FdoLiteralValue> value = parameterValue->GetValue ();
                    propertyValue = FdoPropertyValue::Create (identifier, value);
                }
                propertyValues->Add(propertyValue);
            }

            FdoCommonMiscUtil::HandleReadOnlyAndDefaultValues(logicalClass, propertyValues, true);

            // insert the row

            // ESRI numbers their shapes starting at 1
            Shape* shape = NullShape::NewNullShape (first + row + 1);
            RowData* rowData = fileset->GetDbfFile ()->NewRow ();

            fileset->PutData (FdoPtr<ShpConnection>((ShpConnection*)GetConnection()), FdoPtr<FdoIdentifier> (GetFeatureClassName ())->GetName(), 
                propertyValues, rowData, shape, true);
        }
        fileset->Flush ();
    }

    // get the new next feature id
    last = fileset->GetNumRecords ();

    // create a filter for the range of features added
    filter = FdoBinaryLogicalOperator::Create (
        FdoPtr<FdoComparisonCondition> (FdoComparisonCondition::Create (
            featid,
            FdoComparisonOperations_GreaterThanOrEqualTo,
            FdoPtr<FdoInt32Value> (FdoInt32Value::Create (first+1)))),  // featid are 0-based internally but 1-based externally (to FDO users).
        FdoBinaryLogicalOperations_And,
        FdoPtr<FdoComparisonCondition> (FdoComparisonCondition::Create (
            featid,
            FdoComparisonOperations_LessThan,
            FdoPtr<FdoInt32Value> (FdoInt32Value::Create (last+1)))));  // featid are 0-based internally but 1-based externally (to FDO users).

    ret = new ShpFeatureReader (connection, class_name, filter, id);

    return (FDO_SAFE_ADDREF (ret.p));
}

