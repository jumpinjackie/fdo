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

#ifndef CPP_UNIT_DATATYPETESTS_H
#define CPP_UNIT_DATATYPETESTS_H

#include <Inc/Rdbi/proto.h>

#define DB_SUFFIX "_rdbitest"

class DataTypeTests :
    public CppUnit::TestCase
{
protected:
    rdbi_context_def* mRdbiContext;
    int mId;

private:
    CPPUNIT_TEST_SUITE (DataTypeTests);
	CPPUNIT_TEST (geometry);
    CPPUNIT_TEST (single_char);
    CPPUNIT_TEST (single_character);
    CPPUNIT_TEST (dozen_char);
    CPPUNIT_TEST (dozen_character);
    CPPUNIT_TEST (char_varying);
    CPPUNIT_TEST (character_varying);
    CPPUNIT_TEST (varchar);
    CPPUNIT_TEST (numeric);
    CPPUNIT_TEST (numeric_2_0);
    CPPUNIT_TEST (numeric_6_0);
    CPPUNIT_TEST (numeric_6_2);
    CPPUNIT_TEST (decimal);
    CPPUNIT_TEST (decimal_2_0);
    CPPUNIT_TEST (decimal_6_0);
    CPPUNIT_TEST (decimal_6_2);
    CPPUNIT_TEST (dec);
    CPPUNIT_TEST (dec_2_0);
    CPPUNIT_TEST (dec_6_0);
    CPPUNIT_TEST (dec_6_2);
    CPPUNIT_TEST (integer);
    CPPUNIT_TEST (smallint);
    CPPUNIT_TEST (floating);
    CPPUNIT_TEST (floating_15);
    CPPUNIT_TEST (real);
    CPPUNIT_TEST (double_precision);
    CPPUNIT_TEST (date);
    CPPUNIT_TEST (time);
    CPPUNIT_TEST (timestamp);
    CPPUNIT_TEST (longlong);
    CPPUNIT_TEST_SUITE_END ();

public:
    DataTypeTests (void);
    virtual ~DataTypeTests (void);
	void setUp ();
	void tearDown ();

    void roundtrip_insert (
        char *sql_type,
        int rdbi_type, int rdbi_scale, int rdbi_precision,
        void *variable, int size, double tolerance = 0.0,
        bool is_ora_int64_test = false);

	   void roundtrip_update (
        char *sql_type,
        int rdbi_type, int rdbi_scale, int rdbi_precision,
        void *variable, int size, double tolerance = 0.0,
        bool is_ora_int64_test = false);

protected:
    virtual void set_provider() {  CPPUNIT_FAIL ("naked set_provider() called"); };
	void geometry();
    void single_char ();
    void single_character ();
    void dozen_char ();
    void dozen_character ();
    void char_varying ();
    void character_varying ();
    void varchar ();
    void numeric ();
    void numeric_2_0 ();
    void numeric_6_0 ();
    void numeric_6_2 ();
    void decimal ();
    void decimal_2_0 ();
    void decimal_6_0 ();
    void decimal_6_2 ();
    void dec ();
    void dec_2_0 ();
    void dec_6_0 ();
    void dec_6_2 ();
    void integer ();
    void smallint ();
    void floating ();
    void floating_15 ();
    void real ();
    void double_precision ();
    void date ();
    void time ();
    void timestamp ();
    void longlong ();

    virtual int do_rdbi_init ();
    virtual int do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword);
    virtual char *get_bind_var (int n);
	virtual char *get_geometry_type() {return ""; };
	virtual char *get_geometry_storage() { return ""; }
    virtual char *get_date_time (const struct tm *when);
    virtual char *get_datetime_type() { return "DATE"; };
    virtual bool allow_timestamp_notnull() { return true; };
    virtual bool is_datatype_equal(int rdbi_type, int type) { return (rdbi_type == type); };
    virtual char *get_bigint_data_type () { return "BIGINT"; };

};

#endif // CPP_UNIT_DATATYPETESTS_H
