#include "PatternMatcher.h"


PatternMatcher::PatternMatcher(const string& target) : target(target) {
	resetAll();
}

bool PatternMatcher::isPrefix(const string& text) const {
	if (text.length() > target.length()) {
		return false;
	}
	
	for (int i = 0; i < text.length(); i++) {
		if (text[i] != target[i]) {
			return false;
		}
	}
	
	return true;
}

string PatternMatcher::makeDisplayString(const string& text) const {
//	string display = string(target.length(), '_');
	string display;
	for (int i = 0; i < target.length(); i++) {
		display += "_ ";
	}
	
	for (int i = 0; i < text.length() && i < display.length(); i++) {
		display[i*2] = text[i];
	}
	return display;
}

bool PatternMatcher::input(char key) {
	currentInput += key;
	
	if (!isPrefix(currentInput)) {
		resetInput();
		return false;
	}
	
	if (currentInput.length() > hint.length()) {
		hint = currentInput;
	}
	
	if (currentInput == target) {
		return true;
	}
	
	return false;
}

void PatternMatcher::resetInput() {
	currentInput = "";
}
void PatternMatcher::resetAll() {
	currentInput = "";
	hint = "";
}

const string& PatternMatcher::getTarget() const {
	return target;
}
const string& PatternMatcher::getCurrentInput() const {
	return currentInput;
}
const string& PatternMatcher::getHint() const {
	return hint;
}

string PatternMatcher::getInputDisplay() const {
	return makeDisplayString(currentInput);
}
string PatternMatcher::getHintDisplay() const {
	return makeDisplayString(hint);
}
