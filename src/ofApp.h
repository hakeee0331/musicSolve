#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include <unordered_map>
#include <unordered_set>
#include "midi/MidiOutManager.h"

class ofApp : public ofBaseApp, public ofxMidiListener {
private:
	MidiOutManager midiOut{"IAC Driver ofx_to_FL", 1, 2, 100};

public:
	ofBitmapFont bitmapFont;
	
	const string MIDI_IN_PORT = "IAC Driver FL_to_ofx";
	
	std::string typed ="";
	bool isTypping = false;
	uint64_t typeStart = 0;
	
	std::vector<char> patternInput;
	int patternLength = 4;
	std::string getPatternDisplayText();
	
	void newMidiMessage(ofxMidiMessage& msg) override;
	
	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
	std::string lastMidiText = "No Midi In...";
	
	// MIDI CLOCK
	int clockCount = 0;
	int beatCount = 0;
	
	uint64_t lastBeatTime = 0;
	float bpm = 0.f;
	
	float pulse = 0.f;
	float pulseTarget = 0.f;
	float baseRadius = 80.f;
	float pulseRadius = 80.f;
	
	void panicReset();
	
	
	
	
	
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	
	
	void mouseMoved(int x, int y ) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	void mouseEntered(int x, int y) override;
	void mouseExited(int x, int y) override;
	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;
		
};
