#pragma once

class InputState;

template <typename T>

class Util {
public:
	Util();
	virtual ~Util();

	void SelectValueUpdate(const InputState& input,T up,T down, T& SelectValue, const T minValue,const T maxValue,const T addValue);

};


