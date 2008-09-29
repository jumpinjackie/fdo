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
using System.Diagnostics;

using OSGeo.FDO.Connections;

namespace unit_test.Framework
{
    class BaseTestWithConnection : BaseTest
    {
        private IConnection testConnect;
        private IConnectionProvider m_ConnectProvider;

        public BaseTestWithConnection(IConnectionProvider connectProvider)
        {
            m_ConnectProvider = connectProvider;
            testConnect = m_ConnectProvider.GetConnection();
        }

        public override void SetUp()
        {            
            ConnectionState state = testConnect.Open();
            Debug.Assert(state == ConnectionState.ConnectionState_Open);
        }

        public override void TearDown()
        {
            if (testConnect.ConnectionState == ConnectionState.ConnectionState_Open)
            {
                testConnect.Close();
            }
        }

        public IConnection ConnectionInitialized
        {
            get
            {
                return testConnect;
            }
        }

        public IConnection ConnectionNew
        {
            get
            {
                return m_ConnectProvider.GetConnection();
            }
        }

        public string GetLocation()
        {
            return m_ConnectProvider.GetLocation() ;
        }

        public void SetLocation(string loc)
        {
            m_ConnectProvider.SetLocation(loc);
        }

        public void unit_test_assert(string smsg, bool b)
        {
            Debug.Assert(b, smsg);
        }
    }
}

