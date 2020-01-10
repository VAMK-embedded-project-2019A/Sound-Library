#include "musicplayer.h"

#include <thread>
#include <chrono>
#include <iostream>

MusicPlayer::MusicPlayer()
{
    int err = mpg123_init();
	if(err != MPG123_OK)
	{
        fprintf(stderr, "Basic setup goes wrong : %s\n", mpg123_plain_strerror(err));
        setError(true);
		return;
    }
    _mpg_handle = mpg123_new(nullptr, &err);
    if(err != MPG123_OK || _mpg_handle == nullptr)
    {
        fprintf(stderr, "Basic setup goes wrong : %s\n", mpg123_plain_strerror(err));
        setError(true);
		return;
    }

    _out_handle = out123_new();
    if(!_out_handle)
	{
        fprintf(stderr, "Cannot create output handle \n");
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
	bool handles_opened{false};
	while(true)
	{
		if(hasError())
			break;
		
		auto request = getRequest();
		switch(request)
		{
			case ControlRequest::Play:
				if(handles_opened)
					break;
				loadSong();
				handles_opened = true;
				setIsPlaying(true);
				break;
			case ControlRequest::Stop:
				if(!handles_opened)
					break;
				out123_close(_out_handle);
				mpg123_close(_mpg_handle);
				handles_opened = false;
				setIsPlaying(false);
				break;
			case ControlRequest::Pause:
				if(handles_opened)
					setIsPlaying(false);
				break;
			case ControlRequest::Resume:
				if(handles_opened)
					setIsPlaying(true);
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
		{
			std::cout << "end" << std::endl;
			out123_close(_out_handle);
			mpg123_close(_mpg_handle);
			handles_opened = false;
			setIsPlaying(false);
		}
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

void MusicPlayer::setCurrentSong(const std::string &name)
{
	std::unique_lock<std::mutex> music_player_lock(_mutex);
	_current_song = name;
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
        fprintf(stderr, "Trouble with mpg123_open: %s \n", mpg123_strerror(_mpg_handle));
        setError(true);
		return;
    }
	
	int channels, encoding;
    long rate;
    err = mpg123_getformat(_mpg_handle, &rate, &channels, &encoding);
    if(err != MPG123_OK)
    {
        fprintf(stderr, "Trouble with mpg123_getformat: %s \n", mpg123_strerror(_mpg_handle));
        setError(true);
		return;
    }

    err = out123_open(_out_handle, nullptr, nullptr);
    if(err != 0)
    {
        fprintf(stderr, "Trouble with out123: %s\n", out123_strerror(_out_handle));
        setError(true);
		return;
    }
    mpg123_format(_mpg_handle, rate, channels, encoding);
    out123_start(_out_handle, rate, channels, encoding);

    _out_block_size = mpg123_outblock(_mpg_handle);
	if(_out_block)
	{
		delete []_out_block;
		_out_block = nullptr;
	}
	_out_block = new unsigned char[_out_block_size];
}