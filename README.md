# LCD4Linux for AX206 USB Displays

This is a fork of [MaxWiesel's lcd4linux-max](https://github.com/MaxWiesel/lcd4linux-max) focused on supporting AX206 USB LCD displays (commonly sold as "AIDA64 3.5" USB displays") on modern Linux systems including Rocky/RHEL, Ubuntu/Mint, and Raspberry Pi.

The best source for general LCD4Linux information is [The unofficial LCD4Linux Wiki](https://wiki.lcd4linux.tk/doku.php/start).

I also have another repo, https://github.com/ukoda/ax206vncclient, that provides an AX206 VNC client that can be paired up with the LCD4Linux VNC server display driver.  When paired with the display mirroring feature added in this repo it makes for an easy way to have an AX206 on a server in a rack and see the same information on a remote AX206 display.

As an example here is a screen capture of a VNC client mirroring the physical AX206 display on a remote server:
![Example](lcd4linux_example.png)

## Features

- DPF, VNC, and X11 display drivers
- Display mirroring (e.g. DPF + VNC for remote viewing)
- Pixel-level brightness control (0-100) separate from hardware backlight (0-7)
- GraphicBar widget with color thresholds
- TrueType font rendering with background color support
- `precision()` function for formatted numeric display
- MySQL/MariaDB reconnect fix

## Warnings

Be aware that AIDA64 is actually Windows software, not the displays themselves.  While LCD4Linux works with the 3.5" AIDA64 USB displays, it will not work with 5" AIDA64 displays as those are HDMI.  The 3.5" USB displays use the AX206 chipset designed for photo frames but can be hacked for general use.  All 3.5" USB displays I have purchased so far have the correct firmware for use with LCD4Linux.  Searching online for AX206 displays yields few results so you are better searching for AIDA64 displays then filtering for USB versions only.

## Installation

Pre-built packages are available for Debian/Ubuntu and Rocky/RHEL/Fedora systems. Packages are built automatically by CI for amd64, arm64, and armhf (Debian) or x86_64 and aarch64 (RPM) architectures, so manual cross-compilation is not needed.

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

### What gets installed

- `/usr/bin/lcd4linux` — the binary
- `/etc/lcd4linux/lcd4linux.conf` — default configuration (320x240 DPF layout)
- `/etc/lcd4linux/examples/` — additional example configs
- `lcd4linux-ax206.service` — systemd service unit

### Starting the service

```bash
sudo systemctl start lcd4linux-ax206
sudo systemctl enable lcd4linux-ax206    # start on boot
```

The service reads from `/etc/lcd4linux/lcd4linux.conf`. Edit this file to match your display and desired layout.

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

## precision function

Formats floating point numbers with a fixed number of decimal places (adds trailing zeros for column alignment).  The output is a string, so use it after any math.

Example:
```
expression  precision(file::readline(F_PW, 3), 3) . ' kW '
```
