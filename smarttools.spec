%define LIBNAME smarttools
Summary: smarttools library
Name: libsmartmet-%{LIBNAME}
Version: 10.9.14
Release: 1.el5.fmi
License: FMI
Group: Development/Libraries
URL: http://www.weatherproof.fi
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-buildroot-%(%{__id_u} -n)
BuildRequires: libsmartmet-newbase >= 10.9.14-1
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
%{_libdir}/libsmartmet_%{LIBNAME}-mt.a

%changelog
* Tue Sep 14 2010 Mika Heiskanen <mika.heiskanen@fmi.fi> - 10.9.14-1.el5.fmi
* Upgrade to boost 1.44 and newbase 10.9.14-1
* Mon Jul  5 2010 Mika Heiskanen <mika.heiskanen@fmi.fi> - 10.7.5-1.el5.fmi
- Compiled with newbase 10.7.5-1
* Thu May  6 2010 Mika Heiskanen <mika.heiskanen@fmi.fi> - 10.5.6-1.el5.fmi
- A large number of changes by Marko
* Fri Jan 15 2010 Mika Heiskanen <mika.heiskanen@fmi.fi> - 10.1.15-1.el5.fmi
- Recompiled with latest newbase
* Mon Jan 11 2010 Mika Heiskanen <mika.heiskanen@fmi.fi> - 10.1.11-1.el5.fmi
- Numerous minor updates
* Tue Jul 14 2009 Mika Heiskanen <mika.heiskanen@fmi.fi> - 9.7.14-1.el5.fmi
- Upgrade to boost 1.39 
- Minor changes by Marko Pietarinen
* Wed Apr 22 2009 mheiskan <mika.heiskanen@fmi.fi> - 9.4.22-1.el5.fmi
- Compile single- and multithread versions
* Fri Mar 27 2009 mheiskan <mika.heiskanen@fmi.fi> - 9.3.27-1.el5.fmi
- Updates from Marko Pietarinen
* Tue Jan 19 2009 Marko Pietarinen <marko.pietarinen@fmi.fi> - 9.1.20-1.el5.fmi
- Some bugfixes to newbase
* Mon Jan 19 2009 Marko Pietarinen <marko.pietarinen@fmi.fi> - 9.1.19-1.el5.fmi
- Smallish upgrades by Marko
* Mon Sep 30 2008 westerba <antti.westerberg@fmi.fi> - 8.9.30-1.el5.fmi
- Fixed typecast problems in NFmiSmartToolIntepreter.cpp
* Mon Sep 29 2008 mheiskan <mika.heiskanen@fmi.fi> - 8.9.29-1.el5.fmi
- Newbase header change forced rebuild
* Mon Sep 22 2008 mheiskan <mika.heiskanen@fmi.fi> - 8.9.22-1.el5.fmi
- A few updates by Marko
* Thu Sep 11 2008 mheiskan <mika.heiskanen@fmi.fi> - 8.9.11-1.el5.fmi
- A few API updates
* Tue Jul 15 2008 mheiskan <mika.heiskanen@fmi.fi> - 8.7.15-1.el5.fmi
- Compiled with newbase 8.7.15-1
* Tue Mar 11 2008 mheiskan <mika.heiskanen@fmi.fi> - 8.3.11-1.el5.fmi
- Linked against 8.3.11-1
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
