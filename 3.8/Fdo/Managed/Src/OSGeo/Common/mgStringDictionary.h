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

//#pragma once
//
//BEGIN_NAMESPACE_COMMON
//
//public __sealed __gc class StringDictionary : public DictionaryBase
//{
//public:
//	__property String* get_Item( String* key )
//	{
//		return __try_cast<String*>(Dictionary->Item[key]);
//	}
//
//	__property void set_Item( String* key, String* value )
//	{
//		Dictionary->Item[key] = value;
//	}
//
//	__property ICollection* get_Keys()
//	{
//		return(Dictionary->Keys);
//	}
//
//	__property ICollection* get_Values()
//	{
//		return(Dictionary->Values);
//	}
//
//	void Add(String* key, String* value) {
//		Dictionary->Add(key, value);
//	}
//
//	bool Contains(String* key) {
//		return(Dictionary->Contains(key));
//	}
//
//	void Remove(String* key) {
//		Dictionary->Remove(key);
//	}
//
//protected:
//	void OnInsert(Object* key, Object* value) 
//	{
//		if (key->GetType() != Type::GetType(S"System.String"))
//			throw new ArgumentException(S"key must be of type String.", S"key");
//
//		if (value->GetType() != Type::GetType(S"System.String"))
//			throw new ArgumentException(S"value must be of type String.", S"value");
//	}
//
//	void OnRemove(Object* key, Object* value) 
//	{
//		if (key->GetType() != Type::GetType(S"System.String"))
//			throw new ArgumentException(S"key must be of type String.", S"key");
//	}
//
//	void OnSet(Object* key, Object* oldValue, Object* newValue) 
//	{
//		if (key->GetType() != Type::GetType(S"System.String"))
//			throw new ArgumentException(S"key must be of type String.", S"key");
//
//		if (newValue->GetType() != Type::GetType(S"System.String"))
//			throw new ArgumentException(S"newValue must be of type String.", S"newValue");
//	}
//
//	void OnValidate(Object* key, Object* value) 
//	{
//		if (key->GetType() != Type::GetType(S"System.String"))
//			throw new ArgumentException(S"key must be of type String.", S"key");
//
//		if (value->GetType() != Type::GetType(S"System.String"))
//			throw new ArgumentException(S"value must be of type String.", S"value");
//	}
//
//};
//
//END_NAMESPACE_COMMON


