# Sound-Library
-------

1. Start setting up audio working on Raspberry Pi 3 with Raspbian
-  For any version [Raspbian Buster](https://www.raspberrypi.org/downloads/raspbian/), ALSA and its dependencies are pre-installed with the operating system. 

- For older version of Raspbian, check if it has ALSA and dependencies installed, otherwise, following [this instruction](https://alsa.opensrc.org/Quick_Install) to install ALSA.

- Load the sound driver and check if it is loaded:
    > sudo modprobe snd-bcm2835
    
    >sudo lsmod | grep 2835

- Select the output device for sound (0 = default/auo, 1 = jack 3.5, 2 = HDMI)
    > sudo amixer cset numid=3 1
    
- Open terminal and test ALSA working
    > aplay /usr/share/sounds/alsa/Front_Center.wav (the output is only one short beep sound)
    
    >speaker-test -t sine -f 440 -c 2 -s 1

- If you get the following error message and still get the output sound properly, then just let it be, otherwise, for some RPi or some systems, if you fix this error, there will be no sound:
    > ALSA lib pcm.c:2217:(snd_pcm_open_noupdate) Unknown PCM cards.pcm.front

- If you get the same error message above and have no output sound, then edit the file at /usr/share/alsa/alsa.conf:
    > sudo nano /usr/share/alsa/alsa.conf
    
- Change the line **"pcm.front cards.pcm.front”** to **“pcm.front cards.pcm.default”"**
    
- For some systems and/or applications, it will be required to have **.asoundrc** or **asound.conf** file to set up output hardware/soundcard properly.

- Use ALSA command to find which card number is your devicce:
    > aplay -l

- The **.asoundrc** file is typically installed in a user's home directory, if it's not there, create one:
    > nano ~/.asoundrc 

- Copy and paste the following code and save it:
```pcm.!default {
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

- Or create configuration file **asound.conf** at root: /etc/asound.conf with the same code above.

2. mpg123 library
- This music player project is built based on the library [mpg123 and libout123](https://www.mpg123.de/#2019-10-26)

- Install libmpg123, you may need to install package mpg123 separately if mpg123 command does not work. 
    > sudo apt-get install libmpg123-dev

- And:
    > sudo apt-get install mpg123

- In latest release, libout123 automatically installed with mpg123, if libout123(out123.so) is lacked in any older version, install libout123 separately by following [this instruction](http://www.linuxfromscratch.org/blfs/view/svn/multimedia/mpg123.html)

3. Test mpg123

- Normally, simply use mpg123 command:
    > mpg123 file_name.mp3

- Or use remote control interface, you can play file by commands 
    > mpg123 -R 

- For remote interface commands, check [this link](https://github.com/georgi/mpg123/blob/master/doc/README.remote)

- If mpg123 does not work, it could be the error of picking up wrong output port, try this command by using ALSA to specify output hardware:
    > aplay -l mpg123 -a hw:1,0 file_name.mp3

4. Compile and run our code
-  Simply run 
    > make 
    
    >sudo ./main 
