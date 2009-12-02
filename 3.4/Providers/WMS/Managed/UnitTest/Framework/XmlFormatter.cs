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
using System.Collections.Generic;
using System.Text;

using OSGeo.Common.Io;

namespace unit_test.Framework
{
    class XmlFormatter
    {
        private IoStream mInStream;
        private IoStream mOutStream;
        private int mIndent;
        private int mLevel;

        public XmlFormatter(IoStream inStream, IoStream outStream, int elementIndent)
        {
            mInStream = inStream;
            mOutStream = outStream;
            mIndent = elementIndent;
            mLevel = 0;
        }

        public void Format()
        {
            Byte[] buffer = new Byte[1];
            Byte cr = (Byte)'\n';
            Byte cr2 = (Byte)'\r';
            Byte cr3 = (Byte)'\t';

            while ( mInStream.Read( buffer, 1 ) > 0 ) {
                if ( buffer[0] != cr && buffer[0] != cr2 && buffer[0] != cr3)
                    mOutStream.Write( buffer, 1 );
            }
        }
    }
}
