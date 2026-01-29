#!/usr/bin/env bash
export DEBIAN_FRONTEND=noninteractive
sudo apt-get -y update && sudo apt-get -y upgrade
sudo apt-get -y install software-properties-common
sudo add-apt-repository --yes --ppa ppa:lely/ppa
sudo apt-get -y update
sudo apt-get -y install liblely-coapp-dev liblely-co-tools python3-dcf-tools cmake clang-format