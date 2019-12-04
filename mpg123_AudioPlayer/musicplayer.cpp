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

void MusicPlayer::loadSong() {

    //mpg123_format(mh, &r, &channels, &encoding);
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
    mpg123_format(mh, rate, channels, encoding);
    out123_start(ao, rate, channels, encoding);

    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

}

void MusicPlayer::play()
{
    _state = State::play_live;

    do
    {
       err = mpg123_read(mh, buffer, buffer_size, &done);
       out123_play(ao, buffer, done);

       switch(_control_request)
       {
           case Pause:
               pause();
               break;

           case Stop:
               stop();
               break;

           case Continue:
               resume();
               break;

           case Next:
               break;

           case Prev:
               break;

           default:
               break;
       }

    } while( done && err == MPG123_OK);
  
}

void MusicPlayer::pause()
{
    /*if(isPlaying()){
        out123_pause(ao);
        _state = State::play_paused;
    }  */
}

void MusicPlayer::resume()
{
    if(!isPlaying()){
        out123_continue(ao);
        err = out123_errcode(ao);
        if(err == OUT123_BAD_HANDLE)
        {
            fprintf(stderr, "Trouble with out123: %s\n", out123_plain_strerror(err));
            CleanUp(mh, ao);
        }
        
        _state = State::play_live;
    }
}

void MusicPlayer::stop()
{
    out123_drop(ao);
    out123_stop(ao);
    _state = State::play_stopped;
}

bool MusicPlayer::isPlaying()
{
    return (_state == State::play_live);
}

bool MusicPlayer::Control_Requested(Control_Requests_Enum _request) {
    if(_control_request != None)
        return false;
    _control_request = _request;
    return true;
}

void MusicPlayer::CleanUp(mpg123_handle *mh, out123_handle *ao)
{
    free(buffer);
    out123_del(ao);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}