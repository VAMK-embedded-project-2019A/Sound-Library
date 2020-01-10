#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <string>
#include <mutex>

#include "mpg123.h"
#include "out123.h"

class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();
	
	enum ControlRequest
	{
		Play,
		Stop,
        Pause,
        Resume,
        Next,
        Prev,
        None
    };
	
	void start();
	bool isPlaying();
	bool hasError();
	std::string getCurrentSong();
	
	void setCurrentSong(const std::string &name);
	bool control(ControlRequest request);
	
private:
	void setError(bool error);
	void setIsPlaying(bool is_playing);
	ControlRequest getRequest();
	void loadSong();

	mpg123_handle *_mpg_handle = nullptr;
    out123_handle *_out_handle = nullptr;
	size_t _out_block_size;
	unsigned char *_out_block{nullptr};

	std::string _current_song;
	bool _is_playing{false};
	ControlRequest _control_request{None};
	
	bool _error{false};
	std::mutex _mutex;
};

#endif