//
//  main.cpp
//  musicplayer
//
//  Created by Linh Nguyen on 30.10.2019.
//  Copyright © 2019 Linh Nguyen. All rights reserved.
//

#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <future>
#include <mpg123.h>
#include <out123.h>
#include <mutex>
#include <string>
#include <unistd.h>

#include "musicplayer.h"


int main(int argc, const char * argv[]) {
    std::cout << "Hello, World of Music!\n";
    char option;
    MusicPlayer song;
    song.setFileName("Portals - Alan Silvestri.mp3");
    song.loadSong();

    auto _future_song = std::async(std::launch::async, &MusicPlayer::play, &song);
    while(_future_song.wait_for(std::chrono::seconds(0)) == std::future_status::deferred);
    std::cout << "abc\n";
    std::cout << song.isPlaying() << '\n';
    while(song.isPlaying()){
        std::cout << "To stop the song, press: q\n" << std::endl
                  << "To pause the song, press: p \n" << std::endl
                  << "To resume the song, press: r \n" << std::endl;
        std::cout << "Options for the song: ";
        std::cin >> option;
        song.Control_Requested(MusicPlayer::Pause);
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
}
