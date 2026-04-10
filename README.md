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

The best documentation on the configuration file is at [The unofficial LCD4Linux Wiki](https://wiki.lcd4linux.tk/doku.php/start).  Below is information specific to this fork.

## Display settings

The `backlight` setting is not linear and will blank the display or flicker at low values.  To improve this a `brightness` setting has been added with the range 0 to 100 (default 100).  It scales pixel values as a percentage.  Set the lowest `backlight` value that gives a stable backlight, then use `brightness` to dim to suit your needs.  Setting backlight higher than needed will cause light bleed on black areas.

## Display Plugins

The display has an undocumented `LCD::backlight` plugin that you would normally call from a Timer Widget to adjust the backlight during operation.  A `LCD::brightness` plugin has been added to control pixel brightness in the same way.

## TrueType Widget

The TrueType widget class from Eric Loxat renders text using TrueType fonts.

Fields:
- `class`: Must be 'Truetype'.
- `expression`: The expression for the text to display.
- `font`: The full path to the TrueType font file.
- `size`: Font height (not including descenders, not in pixels). If 0 or omitted, auto-sizes to fit the box.
- `width`: Width of the bounding box in pixels.
- `height`: Height of the bounding box in pixels.
- `align`: Horizontal alignment: 'L', 'C', or 'R'.
- `fcolor`: Font color as RGB or RGBA hex (e.g. 'ffffff').
- `background`: Background color as RGB or RGBA. If not defined, background is transparent. *(Added in this fork)*
- `inverted`: Inverts fcolor (e.g. '0000ff' becomes 'ffff00').
- `debugborder`: Color for the bounding box outline, useful for layout debugging.
- `update`: Update time in ms.
- `visible`: Visibility expression.

If `size` is given and the text is too large for the box, it will be cropped. Text is always vertically centered, ignoring descenders.

Example:
```
Widget Debug {
    class 'Truetype'
    expression 'Test'
    font '/usr/share/fonts/gnu-free/FreeSans.ttf'
    width 60
    height 80
    fcolor 'ffffff'
}
```

## Driver mirroring

This allows a second driver to mirror the normal driver.  Intended use case is where a machine has a local physical display (e.g. AX206) and you wish to view that screen remotely via VNC or X11.

The mirror driver must be different from the main driver and both must be graphics displays with the same resolution.

To use, define two `Display` sections and add a `Mirror` declaration:

```
Layout  'Compact'
Display 'DPF'
Mirror  'VNC'

Display dpf {
    driver      'DPF'
    port        'usb0'
    font        '6x8'
    orientation 0
    backlight   4
    foreground  _white
    background  _black
    basecolor   _black
}

Display VNC {
    Driver       'VNC'
    Font         '6x8'
    Port         '5900'
    Xres         '480'
    Yres         '320'
    Bpp          '4'
    Maxclients   '2'
    Osd_showtime '2000'
}
```

NB: Only tested with DPF paired with VNC or X11 drivers.

## Gauge Widget

A circular arc gauge widget that fills proportionally to an expression value. Draws a ring arc using GD, with configurable start angle, sweep, thickness, and fill direction. Supports color thresholds and reverse fill mode.

Fields:
- `class`: Must be 'Gauge'.
- `expression`: The expression for the gauge value.
- `width`: Width of the bounding box in pixels.
- `height`: Height of the bounding box in pixels.
- `min` / `max`: Value range. Defaults to 0–100.
- `start`: Start angle in degrees (GD convention: 0 = 3 o'clock, increases clockwise). Default: 135.
- `sweep`: Total arc sweep in degrees. Default: 270.
- `thickness`: Ring thickness in pixels. If 0 or omitted, draws a filled pie wedge.
- `direction`: Fill direction: 'CW' (clockwise, default) or 'CCW' (counter-clockwise).
- `color`: Foreground (filled) arc color (RGB/RGBA). Default: bright green.
- `background`: Background (unfilled) arc color (RGB/RGBA). Use `000000ff` for transparent.
- `valuelow` / `colorlow`: Threshold and color for low values.
- `valuehigh` / `colorhigh`: Threshold and color for high values.
- `reverse`: Set to 1 to invert fill (foreground shows remaining portion).
- `update`: Update time in ms.

Example:
```
Widget CPU_Gauge {
    class 'Gauge'
    expression proc_stat::cpu('busy', 500)
    width 66
    height 66
    min 0
    max 100
    start 180
    sweep 360
    thickness 5
    color '35bf5c'
    background '000000ff'
    update 500
    direction 'CCW'
}
```

## GraphicBar Widget

A pixel-based bar graph widget for graphics displays. Key differences from the text-mode Bar widget:
- Single value (no two-value kludge)
- Pixel-based sizing and positioning
- Supports negative minimum values (bar starts from the zero point)
- Color thresholds for value ranges (e.g. green at low loads, yellow at moderate, red at high)

Fields:
- `class`: Must be 'GraphicBar'.
- `expression`: The expression for the bar value.
- `length`: Length of the bar axis in pixels.
- `width`: Thickness of the bar in pixels.
- `direction`: Direction of bar travel: 'N', 'E', 'S', or 'W'. Default is 'E'.
- `min` / `max`: Axis range. If omitted, auto-ranges.
- `color`: Nominal bar color (RGB/RGBA). Default: bright green.
- `background`: Background color (RGB/RGBA). Default: dark gray.
- `valuelow` / `colorlow`: Threshold and color for low values.
- `valuehigh` / `colorhigh`: Threshold and color for high values.
- `style`: Set to 'H' for hollow bar.
- `update`: Update time in ms.

## Sparkline Widget

A line graph widget that records historical values in a ring buffer and renders them as a connected line graph. Useful for visualizing trends over time such as temperature, CPU usage, or disk I/O.

- One sample per pixel column — a 140px wide widget stores 140 data points
- Auto-scales Y-axis by default, with optional fixed `min`/`max` overrides
- Color thresholds for value ranges (same as GraphicBar)

Fields:
- `class`: Must be 'Sparkline'.
- `expression`: The expression to sample on each update.
- `length`: Length of the graph axis in pixels (number of samples stored).
- `width`: Thickness of the graph in pixels.
- `min` / `max`: Y-axis range. If omitted, auto-scales to fit the data.
- `color`: Line color (RGB/RGBA). Default: bright green.
- `background`: Background color (RGB/RGBA). Default: dark gray.
- `valuelow` / `colorlow`: Threshold and color for low values.
- `valuehigh` / `colorhigh`: Threshold and color for high values.
- `update`: Update/sample interval in ms.

Example:
```
Widget CPU_Spark {
    class      'Sparkline'
    expression  proc_stat::cpu('user', 500)
    width       50
    length      140
    color       '00ff00'
    background  '333333'
    min         0
    max         100
    update      1000
}
```

## precision function

Formats floating point numbers with a fixed number of decimal places (adds trailing zeros for column alignment).  The output is a string, so use it after any math.

Example:
```
expression  precision(file::readline(F_PW, 3), 3) . ' kW '
```

## Theme Editor

A visual theme editor is included in the [`ThemeEditor/`](ThemeEditor/) directory. It is a cross-platform desktop application (Avalonia UI, .NET 10) that lets you open, preview, and edit lcd4linux theme config files without touching the raw text.

Features:
- Opens any lcd4linux `.conf` file and renders its widgets on a scaled canvas
- Drag widgets to reposition them; positions are written back to the `Layout` section on save
- Property panel on the right lets you edit widget fields — text, colors (with a color picker), numbers, dropdowns for `class` and `align` fields
- Image widgets display their actual image file on the canvas
- Zoom slider (50%–300%) for detailed layout work
- Undo/redo support
- Detects display orientation from the config (`Orientation` field) and sizes the canvas accordingly (480×320 landscape, 320×480 portrait)

### Building the Theme Editor

Requires the [.NET 10 SDK](https://dotnet.microsoft.com/download).

```bash
cd ThemeEditor
dotnet build
dotnet run
```

### Usage

1. Launch the editor and click **Load Theme**
2. Browse to a theme `.conf` file (e.g. `themes/SimpleWhite/dpf_simplewhite.conf`)
3. The display canvas shows all widgets at their configured positions
4. Click a widget to select it and edit its properties in the panel on the right
5. Drag widgets to reposition them on the canvas
6. Click **Save Theme** to write changes back to the original `.conf` file

## Themes

35+ pre-built themes are included in the [`themes/`](themes/) directory, covering system monitors, NAS dashboards, landscape layouts, cyberpunk/terminal styles, and more — in portrait and landscape orientations with color variants for each style.

**[Browse the Theme Gallery →](themes/README.md)**

To use a theme, point lcd4linux at its config file:

```bash
lcd4linux -F -c themes/SimpleWhite/dpf_simplewhite.conf
```

Original theme designs from [turing-smart-screen-python](https://github.com/mathoudebine/turing-smart-screen-python), adapted for lcd4linux. Theme authors: [@takattila](https://github.com/takattila), [@mathoudebine](https://github.com/mathoudebine), [@napobear](https://github.com/napobear), [@amiltonjr](https://github.com/amiltonjr). Licensed under GPL-3.0.

