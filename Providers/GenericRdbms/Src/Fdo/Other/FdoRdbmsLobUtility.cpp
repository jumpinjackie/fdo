/***************************************************************************
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
 ***************************************************************************/

#include "stdafx.h"

#include "FdoRdbmsLobUtility.h"
#include "FdoRdbmsSchemaUtil.h"
#include "Inc/ut.h"

///////////////////////////////////////////////////////////////////////////////
bool  FdoRdbmsLobUtility::ContainsLobs( const FdoSmLpClassDefinition *classDefinition )
{
    bool    hasLobs = false;

    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();

    for ( int i = 0; i < propertyDefinitions->GetCount() && !hasLobs; i++)
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(i);
        FdoPropertyType  propType = propertyDefinition->GetPropertyType();

        if ( propType == FdoPropertyType_DataProperty)
        {
            const FdoSmLpDataPropertyDefinition* dataProp =
                        static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);

            hasLobs = ( dataProp->GetDataType() == FdoDataType_BLOB );
        }
    }
    return hasLobs;
}

///////////////////////////////////////////////////////////////////////////////
bool  FdoRdbmsLobUtility::ContainsStreamedLobs( FdoPropertyValueCollection  *propValCollection )
{
    bool    exists = false;

    for ( int i = 0; !exists && i < propValCollection->GetCount(); i++ )
    {
        FdoPtr<FdoPropertyValue> propertyValue = propValCollection->GetItem( i );
        FdoPtr<FdoIStreamReader> streamReader = propertyValue->GetStreamReader();
        exists = ( streamReader != NULL );
    }
    return exists;
}

