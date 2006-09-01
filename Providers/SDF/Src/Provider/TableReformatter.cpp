// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "TableReformatter.h"
#include "SdfConnection.h"

TableReformatter::TableReformatter( 
        FdoString* name, 
        SdfConnection* connection, 
        DataDb* origData, 
        KeyDb* origKeys, 
        SdfRTree* origRTree, 
        FdoFeatureSchema* newSchema
) :
    mName(name),
    mConnection(connection),
    mOrigData(origData),
    mOrigKeys(origKeys),
    mOrigRTree(origRTree),
    mNewSchema(newSchema),
    mModClassIds(false)
{
}


TableReformatter::~TableReformatter()
{
}

FdoString* TableReformatter::GetName()
{
    return mName;
}

void TableReformatter::Reformat()
{
    // Table already reformatted, nothing to do.
    if ( mState == stateFinal ) 
        return;

    // If there is something to do ...
    if ( GetModClassId() ) {

        // Create a backup table. As rows are updated, their original values
        // are written to the backup table. Rollbacks are performed by restoring
        // rows from the backup table
        SQLiteTable* backupTable = OpenBackupTable( true );

        SQLiteDataBase* env = mConnection->GetDataBase();
        bool transactionStarted = false;
        bool cursorOpened = false;

        // This is more for the future. Transaction functions are currently a no-op, hence the
        // need for the backup table.
        if ( !env->transaction_started() ) {
            if ( env->begin_transaction() != 0 ) 
                throw FdoSchemaException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_78_START_TRANSACTION)));

            transactionStarted = true;
        }

        try {
            FdoClassesP oldClasses = mConnection->GetSchema()->GetClasses();
            FdoClassesP newClasses = mNewSchema->GetClasses();

            SQLiteData key;
            SQLiteData data;


            // Get each row from the data table.

            int ret = mOrigData->GetFirstFeature( &key, & data );
            cursorOpened = true;
            REC_NO recNo = 0;

            for ( ; ; ) {
            
                if (ret == SQLiteDB_NOTFOUND)
                    break;

                if (ret != SQLiteDB_OK)
                    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));

		        if( key.get_size() == 4 )
			        recNo = *(REC_NO*)(key.get_data());
		        else
			        recNo++;

			    key.set_data(&recNo); // This is required since m_currentKey was pointing to a memory location that may get re-used/freed
                // Make a backup copy of the row
                if ( backupTable->put_exclusive( NULL, &key, &data, SQLiteDB_APPEND) != 0 )
                    throw FdoException::Create(
                        NlsMsgGetMain(
                            FDO_NLSID(SDFPROVIDER_86_WRITE_TABLE),
                            (FdoString*) GetBackupTableName()
                        )
                    );

                BinaryReader rdr( (unsigned char*)(data.get_data()), data.get_size() );
                unsigned short oldClassId = rdr.ReadUInt16();
                unsigned short newClassId;

                if ( GetModClassId() ) {
                    newClassId = CvtClassId( oldClassId, oldClasses, newClasses );
                    *((unsigned short*)(data.get_data())) = newClassId;
                }

                // Update the data table row
                mOrigData->UpdateFeature( recNo, &data );

			    key.set_data(&recNo); // This is required since m_currentKey was pointing to a memory location that may get re-used/freed
                ret = mOrigData->GetNextFeature( &key, &data );
            }
        }
        catch ( ... ) {
            if ( cursorOpened )
                mOrigData->CloseCursor();
            // Rollback on error
            if ( transactionStarted ) 
                env->rollback();

            throw;
        }

        mOrigData->Flush();
        mOrigData->CloseCursor();

        // Transaction handling currently a no-op
        if ( transactionStarted ) {
            // Successful, so commit schema changes.
            if ( env->commit() != 0 ) 
                throw FdoSchemaException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_79_COMMIT_TRANSACTION)));

        }
    }

    mState = stateFinal;
}

