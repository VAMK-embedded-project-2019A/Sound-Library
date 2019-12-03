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
#include <out123.h>
#include <unistd.h>

#include "musicplayer.h"

MusicPlayer::MusicPlayer()
{
    init();
}

MusicPlayer::~MusicPlayer()
{
    CleanUp(mh, ao);
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

void MusicPlayer::init()
{
    err = mpg123_init();
    mh = mpg123_new(NULL, &err);
    if(err != MPG123_OK || mh == NULL)
    {
        fprintf(stderr, "Basic setup goes wrong : %s\n", mpg123_plain_strerror(err));
        CleanUp(mh, ao);
    }

    ao = out123_new();
    if(!ao){
        fprintf(stderr, "Cannot create output handle \n");
        CleanUp(mh, ao);
    }
}


/*void MusicPlayer::loadSong(){
    int opened_file = open(_file_name, O_RDONLY);
    if(opened_file < 0){
        std::cout << "ERROR opening file: " << strerror(opened_file) << "\n";
        exit(0);
    }
    close(opened_file);

    // Open file and get the decoding format
    int opened_val = mpg123_open(mh, _file_name);
    mpg123_getformat(mh, &rate, &channels,  &encoding);

    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
}*/

void MusicPlayer::loadSong() {
    //handle to accept all format (also any custom rate)
    //mpg123_format_all(mh);
//    long r = 48000;
//    const long *rates;
//    rates = &r;
//    size_t rate_count, i;
//    int enc = out123_enc_byname("48000");
    mpg123_format_none(mh);
//    mpg123_rates(&rates, &rate_count);
//    for (i = 0; i < rate_count; ++i)
//    {
//        mpg123_format(mh, rates[i], MPG123_MONO|MPG123_STEREO, enc);
//    }
    mpg123_format(mh, rate, channels, encoding);
    int opened_file = mpg123_open(mh, _file_name);
    int format_file = mpg123_getformat(mh, &rate, &channels, &encoding);
    if(opened_file != MPG123_OK || format_file != MPG123_OK)
    {
        fprintf(stderr, "Trouble with mpg123: %s \n", mpg123_strerror(mh));
        CleanUp(mh, ao);
    }

    err = out123_open(ao, driver, NULL);
    if(err != MPG123_OK)
    {
        fprintf(stderr, "Trouble with out123: %s\n", out123_strerror(ao));
        CleanUp(mh, ao);
    }
    //mpg123_format(mh, rate, channels, encoding);
    out123_start(ao, rate, channels, encoding);

    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

}

void MusicPlayer::play()
{
    //mpg123_seek(mh, 0, SEEK_SET); // SEEK_SET: set positions to (or near to) specified offset

    do
    {
       err = mpg123_read(mh, buffer, buffer_size, &done);
       played = out123_play(ao, buffer, done);
    } while( done && err == MPG123_OK);
}

void MusicPlayer::pause()
{
    out123_pause(ao);
}

void MusicPlayer::resume()
{
    out123_continue(ao);
}

void MusicPlayer::stop()
{
    out123_drop(ao);
    out123_stop(ao);
}

bool MusicPlayer::isPlaying()
{
    return _is_Playing;
}

void MusicPlayer::CleanUp(mpg123_handle *mh, out123_handle *ao)
{
    free(buffer);
    out123_del(ao);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}
