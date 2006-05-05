Summary: FDO SDF Provider
Name: SDFProvider
Version: 3.2.0.0
Release: 1
Copyright: Autodesk
Group: Development/Libraries 
BuildRoot: /tmp/SDF

%undefine __check_files
%description
FDO SDF provider

%install

# Clean and prep directories:
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.2.0
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/lib
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/nls
mkdir -p $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/include/SDF

# Install shared libraries:
install -m 555 $BUILDROOT/Providers/SDF/Src/Provider/linux/libSDFProvider.so $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/lib

# Install nls message catalog (.CAT file):
install -m 444 $BUILDROOT/Providers/SDF/Src/Message/nls/linux/en_US/* $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/nls

# Install header files
install -m 444 $BUILDROOT/Providers/SDF/Inc/SDF/* $RPM_BUILD_ROOT/usr/local/fdo-3.2.0/include/SDF

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
/usr/local/fdo-3.2.0/lib/libSDFProvider.so 
/usr/local/fdo-3.2.0/nls/*
/usr/local/fdo-3.2.0/include/*

