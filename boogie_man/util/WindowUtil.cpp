#include "WindowUtil.h"
#include <iostream>


Vec2<float> getWsize()
{
	Vec2<float> temp(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()));
	return temp;
}

std::string GetRelativeTexturePath(std::string textureName)
{
	std::string WorkingDir(GetWorkingDirectory());

	for (size_t i = 0; i < WorkingDir.size(); i++)
	{
		if (WorkingDir.at(i) == '\\') {
			WorkingDir.at(i) = '/';
		}
	}

	std::cout << "WORKING DIR: " << (WorkingDir + "/textures/" + textureName).c_str() << std::endl;

	return (WorkingDir + "/textures/" + textureName);
}