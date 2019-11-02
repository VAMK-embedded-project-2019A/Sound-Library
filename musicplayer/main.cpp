//
//  main.cpp
//  musicplayer
//
//  Created by Linh Nguyen on 30.10.2019.
//  Copyright © 2019 Linh Nguyen. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string>
#include "../include/irrKlang.h"
#include "musicplayer.h"



int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    
    char option;
    MusicPlayer song;
    song.setFileName("getout.ogg");
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
                break;
                
            case 'p':
                song.pause();
                break;
                
            case 'r':
                song.resume();
                break;
                
            default:
                break;
        }
    }


    return 0;
}
