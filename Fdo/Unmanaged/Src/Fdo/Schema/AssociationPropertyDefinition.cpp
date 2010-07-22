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
#include <Fdo/Schema/AssociationPropertyDefinition.h>
#include "StringUtility.h"
#include "XmlContext.h"

FdoAssociationPropertyDefinition* FdoAssociationPropertyDefinition::Create()
{
    return new FdoAssociationPropertyDefinition();
}

FdoAssociationPropertyDefinition* FdoAssociationPropertyDefinition::Create(FdoString* name, FdoString* description, bool system)
{
    return new FdoAssociationPropertyDefinition(name, description, system);
}

FdoAssociationPropertyDefinition::~FdoAssociationPropertyDefinition()
{
    if (m_reverseNameCHANGED && m_reverseName != m_reverseNameCHANGED)
        FdoStringUtility::ClearString(m_reverseNameCHANGED);
    FdoStringUtility::ClearString(m_reverseName);

    if (m_multiplicityCHANGED && m_multiplicity != m_multiplicityCHANGED)
        FdoStringUtility::ClearString(m_multiplicityCHANGED);
    FdoStringUtility::ClearString(m_multiplicity);

    if (m_reverseMultiplicityCHANGED && m_reverseMultiplicity != m_reverseMultiplicityCHANGED)
        FdoStringUtility::ClearString(m_reverseMultiplicityCHANGED);
    FdoStringUtility::ClearString(m_reverseMultiplicity);

    FDO_SAFE_RELEASE(m_identityProperties);
    FDO_SAFE_RELEASE(m_identityReverseProperties);
    if ( ! m_isReadOnly )
    {
        FDO_SAFE_RELEASE(m_associatedClass);
        FDO_SAFE_RELEASE(m_associatedClassCHANGED);
    }
    FDO_SAFE_RELEASE(m_dataPropertyHandler)
}

void FdoAssociationPropertyDefinition::Dispose()
{
    if ( ! m_isReadOnly &&  m_reverseName != NULL && m_associatedClass != NULL  )
    {
        FdoPtr<FdoPropertyDefinitionCollection> props = m_associatedClass->GetProperties();
        FdoPtr<FdoAssociationPropertyDefinition> prop = (FdoAssociationPropertyDefinition*)props->FindItem( m_reverseName );
        if( prop != NULL )
            prop->SetAssociatedClass( NULL );
    }
    delete this;
}

FdoAssociationPropertyDefinition::FdoAssociationPropertyDefinition() : FdoPropertyDefinition()
{
    m_associatedClass = NULL;
    m_deleteRule = (FdoDeleteRule)0;
    m_lockCascade = false;
    m_identityProperties = FdoDataPropertyDefinitionCollection::Create(NULL);
    m_identityReverseProperties = FdoDataPropertyDefinitionCollection::Create(NULL);
    m_reverseName = NULL;
    m_isReadOnly = false;
    m_multiplicity = FdoStringUtility::MakeString(L"m");
    m_reverseMultiplicity = FdoStringUtility::MakeString(L"0_1");
    m_dataPropertyHandler = NULL;


    m_associatedClassCHANGED = NULL;
    m_deleteRuleCHANGED = (FdoDeleteRule)0;
    m_lockCascadeCHANGED = false;
    m_reverseNameCHANGED = NULL;
    m_isReadOnlyCHANGED = false;
    m_multiplicityCHANGED = FdoStringUtility::MakeString(L"m");
    m_reverseMultiplicityCHANGED = FdoStringUtility::MakeString(L"0_1");
}

FdoAssociationPropertyDefinition::FdoAssociationPropertyDefinition(FdoString* name, FdoString* description, bool system) :
    FdoPropertyDefinition(name, description, system)
{
    m_associatedClass = NULL;
    m_deleteRule = (FdoDeleteRule)0;
    m_lockCascade = false;
    m_identityProperties = FdoDataPropertyDefinitionCollection::Create(NULL);
    m_identityReverseProperties = FdoDataPropertyDefinitionCollection::Create(NULL);
    m_reverseName = NULL;
    m_isReadOnly = false;
    m_multiplicity = FdoStringUtility::MakeString(L"m");
    m_reverseMultiplicity = FdoStringUtility::MakeString(L"0_1");
    m_dataPropertyHandler = NULL;

    m_associatedClassCHANGED = NULL;
    m_deleteRuleCHANGED = (FdoDeleteRule)0;
    m_lockCascadeCHANGED = false;
    m_reverseNameCHANGED = NULL;
    m_isReadOnlyCHANGED = false;
    m_multiplicityCHANGED = FdoStringUtility::MakeString(L"m");
    m_reverseMultiplicityCHANGED = FdoStringUtility::MakeString(L"0_1");
}

FdoPropertyType FdoAssociationPropertyDefinition::GetPropertyType()
{
	return FdoPropertyType_AssociationProperty;
}

