#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include <unordered_map>
#include <unordered_set>

class ofApp : public ofBaseApp{
public:
	ofBitmapFont bitmapFont;
	
	ofxMidiOut midiOut;
	const string MIDI_PORT = "IAC Driver ofx_to_FL";
	const int MIDI_CHANNEL = 1;
	const int MIDI_CONTROL_CHANNEL = 2;
	const int MIDI_VELOCITY = 100;
	
	std::unordered_map<int, int> keyToMidi;		// key mapping
	
	std::unordered_set<int> pressedKeys;
	std::unordered_set<int> soundingNotes;
	
	std::string typed ="";
	bool isTypping = false;
	uint64_t typeStart = 0;
	
	std::vector<char> patternInput;
	int patternLength = 4;
	std::string getPatternDisplayText();
	
	void setKeyToMIDI();
	void sendNoteOn(int note);
	void sendNoteOff(int note);
	
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
