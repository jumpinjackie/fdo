#ifndef FDO_COMMON_INTERNAL_H
#define FDO_COMMON_INTERNAL_H
// 

//
// Copyright (C) 2004-2007  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include <FdoCommon.h>
#include <Common/Xml/Attribute.h>

// FdoCommonInternal contains various functions internal to FdoCommon.dll.
// These functions were created as workarounds to preserve binary compatibility.
// Eventually they should be moved to the classes that they operate on.
class FdoCommonInternal
{

public:

    // Faster dictionary element creation function. Re-uses name and value strings,
    // rather than allocating new memory for them.
    static FdoDictionaryElement* CreateDictionaryElement(FdoStringP& name, FdoStringP& value, int dummy);

    // Faster XML Attribute creation function. Re-uses name and value strings,
    // rather than allocating new memory for them.
    //
    /// \param name 
    /// Input unique attribute name. If the attribute name is namespace qualified
    /// the name is {uri}:{localName}. Otherwise, it is {localName}
    /// \param value 
    /// Input attribute value.
    /// \param localName 
    /// Input attribute name without namespace qualification.
    /// \param uri 
    /// Input uri for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param prefix 
    /// Input prefix for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param valueUri 
    /// Input uri for the attribute value's namespace. L"" if the attribute
    /// value is not namespace qualified.
    /// \param localValue 
    /// Input attribute value without namespace qualification.
    /// \param valuePrefix 
    /// Input prefix for the attribute value's namespace. L"" if the attribute
    /// value is not namespace qualified.
    /// 
    /// \return
    /// Returns FdoXmlAttribute
    /// 
    static FdoXmlAttribute* CreateXmlAttribute(
        FdoStringP& name, 
        FdoStringP& value, 
        FdoStringP& localName,
        FdoStringP& uri,
        FdoStringP& prefix,
        FdoStringP& valueUri,
        FdoStringP& localValue,
        FdoStringP& valuePrefix
    );
};

#endif


