#include "SceneManager.h"


void SceneManager::setup() {
	scenes.push_back(make_unique<IntroScene>("hope"));
	scenes.push_back(make_unique<Scene>("pain"));
	scenes.push_back(make_unique<Scene>("love"));
	scenes.push_back(make_unique<Scene>("life"));
	/*
	scenes.push_back(IntroScene("hope"));
	scenes.push_back(Scene("pain"));
	scenes.push_back(Scene("next"));
	scenes.push_back(Scene("life"));
	*/
	currentSceneIndex = 0;
}
void SceneManager::update() {
	scenes[currentSceneIndex]->update();
}
void SceneManager::draw() {
	scenes[currentSceneIndex]->draw();
	
	ofPushStyle();
	ofSetColor(255);
	
	string sceneNum;
	if (changeWait) {
		sceneNum = "Correct! Please wait for next scene...";
//		printf("%d \n", currentSceneIndex);
		if (currentSceneIndex == 3) sceneNum = "It's Done... Thank you for playing!";
	}
	else
		sceneNum = "Scene: " + to_string(currentSceneIndex + 1);
	
	ofBitmapFont bitmapFont;
	ofRectangle bounds = bitmapFont.getBoundingBox(sceneNum, 0, 0, OF_BITMAPMODE_SIMPLE, true);
	ofDrawBitmapString(sceneNum, ofGetWidth() / 2 - bounds.getWidth() / 2, 30);
	
	ofPopStyle();
}
bool SceneManager::keyPressed(int key) {
	if (changeWait) return false;
	
	if (scenes[currentSceneIndex]->keyPressed(key)) {
		changeWait = true;
		return true;
	};
	
	return false;
}
void SceneManager::nextScene() {
	if (scenes.empty() || !changeWait) return;
	
	currentSceneIndex++;
	if (currentSceneIndex >= scenes.size()) currentSceneIndex = 0;
	
	scenes[currentSceneIndex].reset();
	
	changeWait = false;
}

Scene& SceneManager::getCurrentScene() {
	return *scenes[currentSceneIndex];
}
int SceneManager::getCurrentSceneIndex() const {
	return currentSceneIndex;
}

bool SceneManager::isWaiting() const {
	return changeWait;
}
