#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#define SAMPLE_RATE 44100
#define BUFFER_FRAME 256

#include <string>

namespace RtAudio {
    struct StreamParameters;
}
class RtAudio;

class MusicPlayer
{
public:
	MusicPlayer();
    ~MusicPlayer();
    
	void setFileName(std::string file_name);
	void play();
	void pause();
	bool hasError() const;
	
private:
    RtAudio *device;
    RtAudio::StreamParameters *stream;
	std::string _file_name;
    int Output(void * outputBuff, void *, BUFFER_FRAME, double, RtAudioStreamStatus, void *data);
    void loadfile();
    
	bool _is_error{false};
};

#endif // MUSICPLAYER_H
