//
//  main.cpp
//  musicplayer
//
//  Created by Linh Nguyen on 30.10.2019.
//  Copyright © 2019 Linh Nguyen. All rights reserved.
//  Modification 03.12.2019 Linh Nguyen

#include <iostream>
#include <cstring>
#include <string>
#include <mpg123.h>
#include <fcntl.h>
#include <unistd.h>

#include "musicplayer.h"


int main(int argc, const char * argv[]) {
    std::cout << "Hello, World of Testing!\n";
    char option;
    MusicPlayer song;
    song.setFileName("ophelia.mp3");
    song.loadSong();
    song.play();

    std::cout << " To stop the song, press: q\n" << std::endl
              << "To pause the song, press: p \n" << std::endl
              << "To resume the song, press: r \n" << std::endl;

    while(song.isPlaying())
    {
        std::cout << "Options for the song: ";
        std::cin >> option;

        switch (option) {
            case 'q':
                song.stop();
                std::cout << "Song is stopped\n";
                break;

            case 'p':
                song.pause();
                std::cout << "Song is paused\n";
                break;

            case 'r':
                song.resume();
                std::cout << "Song is continued playing\n";
                break;

            default:
                break;
        }
    }
    return 0;
}
