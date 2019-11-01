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
    
    MusicPlayer song;
    song.setFileName("getout.ogg");
    do
    {
        song.play();
    }
    while(getch() != 'q');
    song.stop();

    return 0;
}