FdoClassDefinition* FdoAssociationPropertyDefinition::GetAssociatedClass()
{
    return FDO_SAFE_ADDREF(m_associatedClass);
}

void FdoAssociationPropertyDefinition::SetAssociatedClass(FdoClassDefinition* value)
{
    _StartChanges();
    if( ! m_isReadOnly )
    {
        // For read only properties which are created internally, we do not ref count the
        // asociated class. this is needed to avoid circular references.
        // The creator will make sure to reset our associated class to NULL before 
        // it is released. This way we will not end up with a stale pointer.
        FDO_SAFE_RELEASE(m_associatedClass);
        FDO_SAFE_ADDREF(value);
    }
    m_associatedClass = value;

    SetElementState(FdoSchemaElementState_Modified);
}

FdoDataPropertyDefinitionCollection* FdoAssociationPropertyDefinition::GetIdentityProperties()
{
    return FDO_SAFE_ADDREF(m_identityProperties);
}

FdoDataPropertyDefinitionCollection* FdoAssociationPropertyDefinition::GetReverseIdentityProperties()
{
    return FDO_SAFE_ADDREF(m_identityReverseProperties);
}

FdoString* FdoAssociationPropertyDefinition::GetReverseName()
{
    return m_reverseName;
}

void FdoAssociationPropertyDefinition::SetReverseName(FdoString* name)
{
     _StartChanges();
    if (m_reverseName != NULL)
    {
        if (m_reverseNameCHANGED != m_reverseName)
            FdoStringUtility::ClearString(m_reverseName);

        m_reverseName = NULL;
    }

    if( name && wcslen(name) != 0 )
        m_reverseName = FdoStringUtility::MakeString(name);

    SetElementState(FdoSchemaElementState_Modified);
}

FdoDeleteRule FdoAssociationPropertyDefinition::GetDeleteRule()
{
    return m_deleteRule;
}

void FdoAssociationPropertyDefinition::SetDeleteRule(FdoDeleteRule value)
{
    _StartChanges();
    m_deleteRule = value;
    SetElementState(FdoSchemaElementState_Modified);
}

bool FdoAssociationPropertyDefinition::GetLockCascade()
{
    return m_lockCascade;
}

void FdoAssociationPropertyDefinition::SetLockCascade(bool value)
{
    _StartChanges();
    m_lockCascade = value;
    SetElementState(FdoSchemaElementState_Modified);
}

bool FdoAssociationPropertyDefinition::GetIsReadOnly()
{
    return m_isReadOnly;
}

void FdoAssociationPropertyDefinition::SetIsReadOnly(bool value)
{
    _StartChanges();
    m_isReadOnly = value;
    SetElementState(FdoSchemaElementState_Modified);
}

FdoString* FdoAssociationPropertyDefinition::GetMultiplicity()
{
    return m_multiplicity;
}

void FdoAssociationPropertyDefinition::SetMultiplicity(FdoString* value)
{
    _StartChanges();
    if (m_multiplicity != NULL)
    {
        if (m_multiplicityCHANGED != m_multiplicity)
            FdoStringUtility::ClearString(m_multiplicity);
    }

    m_multiplicity = FdoStringUtility::MakeString(value);

    SetElementState(FdoSchemaElementState_Modified);
}

FdoString* FdoAssociationPropertyDefinition::GetReverseMultiplicity()
{
    return m_reverseMultiplicity;
}

void FdoAssociationPropertyDefinition::SetReverseMultiplicity(FdoString* value)
{
    _StartChanges();
    if (m_reverseMultiplicity != NULL)
    {
        if (m_reverseMultiplicityCHANGED != m_reverseMultiplicity)
            FdoStringUtility::ClearString(m_reverseMultiplicity);
    }

    m_reverseMultiplicity = FdoStringUtility::MakeString(value);
    
    SetElementState(FdoSchemaElementState_Modified);
}

void FdoAssociationPropertyDefinition::_StartChanges()
{
    if (!(m_changeInfoState & (CHANGEINFO_PRESENT|CHANGEINFO_PROCESSING)))
    {
        FdoPropertyDefinition::_StartChanges();

        m_associatedClassCHANGED = m_associatedClass;
        if( ! m_isReadOnly )
            FDO_SAFE_ADDREF(m_associatedClass);
        m_deleteRuleCHANGED = m_deleteRule;
        m_lockCascadeCHANGED = m_lockCascade;
        m_reverseNameCHANGED = m_reverseName;
        m_isReadOnlyCHANGED = m_isReadOnly;
        m_identityProperties->_StartChanges();
        m_identityReverseProperties->_StartChanges();
    }
}

