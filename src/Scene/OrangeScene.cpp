#include "OrangeScene.h"
#include <algorithm>

void OrangeScene::setClock(const Clock* clock) {
	this->clock = clock;
}

void OrangeScene::setupGraphic() {
	generateCircles();
}

void OrangeScene::generateCircles() {
	circles.clear();

	int attempts = 0;
	int maxAttempts = 8000;

	while (circles.size() < circleCount && attempts < maxAttempts) {
		attempts++;

		float radius = ofRandom(28, 75);

		glm::vec2 pos(
			ofRandom(radius + spacing, ofGetWidth() - radius - spacing),
			ofRandom(radius + spacing, ofGetHeight() - radius - spacing)
		);

		if (canPlaceCircle(pos, radius)) {
			circles.emplace_back(pos, radius, getOrangeColor());
		}
	}
}

bool OrangeScene::canPlaceCircle(const glm::vec2& pos, float radius) const {
	for (const auto& other : circles) {
		float minDist = radius + other.radius + spacing;
		float dist = glm::distance(pos, other.position);

		if (dist < minDist) {
			return false;
		}
	}

	return true;
}

ofColor OrangeScene::getOrangeColor() const {
	ofColor c;

	float hue = ofRandom(18, 34);          // 주황 계열
	float saturation = ofRandom(180, 240);
	float brightness = ofRandom(210, 255);

	c.setHsb(hue, saturation, brightness);
	return c;
}

void OrangeScene::keyPressedGraphic() {
	moveAllCircles();
}

void OrangeScene::moveAllCircles() {
	for (auto& circle : circles) {
		float angle = ofRandom(TWO_PI);

		glm::vec2 dir(
			cos(angle),
			sin(angle)
		);

		glm::vec2 next = circle.targetPosition + dir * ofRandom(20, moveDistance);

		float margin = circle.radius + spacing;

		next.x = ofClamp(next.x, margin, ofGetWidth() - margin);
		next.y = ofClamp(next.y, margin, ofGetHeight() - margin);

		circle.targetPosition = next;
	}

	for (int i = 0; i < 8; i++) {
		resolveCollisions();
	}
}

void OrangeScene::updateGraphic() {
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

	for (auto& circle : circles) {
		circle.position = glm::mix(
			circle.position,
			circle.targetPosition,
			moveLerp
		);

		keepInsideScreen(circle);
	}

	for (int i = 0; i < 3; i++) {
			resolveCollisions();
	}
}

void OrangeScene::resolveCollisions() {
	for (int i = 0; i < circles.size(); i++) {
		for (int j = i + 1; j < circles.size(); j++) {
			auto& a = circles[i];
			auto& b = circles[j];

			glm::vec2 diff = b.position - a.position;
			float dist = glm::length(diff);

			float minDist = a.radius + b.radius + spacing;

			if (dist < 0.001f) {
				diff = glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1));
				dist = glm::length(diff);
			}

			if (dist < minDist) {
				glm::vec2 dir = diff / dist;
				float overlap = minDist - dist;

				a.position -= dir * overlap * 0.5f;
				b.position += dir * overlap * 0.5f;

//				a.targetPosition = a.position;
//				b.targetPosition = b.position;

				keepInsideScreen(a);
				keepInsideScreen(b);
			}
		}
	}
}

void OrangeScene::keepInsideScreen(CircleBlob& circle) {
	float margin = circle.radius + spacing;

	circle.position.x = ofClamp(circle.position.x, margin, ofGetWidth() - margin);
	circle.position.y = ofClamp(circle.position.y, margin, ofGetHeight() - margin);

	circle.targetPosition.x = ofClamp(circle.targetPosition.x, margin, ofGetWidth() - margin);
	circle.targetPosition.y = ofClamp(circle.targetPosition.y, margin, ofGetHeight() - margin);
}


void OrangeScene::drawGraphic() {
	ofBackground(20, 11, 4);

	float scale = 1.0f + pulse * pulseAmount;

	ofPushStyle();

	for (const auto& circle : circles) {
		float drawRadius = circle.radius * scale;

		ofSetColor(circle.color);
		ofDrawCircle(circle.position, drawRadius);
	}

	ofPopStyle();
}