///////////////////////////////////////////////////////////////////////////////
void  FdoRdbmsLobUtility::FetchLobLocators( DbiConnection *connection,
                                            const FdoSmLpClassDefinition *classDefinition,
                                            FdoPropertyValueCollection  *propValCollection,
                                            FdoRdbmsPvcBindDef  *bindArray, int bindCount, int& lobQueryQid)
{
    int                 qid = -1;
    int                 ccode;
    FdoStringP          lobColsListString;      // lob1,lob2 ...
    FdoStringP          whereString;            // pk1=:1, pk2=:2 ...
    char                buffer[16];
    bool                first = true;
    int                 indexBind = 1;
    int                 numRows = 0;

    try
    {
        bool isFeatClass = (classDefinition->GetClassType() == FdoClassType_FeatureClass);

        // Build selected columns list
        for ( int j = 0; j < bindCount; j++ )
        {
            bindArray[j].pos = -1;  // Mark for skipping

            if ( bindArray[j].type != FdoDataType_BLOB)
                continue;

            // Check if BLOB by ref.
            const wchar_t *propertyName = bindArray[j].propertyName;
            FdoPtr<FdoPropertyValue> propertyValue;
            try
            {
                propertyValue = propValCollection->GetItem( propertyName );
            }
            catch( FdoException* ex )
            {
                // The BLOB might be NULL, valid case
                ex->Release();
            }

            if ( propertyValue && propertyValue->GetStreamReader() )
            {
                lobColsListString += first ? "" : ",";
                lobColsListString += connection->GetSchemaUtil()->Property2ColName( classDefinition->GetName(), propertyName );
                first = false;
            }
        }

        // Build WHERE clause based on primary keys
        if ( isFeatClass )
        {
            const FdoSmLpDataPropertyDefinition  *featIdProp = classDefinition->RefFeatIdProperty();

            if ( featIdProp == NULL )
                throw FdoException::Create(NlsMsgGet1(FDORDBMS_187, "Schema error; Feature class %1$ls has no feature id property", (const wchar_t*) classDefinition->GetQName() ));

            for ( int j = 0; j < bindCount; j++ )
            {
                const wchar_t *propertyName = bindArray[j].propertyName;
                if ( wcscmp( featIdProp->GetName(), propertyName ) == 0 )
                {
                    whereString += propertyName;
                    whereString += L"=:";
                    whereString += ut_itoa( indexBind, buffer );
                    bindArray[j].pos = indexBind++;
                    break;
                }
            }
        }
        else
        {
            first = false;
            const FdoSmLpDataPropertyDefinitionCollection *idProperties = classDefinition->RefIdentityProperties();
            for ( int j = 0; j < bindCount; j++ )
            {
                const wchar_t *propertyName = bindArray[j].propertyName;
                for ( int i = 0; i < idProperties->GetCount(); i++ )
                {
                    const FdoSmLpPropertyDefinition *idProperty = idProperties->RefItem(i);
                    if ( wcscmp( idProperty->GetName(), propertyName ) == 0 )
                    {
                        whereString += first? L"" : L" AND ";
                        whereString += propertyName;
                        whereString += L"=:";
                        whereString += ut_itoa( indexBind, buffer );
                        bindArray[j].pos = indexBind++;
                        break;
                    }
                }
            }
        }

        // Since this function is supposed to update an unique row (after insert or update)
        // the WHERE clause is mandatory
        if ( whereString == L"" )
            throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_388, "Schema error class '%1$ls': a row must be unique in order to insert a streamed LOB", (const wchar_t*) classDefinition->GetQName() ));

        FdoStringP  gql = FdoStringP::Format(L"SELECT %ls FROM %ls WHERE %ls FOR UPDATE",
                                             (const wchar_t *)lobColsListString, (const wchar_t *)classDefinition->RefDbObject()->GetName(), (const wchar_t *)whereString);

        (void) connection->dbi_gql( (char *)(const char *)(gql), &qid );

        // Define LOB locations and bind PKs
        for ( int j = 0; j < bindCount; j++ )
        {
            // Check if BLOB by ref.
            const wchar_t *propertyName = bindArray[j].propertyName;
            FdoPtr<FdoPropertyValue> propertyValue;

            try
            {
                propertyValue = propValCollection->GetItem( propertyName );
            }
            catch( FdoException* ex )
            {
                // The BLOB might be NULL, valid case
                ex->Release();
            }
            if ( propertyValue == NULL )
                continue;

            if ( bindArray[j].type == FdoDataType_BLOB && propertyValue->GetStreamReader() )
            {
                (void) connection->dbi_define( qid, (char*)connection->GetUtility()->UnicodeToUtf8(bindArray[j].propertyName), RDBI_BLOB_REF, sizeof(void *), (char*)&bindArray[j].value.strvalue, NULL, NULL);
            }
            else if ( bindArray[j].pos != -1)
            {
                connection->dbi_set_nnull((char *)&bindArray[j].null_ind, 0,0);

                connection->dbi_bind( qid, ut_itoa(bindArray[j].pos, buffer), RDBI_STRING, (int)strlen((char*)bindArray[j].value.strvalue)+1,
                                    (char *)bindArray[j].value.strvalue, (char *)&bindArray[j].null_ind, NULL );
            }
        }

        (void) connection->dbi_execute( qid, NULL, NULL);

        while ( TRUE )
        {
            (void) connection->dbi_fetch( qid, &ccode );

            if ( ccode == RDBI_END_OF_FETCH ) break;

            numRows++;

            // Fetch the lob locators
            for ( int j = 0; j < bindCount; j++ )
            {
                if ( bindArray[j].type == FdoDataType_BLOB && bindArray[j].reader != NULL )
                    (void) connection->dbi_get_val_b( qid, (char*)connection->GetUtility()->UnicodeToUtf8(bindArray[j].propertyName), sizeof(void *), (char*)&bindArray[j].value.strvalue, NULL, NULL);
            }
        }

        // The row to update must be unique
        if ( numRows != 1 )
            throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_388, "Schema error class '%1$ls': a row must be unique in order to insert a streamed LOB", (const wchar_t*) classDefinition->GetQName() ));

        // Copy to output
        lobQueryQid = qid;
    }
    catch( ... )
    {
        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsLobUtility::InsertStreamedLobs(   DbiConnection *connection,
                                               const FdoSmLpClassDefinition *classDefinition,
                                               FdoPropertyValueCollection  *propValCollection,
                                               FdoRdbmsPvcBindDef *bindArray, int bindCount )
{
    FdoInt32    BLOC_SIZE = 4096;
    FdoByte     *buffer = NULL;
    FdoInt32    count;
    int         qid = -1;

    // Quick return if nothing to do.
    if ( !ContainsStreamedLobs( propValCollection ) )
        return;

    try
    {
        // Fetch the lob locators for the current row.
        FetchLobLocators( connection, classDefinition, propValCollection, bindArray, bindCount, qid );

        // Identify the streamed properties
        for ( int i = 0; i < bindCount; i++)
        {
            FdoRdbmsPvcBindDef   boundProp = bindArray[i];

            if ( boundProp.type != FdoDataType_BLOB || boundProp.reader == NULL )
                continue;

            FdoPtr<FdoIoByteStreamReader> streamReader = (FdoIoByteStreamReader *)boundProp.reader;
            void   *lobRef = boundProp.value.strvalue;

            if ( buffer == NULL )
                buffer = new FdoByte[BLOC_SIZE];

            // Open LOB locator for write
            connection->dbi_lob_open( qid, lobRef, FALSE);

            streamReader->Reset();

            // Read from the stream and write to the LOB
            while ( count = streamReader->ReadNext( buffer, 0, BLOC_SIZE ) )
            {
                connection->dbi_lob_write_next( qid, lobRef, RDBI_BLOB, count, (char *)buffer );
            }

            // Close LOB locator
            connection->dbi_lob_close( qid, lobRef );
            connection->dbi_lob_destroy_ref( qid, lobRef );
            boundProp.value.strvalue = NULL;

            FDO_SAFE_RELEASE(boundProp.reader );
            boundProp.reader = NULL;
        }

        if ( qid != -1 )
            connection->dbi_free( qid, NULL );

        if ( buffer )
            delete[] buffer;
    }
    catch (...)
    {
        if ( qid != -1 )
        {
            FreeLobLocators( connection, bindArray, bindCount, qid );
            connection->dbi_free( qid, NULL );
        }
        if ( buffer )
            delete[] buffer;

        throw;
    }
}

////////////////////////////////////////////////////////////////////////////////
void FdoRdbmsLobUtility::FreeLobLocators( DbiConnection *connection, FdoRdbmsPvcBindDef  *bindArray, int bindCount, int qid )
{
    try
    {
        for ( int i = 0; i < bindCount; i++ )
        {
            FdoRdbmsPvcBindDef   boundProp = bindArray[i];

            // Exclude any other streamed types except BLOBs (for now)
            if ( boundProp.type != FdoDataType_BLOB || boundProp.value.strvalue == NULL || boundProp.reader == NULL )
                continue;

            connection->dbi_lob_destroy_ref( qid, boundProp.value.strvalue );

            FDO_SAFE_RELEASE(boundProp.reader );

            boundProp.value.strvalue = NULL;
            boundProp.reader = NULL;
        }
    }
    catch (...)
    {
        // Don't mask the previous exception
    }
}
