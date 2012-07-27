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
using System.Text;
using OSGeo.FDO.Common.Io;

namespace Fdo_Test
{
    /// <summary>
    /// Summary description for mgStringStream.
    /// </summary>
    public class mgStringStream : IoStream
    {
        public mgStringStream(string contents)
        {
            mContents = (new ASCIIEncoding()).GetBytes(contents);
            mIndex = 0;
        }
       
        public virtual Int64 Read( byte[] buffer, Int64 count )
        {
            if ( mContents[mIndex] == 0 )
                return 0;

            buffer[0] = mContents[mIndex++];
            return 1;
        }

        public  virtual void Write( byte[] buffer, Int64 count )
        {
        }

        public virtual void Write( IoStream stream, Int64 count )
        {
        }

        public override void Write( IoStream stream)
        {
            Write(stream, 0);
        }

        public override Int64 Length
        {
            get
            {
                return -1;
            }
            set
            {
                
            }
        }

        public override Int64 Index
        {
            get
            {
                return 0;
            }
        }

        public override void Skip( Int64 offset )
        {
        }

        public override void Reset()
        {
        }

        public override bool CanWrite
        {
            get
            {
                return false;
            }
        }

        public override bool HasContext
        {
            get
            {
                return false;
            }
        }

        private byte [] mContents;
        private int mIndex;
    }
}