void FdoAssociationPropertyDefinition::_RejectChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed
    FdoPropertyDefinition::_RejectChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {
        // restore
        if (m_associatedClass != m_associatedClassCHANGED)
        {
             if( ! m_isReadOnly )
            {
                FDO_SAFE_RELEASE(m_associatedClass);
                m_associatedClass = FDO_SAFE_ADDREF(m_associatedClassCHANGED);
            }
            else
                m_associatedClass = m_associatedClassCHANGED;
        }
        m_deleteRule = m_deleteRuleCHANGED;
        m_lockCascade = m_lockCascadeCHANGED;
        m_isReadOnly = m_isReadOnlyCHANGED;
        
        if (m_reverseName != m_reverseNameCHANGED)
        {
            if (m_reverseName)
                FdoStringUtility::ClearString(m_reverseName);
            m_reverseName = m_reverseNameCHANGED;
        }
        if (m_multiplicity != m_multiplicityCHANGED)
        {
            if (m_multiplicity)
                FdoStringUtility::ClearString(m_multiplicity);
            m_multiplicity = m_multiplicityCHANGED;
        }
        if (m_reverseMultiplicity != m_reverseMultiplicityCHANGED)
        {
            if (m_reverseMultiplicity)
                FdoStringUtility::ClearString(m_reverseMultiplicity);
            m_reverseMultiplicity = m_reverseMultiplicityCHANGED;
        }
        
        // reset
        if( ! m_isReadOnly )
        {
            FDO_SAFE_RELEASE(m_associatedClassCHANGED);
        }
        else
            m_associatedClassCHANGED = NULL;
        m_deleteRuleCHANGED = (FdoDeleteRule)0;
        m_lockCascadeCHANGED = false;
        m_isReadOnlyCHANGED = false;
        m_multiplicityCHANGED = FdoStringUtility::MakeString(L"m");
        m_reverseMultiplicityCHANGED = FdoStringUtility::MakeString(L"0");

        m_reverseNameCHANGED = NULL;
    }
    if( m_identityProperties )
        m_identityProperties->_RejectChanges();
    if( m_identityReverseProperties )
        m_identityReverseProperties->_RejectChanges();
}

void FdoAssociationPropertyDefinition::_AcceptChanges()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSED)
        return; // already processed

    // Perform some data integrity checks. 

    Validate();

    FdoPropertyDefinition::_AcceptChanges();

    if (m_changeInfoState & CHANGEINFO_PRESENT)
    {  

        ModifyAssociatedClass();

        // reset
        if( ! m_isReadOnly )
        {
            FDO_SAFE_RELEASE(m_associatedClassCHANGED);
        }
        else
            m_associatedClassCHANGED = NULL;

        m_deleteRuleCHANGED = (FdoDeleteRule)0;
        m_lockCascadeCHANGED = false;
        m_isReadOnlyCHANGED = false;
        if (m_multiplicity != m_multiplicityCHANGED)
        {
            if (m_multiplicityCHANGED)
                FdoStringUtility::ClearString(m_multiplicityCHANGED);
        }
        m_multiplicityCHANGED = FdoStringUtility::MakeString(L"m");
        if (m_reverseMultiplicity != m_reverseMultiplicityCHANGED)
        {
            if (m_reverseMultiplicityCHANGED)
                FdoStringUtility::ClearString(m_reverseMultiplicityCHANGED);
        }
        m_reverseMultiplicityCHANGED = FdoStringUtility::MakeString(L"0");


        if (m_reverseName != m_reverseNameCHANGED)
        {
            
            if (m_reverseNameCHANGED)
                FdoStringUtility::ClearString(m_reverseNameCHANGED);
        }

      
        m_reverseNameCHANGED = NULL;
    }

    if (m_identityProperties)
        m_identityProperties->_AcceptChanges();
    if (m_identityReverseProperties)
        m_identityReverseProperties->_AcceptChanges();
}

void FdoAssociationPropertyDefinition::SetParent(FdoSchemaElement* value)
{
    FdoPropertyDefinition::SetParent( value );
    ModifyAssociatedClass();
}

