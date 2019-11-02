//
//  musicplayer.h
//  musicplayer
//
//  Created by Linh Nguyen on 30.10.2019.
//  Copyright © 2019 Linh Nguyen. All rights reserved.
//

#ifndef musicplayer_h
#define musicplayer_h

#include <string>
#include <stdio.h>

#include "../include/irrKlang.h"

// include console I/O methods (conio.h for windows, our wrapper in linux)

#include "conio.h"
#pragma comment (lib, "irrKlang.lib") //link with irrKlang.dll

using namespace irrklang;

class MusicPlayer
{
    
public:
    MusicPlayer();
    ~MusicPlayer();
    
    void setFileName(std::string file_name);
    std::string getFileName();
    void play();
    bool isPlaying();
    void pause();
    void resume();
    void stop();
//    bool hasError() const;
    
private:
    std::string _file_name;
//    bool _is_error{false};
    ISoundEngine* engine;
    ISound * sound;
//    int volume;
    int position;
};

#endif /* musicplayer_h */
