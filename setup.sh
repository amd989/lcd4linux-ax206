#!/bin/bash
set -e
echo "Adding lcd4linux-ax206 APT repository..."
curl -fsSL https://amd989.github.io/lcd4linux-ax206/gpg.key | gpg --dearmor -o /usr/share/keyrings/lcd4linux-ax206.gpg
echo "deb [signed-by=/usr/share/keyrings/lcd4linux-ax206.gpg] https://amd989.github.io/lcd4linux-ax206 stable main" > /etc/apt/sources.list.d/lcd4linux-ax206.list
apt-get update
echo "Done! You can now run: sudo apt install lcd4linux-ax206"
