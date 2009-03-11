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
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Expression\Function.h>

#include "FDO\Expression\mgFunction.h"
#include "FDO\Expression\mgExpressionCollection.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::Function::Function(IntPtr unmanaged, Boolean autoDelete) : Expression(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::Function::Function() : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFunction::Create()), true))
}

FdoFunction* NAMESPACE_OSGEO_FDO_EXPRESSION::Function::GetImpObj()
{
	return static_cast<FdoFunction*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Function::Function(System::String^ name, array<NAMESPACE_OSGEO_FDO_EXPRESSION::Expression^>^ arguments) : Expression(IntPtr::Zero, false)
{
	FdoInt32 unlength = arguments->Length;
    FdoExpression** unargs = new FdoExpression* [unlength];
    try
    {
	    for(FdoInt32 i = 0; i < unlength; i ++)
	    {
		    *(unargs + i) = arguments[i]->GetImpObj();
	    }
	    EXCEPTION_HANDLER(Attach(IntPtr(FdoFunction::Create(StringToUni(name), unargs, unlength)), true))
    }
    finally
    {
        if (unargs != nullptr)
            delete[] unargs;
    }
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Function::Function(System::String^ name, NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionCollection^ arguments) : Expression(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoFunction::Create(StringToUni(name), arguments->GetImpObj())), true))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::Function::Name::get()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetName())
    return CHECK_STRING(unstr);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Function::Name::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionCollection^ NAMESPACE_OSGEO_FDO_EXPRESSION::Function::Arguments::get()
{
	FdoExpressionCollection* result;
	EXCEPTION_HANDLER(result = GetImpObj()->GetArguments())
	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateExpressionCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Function::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::Function::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
