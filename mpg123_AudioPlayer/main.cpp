//
//  main.cpp
//  musicplayer
//
//  Created by Linh Nguyen on 30.10.2019.
//  Copyright © 2019 Linh Nguyen. All rights reserved.
//

#include <ao/ao.h>
#include <iostream>
#include <cstring>
#include <string>
#include <mpg123.h>
#include <fcntl.h>
#include <unistd.h>

#include "musicplayer.h"


int main(int argc, const char * argv[]) {
    std::cout << "Hello, World of Testing!\n";
    
    ao_initialize();
    int driver = ao_default_driver_id();
    
    ao_sample_format format;
    format.bits = 16;
    format.rate = 44100;
    format.channels = 2;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    
    ao_device *device;
    device = ao_open_live(ao_default_driver_id(), &format, NULL);
    
    int err = mpg123_init();
    if(err != MPG123_OK){
        std::cout << "Error basic setup, " << mpg123_plain_strerror(err) << "\n";
    }
    
    MusicPlayer song;
    song.setFileName("ophelia.mp3");
    song.play();
    
    ao_close(device);
    mpg123_exit();
    ao_shutdown();

    return 0;
}