void FdoAssociationPropertyDefinition::ModifyAssociatedClass()
{
    // If the reverse name changed or the associated class changed, then we need to add the read-only
    // association property on the associated class.
    if ( ! m_isReadOnly )
    {
        // First if one already added, we remove it. We do that using the old values.
        if( m_associatedClassCHANGED != NULL && m_reverseNameCHANGED != NULL )
        {
            FdoPtr<FdoPropertyDefinitionCollection> props = m_associatedClassCHANGED->GetProperties();
            
            FdoPtr<FdoPropertyDefinition> prop = props->FindItem( m_reverseNameCHANGED );
            if( prop != NULL )
                props->Remove( prop );
        }

        // Add the new read-only association property.
        FdoPtr<FdoSchemaElement> parent = GetParent();
        if( m_reverseName != NULL && m_associatedClass != NULL && parent != NULL )
        {
            FdoPtr<FdoAssociationPropertyDefinition> readOnlyAssoc = FdoAssociationPropertyDefinition::Create(m_reverseName, L"Reverse Property");
            readOnlyAssoc->SetIsReadOnly( true );
            readOnlyAssoc->SetAssociatedClass( (FdoClassDefinition*)parent.p );
            readOnlyAssoc->SetLockCascade( false );
            readOnlyAssoc->SetDeleteRule( FdoDeleteRule_Break );
            readOnlyAssoc->SetReverseName( GetName() );
            readOnlyAssoc->SetMultiplicity( m_reverseMultiplicity );
            readOnlyAssoc->SetReverseMultiplicity( m_multiplicity );
            
            for( int i=0; i<m_identityProperties->GetCount(); i++ )
                FdoPtr<FdoDataPropertyDefinitionCollection>(readOnlyAssoc->GetReverseIdentityProperties())->Add( 
                        FdoPtr<FdoDataPropertyDefinition>( m_identityProperties->GetItem( i ) )
                        );

            for( int i=0; i<m_identityReverseProperties->GetCount(); i++ )
                FdoPtr<FdoDataPropertyDefinitionCollection>(readOnlyAssoc->GetIdentityProperties())->Add( 
                        FdoPtr<FdoDataPropertyDefinition>( m_identityReverseProperties->GetItem( i ) )
                        );
                
			FdoPtr<FdoPropertyDefinitionCollection> associatedClassProps = m_associatedClass->GetProperties();
			FdoPtr<FdoPropertyDefinition>prop = associatedClassProps->FindItem(m_reverseName);
			if( prop != NULL )  // remove the existing reverse property if one already exists
				associatedClassProps->Remove( prop );

            associatedClassProps->Add( readOnlyAssoc );
        }
    }
}

void FdoAssociationPropertyDefinition::_BeginChangeProcessing()
{
    if (m_changeInfoState & CHANGEINFO_PROCESSING)
        return; // already marked for processing
    FdoPropertyDefinition::_BeginChangeProcessing();

    if (m_identityProperties)
        m_identityProperties->_BeginChangeProcessing();
    if (m_identityReverseProperties)
        m_identityReverseProperties->_BeginChangeProcessing();
}

void FdoAssociationPropertyDefinition::_EndChangeProcessing()
{
    if (!(m_changeInfoState & CHANGEINFO_PROCESSING))
        return; // processing flag already cleared

    FdoPropertyDefinition::_EndChangeProcessing();
    if (m_identityProperties)
        m_identityProperties->_EndChangeProcessing();
    if (m_identityReverseProperties)
        m_identityReverseProperties->_EndChangeProcessing();
}


