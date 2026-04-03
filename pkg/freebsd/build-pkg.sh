#!/bin/sh
# Build a FreeBSD .pkg from the compiled lcd4linux binary
# Usage: ./build-pkg.sh <version>
set -e

VERSION="${1:?Usage: build-pkg.sh <version>}"
PKG_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$PKG_DIR/../.." && pwd)"
STAGING="$(mktemp -d)"

trap 'rm -rf "$STAGING"' EXIT

# Stage files into pkg layout under prefix /usr/local
mkdir -p "$STAGING/usr/local/bin"
mkdir -p "$STAGING/usr/local/etc/rc.d"
mkdir -p "$STAGING/usr/local/etc/lcd4linux/examples"

# Binary
cp "$ROOT_DIR/lcd4linux" "$STAGING/usr/local/bin/lcd4linux"
chmod 755 "$STAGING/usr/local/bin/lcd4linux"

# rc.d service script
cp "$PKG_DIR/rc.d/lcd4linux" "$STAGING/usr/local/etc/rc.d/lcd4linux"
chmod 755 "$STAGING/usr/local/etc/rc.d/lcd4linux"

# Config examples
cp "$ROOT_DIR/dpf_320x240.conf" "$STAGING/usr/local/etc/lcd4linux/examples/dpf_320x240.conf"
if [ -f "$ROOT_DIR/lcd4linux.conf.sample" ]; then
    cp "$ROOT_DIR/lcd4linux.conf.sample" "$STAGING/usr/local/etc/lcd4linux/examples/lcd4linux.conf.sample"
fi

# Generate plist (file manifest) — paths relative to prefix /usr/local
PLIST="$(mktemp)"
(cd "$STAGING/usr/local" && find . -type f | sed 's|^\./||') > "$PLIST"

# Generate manifest with version substituted
sed "s/\${VERSION}/$VERSION/" "$PKG_DIR/+MANIFEST" > "$STAGING/+MANIFEST"
cp "$PKG_DIR/+POST_INSTALL" "$STAGING/+POST_INSTALL"
cp "$PKG_DIR/+POST_DEINSTALL" "$STAGING/+POST_DEINSTALL"

# Build the package
mkdir -p "$ROOT_DIR/dist"
pkg create -m "$STAGING" -r "$STAGING" -p "$PLIST" -o "$ROOT_DIR/dist/"

rm -f "$PLIST"
echo "Package created in $ROOT_DIR/dist/"
ls -lh "$ROOT_DIR/dist/"*.pkg 2>/dev/null || echo "Warning: no .pkg file found"
