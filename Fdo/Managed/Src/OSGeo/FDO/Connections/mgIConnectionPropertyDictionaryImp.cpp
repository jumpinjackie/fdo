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
#include <Fdo.h>

#include "FDO\Connections\mgIConnectionPropertyDictionaryImp.h"

NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::IConnectionPropertyDictionaryImp(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoIConnectionPropertyDictionary* NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::GetImpObj()
{
    return static_cast<FdoIConnectionPropertyDictionary*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

array<System::String^>^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::PropertyNames::get()
{
    FdoString** unstr;
    FdoInt32 uncount;
    EXCEPTION_HANDLER(unstr = GetImpObj()->GetPropertyNames(uncount))
    return FdoStringArrayToStringArray(unstr, uncount);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::GetProperty(System::String^ name)
{
    FdoString* result;
    EXCEPTION_HANDLER(result = GetImpObj() -> GetProperty(StringToUni(name)))
    return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::SetProperty( System::String^ name,  System::String^ value)
{
    EXCEPTION_HANDLER(GetImpObj() -> SetProperty(StringToUni(name), StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::GetPropertyDefault( System::String^ name)
{
    FdoString* result;
    EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyDefault(StringToUni(name)))
    return CHECK_STRING(result);
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::IsPropertyRequired(System::String^ name)
{
    System::Boolean b;
    EXCEPTION_HANDLER(b = !!GetImpObj()->IsPropertyRequired(StringToUni(name)))
    return b;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::IsPropertyProtected(System::String^ name)
{
    System::Boolean b;
    EXCEPTION_HANDLER(b = !!GetImpObj()->IsPropertyProtected(StringToUni(name)))
    return b;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::IsPropertyEnumerable(System::String^ name)
{
    System::Boolean b;
    EXCEPTION_HANDLER(b = !!GetImpObj()->IsPropertyEnumerable(StringToUni(name)))
    return b;
}

array<System::String^>^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::EnumeratePropertyValues(System::String^ name)
{
    FdoString** unstr;
    FdoInt32 uncount;
    EXCEPTION_HANDLER(unstr = GetImpObj()->EnumeratePropertyValues(StringToUni(name), uncount))
    return FdoStringArrayToStringArray(unstr, uncount);
}

System::String^ NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::GetLocalizedName(System::String^ name)
{
    FdoString* result;
    EXCEPTION_HANDLER(result = GetImpObj()->GetLocalizedName(StringToUni(name)))
    return CHECK_STRING(result);
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::IsPropertyFileName(System::String^ name)
{
    System::Boolean b;
    EXCEPTION_HANDLER(b = !!GetImpObj()->IsPropertyFileName(StringToUni(name)))
    return b;
}

System::Boolean NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionaryImp::IsPropertyFilePath(System::String^ name)
{
    System::Boolean b;
    EXCEPTION_HANDLER(b = !!GetImpObj()->IsPropertyFilePath(StringToUni(name)))
    return b;
}
