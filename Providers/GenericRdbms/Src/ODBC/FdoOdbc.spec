Summary: FDO ODBC Provider
Name: FdoOdbc
Version: 3.2.0.0
Release: 1
Copyright: Autodesk
Group: Development/Libraries 
BuildRoot: /tmp/FdoOdbc
Autoreqprov: no

%undefine __check_files
%description
OSGeo FDO Provider for ODBC


%install

rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/lib
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/lib/com
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/nls

install -m 555 $BUILDROOT/Providers/GenericRdbms/Lib/Linux/libODBCProvider.so $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/lib
install -m 555 $BUILDROOT/Providers/GenericRdbms/Lib/Linux/libSchemaMgr_OV.so $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/lib
install -m 444 $BUILDROOT/Providers/GenericRdbms/com/fdo_sys.sql $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/lib/com
install -m 444 $BUILDROOT/Providers/GenericRdbms/com/fdo_sys_idx.sql $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/lib/com

pushd .
cd $BUILDROOT/Providers/GenericRdbms/Inc/Rdbms/Override
find . -name '*.h' -exec install -D -m 444 {} $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/include/Rdbms/Override/{} \;

rm -rf $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/include/Rdbms/Override/MySQL
rm -rf $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/include/Rdbms/Override/Oracle
rm -rf $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/include/Rdbms/Override/SqlServer
popd

# Install nls message catalog (.CAT file):
install -m 444 $BUILDROOT/Providers/GenericRdbms/Nls/nls/linux/en_US/*.cat $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/nls
install -m 444 $FDOUTILITIES/SchemaMgr/Nls/Nls/linux/en_US/*.cat $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/nls


%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
/usr/local/fdo-3.2.0/lib/libODBCProvider.so 
/usr/local/fdo-3.2.0/lib/libSchemaMgr_OV.so 
/usr/local/fdo-3.2.0/lib/com/fdo_sys.sql
/usr/local/fdo-3.2.0/lib/com/fdo_sys_idx.sql
/usr/local/fdo-3.2.0/include
/usr/local/fdo-3.2.0/nls/*

