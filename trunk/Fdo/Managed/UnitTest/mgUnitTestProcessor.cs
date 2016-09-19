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
using System.Diagnostics;
using OSGeo.FDO.Expression;
using OSGeo.FDO.Filter;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgUnitTestProcessor.
	/// </summary>
	public class mgUnitTestProcessor : IExpressionProcessor, IFilterProcessor
	{
		protected int m_tabLevel;

		protected void prependTabs()
		{
		}
 
		public mgUnitTestProcessor(int tabLevel)
		{
			m_tabLevel = tabLevel;
		}

		public mgUnitTestProcessor()
		{
			m_tabLevel = 0;
		}

		public void Dispose()
		{
			this.Dispose();
		}

		public void ProcessBinaryExpression(BinaryExpression expr)
		{
			m_tabLevel++;
			Expression  pExpr = expr.LeftExpression;
			pExpr.Process(this);
			m_tabLevel--;

			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("BinaryExpression : {0}\n", expr.Operation);
#endif
			m_tabLevel++;
			pExpr = expr.RightExpression;
			pExpr.Process(this);
			m_tabLevel--;
		}

		public void ProcessBooleanValue(BooleanValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("BooleanValue : {0}\n", expr.Boolean ? "TRUE" : "FALSE");
#endif
        }

		public void  ProcessByteValue(ByteValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("ByteValue : {0}\n", (int)expr.Byte);
#endif
		}

		public void ProcessDateTimeValue(DateTimeValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("DateTimeValue : {0}\n", expr.DateTime);
#endif
		}

		public void ProcessDecimalValue(DecimalValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
            Console.WriteLine("DecimalValue : {0}\n", expr.Decimal);
#endif
        }

		public void ProcessDoubleValue(DoubleValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("DoubleValue : {0}\n", expr.Double);
#endif
        }

		public void ProcessInt16Value(Int16Value expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("Int16Value : {0}\n", (Int16)expr.Int16);
#endif
		}

		public void ProcessInt32Value(Int32Value expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("Int32Value : {0}\n", expr.Int32);
#endif
		}

		public void ProcessInt64Value(Int64Value expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("Int64Value :  {0}\n", expr.Int64);
#endif
		}

		public void ProcessSingleValue(SingleValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("SingleValue :  {0}\n", (double)expr.Single);
#endif
		}

		public void ProcessStringValue(StringValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("StringValue :  {0}\n", expr.String);
#endif
		}

		public void ProcessBLOBValue(BLOBValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("BLOBValue :\n");
#endif
		}

		public void ProcessCLOBValue(CLOBValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("CLOBValue :\n");
#endif
		}

		public void ProcessFunction(Function expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("Function : {0}\n", expr.Name);
#endif
			ExpressionCollection pColl = expr.Arguments;
			m_tabLevel++;
			for (int i = 0; i < pColl.Count; i++)
			{
				Expression   pExpr;

				pExpr = pColl[i];
				pExpr.Process(this);
			}
			m_tabLevel--;
		}

		public void ProcessGeometryValue(GeometryValue expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("GeometryValue : {0}\n", expr.IsNull() ? "NULL" : "not NULL");
#endif
		}

		public void ProcessIdentifier(Identifier expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("Identifier : {0}\n", expr.Text);
#endif
		}

		public void ProcessComputedIdentifier(ComputedIdentifier expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("Computed Identifier Alias : {0}\n", expr.Text);
#endif
			mgUnitTestProcessor    expProc = new mgUnitTestProcessor();
			Expression exp = expr.Expression;
            if (exp != null)
            {
#if DUMPTESTCONTENT
                Console.WriteLine("Computed Identifier Expression: \n");
#endif
                exp.Process(expProc);
            }
            else
#if DUMPTESTCONTENT
				Console.WriteLine("NULL expression\n");
#else
            ;
#endif
		}

		public void ProcessParameter(Parameter expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("Parameter : {0}\n", expr.Name);
#endif
		}

		public void ProcessUnaryExpression(UnaryExpression expr)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("UnaryExpression : \n");
#endif

			m_tabLevel++;
			Expression  pExpr = expr.Expressions;
			pExpr.Process(this);
			m_tabLevel--;
		}

		public void ProcessBinaryLogicalOperator(BinaryLogicalOperator filter)
		{
			m_tabLevel++;
			Filter  pFilter = filter.LeftOperand;
			pFilter.Process(this);
			m_tabLevel--;

			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("BinaryLogicalOperator : {0}\n", filter.Operation);
#endif

			m_tabLevel++;
			pFilter = filter.RightOperand;
			pFilter.Process(this);
			m_tabLevel--;
		}

		public void ProcessComparisonCondition(ComparisonCondition filter)
		{
			m_tabLevel++;
			Expression  pExpr = filter.LeftExpression;
			pExpr.Process(this);
			m_tabLevel--;

			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("ComparisonCondition : {0}\n", filter.Operation);
#endif

			m_tabLevel++;
			pExpr = filter.RightExpression;
			pExpr.Process(this);
			m_tabLevel--;
		}

		public void ProcessDistanceCondition(DistanceCondition filter)
		{
			prependTabs();
#if DUMPTESTCONTENT
			Console.WriteLine("DistanceCondition : {0} - {1}\n", filter.Operation, filter.Distance);
#endif

			m_tabLevel++;
			Expression  pExpr = filter.Geometry;
			pExpr.Process(this);
			m_tabLevel--;
		}

		public void ProcessInCondition(InCondition filter)
		{
			prependTabs();
			Identifier  pIdent = filter.PropertyName;
#if DUMPTESTCONTENT
			Console.WriteLine("InCondition : {0}\n", pIdent.Text);
#endif

			m_tabLevel++;
			ValueExpressionCollection pColl = filter.Values;
			for (int i = 0; i < pColl.Count; i++)
			{
				ValueExpression pExpr;

				pExpr = pColl[i];
				pExpr.Process(this);
			}
			m_tabLevel--;
		}

		public void ProcessNullCondition(NullCondition filter)
		{
			prependTabs();
			Identifier pIdent = filter.PropertyName;
#if DUMPTESTCONTENT
			Console.WriteLine("NullCondition : {0}\n", pIdent.Text);
#endif
        }

		public void ProcessSpatialCondition(SpatialCondition filter)
		{
			prependTabs();
			Identifier pIdent = filter.PropertyName;
#if DUMPTESTCONTENT
			Console.WriteLine("SpatialCondition : {0}- {1}\n", filter.Operation, pIdent.Text);
#endif

			m_tabLevel++;
			Expression pExpr = filter.Geometry;
			pExpr.Process(this);
			m_tabLevel--;
		}

		public void ProcessUnaryLogicalOperator(UnaryLogicalOperator filter)
	{
		prependTabs();
#if DUMPTESTCONTENT
		Console.WriteLine("UnaryLogicalOperator : {0}\n", filter.Operand);
#endif

		m_tabLevel++;
		Filter  pFilter = filter.Operand;
		pFilter.Process(this);
		m_tabLevel--;
	}

//		private void prependTabs()
//		{
////			for (int i = 0; i < m_tabLevel; i++)
////			{
////				_putwch(L' ');
////			}
//		}
	}
}



