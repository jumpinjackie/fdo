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

#include "Pch.h"
#include "CommonTest.h"
#include <Common/Vector.h>

#ifdef _DEBUG
//#define DEBUG_DETAIL	1
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (CommonTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (CommonTest, "CommonTest");

CommonTest::CommonTest(void)
{

}

CommonTest::~CommonTest(void)
{
}

void CommonTest::setUp()
{
    // nothing to do at setup time... yet
}

// test types across platforms
void CommonTest::testTypes()
{
	// Test that the types used in Fdo work on all platforms.
	// Note: a failure here may require changes in the code elsewhere.

	// ... first make sure there the sizes we expect
	FDO_CPPUNIT_ASSERT(sizeof(unsigned char) == 1);
	FDO_CPPUNIT_ASSERT(sizeof(FdoInt8) == 1);
	FDO_CPPUNIT_ASSERT(sizeof(FdoInt16) == 2);
	FDO_CPPUNIT_ASSERT(sizeof(FdoInt32) == 4);
	FDO_CPPUNIT_ASSERT(sizeof(FdoInt64) == 8);
#ifdef _WIN32
	FDO_CPPUNIT_ASSERT(sizeof(wchar_t) == 2);
#else
	FDO_CPPUNIT_ASSERT(sizeof(wchar_t) == 4);
#endif
	FDO_CPPUNIT_ASSERT(sizeof(bool) == 1);
	FDO_CPPUNIT_ASSERT(sizeof(double) == 8);
	FDO_CPPUNIT_ASSERT(sizeof(float) == 4);

	// if this assertion fails FDO should be OK since the constant
	// size _int32 FdoInt32 is used everywhere, but all providers
	// and anything else running on the same machine needs checked
	// for "int" use.
	FDO_CPPUNIT_ASSERT(sizeof(int) == sizeof(FdoInt32));

	// ...now check that the range of values supported work
	FdoInt8	i8 = -128;				// SCHAR_MIN
	FDO_CPPUNIT_ASSERT(i8 == -128);
	i8 = 127;						// SCHAR_MAX
	FDO_CPPUNIT_ASSERT(i8 == 127);

	FdoByte	b = 0;
	FDO_CPPUNIT_ASSERT(b == 0);
	b = 0xff;						// UCHAR_MAX
	FDO_CPPUNIT_ASSERT(b == 255);			

	FdoInt16	i16 = -32768;		// SHRT_MIN
	FDO_CPPUNIT_ASSERT(i16 == -32768);
	i16 = 32767;					// SHRT_MAX
	FDO_CPPUNIT_ASSERT(i16 == 32767);

	FdoInt32	i32 = (-2147483647-1);	// INT_MIN/LONG_MIN
	FDO_CPPUNIT_ASSERT(i32 == (-2147483647-1));
	i32 = 2147483647;					// INT_MAX/LONG_MAX
	FDO_CPPUNIT_ASSERT(i32 == 2147483647);

	// TODO: byte comparision for big-endian little-endian
}

// test exceptions
void CommonTest::testExceptions()
{
	// test out of memory failures
	// ... specifically new NULL vs exception

	// ...now check for floating point exceptions handling
	// TODO: overflow
	// TODO: underflow
	// TODO: outofrange

	// ...Others??

}

void CommonTest::testCollections()
{
    // Test named collection, with hash map, for objects that can be renamed.

    FdoFeatureSchemasP schemas = FdoFeatureSchemaCollection::Create(NULL);

    FdoInt32 idx;

    // Add enough items to force use of hash map
    for ( idx = 0; idx < 1000; idx++ ) {
        schemas->Add(
            FdoFeatureSchemaP(
                FdoFeatureSchema::Create(
                    FdoStringP::Format( L"%d", idx ),
                    L""
                )
            )
        );
    }

    FdoFeatureSchemaP schema = schemas->FindItem(L"593");
	FDO_CPPUNIT_ASSERT(schema);
    // Rename an item. This makes the hash map stale. In hash map, "renamed" schema is still under
    // key "593".
    schema->SetName( L"renamed" );

    // Add another "593". should work since other one was renamed
    schema = FdoFeatureSchema::Create( L"593", L"" );
    schemas->Add( schema );

    // Add a second "renamed" schema. Must fail since duplicate.
    schema = FdoFeatureSchema::Create( L"renamed", L"" );
	bool failed = false;
	try {
        schemas->Add( schema );
	}
	catch ( FdoException* ex) {
        ex->Release();
		failed = true;
	}

	FDO_CPPUNIT_ASSERT(failed);
}

// Do some stuff with FdoIntArray, as non-Byte arrays are not currently 
// tested elsewhere.
void TestIntArray()
{
    FdoIntArray* ints = FdoIntArray::Create();
    ints = FdoIntArray::Append(ints, 7);
    FdoInt32 moreInts [] = { 8, 9 };
    FdoInt32 numMoreInts = sizeof(moreInts)/sizeof(moreInts[0]);
    ints = FdoIntArray::Append(ints, numMoreInts, moreInts);
    int element = (*ints)[1];
    int* arrayAddress = &((*ints)[0]);
    int* dataAddress = ints->GetData();
#ifdef DEBUG_DETAIL
    printf("The second element is %d\n", element);
	char* addressCheck = (arrayAddress == dataAddress) ? "fine" : "worrisome";
    printf("Everything looks %s\n", addressCheck);;
#endif

    (*ints)[0] = -7;		    // Test value assigment.
	(*ints)[1] = 9;
//	ints->SetValue(1, 9);		// more normal assignment

	// Sure to cause re-allocation.
    FdoInt32 evenMoreInts [] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    numMoreInts = sizeof(evenMoreInts)/sizeof(evenMoreInts[0]);
    ints = FdoIntArray::Append(ints, numMoreInts, evenMoreInts);

#ifdef DEBUG_DETAIL
    printf("Full array:");
    for (FdoInt32 i=0;  i < ints->GetCount();  i++)
    {
//        printf(" %d", ints->GetValue(i));
        printf(" %d", (*ints)[i]);
    }
    printf("\n");

	// test array index access
    for (FdoInt32 i=0;  i < ints->GetCount();  i++)
    {
		printf("[%d]=%d\n", i, (*ints)[i]);
    }
#endif
	FDO_CPPUNIT_ASSERT((*ints)[0] == -7);
	FDO_CPPUNIT_ASSERT((*ints)[1] == 9);
	FDO_CPPUNIT_ASSERT((*ints)[2] == 9);
	FDO_CPPUNIT_ASSERT((*ints)[3] == 10);
	FDO_CPPUNIT_ASSERT((*ints)[4] == 11);
	FDO_CPPUNIT_ASSERT((*ints)[5] == 12);
	FDO_CPPUNIT_ASSERT((*ints)[6] == 13);
	FDO_CPPUNIT_ASSERT((*ints)[7] == 14);
	FDO_CPPUNIT_ASSERT((*ints)[8] == 15);
	FDO_CPPUNIT_ASSERT((*ints)[9] == 16);
	FDO_CPPUNIT_ASSERT((*ints)[10] == 17);
	FDO_CPPUNIT_ASSERT((*ints)[11] == 18);
	FDO_CPPUNIT_ASSERT((*ints)[12] == 19);
	FDO_CPPUNIT_ASSERT((*ints)[13] == 20);
	// Full array: -7 9 9 10 11 12 13 14 15 16 17 18 19 20

	// test reuse
	ints->Clear();
	for (FdoInt32 j=0; j<10000; j++)
	{
		ints = FdoIntArray::Append(ints, j);
	}
	FDO_CPPUNIT_ASSERT((*ints)[0] == 0);
	FDO_CPPUNIT_ASSERT((*ints)[5000] == 5000);
	FDO_CPPUNIT_ASSERT((*ints)[9999] == 9999);

	// cleanup
    FDO_SAFE_RELEASE(ints);
}

void CommonTest::testArrays()
{
	//TODO: test base Array.h and ArrayHelper.h operations

	// test arrays
    TestIntArray();

	//TODO: add other types
}

void CommonTest::testOutput()
{
/* TODO: can't call FdoStringUtility from UnitTest.
    wchar_t szNumber[64];

    FdoStringUtility::FormatDouble(.01, szNumber, 64);
    FdoStringUtility::FormatDouble(.0000001, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e-6, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e-15, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e-16, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e-17, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e-64, szNumber, 64);
    FdoStringUtility::FormatDouble(10, szNumber, 64);
    FdoStringUtility::FormatDouble(1000000, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e6, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e15, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e16, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e17, szNumber, 64);
    FdoStringUtility::FormatDouble(1.0e64, szNumber, 64);
    FdoStringUtility::FormatDouble(0, szNumber, 64);
    FdoStringUtility::FormatDouble(-.01, szNumber, 64);
    FdoStringUtility::FormatDouble(-.0000001, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e-6, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e-15, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e-16, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e-17, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e-64, szNumber, 64);
    FdoStringUtility::FormatDouble(-10, szNumber, 64);
    FdoStringUtility::FormatDouble(-1000000, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e6, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e15, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e16, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e17, szNumber, 64);
    FdoStringUtility::FormatDouble(-1.0e64, szNumber, 64);
*/
}

void CommonTest::testString()
{
	FdoStringP s1 = FdoStringP::Format( NULL );
	s1 = L"abcdefghi";

	CPPUNIT_ASSERT( wcscmp( s1.Mid(2,5), L"cdefg" ) == 0 );

	FdoStringP s2 = s1.Replace( NULL, L"z" );

	CPPUNIT_ASSERT( wcscmp( s2, L"abcdefghi" ) == 0 );

	s2 = s1.Replace( L"g", NULL );

	s1 = s2.Replace( L"i", L"" );

	s2 = s1.Replace( L"cd", L"c_d" );

	s2 = s2.Replace( L"abc", L"ca" );

	CPPUNIT_ASSERT( wcscmp( s2, L"ca_defh" ) == 0 );

	s1 = L"Mixed Case";

	CPPUNIT_ASSERT( wcscmp(s1.Upper(), L"MIXED CASE") == 0 );
	CPPUNIT_ASSERT( wcscmp(s1.Lower(), L"mixed case") == 0 );

    // more NULL string tests
	CPPUNIT_ASSERT( !(s1 == NULL) );
	CPPUNIT_ASSERT( s1 != NULL );
	CPPUNIT_ASSERT( s1.Contains(NULL) );
	CPPUNIT_ASSERT( (s1 + ((FdoString*)NULL)) == L"Mixed Case" );
    s1 += NULL;
	CPPUNIT_ASSERT( s1 == L"Mixed Case" );
	CPPUNIT_ASSERT( s1.Left(NULL) == L"" );
	CPPUNIT_ASSERT( s1.Right(NULL) == L"Mixed Case" );

    s1 = L"";
	CPPUNIT_ASSERT( s1 == NULL );
	CPPUNIT_ASSERT( s1.Contains(NULL) );

    FdoStringP s3( L"static string", true );
    s1 = s3;
    s3 = L"non static string";

	CPPUNIT_ASSERT( s3 == L"non static string" );

	CPPUNIT_ASSERT( s1 == L"static string" );

    // IsNumber tests

    s1 = L"1"; // simple integer
	CPPUNIT_ASSERT( s1.IsNumber() );

    s1 = L"6232555"; // bigger integer
	CPPUNIT_ASSERT( s1.IsNumber() );

    s1 = L"3.14159";  // real
	CPPUNIT_ASSERT( s1.IsNumber() );

    s1 = L"1.02.03"; // two radixes, not a number
	CPPUNIT_ASSERT( !s1.IsNumber() );

    s1 = L"1b"; // mixed, digit first
	CPPUNIT_ASSERT( !s1.IsNumber() );

    s1 = L"ghi4"; // mixed, alpha first
	CPPUNIT_ASSERT( !s1.IsNumber() );

    s1 = L"ghi"; // alphabetic
	CPPUNIT_ASSERT( !s1.IsNumber() );

    s1 = L"\xF6\xE4\x33\xB4\xDF\xC4"; // some non-ASCII7 characters
	CPPUNIT_ASSERT( !s1.IsNumber() );

    s1 = L"12345";
	CPPUNIT_ASSERT( s1.ToLong() == 12345 );

    s1 = L"12345.678";
	CPPUNIT_ASSERT( s1.ToLong() == 12345 );

    s1 = L"0";
	CPPUNIT_ASSERT( s1.ToLong() == 0 );

    s1 = L"0.0";
	CPPUNIT_ASSERT( s1.ToLong() == 0 );

    s1 = L"0x10";
	CPPUNIT_ASSERT( s1.ToLong() == 16 );

    s1 = L"0X101";
	CPPUNIT_ASSERT( s1.ToLong() == 257 );

    s1 = L"\\0xff";
	CPPUNIT_ASSERT( s1.ToLong() == 255 );

    s1 = L"\\0Xff";
	CPPUNIT_ASSERT( s1.ToLong() == 255 );
}

void CommonTest::testStringCollection()
{
    FdoStringsP c0 = FdoStringCollection::Create( L"orange yellow violet", L" " );
	FdoStringsP c1;
	FdoStringsP c2 = FdoStringCollection::Create( L"red, green, blue", L", " );
	FdoStringsP c3;

	c1 = c0;

	FdoStringsP c4 = FdoStringCollection::Create(c1);

	c1 += c2;

	FDO_CPPUNIT_ASSERT( wcscmp( c1->ToString(), L"orange, yellow, violet, red, green, blue" ) == 0 );
	
	c3 = c2 + c4;

	FDO_CPPUNIT_ASSERT( wcscmp( c3->ToString(), L"red, green, blue, orange, yellow, violet" ) == 0 );

	c4 += FdoStringP(L"black");

	c3 = c4 + c2 + L"white";

	FDO_CPPUNIT_ASSERT( wcscmp( c4->ToString(L" "), L"orange yellow violet black" ) == 0 );
	
	FDO_CPPUNIT_ASSERT( wcscmp( c3->ToString(L"-"), L"orange-yellow-violet-black-red-green-blue-white" ) == 0 );

    FDO_CPPUNIT_ASSERT( c3->IndexOf(L"violet") == 2 );
    FDO_CPPUNIT_ASSERT( c3->IndexOf(L"Violet") == -1 );
    FDO_CPPUNIT_ASSERT( c3->IndexOf(L"Violet",false) == 2 );
    FDO_CPPUNIT_ASSERT( c3->IndexOf(L"orange",true) == 0 );
    FDO_CPPUNIT_ASSERT( c3->IndexOf(L"orangE",true) == -1 );

}

void CommonTest::testDictionary()
{
	bool failed = false;
	FdoDictionaryP dictionary = FdoDictionary::Create();

	// Add some items where name is Roman and value is Indo-Arabic

	dictionary->Add( FdoDictionaryElementP( FdoDictionaryElement::Create(L"CM", L"900") ) );
	dictionary->Add( FdoDictionaryElementP( FdoDictionaryElement::Create(L"MCMLXXX", L"1980") ) );
	dictionary->Add( FdoDictionaryElementP( FdoDictionaryElement::Create(L"MMIII", L"2003") ) );
	dictionary->Insert( 1, FdoDictionaryElementP( FdoDictionaryElement::Create(L"MMIV", L"2004") ) );

	// Get and find existing items

	FDO_CPPUNIT_ASSERT(
		wcscmp( 
			FdoDictionaryElementP( dictionary->GetItem(L"MCMLXXX") )->GetValue(),
			L"1980" 
		) == 0 
	);

	FDO_CPPUNIT_ASSERT(
		wcscmp( 
			FdoDictionaryElementP( dictionary->FindItem(L"MMIV") )->GetValue(),
			L"2004" 
		) == 0 
	);

	// Get non-existing item, throws exception

	failed = false;
	try {
		dictionary->GetItem( L"MCMIX" );
	}
	catch ( FdoException* ex) {
        ex->Release();
		failed = true;
	}

	FDO_CPPUNIT_ASSERT(failed);

	// Find non-existing item, returns null

	FDO_CPPUNIT_ASSERT( dictionary->FindItem( L"MCMIX") == NULL );

	// Replace an item and then try to retrieve it.

	dictionary->SetItem( 1, FdoDictionaryElementP( FdoDictionaryElement::Create(L"MCMIX", L"1909" ) ) );

	FDO_CPPUNIT_ASSERT(
		wcscmp( 
			FdoDictionaryElementP( dictionary->GetItem(L"MCMIX") )->GetValue(),
			L"1909" 
		) == 0 
	);

	// Add duplicate name
		
	failed = false;
	try {
		dictionary->Add( FdoDictionaryElementP( FdoDictionaryElement::Create(L"MCMLXXX", L"1980") ) );
	}
	catch ( FdoException* ex) {
        ex->Release();
		failed = true;
	}
	FDO_CPPUNIT_ASSERT(failed);

	// Insert duplicate name

	failed = false;
	try {
		dictionary->Insert( 0, FdoDictionaryElementP( FdoDictionaryElement::Create(L"CM", L"900") ) );
	}
	catch ( FdoException* ex) {
        ex->Release();
		failed = true;
	}
	FDO_CPPUNIT_ASSERT(failed);

	// Replace with duplicate name.

	failed = false;
	try {
		dictionary->SetItem( 0, FdoDictionaryElementP( FdoDictionaryElement::Create(L"MMIII", L"2003") ) );
	}
	catch ( FdoException* ex) {
        ex->Release();
		failed = true;
	}
	FDO_CPPUNIT_ASSERT(failed);

	// This replace should succeed since names of new item and
	// item being replaced are the same.
	dictionary->SetItem( 0, FdoDictionaryElementP( FdoDictionaryElement::Create(L"CM", L"Nine Hundred") ) );

	// Verify that previous setitem worked.
	FDO_CPPUNIT_ASSERT(
		wcscmp( 
			FdoDictionaryElementP( dictionary->GetItem(L"CM") )->GetValue(),
			L"Nine Hundred" 
		) == 0 
	);

    // Test that duplicate name checking works when against the hash map.
    // First, put lots of items in the dictionary to trigger the user of the map

    int idx;

    for ( idx = 0; idx < 1000; idx++ ) {
		dictionary->Add( 
            FdoDictionaryElementP( 
                FdoDictionaryElement::Create(
                    FdoStringP::Format( L"%d", idx ),
                    L""
                ) 
            ) 
        );
    }

    // Try adding a duplicate entry.
    failed = false;
	try {
		dictionary->Add( 
            FdoDictionaryElementP( 
                FdoDictionaryElement::Create(
                    FdoStringP::Format( L"535", idx ),
                    L""
                ) 
            ) 
        );
	}
	catch ( FdoException* ex) {
        ex->Release();
		failed = true;
	}
	FDO_CPPUNIT_ASSERT(failed);

}

void CommonTest::testVector()
{
    FdoVectorP vec1 = FdoVector::Create( L"3.5 2.1 8.3 4", L" " );
    FdoVectorP vec2 = FdoVector::Create( L"3.5 2.1 8.3 4", L" " );
    FdoVectorP vec3 = FdoVector::Create( L"3.5 2.12 8.3 4", L" " );
    FdoVectorP vec4 = FdoVector::Create( L"3.5 2.1 8.3 4 0.1", L" " );
    FdoVectorP vec5 = FdoVector::Create( L"3.5 2.1 8.29 4", L" " );
    FdoVectorP vec6 = FdoVector::Create( L"3.5 2.1 8.3", L" " );
    FdoVectorP vec7 = FdoVector::Create( L"3.5 2.1 8.3 4 -0.1", L" " );

    FdoVectorP vec8 = vec1 + vec4;
    FdoVectorP vec9 = vec1 - vec4;
    FdoVectorP vec10 = vec1 + vec3;
    FdoVectorP vec11 = FdoVector::Create( vec1 );
    vec11->Append( vec5 );

    FDO_CPPUNIT_ASSERT( vec1 == vec2 );
    FDO_CPPUNIT_ASSERT( vec1 != vec3 );
    FDO_CPPUNIT_ASSERT( vec1 <= vec2 );
    FDO_CPPUNIT_ASSERT( vec1 >= vec2 );

    FDO_CPPUNIT_ASSERT( !(vec1 != vec2) );
    FDO_CPPUNIT_ASSERT( !(vec1 == vec3) );
    FDO_CPPUNIT_ASSERT( !(vec1 > vec2) );
    FDO_CPPUNIT_ASSERT( !(vec1 < vec2) );

    FDO_CPPUNIT_ASSERT( vec1 < vec3 );
    FDO_CPPUNIT_ASSERT( vec1 < vec4 );
    FDO_CPPUNIT_ASSERT( vec1 > vec5 );
    FDO_CPPUNIT_ASSERT( vec1 > vec6 );
    FDO_CPPUNIT_ASSERT( vec1 > vec7 );
    
    FDO_CPPUNIT_ASSERT( !(vec1 >= vec3) );
    FDO_CPPUNIT_ASSERT( !(vec1 >= vec4) );
    FDO_CPPUNIT_ASSERT( !(vec1 <= vec5) );
    FDO_CPPUNIT_ASSERT( !(vec1 <= vec6) );
    FDO_CPPUNIT_ASSERT( !(vec1 <= vec7) );

    FDO_CPPUNIT_ASSERT( vec1 <= vec3 );
    FDO_CPPUNIT_ASSERT( vec1 <= vec4 );
    FDO_CPPUNIT_ASSERT( vec1 >= vec5 );
    FDO_CPPUNIT_ASSERT( vec1 >= vec6 );
    FDO_CPPUNIT_ASSERT( vec1 >= vec7 );

    FDO_CPPUNIT_ASSERT( !(vec1 > vec3) );
    FDO_CPPUNIT_ASSERT( !(vec1 > vec4) );
    FDO_CPPUNIT_ASSERT( !(vec1 < vec5) );
    FDO_CPPUNIT_ASSERT( !(vec1 < vec6) );
    FDO_CPPUNIT_ASSERT( !(vec1 < vec7) );

    FdoStringP s8 = vec8->ToString( L" " );
    FdoStringP s9 = vec9->ToString( L" " );
    FdoStringP s10 = vec10->ToString( L" " );
    FdoStringP s11 = vec11->ToString( L" " );

    FDO_CPPUNIT_ASSERT( s8 == FdoStringP(L"7 4.2 16.6 8 0.1") );
    FDO_CPPUNIT_ASSERT( s9 == FdoStringP(L"0 0 0 0 -0.1") );
    FDO_CPPUNIT_ASSERT( s10 == FdoStringP(L"7 4.22 16.6 8") );
    FDO_CPPUNIT_ASSERT( s11 == FdoStringP(L"3.5 2.1 8.3 4 3.5 2.1 8.29 4") );
}



