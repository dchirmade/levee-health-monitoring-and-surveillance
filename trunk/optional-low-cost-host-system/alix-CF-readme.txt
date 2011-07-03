# Alixboard flashing script. Works fine with http://pcengines.ch/alix6f2.htm. 
# This script will be explained in detail on wiki page at later stage of the project. 


#!/bin/sh
# Unmount iso image
umount /dev/loop0

# Unmount device
umount /dev/sdb1

# Mounting iso image
mount -t iso9660 -o loop <Path To Ubutnu Installation Iso> /mnt/iso/

# Mounting cf card
mount /dev/<Your Device Handle> /mnt/cf

# Installing core system
debootstrap --arch=i386 natty /mnt/cf file:///mnt/iso/

# Switching to new file system
mount -o bind /dev /mnt/cf/dev
mount -o bind /dev/pts /mnt/cf/dev/pts
mount -o bind /sys /mnt/cf/sys
mount -o bind /proc /mnt/cf/proc
chroot /mnt/cf /bin/bash

# Configuring hostname 
echo 'levee-monitoring' > /etc/hostname


# Configuring hosts
echo "127.0.0.1               localhost
127.0.1.1               levee-monitoring.example.com levee-monitoring

# The following lines are desirable for IPv6 capable hosts
::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
ff02::3 ip6-allhosts" > /etc/hosts

# Configuring dns servers
echo "nameserver 8.8.8.8
nameserver 8.8.4.4" > /etc/resolv.conf

# Configuring fs-tab
echo "# /etc/fstab: static file system information.
#
# <file system>                 <mount point>   <type>  <options>                       <dump>  <pass>
proc                            /proc           proc    defaults                        0       0

# /dev/sda1
LABEL=root                      /               ext3    noatime,errors=remount-ro       0       0

tmpfs                           /tmp            tmpfs   defaults,noatime                0       0
tmpfs                           /var/tmp        tmpfs   defaults,noatime                0       0" > /etc/fstab


# Protect flash memory
echo "vm.dirty_writeback_centisecs = 1500" >> /etc/sysctl.conf

# Further flash memory protection
echo "RAMRUN=yes
RAMLOCK=yes" >> /etc/default/rcS

# Network settings
echo "# Used by ifup(8) and ifdown(8). See the interfaces(5) manpage or
# /usr/share/doc/ifupdown/examples for more information.


# The loopback network interface
auto lo
iface lo inet loopback


# The primary network interface
auto eth0
iface eth0 inet static
        address         192.168.1.1
        netmask         255.255.255.0
        gateway         192.168.1.250
        # emergency fallback addresses
        post-up         ip addr add 169.254.19.65/16 dev eth0
        pre-down        ip addr del 169.254.19.65/16 dev eth0" > /etc/network/interfaces

# Configuring apt-get source list
echo "
# Generated at http://repogen.simplylinux.ch/ 

###### Ubuntu Main Repos
deb http://in.archive.ubuntu.com/ubuntu/ natty main restricted universe multiverse 
deb-src http://in.archive.ubuntu.com/ubuntu/ natty main restricted universe multiverse 

###### Ubuntu Update Repos
deb http://in.archive.ubuntu.com/ubuntu/ natty-security main restricted universe multiverse 
deb http://in.archive.ubuntu.com/ubuntu/ natty-updates main restricted universe multiverse 
deb http://in.archive.ubuntu.com/ubuntu/ natty-proposed main restricted universe multiverse 
deb http://in.archive.ubuntu.com/ubuntu/ natty-backports main restricted universe multiverse 
deb-src http://in.archive.ubuntu.com/ubuntu/ natty-security main restricted universe multiverse 
deb-src http://in.archive.ubuntu.com/ubuntu/ natty-updates main restricted universe multiverse 
deb-src http://in.archive.ubuntu.com/ubuntu/ natty-proposed main restricted universe multiverse 
deb-src http://in.archive.ubuntu.com/ubuntu/ natty-backports main restricted universe multiverse 

" > /etc/apt/sources.list

# Configuring apt-get dependencies
echo "APT::Install-Recommends \"0\";
APT::Install-Suggests \"0\";" >  /etc/apt/apt.conf.d/00onlydepends

# Update apt-get repository
apt-get update

# Configuring Regional support
echo en_US.UTF-8 UTF-8> /var/lib/locales/supported.d/local

# Install language package 
apt-get -y install language-pack-en

# Configure kernel image 
echo "# Kernel image management overrides
# See kernel-img.conf(5) for details
do_symlinks = yes
relative_links = yes
do_bootloader = no
do_bootfloppy = no
do_initrd = yes
link_in_boot = no" > /etc/kernel-img.conf

# Install kernel image 
apt-get -y install linux-image-386

# Install grub-pc
apt-get -y install grub-pc

# Configure grub
echo "# If you change this file, run 'update-grub' afterwards to update
# /boot/grub/grub.cfg.

GRUB_DEFAULT=0
#GRUB_HIDDEN_TIMEOUT=0
GRUB_HIDDEN_TIMEOUT_QUIET=true
GRUB_TIMEOUT=10
GRUB_DISTRIBUTOR=`lsb_release -i -s 2> /dev/null || echo Debian`
GRUB_CMDLINE_LINUX_DEFAULT=\"verbose console=ttyS0,9600n8 reboot=bios\"  # Set \"quiet splash\" in production.
GRUB_CMDLINE_LINUX=\"\"

# Remove serial support in production. 
GRUB_SERIAL_COMMAND=\"serial --unit=0 --speed=9600\"
GRUB_TERMINAL=serial

# Uncomment to disable graphical terminal (grub-pc only)
#GRUB_TERMINAL=console

# The resolution used on graphical terminal
# note that you can use only modes which your graphic card supports via VBE
# you can see them in real GRUB with the command 'vbeinfo'
#GRUB_GFXMODE=640x480

# Uncomment if you don't want GRUB to pass \"root=UUID=xxx\" parameter to Linux
#GRUB_DISABLE_LINUX_UUID=true

# Uncomment to disable generation of recovery mode menu entrys
#GRUB_DISABLE_LINUX_RECOVERY=\"true\" " > /etc/default/grub

#configuring serial 
echo "# ttyS0 - getty
#
# This service maintains a getty on tty1 from the point the system is
# started until it is shut down again.

start on stopped rc RUNLEVEL=[2345]
stop on runlevel [!2345] " > /etc/init/ttyS0.conf

# Configure device map
echo "(hd0)   /dev/sdb" > /boot/grub/devices.map

# Update grub
update-grub

# Kernel configuration to update grub
echo "postinst_hook = update-grub
postrm_hook = update-grub" >> /etc/kernel-img.conf

# Set root password
passwd

# Install ssh server
apt-get -y install ssh

# Install cryptsetupi [Only needed if case if you're using encrypted filesystem]
aptitude install cryptsetup

# Install ldconfig
ldconfig

# Install VIM,as by default tiny VI is installed.
apt-get install vim

# Exit from the target and unmount
exit
umount /mnt/cf/proc
umount /mnt/cf/sys
umount /mnt/cf/dev/pts
umount /mnt/cf/dev
umount /dev/sdb1
umount /dev/loop0
