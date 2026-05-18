#include "MidiOutManager.h"

void MidiOutManager::setup() {
	manager.listOutPorts();
	manager.openPort(MIDI_PORT);
	setMapping();
}

void MidiOutManager::update() {
	for (auto& key: pressedKeys) {
		int note = keyToMidi.at(key);
		if (soundingNotes.contains(note)) continue;
		manager.sendNoteOn(MIDI_CHANNEL, note, MIDI_VELOCITY);
		soundingNotes.insert(note);
	}
	
	if (pendingTime - ofGetElapsedTimeMillis() > 100) {
		offControlNote();
	}
	
}

void MidiOutManager::keyPressed(int key) {
	if (key == OF_KEY_ESC) { panicReset(); return; }
	if (!keyToMidi.contains(key)) { return; }
	if (pressedKeys.contains(key)) { return; }
	if (soundingNotes.contains(keyToMidi.at(key))) return;
	
	pressedKeys.insert(key);
}

void MidiOutManager::keyReleased(int key) {
	if (!keyToMidi.contains(key)) { return; }
	if (!pressedKeys.contains(key)) { return; }
	pressedKeys.erase(key);
	
	int note = keyToMidi[key];
	if (soundingNotes.contains(note)) {
		manager.sendNoteOff(MIDI_CHANNEL, note, MIDI_VELOCITY);
		soundingNotes.erase(note);
	}
}

void MidiOutManager::setMapping() {
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

void MidiOutManager::panicReset() {
	for (auto& [key, note]: keyToMidi) {
		manager.sendNoteOff(MIDI_CHANNEL, note, 0);
	}
	pressedKeys.clear();
	soundingNotes.clear();
}

void MidiOutManager::changeScene(int sceneNum) {
	switch (sceneNum) {
		case 0:
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 1, MIDI_VELOCITY);
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 13, MIDI_VELOCITY);
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 25, MIDI_VELOCITY);
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 37, MIDI_VELOCITY);
			pendingControlNote.push_back(1);
			pendingControlNote.push_back(13);
			pendingControlNote.push_back(25);
			break;
		case 1:
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 40, MIDI_VELOCITY);
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 52, MIDI_VELOCITY);
			pendingControlNote.push_back(40);
			pendingControlNote.push_back(52);
			break;
		case 2:
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 41, MIDI_VELOCITY);
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 53, MIDI_VELOCITY);
			pendingControlNote.push_back(41);
			pendingControlNote.push_back(53);
			break;
		case 3:
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 36, MIDI_VELOCITY);
			manager.sendNoteOn(MIDI_CONTROL_CHANNEL, 48, MIDI_VELOCITY);
			pendingControlNote.push_back(36);
			pendingControlNote.push_back(48);
			break;
			
		default:
			return;
	}
	pendingTime = ofGetElapsedTimeMillis();
}

void MidiOutManager::offControlNote() {
	for (auto& x: pendingControlNote) {
		manager.sendNoteOff(MIDI_CHANNEL, x, 0);
	}
}

/*---getter---*/

const unordered_set<int>& MidiOutManager::getPressedKeys () const {
	return pressedKeys;
}

const uint64_t& MidiOutManager::getPendingTime() const {
	return pendingTime;
}

int MidiOutManager::key2midi(int key) {
	return keyToMidi[key];
}
