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
