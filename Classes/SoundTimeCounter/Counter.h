#ifndef __COUNTER_H__
#define __COUNTER_H__

class Counter {
private:
public:
	static float SOUND_COUNTER;
	static void increase(float amount) { SOUND_COUNTER += amount; };
	static void decrease(float amount) { SOUND_COUNTER -= amount; };
};
 
float Counter::SOUND_COUNTER = 0;
#endif // __COUNTER_H__