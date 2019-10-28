#include "musicplayer.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>

MusicPlayer::MusicPlayer()
{
//  initialize output device
    device = new RtAudio();
    stream = new StreamParameters();
    
    if(device.getDeviceCount() == 0){
        std::cout << "No audio/output devices found\n";
        exit (0);
    }
    
    stream->deviceId = device.getDefaultOutputDevice();
    stream->nchannels = 2;
    stream->firstChannel = 0;
    
    RtAudio::StreamOptions option;
    option.flags = RTAUDIO_NONINTERLEAVED; //default is interleaved buffer
}

MusicPlayer::~MusicPlayer(){
    device->closeStream();
}



int MusicPlayer::Output(void * outputBuff, void *, BUFFER_FRAME, double, RtAudioStreamStatus, void *data){
    RtAudio::OutputData *outData = (OutputData*) data;
    
    unsigned int count = fread(outputBuff, outData->channel * sizeof(MY_TYPE), BUFFER_FRAME, outData->fd);

    if(count < BUFFER_FRAME){
        unsigned int bytes = (BUFFER_FRAME - count) * outData->channel * sizeof(MY_TYPE);
        unsigned int startByte = count * outData->channel * sizeof(MY_TYPE);
        memset((chhar *)(outputBuff) + startByte, 0, bytes);
        return 1;
    }
    return 0;
}


void MusicPlayer::setFileName(std::string file_name)
{
    if(file_name.empty())
        return;
    
    _file_name = file_name;
    _file_name.loadfile();
}

void MusicPlayer::loadfile(){
    
    char *file = _file_name;
    OutputData data;
    data.fd = fopen(file, "rb");
    if(!data.fd){
        std::cout << "Cannot find file\n";
        exit(1);
    }
    
    try {
        device->openStream(&stream, NULL, FORMAT, SAMPLE_RATE, &BUFFER_FRAME, &Output, (void *)&data);
    }
    catch(RtAudioError& e){
        std::cout << e.printMessage() << '\n';
        fclose(data.fd);
        device->closeStream();
    }
}


void MusicPlayer::play()
{
    device->startStream();
}

void MusicPlayer::pause()
{
    device->stopStream();
}

bool MusicPlayer::hasError() const
{
        return _is_error;
}
