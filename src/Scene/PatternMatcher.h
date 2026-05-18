#pragma once

#include <string>
using namespace std;

class PatternMatcher {
private:
	string target;
	string currentInput;
	string hint;
	
	bool isPrefix(const string& text) const;
	string makeDisplayString(const string& text) const;
	
public:
	PatternMatcher(const string& target);
	
	bool input(char key);
	
	void resetInput();
	void resetAll();
	
	const string& getTarget() const;
	const string& getCurrentInput() const;
	const string& getHint() const;
	
	string getInputDisplay() const;
	string getHintDisplay() const;
	
};
