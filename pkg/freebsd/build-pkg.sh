#!/bin/sh
# Build FreeBSD .pkg files from the compiled lcd4linux binary.
# Produces two packages:
#   lcd4linux-ax206-<version>.pkg        — binary + service script
#   lcd4linux-ax206-themes-<version>.pkg — themes, fonts, lcd4linux-theme CLI
#
# Usage: ./build-pkg.sh <version>
set -e

VERSION="${1:?Usage: build-pkg.sh <version>}"
PKG_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$PKG_DIR/../.." && pwd)"

DIST_DIR="$ROOT_DIR/dist"
mkdir -p "$DIST_DIR"

# ---------------------------------------------------------------------------
# Package 1: lcd4linux-ax206 (binary only)
# ---------------------------------------------------------------------------
STAGING_MAIN="$(mktemp -d)"
trap 'rm -rf "$STAGING_MAIN" "$STAGING_THEMES"' EXIT

mkdir -p "$STAGING_MAIN/usr/local/bin"
mkdir -p "$STAGING_MAIN/usr/local/etc/rc.d"
mkdir -p "$STAGING_MAIN/usr/local/etc/lcd4linux/examples"
mkdir -p "$STAGING_MAIN/usr/local/share/lcd4linux"
touch "$STAGING_MAIN/usr/local/share/lcd4linux/.keep"

cp "$ROOT_DIR/lcd4linux" "$STAGING_MAIN/usr/local/bin/lcd4linux"
chmod 755 "$STAGING_MAIN/usr/local/bin/lcd4linux"

cp "$PKG_DIR/rc.d/lcd4linux" "$STAGING_MAIN/usr/local/etc/rc.d/lcd4linux"
chmod 755 "$STAGING_MAIN/usr/local/etc/rc.d/lcd4linux"

cp "$ROOT_DIR/dpf_320x240.conf" "$STAGING_MAIN/usr/local/etc/lcd4linux/examples/dpf_320x240.conf"
if [ -f "$ROOT_DIR/lcd4linux.conf.sample" ]; then
    cp "$ROOT_DIR/lcd4linux.conf.sample" "$STAGING_MAIN/usr/local/etc/lcd4linux/examples/lcd4linux.conf.sample"
fi

PLIST_MAIN="$(mktemp)"
(cd "$STAGING_MAIN/usr/local" && find . -type f | sed 's|^\./||') > "$PLIST_MAIN"

sed "s/\${VERSION}/$VERSION/g" "$PKG_DIR/+MANIFEST" > "$STAGING_MAIN/+MANIFEST"
cp "$PKG_DIR/+POST_INSTALL" "$STAGING_MAIN/+POST_INSTALL"
cp "$PKG_DIR/+POST_DEINSTALL" "$STAGING_MAIN/+POST_DEINSTALL"

pkg create -m "$STAGING_MAIN" -r "$STAGING_MAIN" -p "$PLIST_MAIN" -o "$DIST_DIR/"
rm -f "$PLIST_MAIN"

# ---------------------------------------------------------------------------
# Package 2: lcd4linux-ax206-themes (themes, fonts, lcd4linux-theme CLI)
# ---------------------------------------------------------------------------
STAGING_THEMES="$(mktemp -d)"

mkdir -p "$STAGING_THEMES/usr/local/bin"
mkdir -p "$STAGING_THEMES/usr/local/share/lcd4linux"

cp "$ROOT_DIR/lcd4linux-theme" "$STAGING_THEMES/usr/local/bin/lcd4linux-theme"
chmod 755 "$STAGING_THEMES/usr/local/bin/lcd4linux-theme"

cp -r "$ROOT_DIR/themes" "$STAGING_THEMES/usr/local/share/lcd4linux/themes"
cp -r "$ROOT_DIR/fonts" "$STAGING_THEMES/usr/local/share/lcd4linux/fonts"

PLIST_THEMES="$(mktemp)"
(cd "$STAGING_THEMES/usr/local" && find . -type f | sed 's|^\./||') > "$PLIST_THEMES"

sed "s/\${VERSION}/$VERSION/g" "$PKG_DIR/+MANIFEST.themes" > "$STAGING_THEMES/+MANIFEST"
cp "$PKG_DIR/+POST_INSTALL.themes" "$STAGING_THEMES/+POST_INSTALL"

pkg create -m "$STAGING_THEMES" -r "$STAGING_THEMES" -p "$PLIST_THEMES" -o "$DIST_DIR/"
rm -f "$PLIST_THEMES"

# ---------------------------------------------------------------------------
echo "Packages created in $DIST_DIR/"
ls -lh "$DIST_DIR/"*.pkg 2>/dev/null || echo "Warning: no .pkg files found"
