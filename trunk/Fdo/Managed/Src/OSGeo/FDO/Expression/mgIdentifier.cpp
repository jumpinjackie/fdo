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
#include <Fdo\Expression\Identifier.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Expression\mgIdentifier.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::Identifier(IntPtr unmanaged, Boolean autoDelete) : Expression(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::Identifier() : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoIdentifier::Create()), true)) 
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::Identifier(System::String^ text) : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoIdentifier::Create(StringToUni(text))), true)) 
}

FdoIdentifier* NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::GetImpObj()
{
	return static_cast<FdoIdentifier*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::Text::get()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetText())
	return CHECK_STRING(unstr);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::Text::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetText(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::Name::get()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetName())
	return CHECK_STRING(unstr);
}

array<System::String^>^ NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::Scope::get()
{
	FdoString** unobj;
	FdoInt32 unlength;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetScope(unlength))
	array<System::String^>^ mystrs = gcnew array<System::String^>(unlength);
	for(FdoInt32 i = 0; i < unlength; i ++)
	{
		mystrs[i] = CHECK_STRING(*(unobj + i));
	}
	return mystrs;
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::SchemaName::get()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetSchemaName())
	return CHECK_STRING(unstr);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
