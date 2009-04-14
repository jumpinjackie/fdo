@echo off

rem 
rem Copyright (C) 2004-2006  Autodesk, Inc.
rem 
rem This library is free software; you can redistribute it and/or
rem modify it under the terms of version 2.1 of the GNU Lesser
rem General Public License as published by the Free Software Foundation.
rem 
rem This library is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
rem Lesser General Public License for more details.
rem 
rem You should have received a copy of the GNU Lesser General Public
rem License along with this library; if not, write to the Free Software
rem Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
rem 

echo bison yyExpression.y
bison -y -ldv Fdo\Unmanaged\Src\Fdo\Parse\yyExpression.y
sed -f Fdo\Unmanaged\Src\Fdo\Parse\script < y.tab.c >Fdo\Unmanaged\Src\Fdo\Parse\yyExpressionWin.cpp
del y.tab.c
sed -f Fdo\Unmanaged\Src\Fdo\Parse\script_h < y.tab.h >Fdo\Unmanaged\Src\Fdo\Parse\yyExpressionWin.h
del y.tab.h
del y.output

echo bison yyFilter.y
bison -y -ldv Fdo\Unmanaged\Src\Fdo\Parse\yyFilter.y
sed -f Fdo\Unmanaged\Src\Fdo\Parse\script2< y.tab.c >Fdo\Unmanaged\Src\Fdo\Parse\yyFilterWin.cpp
del y.tab.c
sed -f Fdo\Unmanaged\Src\Fdo\Parse\script2_h < y.tab.h >Fdo\Unmanaged\Src\Fdo\Parse\yyFilterWin.h
del y.tab.h
del y.output

echo bison yyFgft.y
bison -y -ldv Fdo\Unmanaged\Src\Geometry\Parse\yyFgft.y
sed -f Fdo\Unmanaged\Src\Geometry\Parse\script< y.tab.c >Fdo\Unmanaged\Src\Geometry\Parse\yyFgftWin.cpp
del y.tab.c
sed -f Fdo\Unmanaged\Src\Geometry\Parse\script_h < y.tab.h >Fdo\Unmanaged\Src\Geometry\Parse\yyFgftWin.h
del y.tab.h
del y.output

echo bison yyConstraint.y
bison -y -ldv Utilities\Common\Src\Parse\yyConstraint.y
sed -f Utilities\Common\Src\Parse\script3 < y.tab.c >Utilities\Common\Src\Parse\yyConstraintWin.cpp
del y.tab.c
sed -f Utilities\Common\Src\Parse\script3_h < y.tab.h >Utilities\Common\Inc\Parse\yyConstraintWin.h
del y.tab.h
del y.output

:end
exit /B 0
