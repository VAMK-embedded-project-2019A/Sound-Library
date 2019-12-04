# Sound-Library

Install and configure in Raspberry Pi 3 

Install libmpg123: sudo apt-get install libmpg123-dev
It should come along with libout123 in some systems but if libout123(out123.so) is lacked, install libout123 separately 
by following this instruction.

Download (HTTP): https://downloads.sourceforge.net/mpg123/mpg123-1.25.13.tar.bz2

Download MD5 sum: 294a6c30546504ec3d0deac2b2ea22be
Download size: 892 KB
Estimated disk space required: 13 MB
Estimated build time: 0.2 SBU

Untar file and change directory to the folder where source file is extracted, run the following commands:
  ./configure --prefix=/usr && make
  
Now as the root user, run: 
  sudo make install 


ref: http://www.linuxfromscratch.org/blfs/view/svn/multimedia/mpg123.html
