#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Ripple.h"
#include <string>

class IntroScene : public Scene {
private:
	vector<Ripple> ripples;
	
	void spawnIntroRipple();
	ofColor getIntroColor() const;
	
public:
	IntroScene(const std::string& pattern) : Scene(pattern) {}
	
	void setupGraphic() override;
	void updateGraphic() override;
	void drawGraphic() override;
	void keyPressedGrahpic() override;
};
