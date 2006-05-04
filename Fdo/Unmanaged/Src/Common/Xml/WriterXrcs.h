#ifndef FDO_XML_WRITER_XRCS_H
#define FDO_XML_WRITER_XRCS_H
// 

//
// Copyright (C) 2004-2006  Autodesk, Inc.
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


#include <Common.h>

/// \cond DOXYGEN-IGNORE

// FdoXmlWriter sub-class that uses Xerces for XML name validation.
// This class is not directly exposed throught the FDO API, only via
// the FdoXmlWriter interface.
class FdoXmlWriterXrcs : public FdoXmlWriter
{
public:
    // Create the XML Writer.
    FDO_API_COMMON static FdoXmlWriterXrcs* Create( 
        FdoIoTextWriter* writer,
        FdoBoolean defaultRoot = true,
        LineFormat lineFormat = LineFormat_None,
        FdoSize    lineLength = 0
    );

    // Convert name to valid XML name, see FdoXmlWriter
    FDO_API_COMMON virtual FdoStringP EncodeName ( FdoStringP name );

    // Check if name is valid, see FdoXmlWriter
    FDO_API_COMMON virtual FdoBoolean IsValidName ( FdoStringP name );

protected:
    FdoXmlWriterXrcs() {}
    FdoXmlWriterXrcs( 
        FdoIoTextWriter* writer,
        FdoBoolean defaultRoot = true,
        LineFormat lineFormat = LineFormat_None,
        FdoSize    lineLength = 0
    );

    virtual ~FdoXmlWriterXrcs(void);

private:
};

typedef FdoPtr<FdoXmlWriterXrcs> FdoXmlWriterXrcsP;
/// \endcond

#endif

