#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Clock.h"

#include <vector>
#include <string>

struct StringLine {
	float y;

	float amplitude = 0.0f;
	float targetAmplitude = 0.0f;

	float phase = 0.0f;
	float frequency = 0.015f;

	float thickness = 1.5f;
	ofColor color;

	StringLine(float y, ofColor color)
	: y(y)
	, color(color)
	{
		phase = ofRandom(TWO_PI);
		frequency = ofRandom(0.008f, 0.022f);
		thickness = ofRandom(1.0f, 2.4f);
	}
};

class StringScene : public Scene {
private:
	std::vector<StringLine> strings;

	const Clock* clock = nullptr;

	int stringCount = 13;

	int lastBeatCount = -1;
	float pulse = 0.0f;
	float pulseTarget = 0.0f;
	float pulseAmount = 0.45f;

	void generateStrings();
	void vibrateRandomStrings();

	ofColor getStringColor() const;

public:
	StringScene(const std::string& pattern)
	: Scene(pattern)
	{
	}

	void setClock(const Clock* clock) override;

	void setupGraphic() override;
	void updateGraphic() override;
	void drawGraphic() override;
	void keyPressedGraphic() override;
};