void FdoAssociationPropertyDefinition::Set( FdoPropertyDefinition* pProperty, FdoSchemaMergeContext* pContext )
{
    FdoPropertyDefinition::Set(pProperty, pContext);

    // Base function catches property type mismatch so silently quit on type mismatch
    if ( GetPropertyType() == pProperty->GetPropertyType() ) {
        if ( pContext->GetIgnoreStates() || (GetElementState() == FdoSchemaElementState_Added) || (pProperty->GetElementState() == FdoSchemaElementState_Modified) ) {
            FdoAssociationPropertyDefinition* pAssociationProperty = (FdoAssociationPropertyDefinition*) pProperty;

            // Set each member from the given association property. The same pattern is followed 
            // for each:
            //
            // If new and old values differ
            //    If modification allowed (always allowed if this is a new property).
            //      If value is an object
            //        Add a reference to be resolved later (when we're sure that referenced
            //          object exists).
            //      else
            //        Perform the modification
            //      end if
            //    else
            //      log an error
            //    end if
            //  end if

            // Set associated class.

            FdoClassDefinitionP newAssocClass = pAssociationProperty->GetAssociatedClass();
            FdoSchemaElementP newAssocParent = newAssocClass ? newAssocClass->GetParent() : (FdoSchemaElement*) NULL;
            if ( newAssocClass && !newAssocParent ) {
                // New associated class is an orphan,
                pContext->AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_51_CLASSNOSCHEMA),
                                (FdoString*) GetQualifiedName(),
                                newAssocClass->GetName()
                            )
                        )
                    )
                );
            }
            else {
                FdoStringP oldAssocClassName = m_associatedClass ? m_associatedClass->GetQualifiedName() : FdoStringP();
                FdoStringP newAssocClassName = newAssocClass ? newAssocClass->GetQualifiedName() : FdoStringP();
                
                if ( oldAssocClassName != newAssocClassName ) { 
                    if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocClass(pAssociationProperty)) ) {
                        pContext->AddAssocPropRef( 
                            this, 
                            newAssocParent ? newAssocParent->GetName() : L"",
                            newAssocClass ? newAssocClass->GetName() : L""
                        );
                    }
                    else {
                        pContext->AddError( 
                            FdoSchemaExceptionP(
                                FdoSchemaException::Create(
                                    FdoException::NLSGetMessage(
                                        FDO_NLSID(SCHEMA_103_MODASSOCCLASS),
                                        (FdoString*) GetQualifiedName(),
                                        (FdoString*) oldAssocClassName,
                                        (FdoString*) newAssocClassName
                                    )
                                )
                            )
                        );
                    }
                }
            }

            // Set reverse name

            if ( FdoStringP(GetReverseName()) != FdoStringP(pAssociationProperty->GetReverseName()) ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocReverseName(pAssociationProperty)) ) 
                    SetReverseName( pAssociationProperty->GetReverseName() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_104_MODASSOCREVNAME),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) FdoStringP(GetReverseName()),
                                    (FdoString*) FdoStringP(pAssociationProperty->GetReverseName())
                                )
                            )
                        )
                    );
            }

            // Set delete rule

            if ( GetDeleteRule() != pAssociationProperty->GetDeleteRule() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocDeleteRule(pAssociationProperty)) ) 
                    SetDeleteRule( pAssociationProperty->GetDeleteRule() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_105_MODASSOCDELRULE),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Lock Cascade setting

            if ( GetLockCascade() != pAssociationProperty->GetLockCascade() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocLockCascade(pAssociationProperty)) ) 
                    SetLockCascade( pAssociationProperty->GetLockCascade() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_106_MODASSOCCASCLOCK),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Read Only setting

            if ( GetIsReadOnly() != pAssociationProperty->GetIsReadOnly() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocReadOnly(pAssociationProperty)) ) 
                    SetIsReadOnly( pAssociationProperty->GetIsReadOnly() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(pAssociationProperty->GetIsReadOnly() ? SCHEMA_95_MODPROPRDONLY : SCHEMA_96_MODPROPWRITABLE),
                                    (FdoString*) GetQualifiedName()
                                )
                            )
                        )
                    );
            }

            // Multiplicity

            if ( FdoStringP(GetMultiplicity()) != FdoStringP(pAssociationProperty->GetMultiplicity()) ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocMultiplicity(pAssociationProperty)) ) 
                    SetMultiplicity( pAssociationProperty->GetMultiplicity() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_107_MODASSOCMULT),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) FdoStringP(GetMultiplicity()),
                                    (FdoString*) FdoStringP(pAssociationProperty->GetMultiplicity())
                                )
                            )
                        )
                    );
            }

            // Reverse Multiplicity

            if ( FdoStringP(GetReverseMultiplicity()) != FdoStringP(pAssociationProperty->GetReverseMultiplicity()) ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocReverseMultiplicity(pAssociationProperty)) ) 
                    SetReverseMultiplicity( pAssociationProperty->GetReverseMultiplicity() );
                else
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_108_MODASSOCREVMULT),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) FdoStringP(GetReverseMultiplicity()),
                                    (FdoString*) FdoStringP(pAssociationProperty->GetReverseMultiplicity())
                                )
                            )
                        )
                    );
            }

            // Association Identity Properties

            FdoInt32 idx = -1;
            FdoDataPropertiesP idProps = pAssociationProperty->GetIdentityProperties();
            
            FdoStringsP oldIdPropNames = FdoStringCollection::Create();
            for ( idx = 0; idx < m_identityProperties->GetCount(); idx++ ) 
                oldIdPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(m_identityProperties->GetItem(idx))->GetName() );

            FdoStringsP newIdPropNames = FdoStringCollection::Create();
            for ( idx = 0; idx < idProps->GetCount(); idx++ ) 
                newIdPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(idProps->GetItem(idx))->GetName() );

            if ( oldIdPropNames->ToString() != newIdPropNames->ToString() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocIdentity(pAssociationProperty)) ) {
                    pContext->AddAssocIdPropRef( 
                        this, 
                        newIdPropNames
                    );
                }
                else {
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_109_MODASSOCID),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) oldIdPropNames->ToString(),
                                    (FdoString*) newIdPropNames->ToString()
                                )
                            )
                        )
                    );
                }
            }

            // Reverse Identity Properties

            FdoDataPropertiesP revIdProps = pAssociationProperty->GetReverseIdentityProperties();
            
            FdoStringsP oldRevIdPropNames = FdoStringCollection::Create();
            for ( idx = 0; idx < m_identityReverseProperties->GetCount(); idx++ ) 
                oldRevIdPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(m_identityReverseProperties->GetItem(idx))->GetName() );

            FdoStringsP newRevIdPropNames = FdoStringCollection::Create();
            for ( idx = 0; idx < idProps->GetCount(); idx++ ) 
                newRevIdPropNames->Add( FdoPtr<FdoDataPropertyDefinition>(revIdProps->GetItem(idx))->GetName() );

            if ( oldRevIdPropNames->ToString() != newRevIdPropNames->ToString() ) {
                if ( (GetElementState() == FdoSchemaElementState_Added) || (pContext->CanModAssocReverseIdentity(pAssociationProperty)) ) {
                    pContext->AddAssocIdReversePropRef( 
                        this, 
                        newRevIdPropNames
                    );
                }
                else {
                    pContext->AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create(
                                FdoException::NLSGetMessage(
                                    FDO_NLSID(SCHEMA_110_MODREVASSOCID),
                                    (FdoString*) GetQualifiedName(),
                                    (FdoString*) oldRevIdPropNames->ToString(),
                                    (FdoString*) newRevIdPropNames->ToString()
                                )
                            )
                        )
                    );
                }
            }
        }
    }
}

