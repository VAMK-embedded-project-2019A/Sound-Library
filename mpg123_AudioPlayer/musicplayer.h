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
#include <iostream>
#include <fcntl.h>
#include <future>
#include <mpg123.h>
#include <mutex>
#include <out123.h>
#include <string>
#include <unistd.h>

enum class State {
    play_dead = 0,
    play_stopped,
    play_paused,
    play_live
};

class MusicPlayer
{
    
public:
    MusicPlayer();
    ~MusicPlayer();

    enum Control_Requests_Enum{
        Pause,
        Continue,
        Stop,
        Next,
        Prev,
        None
    };

    void setFileName(const char * file_name);
    char * getFileName();
    void loadSong();
    void play();
    void pause();
    void resume();
    void stop();
    /*void VolumeUp();
    void VolumeDown();
    void getVolume();
    void Next();
    void Prev();*/
    bool isPlaying();
    bool Control_Requested(Control_Requests_Enum _request);
    
private:
    void init();
    void CleanUp(mpg123_handle *mh, out123_handle *ao);
    mpg123_handle *mh = NULL;
    out123_handle *ao = NULL;
    State _state = State::play_dead;
    Control_Requests_Enum _control_request;
    char * _file_name;
    char *driver = NULL;
    unsigned char *buffer = NULL;
    size_t buffer_size;
    size_t done;
    size_t played;
    int channels, encoding, err;
    long rate;
    bool _is_Playing{true};


};

#endif /* musicplayer_h */
