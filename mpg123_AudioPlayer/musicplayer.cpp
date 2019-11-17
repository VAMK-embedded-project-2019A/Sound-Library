//
//  musicplayer.cpp
//  musicplayer
//
//  Created by Linh Nguyen on 30.10.2019.
//  Copyright © 2019 Linh Nguyen. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <mpg123.h>
#include <fcntl.h>
#include <unistd.h>

#include "musicplayer.h"

MusicPlayer::MusicPlayer()
{
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
}

MusicPlayer::~MusicPlayer()
{
    free(buffer);
    mpg123_close(mh);
    mpg123_delete(mh);
}

void MusicPlayer::setFileName(const char * file_name)
{
    if(file_name == "\0")
        return ;
    
    else
    {
      _file_name = (char*)file_name;

    }
}

char * MusicPlayer::getFileName()
{
    return _file_name;
}

void MusicPlayer::loadSong(){
    int fnum = open(_file_name, O_RDONLY);
    if(fnum < 0){
        std::cout << "ERROR opening file: " << strerror(fnum) << "\n";
        exit(0);
    }
    
    int opened_val = mpg123_open(mh, _file_name);
    close(fnum);
    mpg123_read(mh, buffer, buffer_size, &done);
}


void MusicPlayer::play()
{
//    if(_file_name == "\0")
//    {
//        std::cout << "Error: No sound file detected" << std::endl;
//        return;
//    }
    

        mpg123_seek(mh, 0, SEEK_SET); // SEEK_SET: set positions to (or near to) specified offset
        
//        decode and play file
        while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
            ao_play(dev, reinterpret_cast<char*>(buffer), done);
        }
    
}

