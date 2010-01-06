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

#ifndef CPP_UNIT_FDODELETETEST_H
#define CPP_UNIT_FDODELETETEST_H

/* 
 * A test case for the DeleteCommand.
 */
class FdoDeleteTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoDeleteTest );
  CPPUNIT_TEST( FeatureDelete );
  CPPUNIT_TEST( ObjectPropDelete );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoDeleteTest(void);
    FdoDeleteTest(wchar_t*);
    virtual ~FdoDeleteTest(void);
    virtual void setUp ();

    virtual void FeatureDelete ();
    virtual void ObjectPropDelete();

protected:
    virtual void  set_provider() {};   

private:
    wchar_t  mSuffix[12];
};

#endif // CPP_UNIT_FDODELETETEST_H
