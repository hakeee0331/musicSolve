#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include <unordered_map>
#include <unordered_set>
using namespace std;

class MidiOutManager {
private:
	ofxMidiOut 				manager;
	const string 			MIDI_PORT;
	const int				MIDI_CHANNEL;
	const int				MIDI_CONTROL_CHANNEL;
	const int				MIDI_VELOCITY;
	
	unordered_map<int, int>	keyToMidi;
	unordered_set<int> 		pressedKeys;
	unordered_set<int> 		soundingNotes;

public:
	MidiOutManager(string port, int channel, int controlChannel, int velocity):
		MIDI_PORT(port),
		MIDI_CHANNEL(channel),
		MIDI_CONTROL_CHANNEL(channel),
		MIDI_VELOCITY(velocity) {}
	
	void setup();
	void update();
	void keyPressed(int key);
	void keyReleased(int key);
	
	void setMapping();
	void panicReset();
	
	const unordered_set<int>& getPressedKeys () const;
	int key2midi(int key);
	
};
