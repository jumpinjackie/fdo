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


class ExpressionEngineInitializeClass
{

public:
    FdoPtr<FdoExpressionEngineFunctionCollection> m_StandardFunctions;
    FdoPtr<FdoExpressionEngineFunctionCollection> m_UserDefinedFunctions;

	ExpressionEngineInitializeClass()
	{
        m_UserDefinedFunctions = FdoExpressionEngineFunctionCollection::Create();
        m_StandardFunctions = FdoExpressionEngineFunctionCollection::Create();

        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionAvg::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionCount::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionMax::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionMedian::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionMin::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionSpatialExtents::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionStddev::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionSum::Create()));

        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionNullValue::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionToDate::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionToDouble::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionToFloat::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionToInt32::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionToInt64::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionToString::Create()));

        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionAddMonths::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionCurrentDate::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionExtract::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionMonthsBetween::Create()));

        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionArea2D::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionLength2D::Create()));

        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionAbs::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionAcos::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionAsin::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionAtan::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionAtan2::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionCos::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionExp::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionLn::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionLog::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionMod::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionPower::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionRemainder::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionSin::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionSqrt::Create()));
        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionTan::Create()));

        m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionCeil::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionFloor::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionRound::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionSign::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionTrunc::Create()));

		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionConcat::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionInstr::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionLength::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionLower::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionLpad::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionLtrim::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionRpad::Create()));

		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionRtrim::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionSoundex::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionSubstr::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionTranslate::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionTrim::Create()));
		m_StandardFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionUpper::Create()));
    }

	~ExpressionEngineInitializeClass()
	{
	};

	FdoExpressionEngineFunctionCollection* GetStandardFunctions()
	{
		return FDO_SAFE_ADDREF(m_StandardFunctions.p);
	}
};