void FdoAssociationPropertyDefinition::CheckReferences( FdoSchemaMergeContext* pContext )
{
    // No need to check references if this element is going away.
    if ( GetElementState() != FdoSchemaElementState_Deleted ) {
        FdoSchemaElement::CheckReferences(pContext);

        // Check if associated class marked for delete

        FdoClassDefinitionP assocClass = GetAssociatedClass();

        if ( assocClass && (assocClass->GetElementState() == FdoSchemaElementState_Deleted) )
            pContext->AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_130_DELASSOCCLASS),
                            (FdoString*) assocClass->GetQualifiedName(),
                            (FdoString*) GetQualifiedName()
                        )
                    )
                )
            );

        // Check association identity properties
        FdoDataPropertiesP idProps = GetIdentityProperties();
        CheckIdentityReferences( pContext, idProps, SCHEMA_131_DELASSOCID );

        // Check reverse identity properties
        idProps = GetReverseIdentityProperties();
        CheckIdentityReferences( pContext, idProps, SCHEMA_132_DELASSOCREVID );
    
        // Check data integrity - e.g.: identity properties match reverse identity properties.
        
        Validate( pContext);
    }
}

void FdoAssociationPropertyDefinition::CheckIdentityReferences( FdoSchemaMergeContext* pContext, FdoDataPropertyDefinitionCollection* idProps, long messageId )
{
    int idx;

    for ( idx = 0; idx < idProps->GetCount(); idx++ ) {
        FdoDataPropertyP idProp = idProps->GetItem( idx );

        if ( idProp->GetElementState() == FdoSchemaElementState_Deleted ) {
            pContext->AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(messageId),
                            (FdoString*) idProp->GetQualifiedName(),
                            (FdoString*)GetQualifiedName()
                        )
                    )
                )
            );
        }
    }
}

void FdoAssociationPropertyDefinition::InitFromXml(FdoString* propertyTypeName, FdoSchemaXmlContext* pContext, FdoXmlAttributeCollection* attrs)
{
    // Error if FDO and XML property types are not the same.
    if ( wcscmp( propertyTypeName, L"AssociationProperty" ) != 0 ) {
        pContext->AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_25_PROPERTYTYPECONFLICT),
                        (FdoString*) GetQualifiedName()
                    )
                )
            )
        );

        return;
    }

    // Initialize generic parts
    FdoPropertyDefinition::InitFromXml(pContext, attrs );

    // Reset data property attributes to defaults.
    FDO_SAFE_RELEASE(m_associatedClass);
    m_deleteRule = (FdoDeleteRule)0;
    m_lockCascade = false;
    m_identityProperties->Clear();
    m_identityReverseProperties->Clear();
    m_reverseName = NULL;
    m_isReadOnly = false;
    FdoStringUtility::ClearString(m_multiplicity);
    m_multiplicity = FdoStringUtility::MakeString(L"m");
    FdoStringUtility::ClearString(m_reverseMultiplicity);
    m_reverseMultiplicity = FdoStringUtility::MakeString(L"0");
    FDO_SAFE_RELEASE(m_dataPropertyHandler);

    FdoXmlAttributeP attr = attrs->FindItem( L"deleteRule" );

    if ( attr != NULL ) 
        SetDeleteRule(StringToDeleteRule(FdoStringP(attr->GetValue())));

    attr = attrs->FindItem( L"lockCascade" );
    if ( attr != NULL )
        SetLockCascade( FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem( L"reverseName" );
    if ( attr != NULL )
        SetReverseName( FdoStringP(attr->GetValue()));

    attr = attrs->FindItem( L"isReadOnly" );
    if ( attr != NULL )
        SetIsReadOnly( FdoStringP(attr->GetValue()).ToBoolean());

    attr = attrs->FindItem( L"multiplicity" );
    if ( attr != NULL )
        SetMultiplicity( attr->GetValue() );

    FdoXmlAttributeP apSchema = attrs->FindItem( L"associatedClassSchema" );
    FdoXmlAttributeP apClass = attrs->FindItem( L"associatedClass" );

    if ( (apSchema != NULL) && (apClass != NULL) )
        // Associated property class might not have been read yet.
        // Just add a reference to be resolved later
        pContext->GetMergeContext()->AddAssocPropRef( this, pContext->DecodeName(apSchema->GetValue()), pContext->DecodeName(apClass->GetValue()) );
}

