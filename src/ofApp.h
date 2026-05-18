#pragma once

#include "ofMain.h"
#include "midi/MidiOutManager.h"
#include "midi/MidiInManager.h"
#include "Scene/SceneManager.h"



class ofApp : public ofBaseApp {
private:
	MidiOutManager midiOut{"IAC Driver ofx_to_FL", 1, 2, 100};
	MidiInManager midiIn{"IAC Driver FL_to_ofx"};
	SceneManager sceneManager;
	
public:
	ofBitmapFont bitmapFont;
	
	std::string typed ="";
	bool isTypping = false;
	uint64_t typeStart = 0;
	
	std::vector<char> patternInput;
	int patternLength = 4;
	std::string getPatternDisplayText();
	
	void panicReset();
	void debugDraw();
	
	
	
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
