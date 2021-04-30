//@BridgetACasey

#pragma once

void clamp(int& value, int lowerBound, int upperBound)
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

void adjustValue(int& value, bool increment, float deltaTime)
{
	if (increment)
	{
		value += 1;
	}

	else
	{
		value -= 1;
	}
}

void adjustValue(double& value, bool increment, float deltaTime)
{
	if (increment)
	{
		value += 0.1 * deltaTime;
	}

	else
	{
		value -= 0.1 * deltaTime;
	}
}