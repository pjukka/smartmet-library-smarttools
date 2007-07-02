Summary: smarttools library
Name: smarttools
Version: 1.0
Release: 1
License: FMI
Group: Development/Libraries
URL: http://www.weatherproof.fi
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}
#Requires: libstdc++.so.6

%description
FMI smarttools library

%prep
rm -rf $RPM_BUILD_ROOT
mkdir $RPM_BUILD_ROOT

%setup -q -n %{name}
 
%build
make %{_smp_mflags} 

%install
make install prefix="${RPM_BUILD_ROOT}/data/local"
mkdir -p ${RPM_BUILD_ROOT}/data/local/src/c++/lib/smarttools
cp -r . ${RPM_BUILD_ROOT}/data/local/src/c++/lib/smarttools

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,www,www,0775)
/data/local/include/smarttools
/data/local/lib/libsmarttools.a
/data/local/src/c++/lib/smarttools


%changelog
* Thu Jun  7 2007 tervo <tervo@xodin.weatherproof.fi> - 
- Initial build.

