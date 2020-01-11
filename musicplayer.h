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
		VolumeUp,
		VolumeDown,
        None
    };
	
	void start();
	bool isPlaying();
	bool hasError();
	std::string getCurrentSong();
	std::string getNextSong();
	int getVolume();
	
	void setCurrentSong(const std::string &name);
	void setNextSong(const std::string &name);
	bool control(ControlRequest request);
	
private:
	void setError(bool error);
	void setIsPlaying(bool is_playing);
	ControlRequest getRequest();
	void loadSong();
	
	void handlePlayRequest();
	void handleStopRequest();
	void handleNextRequest();
	void handlePrevRequest();

	// handles and lib properties
	mpg123_handle *_mpg_handle = nullptr;
    out123_handle *_out_handle = nullptr;
	size_t _out_block_size;
	unsigned char *_out_block{nullptr};

	// properties that can be accessed from many threads
	std::string _current_song;
	std::string _next_song;
	ControlRequest _control_request{None};
	bool _is_playing{false};
	bool _error{false};
	std::mutex _mutex;
	
	// private
	bool _handles_opened{false};
	std::vector<std::string> _song_history;
	const unsigned int MAX_HISTORY{10};
	const double MAX_VOLUME{0.5};
	const double VOLUME_PRECISION{0.005};
	const std::string FILE_PATH{"./"};
	const std::string WAITING_TRACK{"Downloading next song.mp3"};
	bool _waiting_next_song{false};
};

#endif