void TableReformatter::Rollback()
{
    if ( !(mState == stateFinal) ) 
        return;

    SQLiteTable* backupTable = OpenBackupTable( false);

    // Nothing to roll back if no backup table.
    if ( backupTable ) {
        SQLiteDataBase* env = mConnection->GetDataBase();
        bool transactionStarted = false;
        bool cursorOpened = false;

        if ( !env->transaction_started() ) {
            if ( env->begin_transaction() != 0 ) 
                throw FdoSchemaException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_78_START_TRANSACTION)));

            transactionStarted = true;
        }

        try {
            FdoClassesP oldClasses = mConnection->GetSchema()->GetClasses();
            FdoClassesP newClasses = mNewSchema->GetClasses();

            SQLiteCursor* cursor;
            if (  backupTable->cursor(0, &cursor, false) != 0 )
                throw FdoException::Create(
                    NlsMsgGetMain(
                        FDO_NLSID(SDFPROVIDER_87_OPEN_CURSOR),
                        (FdoString*) GetBackupTableName()
                    )
                );

            // Get each row from the backup table.
            int ret = cursor->first();;
            cursorOpened = true;
            REC_NO recNo = 0;

            for ( ; ; ) {
            
                if (ret == SQLiteDB_NOTFOUND)
                    break;

                if (ret != SQLiteDB_OK)
                    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));
                
                int keyLen;
                char* keyData;
                int dataLen;
                char* dataData;

                if ( cursor->get_key( &keyLen, &keyData ) != 0 )
                    throw FdoException::Create(
                        NlsMsgGetMain(
                            FDO_NLSID(SDFPROVIDER_88_ACCESS_CURSOR),
                            (FdoString*) GetBackupTableName()
                        )
                    );
                
                if ( cursor->get_data( &dataLen, &dataData, false ) != 0 )
                    throw FdoException::Create(
                        NlsMsgGetMain(
                            FDO_NLSID(SDFPROVIDER_88_ACCESS_CURSOR),
                            (FdoString*) GetBackupTableName()
                        )
                    );

                SQLiteData key( keyData, keyLen );
                SQLiteData data( dataData, dataLen );

		        if( key.get_size() == 4 )
			        recNo = *(REC_NO*)(key.get_data());
		        else
			        recNo++;

                // Replace the row in the data table.
                mOrigData->UpdateFeature( recNo, &data );

                ret = cursor->next();
            }
        }
        catch ( ... ) {
            if ( cursorOpened )
                backupTable->close_cursor();
            // Rollback on error
            if ( transactionStarted ) 
                env->rollback();

            throw;
        }

        mOrigData->Flush();
        mOrigData->CloseCursor();
        backupTable->close_cursor();

        if ( transactionStarted ) {
            // Successful, so commit schema changes.
            if ( env->commit() != 0 ) 
                throw FdoSchemaException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_79_COMMIT_TRANSACTION)));

        }

        mState = stateInitial;
    }
}

bool TableReformatter::GetModClassId()
{
    return mModClassIds;
}

void TableReformatter::SetModClassid( bool modClassId )
{
    mModClassIds = modClassId;
}

unsigned short TableReformatter::CvtClassId( 
    unsigned short srcClassId, 
    FdoClassCollection* srcClasses, 
    FdoClassCollection* destClasses 
)
{
    FdoClassDefinitionP srcClass = srcClasses->GetItem( srcClassId );
    FdoClassDefinitionP destClass = destClasses->GetItem( srcClass->GetName() );

    return destClasses->IndexOf( destClass );
}

PropertyIndex* TableReformatter::GetPropertyIndex( FdoClassDefinition* classDef )
{
#if 0
    PropertyIndex* pi = m_newPropertyIndexes[classDef];

    if ( !pi ) {
        FdoClassesP classes = mNewSchema->GetClasses();
        GisInt32 classIx = -1;
        GisInt32 idx;

        for ( idx = 0; idx < classes->GetCount(); idx++ ) {
            FdoClassDefinitionP currClass = classes->GetItem(idx);

            if ( curClass->GetElementState() != FdoSchemaElementState_Deleted ) {
                classIx++;
            
            if ( wcscmp(classDef->GetName(), currClass->GetName()) == 0 ) 
                break;
        }

        pi = new PropertyIndex( classdef, (unsigned) classIx );
        m_newPropertyIndexes[classDef] = pi;
    }
#endif
    return NULL /*pi*/;
}

SQLiteTable* TableReformatter::OpenBackupTable( bool bCreate ) 
{
    if ( bCreate ) 
        DropBackupTable();

    FdoStringP backupName = GetBackupTableName();

    SQLiteTable* backupTbl = new SQLiteTable(mConnection->GetDataBase());

    if ( backupTbl->open( 0, mConnection->GetFilename(), (const char*) backupName, bCreate ? SQLiteDB_CREATE : 0, false ) != 0 ) 
    {
        backupTbl->close(0);
        delete backupTbl;
        backupTbl = NULL;
        if ( bCreate ) 
            throw FdoException::Create(
                NlsMsgGetMain(
                    FDO_NLSID(SDFPROVIDER_89_OPEN_TABLE),
                    (FdoString*) backupName
                )
            );
    }

    return backupTbl;
}

void TableReformatter::DropBackupTable() 
{
    FdoStringP backupName = GetBackupTableName();

    SQLiteTable* backupTbl = new SQLiteTable(mConnection->GetDataBase());

    if ( backupTbl->open( 0, mConnection->GetFilename(), (const char*) backupName, 0, false ) != 0 ) 
    {
        backupTbl->close(0);
        backupTbl->Drop();
    }
    else {
        backupTbl->close(0);
    }

    delete backupTbl;
}

FdoStringP TableReformatter::GetBackupTableName()
{
    return FdoStringP(L"backup_") + mOrigData->GetDbName();
}
