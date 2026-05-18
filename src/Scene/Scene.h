#pragma once

#include "ofMain.h"
#include "PatternMatcher.h"
#include <string>
using namespace std;

class Scene {
private:
	PatternMatcher patternMatcher;
	
public:
	Scene(const std::string& pattern);
	
	void init();
	void update();
	void draw();
	
	bool keyPressed(int key);
	void reset();
	string getPattern() const;
	
};
