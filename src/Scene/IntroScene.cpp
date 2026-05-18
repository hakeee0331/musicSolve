#include "IntroScene.h"

ofColor IntroScene::getIntroColor() const {
	float hue = ofRandom(20, 245);
	float saturation = ofRandom(200, 255);   // 너무 강하지 않게
	float brightness = ofRandom(210, 255); // 밝게

	ofColor c;
	c.setHsb(hue, saturation, brightness);
	return c;
}

void IntroScene::spawnIntroRipple() {
	glm::vec2 basePos(
		ofRandomWidth(),
		ofRandomHeight()
	);

	ofColor baseColor = getIntroColor();

	int count = ofRandom(1, 4);

	for (int i = 0; i < count; i++) {
		glm::vec2 offset(
			ofRandom(-25, 25),
			ofRandom(-25, 25)
		);

		float radius = ofRandom(70, 220);

		ofColor c = baseColor;
		c.setHueAngle(fmod(c.getHueAngle() + ofRandom(-18, 18), 360));

		ripples.emplace_back(basePos + offset, radius, c);
	}
}

void IntroScene::setupGraphic() {
	
}

void IntroScene::updateGraphic() {
	ripples.erase(
			remove_if(
				ripples.begin(),
				ripples.end(),
				[](const Ripple& r) {
					return r.isDead();
				}
			),
			ripples.end()
		);
}

void IntroScene::drawGraphic() {
	ofSetColor(8, 10, 18, 35);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	for (const auto& ripple : ripples) {
		ripple.draw();
	}
}

void IntroScene::keyPressedGrahpic() {
	spawnIntroRipple();
}

