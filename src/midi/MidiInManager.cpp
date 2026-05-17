#include "MidiInManager.h"

void MidiInManager::setup() {
	manager.listInPorts();
	manager.openPort(MIDI_PORT);
	manager.ignoreTypes(false, false, false);
	manager.addListener(this);
	manager.setVerbose(true);
}
void MidiInManager::update() {
	
	
	debug.clockUpdate();
}

void MidiInManager::draw() {
	
	
	debug.clockDraw(clock, 0, ofGetHeight() / 2 );
	debug.midiInDraw(midiMessage, ofGetWidth() / 2 + 50, 0);
}

void MidiInManager::exit() {
	manager.removeListener(this);
	manager.closePort();
}

void MidiInManager::newMidiMessage(ofxMidiMessage& msg) {
	//Clock Update
	if (msg.status == MIDI_TIME_CLOCK) {
		clock.clockCount++;
		
		// 24틱마다 1비트
		if (clock.clockCount >= 24) {
			clock.clockCount = 0;
			clock.beatCount++;
			
			uint64_t now = ofGetElapsedTimeMicros();
			if (clock.lastBeatTime != 0) {
				uint64_t diff = now - clock.lastBeatTime;
				float secondsPerBeat = diff / 1000000.f;
				clock.bpm = 60 / secondsPerBeat;
			}
			clock.lastBeatTime = now;
			
			// for debug
			ofLogNotice() << "BEAT " << clock.beatCount << " | BPM: " << clock.bpm;
			debug.pulseTarget = 1.f;
		}
		return;
	}
	
	//Midi In update for debug
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
	debug.lastMidiText = (
						  typeText
						  + " | channel: " + ofToString(msg.channel)
						  + " | pitch: " + ofToString(msg.pitch)
						  + " | velocity: " + ofToString(msg.velocity)
						  + " | control: " + ofToString(msg.control)
						  + " | value: " + ofToString(msg.value)
						  );
//	ofLogNotice() << debug.lastMidiText;
}

//--- Debug ---

void MidiInManager::Debug::midiInUpdate() {
	
}

void MidiInManager::Debug::midiInDraw(ofxMidiMessage& msg, int x = 0, int y = 0) {
	ofDrawBitmapString("MIDI INPUT TEST", x, y + 50);
	ofDrawBitmapString(lastMidiText, x, y + 90);

	
	ofDrawBitmapString("Last status: " + ofToString(msg.status), x, y + 140);
	ofDrawBitmapString("Channel: " + ofToString(msg.channel), x, y + 170);
	ofDrawBitmapString("Pitch: " + ofToString(msg.pitch), x, y + 200);
	ofDrawBitmapString("Velocity: " + ofToString(msg.velocity), x, y + 230);
	ofDrawBitmapString("Control: " + ofToString(msg.control), x, y + 260);
	ofDrawBitmapString("Value: " + ofToString(msg.value), x, y + 290);
}

void MidiInManager::Debug::clockUpdate() {
	pulse += (pulseTarget - pulse) * 0.35f;
	pulseTarget *= 0.75f;
	if (pulse < 0.001f) pulse = 0.f;
}

void MidiInManager::Debug::clockDraw(const Clock& clock, int x = 0, int y = 0) {
	ofSetColor(255);
	
	ofDrawBitmapString("MIDI Clock Pulse Test", x + 50, y + 50);
	ofDrawBitmapString("BPM: " + ofToString(clock.bpm, 2), x + 50, y + 80);
	ofDrawBitmapString("Clock Count: " + ofToString(clock.clockCount), x + 50, y + 110);
	ofDrawBitmapString("Beat Count: " + ofToString(clock.beatCount), x + 50, y + 140);
	ofDrawBitmapString("Pulse: " + ofToString(pulse, 2), x + 50, y + 170);
	
	float radius = baseRadius + pulse * pulseRadius;
	ofDrawCircle(x + 400, y + 110, radius);
}
