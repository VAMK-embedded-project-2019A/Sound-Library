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
    
    MusicPlayer song;
    song.setFileName("ophelia.mp3");
    song.loadSong();
    song.play();
    
    /*ao_close(device);
    mpg123_exit();
    ao_shutdown();*/

    return 0;
}
