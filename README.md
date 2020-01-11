# Music Player

#### 1. Start setting up audio working on Raspberry Pi 3 with Raspbian
- For the September 2019 version of Raspbian Buster, ALSA and its dependencies are pre-installed with the operating system. 
- Load the sound driver and check if it is loaded:
    > sudo modprobe snd-bcm2835   
    > sudo lsmod | grep 2835
- Select the output device for sound (0 = default/auo, 1 = jack 3.5, 2 = HDMI)
    > sudo amixer cset numid=3 1
- Open terminal and test ALSA working
    > aplay /usr/share/sounds/alsa/Front_Center.wav
- It is required to have the file **asound.conf** to set up output hardware/soundcard properly.
    > sudo nano /etc/asound.conf
- Copy and paste the following code and save it:
```
pcm.!default {
	type plug
	slave {
		pcm "hw:1,0"
	}
}
ctl.!default {
	type hw           
	card 1
}
```
#### 2. mpg123 library
- This music player project is built based on the library [mpg123 and libout123](https://www.mpg123.de/#2019-10-26)
- Install libmpg123, you may need to install package mpg123 separately if mpg123 command does not work. 
    > sudo apt-get install libmpg123-dev
- And:
    > sudo apt-get install mpg123
- In latest release, libout123 automatically installed with mpg123, if libout123(out123.so) is lacked in any older version, install libout123 separately by following [this instruction](http://www.linuxfromscratch.org/blfs/view/svn/multimedia/mpg123.html)

#### 3. Test mpg123
- Normally, simply use mpg123 command:
    > mpg123 file_name.mp3
- Or use remote control interface, you can play file by commands 
    > mpg123 -R 
- For remote interface commands, check [this link](https://github.com/georgi/mpg123/blob/master/doc/README.remote)
- If mpg123 does not work, it could be the error of picking up wrong output port, try this command by using ALSA to specify output hardware:
    > aplay -l mpg123 -a hw:1,0 file_name.mp3

#### 4. Compile and run our code
-  Simply run 
    > make  
    > ./a.out 
