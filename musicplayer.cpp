#include "musicplayer.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>

MusicPlayer::MusicPlayer()
{
	int err = mpg123_init();
	if(err != MPG123_OK)
	{
		std::cout << "Basic setup goes wrong: " << mpg123_plain_strerror(err) << std::endl;
		setError(true);
		return;
	}
	_mpg_handle = mpg123_new(nullptr, &err);
	if(err != MPG123_OK || _mpg_handle == nullptr)
	{
		std::cout << "Basic setup goes wrong: " << mpg123_plain_strerror(err) << std::endl;
		setError(true);
		return;
	}
	mpg123_volume(_mpg_handle, 0.025);

	_out_handle = out123_new();
	if(!_out_handle)
	{
		std::cout << "Cannot create output handle" << std::endl;
		setError(true);
		return;
	}
}

MusicPlayer::~MusicPlayer()
{
	if(_out_block)
	{
		delete []_out_block;
		_out_block = nullptr;
	}
	out123_del(_out_handle);
	mpg123_delete(_mpg_handle);
}

void MusicPlayer::start()
{
	while(true)
	{
		if(hasError())
			break;
		
		if(_waiting_next_song)
			handlePlayRequest();
		
		auto request = getRequest();
		switch(request)
		{
			case ControlRequest::Play:
				handlePlayRequest();
				break;
			case ControlRequest::Stop:
				handleStopRequest();
				break;
			case ControlRequest::Pause:
				_waiting_next_song = false;
				if(_handles_opened)
					setIsPlaying(false);
				break;
			case ControlRequest::Resume:
				if(_handles_opened)
					setIsPlaying(true);
				break;
			case ControlRequest::Next:
				handleNextRequest();
				break;
			case ControlRequest::Prev:
				handlePrevRequest();
				break;
			case ControlRequest::VolumeUp:
			{
				double base, really, rva;
				mpg123_getvolume(_mpg_handle, &base, &really, &rva);
				if(really + VOLUME_PRECISION > MAX_VOLUME)
					break;
				mpg123_volume_change(_mpg_handle, VOLUME_PRECISION);
				break;
			}
			case ControlRequest::VolumeDown:
				mpg123_volume_change(_mpg_handle, -VOLUME_PRECISION);
				break;
		}
			
		if(!isPlaying())
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}
		
		size_t written_bytes_count;
		int err = mpg123_read(_mpg_handle, _out_block, _out_block_size, &written_bytes_count);
		out123_play(_out_handle, _out_block, written_bytes_count);
		if(written_bytes_count == 0 || err != MPG123_OK)
			handleNextRequest();
	}
}

bool MusicPlayer::isPlaying()
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	return _is_playing;
}

bool MusicPlayer::hasError()
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	return _error;
}

std::string MusicPlayer::getCurrentSong()
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	return _current_song;
}

std::string MusicPlayer::getNextSong()
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	return _next_song;
}

int MusicPlayer::getVolume()
{
	double base, really, rva;
	if(mpg123_getvolume(_mpg_handle, &base, &really, &rva) != MPG123_OK)
		return -1;
	return std::lround(really/VOLUME_PRECISION);
}

void MusicPlayer::setCurrentSong(const std::string &name)
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	_current_song = FILE_PATH + name;
}

void MusicPlayer::setNextSong(const std::string &name)
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	_next_song = FILE_PATH + name;
}

bool MusicPlayer::control(ControlRequest request)
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	if(_control_request != ControlRequest::None)
		return false;
	_control_request = request;
	return true;
}

void MusicPlayer::setError(bool error)
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	_error = error;
}

void MusicPlayer::setIsPlaying(bool is_playing)
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	_is_playing = is_playing;
}

MusicPlayer::ControlRequest MusicPlayer::getRequest()
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	auto request = _control_request;
	_control_request = ControlRequest::None;
	return request;
}

void MusicPlayer::loadSong()
{
	int err = mpg123_open(_mpg_handle, getCurrentSong().c_str());
	if(err != MPG123_OK)
	{
		std::cout << "Trouble with mpg123_open: " << mpg123_strerror(_mpg_handle) << std::endl;
		setError(true);
		return;
	}
	
	int channels, encoding;
	long rate;
	err = mpg123_getformat(_mpg_handle, &rate, &channels, &encoding);
	if(err != MPG123_OK)
	{
		std::cout << "Trouble with mpg123_getformat: " << mpg123_strerror(_mpg_handle) << std::endl;
		setError(true);
		return;
	}

	err = out123_open(_out_handle, nullptr, nullptr);
	if(err != 0)
	{
		std::cout << "Trouble with out123: " << out123_strerror(_out_handle) << std::endl;
		setError(true);
		return;
	}
	mpg123_format(_mpg_handle, rate, channels, encoding);
	out123_start(_out_handle, rate, channels, encoding);
	_handles_opened = true;
}

void MusicPlayer::handlePlayRequest()
{
	if(_handles_opened)
		return;
	
	// play WAITING_TRACK if no song available
	auto current_song = getCurrentSong();
	if(current_song.empty())
	{
		if(!_waiting_next_song)
		{
			setCurrentSong(WAITING_TRACK);
			_waiting_next_song = true;
		}
		else
			return;
	}
	else if(current_song != WAITING_TRACK)
		_waiting_next_song = false;
	
	loadSong();
	_out_block_size = mpg123_outblock(_mpg_handle);
	_out_block = new unsigned char[_out_block_size];
	
	_handles_opened = true;
	setIsPlaying(true);
}

void MusicPlayer::handleStopRequest()
{
	if(!_handles_opened)
		return;
	
	delete []_out_block;
	_out_block = nullptr;
		
	out123_close(_out_handle);
	mpg123_close(_mpg_handle);
	_handles_opened = false;
	setIsPlaying(false);
}

void MusicPlayer::handleNextRequest()
{
	auto current_song = getCurrentSong();
	auto next_song = getNextSong();
	if(!current_song.empty() && current_song != WAITING_TRACK)
	{
		while(_song_history.size() >= MAX_HISTORY)
			_song_history.erase(_song_history.begin());
		_song_history.push_back(current_song);
	}
	setCurrentSong(next_song);
	setNextSong("");
	
	handleStopRequest();
	handlePlayRequest();
}

void MusicPlayer::handlePrevRequest()
{
	if(_song_history.empty())
		return;
	setCurrentSong(_song_history.back());
	_song_history.pop_back();
	
	handleStopRequest();
	handlePlayRequest();
}