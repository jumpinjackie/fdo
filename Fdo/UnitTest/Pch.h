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

#include <Fdo.h>

#ifdef _WIN32

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>         // MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT
#endif

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

// This behaves like CPPUNIT_ASSERT, but you can trace 
// into it easily using the debugger.
#define FDO_CPPUNIT_ASSERT(condition)                      \
  if (!(condition)) ::CppUnit::Asserter::failIf( true,   \
                                 (#condition),             \
                                 CPPUNIT_SOURCELINE() )

// This works with the next macro to wrap each unit test so that any 
// uncaught FdoExceptions are caught and reported.
#define FDO_CPPUNIT_DEFINE( testMethod )\
    void UNITTEST_##testMethod()\
    {\
        try\
        {\
            __ThisTestFixtureType::testMethod();\
        }\
        catch (FdoException* exception)\
        {\
            PrintException(exception);\
            exception->Release();\
            CPPUNIT_ASSERT_MESSAGE("Unhandled FdoException in " #testMethod, false);\
        }\
    }\

// This is a modification of CPPUNIT_TEST
#undef  CPPUNIT_TEST
#define CPPUNIT_TEST( testMethod )\
    builder.addTestCaller( #testMethod,\
        &__ThisTestFixtureType::UNITTEST_##testMethod,\
        (__ThisTestFixtureType*)factory->makeFixture() ) 


// This should work on other compilers/platforms. It works 
// because IDisposable is always the base class and its first data
// member following the vtable is an integer that holds the 
// refcount.  We can't just cast to FdoIDisposable because the 
// m_refCount member is protected.
class DbgRefCount
{
public:
	// this member forces this class to have a vtable.
	virtual     void ForceVTable() {;}

	FdoInt32    m_refCount;
};

inline FdoInt32 GET_REFCOUNT(FdoIDisposable* p)
{
	return ((DbgRefCount*)p)->m_refCount;
}

// This is a method to dump the messages from an exception and all its parents.
void PrintException(FdoException* exception);


