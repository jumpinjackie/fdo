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
using OSGeo.Common.Io;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgXmlFormatter.
	/// </summary>
	public class mgXmlFormatter
	{
        private void initial(IoStream inStream, IoStream outStream, int elementIndent)
        {
            mInStream = inStream;
            mOutStream = outStream;
            mIndent = elementIndent;
            mLevel = 0;
        }

        public mgXmlFormatter(IoStream inStream, IoStream outStream, int elementIndent)
        {
            initial(inStream, outStream, elementIndent);
        }

        public mgXmlFormatter(IoStream inStream, IoStream outStream)
        {
            initial(inStream, outStream, 4);
        }

        public void Format()
        {
            byte [] inBuffer = new byte[4096];
            byte [] outBuffer = new byte[8192];
            Int64 len = 0;
            Int64 inIdx = 0;
            Int64 outIdx = 0;
            byte cr = Convert.ToByte('\n');

            while ( (len = mInStream.Read( inBuffer, 4096 )) > 0 ) 
            {
                for ( inIdx = 0, outIdx = 0; inIdx < len; inIdx++ ) 
                {
                    outBuffer[outIdx++] = inBuffer[inIdx];
                    if ( inBuffer[inIdx] == '>' )
                        outBuffer[outIdx++] = cr;
                }
                mOutStream.Write( outBuffer, Convert.ToUInt32(outIdx) );
            }
        }


        private IoStream mInStream;
        private IoStream mOutStream;
        private int mIndent;
        private int mLevel;
	}
}

