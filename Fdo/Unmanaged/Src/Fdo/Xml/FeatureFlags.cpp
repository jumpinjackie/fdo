// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <Fdo.h>
#include <Fdo/Xml/FeatureFlags.h>

FdoXmlFeatureFlags::FdoXmlFeatureFlags()
    : FdoXmlFlags(),
      mConflictOption(ConflictOption_Add),
      mWriteCollection(true),
      mWriteMember(true),
      mCollectionUri(FdoXml::mGmlUri),
      mCollectionName(FdoXml::mFeatureCollectionName),
      mMemberUri(FdoXml::mGmlUri),
      mMemberName(FdoXml::mFeatureMemberName)
{
    mNamespaces = FdoStringCollection::Create();
    mSchemaLocations = FdoStringCollection::Create();
}

FdoXmlFeatureFlags::FdoXmlFeatureFlags(FdoString* url, ErrorLevel errorLevel, FdoBoolean nameAdjust, ConflictOption conflictOption)
    : FdoXmlFlags(url, errorLevel, nameAdjust),
      mConflictOption(conflictOption),
      mWriteCollection(true),
      mWriteMember(true),
      mCollectionUri(FdoXml::mGmlUri),
      mCollectionName(FdoXml::mFeatureCollectionName),
      mMemberUri(FdoXml::mGmlUri),
      mMemberName(FdoXml::mFeatureMemberName)
{
    mNamespaces = FdoStringCollection::Create();
    mSchemaLocations = FdoStringCollection::Create();
}

FdoXmlFeatureFlags::~FdoXmlFeatureFlags()
{
}

FdoXmlFeatureFlags* FdoXmlFeatureFlags::Create(FdoString* url, ErrorLevel errorLevel, FdoBoolean nameAdjust, ConflictOption conflictOption) 
{
	return new FdoXmlFeatureFlags(url, errorLevel, nameAdjust, conflictOption);
}

void FdoXmlFeatureFlags::SetConflictOption(ConflictOption conflictOption )
{
    mConflictOption = conflictOption;
}

FdoXmlFeatureFlags::ConflictOption FdoXmlFeatureFlags::GetConflictOption() const
{
	return mConflictOption;
}

void FdoXmlFeatureFlags::SetWriteCollection( FdoBoolean writeCollection )
{
    mWriteCollection = writeCollection;
}

FdoBoolean FdoXmlFeatureFlags::GetWriteCollection() const
{
	return mWriteCollection;
}

void FdoXmlFeatureFlags::SetWriteMember( FdoBoolean writeMember )
{
    mWriteMember = writeMember;
}

FdoBoolean FdoXmlFeatureFlags::GetWriteMember() const
{
	return mWriteMember;
}

void FdoXmlFeatureFlags::SetCollectionUri( FdoString* uri )
{
    mCollectionUri = uri;
}

FdoString* FdoXmlFeatureFlags::GetCollectionUri() const
{
	return mCollectionUri;
}

void FdoXmlFeatureFlags::SetCollectionName( FdoString* name )
{
    mCollectionName = name;
}

FdoString* FdoXmlFeatureFlags::GetCollectionName() const
{
	return mCollectionName;
}

void FdoXmlFeatureFlags::SetMemberUri( FdoString* uri )
{
    mMemberUri = uri;
}

FdoString* FdoXmlFeatureFlags::GetMemberUri()const
{
	return mMemberUri;
}

void FdoXmlFeatureFlags::SetMemberName( FdoString* name )
{
    mMemberName = name;
}

FdoString* FdoXmlFeatureFlags::GetMemberName()const
{
	return mMemberName;
}

void FdoXmlFeatureFlags::SetGmlIdPrefix( FdoString* prefix )
{
    mGmlIdPrefix = prefix;
}

FdoString* FdoXmlFeatureFlags::GetGmlIdPrefix()const
{
	return mGmlIdPrefix;
}

void FdoXmlFeatureFlags::SetSchemaLocation(FdoString* nameSpace, FdoString* schemaLocation) {
    FdoInt32 count = mNamespaces->GetCount();
    int i;
    for (i = 0; i < count; i++) {
        if (wcscmp(nameSpace, mNamespaces->GetString(i)) == 0)
            break;
    }

    if (i < count) { // found 
        mNamespaces->RemoveAt(i);
        mSchemaLocations->RemoveAt(i);
    }
    mNamespaces->Add(nameSpace);
    mSchemaLocations->Add(schemaLocation);
}

FdoString* FdoXmlFeatureFlags::GetSchemaLocation(FdoString* nameSpace) {
    FdoInt32 count = mNamespaces->GetCount();
    int i;
    for (i = 0; i < count; i++) {
        if (wcscmp(nameSpace, mNamespaces->GetString(i)) == 0)
            break;
    }
    FdoString* rv = NULL;
    if (i < count)
        rv = mSchemaLocations->GetString(i);
    return rv;
}

FdoStringCollection* FdoXmlFeatureFlags::GetNamespaces() {
    return FDO_SAFE_ADDREF(mNamespaces.p);
}

void FdoXmlFeatureFlags::SetDefaultNamespace(FdoString* defaultNamespace) {
    mDefaultNamespace = defaultNamespace;
}

FdoString* FdoXmlFeatureFlags::GetDefaultNamespace() {
    return mDefaultNamespace;
}

