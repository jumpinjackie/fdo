/*
 * Copyright (C) 2004-2006  SL-King, Inc.
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
* License along with this library; 
 */

#ifndef FDOKGORAPROVIDER_H
#define FDOKGORAPROVIDER_H


#ifdef _WIN32

#pragma once


#ifdef FDOKGORA_EXPORTS
#define FDOKGORA_API __declspec(dllexport)
#else
#define FDOKGORA_API __declspec(dllimport)
#endif


#else //_WIN32

#define FDOKGORA_API

#endif //_WIN32

#endif // FDOKGORAPROVIDER_H


