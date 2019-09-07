// Sound.cpp

#include "Main.hpp"
#include "Engine.hpp"
#include "Sound.hpp"

Sound::Sound(const char* _name) : Asset(_name) {
	path = mainEngine->buildPath(_name).get();

	mainEngine->fmsg(Engine::MSG_DEBUG,"loading sound '%s'...",_name);
	if( (chunk=Mix_LoadWAV(path.get())) == NULL ) {
		mainEngine->fmsg(Engine::MSG_ERROR, "unable to load sound file '%s': %s", _name, Mix_GetError());
		return;
	} 

	loaded = true;
}

Sound::~Sound() {
	Mix_FreeChunk(chunk);
}

int Sound::play(const bool loop) {
	int loops = loop ? -1 : 0;
	int channel = Mix_PlayChannel(-1, chunk, loops);
	if( channel != -1 ) {
		Mix_Volume(channel,32);
	}
	return channel;
}