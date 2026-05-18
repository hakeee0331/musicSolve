#pragma once

struct Clock {
	int clockCount = 0;
	int beatCount = 0;
	uint64_t lastBeatTime = 0;
	float bpm = 0.f;
};
