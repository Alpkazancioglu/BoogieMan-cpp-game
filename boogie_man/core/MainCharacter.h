#pragma once
#include "GameObject.h"


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


		void Move();
		void Jump();




	};
}



