//@BridgetACasey

#pragma once

//Clamps a value between a minimum and maximum bound
void clamp(float& value, float lowerBound, float upperBound)
{
	if (value > upperBound)
	{
		value = upperBound;
	}

	if (value < lowerBound)
	{
		value = lowerBound;
	}
}

//Clamps a value between a minimum and maximum bound
void clamp(double& value, double lowerBound, double upperBound)
{
	if (value > upperBound)
	{
		value = upperBound;
	}

	if (value < lowerBound)
	{
		value = lowerBound;
	}
}

//Increments or decrements a value by a given speed and relative to delta time
void adjustValue(float& value, bool isIncrementing, float speed, float deltaTime)
{
	if (isIncrementing)
	{
		value += speed * deltaTime;
	}

	else
	{
		value -= speed * deltaTime;
	}
}

//Increments or decrements a value by a given speed and relative to delta time
void adjustValue(double& value, bool isIncrementing, double speed, float deltaTime)
{
	if (isIncrementing)
	{
		value += speed * deltaTime;
	}

	else
	{
		value -= speed * deltaTime;
	}
}