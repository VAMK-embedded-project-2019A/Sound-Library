//
//  musicplayer.h
//  musicplayer
//
//  Created by Linh Nguyen on 30.10.2019.
//  Copyright © 2019 Linh Nguyen. All rights reserved.
//

#ifndef musicplayer_h
#define musicplayer_h

#include <cstring>
#include <string>
#include <mpg123.h>
#include <iostream>
#include <out123.h>
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
    bool isPlaying();
    void pause();
    void resume();
    void stop();
    
private:
    int init();
    void CleanUp(mpg123_handle *mh, out123_handle *ao);
    char * _file_name;
    char *driver = NULL;
    mpg123_handle *mh = NULL; //note: cpp doesn't initialize pointers = NULL by default
    out123_handle *ao = NULL;
    unsigned char *buffer = NULL;
    size_t done;
    size_t buffer_size;
    size_t played;
    //const char *encname;
    int channels, encoding, err;
    //int frameSize;
    long rate; 
    bool _is_Playing{true};
};

#endif /* musicplayer_h */
