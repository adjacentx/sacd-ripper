
   DISCLAIMER.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
   BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
   FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
   OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
   TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
   USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
   DAMAGE.

SACD Ripper PS3 installation
============================

There are three main steps to install the PS3 SACD Ripper:

1. Install a custom firmware (see instructions below) that allows patching
   and the installation of custom applications.
2. Download or Compile the sacd-ripper.pkg file and install this on your PS3
3. Download the PS3 keys and use this on first time installation (see 
   instructions below)

Custom Firmware Installation
============================

Make sure you have a PS3 model with SACD compatible player. For more information
on what model specifically go to: http://ps3sacd.com/faq.html#_Toc180147566

You need to install a custom firmware to be able to rip SACDs. At this time of
writing installing custom PS3 firmware isn't possible for firmware 3.56 and upwards.
So in order to install the correct firmware you'll have to make sure your firmware
is lower or equal to 3.55. A downgrade for a firmware higher than 3.55 is NOT
available.

The custom firmware that is needed must be 3.55 and should allow patching. Most
custom firmwares like Kmeaw, Rebug, etc.. all facilitate this. Google for
"kmeaw cfw" and you'll find several download links.

Custom Firmware installation steps:

1. Plug a USB stick into your computer
2. Copy the custom firmware file that you downloaded to a USB memory
   stick as /PS3/UPDATE/PS3UPDAT.PUP. 
3. Plug the USB stick into your PS3
4. Navigate to Settings Tab
5. Choose System Update
6. Choose Update via Storage Media
7. It will say it found Version 3.55
8. Choose OK
9. Accept Conditions and Follow the instructions

If the steps above did not work for you, then you are probably on 3.55 already.
You will have to do a recovery menu mode installation:

1. Plug a USB stick into your computer
2. Copy the custom firmware file that you downloaded to a USB memory
   stick as /PS3/UPDATE/PS3UPDAT.PUP. 
3. Plug the USB stick into your PS3
4. Power down the PS3 through the menu
5. Now press and HOLD the power button, the system will startup and shutdown
   again
6. Release the power button, then press & HOLD power again, you'll hear one
   beep followed by two consecutive beeps
7. Release power then follow the on-screen instructions. You're now in the
   recovery menu
8. Connect the USB device and select "System Update."
9. Accept Conditions and Follow the instructions

SACD Ripper Installation Instructions
=====================================

Now you've completed the firmware upgrade you are ready to install
sacd-ripper.pkg. You may compile the sacd-ripper.pkg or you can use your
Google skills to find a pre-compiled package. (no, I will not distribute
the sacd-ripper.pkg as that could potentially give me legal issues).

In order for the PS3 BluRay player to authenticate the SACD disc and to decode
DST to DSD two SPU isoself modules will be extracted from the system. In order
to do so you must give the SACD-Ripper the PS3 keys so it can extract them
automatically.

Get the PS3 keys from https://github.com/Mamdooh/PS3keys and put these in the
root of an USB disc. They will be asked for during first time installation,
after that you can remove the keys from your USB disc.

SACD Ripper Build Instructions
==============================

To compile you will need the psl1ght suite. If you want to compile on a windows
environment I advice you to follow the CygWin installation as the MingW
installation is unstable.

When psl1ght has been properly setup you need to configure SACD-Ripper with the
following commands::

    git clone https://github.com/sacd-ripper/sacd-ripper.git
    cd sacd-ripper
    sh configure
    make

SACD-Ripper should compile without errors and warnings. Once completed you
should have the sacd-ripper.pkg package that can be installed on your PS3.

SACD Extract Build Instructions
===============================

First you need to install the latest version of cmake: http://www.cmake.org/
Now you can configure SACD extract using a "cmake ." followed by a "make"
command or when you don't like the command line you can generate 
project files for your favorite GUI (XCode, MSVC, etc..).

On Windows you need pthread support and libiconv. Pthread support for windows
can be downloaded from: http://sourceware.org/pthreads-win32/

And to compile libiconv on windows you can do the following:

1. Open the Visual Studio Command prompt
2. Change directory to libs\libiconv
3. run: "nmake -f Makefile.msvc NO_NLS=1"
4. now you can compile using the MSVC project file

On Linux the program currently builds only in root's home directory.
You do not need to manually build the libiconv program.  There is no
installation step available yet, so you will need to copy the program
to a directory in your PATH, such as /usr/local/bin on most Linux
installations. To compile on Linux use a sequence like this::

    sudo su -
    [Enter password if needed]
    cd /root
    git clone https://github.com/sacd-ripper/sacd-ripper.git
    cd sacd-ripper/tools/sacd_extract/
    cmake .
    make
    cp sacd_extract /usr/local/bin/

SACD Ripper/Extract Usage Instructions
======================================

From version 0.3.6 and upwards SACD Ripper has two modes of operation:

1. Daemon mode (selected by default if you don't make a selection within 5
   seconds after booting SACD Ripper).
2. ISO extraction mode (the previous versions of SACD Ripper allowed you to
   extract DSDIFF, DSF directly, but this feature has been removed in favor 
   of local extraction using SACD Extract).

SACD Extract (available on Windows, Linux, OS X) allows you to extract
individual tracks from ISO files when operated in file mode or from the SACD 
Ripper when operated in Daemon mode.

The following options are available for the sacd_extract commandline tool: ::

  Usage: sacd_extract [options] [outfile]
    -2, --2ch-tracks                : Export two channel tracks (default)
    -m, --mch-tracks                : Export multi-channel tracks
    -e, --output-dsdiff-em          : output as Philips DSDIFF (Edit Master) file
    -p, --output-dsdiff             : output as Philips DSDIFF file
    -s, --output-dsf                : output as Sony DSF file
    -I, --output-iso                : output as RAW ISO
    -c, --convert-dst               : convert DST to DSD
    -C, --export-cue                : Export a CUE Sheet
    -i, --input[=FILE]              : set source and determine if "iso" image,
                                      device or server (ex. -i192.168.1.10:2002)
    -P, --print                     : display disc and track information
  
  Help options:
    -?, --help                      : Show this help message
    --usage                         : Display brief usage message

Usage examples
==============

Extract all stereo tracks to multiple DSDIFF files and convert all DST to DSD::

    $ sacd_extract -2 -p -c -i"Foo_Bar_RIP.ISO"

Extract all multi channel tracks from the given ISO to multiple DSF files and 
convert all DST to DSD::

    $ sacd_extract -2 -s -i"Foo_Bar_RIP.ISO"

Extract a single DSDIFF/DSD Multi-Channel Edit Master track from the given ISO
and convert all DST to DSD::

    $ sacd_extract -m -e -c -i"Foo_Bar_RIP.ISO"

Extract a single ISO file from the SACD Ripper Daemon (IP address and Port is
displayed on startup). You can use SACD Extract again on the ISO file to extract
the DSD data (see the three examples above)::

    $ sacd_extract -I -i192.168.1.10:2002

Extract all multi channel tracks from the SACD Ripper Daemon (IP address and
Port is displayed on startup) to multiple DSDIFF files and keep the DST format::

    $ sacd_extract -m -p -i192.168.1.10:2002

Generate a sacd_log.txt file that contains the ISRC codes which should/could
be used for ISO verification::

    $ sacd_extract -P -i192.168.1.10:2002 >sacd_log.txt


Thank you!
==========

A big thank you goes to:

* Graf Chokolo
* Geoffrey Levand :-)
* Max
* vfalks
* Patrick


