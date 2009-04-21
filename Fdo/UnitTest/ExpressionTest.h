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

#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_EXPRESSIONTEST_H
#define CPP_UNIT_EXPRESSIONTEST_H

// Test cases for Expressions.
class ExpressionTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testBinaryExpression);
    FDO_CPPUNIT_DEFINE(testDataValue);
    FDO_CPPUNIT_DEFINE(testFunction);
    FDO_CPPUNIT_DEFINE(testGeometryValue);
    FDO_CPPUNIT_DEFINE(testIdentifier);
    FDO_CPPUNIT_DEFINE(testParameter);
    FDO_CPPUNIT_DEFINE(testUnaryExpression);
    FDO_CPPUNIT_DEFINE(testComputedIdentifier);
    FDO_CPPUNIT_DEFINE(testDataValueConvert);

    CPPUNIT_TEST_SUITE(ExpressionTest);
//TODO:RAA need valid FGF    CPPUNIT_TEST(testExpressionTree);
    CPPUNIT_TEST(testBinaryExpression);
    CPPUNIT_TEST(testDataValue);
    CPPUNIT_TEST(testFunction);
    CPPUNIT_TEST(testGeometryValue);
    CPPUNIT_TEST(testIdentifier);
    CPPUNIT_TEST(testParameter);
    CPPUNIT_TEST(testUnaryExpression);
    CPPUNIT_TEST(testComputedIdentifier);
    CPPUNIT_TEST(testDataValueConvert);
    CPPUNIT_TEST_SUITE_END();

public:
    ExpressionTest(void);
    virtual ~ExpressionTest(void);
    void setUp ();

protected:
    void testExpressionTree();
    void testBinaryExpression();
    void testDataValue();
    void testFunction();
    void testGeometryValue();
    void testIdentifier();
    void testParameter();
    void testUnaryExpression();
    void testComputedIdentifier();
    void testDataValueConvert();

private:

// Supporting functions for TestDataValueConvert

    template <class CI, class TI> static void NormalCase ( int valCount, TI* vals, FdoString** expected, FdoDataType outType )
    {
        int i = 0;

        for ( i = 0; i < valCount; i++ ) 
        {
            FdoPtr<CI> dv1 = CI::Create( vals[i] );
            NormalSubCase( dv1.p, expected[i], outType, false, false, false );
            NormalSubCase( dv1.p, expected[i], outType, false, false, true );
            NormalSubCase( dv1.p, expected[i], outType, false, true, false );
            NormalSubCase( dv1.p, expected[i], outType, false, true, true );
            NormalSubCase( dv1.p, expected[i], outType, true, false, false );
            NormalSubCase( dv1.p, expected[i], outType, true, false, true );
            NormalSubCase( dv1.p, expected[i], outType, true, true, false );
            NormalSubCase( dv1.p, expected[i], outType, true, true, true );
        }
    }

    template <class CI, class TI> static void NormalToBoolCase ()
    {
        TI vals[] = { 0, 1 };
        FdoString* expectedVals[] = { L"FALSE", L"TRUE" };
        NormalCase<CI,TI>( 2, vals, expectedVals, FdoDataType_Boolean );
    }

    template <class CI> static void NullCases ( )
    {
        NullCase<CI>( FdoDataType_BLOB );
        NullCase<CI>( FdoDataType_Boolean );
        NullCase<CI>( FdoDataType_Byte );
        NullCase<CI>( FdoDataType_CLOB );
        NullCase<CI>( FdoDataType_DateTime );
        NullCase<CI>( FdoDataType_Decimal );
        NullCase<CI>( FdoDataType_Double );
        NullCase<CI>( FdoDataType_Int16 );
        NullCase<CI>( FdoDataType_Int32 );
        NullCase<CI>( FdoDataType_Int64 );
        NullCase<CI>( FdoDataType_Single );
        NullCase<CI>( FdoDataType_String );
    }

    template <class CI> static void NullCase ( FdoDataType outType )
    {
        NullSubCase<CI>( outType, false, false, false );
        NullSubCase<CI>( outType, false, false, true );
        NullSubCase<CI>( outType, false, true, false );
        NullSubCase<CI>( outType, false, true, true );
        NullSubCase<CI>( outType, true, false, false );
        NullSubCase<CI>( outType, true, false, true );
        NullSubCase<CI>( outType, true, true, false );
        NullSubCase<CI>( outType, true, true, true );
    }

    template <class CI> static void NullSubCase ( FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate )
    {
        FdoPtr<CI> dv = CI::Create();
        NullSubSubCase( (CI*) NULL, outType, nullIfIncompatible, shift, truncate );
        NullSubSubCase( dv.p, outType, nullIfIncompatible, shift, truncate );
    }

    template <class CI, class TI> static void IncompatibleCase ( TI val, FdoDataType outType )
    {
        FdoPtr<CI> dv1 = CI::Create( val );
        IncompatibleSubCase( dv1.p, outType, false, false, false );
        IncompatibleSubCase( dv1.p, outType, false, false, true );
        IncompatibleSubCase( dv1.p, outType, false, true, false );
        IncompatibleSubCase( dv1.p, outType, false, true, true );
        IncompatibleSubCase( dv1.p, outType, true, false, false );
        IncompatibleSubCase( dv1.p, outType, true, false, true );
        IncompatibleSubCase( dv1.p, outType, true, true, false );
        IncompatibleSubCase( dv1.p, outType, true, true, true );
    }

    template <class CI, class TI> static void TruncateCase ( int valCount, TI* vals, FdoString** expected, FdoDataType outType )
    {
        int i = 0;

        for ( i = 0; i < valCount; i++ ) 
        {
            FdoPtr<CI> dv1 = CI::Create( vals[i] );
            TruncateSubCase( dv1.p, expected[i], outType, false, false, false );
            TruncateSubCase( dv1.p, expected[i], outType, false, false, true );
            TruncateSubCase( dv1.p, expected[i], outType, false, true, false );
            TruncateSubCase( dv1.p, expected[i], outType, false, true, true );
            TruncateSubCase( dv1.p, expected[i], outType, true, false, false );
            TruncateSubCase( dv1.p, expected[i], outType, true, false, true );
            TruncateSubCase( dv1.p, expected[i], outType, true, true, false );
            TruncateSubCase( dv1.p, expected[i], outType, true, true, true );
        }
    }

    template <class CI, class TI> static void ShiftCase ( int valCount, TI* vals, FdoString** expected, FdoDataType outType )
    {
        int i = 0;

        for ( i = 0; i < valCount; i++ ) 
        {
            FdoPtr<CI> dv1 = CI::Create( vals[i] );
            ShiftSubCase( dv1.p, expected[i], outType, false, false, false );
            ShiftSubCase( dv1.p, expected[i], outType, false, false, true );
            ShiftSubCase( dv1.p, expected[i], outType, false, true, false );
            ShiftSubCase( dv1.p, expected[i], outType, false, true, true );
            ShiftSubCase( dv1.p, expected[i], outType, true, false, false );
            ShiftSubCase( dv1.p, expected[i], outType, true, false, true );
            ShiftSubCase( dv1.p, expected[i], outType, true, true, false );
            ShiftSubCase( dv1.p, expected[i], outType, true, true, true );
        }
    }

    static void NormalSubCase( FdoDataValue* val, FdoString* expected, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate );
    static void NullSubSubCase( FdoDataValue* val, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate );
    static void IncompatibleSubCase( FdoDataValue* val, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate );
    static void TruncateSubCase( FdoDataValue* val, FdoString* expected, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate );
    static void ShiftSubCase( FdoDataValue* val, FdoString* expected, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate );
};

#endif


