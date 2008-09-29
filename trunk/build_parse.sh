#!/bin/sh

##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

echo Parsing Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y
yacc -dv Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.y
sed -f Fdo/Unmanaged/Src/Fdo/Parse/script_linux < y.tab.c >Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.cpp
rm y.tab.c
sed -e '/^# *line/s/\\/\\\\/g' -e '/^# *line/b' -e 's/yy/fdo_expression_yy/g' -e 's/YY/FDO_EXPRESSION_YY/g' -e '/extern /D' < y.tab.h >Fdo/Unmanaged/Src/Fdo/Parse/yyExpression.h
rm y.tab.h
rm y.output

echo Parsing Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y
yacc -dv Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.y
sed -f Fdo/Unmanaged/Src/Fdo/Parse/script2_linux < y.tab.c >Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.cpp
rm y.tab.c
sed -e '/^# *line/s/\\/\\\\/g' -e '/^# *line/b' -e 's/yy/fdo_filter_yy/g' -e 's/YY/FDO_FILTER_YY/g' -e '/extern /D' < y.tab.h >Fdo/Unmanaged/Src/Fdo/Parse/yyFilter.h
rm  y.tab.h
rm  y.output

echo Parsing Fdo/Unmanaged/Src/Geometry/Parse/yyFgft.y
yacc -d Fdo/Unmanaged/Src/Geometry/Parse/yyFgft.y
sed -f Fdo/Unmanaged/Src/Geometry/Parse/script_linux < y.tab.c >Fdo/Unmanaged/Src/Geometry/Parse/yyFgft.cpp
rm y.tab.c
sed -e '/^# *line/s/\\/\\\\/g' -e '/^# *line/b' -e 's/yy/fdo_fgft_yy/g' -e 's/YY/FDO_FGFT_YY/g' -e '/extern /D' < y.tab.h > Fdo/Unmanaged/Src/Geometry/Parse/yyFgft.h
rm y.tab.h

echo Parsing Utilities/Common/Src/Parse/yyConstraint.y
yacc -d Utilities/Common/Src/Parse/yyConstraint.y
sed -f Utilities/Common/Src/Parse/script3_linux < y.tab.c >Utilities/Common/Src/Parse/yyConstraint.cpp
rm y.tab.c
sed -e '/^# *line/s/\\/\\\\/g' -e '/^# *line/b' -e 's/yy/fdo_constraint_yy/g' -e 's/YY/FDO_CONSTRAINT_YY/g' -e '/extern /D' < y.tab.h > Utilities/Common/Src/Parse/yyConstraint.h
rm y.tab.h

exit 0

