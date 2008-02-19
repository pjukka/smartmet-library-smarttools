%define LIBNAME smarttools
Summary: smarttools library
Name: libsmartmet-%{LIBNAME}
Version: 8.2.18
Release: 1.el5.fmi
License: FMI
Group: Development/Libraries
URL: http://www.weatherproof.fi
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot-%(%{__id_u} -n)
BuildRequires: libsmartmet-newbase >= 8.2.18-1
Provides: %{LIBNAME}

%description
FMI smarttools library

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q -n %{LIBNAME}
 
%build
make %{_smp_mflags}

%install
%makeinstall includedir=%{buildroot}%{_includedir}/smartmet

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,0775)
%{_includedir}/smartmet/%{LIBNAME}
%{_libdir}/libsmartmet_%{LIBNAME}.a

%changelog
* Mon Feb 18 2008 pkeranen <pekka.keranen@fmi.fi> - 8.2.18-1.el5.fmi
- Linked against newbase 8.2.18-1 with new WeatherAndCloudinessParam
* Wed Jan 30 2008 mheiskan <mika.heiskanen@fmi.fi> - 8.1.30-1.el5.fmi
- Linked against newbase 8.1.25-3 with new parameter names
* Thu Dec 27 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.9-1.el5.fmi
- Linked against newbase-1.0.7-1 with fixed wind interpolation
* Wed Dec 19 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.8-1.el5.fmi
- Linked against newbase-1.0.6-1
- Some changes by Marko related to Q2 server
* Fri Nov 30 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.7-1.el5.fmi
- Marko added new features
* Mon Nov 19 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.7-1.el5.fmi
- Linked with newbase 1.0.4-1
* Thu Nov 15 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.6-1.el5.fmi
- Linked with newbase 1.0.3-1
* Thu Oct 18 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.5-1.el5.fmi
- New improvements from Marko Pietarinen
* Mon Sep 24 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.4-1.el5.fmi
- Fixed "make depend".
* Fri Sep 14 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.3-1.el5.fmi
- Added "make tag" feature
* Thu Sep 13 2007 mheiskan <mika.heiskanen@fmi.fi> - 1.0.2-1.el5.fmi
- Improved make system plus new code from Pietarinen
* Thu Jun  7 2007 tervo <tervo@xodin.weatherproof.fi> - 
- Initial build.
