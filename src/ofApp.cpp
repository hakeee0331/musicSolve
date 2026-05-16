/**
 * performance control test -> midi out channel 2, MIDI_CONTROL_CHANNEL 사용
 *
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::sendNoteOn(int note) {
	midiOut.sendNoteOn(MIDI_CHANNEL, note, 100);
}
void ofApp::sendNoteOff(int note) {
	midiOut.sendNoteOff(MIDI_CHANNEL, note, 0);
}

void ofApp::setKeyToMIDI() {
	// Basic Setting
	keyToMidi[' '] = 48; // C5
	keyToMidi[OF_KEY_BACKSPACE] = 84; // C7
	
	keyToMidi['z'] = 48; // C5
	keyToMidi['x'] = 50; // D5
	keyToMidi['c'] = 52; // E5
	keyToMidi['v'] = 53; // F5
	keyToMidi['b'] = 55; // G5
	keyToMidi['n'] = 57; // A5
	keyToMidi['m'] = 59; // B5
	keyToMidi[','] = 60; // C6
	keyToMidi['.'] = 62; // D6
	keyToMidi['/'] = 64; // E6
	
	keyToMidi['a'] = 60; // C5
	keyToMidi['s'] = 62; // D5
	keyToMidi['d'] = 64; // E5
	keyToMidi['f'] = 65; // F5
	keyToMidi['g'] = 67; // G5
	keyToMidi['h'] = 69; // A5
	keyToMidi['j'] = 71; // B5
	keyToMidi['k'] = 72; // C6
	keyToMidi['l'] = 74; // D6
	keyToMidi[';'] = 76; // E6
	keyToMidi['\''] = 77; // F6
	
	keyToMidi['q'] = 72; // C6
	keyToMidi['w'] = 74; // D6
	keyToMidi['e'] = 76; // E6
	keyToMidi['r'] = 77; // F6
	keyToMidi['t'] = 79; // G6
	keyToMidi['y'] = 81; // A6
	keyToMidi['u'] = 83; // B6
	keyToMidi['i'] = 84; // C7
	keyToMidi['o'] = 86; // D7
	keyToMidi['p'] = 88; // E7
	keyToMidi['['] = 89; // F7
	keyToMidi[']'] = 91; // G7
}

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

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	if (msg.status == MIDI_TIME_CLOCK) {
		clockCount++;
		
		if (clockCount >= 24) {
			clockCount = 0;
			beatCount++;
			
			pulseTarget = 1.f;
			uint64_t now = ofGetElapsedTimeMicros();
			
			if (lastBeatTime != 0) {
				uint64_t diff = now - lastBeatTime;
				float secondsPerBeat = diff / 1000000.f;
				bpm = 60 / secondsPerBeat;
			}
			
			lastBeatTime = now;
			
			ofLogNotice() << "BEAT " << beatCount << " | BPM: " << bpm;
		}
		return;
	}
	
	midiMessage = msg;
	
	string typeText;
	
	switch (msg.status) {
		case MIDI_NOTE_ON:
			typeText = "NOTE ON";
			break;
		case MIDI_NOTE_OFF:
			typeText = "NOTE OFF";
			break;
		case MIDI_CONTROL_CHANGE:
			typeText = "CONTROL CHANGE";
			break;
			
		default:
			typeText = "OTHER";
			break;
	}
	
	lastMidiText =
			typeText +
			" | channel: " + ofToString(msg.channel) +
			" | pitch: " + ofToString(msg.pitch) +
			" | velocity: " + ofToString(msg.velocity) +
			" | control: " + ofToString(msg.control) +
			" | value: " + ofToString(msg.value);
	
	ofLogNotice() << lastMidiText;
}

void ofApp::setup(){
	ofSetEscapeQuitsApp(false);
	
	midiOut.listOutPorts();
	midiOut.openPort(MIDI_PORT);
	
	// midi in test
	midiIn.listInPorts();
	midiIn.openPort(MIDI_IN_PORT);
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);
	midiIn.setVerbose(true);
	
	setKeyToMIDI();
	
}

//--------------------------------------------------------------
void ofApp::update(){
	uint64_t now = ofGetElapsedTimeMillis();
	

	for (auto& key: pressedKeys) {
		int note = keyToMidi.at(key);
		if (soundingNotes.contains(note)) continue;
		midiOut.sendNoteOn(MIDI_CHANNEL, note, MIDI_VELOCITY);
		soundingNotes.insert(note);
	}
	
	if (isTypping && now - typeStart >= 1500) {
		typed = "";
		isTypping = false;
	}
	
	// BPM TEST
	pulse += (pulseTarget - pulse) * 0.35f;
	pulseTarget *= 0.75f;
	if (pulse < 0.001f) {
		pulse = 0.0f;
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackground(0);
	
	ofSetColor(100);
	ofDrawLine(0, ofGetHeight() / 2, ofGetWidth(), ofGetHeight() / 2);
	ofDrawLine(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight());
	
	ofSetColor(255);
	std::string debugText = "";
	for (auto& key: pressedKeys) {
		debugText = debugText + '\n'
		+ "KEY PRESSED:" + ofToString(key) + " NOTE: " + ofToString(keyToMidi[key]);
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
	
	
	// MIDI IN TEST
	ofSetColor(255);
	ofDrawBitmapString("MIDI INPUT TEST", ofGetHeight() - 60, 50);
	ofDrawBitmapString(lastMidiText, ofGetHeight() - 60, 90);
	
	ofDrawBitmapString("Last status: " + ofToString(midiMessage.status), ofGetHeight() - 60, 140);
	ofDrawBitmapString("Channel: " + ofToString(midiMessage.channel), ofGetHeight() - 60, 170);
	ofDrawBitmapString("Pitch: " + ofToString(midiMessage.pitch), ofGetHeight() - 60, 200);
	ofDrawBitmapString("Velocity: " + ofToString(midiMessage.velocity), ofGetHeight() - 60, 230);
	ofDrawBitmapString("Control: " + ofToString(midiMessage.control), ofGetHeight() - 60, 260);
	ofDrawBitmapString("Value: " + ofToString(midiMessage.value), ofGetHeight() - 60, 290);
	
	// BPM TEST
	float radius = baseRadius + pulse * pulseRadius;
	ofSetColor(255);
	ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, radius);
	
	ofDrawBitmapString("MIDI Clock Pulse Test", 50, ofGetHeight() - 250);
	ofDrawBitmapString("BPM: " + ofToString(bpm, 2), 50, ofGetHeight() - 220);
	ofDrawBitmapString("Clock Count: " + ofToString(clockCount), 50, ofGetHeight() - 190);
	ofDrawBitmapString("Beat Count: " + ofToString(beatCount), 50, ofGetHeight() - 160);
	ofDrawBitmapString("Pulse: " + ofToString(pulse, 2), 50, ofGetHeight() - 130);
	
	
}

//--------------------------------------------------------------
void ofApp::exit(){
	// MIDI IN TEST
	midiIn.removeListener(this);
	midiIn.closePort();
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
	
	
	
	if (key == OF_KEY_ESC) { panicReset(); return; }
	if (!keyToMidi.contains(key)) { return; }
	if (pressedKeys.contains(key)) { return; }
	if (soundingNotes.contains(keyToMidi.at(key))) return;
	

	isTypping = true;
	typeStart = ofGetElapsedTimeMillis();
	if (key == OF_KEY_BACKSPACE) {
		if (!typed.empty()) typed.pop_back();
	} else typed += (char)key;
	
	pressedKeys.insert(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	/* performance control test
	if (key == '0') {
		midiOut.sendNoteOff(MIDI_CONTROL_CHANNEL, 125, MIDI_VELOCITY);
	} --- end test */
	
	
	if (!keyToMidi.contains(key)) { return; }
	if (!pressedKeys.contains(key)) { return; }

	pressedKeys.erase(key);
	
	int note = keyToMidi[key];
	
	if (soundingNotes.contains(note)) {
		midiOut.sendNoteOff(MIDI_CHANNEL, note, MIDI_VELOCITY);
		soundingNotes.erase(note);
	}
	
}

void ofApp::panicReset() {
	for (auto& [key, note]: keyToMidi) {
		sendNoteOff(note);
	}
	pressedKeys.clear();
	soundingNotes.clear();
	
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
