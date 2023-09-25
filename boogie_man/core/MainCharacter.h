#pragma once
#include "GameObject.h"
<<<<<<< Updated upstream

=======
<<<<<<< HEAD
#define ABILITIES

#ifdef ABILITIES

#define ABILITY_FLAG_MOVE 0x2
#define ABILITY_FLAG_JUMP 0x4
#define ABILITY_FLAG_SPRINT 0x8
#define ABILITY_FLAG_DOUBLE_JUMP 0x16

#endif

typedef long int CHARACTER_ABILITY_FLAG;
=======

>>>>>>> a7c628740530180ca947fab099e99eb8d2f16131
>>>>>>> Stashed changes

namespace BgCharacter
{
	const float gravity = 1000.0f;

<<<<<<< Updated upstream

=======
<<<<<<< HEAD
=======

>>>>>>> a7c628740530180ca947fab099e99eb8d2f16131
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream

=======
<<<<<<< HEAD
		void EnableAbility(CHARACTER_ABILITY_FLAG Flag);
		void DisableAbility(CHARACTER_ABILITY_FLAG Flag);
=======

>>>>>>> a7c628740530180ca947fab099e99eb8d2f16131
>>>>>>> Stashed changes
		void Move();
		void Jump();




	};
}