FdoXmlSaxHandler* FdoAssociationPropertyDefinition::XmlStartElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname, 
        FdoXmlAttributeCollection* atts
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // Handle generic sub-elements
    FdoXmlSaxHandler* pRet = FdoPropertyDefinition::XmlStartElement(context, uri, name, qname, atts);
    FdoPtr<FdoPropertyDefinition> pProp;

    if ( !pRet ) {
        if ( wcscmp(name, L"IdentityProperties") == 0 ) {
            //Sub-element is a list of identity property names.
            //Initialize the id prop name collection.
            m_Default = false;
            FdoXmlAttributeP attr = atts->FindItem( L"default" );
            if (attr)
                m_Default = true;
            m_idPropNames = FdoStringCollection::Create();
        }

        if ( wcscmp(name, L"IdentityProperty") == 0 ) {
            // Sub-element contains is an identity property name.
            // Set up a SAX handler to read the content.
            m_XmlContentHandler = FdoXmlCharDataHandler::Create();
            pRet = m_XmlContentHandler;
        }
        if ( wcscmp(name, L"IdentityReverseProperties") == 0 ) {
            //Sub-element is a list of identity reverse property names.
            //Initialize the id prop name collection.
            m_idRvrsPropNames = FdoStringCollection::Create();
        }

        if ( wcscmp(name, L"DataProperty") == 0 ) {
            // Sub-element contains is an identity reverse property name.
            // Set up a SAX handler to read the content.
            FDO_SAFE_RELEASE(m_dataPropertyHandler);
            m_dataPropertyHandler = FdoDataPropertyDefinition::Create();
            m_dataPropertyHandler->InitFromXml( name, fdoContext, atts);
            pRet = m_dataPropertyHandler;
        }
    }

    return(pRet);
}

FdoBoolean FdoAssociationPropertyDefinition::XmlEndElement(
        FdoXmlSaxContext* context, 
        FdoString* uri, 
        FdoString* name, 
        FdoString* qname
)
{
    FdoSchemaXmlContext* fdoContext = (FdoSchemaXmlContext*) context;

    // End any generic sub-elements.
    FdoPropertyDefinition::XmlEndElement(context, uri, name, qname);

    if ( wcscmp(name, L"IdentityProperties") == 0 ) {
        //End the id property list. Add a reference that is 
        //resolved when the read from XML is complete.
        fdoContext->GetMergeContext()->AddAssocIdPropRef( this, m_idPropNames );
    }

    if ( wcscmp(name, L"IdentityProperty") == 0 ) {
        //Sub-element is an id property name. Get the name from the 
        //content handler and add it to the id property list.
        if (m_Default == false)
            m_idPropNames->Add( fdoContext->DecodeName(m_XmlContentHandler->GetString()) );
    }

    if ( wcscmp(name, L"IdentityReverseProperties") == 0 ) {
        //End the id property list. Add a reference that is 
        //resolved when the read from XML is complete.
        fdoContext->GetMergeContext()->AddAssocIdReversePropRef( this, m_idRvrsPropNames );
    }

    if ( wcscmp(name, L"DataProperty") == 0 ) {
        //Sub-element is an id property name. Get the name from the 
        //content handler and add it to the id property list.
        if (m_Default == false)
            m_idRvrsPropNames->Add( fdoContext->DecodeName(m_dataPropertyHandler->GetName() ));
    }

    return(false);
}


void FdoAssociationPropertyDefinition::_writeXml( FdoSchemaXmlContext* pContext )
{
    int i;
    FdoDataPropertiesP  idProps;
    FdoXmlWriterP       writer = pContext->GetXmlWriter();

    if ( !GetIsReadOnly() ) {
        // Write an association property element
        writer->WriteStartElement( L"AssociationProperty" );

        // Write the association property attributes
        writer->WriteAttribute( L"deleteRule", DeleteRuleToString(m_deleteRule));

        if ( m_lockCascade ) 
            writer->WriteAttribute( L"lockCascade", L"true" );

        writer->WriteAttribute( L"multiplicity", m_multiplicity );

        if ( m_associatedClass ) {
            writer->WriteAttribute( 
                L"associatedClassSchema", 
                pContext->EncodeName( pContext->RefClass2SchemaName(this, m_associatedClass) )
            );
            writer->WriteAttribute( 
                L"associatedClass",
                pContext->EncodeName(m_associatedClass->GetName())
            );
        }

        if ( m_reverseName ) 
            writer->WriteAttribute( L"reverseName", m_reverseName );

        if ( m_isReadOnly ) 
            writer->WriteAttribute( L"isReadOnly", L"true" );

        // Write the generic attributes and sub-elements
        FdoPropertyDefinition::_writeXml(pContext);

        if (m_associatedClass)
        {
            idProps = NULL;

            bool bWriteDefault = false; // write default bool attribute if identity/reverse identity collection is empty
           
            if (m_identityProperties->GetCount() > 0) {
                idProps = FDO_SAFE_ADDREF(m_identityProperties);
            } else {
                FdoClassDefinitionP cls = (FdoClassDefinition *) this->GetAssociatedClass();
                FdoClassDefinitionP base = (FdoClassDefinition *) cls->GetBaseClass();
            
                while( base ) {
                    cls = base;
                    base = (FdoClassDefinition *) cls->GetBaseClass();
                }

                FdoDataPropertiesP identityProps = cls->GetIdentityProperties();
                if (identityProps->GetCount() > 0) {
                    idProps = identityProps;
                    bWriteDefault = true;
                }
            }
            
            if ( idProps && idProps->GetCount() > 0 ) {
                writer->WriteStartElement( L"IdentityProperties" );
                if (bWriteDefault)
                    writer->WriteAttribute( L"default", L"true" );
                for ( i = 0; i < idProps->GetCount(); i++ ) {
                    writer->WriteStartElement( L"IdentityProperty" );

                    FdoString* pPropName = FdoDataPropertyP(idProps->GetItem(i))->GetName();

                    writer->WriteCharacters( pContext->EncodeName(pPropName) );

                    writer->WriteEndElement();
                }
                writer->WriteEndElement();
            }

            // Write the reverse properties

            // use bWriteDefault from above
            // count of m_identityReverseProperties and m_identityProperties are equal

            if (m_identityReverseProperties->GetCount() > 0)
                idProps = FDO_SAFE_ADDREF(m_identityReverseProperties);

            if ( idProps && idProps->GetCount() > 0 ) {
                writer->WriteStartElement( L"IdentityReverseProperties" );    
                if (bWriteDefault)
                    writer->WriteAttribute( L"default", L"true" );
                for ( i = 0; i < idProps->GetCount(); i++ )
                    FdoDataPropertyP(idProps->GetItem(i))->_writeXml(pContext);
                writer->WriteEndElement();
            }
        }
        // close the associate data property element.
        writer->WriteEndElement();
    }
}

