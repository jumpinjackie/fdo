/*
 * 
* Copyright (C) 2004-2007  Autodesk, Inc.
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

#include <stdafx.h>

#include <FdoExpressionEngineIFunction.h>

#include <Functions/Aggregate/FdoFunctionAvg.h>
#include <Functions/Aggregate/FdoFunctionCount.h>
#include <Functions/Aggregate/FdoFunctionMax.h>
#include <Functions/Aggregate/FdoFunctionMedian.h>
#include <Functions/Aggregate/FdoFunctionMin.h>
#include <Functions/Aggregate/FdoFunctionSpatialExtents.h>
#include <Functions/Aggregate/FdoFunctionStddev.h>
#include <Functions/Aggregate/FdoFunctionSum.h>

#include <Functions/Conversion/FdoFunctionNullValue.h>
#include <Functions/Conversion/FdoFunctionToDate.h>
#include <Functions/Conversion/FdoFunctionToDouble.h>
#include <Functions/Conversion/FdoFunctionToFloat.h>
#include <Functions/Conversion/FdoFunctionToInt32.h>
#include <Functions/Conversion/FdoFunctionToInt64.h>
#include <Functions/Conversion/FdoFunctionToString.h>

#include <Functions/Date/FdoFunctionAddMonths.h>
#include <Functions/Date/FdoFunctionCurrentDate.h>
#include <Functions/Date/FdoFunctionExtract.h>
#include <Functions/Date/FdoFunctionMonthsBetween.h>

#include <Functions/Geometry/FdoFunctionArea2D.h>
#include <Functions/Geometry/FdoFunctionLength2D.h>

#include <Functions/Math/FdoFunctionAbs.h>
#include <Functions/Math/FdoFunctionAcos.h>
#include <Functions/Math/FdoFunctionAsin.h>
#include <Functions/Math/FdoFunctionAtan.h>
#include <Functions/Math/FdoFunctionAtan2.h>
#include <Functions/Math/FdoFunctionCos.h>
#include <Functions/Math/FdoFunctionExp.h>
#include <Functions/Math/FdoFunctionLn.h>
#include <Functions/Math/FdoFunctionLog.h>
#include <Functions/Math/FdoFunctionMod.h>
#include <Functions/Math/FdoFunctionPower.h>
#include <Functions/Math/FdoFunctionRemainder.h>
#include <Functions/Math/FdoFunctionSin.h>
#include <Functions/Math/FdoFunctionSqrt.h>
#include <Functions/Math/FdoFunctionTan.h>

#include <Functions/Numeric/FdoFunctionCeil.h>
#include <Functions/Numeric/FdoFunctionFloor.h>
#include <Functions/Numeric/FdoFunctionRound.h>
#include <Functions/Numeric/FdoFunctionSign.h>
#include <Functions/Numeric/FdoFunctionTrunc.h>

#include <Functions/String/FdoFunctionConcat.h>
#include <Functions/String/FdoFunctionInstr.h>
#include <Functions/String/FdoFunctionLength.h>
#include <Functions/String/FdoFunctionLower.h>
#include <Functions/String/FdoFunctionLpad.h>
#include <Functions/String/FdoFunctionLtrim.h>
#include <Functions/String/FdoFunctionRpad.h>
#include <Functions/String/FdoFunctionRtrim.h>
#include <Functions/String/FdoFunctionSoundex.h>
#include <Functions/String/FdoFunctionSubstr.h>
#include <Functions/String/FdoFunctionTranslate.h>
#include <Functions/String/FdoFunctionTrim.h>
#include <Functions/String/FdoFunctionUpper.h>


const int NUMBER_FUNCTIONS = 54;

class ExpressionEngineInitializeClass
{

public:
	FdoExpressionEngineIFunction *m_Functions[NUMBER_FUNCTIONS+1];
	ExpressionEngineInitializeClass()
	{
		int i=0;
        m_Functions[i++] = FdoFunctionAvg::Create();
        m_Functions[i++] = FdoFunctionCount::Create();
        m_Functions[i++] = FdoFunctionMax::Create();
        m_Functions[i++] = FdoFunctionMedian::Create();
        m_Functions[i++] = FdoFunctionMin::Create();
        m_Functions[i++] = FdoFunctionSpatialExtents::Create();
        m_Functions[i++] = FdoFunctionStddev::Create();
        m_Functions[i++] = FdoFunctionSum::Create();

        m_Functions[i++] = FdoFunctionNullValue::Create();
        m_Functions[i++] = FdoFunctionToDate::Create();
        m_Functions[i++] = FdoFunctionToDouble::Create();
        m_Functions[i++] = FdoFunctionToFloat::Create();
        m_Functions[i++] = FdoFunctionToInt32::Create();
        m_Functions[i++] = FdoFunctionToInt64::Create();
        m_Functions[i++] = FdoFunctionToString::Create();

        m_Functions[i++] = FdoFunctionAddMonths::Create();
        m_Functions[i++] = FdoFunctionCurrentDate::Create();
        m_Functions[i++] = FdoFunctionExtract::Create();
        m_Functions[i++] = FdoFunctionMonthsBetween::Create();

        m_Functions[i++] = FdoFunctionArea2D::Create();
        m_Functions[i++] = FdoFunctionLength2D::Create();

        m_Functions[i++] = FdoFunctionAbs::Create();
        m_Functions[i++] = FdoFunctionAcos::Create();
        m_Functions[i++] = FdoFunctionAsin::Create();
        m_Functions[i++] = FdoFunctionAtan::Create();
        m_Functions[i++] = FdoFunctionAtan2::Create();
        m_Functions[i++] = FdoFunctionCos::Create();
        m_Functions[i++] = FdoFunctionExp::Create();
        m_Functions[i++] = FdoFunctionLn::Create();
        m_Functions[i++] = FdoFunctionLog::Create();
        m_Functions[i++] = FdoFunctionMod::Create();
        m_Functions[i++] = FdoFunctionPower::Create();
        m_Functions[i++] = FdoFunctionRemainder::Create();
        m_Functions[i++] = FdoFunctionSin::Create();
        m_Functions[i++] = FdoFunctionSqrt::Create();
        m_Functions[i++] = FdoFunctionTan::Create();

        m_Functions[i++] = FdoFunctionCeil::Create();
		m_Functions[i++] = FdoFunctionFloor::Create();
		m_Functions[i++] = FdoFunctionRound::Create();
		m_Functions[i++] = FdoFunctionSign::Create();
		m_Functions[i++] = FdoFunctionTrunc::Create();

		m_Functions[i++] = FdoFunctionConcat::Create();
		m_Functions[i++] = FdoFunctionInstr::Create();
		m_Functions[i++] = FdoFunctionLength::Create();
		m_Functions[i++] = FdoFunctionLower::Create();
		m_Functions[i++] = FdoFunctionLpad::Create();
		m_Functions[i++] = FdoFunctionLtrim::Create();
		m_Functions[i++] = FdoFunctionRpad::Create();
		m_Functions[i++] = FdoFunctionRtrim::Create();
		m_Functions[i++] = FdoFunctionSoundex::Create();
		m_Functions[i++] = FdoFunctionSubstr::Create();
		m_Functions[i++] = FdoFunctionTranslate::Create();
		m_Functions[i++] = FdoFunctionTrim::Create();
		m_Functions[i++] = FdoFunctionUpper::Create();

		m_Functions[i++] = NULL;
	}

	~ExpressionEngineInitializeClass()
	{
		for (int i=0; m_Functions[i] != NULL; i++)
		{
			m_Functions[i]->Release();
		};
	};

	FdoExpressionEngineIFunction** GetStandardFunctions()
	{
		return m_Functions;
	}
};
