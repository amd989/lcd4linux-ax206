#!/bin/bash
set -e
echo "Adding lcd4linux-ax206 YUM/DNF repository..."
rpm --import https://amd989.github.io/lcd4linux-ax206/gpg.key
cat > /etc/yum.repos.d/lcd4linux-ax206.repo <<REPOEOF
[lcd4linux-ax206]
name=lcd4linux-ax206
baseurl=https://amd989.github.io/lcd4linux-ax206/rpm/
enabled=1
gpgcheck=1
gpgkey=https://amd989.github.io/lcd4linux-ax206/gpg.key
REPOEOF
echo "Done! You can now run: sudo dnf install lcd4linux-ax206"
