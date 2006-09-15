/*
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

#ifndef CPP_UNIT_CAPABILITYTEST_H
#define CPP_UNIT_CAPABILITYTEST_H

/* 
 * A test case for the Capability.
 */
class FdoCapabilityTest : public CppUnit::TestCase
{

  // A few helper classes to verify the list of supported functions.
  class argObject
  {
    public:
        wchar_t     *name;
        FdoDataType dataType;
        bool        found;

        argObject  ();
        ~argObject ();
  };

  class fctSignature
  {
    public:
        FdoDataType     returnDataType;
        FdoPropertyType returnPropertyType;
        bool            expectDataType;
        argObject       *arguments[10];
        FdoInt32        argCount;
        bool            found;

        fctSignature  ();
        ~fctSignature ();
  };

  class fctObject
  {
    public:
        wchar_t      *name;
        bool         isAggregate;
        fctSignature *signature[20];
        FdoInt32     signatureCount;
        bool         found;

        fctObject  ();
        ~fctObject ();
  };


  CPPUNIT_TEST_SUITE( FdoCapabilityTest );
  CPPUNIT_TEST( connection );
  CPPUNIT_TEST( datastore );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoCapabilityTest(void);
    virtual ~FdoCapabilityTest(void);
    void setUp ();

protected:
    virtual void  set_provider() {};   
    void connection ();
    void datastore ();
    virtual void InitSupportedFunctions ();
    virtual fctSignature *InitSignature (FdoDataType     retDataType,
                                         FdoPropertyType retPropertyType,
                                         bool            expDataType);
    virtual argObject *InitArgument (FdoString   *argName,
                                     FdoDataType argDataType);

    FdoString *GetDataType (FdoDataType dataType);
    void CheckFunction (FdoString *name, bool isAggregate);
    void CheckSignature (FdoDataType fctReturnType, FdoString *argName, FdoDataType argDataType);
    void CheckExpectation ();


    fctObject    *m_supportedFunctions[20];
    fctObject    *m_currFctObject;
    FdoInt32     m_numSupportedFunctions;

};

#endif // CPP_UNIT_CAPABILITYTEST_H
