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
#ifndef _SdfExtendedSelect_H_
#define _SdfExtendedSelect_H_

#include "SdfImpExtendedSelect.h"
#include "SDF/IExtendedSelect.h"

class SdfExtendedSelect : public SdfIExtendedSelect
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

    public:
        // constructs a ExtendedSelect command using the specified extended select implementation
		// The ownership of the selct is passed to this class.
		SDF_API SdfExtendedSelect(SdfImpExtendedSelect* select) { m_Select = select; }

    protected:
       
        // default destructor
		virtual ~SdfExtendedSelect() { m_Select->Release(); }

		virtual void Dispose() { delete this; }

    //-------------------------------------------------------
    // FdoISelect implementation
    //-------------------------------------------------------

    public:

		// IExtendedSelect
		virtual SdfIScrollableFeatureReader* ExecuteScrollable( ) { return m_Select->ExecuteScrollable( ); }

		virtual SdfIScrollableFeatureReader* ExecuteScrollable( const wchar_t* sdfCacheFile, FdoDataPropertyDefinitionCollection* extendedProps, FdoPropertyValueCollection*  extendedDefaults ) 
						{ return m_Select->ExecuteScrollable( sdfCacheFile, extendedProps, extendedDefaults ); }
 
        virtual void SetOrderingOption( FdoString* propertyName, FdoOrderingOption  option ) { return m_Select->SetOrderingOption(propertyName, option ); }

        virtual FdoOrderingOption GetOrderingOption( FdoString* propertyName ) { return m_Select->GetOrderingOption( propertyName ); }

		virtual void ClearOrderingOptions( ) { return m_Select->ClearOrderingOptions(); }

        virtual void SetCompareHandler( SdfCompareHandler*  handler ) { return m_Select->SetCompareHandler( handler ); }

		// FdoISelect
		virtual FdoLockType GetLockType() { return m_Select->GetLockType( ); }

		virtual void SetLockType(FdoLockType value) { return m_Select->SetLockType( value ); }

		virtual FdoLockStrategy GetLockStrategy() { return m_Select->GetLockStrategy( ); }

		virtual void SetLockStrategy(FdoLockStrategy value) { return m_Select->SetLockStrategy( value ); }

		virtual FdoIFeatureReader* Execute() { return m_Select->Execute( ); }

		virtual FdoIFeatureReader* ExecuteWithLock() { return m_Select->ExecuteWithLock( ); }

		virtual FdoILockConflictReader* GetLockConflicts() { return m_Select->GetLockConflicts( ); }

		// FdoIBaseSelect
		virtual FdoIdentifierCollection* GetPropertyNames() { return m_Select->GetPropertyNames( ); }

		virtual FdoIdentifierCollection* GetOrdering() { return m_Select->GetOrdering( ); }

		virtual void SetOrderingOption( FdoOrderingOption  option ) { return m_Select->SetOrderingOption( option ); }

		virtual FdoOrderingOption GetOrderingOption( ) { return m_Select->GetOrderingOption( ); }

		// FdoIFeatureCommand
		virtual FdoIdentifier* GetFeatureClassName() { return m_Select->GetFeatureClassName( ); }

		virtual void SetFeatureClassName(FdoIdentifier* value) { return m_Select->SetFeatureClassName( value ); }

		virtual void SetFeatureClassName(FdoString* value) { return m_Select->SetFeatureClassName( value ); }

		virtual FdoFilter* GetFilter() { return m_Select->GetFilter( ); }

		virtual void SetFilter(FdoFilter* value) { return m_Select->SetFilter( value ); }

		virtual void SetFilter(FdoString* value) { return m_Select->SetFilter( value ); }

		//FdoICommand
		virtual FdoIConnection* GetConnection() { return m_Select->GetConnection( ); }

		virtual FdoITransaction* GetTransaction() { return m_Select->GetTransaction( ); }

		virtual void SetTransaction(FdoITransaction* value) { return m_Select->SetTransaction( value ); }

		virtual FdoInt32 GetCommandTimeout() { return m_Select->GetCommandTimeout( ); }

		virtual void SetCommandTimeout(FdoInt32 value) { return m_Select->SetCommandTimeout( value ); }

		virtual FdoParameterValueCollection* GetParameterValues() { return m_Select->GetParameterValues( ); }

		virtual void Prepare() { return m_Select->Prepare( ); }

		virtual void Cancel() { return m_Select->Cancel( ); }

	private:
		SdfImpExtendedSelect* m_Select;

};

#endif
