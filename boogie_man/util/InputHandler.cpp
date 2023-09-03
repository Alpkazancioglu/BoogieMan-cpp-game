#include "InputHandler.h"
#include <raylib.h>
#include <bitset>

static std::bitset<348> Keys = {0};

bool KeyWizard::isKeyPressed(int Key)
{
	if (IsKeyPressed(Key))
	{
		Keys[Key] = 1;
	}
	else if (IsKeyReleased(Key))
	{
		Keys[Key] = 0;
	}

	return (bool)Keys[Key];
}

bool KeyWizard::isKeyReleased(int Key)
{
	if (IsKeyPressed(Key))
	{
		Keys[Key] = 0;
	}
	else if (IsKeyReleased(Key))
	{
		Keys[Key] = 1;
	}

	return (bool)Keys[Key];
}
