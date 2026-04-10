# LCD4Linux for AX206 USB Displays  [![CI Build Test](https://github.com/amd989/lcd4linux-ax206/actions/workflows/ci.yml/badge.svg)](https://github.com/amd989/lcd4linux-ax206/actions/workflows/ci.yml)

This is a fork of [ukoda's lcd4linux-ax206](https://github.com/ukoda/lcd4linux-ax206), which itself is a fork of [MaxWiesel's lcd4linux-max](https://github.com/MaxWiesel/lcd4linux-max), focused on making AX206 USB LCD displays (commonly sold as "AIDA64 3.5" USB displays") easy to set up on modern Linux and FreeBSD systems including Rocky/RHEL, Ubuntu/Mint, Raspberry Pi, and FreeBSD.

The 3.5" AX206 USB displays are cheap and widely available, but most of the software ecosystem around them targets Windows (AIDA64) or uses Turing Smart Screen protocols that these displays aren't compatible with. LCD4Linux supports them natively on Linux, but getting a good-looking setup has historically required writing config files from scratch with little visual guidance.

This fork aims to fix that by providing ready-to-use themes ported from the [Turing Smart Screen](https://github.com/mathoudebine/turing-smart-screen-python) theme ecosystem and adapted to work with LCD4Linux's config system. In the process, new widgets were added: **Gauge** (circular arc rings) and **Sparkline** (line graph history); to support the variety of visual styles these themes use. The goal is to make it as easy as possible to plug in a display and have a polished system monitor running in minutes.

The best source for general LCD4Linux information is [The unofficial LCD4Linux Wiki](https://wiki.lcd4linux.tk/doku.php/start).

![Example](lcd4linux_example.png)

## Features

- DPF, VNC, and X11 display drivers
- Display mirroring (e.g. DPF + VNC for remote viewing)
- Pixel-level brightness control (0-100) separate from hardware backlight (0-7)
- GraphicBar widget with color thresholds
- TrueType font rendering with background color support
- `precision()` function for formatted numeric display
- MySQL/MariaDB reconnect fix
- (NEW✨) APT, DNF, and FreeBSD pkg repositories — install with `apt install`, `dnf install`, or `pkg install`
- (NEW✨) Pre-built packages for amd64, arm64, and armhf (including Raspberry Pi) and FreeBSD amd64
- (NEW✨) Pre-built themes for system monitoring and NAS displays
- (NEW✨) Sparkline widget for line graph history (e.g. temperature, I/O over time)
- (NEW✨) Gauge widget for circular arc gauges (CPU, RAM, disk usage rings)

## Warnings

Be aware that AIDA64 is actually Windows software, not the displays themselves.  While LCD4Linux works with the 3.5" AIDA64 USB displays, it will not work with 5" AIDA64 displays as those are HDMI.  The 3.5" USB displays use the AX206 chipset designed for photo frames but can be hacked for general use.  All 3.5" USB displays I have purchased so far have the correct firmware for use with LCD4Linux.  Searching online for AX206 displays yields few results so you are better searching for AIDA64 displays then filtering for USB versions only.

## Installation

Pre-built packages are available for Debian/Ubuntu, Rocky/RHEL/Fedora, and FreeBSD systems. Packages are built automatically by CI for amd64, arm64, and armhf (Debian), x86_64 and aarch64 (RPM), and amd64 (FreeBSD pkg) architectures, so manual cross-compilation is not needed.

### Debian / Ubuntu / Raspberry Pi OS (APT)

```bash
# Add the repository GPG key and source
curl -fsSL https://amd989.github.io/lcd4linux-ax206/gpg.key | sudo gpg --dearmor -o /usr/share/keyrings/lcd4linux-ax206.gpg
echo "deb [signed-by=/usr/share/keyrings/lcd4linux-ax206.gpg] https://amd989.github.io/lcd4linux-ax206 stable main" | sudo tee /etc/apt/sources.list.d/lcd4linux-ax206.list

# Install
sudo apt update
sudo apt install lcd4linux-ax206
```

Or use the convenience script:
```bash
curl -fsSL https://amd989.github.io/lcd4linux-ax206/setup.sh | sudo bash
```

### Rocky / RHEL / Fedora (DNF)

```bash
# Add the repository
sudo curl -fsSL https://amd989.github.io/lcd4linux-ax206/setup-rpm.sh | sudo bash

# Install
sudo dnf install lcd4linux-ax206
```

### FreeBSD (pkg)

```bash
# Add the repository
curl -fsSL https://amd989.github.io/lcd4linux-ax206/setup-pkg.sh | sudo sh

# Install
sudo pkg install lcd4linux-ax206
```

### What gets installed

**Linux:**
- `/usr/bin/lcd4linux` — the binary
- `/etc/lcd4linux/lcd4linux.conf` — default configuration (320x240 DPF layout)
- `/etc/lcd4linux/examples/` — additional example configs
- `lcd4linux-ax206.service` — systemd service unit

**FreeBSD:**
- `/usr/local/bin/lcd4linux` — the binary
- `/usr/local/etc/lcd4linux/lcd4linux.conf` — default configuration
- `/usr/local/etc/lcd4linux/examples/` — additional example configs
- `/usr/local/share/lcd4linux/themes/` — pre-built themes
- `/usr/local/etc/rc.d/lcd4linux` — rc.d service script

### Starting the service

**Linux:**
```bash
sudo systemctl start lcd4linux-ax206
sudo systemctl enable lcd4linux-ax206    # start on boot
```

**FreeBSD:**
```bash
sudo sysrc lcd4linux_enable=YES
sudo service lcd4linux start
```

The service reads from `/etc/lcd4linux/lcd4linux.conf` (Linux) or `/usr/local/etc/lcd4linux/lcd4linux.conf` (FreeBSD). Edit this file to match your display and desired layout.

## Building from source

If you prefer to build from source, or need to modify the code:

### Build dependencies

**Debian/Ubuntu:**
```bash
sudo apt install build-essential autoconf automake libtool libtool-bin pkg-config \
    libusb-1.0-0-dev libgd-dev libvncserver-dev libx11-dev libxext-dev libxpm-dev
```

**Rocky/RHEL:**
```bash
sudo dnf install epel-release
sudo dnf config-manager --set-enabled crb    # or powertools on older versions
sudo dnf install gcc make autoconf automake libtool pkgconfig \
    libusbx-devel gd-devel libvncserver-devel \
    libX11-devel libXext-devel libXpm-devel gettext-devel
```

**FreeBSD:**
```bash
# libusb is part of FreeBSD base — no package needed
sudo pkg install autoconf automake libtool pkgconf gettext-tools \
    libgd libvncserver xorg-libraries
```

### Build

```bash
./bootstrap
./configure --with-drivers=DPF,VNC,X11
touch lcd4linux.c && make
```

You can select only the drivers you need:
```bash
./configure --with-drivers=DPF          # USB display only
./configure --with-drivers=DPF,X11      # USB + X11 window
./configure --with-drivers=DPF,VNC,X11  # USB + VNC server + X11
```

### Run

```bash
./lcd4linux -F -f /path/to/config.conf   # foreground mode
./lcd4linux -l                            # list available drivers/plugins
```

## Known issues

### Variables evaluated alphabetically

Variables in the config are evaluated in alphabetical order, not definition order. This means:

```
Variables {
    white      'ffffff'
    std_fg     white
}
```
`std_fg` will be empty because `std_fg` is evaluated before `white` (s < w). Use the hex value directly or choose variable names that sort correctly.

### New widget struct field order

When creating a new widget, the `typedef struct WIDGET_MYNEWWIDGET` must have its first fields (up through `visible`) in the same order as `WIDGET_IMAGE`. Internal code casts widget pointers to `WIDGET_IMAGE*`, so mismatched field order causes subtle bugs.

Config file information
=======================

The best documentation on the configuration file is at [The unofficial LCD4Linux Wiki](https://wiki.lcd4linux.tk/doku.php/start). Full documentation for features specific to this fork is on the [AX206 fork wiki](https://github.com/amd989/lcd4linux-ax206/wiki/ax206_features). Brief summaries are below.

## Display settings

A `brightness` setting (0–100) has been added alongside `backlight` for smooth dimming without flicker. A `LCD::brightness` plugin mirrors `LCD::backlight` for runtime adjustment via a Timer widget.

See [AX206 Fork Features](https://github.com/amd989/lcd4linux-ax206/wiki/ax206_features) on the wiki for details.

## TrueType Widget

Renders text using TrueType fonts via libgd. Supports variable font size, alignment, background color, and a debug border mode.

See the [TrueType Widget wiki page](https://github.com/amd989/lcd4linux-ax206/wiki/widget_truetype) for full field reference and examples.

## Driver mirroring

Allows a second driver (VNC or X11) to display a live copy of the primary display — useful for remote viewing of a physical AX206 display. Both drivers must be graphics displays with the same resolution.

See [AX206 Fork Features](https://github.com/amd989/lcd4linux-ax206/wiki/ax206_features#driver-mirroring) on the wiki for the full config example.

## Gauge Widget

A circular arc gauge that fills proportionally to a value. Supports configurable start angle, sweep, ring thickness, fill direction, color thresholds, and reverse fill mode.

See the [Gauge Widget wiki page](https://github.com/amd989/lcd4linux-ax206/wiki/widget_gauge) for full field reference and examples.

## GraphicBar Widget

A pixel-based bar graph for graphics displays. Supports pixel-level sizing, negative minimums, directional fill (N/E/S/W), color thresholds, and hollow style.

See the [GraphicBar Widget wiki page](https://github.com/amd989/lcd4linux-ax206/wiki/widget_graphicbar) for full field reference and examples.

## Sparkline Widget

A line graph widget that records historical values in a ring buffer and renders a connected trend line. Auto-scales Y-axis by default; supports fixed range and color thresholds.

See the [Sparkline Widget wiki page](https://github.com/amd989/lcd4linux-ax206/wiki/widget_sparkline) for full field reference and examples.

## precision function

Formats a float with a fixed number of decimal places (adds trailing zeros for alignment). Returns a string — apply it after any math.

```
expression  precision(file::readline(F_PW, 3), 3) . ' kW '
```

## Theme Editor

A visual theme editor is included in the [`ThemeEditor/`](ThemeEditor/) directory — a cross-platform desktop app (Avalonia UI, .NET 10) for opening, previewing, and editing lcd4linux theme config files. Supports drag-to-reposition, a color picker, undo/redo, and zoom.

See [AX206 Fork Features](https://github.com/amd989/lcd4linux-ax206/wiki/ax206_features#theme-editor) on the wiki for build instructions and usage.

## Themes

35+ pre-built themes are included in the [`themes/`](themes/) directory, covering system monitors, NAS dashboards, landscape layouts, cyberpunk/terminal styles, and more — in portrait and landscape orientations with color variants for each style.

**[Browse the Theme Gallery →](themes/README.md)**

To use a theme, point lcd4linux at its config file:

```bash
lcd4linux -F -c themes/SimpleWhite/dpf_simplewhite.conf
```

Original theme designs from [turing-smart-screen-python](https://github.com/mathoudebine/turing-smart-screen-python), adapted for lcd4linux. Theme authors: [@takattila](https://github.com/takattila), [@mathoudebine](https://github.com/mathoudebine), [@napobear](https://github.com/napobear), [@amiltonjr](https://github.com/amiltonjr). Licensed under GPL-3.0.

