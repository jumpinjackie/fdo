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

using System;
using OSGeo.Common.Xml;

namespace Fdo_Test
{
    /// <summary>
    /// Summary description for mgExtractor.
    /// </summary>
    public class mgExtractor : XmlSaxHandler
    {
        public mgExtractor(XmlWriter writer)
        {
            mWriter = writer;
        }

        public override IXmlSaxHandler XmlStartElement( XmlSaxContext saxContext, string uri,  string localName, string qName, XmlAttributeCollection atts )
        {
            // return NULL ( keep this object as the SAX Handler ) by default.
            XmlSaxHandler ret = null;

            // If the current element is an fds:CopyTag
            if ((uri == "http://fdo.osgeo.org/schemas/fds") && (localName == "CopyTag")) 
            {

                // Todo. Most explicitly free the old copy handler first to ensure
                // that the previous CopyTag is ended before this one is written. 
                // Changes to GisXmlReader (to dereference popped SAX Handlers ) 
                // are required before this workaround can be removed.
                mCopyHandler = null;

                // Create a copy handler that writes the CopyTag and its sub-elements
                // to the destination doc.
                mCopyHandler = new XmlCopyHandler( mWriter, uri, localName, qName, atts, null );
		
                // Declare any namespaces needed in the output doc. These are 
                // placed on the CopyTag.
                mWriter.WriteAttribute( "xmlns:ctn", "http://www.ctn.com" );
                mWriter.WriteAttribute( "xmlns:dest", "http://www.changenamespace.com" );

                // Activate it by making it the new SAX handler. When the reader is finished parsing
                // the spatial context, this SCExtractor will be automatically re-instated as the 
                // SAX Handler. 
                ret = (XmlCopyHandler) mCopyHandler;
            }

            return ret;
        }

        protected XmlWriter mWriter;
        protected XmlCopyHandler mCopyHandler;
    }
}

