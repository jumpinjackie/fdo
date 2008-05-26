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
#include "FdoRdbmsSchemaUtil.h"

#include <malloc.h>
#ifdef _WIN32
#include <CRTDBG.H>
#endif

// FdoRdbmsSelectCommand
#include "FdoRdbmsSelectAggregates.h"
#include "FdoRdbmsFilterProcessor.h"
#include <FdoCommonExpressionExecutor.h>
#include <FdoCommonSchemaUtil.h>
#include <FdoExpressionEngine.h>
#include "Util/FdoExpressionEngineUtilDataReader.h"

FdoRdbmsSelectAggregates::FdoRdbmsSelectAggregates ()
: mbDistinct(false),
  mIConnection(NULL)
{
    pSelect = new FdoRdbmsSelectCommand();
}

FdoRdbmsSelectAggregates::FdoRdbmsSelectAggregates (FdoIConnection *connection)
: mbDistinct(false)
{
    mIConnection = connection;
    pSelect = new FdoRdbmsSelectCommand(connection);
}

// The original implementation of the command delegated the request to the
// select-command implementation and converted the returned feature reader
// into a data reader afterwards. However, with the addition of new expression
// functions this is no longer possible. The case where an expression is used
// either as part of a computed property being returned or as part of the filter
// that does not have native support needs to be treated separately. Otherwise
// the delegation to the select-command is still issued.
FdoIDataReader *FdoRdbmsSelectAggregates::Execute ()
{

    if (pSelect.p == NULL)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, errorMsg ));

    // Define some needed connection cbjects.

    if (mIConnection == NULL)
        throw FdoCommandException::Create(
                        NlsMsgGet(FDORDBMS_13, "Connection not established"));
    
    DbiConnection      *dbiConnection   = NULL;
    FdoRdbmsConnection *rdbmsConnection =
                                static_cast<FdoRdbmsConnection*>(mIConnection);
    if (rdbmsConnection)
        dbiConnection = rdbmsConnection->GetDbiConnection();

    FdoFilter                       *filter = pSelect.p->GetFilterRef();
    FdoIdentifier                   *cName  = pSelect.p->GetClassNameRef();
    FdoPtr<FdoIdentifierCollection> idCol   = pSelect.p->GetPropertyNames();

    const FdoSmLpClassDefinition *classDefinition =
                dbiConnection->GetSchemaUtil()->GetClass(cName->GetText());
    // Verify if this is a special case we can optimize (no filter, no grouping filter,
    // and only aggregate functions Count() and/or SpatialExtents())
    // Check if we can optimize first, since property list and filter
    // might be supported in the optimized case but not the non-optimized 
    // case.
    FdoPtr<FdoRdbmsFeatureReader> optReader = pSelect->GetOptimizedFeatureReader( classDefinition );
    if (optReader)
        return new FdoRdbmsDataReader(optReader);

    // Get a filter processor and check the property list and filter.

    FdoPtr<FdoRdbmsFilterProcessor>filterProcessor =
                                        rdbmsConnection->GetFilterProcessor();

    bool isValidFilter     = true,
         isValidSelectList = true;

    if (filter != NULL )
        isValidFilter = filterProcessor->IsValidExpression(filter);
    isValidSelectList = filterProcessor->IsValidExpression(idCol);

    // If the filter or the selection list contain expression functions that
    // do not have native support, execute the special case and return the
    // generated data reader.

    if ((!isValidFilter) || (!isValidSelectList)) {

        // Either the selected property list of the the filter is invalid. In
        // any case create a SQL statement that selects all properties for the
        // current class. If the filter is valid it is used to narrow the
        // selected amount of data.

        bool isFeatureClass =
              ((classDefinition != NULL) &&
               (classDefinition->GetClassType() == FdoClassType_FeatureClass));

        FdoString *sqlStatement =
                            filterProcessor->FilterToSql(
                                            ((isValidFilter) ? filter : NULL),
                                            cName->GetText());

        GdbiQueryResult *queryRslt =
                dbiConnection->GetGdbiConnection()->ExecuteQuery(sqlStatement);
        FdoPtr<FdoIFeatureReader> featureReader = 
                        new FdoRdbmsFeatureReader(
                                    FdoPtr<FdoIConnection>(GetConnection()),
                                    queryRslt,
                                    isFeatureClass,
                                    classDefinition,
                                    NULL,
                                    NULL,
                                    0,
                                    NULL);

        // The Expression Engine cannot work with a class definition of type
        // "FdoSmLpClassDefinition". Instead it is necessary to get the corres-
        // ponding definition of type "FdoClassDefinition". This is done next.

        const FdoSmLpSchema *schema = dbiConnection->GetSchema(
                                                            cName->GetText());
        FdoFeatureSchemasP fdoFeatureSchemas =
                rdbmsConnection->GetSchemaManager()->GetFdoSchemas(
                                                            schema->GetName());
        FdoClassCollection *classCol =
            (FdoClassCollection *)fdoFeatureSchemas->FindClass(
                                                            cName->GetText());
        FdoClassDefinition *classDef = classCol->GetItem(0);

        // Create a list of the aggregate functions.

		FdoCommonExpressionType exprType;
		FdoPtr<FdoIExpressionCapabilities> expressionCaps =
                                    mIConnection->GetExpressionCapabilities();
		FdoPtr<FdoFunctionDefinitionCollection> functions =
                                                expressionCaps->GetFunctions();
		FdoPtr< FdoArray<FdoFunction*> > aggrIdents =
            FdoExpressionEngineUtilDataReader::GetAggregateFunctions(
                                                                    functions,
                                                                    idCol,
                                                                    exprType);
        return new FdoExpressionEngineUtilDataReader(functions,
                                                     featureReader,
                                                     classDef,
                                                     idCol,
                                                     mbDistinct,
                                                     GetOrdering(),
                                                     GetOrderingOption(),
                                                     idCol,
                                                     aggrIdents);

    }  //  if ((!isValidFilter) || (!isValidSelectList)) ...

    // The filter and the select list contain only natively supported expression
    // functions (if any). Delegate the request to the select-command.

    return new FdoRdbmsDataReader(
                FdoPtr<FdoRdbmsFeatureReader>(
                            (FdoRdbmsFeatureReader*)pSelect->Execute(
                                        mbDistinct,
                                        FdoCommandType_SelectAggregates)));

}
