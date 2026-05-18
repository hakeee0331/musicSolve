#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Clock.h"
#include "Ripple.h"

#include <vector>
#include <string>

struct FinalCircle {
	glm::vec2 position;
	glm::vec2 targetPosition;

	float radius;
	ofColor color;

	FinalCircle(glm::vec2 pos, float r, ofColor c)
	: position(pos)
	, targetPosition(pos)
	, radius(r)
	, color(c)
	{
	}
};

struct FinalStringLine {
	float y;

	float amplitude = 0.0f;
	float targetAmplitude = 0.0f;

	float phase = 0.0f;
	float frequency = 0.015f;

	float thickness = 1.3f;
	ofColor color;

	FinalStringLine(float y, ofColor color)
	: y(y)
	, color(color)
	{
		phase = ofRandom(TWO_PI);
		frequency = ofRandom(0.008f, 0.020f);
		thickness = ofRandom(0.8f, 2.0f);
	}
};

class FinalScene : public Scene {
private:
	const Clock* clock = nullptr;

	std::vector<FinalCircle> circles;
	std::vector<FinalStringLine> strings;
	std::vector<Ripple> ripples;

	int circleCount = 18;
	int stringCount = 9;

	float spacing = 10.0f;
	float moveDistance = 60.0f;
	float moveLerp = 0.05f;

	int lastBeatCount = -1;
	float pulse = 0.0f;
	float pulseTarget = 0.0f;
	float pulseAmount = 0.10f;

	void generateCircles();
	void generateStrings();

	void moveCircles();
	void resolveCircleCollisions();
	void keepCircleInsideScreen(FinalCircle& circle);

	bool canPlaceCircle(const glm::vec2& pos, float radius) const;

	void spawnRipple();
	void vibrateStrings();

	ofColor getCircleColor() const;
	ofColor getRippleColor() const;
	ofColor getStringColor() const;

public:
	FinalScene(const std::string& pattern)
	: Scene(pattern)
	{
	}

	void setClock(const Clock* clock) override;

	void setupGraphic() override;
	void updateGraphic() override;
	void drawGraphic() override;
	void keyPressedGraphic() override;
};
