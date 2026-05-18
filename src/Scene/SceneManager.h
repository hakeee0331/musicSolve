#pragma once

#include "Scene.h"
#include "IntroScene.h"
#include "OrangeScene.h"
#include "StringScene.h"
#include "Clock.h"
#include <vector>
#include <string>
using namespace std;

class SceneManager {
private:
	vector<std::unique_ptr<Scene>> scenes;
	int currentSceneIndex = 0;
	bool changeWait = false;
	
public:
	void setup(const Clock* clock);
	void update();
	void draw();
	bool keyPressed(int key);
	void nextScene();
	bool isWaiting() const;
	
	Scene& getCurrentScene();
	int getCurrentSceneIndex() const;
	
};
