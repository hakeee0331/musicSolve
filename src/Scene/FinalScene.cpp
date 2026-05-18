#include "FinalScene.h"
#include <algorithm>

void FinalScene::setClock(const Clock* clock) {
	this->clock = clock;
}

void FinalScene::setupGraphic() {
	generateCircles();
	generateStrings();

	ripples.clear();

	lastBeatCount = -1;
	pulse = 0.0f;
	pulseTarget = 0.0f;
}

void FinalScene::generateCircles() {
	circles.clear();

	int attempts = 0;
	int maxAttempts = 6000;

	while (circles.size() < circleCount && attempts < maxAttempts) {
		attempts++;

		float radius = ofRandom(24, 58);

		glm::vec2 pos(
			ofRandom(radius + spacing, ofGetWidth() - radius - spacing),
			ofRandom(radius + spacing, ofGetHeight() - radius - spacing)
		);

		if (canPlaceCircle(pos, radius)) {
			circles.emplace_back(pos, radius, getCircleColor());
		}
	}
}

bool FinalScene::canPlaceCircle(const glm::vec2& pos, float radius) const {
	for (const auto& other : circles) {
		float minDist = radius + other.radius + spacing;
		float dist = glm::distance(pos, other.position);

		if (dist < minDist) {
			return false;
		}
	}

	return true;
}

ofColor FinalScene::getCircleColor() const {
	ofColor c;

	float hue = ofRandom(18, 34);
	float saturation = ofRandom(170, 230);
	float brightness = ofRandom(190, 250);

	c.setHsb(hue, saturation, brightness);
	return c;
}

void FinalScene::generateStrings() {
	strings.clear();

	float topMargin = ofGetHeight() * 0.20f;
	float bottomMargin = ofGetHeight() * 0.80f;

	for (int i = 0; i < stringCount; i++) {
		float t = 0.0f;

		if (stringCount > 1) {
			t = static_cast<float>(i) / static_cast<float>(stringCount - 1);
		}

		float y = ofLerp(topMargin, bottomMargin, t);

		strings.emplace_back(y, getStringColor());
	}
}

ofColor FinalScene::getStringColor() const {
	int palette = static_cast<int>(ofRandom(3));

	if (palette == 0) {
		return ofColor(230, 220, 255);
	}

	if (palette == 1) {
		return ofColor(255, 225, 180);
	}

	return ofColor(190, 220, 255);
}

void FinalScene::keyPressedGraphic() {
	spawnRipple();
	moveCircles();
	vibrateStrings();
}

void FinalScene::spawnRipple() {
	glm::vec2 basePos(
		ofRandomWidth(),
		ofRandomHeight()
	);

	ofColor baseColor = getRippleColor();

	int count = static_cast<int>(ofRandom(1, 3));

	for (int i = 0; i < count; i++) {
		glm::vec2 offset(
			ofRandom(-20, 20),
			ofRandom(-20, 20)
		);

		float radius = ofRandom(80, 220);

		ofColor c = baseColor;
		c.setHue(ofClamp(c.getHue() + ofRandom(-8, 8), 225, 245));

		ripples.emplace_back(basePos + offset, radius, c);
	}
}

ofColor FinalScene::getRippleColor() const {
	ofColor c;

	float hue = ofRandom(225, 245);
	float saturation = ofRandom(200, 255);
	float brightness = ofRandom(215, 255);

	c.setHsb(hue, saturation, brightness);
	return c;
}

void FinalScene::moveCircles() {
	for (auto& circle : circles) {
		float angle = ofRandom(TWO_PI);

		glm::vec2 dir(
			cos(angle),
			sin(angle)
		);

		glm::vec2 next = circle.targetPosition + dir * ofRandom(15, moveDistance);

		float margin = circle.radius + spacing;

		next.x = ofClamp(next.x, margin, ofGetWidth() - margin);
		next.y = ofClamp(next.y, margin, ofGetHeight() - margin);

		circle.targetPosition = next;
	}
}

