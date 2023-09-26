#pragma once
#include "GameObject.h"

#define ABILITIES

#ifdef ABILITIES

#define ABILITY_FLAG_MOVE 0x2
#define ABILITY_FLAG_JUMP 0x4
#define ABILITY_FLAG_SPRINT 0x8
#define ABILITY_FLAG_DOUBLE_JUMP 0x16

#endif

typedef long int CHARACTER_ABILITY_FLAG;


namespace BgCharacter
{
	const float gravity = 1000.0f;

	struct Abilities
	{
		bool move;
		bool jump;
		bool sprint;
		bool DoubleJump;

	};

	class MainCharacter : public Character
	{
	public:

		Abilities abilities;


		void EnableAbility(CHARACTER_ABILITY_FLAG Flag);
		void DisableAbility(CHARACTER_ABILITY_FLAG Flag);
		void Move();
		void Jump();

	};
}



