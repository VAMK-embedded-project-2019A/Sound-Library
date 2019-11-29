//
//  musicplayer.h
//  musicplayer
//
//  Created by Linh Nguyen on 30.10.2019.
//  Copyright © 2019 Linh Nguyen. All rights reserved.
//

#ifndef musicplayer_h
#define musicplayer_h

#include <ao/ao.h>
#include <cstring>
#include <string>
#include <mpg123.h>
#include <iostream>

#define BITS 8


class MusicPlayer
{
    
public:
    MusicPlayer();
    ~MusicPlayer();
    
    void setFileName(const char * file_name);
    char * getFileName();
    void loadSong();
    void play();
//  bool isPlaying();
//    void pause();
//    void resume(); 
    
private:
    void init();
    char * _file_name;
    mpg123_handle *mh = NULL; //note: cpp doesn't initialize pointers = NULL by default
    unsigned char *buffer = NULL;
    size_t done;
    size_t buffer_size;
    ao_device *dev;
    ao_sample_format format;
    int driver, channels, encoding, err;
    long rate;
};

#endif /* musicplayer_h */
