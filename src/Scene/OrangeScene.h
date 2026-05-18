#pragma once

#include "ofMain.h"
#include "Scene.h"
#include "Clock.h"
#include <vector>

struct CircleBlob {
	glm::vec2 position;
	glm::vec2 targetPosition;

	float radius;
	ofColor color;

	CircleBlob(glm::vec2 pos, float r, ofColor c)
	: position(pos)
	, targetPosition(pos)
	, radius(r)
	, color(c)
	{
	}
};


class OrangeScene : public Scene {
private:
	std::vector<CircleBlob> circles;

	const Clock* clock = nullptr;

	int circleCount = 40;
	float spacing = 14.0f;
	float moveDistance = 55.0f;
	float moveLerp = 0.065f;

	int lastBeatCount = -1;

	float pulse = 0.0f;
	float pulseTarget = 0.0f;
	float pulseAmount = 0.12f;
	
	void generateCircles();
	bool canPlaceCircle(const glm::vec2& pos, float radius) const;

	void moveAllCircles();
	void resolveCollisions();
	void keepInsideScreen(CircleBlob& circle);

	ofColor getOrangeColor() const;

public:
	OrangeScene(const std::string& pattern)
	: Scene(pattern)
	{
	}

	void setClock(const Clock* clock) override;

	void setupGraphic() override;
	void updateGraphic() override;
	void drawGraphic() override;
	void keyPressedGraphic() override;
};