void FinalScene::vibrateStrings() {
	if (strings.empty()) return;

	int hitCount = static_cast<int>(ofRandom(2, 5));

	for (int i = 0; i < hitCount; i++) {
		int index = static_cast<int>(ofRandom(strings.size()));

		strings[index].targetAmplitude = ofRandom(8.0f, 24.0f);
		strings[index].frequency = ofRandom(0.010f, 0.024f);
	}
}

void FinalScene::updateGraphic() {
	// BPM pulse
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

	// ripple 제거
	ripples.erase(
		std::remove_if(
			ripples.begin(),
			ripples.end(),
			[](const Ripple& r) {
				return r.isDead();
			}
		),
		ripples.end()
	);

	// 원 이동
	for (auto& circle : circles) {
		circle.position = glm::mix(
			circle.position,
			circle.targetPosition,
			moveLerp
		);

		keepCircleInsideScreen(circle);
	}

	for (int i = 0; i < 3; i++) {
		resolveCircleCollisions();
	}

	// 현 업데이트
	for (auto& line : strings) {
		line.amplitude += (line.targetAmplitude - line.amplitude) * 0.18f;
		line.targetAmplitude *= 0.90f;

		if (line.amplitude < 0.001f) {
			line.amplitude = 0.0f;
		}

		line.phase += 0.08f + line.amplitude * 0.002f;
	}
}

void FinalScene::resolveCircleCollisions() {
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

				if (dist < 0.001f) {
					diff = glm::vec2(1, 0);
					dist = 1.0f;
				}
			}

			if (dist < minDist) {
				glm::vec2 dir = diff / dist;
				float overlap = minDist - dist;

				a.position -= dir * overlap * 0.5f;
				b.position += dir * overlap * 0.5f;

				keepCircleInsideScreen(a);
				keepCircleInsideScreen(b);
			}
		}
	}
}

void FinalScene::keepCircleInsideScreen(FinalCircle& circle) {
	float margin = circle.radius + spacing;

	circle.position.x = ofClamp(circle.position.x, margin, ofGetWidth() - margin);
	circle.position.y = ofClamp(circle.position.y, margin, ofGetHeight() - margin);

	circle.targetPosition.x = ofClamp(circle.targetPosition.x, margin, ofGetWidth() - margin);
	circle.targetPosition.y = ofClamp(circle.targetPosition.y, margin, ofGetHeight() - margin);
}

void FinalScene::drawGraphic() {
	ofBackground(10, 6, 12);

	float circleScale = 1.0f + pulse * pulseAmount;
	float stringBrightnessPulse = 1.0f + pulse * 0.35f;
	float stringThicknessPulse = 1.0f + pulse * 0.6f;

	ofPushStyle();

	// 1. 주황 원
	for (const auto& circle : circles) {
		float drawRadius = circle.radius * circleScale;

		ofColor c = circle.color;
		ofSetColor(c, 210);
		ofDrawCircle(circle.position, drawRadius);
	}

	// 2. 분홍 ripple
	for (const auto& ripple : ripples) {
		ripple.draw();
	}

	// 3. 현
	for (const auto& line : strings) {
		ofColor c = line.color;

		c.r = ofClamp(c.r * stringBrightnessPulse, 0, 255);
		c.g = ofClamp(c.g * stringBrightnessPulse, 0, 255);
		c.b = ofClamp(c.b * stringBrightnessPulse, 0, 255);

		ofSetColor(c, 220);
		ofSetLineWidth(line.thickness * stringThicknessPulse);

		ofNoFill();
		ofBeginShape();

		for (float x = 0; x <= ofGetWidth(); x += 10.0f) {
			float wave = sin(x * line.frequency + line.phase) * line.amplitude;
			float subtleWave = sin(x * 0.006f + line.phase * 0.4f) * 1.0f;

			ofVertex(x, line.y + wave + subtleWave);
		}

		ofEndShape(false);
	}

	ofPopStyle();
}
