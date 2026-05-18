#pragma once

#include "ofMain.h"

struct Ripple {
	glm::vec2 position;

	float startRadius;
	float endRadius;
	float duration;
	float startTime;

	ofColor color;

	Ripple(glm::vec2 pos, float radius, ofColor c)
	: position(pos)
	, startRadius(radius * 0.15f)
	, endRadius(radius)
	, duration(ofRandom(1.4f, 2.4f))
	, startTime(ofGetElapsedTimef())
	, color(c)
	{
	}

	float getProgress() const {
		float now = ofGetElapsedTimef();
		return ofClamp((now - startTime) / duration, 0.0f, 1.0f);
	}

	bool isDead() const {
		return getProgress() >= 1.0f;
	}

	float easeOutCubic(float t) const {
		return 1.0f - powf(1.0f - t, 3.0f);
	}

	void draw() const {
		float t = getProgress();
		float eased = easeOutCubic(t);

		float radius = ofLerp(startRadius, endRadius, eased);

		float alpha = 255.0f;

		if (t > 0.65f) {
			float fadeT = ofMap(t, 0.65f, 1.0f, 0.0f, 1.0f, true);
			alpha = ofLerp(255.0f, 0.0f, fadeT);
		}

		ofFill();

		ofSetColor(color, alpha);
		ofDrawCircle(position, radius);

//		ofNoFill();
//		ofSetLineWidth(3.0f);
//		ofSetColor(color, alpha);
//		ofDrawCircle(position, radius);

		ofFill();
	}
};
