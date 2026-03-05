Name:           lcd4linux-ax206
Version:        %{_version}
Release:        1%{?dist}
Summary:        LCD display driver for AX206 USB displays
License:        GPL-2.0-or-later
URL:            https://github.com/amd989/lcd4linux-ax206
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make
BuildRequires:  autoconf
BuildRequires:  automake
BuildRequires:  libtool
BuildRequires:  pkgconfig
BuildRequires:  libusbx-devel
BuildRequires:  gd-devel
BuildRequires:  libvncserver-devel
BuildRequires:  libX11-devel
BuildRequires:  libXext-devel
BuildRequires:  libXpm-devel
BuildRequires:  gettext-devel

Requires:       libusbx
Requires:       gd
Requires:       libvncserver
Requires:       libX11

%description
LCD4Linux fork focused on AX206 USB LCD displays (AIDA64 3.5" USB displays)
with support for DPF, VNC, and X11 drivers. Reads system data via plugins
and renders widgets onto the display.

Features include pixel-level brightness control, VNC mirror display support,
TrueType font rendering, and graphical bar widgets with color thresholds.

%prep
%setup -q
# automake in gnu mode requires README; project uses README.md
ln -sf README.md README

%build
./bootstrap
%configure --with-drivers=DPF,VNC,X11
touch lcd4linux.c
make %{?_smp_mflags}

%install
install -D -m 0755 lcd4linux %{buildroot}%{_bindir}/lcd4linux
install -D -m 0644 dpf_320x240.conf %{buildroot}%{_sysconfdir}/lcd4linux/examples/dpf_320x240.conf
install -D -m 0644 lcd4linux.conf.sample %{buildroot}%{_sysconfdir}/lcd4linux/examples/lcd4linux.conf.sample
install -D -m 0644 debian/lcd4linux-ax206.service %{buildroot}%{_unitdir}/lcd4linux-ax206.service

%files
%license COPYING
%doc README.md
%{_bindir}/lcd4linux
%{_unitdir}/lcd4linux-ax206.service
%dir %{_sysconfdir}/lcd4linux
%dir %{_sysconfdir}/lcd4linux/examples
%config %{_sysconfdir}/lcd4linux/examples/*.conf
%config %{_sysconfdir}/lcd4linux/examples/*.sample

%changelog
