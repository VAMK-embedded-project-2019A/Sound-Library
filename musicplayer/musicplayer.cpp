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

#include "../include/irrKlang.h"
#include "musicplayer.h"

MusicPlayer::MusicPlayer()
{
    engine = createIrrKlangDevice();
    if(!engine)
    {
        std::cout << "Error engine starting\n" << std::endl;
        exit(0);
    }
    
    sound = 0;
    _file_name = "\0";
    position = 0;
}

MusicPlayer::~MusicPlayer()
{
    engine->drop();
    sound->drop();
}

void MusicPlayer::setFileName(std::string file_name)
{
    if(file_name.empty())
        return ;
    
    else
    {
      _file_name = file_name;

    }
}

std::string MusicPlayer::getFileName()
{
    return _file_name;
}

void MusicPlayer::play()
{
    if(_file_name == "\0")
    {
        std::cout << "Error: No sound file detected" << std::endl;
        return;
    }
    
    else
    {
        sound = engine->play2D(_file_name.c_str(), false, false, true);
        position = sound->getPlayPosition();
    }

    
    if(!sound)
    {
        std::cout << "Error: Could not play this file\n" << std::endl;
        exit(0);
    }

}

void MusicPlayer::pause()
{
        position = sound->getPlayPosition();
        sound->setIsPaused(true);
}
    
void MusicPlayer::resume()
{
    sound = engine->play2D(_file_name.c_str(), false, false, true);
    if(position != -1)
        sound->setPlayPosition(position);
}
    
void MusicPlayer::stop()
{
    engine->drop();
}

bool MusicPlayer::isPlaying()
{
    return !sound->isFinished();
}

//bool MusicPlayer::hasError() const
//{
//        return _is_error;
//}

