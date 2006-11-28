/*
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
 */

#ifndef CPP_UNIT_FDOUPDATETEST_H
#define CPP_UNIT_FDOUPDATETEST_H

#include <Sm/SchemaManager.h>

/*
 * A test case for the connection.
 */
class FdoUpdateTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoUpdateTest );
  CPPUNIT_TEST( Update );
  CPPUNIT_TEST( UpdateAttributesOnly );
  CPPUNIT_TEST( UpdateNonFeatureClass );
  CPPUNIT_TEST(UpdateObjectProperty);
  CPPUNIT_TEST( FdoUpdateTestTypes );
  CPPUNIT_TEST(UpdateMultiIdFeatureClass);
 // CPPUNIT_TEST(UpdateSingleIdFeatureClass);
 // CPPUNIT_TEST( ConditionalUpdate );
  CPPUNIT_TEST( UpdateNoMeta );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoUpdateTest(void);
    FdoUpdateTest(char*);
    virtual ~FdoUpdateTest(void);
    void setUp ();

    virtual void MainFdoUpdateTest (FdoIConnection* Conn=NULL);
    virtual void Update ();
    virtual void UpdateAttributesOnly ();
    virtual void UpdateNonFeatureClass ();
    virtual void FdoUpdateTestTypes ();
    virtual void UpdateObjectProperty();
    virtual void UpdateMultiIdFeatureClass ();
    virtual void UpdateSingleIdFeatureClass ();
	virtual void ConditionalUpdate ();
	virtual void UpdateNoMeta ();
    virtual void DisableFailures(){m_DisableFailures=true;};
protected:
    virtual void  set_provider() {};   

    void CreateExternalTable( FdoSmPhOwnerP owner, FdoStringP tableName, bool hasKey, bool hasGeom, bool hasAssoc );
    void CreateExternalData( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasGeom, bool hasAssoc );
    void SelectNoMetaAll( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasGeom, bool hasAssoc );
    void SelectNoMetaFilter( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasGeom, bool hasAssoc );
    void SelectNoMetaProps( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasGeom );
    void SelectNoMetaSpatial( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName, bool hasAssoc );

    void VldNoMetaRow( 
        FdoSmPhMgrP phMgr, 
        FdoPtr<FdoIFeatureReader> rdr, 
        FdoPtr<FdoIdentifierCollection> props, 
        bool hasGeom, 
        bool propsPruned,
        bool hasAssoc
    );
    void CheckGeometry(FdoPtr<FdoIFeatureReader> rdr, FdoString* propName, double expectedX, double expectedY, double expectedZ);

    virtual const char* NoMetaSuffix();
    virtual const char* MultiIdSuffix();
    virtual FdoStringP TableIdGeomName();
    virtual FdoStringP TableNoIdNoGeomName();
    virtual FdoStringP Key2ColName();
    virtual FdoStringP ValueColName();

protected:
	static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name );

protected:
	bool m_hasGeom;
	bool m_hasAssoc;
    bool m_DisableFailures;

protected:
    void ConditionalUpdateSetup (FdoIConnection *connection, FdoStringP insLtName);
    char  mSuffix[12];
};

#endif // CPP_UNIT_FDOUPDATETEST_H
