#include "musicplayer.h"

#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

int main()
{
	MusicPlayer mp;
	auto _future_song = std::async(std::launch::async, &MusicPlayer::start, &mp);
	while(_future_song.wait_for(std::chrono::seconds(0)) == std::future_status::deferred);
	mp.setCurrentSong("Alan Walker - Fade.mp3");
	mp.setNextSong("Alan Walker - Force.mp3");
	mp.control(MusicPlayer::ControlRequest::Play);
	while(!mp.isPlaying()); // waiting for mp to start playing
	while(true)
	{
		std::string option;
		std::cout << "Options: play/stop/pause/resume/next/prev: ";
		std::cin >> option;
		if(option == "play")
			mp.control(MusicPlayer::ControlRequest::Play);
		else if(option == "stop")
			mp.control(MusicPlayer::ControlRequest::Stop);
		else if(option == "pause")
			mp.control(MusicPlayer::ControlRequest::Pause);
		else if(option == "resume")
			mp.control(MusicPlayer::ControlRequest::Resume);
		else if(option == "next")
			mp.control(MusicPlayer::ControlRequest::Next);
		else if(option == "prev")
			mp.control(MusicPlayer::ControlRequest::Prev);
	}
	return 0;
}