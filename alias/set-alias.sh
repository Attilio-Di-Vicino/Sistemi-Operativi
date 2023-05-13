#!/bin/bash

# ls..
echo 'alias vrimm="ls"' >> ~/.bashrc
echo 'alias vrimm-tutt-cos="ls -la"' >> ~/.bashrc

# cd..
echo 'alias tras="cd"' >> ~/.bashrc
echo 'alias aret="cd .."' >> ~/.bashrc

# shutdown
echo 'alias accirt="shutdown -h now"' >> ~/.bashrc
echo 'alias buonanotte="shutdown -h now"' >> ~/.bashrc
echo 'alias stutall="shutdown -h now"' >> ~/.bashrc

# update
echo 'alias scarica="sudo apt update"' >> ~/.bashrc
echo 'alias aggiorn="sudo apt upgrade"' >> ~/.bashrc
echo 'alias aggiorn-tutt-cos="sudo apt dist-upgrade"' >> ~/.bashrc
echo 'alias cancell="sudo apt autoremove"' >> ~/.bashrc
echo 'alias pulezz="sudo apt-get clean"' >> ~/.bashrc
echo 'alias pulezz-tutt-cos="sudo apt-get autoclean"' >> ~/.bashrc

source ~/.bashrc