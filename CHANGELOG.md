## What's Changed
* docs: 📝 add changelog.md
* docs: 📝 update readme.md by @amd989

## What's Changed in 0.13.0
* style: 💄 add LandscapeMagicBlue theme by @amd989
* style: 💄 add simplecyberpunkgauge theme by @amd989
* style: 💄 add LandscapeModernDevice35 theme by @amd989
* style: 💄 add LandscapeEarth theme by @amd989
* style: 💄 add cyberpunk-net theme by @amd989
* feat: ✨ add gauge widget by @amd989 in [#3](https://github.com/amd989/lcd4linux-ax206/pull/3)

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/v0.12.0...v0.13.0

## What's Changed in 0.12.0
* ci: 💚 fix release workflow by @amd989
* feat: ✨ add sparkline widget by @amd989 in [#2](https://github.com/amd989/lcd4linux-ax206/pull/2)
* docs: 📝 update readme.md by @amd989
* chore: add FUNDING.yml by @amd989
* ci: 👷 only run ci on pr by @amd989

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/v0.11.0...v0.12.0

## What's Changed in 0.11.0
* fix: 🐛 service typo by @amd989
* ci: 👷 add ci/cd pipelines by @amd989
* feat: 💄 add simplewhite theme by @amd989
* feat: 💄add themes by @amd989
* chore: misc changes by @amd989
* docs: 📝 update readme.md by @amd989
* fix: 🐛 correct casting error by @amd989
* Corrections in README.md
* Fixed typo
* Added brightness control.  Removed a few build warning messages
* Updated to show build date and time if lcd4linux.c is touched. Improved SQL plugin error handling
* Typo fix
* Updated READ.md for requirements so SQL plugin will be included
* Added precision function
* Updated README.md about systemd file
* Added notes on using systemd
* Some readme formatting fixes and typo corrections
* Added example image
* Improved readme formatting and link to ax206vncclient repo
* Added Raspberry Pi build instructions
* Added GraphicBar widget.  Cleaned up some build warnings
* Minor README.md typo fix
* Added driver mirroring functionality
* Update build info based on building a a few systems
* Added nots on building with VNC driver on Rocky
* Fixed TrueType size documentation
* Docmented issue with variables evaluation
* Updated README.md for TrueType widget additions
* Added background colour option to TrueType widget.
* Added Mint build instructions
* More typo fixes.  Hard to believe English is my first language!
* Added documentation on TrueType widget.
* Updated README.md to better reflect current state
* Another typo fix
* Now able to build lcd4linux with DPF and X11 support.  These files are autocreated but where in original repo so updating now it builds for me
* Fix for DPF driver to link to correct USB library
* Updated .gitignore to ignore object files
* Updated drv_dpf.c to use ver 1.0 of USB library.  Linker still needs fixed to use 1.0 as well
* Updated INSTALL to add warning about its usablity.  Should now be able to set up build enviroment to start building
* Build set is stubborn, demands README.  Dummy file added to keep it happy
* WIP. Corrections to build set to handle README renamed as READM.md
* Fix typos
* Fix URL formatting
* Fixed readme file name so markdown works
* Updated README to reflect intent of this fork
* Changed indent.sh to replace tabs with spaces so any editor can be used, then updated all C source files.
* max ttf max size by @TangoCash
* fix aligment for ttf, add debugborder paramter by @TangoCash
* remove chmod check for cfg file by @TangoCash
* bpp fix in sample file
* ili9486 colors fix
* add TEW673GRU driver
* fix last commit
* add generic spidev driver
* aarch64 support
* add description for ili9486
* add sample config for ili9486
* add ili9486 orange pi/rapsberry pi lcd support
* Merge branch 'master' of https://github.com/TangoCash/lcd4linux
* add SPF-83M by @TangoCash
* fix last commit by @BPanther
* vusolo4k -> vuplus4k, working on vusolo4k, vuduo4k
* fix last commit
* vusolo4k: calculate brightness
* add stride
* fix commit aa03a3f2eb232b3a398836589134fca815b3afdc
* vusolo4k -> vuplus4k, working on vusolo4k, vuduo4k
* fix last commit
* vusolo4k: calculate brightness
* Update .gitignore by @redblue-pkt
* Merge branch 'master' of https://github.com/TangoCash/lcd4linux
* read bpp, xres, yres from driver by @redblue-pkt
* - plugin_math: add function to round digits to next integer by @svenhoefer
* Merge branch 'master' of https://github.com/TangoCash/lcd4linux
* increase layers to 5 by @TangoCash
* read bpp, xres, yres from driver by @redblue-pkt
* Merge branch 'master' of github.com:TangoCash/lcd4linux by @TangoCash
* fix colors
* add support vusolo4k
* - widget_image: allow png/jpg/gif/bmp images; thx to BPanther by @svenhoefer
* - cfg: allow up to 512 chars in the lines of config-file by @svenhoefer
* - plugin_file: allow up to 265 chars in file::readline function by @svenhoefer
* fix longer Display names by @TangoCash
* supplemental to last commit by @TangoCash
* rework centering images by @TangoCash
* adding alphachannel to truetype color by @TangoCash
* re-add obsolete -q parameter due compatibility issues by @TangoCash
* - .cvsignore -> .gitignore by @svenhoefer

### New Contributors
* @amd989 made their first contribution
* @BPanther made their first contribution
* @redblue-pkt made their first contribution

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.11.0...v0.11.0

## What's Changed in 0.11.0
* - update configure stuff; remove substring 'SVN' from version string by @svenhoefer
* add option to ttf to use alignment without autosize by @TangoCash
* adding autosize with width, heigth and alignment by @TangoCash
* format source code - no code changes by @TangoCash
* adding resize to width and/or height in image widget by @TangoCash
* auto-detect Samsung SPF by @TangoCash
* - update version handling; call 'make vcs_version' before 'make all' by @svenhoefer
* - drv_generic_graphic.c: allow to define vertical area ... by @svenhoefer
* - widget_ttf.c: remove wrong brackets by @svenhoefer
* - spf: don't use suffixes like 'new' and 'old' anymore by @svenhoefer
* - dpf: read goodbye message from /tmp/lcd/goodbye by @svenhoefer
* - add center function for images/ttf by @svenhoefer
* - plugin_file.c: add fileexist function by @svenhoefer
* - spf: send data all in one by @svenhoefer
* - spf: keep SPF87h and friends in MiniMonitorMode by @svenhoefer
* - spf: add some more spf's by @svenhoefer
* - spf: add jpeg_mem_dest.h by @svenhoefer
* - font_6x8*.h: add german umlauts; small fixes by @svenhoefer
* - drv_dpf.c: use drv_generic_graphic_clear() while init by @svenhoefer
* - plugin_netinfo.c: log2l -> log2f by @svenhoefer
* - timer.c: do also check diff.tv_usec to trigger next update by @svenhoefer
* reformat new source code by @TangoCash
* update sample config by @TangoCash
* adding scaling option to image widget by @TangoCash
* adding truetype rendering widget by @TangoCash
* quiet start as default by @TangoCash
* Replace non-portable datatypes with C99 versions
* drv_HD44780: add 8bit i2c mode for HD44780 by Michael Grzeschik <m.grzeschik@pengutronix.de>
* updated to automake 1.14.1
* autotools update (iconv?)
* indentation
* DPF patch by superelchi
* raspi: Disabled unless explicitly enabled by user
* disable plugin sensors if not found
* new plugin raspi
* added new examples to config file
* Ticket #284
* stabilize text widget when no string is available; info message when serdisplib cannot open port
* widget_timer: oops, forgot to update includes...
* widget_timer: timer widgets should not abuse timer groups (fixes ticket #278)
* Mitigate format string exploits
* Add drv_generic_serial.c functions to header file
* typo fix from Robert Resch
* patch to support hex sequences from Marcus Menzel
* Fixed realloc error handling in timer_group.c; changed the interval member in the TIMER_GROUP structure from int* to int.
* handle failing realloc() correctly (thanks to Mattia)
* changed error() to info() to avoid syslog flooding
* timer activation: use defines instead of integers for easier reading
* allow fine-grained backlight control for picoLCDGraphic
* jpeglib dependency typo fixed
* drv_SamsungSPF.h removed
* some fixes from Sascha Plazar
* driver for Samsung SPF by Sascha Plazar
* driver for Futaba VFD by Marcus Menzel
* teakLCM set GPIO='no' which produced linker errors (thanks to Marcus menzel for reporting)
* default model chnaged from 'generic' to 'M50530'
* driver for TeakLCM by Andreas Thienemann
* patch from Robert Tesch: support KIT120-5 and KIT129-6, GPO code repaired, use Scalefont
* Patch for Driver EA232Graphic by Robert Resch
* scaletext patch from Stefan Kuhne
* update to automake 1.11.2
* DPF driver by Stefan Kuhne 8note that you need libdpf)
* [PATCH 2/2] gcc warnings by Jonathan McCrohan
* ATCH 1/2] gcc warnings by Jonathan McCrohan
* old ancient legacy port access disabled by default
* All compilation related issues in the headers have been fixed. Now each header compiles with no warnings as a standalone file.
* Replaced parameter NoPowerCycle with PowerCycle
* Blit speedup, switch PICASO back to 9600 baud when exiting
* Removed compiler warning
* SGC support
* Fix spelling errors. Flagged by Debian lintian check
* driver for Futaba MDM166A Graphic(96x16) vf-displays by Andreas Brachold
* driver for Newhaven's NHD‐0420D3Z‐FL‐GBW. 4x20 LCD by Rusty Clarkson
* indentation :-(
* magic bold escape char by Andrew Thompson
* picoLCDGraphics needs keypad linked
* various compiler warnings fixed
* Autoconf 2.68
* run smoketest on all of my 8 cores
* indentation
* patch to replace outdate library libmpd with recent library libmpdclient from Andreas Brachold
* lcd4linux-wireless.patch from Hans de Goede
* st2205-width-height-swap.patch from Hans de Goede
* hwmon patch from Hans de Goede
* patch to support SureElectronicsDE-LD023 from Matt Joice
* Fixed keypad widget in GLCD2USB
* timer.c: fixed detection of positive clock skew (and some typos)
* timer.c: fixed clock skew detection
* timer.c: skip intermediate timer intervals
* fix for a bug introduced in the DE-LD021 driver
* indentation
* support for Sure Electronics DE-LD021 by Natanael Olaiz
* driver for EFN LED modules by Tilman Glötzner
* commented, corrected and beautified both timer and timer group code
* finished commenting the timer code
* started commenting the timer code
* Added brightness support for GLCD2USB
* Added 128x64 sample graphic
* Removed accidentially added file
* Minor description fixes
* patch from Jar: typo fixed, documentation which is now in the wiki removed
* added netmask_short in plugin_netinfo for CIDR notation
* compile error fixed (thanks to Tom)
* support for Sure Electronics's 1602 LCD Display Board with USB from Mikhail
* X11 driver: possibility to ignore auto-repeated KeyPress events
* ooops... forgot to add drv_ASTUSB.c
* driver for Allnet FW8888 from Claas Hilbrecht
* ASTUSB driver from Henrik Schondorff
* Ticket #170 (picolcd graphic build error) fixed by matt
* Ticket #184 (LCD::contrast and LCD::backlight segfaulting) fixed
* drv_picoLCDGraphic.c: changed two annoying info messages to debugging messages
* drv_picoLCDGraphic.c: alternative use of keypad widgets (instead of GPIO)
* X11 driver: correctly handle key releases
* changed hardcoded pixel resolution to configuration; usb_*_np functions only if available; try to get USB vendor name
* 'Fix the huawei e220 at commands' by Jar
* drv_picoLCDGraphic.c: minimum USB read timeout is now 1 ms
* drv_picoLCDGraphic.c: added configuration for USB read timeout
* indent.sh run
* plugin_huawei-fix-interval-check_V2.patch from Jar
* Improved config behavior.
* Fixed a segmentation fault bug in cfg_get().
* All static memory allocations were turned into dynamical ones.
* timer.c: two small optimizations (removed "flag"; tv_usec can't be negative)
* added grouping of widgets by update interval (new file "timer_group.c")
* BUG: handle negative delays in timer_process() (timer.c)
* timer.c: I had forgotten how to initialize a "struct" :)
* BUG: compensate timers for processing delay ("timer.c")
* timer.c: exchanged "proprietary code" with timeradd() and timersub() functions
* timer.c: removed fruitless changes to variable "flag" from timer_process()
* According to its man page, timercmp() is broken on some systems -- applied the recommended (and equivalent) change.
* small warning removed
* clear errno after creating the FIFO by Claas Hilbrecht
* plugin_huawei from Jar (forgot to add it last time)
* plugin_huawei from Jar
* picoLCDGraphic: updated example configuration files in "contrib/picoLCD"
* picoLCDGraphic: update display regularly using a timer (*much* faster than redrawing the whole display after each changed widget; you may get the old behaviour by setting "Update 0")
* additional debugging output
* fixed compile error on systems without strndup()
* include config.h in *every* source file
* X11 driver: do not display key strokes that do not map to a button
* indent.sh: ignore indentation profiles, so things are not messed up accidentally
* X11 driver: handle key presses (arrows, RET and ESC) in addition to clicks on the GUI buttons
* included "config.h" in "plugin.h" and changed "plugin.c" and "plugin_sample.c" accordingly
* plugin_python.c: print tracebacks also for Python scripts that do not "compile"
* plugin_python.c: print Python tracebacks on stderr (yay!)
* fixed compile error by including "config.h" in "plugin_python.c"
* support for 'aligned pingpong' by Martin Zuther
* use own strndup() from evaluator if not available on system
* fixed typo
* fixed memory leak in using strdup() instead of strdupa()
* Ticket 182
* strdupa uses alloca which is not available everywhere. strdup is used instead
* plugins work on MacOS X: proc_stat, cpuinfo
* linker error with LW_ABP corrected
* update to autoconf 2.65
* indentation
* indentation
* add event.h to extra sources
* Logic Way ABP08 ABP09 enhancements by Arndt Kritzner
* Remove warnings from mpris plugin
* Fix the dbus/pkg-config confgure (Now the configure script will probably require pkg-config to be installed)
* * Fix segfault on shutdown in drv_picoLCDGraphic.c
* special commandline parameters for X11
* Make the dbus plugin shutdown without showing dbus leaks when compiled with -DDEBUG
* drv_PICGraphic.c
* the DBus plugin is now disabled when dbus is not installed instead of stopping the build
* Prevent HD44780 and HD44780_I2C from building at the same time
* disabling some driver if no parport available
* Removed warning for the mpris dbus plugin
* MatrixOrbitalGX
* ppoll() is not available in glibc before 2.4
* Fixed a few more leaks,
* plugin_dbus.c:
* dbus_watch_get_unix_fd() only exists in libdbus >= 1.1.1
* small memory leak fixed (Thnaks to Ed and Valgrind)
* event plugin and dbus interface by Ed Martin (forgot to add new files)
* event plugin and dbus interface by Ed Martin
* Logic Way patch from Arndt Kritzner
* added protocol details to PICGraphic driver, fixed typos is generic headers
* updating Makefile.am for PICGraphic driver, reverting mistaken commit of aclocal.m4
* adding PICGraphic driver
* svn_version bumped up
* Logic Way GmbH ABP08 ABP09 enhancements by Arndt Kritzner
* driver for Logic Way GmbH ABP08 (serial) and ABP09 (USB) appliance control panels by Arndt Kritzner
* additional debug output
* autoconf patches from PT M.
* indentation
* indentation
* suppress compiler warning
* warn() => info()
* added support for Z-10 speaker to G-15 driver
* evaluator bug fixed (thanks to lleo19)
* indent run
* asterix extensions by gabrielortizlour@gmail.com
* icon frame patch by Marcus Brown
* update autoconf
* add qnap log plugin, thanks to rolf tralow
* indent
* timer fix for vnc driver
* more timer fixes
* make the code more sexy...
* update make files
* fix timestamp loop issue
* add widget sample
* add mpris dbus plugin, untested!
* indent
* update autoconf scripts
* add shuttle VFD driver
* update option file
* drv_vnc: http port as option
* drv_vnc: run as http daemon
* fix fps delay error
* add fps limiter
* add UNSECURE password option to drv_vnc - SSH would be the better choice.
* add keypadframe color as parameter, some minor fixups
* update keypad handling, add more options, remove compiler waarnings
* update lcd4linux sample config
* add server port as config option, rename options
* vnc driver: keypad works now
* some more keypad stuff for vnc driver...
* update keypad function for vnc driver
* vnc driver: add beta keypad support
* remove unneeded libs
* add sample cfg for vnc driver, indent
* add max client option
* ignore security check for cygwin
* string compare is not case sensitive anymore
* more cygwin fixes
* fix cygwin compile
* I just wanted to commit the 1000st change to lcd4linux ;)
* minor fixup
* more vnc driver stuff...
* add vnc driver
* test intersection of (displayable) widgets
* Fixed pot. buffer overflow in glcd2usb
* glcd2usb button support
* glcd2usb button support
* autoconf update
* GLCD2USB by Till Harbaum
* add asc255bug setting to MatrixOrbital driver by Maik Dilger
* indent
* driver for GX series lcd from Matrix Orbital by Abbas Kosan
* GLCD2USB driver from Till Harbaum
* added some contrib files from Nicu Pavel
* patch for picoLCDGraphics that adds keypad/gpi/gpo capabilities from Nicu Pavel
* uh-oh... changed perl comment to C comment :-)
* IRLCD patch from Jussi Kilpelainen
* removed backslash handling from cfg.c (its now part of the evaluator)
* add octal escape sequences to evaluator
* update st2205 desc.
* evaluator: escape sequences in strings (fix for ticket #145)
* wifi ioctl() fixed
* style 'H' (hollow) for vertical bars, individual bar colors for vertical bars fixed
* Brightness dimms every color
* more descriptive display names with option -l
* 'emty format string' compiler warning solved
* correct brightness handling, reduced round trips
* Closing XWindow quits without error
* option parsing modified
* dump config file if verbose_level > 1
* indentation
* layout parser simplified with cfg_rename
* new function cfg_rename()
* optimized drawing; check max. screen size
* version bumped; ChangeLog updated
* add support for Dell M1730 LCD to the G15 driver by lcd4linux@hayward.uk.com
* initial brightness fixed
* indentation, svn-version
* speedup X11 display
* new example for X11 config
* heavy X11 reorganization to (hopefully) finally fix Tickets #141 and #117
* list sub-drivers for serdisplib without -v
* moved 'print list of drivers and plugins' to the end of configure
* update copyright
* additional verbose messages
* fix some compiler warnings
* plugin_fifo: fix compile warnings
* drv_st2205u: add verbose error message
* version of linked serdisplib in long verbose output
* hhdtemp plugin fixed
* hddtemp autconf bug fixed
* compiler warnings fixed
* indentation
* unused array 'tokenNames' removed
* compiler warning removed
* initialize plugin on first use
* indentation
* initialize fifo on first use
* cosmetics
* new string operators eq ne gt ge lt le
* indentation
* typo fixed
* disable unimplemented ioctls
* distcheck bugs fixed
* button debugging changed
* widget_bar scale debugging changed
* add buttons to X11 sample conf
* clear pixel to BG_COL
* version bumped to 0.11.0
* netinfo plugin added to smoketest
* ported r847 (new Display CW12832 128x32) from volker_dev
* ported r846 from volker_dev
* ported r839 from volker_dev
* ported r838 from volker_dev
* ported r837 from volker_dev
* ported r835 from volker_dev
* ported r834 from volker_dev
* r828: 'additional verbose and info messages' ported from volker_dev
* includes for umask() ported from volker_dev (r827)
* handle config files with <CR><LF> (DOS line ending) ported from volker_dev (r826)
* netinfo plugin ported from volker_dev (r823)
* format characters in qprintf() with optional length (r821) ported from volker_dev
* short circuit evaluation ported from volker_dev (r820,r822,r824)
* Fixes a buffer underrun error and possible array index issues by mcarey@...
* ported r819 from volker_dev
* ported and modified r818 from volker_dev
* port r817 from volker_dev
* ported (modified) r815 from voker_dev
* cleanup & cosmetics
* driver for the PHAnderson serial-to-HD44780 adapter boards by Nicolas Weill
* w1retap plugin by Jonathan Hudson
* Enables/controls the backlight on the Pertelian LCD device by Jonathan Hudson
* hddtemp plugin from Scott Bronson
* keypad bug fixed
* patch from a.nielsen: inverted parport signals, support GPI on HD44780
* indent run
* fix lcd4linux driver for multible onboard controllers
* fix automake - check for libftdi
* update button_exec plugin - fix segfault
* update fifo plugin - untested
* Replace write to external variable usb_debug by calling usb_set_debug().
* adding GPI to picoLCD for reading button events  by Igor Mammedov <niallain@gmail.com>
* properties fixed
* Rebuild auto-generated autoconf stuff due to last change in drivers.m4.
* Add 'ULA200' driver when --with-drivers=all is specified
* strftime_tz patch from Bernhard Walle
* strftime_tz() plugin by Bernhard Walle
* ULA200 driver by Bernhard Walle
* indent
* Increased the maximum bps to 230400, if defined in <termios.h>.
* git-svn-id: https://ssl.bulix.org/svn/lcd4linux/trunk@889 3ae390bd-cb1e-0410-b409-cd5a39f66f1f
* D4D updates by Sven
* driver for 4D Systems serial displays by Sven Killig
* signed/unsigned mismatch
* added command functions
* added IRLCD driver by Jean-Philippe Civade
* 'autoreconf -f -i' run & config.rpath added
* forgot to add drv_picoLCDGraphic.c :-(
* picoLCDGraphic driver by Nicu Pavel added
* widget_keypad.h added to drv_HD44780
* indent run
* automake-1.10.1
* forgot include
* add signal handler, improve error check
* minor update
* Makefiles again, this time libiconv stuff is fixed
* fix error, introduced in r857
* update Makefiles
* clean up
* Add the year 2008 to the copyright
* Add basic FIFO plugin
* fixed autoconf
* update autoconf
* Add asterisk plugin, thanks to  mcarey@yostengineering.com
* fix typo, close #114
* update charset for Crystalfontz Model 632 and 634
* improve error messages
* reformat plugin_mpd - use c style comments
* correct code format (indent)
* correct code format (indent)
* new plugin: button_exec by Wolfgang Henerbichler
* add new driver (st2205) by Jeroen/Sprite_tm
* new plugin: button_exec by Wolfgang Henerbichler
* add keypad support for module lcm162
* add new driver (st2205) by Jeroen/Sprite_tm
* add signal handler to plugin_mpd
* plugin_mpd, update to v0.81, fix connection issues
* update mpd plugin to v0.8 - makefile needs some work now
* disable default emulator code
* minor updates
* added pingpong scrolling
* git-svn-id: https://ssl.bulix.org/svn/lcd4linux/trunk@848 3ae390bd-cb1e-0410-b409-cd5a39f66f1f
* automatic marquee patch from Manuel Grot
* RDTSC delay and inclusion of asm/msr.h removed
* driver for Pertelian display by Andy Powell
* GPS plugin V0.3 by michu
* GPS plugin V0.2 by michu
* email address changed
* Fix compilation with kernel 2.6.22 by Guillaume LECERF
* MPD plugin patch from michu
* GPS plugin update by michu
* indentation
* default for Brightness is 255
* LCD::brightness in drv_X11 corrected
* global variable for backlight color
* keypad support completed; new property brightness implemented
* maximum value for brightness is 8
* buttons for X11 driver (thanks to Volker Gering)
* Apple has no utsbuf.domainname (thanks to Volker Gering)
* strndup() replacement
* better usage output by Volker Gering
* Image driver libgd dependancy fix
* big MPD patch from Robert Buchholz
* gps plugin, code by michu / www.neophob.com
* compile error on m68k fixed (debian bug 405898)
* typo in LUIse driver fixed
* some compiler warnings fixed
* LCD4Linux-0.10.1-RC2
* ChangeLog updated
* svn2cl moved and optimized
* svn2cl.sh added
* link Cwlinux with keypad support
* patch from Fox: Fix a typo that prevent Bar display to work
* patch from FoX that fixes compilation with kernel 2.6.21
* MPD plugin patch from michu
* keypad and firmware support for Cwlinux (patch from Volker Gering)
* iowait,irq and softirq added (patch from Roman Jozsef)
* plugin_file patch from Mark Richards
* Crystalfontz keypad patch from kenson
* ported changeset 778 from branches/0.10.1
* imon::quantity and imon::status patch from Stefan Gmeiner / Christian Brueggemann
* ChangeLog refresh
* compiler warnings on x86_64 fixed
* ChangeLog refreshed
* lots of compiler warnings removed, C++-style comments removed, changed struc initialisation from 'field:value' to '.field=value'
* strstr() and substr() added to string plugin
* ChangeLog processed with svn2cl
* trunk version incremented to 0.10.2-SVN

### New Contributors
* @svenhoefer made their first contribution
* @TangoCash made their first contribution

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.10.1-RC1...0.11.0

## What's Changed in 0.10.1-RC1
* Version number changed to 0.10.1-RC1
* only use picoLCD if libusb is available
* svn version bumped manually
* autoconf & smoketest bugs fixed
* 'Electronic Assembly' driver by Stefan Gmeiner
* picoLCD driver bugs fixed
* drv_picoLCD was missing in distribution
* removed -SVN from version string
* try to add svn version
* indent
* new function 'decode()'
* removed unused variable
* keypad widget cleanup
* GPO cleanup
* minor fixes to evaluator
* dynamic properties for bars; new 'property_valid()' helper
* indent
* driver for picoLCD displays from mini-box.com by Nicu Pavel
* dynamic properties for keypad widget
* indent
* dynamic properties for Icon widget
* dynamic properties for GPO's
* removed DOS newlines again :-(
* indent
* changed version number from 0.10.1-CVS to 0.10.1-SVN
* new driver 'HD44780-I2C'
* Fix compilation of plugin_wireless.c with linux kernel 2.6.19 from Guillaume LECERF
* Patch from Robert Buchholz: NULL driver link error fixed, m4 files included in distribution
* patch from Mattia Jona-Lasinio for lcd-linux 0.12.0
* svn keywords
* removed some DOS newlines
* minor cosmetic changes and fixes
* re-bootstrapped
* svn keywords corrected
* svn keywords corrected
* added svn properties and keywords
* changed $Revision to $Rev
* removed all  tags
* svn properties fixed
* svn:ignore ported from .cvsignore
* switch from CVS to SVN
* [lcd4linux @ 2006-10-01 11:54:38 by reinelt]
* [lcd4linux @ 2006-09-29 04:48:21 by reinelt]
* image widget memory leaks fixed (thanks to Magne Torresen)
* [lcd4linux @ 2006-09-19 11:20:52 by entropy]
* [lcd4linux @ 2006-09-15 20:30:03 by siretart]
* [lcd4linux @ 2006-09-15 19:06:47 by entropy]
* [lcd4linux @ 2006-09-15 19:00:50 by entropy]
* [lcd4linux @ 2006-09-14 20:36:39 by entropy]
* [lcd4linux @ 2006-09-14 11:19:29 by entropy]
* [lcd4linux @ 2006-09-14 04:08:54 by reinelt]
* [lcd4linux @ 2006-09-14 03:49:14 by reinelt]
* [lcd4linux @ 2006-09-13 20:07:59 by entropy]
* [lcd4linux @ 2006-09-13 20:04:57 by entropy]
* [lcd4linux @ 2006-09-13 05:33:39 by reinelt]
* [lcd4linux @ 2006-09-08 19:00:46 by reinelt]
* [lcd4linux @ 2006-09-07 09:06:25 by reinelt]
* [lcd4linux @ 2006-09-06 05:22:09 by reinelt]
* [lcd4linux @ 2006-09-04 16:48:32 by siretart]
* [lcd4linux @ 2006-09-04 16:47:39 by siretart]
* [lcd4linux @ 2006-09-04 16:41:21 by siretart]
* [lcd4linux @ 2006-09-04 16:35:43 by siretart]
* [lcd4linux @ 2006-09-04 16:33:21 by siretart]
* [lcd4linux @ 2006-08-23 17:45:37 by harbaum]
* [lcd4linux @ 2006-08-23 05:05:40 by reinelt]
* [lcd4linux @ 2006-08-17 19:11:40 by harbaum]
* [lcd4linux @ 2006-08-17 05:06:26 by reinelt]
* [lcd4linux @ 2006-08-16 14:18:14 by reinelt]
* [lcd4linux @ 2006-08-15 17:28:27 by harbaum]
* [lcd4linux @ 2006-08-14 19:24:22 by harbaum]
* [lcd4linux @ 2006-08-14 05:54:03 by reinelt]
* [lcd4linux @ 2006-08-13 18:45:25 by harbaum]
* [lcd4linux @ 2006-08-13 18:14:03 by harbaum]
* [lcd4linux @ 2006-08-13 11:38:20 by reinelt]
* [lcd4linux @ 2006-08-13 09:53:10 by reinelt]
* [lcd4linux @ 2006-08-13 06:46:51 by reinelt]
* [lcd4linux @ 2006-08-11 11:59:29 by reinelt]
* [lcd4linux @ 2006-08-10 20:40:46 by reinelt]
* [lcd4linux @ 2006-08-10 19:06:52 by reinelt]
* [lcd4linux @ 2006-08-09 17:25:34 by harbaum]
* [lcd4linux @ 2006-08-08 20:16:28 by harbaum]
* [lcd4linux @ 2006-08-08 19:35:21 by reinelt]
* [lcd4linux @ 2006-08-08 19:28:18 by reinelt]
* [lcd4linux @ 2006-08-05 21:08:01 by harbaum]
* [lcd4linux @ 2006-07-31 03:48:09 by reinelt]
* [lcd4linux @ 2006-07-30 11:29:02 by lfcorreia]
* [lcd4linux @ 2006-07-30 11:16:27 by lfcorreia]
* [lcd4linux @ 2006-07-29 21:12:31 by lfcorreia]
* [lcd4linux @ 2006-07-29 21:04:43 by lfcorreia]
* [lcd4linux @ 2006-07-29 20:59:12 by lfcorreia]
* [lcd4linux @ 2006-07-19 01:57:01 by cmay]
* [lcd4linux @ 2006-07-19 01:48:11 by cmay]
* [lcd4linux @ 2006-07-19 01:35:31 by cmay]
* [lcd4linux @ 2006-07-18 17:04:55 by nicowallmeier]
* [lcd4linux @ 2006-07-14 20:15:11 by reinelt]
* [lcd4linux @ 2006-07-12 21:01:41 by reinelt]
* [lcd4linux @ 2006-07-12 20:47:51 by reinelt]
* [lcd4linux @ 2006-07-12 20:45:30 by reinelt]
* [lcd4linux @ 2006-06-25 15:13:00 by reinelt]
* [lcd4linux @ 2006-06-25 14:58:38 by reinelt]
* [lcd4linux @ 2006-06-21 05:12:43 by reinelt]
* [lcd4linux @ 2006-06-20 08:50:58 by reinelt]
* [lcd4linux @ 2006-06-19 15:12:54 by reinelt]
* [lcd4linux @ 2006-06-19 12:02:17 by reinelt]
* [lcd4linux @ 2006-06-19 11:54:08 by reinelt]
* [lcd4linux @ 2006-04-17 08:10:42 by reinelt]
* [lcd4linux @ 2006-04-15 05:22:52 by reinelt]
* [lcd4linux @ 2006-04-14 20:59:38 by harbaum]
* [lcd4linux @ 2006-04-09 17:46:14 by reinelt]
* [lcd4linux @ 2006-04-09 14:17:49 by reinelt]
* [lcd4linux @ 2006-03-29 08:57:58 by reinelt]
* [lcd4linux @ 2006-03-18 14:54:36 by harbaum]
* [lcd4linux @ 2006-03-10 18:06:52 by tooly-bln]
* [lcd4linux @ 2006-02-27 08:12:34 by reinelt]
* [lcd4linux @ 2006-02-27 07:53:52 by reinelt]
* [lcd4linux @ 2006-02-27 06:15:55 by reinelt]
* [lcd4linux @ 2006-02-27 06:14:46 by reinelt]
* [lcd4linux @ 2006-02-25 13:36:33 by geronet]
* [lcd4linux @ 2006-02-24 13:07:10 by geronet]
* [lcd4linux @ 2006-02-22 15:59:39 by cmay]
* [lcd4linux @ 2006-02-21 21:43:03 by harbaum]
* [lcd4linux @ 2006-02-21 15:55:59 by cmay]
* [lcd4linux @ 2006-02-21 15:52:30 by cmay]
* [lcd4linux @ 2006-02-21 05:50:34 by reinelt]
* [lcd4linux @ 2006-02-19 15:42:18 by reinelt]
* [lcd4linux @ 2006-02-19 15:37:38 by reinelt]
* [lcd4linux @ 2006-02-19 07:20:53 by reinelt]
* [lcd4linux @ 2006-02-12 14:32:24 by harbaum]
* [lcd4linux @ 2006-02-09 20:32:49 by harbaum]
* [lcd4linux @ 2006-02-08 04:55:03 by reinelt]
* [lcd4linux @ 2006-02-07 05:36:13 by reinelt]
* [lcd4linux @ 2006-02-06 06:29:30 by reinelt]
* [lcd4linux @ 2006-01-30 20:21:51 by harbaum]
* [lcd4linux @ 2006-01-30 12:53:07 by reinelt]
* [lcd4linux @ 2006-01-30 06:31:25 by reinelt]
* [lcd4linux @ 2006-01-30 06:25:48 by reinelt]
* [lcd4linux @ 2006-01-30 06:17:17 by reinelt]
* [lcd4linux @ 2006-01-30 06:11:36 by reinelt]
* [lcd4linux @ 2006-01-30 05:47:34 by reinelt]
* [lcd4linux @ 2006-01-28 15:36:17 by harbaum]
* [lcd4linux @ 2006-01-26 19:26:26 by harbaum]
* [lcd4linux @ 2006-01-23 06:17:18 by reinelt]
* [lcd4linux @ 2006-01-22 10:01:09 by reinelt]
* [lcd4linux @ 2006-01-22 09:16:05 by reinelt]
* [lcd4linux @ 2006-01-21 17:43:25 by reinelt]
* [lcd4linux @ 2006-01-21 15:25:02 by reinelt]
* [lcd4linux @ 2006-01-21 13:26:43 by reinelt]
* [lcd4linux @ 2006-01-21 09:40:20 by reinelt]
* [lcd4linux @ 2006-01-20 15:58:05 by reinelt]
* [lcd4linux @ 2006-01-20 15:43:25 by reinelt]
* [lcd4linux @ 2006-01-18 11:49:48 by reinelt]
* [lcd4linux @ 2006-01-16 15:39:58 by reinelt]
* [lcd4linux @ 2006-01-06 16:56:49 by tooly-bln]
* [lcd4linux @ 2006-01-06 08:12:19 by reinelt]
* [lcd4linux @ 2006-01-06 07:06:57 by reinelt]
* [lcd4linux @ 2006-01-05 19:27:26 by reinelt]
* [lcd4linux @ 2006-01-05 18:56:57 by reinelt]
* [lcd4linux @ 2006-01-05 15:53:45 by nicowallmeier]
* [lcd4linux @ 2006-01-03 13:20:05 by reinelt]
* [lcd4linux @ 2006-01-03 06:13:44 by reinelt]
* [lcd4linux @ 2005-12-20 07:07:43 by reinelt]
* [lcd4linux @ 2005-12-19 05:08:27 by reinelt]
* [lcd4linux @ 2005-12-18 16:18:34 by reinelt]
* [lcd4linux @ 2005-12-13 14:07:28 by reinelt]
* [lcd4linux @ 2005-12-12 09:08:08 by reinelt]
* [lcd4linux @ 2005-12-12 05:52:03 by reinelt]
* [lcd4linux @ 2005-12-11 14:55:28 by reinelt]
* [lcd4linux @ 2005-11-06 09:54:43 by reinelt]
* [lcd4linux @ 2005-11-06 09:17:20 by reinelt]
* [lcd4linux @ 2005-11-05 06:34:09 by reinelt]
* [lcd4linux @ 2005-11-05 06:26:51 by reinelt]
* [lcd4linux @ 2005-11-04 14:10:38 by reinelt]
* [lcd4linux @ 2005-11-04 05:39:33 by reinelt]
* [lcd4linux @ 2005-11-04 04:53:08 by reinelt]
* [lcd4linux @ 2005-11-04 04:44:52 by reinelt]
* [lcd4linux @ 2005-10-02 07:58:48 by reinelt]
* [lcd4linux @ 2005-09-14 15:08:32 by reinelt]
* [lcd4linux @ 2005-09-07 06:51:44 by reinelt]
* [lcd4linux @ 2005-09-02 05:27:08 by reinelt]
* [lcd4linux @ 2005-08-27 07:02:25 by reinelt]
* [lcd4linux @ 2005-08-22 05:44:43 by reinelt]
* [lcd4linux @ 2005-08-21 08:18:56 by reinelt]
* [lcd4linux @ 2005-08-20 10:10:13 by reinelt]
* [lcd4linux @ 2005-07-06 04:40:18 by reinelt]
* [lcd4linux @ 2005-06-19 17:57:06 by reinelt]
* [lcd4linux @ 2005-06-15 05:24:35 by reinelt]
* [lcd4linux @ 2005-06-13 03:43:46 by reinelt]
* [lcd4linux @ 2005-06-13 03:38:25 by reinelt]
* [lcd4linux @ 2005-06-11 10:57:45 by reinelt]
* [lcd4linux @ 2005-06-11 04:14:05 by reinelt]
* [lcd4linux @ 2005-06-10 11:22:28 by reinelt]
* [lcd4linux @ 2005-06-10 09:51:44 by reinelt]
* [lcd4linux @ 2005-06-10 05:02:28 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.10.0...0.10.1-RC1

## What's Changed in 0.10.0
* [lcd4linux @ 2005-06-10 05:00:36 by reinelt]
* [lcd4linux @ 2005-06-10 04:54:10 by reinelt]
* [lcd4linux @ 2005-06-10 04:53:44 by reinelt]
* [lcd4linux @ 2005-06-09 17:41:47 by reinelt]
* [lcd4linux @ 2005-06-06 09:24:07 by reinelt]
* [lcd4linux @ 2005-06-03 17:04:52 by reinelt]
* [lcd4linux @ 2005-06-03 04:45:57 by reinelt]
* [lcd4linux @ 2005-06-01 12:50:25 by reinelt]
* [lcd4linux @ 2005-06-01 12:46:31 by reinelt]
* [lcd4linux @ 2005-06-01 12:09:11 by reinelt]
* [lcd4linux @ 2005-06-01 11:17:54 by pk_richman]
* [lcd4linux @ 2005-05-31 21:30:56 by lfcorreia]
* [lcd4linux @ 2005-05-31 21:28:42 by lfcorreia]
* [lcd4linux @ 2005-05-31 21:26:56 by lfcorreia]
* [lcd4linux @ 2005-05-31 21:06:36 by lfcorreia]
* [lcd4linux @ 2005-05-31 20:42:54 by lfcorreia]
* [lcd4linux @ 2005-05-28 09:08:30 by reinelt]
* [lcd4linux @ 2005-05-28 09:06:14 by reinelt]
* [lcd4linux @ 2005-05-13 05:44:44 by reinelt]
* [lcd4linux @ 2005-05-13 05:43:25 by reinelt]
* [lcd4linux @ 2005-05-12 14:55:47 by reinelt]
* [lcd4linux @ 2005-05-12 05:52:43 by reinelt]
* [lcd4linux @ 2005-05-11 04:27:49 by reinelt]
* [lcd4linux @ 2005-05-10 13:20:10 by reinelt]
* [lcd4linux @ 2005-05-08 04:32:43 by reinelt]
* [lcd4linux @ 2005-05-06 06:41:53 by reinelt]
* [lcd4linux @ 2005-05-06 06:37:34 by reinelt]
* [lcd4linux @ 2005-05-06 05:40:02 by reinelt]
* [lcd4linux @ 2005-05-06 05:36:58 by reinelt]
* [lcd4linux @ 2005-05-05 08:36:12 by reinelt]
* [lcd4linux @ 2005-05-04 07:18:44 by obconseil]
* [lcd4linux @ 2005-05-04 06:13:05 by reinelt]
* [lcd4linux @ 2005-05-04 05:42:37 by reinelt]
* [lcd4linux @ 2005-05-04 05:22:12 by reinelt]
* [lcd4linux @ 2005-05-03 11:13:23 by reinelt]
* [lcd4linux @ 2005-05-02 10:29:20 by reinelt]
* [lcd4linux @ 2005-05-02 05:15:46 by reinelt]
* [lcd4linux @ 2005-04-30 06:02:09 by reinelt]
* [lcd4linux @ 2005-04-24 05:27:09 by reinelt]
* [lcd4linux @ 2005-04-24 04:41:28 by reinelt]
* [lcd4linux @ 2005-04-24 04:33:46 by reinelt]
* [lcd4linux @ 2005-04-20 05:49:21 by reinelt]
* [lcd4linux @ 2005-04-09 07:36:42 by reinelt]
* [lcd4linux @ 2005-04-05 06:57:39 by reinelt]
* [lcd4linux @ 2005-04-05 05:12:48 by reinelt]
* [lcd4linux @ 2005-04-05 04:46:06 by reinelt]
* [lcd4linux @ 2005-04-04 20:11:14 by nicowallmeier]
* [lcd4linux @ 2005-04-03 07:07:43 by reinelt]
* [lcd4linux @ 2005-04-02 05:28:58 by reinelt]
* [lcd4linux @ 2005-04-01 05:16:04 by reinelt]
* [lcd4linux @ 2005-03-30 04:57:50 by reinelt]
* [lcd4linux @ 2005-03-28 22:29:23 by reinelt]
* [lcd4linux @ 2005-03-28 19:39:14 by reinelt]
* [lcd4linux @ 2005-03-25 15:44:43 by reinelt]
* [lcd4linux @ 2005-03-23 12:23:35 by reinelt]
* [lcd4linux @ 2005-02-24 07:07:55 by reinelt]
* [lcd4linux @ 2005-02-24 07:06:45 by reinelt]
* [lcd4linux @ 2005-02-24 06:51:40 by reinelt]
* [lcd4linux @ 2005-01-30 06:43:22 by reinelt]
* [lcd4linux @ 2005-01-29 09:30:56 by reinelt]
* [lcd4linux @ 2005-01-22 22:57:57 by reinelt]
* [lcd4linux @ 2005-01-22 12:44:41 by reinelt]
* [lcd4linux @ 2005-01-18 06:30:21 by reinelt]
* [lcd4linux @ 2005-01-17 06:38:48 by reinelt]
* [lcd4linux @ 2005-01-17 06:29:24 by reinelt]
* [lcd4linux @ 2005-01-15 13:13:57 by reinelt]
* [lcd4linux @ 2005-01-15 13:13:21 by reinelt]
* [lcd4linux @ 2005-01-15 13:10:13 by reinelt]
* [lcd4linux @ 2005-01-11 10:25:26 by reinelt]
* [lcd4linux @ 2005-01-11 10:19:33 by reinelt]
* [lcd4linux @ 2005-01-09 10:57:25 by reinelt]
* [lcd4linux @ 2005-01-09 10:53:22 by reinelt]
* [lcd4linux @ 2005-01-06 16:54:53 by reinelt]
* [lcd4linux @ 2004-12-22 20:24:00 by reinelt]
* [lcd4linux @ 2004-11-30 05:01:25 by reinelt]
* [lcd4linux @ 2004-11-29 04:42:06 by reinelt]
* [lcd4linux @ 2004-11-28 15:50:24 by reinelt]
* [lcd4linux @ 2004-10-17 09:24:31 by reinelt]
* [lcd4linux @ 2004-10-02 09:31:55 by reinelt]
* [lcd4linux @ 2004-09-24 21:40:52 by reinelt]
* [lcd4linux @ 2004-09-19 09:31:19 by reinelt]
* [lcd4linux @ 2004-09-18 15:58:57 by reinelt]
* [lcd4linux @ 2004-09-18 10:57:29 by reinelt]
* [lcd4linux @ 2004-09-18 09:48:29 by reinelt]
* [lcd4linux @ 2004-09-18 08:22:59 by reinelt]
* [lcd4linux @ 2004-08-30 12:48:52 by rjoco77]
* [lcd4linux @ 2004-08-29 20:07:55 by reinelt]
* [lcd4linux @ 2004-08-29 13:03:40 by reinelt]
* [lcd4linux @ 2004-07-14 04:44:44 by reinelt]
* [lcd4linux @ 2004-06-29 04:49:30 by reinelt]
* [lcd4linux @ 2004-06-26 12:04:59 by reinelt]
* [lcd4linux @ 2004-06-26 09:27:20 by reinelt]
* [lcd4linux @ 2004-06-26 06:12:14 by reinelt]
* [lcd4linux @ 2004-06-24 20:18:08 by nicowallmeier]
* [lcd4linux @ 2004-06-20 10:12:27 by reinelt]
* [lcd4linux @ 2004-06-20 10:09:52 by reinelt]
* [lcd4linux @ 2004-06-19 08:20:19 by reinelt]
* [lcd4linux @ 2004-06-17 10:58:57 by reinelt]
* [lcd4linux @ 2004-06-17 06:23:39 by reinelt]
* [lcd4linux @ 2004-06-13 01:12:52 by reinelt]
* [lcd4linux @ 2004-06-09 06:40:29 by reinelt]
* [lcd4linux @ 2004-06-08 21:46:38 by reinelt]
* [lcd4linux @ 2004-06-08 12:35:24 by reinelt]
* [lcd4linux @ 2004-06-07 07:02:13 by reinelt]
* [lcd4linux @ 2004-06-07 06:56:55 by reinelt]
* [lcd4linux @ 2004-06-06 06:51:59 by reinelt]
* [lcd4linux @ 2004-06-05 14:56:48 by reinelt]
* [lcd4linux @ 2004-06-05 06:41:39 by reinelt]
* [lcd4linux @ 2004-06-05 06:13:11 by reinelt]
* [lcd4linux @ 2004-06-02 10:09:22 by reinelt]
* [lcd4linux @ 2004-06-02 09:41:19 by reinelt]
* [lcd4linux @ 2004-06-02 05:56:25 by reinelt]
* [lcd4linux @ 2004-06-02 05:35:55 by reinelt]
* [lcd4linux @ 2004-06-02 05:27:59 by reinelt]
* [lcd4linux @ 2004-06-02 05:14:16 by reinelt]
* [lcd4linux @ 2004-06-01 06:45:28 by reinelt]
* [lcd4linux @ 2004-06-01 06:04:25 by reinelt]
* [lcd4linux @ 2004-05-31 21:23:16 by reinelt]
* [lcd4linux @ 2004-05-31 21:05:13 by reinelt]
* [lcd4linux @ 2004-05-31 16:39:05 by reinelt]
* [lcd4linux @ 2004-05-31 06:27:34 by reinelt]
* [lcd4linux @ 2004-05-31 06:24:42 by reinelt]
* [lcd4linux @ 2004-05-31 05:38:02 by reinelt]
* [lcd4linux @ 2004-05-31 01:31:01 by andy-b]
* [lcd4linux @ 2004-05-30 08:25:50 by reinelt]
* [lcd4linux @ 2004-05-29 23:30:20 by reinelt]
* [lcd4linux @ 2004-05-29 15:53:28 by reinelt]
* [lcd4linux @ 2004-05-29 01:07:56 by reinelt]
* [lcd4linux @ 2004-05-29 00:27:14 by reinelt]
* [lcd4linux @ 2004-05-28 14:38:10 by reinelt]
* [lcd4linux @ 2004-05-28 14:36:10 by reinelt]
* [lcd4linux @ 2004-05-28 13:51:41 by reinelt]
* [lcd4linux @ 2004-05-27 06:29:29 by nicowallmeier]
* [lcd4linux @ 2004-05-27 03:49:41 by reinelt]
* [lcd4linux @ 2004-05-27 03:39:47 by reinelt]
* [lcd4linux @ 2004-05-26 11:37:35 by reinelt]
* [lcd4linux @ 2004-05-26 05:03:24 by reinelt]
* [lcd4linux @ 2004-05-25 19:54:11 by reinelt]
* [lcd4linux @ 2004-05-25 19:47:11 by reinelt]
* [lcd4linux @ 2004-05-25 14:28:46 by reinelt]
* [lcd4linux @ 2004-05-25 14:27:21 by reinelt]
* [lcd4linux @ 2004-05-25 14:26:28 by reinelt]
* [lcd4linux @ 2004-05-25 06:42:31 by reinelt]
* [lcd4linux @ 2004-05-23 08:58:30 by reinelt]
* [lcd4linux @ 2004-05-22 18:30:01 by reinelt]
* [lcd4linux @ 2004-05-22 04:23:49 by reinelt]
* [lcd4linux @ 2004-05-22 04:21:02 by reinelt]
* [lcd4linux @ 2004-05-20 07:47:51 by reinelt]
* [lcd4linux @ 2004-05-20 07:14:35 by reinelt]
* [lcd4linux @ 2004-05-19 05:38:25 by reinelt]
* [lcd4linux @ 2004-05-19 05:23:25 by reinelt]
* [lcd4linux @ 2004-05-09 05:41:41 by reinelt]
* [lcd4linux @ 2004-04-17 13:03:34 by nicowallmeier]
* [lcd4linux @ 2004-04-12 11:12:24 by reinelt]
* [lcd4linux @ 2004-04-12 05:59:24 by reinelt]
* [lcd4linux @ 2004-04-12 05:14:42 by reinelt]
* [lcd4linux @ 2004-04-12 04:55:59 by reinelt]
* [lcd4linux @ 2004-04-11 17:37:09 by reinelt]
* [lcd4linux @ 2004-04-09 06:23:28 by reinelt]
* [lcd4linux @ 2004-04-09 06:09:54 by reinelt]
* [lcd4linux @ 2004-04-08 11:59:26 by reinelt]
* [lcd4linux @ 2004-04-08 10:48:23 by reinelt]
* [lcd4linux @ 2004-04-07 08:29:05 by hejl]
* [lcd4linux @ 2004-03-21 22:05:53 by reinelt]
* [lcd4linux @ 2004-03-20 23:09:01 by reinelt]
* [lcd4linux @ 2004-03-20 11:49:40 by reinelt]
* [lcd4linux @ 2004-03-20 07:31:32 by reinelt]
* [lcd4linux @ 2004-03-19 09:17:46 by reinelt]
* [lcd4linux @ 2004-03-19 06:37:47 by reinelt]
* [lcd4linux @ 2004-03-14 07:14:05 by reinelt]
* [lcd4linux @ 2004-03-14 07:11:42 by reinelt]
* [lcd4linux @ 2004-03-14 06:07:33 by reinelt]
* [lcd4linux @ 2004-03-13 19:06:01 by reinelt]
* [lcd4linux @ 2004-03-13 14:58:15 by nicowallmeier]
* [lcd4linux @ 2004-03-13 14:55:14 by nicowallmeier]
* [lcd4linux @ 2004-03-13 06:55:29 by reinelt]
* [lcd4linux @ 2004-03-13 06:49:20 by reinelt]
* [lcd4linux @ 2004-03-12 13:58:14 by reinelt]
* [lcd4linux @ 2004-03-11 06:39:58 by reinelt]
* [lcd4linux @ 2004-03-10 07:16:15 by reinelt]
* [lcd4linux @ 2004-03-08 18:46:21 by hejl]
* [lcd4linux @ 2004-03-08 18:45:52 by hejl]
* [lcd4linux @ 2004-03-08 16:26:26 by reinelt]
* [lcd4linux @ 2004-03-08 04:33:08 by reinelt]
* [lcd4linux @ 2004-03-06 20:31:16 by reinelt]
* [lcd4linux @ 2004-03-03 08:40:07 by hejl]
* [lcd4linux @ 2004-03-03 04:44:16 by reinelt]
* [lcd4linux @ 2004-03-03 03:47:04 by reinelt]
* [lcd4linux @ 2004-03-03 03:41:02 by reinelt]
* [lcd4linux @ 2004-03-01 04:29:51 by reinelt]
* [lcd4linux @ 2004-02-29 17:09:53 by reinelt]
* [lcd4linux @ 2004-02-29 14:30:59 by reinelt]
* [lcd4linux @ 2004-02-27 07:06:24 by reinelt]
* [lcd4linux @ 2004-02-27 06:07:55 by reinelt]
* [lcd4linux @ 2004-02-26 21:42:45 by reinelt]
* [lcd4linux @ 2004-02-24 06:00:22 by reinelt]
* [lcd4linux @ 2004-02-24 05:54:57 by reinelt]
* [lcd4linux @ 2004-02-23 06:44:27 by reinelt]
* [lcd4linux @ 2004-02-22 17:35:41 by reinelt]
* [lcd4linux @ 2004-02-18 14:45:42 by nicowallmeier]
* [lcd4linux @ 2004-02-18 06:39:20 by reinelt]
* [lcd4linux @ 2004-02-17 05:37:20 by reinelt]
* [lcd4linux @ 2004-02-16 13:03:37 by reinelt]
* [lcd4linux @ 2004-02-16 08:19:44 by reinelt]
* [lcd4linux @ 2004-02-15 21:43:43 by reinelt]
* [lcd4linux @ 2004-02-15 08:22:47 by reinelt]
* [lcd4linux @ 2004-02-15 07:23:04 by reinelt]
* [lcd4linux @ 2004-02-14 12:07:27 by nicowallmeier]
* [lcd4linux @ 2004-02-14 11:56:11 by reinelt]
* [lcd4linux @ 2004-02-14 10:09:50 by reinelt]
* [lcd4linux @ 2004-02-10 07:42:35 by reinelt]
* [lcd4linux @ 2004-02-10 06:54:38 by reinelt]
* [lcd4linux @ 2004-02-09 19:49:38 by nicowallmeier]
* [lcd4linux @ 2004-02-07 13:45:23 by reinelt]
* [lcd4linux @ 2004-02-05 23:58:18 by mkeil]
* [lcd4linux @ 2004-02-05 07:10:23 by reinelt]
* [lcd4linux @ 2004-02-04 19:11:44 by reinelt]
* [lcd4linux @ 2004-02-04 19:10:51 by reinelt]
* [lcd4linux @ 2004-02-02 05:22:16 by reinelt]
* [lcd4linux @ 2004-02-01 19:37:40 by reinelt]
* [lcd4linux @ 2004-02-01 18:08:50 by reinelt]
* [lcd4linux @ 2004-02-01 11:51:22 by hejl]
* [lcd4linux @ 2004-02-01 08:05:12 by reinelt]
* [lcd4linux @ 2004-01-30 20:57:55 by reinelt]
* [lcd4linux @ 2004-01-30 07:12:35 by reinelt]
* [lcd4linux @ 2004-01-29 05:55:30 by reinelt]
* [lcd4linux @ 2004-01-29 05:53:47 by reinelt]
* [lcd4linux @ 2004-01-29 04:40:02 by reinelt]
* [lcd4linux @ 2004-01-28 06:43:31 by reinelt]
* [lcd4linux @ 2004-01-27 08:13:39 by reinelt]
* [lcd4linux @ 2004-01-27 06:34:14 by reinelt]
* [lcd4linux @ 2004-01-27 05:06:10 by reinelt]
* [lcd4linux @ 2004-01-27 04:48:57 by reinelt]
* [lcd4linux @ 2004-01-25 05:30:08 by reinelt]
* [lcd4linux @ 2004-01-23 07:04:03 by reinelt]
* [lcd4linux @ 2004-01-23 04:53:23 by reinelt]
* [lcd4linux @ 2004-01-22 08:55:30 by reinelt]
* [lcd4linux @ 2004-01-22 07:57:45 by reinelt]
* [lcd4linux @ 2004-01-21 14:29:03 by reinelt]
* [lcd4linux @ 2004-01-21 12:36:19 by reinelt]
* [lcd4linux @ 2004-01-21 11:32:48 by reinelt]
* [lcd4linux @ 2004-01-21 11:31:23 by reinelt]
* [lcd4linux @ 2004-01-21 10:48:17 by reinelt]
* [lcd4linux @ 2004-01-21 06:39:27 by reinelt]
* [lcd4linux @ 2004-01-20 15:32:48 by reinelt]
* [lcd4linux @ 2004-01-20 14:35:38 by reinelt]
* [lcd4linux @ 2004-01-20 14:26:09 by reinelt]
* [lcd4linux @ 2004-01-20 14:25:12 by reinelt]
* [lcd4linux @ 2004-01-20 12:45:47 by reinelt]
* [lcd4linux @ 2004-01-20 05:36:59 by reinelt]
* [lcd4linux @ 2004-01-20 04:51:39 by reinelt]
* [lcd4linux @ 2004-01-18 21:25:16 by reinelt]
* [lcd4linux @ 2004-01-18 09:01:45 by reinelt]
* [lcd4linux @ 2004-01-18 06:54:08 by reinelt]
* [lcd4linux @ 2004-01-16 11:12:26 by reinelt]
* [lcd4linux @ 2004-01-16 10:09:49 by mkeil]
* [lcd4linux @ 2004-01-16 07:26:25 by reinelt]
* [lcd4linux @ 2004-01-16 05:04:53 by reinelt]
* [lcd4linux @ 2004-01-15 07:47:02 by reinelt]
* [lcd4linux @ 2004-01-15 04:32:14 by reinelt]
* [lcd4linux @ 2004-01-15 04:29:45 by reinelt]
* [lcd4linux @ 2004-01-14 11:33:00 by reinelt]
* [lcd4linux @ 2004-01-13 10:03:01 by reinelt]
* [lcd4linux @ 2004-01-13 08:18:07 by reinelt]
* [lcd4linux @ 2004-01-12 03:51:01 by reinelt]
* [lcd4linux @ 2004-01-11 18:26:02 by reinelt]
* [lcd4linux @ 2004-01-11 09:26:15 by reinelt]
* [lcd4linux @ 2004-01-10 20:22:33 by reinelt]
* [lcd4linux @ 2004-01-10 17:45:26 by reinelt]
* [lcd4linux @ 2004-01-10 17:36:56 by reinelt]
* [lcd4linux @ 2004-01-10 17:34:40 by reinelt]
* [lcd4linux @ 2004-01-10 10:20:22 by reinelt]
* [lcd4linux @ 2004-01-09 17:03:06 by reinelt]
* [lcd4linux @ 2004-01-09 04:16:06 by reinelt]
* [lcd4linux @ 2004-01-08 06:00:28 by reinelt]
* [lcd4linux @ 2004-01-08 05:28:12 by reinelt]
* [lcd4linux @ 2004-01-07 10:15:41 by reinelt]
* [lcd4linux @ 2004-01-06 23:01:37 by reinelt]
* [lcd4linux @ 2004-01-06 22:33:13 by reinelt]
* [lcd4linux @ 2004-01-06 21:14:51 by reinelt]
* [lcd4linux @ 2004-01-06 18:22:41 by reinelt]
* [lcd4linux @ 2004-01-06 17:56:43 by reinelt]
* [lcd4linux @ 2004-01-06 17:37:00 by reinelt]
* [lcd4linux @ 2004-01-06 17:33:45 by reinelt]
* [lcd4linux @ 2004-01-06 15:19:12 by reinelt]
* [lcd4linux @ 2004-01-05 11:57:38 by reinelt]
* [lcd4linux @ 2004-01-02 14:20:15 by reinelt]
* [lcd4linux @ 2004-01-02 14:18:54 by reinelt]
* [lcd4linux @ 2003-12-19 06:27:33 by reinelt]
* [lcd4linux @ 2003-12-19 05:56:13 by reinelt]
* [lcd4linux @ 2003-12-19 05:50:34 by reinelt]
* [lcd4linux @ 2003-12-19 05:49:23 by reinelt]
* [lcd4linux @ 2003-12-19 05:35:13 by reinelt]
* [lcd4linux @ 2003-12-01 07:08:50 by reinelt]
* [lcd4linux @ 2003-11-30 16:18:36 by reinelt]
* [lcd4linux @ 2003-11-28 18:34:55 by nicowallmeier]
* [lcd4linux @ 2003-11-24 11:34:54 by reinelt]
* [lcd4linux @ 2003-11-16 09:45:49 by reinelt]
* [lcd4linux @ 2003-11-14 05:59:37 by reinelt]
* [lcd4linux @ 2003-11-12 05:42:35 by reinelt]
* [lcd4linux @ 2003-11-11 04:40:20 by reinelt]
* [lcd4linux @ 2003-11-11 04:30:41 by reinelt]
* [lcd4linux @ 2003-10-27 09:05:42 by reinelt]
* [lcd4linux @ 2003-10-22 04:32:25 by reinelt]
* [lcd4linux @ 2003-10-22 04:19:16 by reinelt]
* [lcd4linux @ 2003-10-12 06:08:28 by nicowallmeier]
* [lcd4linux @ 2003-10-12 04:46:19 by reinelt]
* [lcd4linux @ 2003-10-11 06:01:52 by reinelt]
* [lcd4linux @ 2003-10-08 14:21:10 by reinelt]
* [lcd4linux @ 2003-10-08 13:39:53 by andy-b]
* [lcd4linux @ 2003-10-08 06:48:47 by nicowallmeier]
* [lcd4linux @ 2003-10-08 06:45:00 by nicowallmeier]
* [lcd4linux @ 2003-10-07 04:12:38 by reinelt]
* [lcd4linux @ 2003-10-06 05:51:15 by reinelt]
* [lcd4linux @ 2003-10-06 05:47:27 by reinelt]
* [lcd4linux @ 2003-10-06 04:34:06 by reinelt]
* [lcd4linux @ 2003-10-06 04:33:06 by reinelt]
* [lcd4linux @ 2003-10-06 04:30:43 by reinelt]
* [lcd4linux @ 2003-10-05 17:58:50 by reinelt]
* [lcd4linux @ 2003-10-04 07:54:17 by reinelt]
* [lcd4linux @ 2003-10-03 03:53:12 by reinelt]
* [lcd4linux @ 2003-10-03 03:51:14 by reinelt]
* [lcd4linux @ 2003-09-29 06:58:36 by reinelt]
* [lcd4linux @ 2003-09-29 06:12:56 by reinelt]
* [lcd4linux @ 2003-09-21 06:43:02 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.9.12...0.10.0

## What's Changed in 0.9.12
* [lcd4linux @ 2003-09-19 08:40:32 by reinelt]
* [lcd4linux @ 2003-09-19 03:51:29 by reinelt]
* [lcd4linux @ 2003-09-13 07:20:51 by reinelt]
* [lcd4linux @ 2003-09-13 06:45:43 by reinelt]
* [lcd4linux @ 2003-09-13 06:20:39 by reinelt]
* [lcd4linux @ 2003-09-11 15:05:24 by reinelt]
* [lcd4linux @ 2003-09-11 04:09:52 by reinelt]
* [lcd4linux @ 2003-09-10 15:59:39 by reinelt]
* [lcd4linux @ 2003-09-10 15:09:21 by reinelt]
* [lcd4linux @ 2003-09-10 14:01:52 by reinelt]
* [lcd4linux @ 2003-09-10 08:37:09 by reinelt]
* [lcd4linux @ 2003-09-10 03:48:22 by reinelt]
* [lcd4linux @ 2003-09-09 11:47:47 by reinelt]
* [lcd4linux @ 2003-09-09 06:54:43 by reinelt]
* [lcd4linux @ 2003-09-09 05:30:33 by reinelt]
* [lcd4linux @ 2003-09-01 07:07:03 by reinelt]
* [lcd4linux @ 2003-09-01 04:09:34 by reinelt]
* [lcd4linux @ 2003-08-24 05:28:31 by reinelt]
* [lcd4linux @ 2003-08-24 05:17:58 by reinelt]
* [lcd4linux @ 2003-08-24 04:31:56 by reinelt]
* [lcd4linux @ 2003-08-22 03:45:08 by reinelt]
* [lcd4linux @ 2003-08-20 05:26:43 by reinelt]
* [lcd4linux @ 2003-08-19 05:23:55 by reinelt]
* [lcd4linux @ 2003-08-19 04:28:41 by reinelt]
* [lcd4linux @ 2003-08-17 16:37:39 by reinelt]
* [lcd4linux @ 2003-08-17 12:11:58 by reinelt]
* [lcd4linux @ 2003-08-17 08:25:30 by reinelt]
* [lcd4linux @ 2003-08-17 06:57:04 by reinelt]
* [lcd4linux @ 2003-08-17 04:38:57 by reinelt]
* [lcd4linux @ 2003-08-16 07:31:35 by reinelt]
* [lcd4linux @ 2003-08-15 07:54:07 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.9.11...0.9.12

## What's Changed in 0.9.11
* [lcd4linux @ 2003-08-14 04:59:30 by reinelt]
* [lcd4linux @ 2003-08-14 03:48:54 by reinelt]
* [lcd4linux @ 2003-08-14 03:47:40 by reinelt]
* [lcd4linux @ 2003-08-12 05:10:31 by reinelt]
* [lcd4linux @ 2003-08-11 04:51:28 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.9.10...0.9.11

## What's Changed in 0.9.10
* [lcd4linux @ 2003-08-08 23:11:34 by reinelt]
* [lcd4linux @ 2003-08-08 23:08:38 by reinelt]
* [lcd4linux @ 2003-08-08 08:05:23 by reinelt]
* [lcd4linux @ 2003-08-08 06:58:06 by reinelt]
* [lcd4linux @ 2003-08-08 05:42:51 by reinelt]
* [lcd4linux @ 2003-08-01 05:15:42 by reinelt]
* [lcd4linux @ 2003-07-29 04:56:13 by reinelt]
* [lcd4linux @ 2003-07-28 08:22:17 by reinelt]
* [lcd4linux @ 2003-07-24 04:48:09 by reinelt]
* [lcd4linux @ 2003-07-21 06:34:14 by reinelt]
* [lcd4linux @ 2003-07-21 06:10:11 by reinelt]
* [lcd4linux @ 2003-07-21 05:56:48 by reinelt]
* [lcd4linux @ 2003-07-19 09:42:42 by reinelt]
* [lcd4linux @ 2003-07-18 04:43:14 by reinelt]
* [lcd4linux @ 2003-06-26 05:31:16 by reinelt]
* [lcd4linux @ 2003-06-22 19:31:24 by reinelt]
* [lcd4linux @ 2003-06-21 05:46:18 by reinelt]
* [lcd4linux @ 2003-06-13 06:35:56 by reinelt]
* [lcd4linux @ 2003-06-13 05:11:10 by reinelt]
* [lcd4linux @ 2003-05-19 05:55:17 by reinelt]
* [lcd4linux @ 2003-05-14 06:17:39 by reinelt]
* [lcd4linux @ 2003-04-12 16:23:10 by reinelt]
* [lcd4linux @ 2003-04-07 06:02:58 by reinelt]
* [lcd4linux @ 2003-04-04 06:01:59 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.9.9...0.9.10

## What's Changed in 0.9.9
* [lcd4linux @ 2003-02-27 07:48:57 by reinelt]
* [lcd4linux @ 2003-02-27 07:43:10 by reinelt]
* [lcd4linux @ 2003-02-24 04:50:57 by reinelt]
* [lcd4linux @ 2003-02-22 07:53:09 by reinelt]
* [lcd4linux @ 2003-02-22 07:23:24 by reinelt]
* [lcd4linux @ 2003-02-18 06:13:44 by reinelt]
* [lcd4linux @ 2003-02-17 06:06:12 by reinelt]
* [lcd4linux @ 2003-02-17 04:27:58 by reinelt]
* [lcd4linux @ 2003-02-13 10:40:17 by reinelt]
* [lcd4linux @ 2003-02-05 04:31:38 by reinelt]
* [lcd4linux @ 2003-01-12 06:51:27 by reinelt]
* [lcd4linux @ 2002-12-08 07:36:06 by reinelt]
* [lcd4linux @ 2002-12-06 07:38:43 by reinelt]
* [lcd4linux @ 2002-12-05 19:23:01 by reinelt]
* [lcd4linux @ 2002-12-05 19:12:47 by reinelt]
* [lcd4linux @ 2002-12-05 19:09:57 by reinelt]
* [lcd4linux @ 2002-09-12 05:24:54 by reinelt]
* [lcd4linux @ 2002-09-11 05:32:35 by reinelt]
* [lcd4linux @ 2002-09-11 05:16:32 by reinelt]
* [lcd4linux @ 2002-08-30 03:54:01 by reinelt]
* [lcd4linux @ 2002-08-22 05:51:36 by reinelt]
* [lcd4linux @ 2002-08-21 06:09:53 by reinelt]
* [lcd4linux @ 2002-08-19 10:51:06 by reinelt]
* [lcd4linux @ 2002-08-19 09:43:43 by reinelt]
* [lcd4linux @ 2002-08-19 09:30:18 by reinelt]
* [lcd4linux @ 2002-08-19 09:11:34 by reinelt]
* [lcd4linux @ 2002-08-19 07:52:19 by reinelt]
* [lcd4linux @ 2002-08-19 07:36:29 by reinelt]
* [lcd4linux @ 2002-08-19 04:41:20 by reinelt]
* [lcd4linux @ 2002-08-18 08:11:11 by reinelt]
* [lcd4linux @ 2002-08-17 14:14:21 by reinelt]
* [lcd4linux @ 2002-08-17 13:10:22 by reinelt]
* [lcd4linux @ 2002-08-17 12:54:08 by reinelt]
* [lcd4linux @ 2002-04-30 07:20:15 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.99...0.9.9

## What's Changed in 0.99
* [lcd4linux @ 2002-04-29 11:00:25 by reinelt]
* [lcd4linux @ 2001-09-14 05:57:06 by reinelt]
* [lcd4linux @ 2001-09-13 07:40:57 by reinelt]
* [lcd4linux @ 2001-09-12 06:17:22 by reinelt]
* [lcd4linux @ 2001-09-12 05:58:16 by reinelt]
* [lcd4linux @ 2001-09-12 05:37:22 by reinelt]
* [lcd4linux @ 2001-09-11 06:43:43 by reinelt]
* [lcd4linux @ 2001-09-11 05:31:37 by reinelt]
* [lcd4linux @ 2001-09-10 13:55:53 by reinelt]
* [lcd4linux @ 2001-09-09 12:26:03 by reinelt]
* [lcd4linux @ 2001-09-07 05:58:44 by reinelt]
* [lcd4linux @ 2001-09-05 09:38:52 by reinelt]
* [lcd4linux @ 2001-08-08 05:40:24 by reinelt]
* [lcd4linux @ 2001-08-05 17:13:29 by reinelt]
* [lcd4linux @ 2001-06-04 07:49:58 by reinelt]
* [lcd4linux @ 2001-05-31 10:26:41 by ltoetsch]
* [lcd4linux @ 2001-05-27 17:32:35 by reinelt]
* [lcd4linux @ 2001-05-27 07:19:28 by reinelt]
* [lcd4linux @ 2001-05-26 06:51:28 by reinelt]
* [lcd4linux @ 2001-05-06 10:01:27 by reinelt]
* [lcd4linux @ 2001-04-27 05:04:57 by reinelt]
* [lcd4linux @ 2001-03-24 09:26:25 by reinelt]
* [lcd4linux @ 2001-03-24 09:04:19 by reinelt]
* [lcd4linux @ 2001-03-17 11:44:10 by ltoetsch]
* [lcd4linux @ 2001-03-17 11:11:31 by ltoetsch]
* [lcd4linux @ 2001-03-16 16:40:17 by ltoetsch]
* [lcd4linux @ 2001-03-16 09:28:08 by ltoetsch]
* [lcd4linux @ 2001-03-15 15:49:22 by ltoetsch]
* [lcd4linux @ 2001-03-15 14:25:05 by ltoetsch]
* [lcd4linux @ 2001-03-15 11:10:53 by ltoetsch]
* [lcd4linux @ 2001-03-15 09:47:13 by reinelt]
* [lcd4linux @ 2001-03-15 09:13:22 by ltoetsch]
* [lcd4linux @ 2001-03-14 16:47:41 by reinelt]
* [lcd4linux @ 2001-03-14 15:30:53 by reinelt]
* [lcd4linux @ 2001-03-14 15:14:59 by reinelt]
* [lcd4linux @ 2001-03-14 13:19:29 by ltoetsch]
* [lcd4linux @ 2001-03-13 08:34:15 by reinelt]
* [lcd4linux @ 2001-03-13 08:03:41 by reinelt]
* [lcd4linux @ 2001-03-13 07:53:00 by reinelt]
* [lcd4linux @ 2001-03-13 07:41:22 by reinelt]
* [lcd4linux @ 2001-03-12 13:44:58 by reinelt]
* [lcd4linux @ 2001-03-12 12:49:24 by reinelt]
* [lcd4linux @ 2001-03-12 12:39:36 by reinelt]
* [lcd4linux @ 2001-03-09 16:24:30 by ltoetsch]
* [lcd4linux @ 2001-03-09 15:04:53 by reinelt]
* [lcd4linux @ 2001-03-09 14:30:01 by reinelt]
* [lcd4linux @ 2001-03-09 14:24:49 by ltoetsch]
* [lcd4linux @ 2001-03-09 13:08:11 by ltoetsch]
* [lcd4linux @ 2001-03-09 12:14:24 by reinelt]
* [lcd4linux @ 2001-03-08 15:25:38 by ltoetsch]
* [lcd4linux @ 2001-03-08 09:02:04 by reinelt]
* [lcd4linux @ 2001-03-08 08:39:54 by reinelt]
* [lcd4linux @ 2001-03-07 18:10:21 by ltoetsch]
* [lcd4linux @ 2001-03-05 18:20:21 by reinelt]
* [lcd4linux @ 2001-03-04 15:01:12 by ltoetsch]
* [lcd4linux @ 2001-03-02 20:18:12 by reinelt]
* [lcd4linux @ 2001-03-02 18:06:18 by reinelt]
* [lcd4linux @ 2001-03-02 17:18:52 by reinelt]
* [lcd4linux @ 2001-03-02 11:04:08 by reinelt]
* [lcd4linux @ 2001-03-02 10:18:03 by ltoetsch]
* [lcd4linux @ 2001-03-01 22:33:50 by reinelt]
* [lcd4linux @ 2001-03-01 15:11:30 by ltoetsch]
* [lcd4linux @ 2001-03-01 11:08:16 by reinelt]
* [lcd4linux @ 2001-02-26 00:33:37 by herp]
* [lcd4linux @ 2001-02-21 04:48:13 by reinelt]
* [lcd4linux @ 2001-02-19 00:15:46 by reinelt]
* [lcd4linux @ 2001-02-18 22:11:34 by reinelt]
* [lcd4linux @ 2001-02-18 21:16:06 by reinelt]
* [lcd4linux @ 2001-02-18 21:15:15 by reinelt]
* [lcd4linux @ 2001-02-18 20:16:13 by reinelt]
* [lcd4linux @ 2001-02-16 14:15:11 by reinelt]
* [lcd4linux @ 2001-02-16 08:23:09 by reinelt]
* [lcd4linux @ 2001-02-14 07:40:16 by reinelt]
* [lcd4linux @ 2001-02-14 05:22:42 by reinelt]
* [lcd4linux @ 2001-02-13 12:43:24 by reinelt]
* [lcd4linux @ 2001-02-13 09:00:13 by reinelt]
* [lcd4linux @ 2001-02-11 23:34:07 by reinelt]
* [lcd4linux @ 2000-12-07 20:47:54 by reinelt]
* [lcd4linux @ 2000-12-01 20:42:37 by reinelt]
* [lcd4linux @ 2000-12-01 07:20:26 by reinelt]
* [lcd4linux @ 2000-11-28 20:20:38 by reinelt]
* [lcd4linux @ 2000-11-28 17:27:19 by reinelt]
* [lcd4linux @ 2000-11-28 16:46:11 by reinelt]
* [lcd4linux @ 2000-11-17 10:36:23 by reinelt]
* [lcd4linux @ 2000-10-25 08:10:48 by reinelt]
* [lcd4linux @ 2000-10-20 07:17:07 by reinelt]
* [lcd4linux @ 2000-10-08 09:16:40 by reinelt]
* [lcd4linux @ 2000-08-10 18:42:20 by reinelt]
* [lcd4linux @ 2000-08-10 09:44:09 by reinelt]
* [lcd4linux @ 2000-08-09 14:14:11 by reinelt]
* [lcd4linux @ 2000-08-09 11:03:07 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.98...0.99

## What's Changed in 0.98
* [lcd4linux @ 2000-08-09 09:50:29 by reinelt]
* [lcd4linux @ 2000-07-31 10:43:44 by reinelt]
* [lcd4linux @ 2000-07-31 06:46:35 by reinelt]
* [lcd4linux @ 2000-06-04 21:43:50 by herp]
* [lcd4linux @ 2000-05-21 06:20:35 by reinelt]
* [lcd4linux @ 2000-05-03 17:14:51 by herp]
* [lcd4linux @ 2000-05-03 09:37:32 by herp]
* [lcd4linux @ 2000-05-02 23:07:48 by herp]
* [lcd4linux @ 2000-05-02 06:05:00 by reinelt]
* [lcd4linux @ 2000-04-30 06:40:42 by reinelt]
* [lcd4linux @ 2000-04-28 05:19:55 by reinelt]
* [lcd4linux @ 2000-04-20 05:48:42 by reinelt]
* [lcd4linux @ 2000-04-19 04:44:20 by reinelt]
* [lcd4linux @ 2000-04-17 05:14:27 by reinelt]
* [lcd4linux @ 2000-04-15 16:56:52 by reinelt]
* [lcd4linux @ 2000-04-15 11:56:35 by reinelt]
* [lcd4linux @ 2000-04-15 11:13:54 by reinelt]
* [lcd4linux @ 2000-04-13 06:09:52 by reinelt]
* [lcd4linux @ 2000-04-12 08:05:45 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.97...0.98

## What's Changed in 0.97
* [lcd4linux @ 2000-04-10 04:40:53 by reinelt]
* [lcd4linux @ 2000-04-07 05:42:20 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.96a...0.97

## What's Changed in 0.96a
* [lcd4linux @ 2000-04-05 05:58:36 by reinelt]
* [lcd4linux @ 2000-04-05 04:12:00 by reinelt]
* [lcd4linux @ 2000-04-03 23:53:23 by herp]
* [lcd4linux @ 2000-04-03 17:31:52 by reinelt]
* [lcd4linux @ 2000-04-03 06:54:03 by reinelt]
* [lcd4linux @ 2000-04-03 06:53:09 by reinelt]
* [lcd4linux @ 2000-04-03 04:46:38 by reinelt]
* [lcd4linux @ 2000-04-03 04:01:31 by reinelt]
* [lcd4linux @ 2000-04-02 22:07:10 by herp]
* [lcd4linux @ 2000-04-01 22:40:42 by herp]
* [lcd4linux @ 2000-04-01 19:33:45 by herp]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.96...0.96a

## What's Changed in 0.96
* [lcd4linux @ 2000-04-01 16:22:38 by reinelt]
* [lcd4linux @ 2000-03-31 04:41:22 by reinelt]
* [lcd4linux @ 2000-03-30 16:46:57 by reinelt]
* [lcd4linux @ 2000-03-28 08:48:33 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.95...0.96

## What's Changed in 0.95
* [lcd4linux @ 2000-03-28 07:22:15 by reinelt]
* [lcd4linux @ 2000-03-26 20:00:44 by reinelt]
* [lcd4linux @ 2000-03-26 19:03:52 by reinelt]
* [lcd4linux @ 2000-03-26 18:46:28 by reinelt]
* [lcd4linux @ 2000-03-26 12:55:03 by reinelt]
* [lcd4linux @ 2000-03-25 05:50:43 by reinelt]
* [lcd4linux @ 2000-03-24 11:37:43 by reinelt]
* [lcd4linux @ 2000-03-24 11:36:56 by reinelt]
* [lcd4linux @ 2000-03-23 07:24:48 by reinelt]
* [lcd4linux @ 2000-03-22 15:36:21 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.94...0.95

## What's Changed in 0.94
* [lcd4linux @ 2000-03-22 07:33:50 by reinelt]
* [lcd4linux @ 2000-03-19 08:41:28 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.92...0.94

## What's Changed in 0.92
* [lcd4linux @ 2000-03-18 10:31:06 by reinelt]
* [lcd4linux @ 2000-03-18 08:07:04 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.91...0.92

## What's Changed in 0.91
* [lcd4linux @ 2000-03-17 09:21:42 by reinelt]

**Full Changelog**: https://github.com/amd989/lcd4linux-ax206/compare/0.9...0.91

## What's Changed in 0.9
* [lcd4linux @ 2000-03-13 15:58:24 by reinelt]
* [lcd4linux @ 2000-03-10 17:36:02 by reinelt]
* [lcd4linux @ 2000-03-10 12:02:43 by reinelt]
* [lcd4linux @ 2000-03-10 11:40:47 by reinelt]
* [lcd4linux @ 2000-03-10 10:49:53 by reinelt]
* [lcd4linux @ 2000-03-07 11:01:34 by reinelt]
* [lcd4linux @ 2000-03-06 06:04:06 by reinelt]
* [lcd4linux @ 2000-01-16 16:58:50 by reinelt]
* [lcd4linux @ 2000-01-16 12:17:59 by reinelt]
* initial import

### New Contributors
* @ made their first contribution

<!-- generated by git-cliff -->
