#!/bin/sh
set -e
echo "Adding lcd4linux-ax206 FreeBSD pkg repository..."
mkdir -p /usr/local/etc/pkg/repos
cat > /usr/local/etc/pkg/repos/lcd4linux-ax206.conf <<REPOEOF
lcd4linux-ax206: {
  url: "https://amd989.github.io/lcd4linux-ax206/freebsd",
  enabled: yes,
  signature_type: "none"
}
REPOEOF
pkg update
echo "Done! Run: pkg install lcd4linux-ax206"
