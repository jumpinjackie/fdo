#ifndef FDORDBMSINSERTCOMMAND_H
#define FDORDBMSINSERTCOMMAND_H     1
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

#include "FdoRdbms.h"

// FeatureCommand
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include "Fdo/Pvc/FdoRdbmsPvcProcessor.h"

#define  LOCAL_BUFFER_SIZE      GDBI_SCHEMA_ELEMENT_NAME_SIZE + 20   // We only need a slightly larger buffer than the maximum schema element size

class FdoRdbmsException;

class FdoRdbmsInsertCommand : public FdoRdbmsCommand<FdoIInsert>
{

    friend class FdoRdbmsConnection;

  private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
      FdoRdbmsInsertCommand(const FdoRdbmsInsertCommand &right);

      FdoRdbmsInsertCommand ();

      FdoRdbmsInsertCommand (FdoIConnection *connection);

      FdoRdbmsException* CheckForNotNullProperties(const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection  *propValCollection );

  protected:
      virtual ~FdoRdbmsInsertCommand();

  public:

      //
      // Prevent the use of the Assignment Operation by definning it and not implemeting it.
      // DO NOT IMPLEMENT
      FdoRdbmsInsertCommand & operator=(const FdoRdbmsInsertCommand &right);

      //    Executes the insert command.
      virtual FdoIFeatureReader* Execute ();

      virtual FdoPropertyValueCollection* GetPropertyValues();

      virtual FdoBatchParameterValueCollection* GetBatchParameterValues()
      {
#if 0
        if (mBatchValues == NULL)
            mBatchValues = FdoBatchParameterValueCollection::Create();
        return mBatchValues;
#endif
        return NULL;
      };


      virtual FdoIdentifier* GetClassNameRef()
      {
          return m_ClassName;
      }

      virtual FdoIdentifier* GetFeatureClassName()
      {
        if( m_ClassName )
            m_ClassName->AddRef();

        return m_ClassName;
      }

      // Sets the name of the class to be operated upon as an Identifier.
      virtual void SetFeatureClassName(FdoIdentifier* value)
      {
        if( NULL == mConnection )
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));


        if (value != NULL)
        {
            if( ! ut_utf8_from_unicode( value->GetText(), LocalConversionBuffer,  LOCAL_BUFFER_SIZE ) ||
              strlen( LocalConversionBuffer ) >= GDBI_SCHEMA_ELEMENT_NAME_SIZE )
                throw FdoCommandException::Create(NlsMsgGet( FDORDBMS_273, "Class name size exceeds the internal storage limit"));

            if (value)
            {
                const FdoSmLpClassDefinition *classDefinition = mConnection->GetSchemaUtil()->GetClass(value->GetText());

                if ( classDefinition == NULL )
                    throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_333, "Class '%1$ls' not found", value->GetText()));

                if (classDefinition->GetIsAbstract())
                    throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_266, "Creating/Updating a standalone instance for class '%1$ls' is not allowed", value->GetText()));
            }

            mConnection->GetSchemaUtil()->CheckClass(value->GetText());
        }

        if( m_ClassName != NULL )
            m_ClassName->Release();

        m_ClassName = NULL;

        if( value != NULL )
        {
            value->AddRef();
            m_ClassName = value;
        }
      }

      // Sets the name of the class to be operated upon as an String
      virtual void SetFeatureClassName(const wchar_t* value)
      {
        if( NULL == mConnection )
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

        if( value != NULL )
        {

            if( ! ut_utf8_from_unicode( value, LocalConversionBuffer,  LOCAL_BUFFER_SIZE ) ||
                  strlen( LocalConversionBuffer ) >= GDBI_SCHEMA_ELEMENT_NAME_SIZE )
                throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_273, "Class name size exceeds the internal storage limit"));
            if (value)
            {
                const FdoSmLpClassDefinition *classDefinition = mConnection->GetSchemaUtil()->GetClass(value);
                if ( classDefinition == NULL )
                    throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_333, "Class '%1$ls' not found", value));

                if (classDefinition->GetIsAbstract())
                    throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_266, "Creating/Updating a standalone instance for class '%1$ls' is not allowed", value));
            }

            mConnection->GetSchemaUtil()->CheckClass(value);

            if( m_ClassName != NULL )
                m_ClassName->Release();

            m_ClassName = NULL;

            m_ClassName = FdoIdentifier::Create( value );
        }
      }

  protected:
      DbiConnection* mConnection;

  private:
    FdoPropertyValueCollection *mPropertyValues;
    FdoPropertyValueCollection *mAutoGenPropertyValues;
    wchar_t         *mCurrentClass;
    FdoBatchParameterValueCollection *mBatchValues;
    FdoIdentifier*  m_ClassName;
	FdoRdbmsPvcProcessor  *mPvcProcessor;
    char  LocalConversionBuffer[LOCAL_BUFFER_SIZE];
    void SetAutoGeneratedValues();
	void InitObjectPropertyAutoGenProp(const FdoSmLpClassDefinition *classDefinition, 
							FdoPropertyValueCollection  *propValCollection, FdoPropertyValueCollection  *featInfoCol);
    FdoStringP GetSequenceName(const wchar_t *propNameStr, const wchar_t* scope, const FdoSmLpClassDefinition *classDefinition);
    bool IsFeatIdAutoincremented( const FdoSmLpClassDefinition *classDefinition );
    bool IsPropertyValueAutoincremented( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValue * propertyValue );
    void FetchAutoincrementedFeatIdValues( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection* featInfoCol );
    void FetchAutoincrementedIdValues( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection* featInfoCol );
    FdoPropertyValueCollection* GetAutoGeneratedPropertyValues();
    FdoPropertyValueCollection* GetAllPropertyValues();

};


#endif // FDORDBMSINSERTCOMMAND_H
