#!/usr/bin/env sh

## General usage

sudo apt-get install console-data

## For kernel compilation

apt-get update
apt-get source linux-image-$(uname -r)
apt-get build-dep linux-image-$(uname -r)
apt-get install build-essential linux-headers-$(uname -r)


## For vimdeck

apt-get update
apt-get install libncurses5-dev
apt-get install ruby{,-dev}
apt-get install libmagickcore-dev imagemagick
apt-get install libmagickwand-dev
gem install vimdeck
