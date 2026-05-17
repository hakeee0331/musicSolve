#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
using namespace std;




class MidiInManager : public ofxMidiListener {
	struct Clock {
		int clockCount = 0;
		int beatCount = 0;
		uint64_t lastBeatTime = 0;
		float bpm = 0.f;
	};
	struct Debug {
		// clock & beat
		float pulse = 0.f;
		float pulseTarget = 0.f;
		float baseRadius = 80.f;
		float pulseRadius = 80.f;
		
		void clockUpdate();
		void clockDraw(const Clock& clock, int x, int y);
		
		// midi in
		string lastMidiText = "No Midi In...";
		
		void midiInUpdate();
		void midiInDraw(ofxMidiMessage& msg, int x, int y);
		
	};
private:
	ofxMidiIn 				manager;
	ofxMidiMessage			midiMessage;
	const string 			MIDI_PORT;
	Clock					clock;
	Debug					debug;
	

public:
	MidiInManager(string port) : MIDI_PORT(port) {}
	
	void newMidiMessage(ofxMidiMessage& msg) override;
	
	void setup();
	void update();
	void draw();
	void exit();
	
};

