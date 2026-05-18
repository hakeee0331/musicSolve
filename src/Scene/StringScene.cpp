#include "StringScene.h"

void StringScene::setClock(const Clock* clock) {
	this->clock = clock;
}

void StringScene::setupGraphic() {
	generateStrings();
}

void StringScene::generateStrings() {
	strings.clear();

	float topMargin = ofGetHeight() * 0.18f;
	float bottomMargin = ofGetHeight() * 0.82f;

	for (int i = 0; i < stringCount; i++) {
		float t = 0.0f;

		if (stringCount > 1) {
			t = static_cast<float>(i) / static_cast<float>(stringCount - 1);
		}

		float y = ofLerp(topMargin, bottomMargin, t);

		strings.emplace_back(y, getStringColor());
	}
}

ofColor StringScene::getStringColor() const {
	int palette = static_cast<int>(ofRandom(3));

	if (palette == 0) {
		return ofColor(220, 210, 255);
	}

	if (palette == 1) {
		return ofColor(255, 230, 180);
	}

	return ofColor(180, 210, 255);
}

void StringScene::updateGraphic() {
	if (clock != nullptr) {
		if (lastBeatCount == -1) {
			lastBeatCount = clock->beatCount;
		}

		if (clock->beatCount != lastBeatCount) {
			pulseTarget = 1.0f;
			lastBeatCount = clock->beatCount;
		}
	}


	pulse += (pulseTarget - pulse) * 0.35f;
	pulseTarget *= 0.75f;

	if (pulse < 0.001f) {
		pulse = 0.0f;
	}


	for (auto& line : strings) {
		line.amplitude += (line.targetAmplitude - line.amplitude) * 0.18f;
		line.targetAmplitude *= 0.90f;

		if (line.amplitude < 0.001f) {
			line.amplitude = 0.0f;
		}

		line.phase += 0.09f + line.amplitude * 0.002f;
	}
}

void StringScene::drawGraphic() {
	ofBackground(9, 7, 18);

	ofPushStyle();

	float brightnessPulse = 1.0f + pulse * pulseAmount;
	float thicknessPulse = 1.0f + pulse * 0.8f;

	for (const auto& line : strings) {
		ofColor c = line.color;
		c.r = ofClamp(c.r * brightnessPulse, 0, 255);
		c.g = ofClamp(c.g * brightnessPulse, 0, 255);
		c.b = ofClamp(c.b * brightnessPulse, 0, 255);

		ofSetColor(c, 210);
		ofSetLineWidth(line.thickness * thicknessPulse);

		ofNoFill();
		ofBeginShape();

		for (float x = 0; x <= ofGetWidth(); x += 10.0f) {
			float wave = sin(x * line.frequency + line.phase) * line.amplitude;

			float subtleWave = sin(x * 0.006f + line.phase * 0.4f) * 1.2f;

			ofVertex(x, line.y + wave + subtleWave);
		}

		ofEndShape(false);
	}

	ofPopStyle();
}

void StringScene::keyPressedGraphic() {
	vibrateRandomStrings();
}

void StringScene::vibrateRandomStrings() {
	if (strings.empty()) return;

	int hitCount = static_cast<int>(ofRandom(2, 5));

	for (int i = 0; i < hitCount; i++) {
		int index = static_cast<int>(ofRandom(strings.size()));

		strings[index].targetAmplitude = ofRandom(12.0f, 32.0f);

		strings[index].frequency = ofRandom(0.010f, 0.026f);
	}
}
