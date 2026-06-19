#include "SceneManager.h"


void SceneManager::setup(const Clock* clock) {
	scenes.push_back(make_unique<IntroScene>("hope"));
	
	unique_ptr<Scene> orange = std::make_unique<OrangeScene>("pain");
	orange->setClock(clock);
	scenes.push_back(std::move(orange));
	
	unique_ptr<Scene> stringScene = make_unique<StringScene>("love");
		stringScene->setClock(clock);
		scenes.push_back(std::move(stringScene));
	
	unique_ptr<Scene> finalScene = make_unique<FinalScene>("life");
		finalScene->setClock(clock);
		scenes.push_back(std::move(finalScene));

	currentSceneIndex = 0;
	
	for (auto& x: scenes) {
		x->init();
	}
}
void SceneManager::update() {
	scenes[currentSceneIndex]->update();
}
void SceneManager::draw() {
	scenes[currentSceneIndex]->draw();
	
	ofPushStyle();
	ofPushMatrix();
	ofSetColor(255);
	
	string sceneNum;
	if (changeWait) {
		sceneNum = "Correct! Please wait for next scene...";
//		printf("%d \n", currentSceneIndex);
		if (currentSceneIndex == 3) sceneNum = "It's Done... Thank you for playing!";
	}
	else {
//		switch (currentSceneIndex) {
//			case 0:
//				sceneNum = "Scene: hope";
//				break;
//			case 1:
//				sceneNum = "Scene: 고통";
//				break;
//			case 2:
//				sceneNum = "Scene: 사랑";
//				break;
//			case 3:
//				sceneNum = "Scene: 인생";
//				break;
//			default:
//				break;
//		}
		 sceneNum = "Scene: " + to_string(currentSceneIndex + 1);
	}
	ofBitmapFont bitmapFont;
	ofRectangle bounds = bitmapFont.getBoundingBox(sceneNum, 0, 0, OF_BITMAPMODE_SIMPLE, true);
	float scale = 1.6f;
	float scaledWidth = bounds.getWidth() * scale;
	float scaledHeight = bounds.getHeight() * scale;
	float x = ofGetWidth() / 2.0f - scaledWidth / 2.0f;
	
	ofTranslate(x, 0);
	ofScale(scale, scale);
	
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	
	ofDrawBitmapString(sceneNum, 0, 60);
	ofPopMatrix();
	ofPopStyle();
}
bool SceneManager::keyPressed(int key) {
	
	if (changeWait){
		scenes[currentSceneIndex]->keyPressedGraphic();	// 그래픽만
		return false;
	}
	
	if (scenes[currentSceneIndex]->keyPressed(key)) {	// 	그래픽과 패턴 처리 모두
		changeWait = true;
		return true;
	};
	
	return false;
}
void SceneManager::nextScene() {
	if (scenes.empty() || !changeWait) return;
	
	currentSceneIndex++;
	if (currentSceneIndex >= scenes.size()) currentSceneIndex = 0;
	
	scenes[currentSceneIndex]->reset();
	scenes[currentSceneIndex]->init();
	
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