FdoString* FdoAssociationPropertyDefinition::DeleteRuleToString(FdoDeleteRule rule)
{
    FdoString* ruleString;

    switch (rule)
    {
        case FdoDeleteRule_Cascade:
            ruleString = L"Cascade";
            break;

        case FdoDeleteRule_Prevent:
            ruleString = L"Prevent";
            break;

        case FdoDeleteRule_Break:
        default:
            ruleString = L"Break";
            break;
    }

    return ruleString;
}


FdoDeleteRule FdoAssociationPropertyDefinition::StringToDeleteRule(FdoString *ruleString)
{
    FdoDeleteRule rule;

    if (wcscmp(ruleString, L"Cascade") == 0)
        rule = FdoDeleteRule_Cascade;
    else if (wcscmp(ruleString, L"Prevent") == 0)
        rule = FdoDeleteRule_Prevent;
    else 
        rule = FdoDeleteRule_Break;

    return rule;
}

void FdoAssociationPropertyDefinition::Validate( FdoSchemaMergeContext* pContext )
{
    // Cannot create an association property without the associated class
    if( m_associatedClass == NULL )
    {
        ValidateError(pContext, FdoPtr<FdoSchemaException>(FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_55_ASSOCIATEDCLASSREQUIRED), (FdoString*) GetQualifiedName() ))));
    }

    if( m_identityProperties && m_identityReverseProperties )
    {
        if( m_identityProperties->GetCount() != m_identityReverseProperties->GetCount() )
            ValidateError(pContext, FdoPtr<FdoSchemaException>(FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_17_ASSOCIATIONMISSMATCHERROR), (FdoString*) GetQualifiedName() ))));
        
        // Make sure that the identity properties and reverse properties are of the same types
        for( int i=0; i<m_identityProperties->GetCount(); i++ )
        {
            FdoPtr<FdoDataPropertyDefinition> indprop = m_identityProperties->GetItem( i );
            FdoPtr<FdoDataPropertyDefinition> revprop = m_identityReverseProperties->GetItem( i );
            if( indprop->GetDataType() != revprop->GetDataType() )
            {
                ValidateError(pContext, FdoPtr<FdoSchemaException>(FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_17_ASSOCIATIONMISSMATCHERROR), (FdoString*) GetQualifiedName() ))));
            }
        }

        // For non-readonly property, make sure that the reverse properties are not system or auto-generated properties(cannot be updated)
        // Each readonly association property is the reverse of a non-readonly association property so
        // readonly association properties will commonly have autogenerated reverse identity properties.
        // This ok; since these are readonly properties, we can't directly set values for them.
        if( m_identityReverseProperties && !m_isReadOnly )
        {
            for( int i=0; i<m_identityReverseProperties->GetCount(); i++ )
            {
                FdoPtr<FdoDataPropertyDefinition> revprop = m_identityReverseProperties->GetItem( i );
                if( revprop->GetIsAutoGenerated() || revprop->GetIsSystem() )
                {
                    ValidateError(pContext, FdoPtr<FdoSchemaException>(FdoSchemaException::Create(FdoException::NLSGetMessage(FDO_NLSID(SCHEMA_53_BADASSOCREVERSEPROP), (FdoString*) GetQualifiedName() ))));
                }
            }
        }        
    }        
}

void FdoAssociationPropertyDefinition::ValidateError( FdoSchemaMergeContext* pContext, FdoSchemaException* pException )
{
    if ( pContext ) 
        pContext->AddError( pException );
    else
        throw FDO_SAFE_ADDREF( pException );
}
