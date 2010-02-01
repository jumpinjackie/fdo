Summary: FDO PostgreSQL Provider
Name: fdo-postgresql
Version: 3.5.0.0
Release: 1
Copyright: Autodesk
Group: Development/Libraries
BuildRoot: /tmp/FdoPostgreSQL
Autoreqprov: no

%undefine __check_files
%description
PostgreSQL FDO provider

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/lib
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/lib/com
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/nls
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/docs/XmlSchema/XmlSchema
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/include/Utilities/SchemaMgr/Overrides

install -m 555 $BUILDROOT/Providers/GenericRdbms/Src/.libs/libFdoPostgreSQL-3.5.0.so $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/lib
ln -s libFdoPostgreSQL-3.5.0.so $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/lib/libFdoPostgreSQL.so
install -m 555 $BUILDROOT/Providers/GenericRdbms/Src/.libs/libFdoPostgreSQL.la $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/lib
install -m 555 $BUILDROOT/Providers/GenericRdbms/Src/.libs/libSchemaMgr_OV-3.5.0.so $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/lib
ln -s libSchemaMgr_OV-3.5.0.so $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/lib/libSchemaMgr_OV.so

pushd .
cd $BUILDROOT/Providers/GenericRdbms/Inc/Rdbms/Override
find . -name '*.h' -exec install -D -m 444 {} $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/include/Rdbms/Override/{} \;

rm -rf $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/include/Rdbms/Override/Oracle
rm -rf $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/include/Rdbms/Override/SqlServer
popd

# Install nls message catalog (.CAT file):
install -m 444 $BUILDROOT/Providers/GenericRdbms/Nls/nls/linux/en_US/*.cat $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/nls
install -m 444 $FDOUTILITIES/SchemaMgr/Nls/Nls/linux/en_US/*.cat $RPM_BUILD_ROOT/usr/local/fdo-3.5.0/nls

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
/usr/local/fdo-3.5.0/lib/*.so 
/usr/local/fdo-3.5.0/include
/usr/local/fdo-3.5.0/nls/*
/usr/local/fdo-3.5.0/docs/*

