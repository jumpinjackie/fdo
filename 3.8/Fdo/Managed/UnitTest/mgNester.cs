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
using OSGeo.FDO.Common.Xml;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgNester.
	/// </summary>
	public class mgNester : XmlSaxHandler
	{
        public mgNester(XmlReader reader)
            : base(IntPtr.Zero, false)
		{
			mReader = reader;
		}

        protected virtual XmlSaxHandler XmlStartElement( XmlSaxContext saxContext, string uri, string localName, string qName, XmlAttributeCollection atts)
        {
            // Try re-entrant call to Parse.
            mReader.Parse();

            return null;
        }

        protected XmlReader mReader;
	}
}

