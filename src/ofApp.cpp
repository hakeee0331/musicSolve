/**
 * performance control test -> midi out channel 2 사용
 *
 */

#include "ofApp.h"

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
	sceneManager.setup();
	
}

void ofApp::update(){
	uint64_t now = ofGetElapsedTimeMillis();
	
	midiOut.update();
	if (midiIn.update()) {
		sceneManager.nextScene();
	}
	
	if (isTypping && now - typeStart >= 1500) {
		typed = "";
		isTypping = false;
	}
	
	sceneManager.update();
}

void ofApp::draw(){
	ofBackground(0);
	
	sceneManager.draw();
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
}

void ofApp::exit(){
	midiIn.exit();
}

void ofApp::keyPressed(int key){
	midiOut.keyPressed(key);
	if (sceneManager.keyPressed(key)) {
		midiOut.changeScene(sceneManager.getCurrentSceneIndex());
	}
	
	if (key == OF_KEY_ESC) { panicReset(); return; }

	isTypping = true;
	typeStart = ofGetElapsedTimeMillis();
	if (key == OF_KEY_BACKSPACE) {
		if (!typed.empty()) typed.pop_back();
	} else typed += (char)key;

}

void ofApp::keyReleased(int key){
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

