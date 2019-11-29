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
    init();
}

MusicPlayer::~MusicPlayer()
{
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
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

void MusicPlayer::init() {
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

}


void MusicPlayer::loadSong(){
    int opened_file = open(_file_name, O_RDONLY);
    if(opened_file < 0){
        std::cout << "ERROR opening file: " << strerror(opened_file) << "\n";
        exit(0);
    }
    close(opened_file);

    // Open file and get the decoding format
    int opened_val = mpg123_open(mh, _file_name);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    //mpg123_read(mh, buffer, buffer_size, &done);
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
}


void MusicPlayer::play()
{
    mpg123_seek(mh, 0, SEEK_SET); // SEEK_SET: set positions to (or near to) specified offset

//        decode and play file
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
        ao_play(dev, reinterpret_cast<char*>(buffer), done);
    }
}

