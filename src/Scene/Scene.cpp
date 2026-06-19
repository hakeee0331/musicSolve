#include "Scene.h"

Scene::Scene(const std::string& pattern) : patternMatcher(pattern) {}

void Scene::init(){
	setupGraphic();
}
void Scene::update(){
	updateGraphic();
}
void Scene::draw(){
	drawGraphic();
	
	string inputDisplay = patternMatcher.getInputDisplay();
	string hintDisplay = " hint:\n\n" + patternMatcher.getHintDisplay();
	
//	cout << patternMatcher.getTarget() << inputDisplay << " " << hintDisplay << '\n';
	
	ofBitmapFont bitmapFont;
	
	
	ofPushStyle();
	ofSetColor(255);
	
	ofPushMatrix(); {
		ofRectangle bounds = bitmapFont.getBoundingBox(inputDisplay, 0, 0, OF_BITMAPMODE_SIMPLE, true);
		float scale = 5.f;
		float scaledWidth = bounds.getWidth() * scale;
		float scaledHeight = bounds.getHeight() * scale;
		float x = ofGetWidth() / 2.0f - scaledWidth / 2.0f;
		float y = ofGetHeight() / 2.0f - scaledHeight / 2.0f;
		
		ofTranslate(x, y);
		ofScale(scale, scale);
		
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
		ofDrawBitmapString(inputDisplay, 0, bounds.getHeight() / 2.0f);
		ofSetDrawBitmapMode(OF_BITMAPMODE_SCREEN);
	} ofPopMatrix();	// draw inputDisplay
	
	ofPushMatrix(); {
		ofRectangle bounds = bitmapFont.getBoundingBox(hintDisplay, 0, 0, OF_BITMAPMODE_SIMPLE, true);
		float scale = 1.3f;
		float scaledWidth = bounds.getWidth() * scale;
		float scaledHeight = bounds.getHeight() * scale;
		float x = ofGetWidth() / 2.0f - scaledWidth / 2.0f;
		float y = ofGetHeight() / 2.0f - scaledHeight / 2.0f;
		
		ofTranslate(x, y);
		ofScale(scale, scale);
		
		ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
		ofDrawBitmapString(hintDisplay, 0, -90);
		ofSetDrawBitmapMode(OF_BITMAPMODE_SCREEN);
	} ofPopMatrix();	// draw hintDisplay
	
	ofPopStyle();
}

bool Scene::keyPressed(int key){
	keyPressedGraphic();
	
	if (patternMatcher.input(static_cast<char>(key))) return true;
	else return false;
}
void Scene::reset(){
	patternMatcher.resetAll();
}
string Scene::getPattern() const{
	return patternMatcher.getTarget();
}
