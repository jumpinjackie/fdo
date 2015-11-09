/*
 * 
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
  *
 */

#include <stdio.h>

const char *nls_ftoa(double number, int width, int precision)
{
	static char buffer[10][32];
	static int current = 0;
	char temp[32];
	char *ptr;

	if (width > 0) {
		if (precision > 0)
		{
			sprintf(temp, "%%%d.%df", width, precision);
		}
		else {
			sprintf(temp, "%%%df", width);

		}
	} else {
		if (precision > 0)
		{
			sprintf(temp, "%%.%df", precision);
		}
		else {
			sprintf(temp, "%%f");
		}
	}
	ptr = buffer[current];
	sprintf(ptr, temp, number);
	current++;
	current %= 10;
	return ptr;
}

