#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <string>
#include <mutex>
#include <vector>

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
	std::string getNextSong();
	
	void setCurrentSong(const std::string &name);
	void setNextSong(const std::string &name);
	bool control(ControlRequest request);
	
private:
	void setError(bool error);
	void setIsPlaying(bool is_playing);
	void setHandlesOpened(bool handles_opened);
	bool handlesOpened();
	ControlRequest getRequest();
	void loadSong();
	
	void handlePlayRequest();
	void handleStopRequest();
	void handleNextRequest();
	void handlePrevRequest();

	mpg123_handle *_mpg_handle = nullptr;
    out123_handle *_out_handle = nullptr;
	size_t _out_block_size;
	unsigned char *_out_block{nullptr};

	std::string _current_song;
	std::string _next_song;
	bool _is_playing{false};
	bool _handles_opened{false};
	ControlRequest _control_request{None};
	
	std::vector<std::string> _song_history;
	const unsigned int MAX_HISTORY{10};
	
	bool _error{false};
	std::mutex _mutex;
};

#endif