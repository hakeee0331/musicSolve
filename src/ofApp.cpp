/**
 * performance control test -> midi out channel 2 사용
 *
 */

#include "ofApp.h"

//--------------------------------------------------------------

std::string ofApp::getPatternDisplayText() {
	char patternInput[] = {'_','_','_','_',};
	
	if (!typed.empty()) {
		for (int i = 0; i < patternLength && typed.size() > i; i++) {
			patternInput[i] = typed.at(i);
		}
	}
	
	
	std::string text = "";
	for (auto& x: patternInput) {
		text += x;
		text += ' ';
	}
	
	
	return text;
}

void ofApp::debugDraw() {
	// guide line
	ofSetColor(100);
	ofDrawLine(0, ofGetHeight() / 2, ofGetWidth(), ofGetHeight() / 2);
	ofDrawLine(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight());
	
	midiIn.draw();
}

void ofApp::setup(){
	ofSetEscapeQuitsApp(false);
	
	midiOut.setup();
	midiIn.setup();
	
}

//--------------------------------------------------------------
void ofApp::update(){
	uint64_t now = ofGetElapsedTimeMillis();
	
	midiOut.update();
	midiIn.update();
	
	if (isTypping && now - typeStart >= 1500) {
		typed = "";
		isTypping = false;
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	
	debugDraw();
	
	ofSetColor(255);
	std::string debugText = "";
	for (auto& key: midiOut.getPressedKeys()) {
		debugText = debugText + '\n'
		+ "KEY PRESSED:" + ofToString(key) + " NOTE: " + ofToString(midiOut.key2midi(key));
	}
	ofDrawBitmapString("test", 0, ofGetHeight());
	ofDrawBitmapString(debugText, 50, 50);
	ofDrawBitmapString(typed, 50, 100);
	
	
	// draw pattern input
	std::string patternText = getPatternDisplayText();
	float scale = 3.f;
	
	ofRectangle bounds = bitmapFont.getBoundingBox(patternText, 0, 0, OF_BITMAPMODE_SIMPLE, true);
	
	float scaledWidth = bounds.getWidth() * scale;
	float scaledHeight = bounds.getHeight() * scale;
	
	float x = ofGetWidth() / 2.0f - scaledWidth / 2.0f;
	float y = ofGetHeight() / 2.0f - scaledHeight / 2.0f;
	
	ofPushMatrix();
	
	ofTranslate(x, y);
	ofScale(scale, scale);
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	ofDrawBitmapString(patternText, 0, bounds.getHeight() / 2.0f);
	ofSetDrawBitmapMode(OF_BITMAPMODE_SCREEN);
	
	ofPopMatrix();
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::exit(){
	midiIn.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	/* performance control test
	if (key == '1') {
		midiOut.sendNoteOn(MIDI_CONTROL_CHANNEL, 0, MIDI_VELOCITY);
	}
	if (key == '4') {
		midiOut.sendNoteOn(MIDI_CONTROL_CHANNEL, 15, MIDI_VELOCITY);
	}
	if (key == '0') {
		midiOut.sendNoteOn(MIDI_CONTROL_CHANNEL, 125, MIDI_VELOCITY);
	} --- test end */
	
	
	midiOut.keyPressed(key);
	if (key == OF_KEY_ESC) { panicReset(); return; }

	isTypping = true;
	typeStart = ofGetElapsedTimeMillis();
	if (key == OF_KEY_BACKSPACE) {
		if (!typed.empty()) typed.pop_back();
	} else typed += (char)key;

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	/* performance control test
	if (key == '0') {
		midiOut.sendNoteOff(MIDI_CONTROL_CHANNEL, 125, MIDI_VELOCITY);
	} --- end test */
	
	
	midiOut.keyReleased(key);
}

void ofApp::panicReset() {
	typed = "";
	isTypping = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
