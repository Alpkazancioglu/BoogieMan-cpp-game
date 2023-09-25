#include "MainCharacter.h"

GameObject object;


void BgCharacter::MainCharacter::EnableAbility(CHARACTER_ABILITY_FLAG Flag)
{
	if (Flag == ABILITY_FLAG_MOVE)
	{
		this->abilities.move = true;
	}
	else if(Flag == ABILITY_FLAG_JUMP)
	{
		this->abilities.jump = true;
	}
	else if (Flag == ABILITY_FLAG_SPRINT)
	{
		this->abilities.sprint = true;
	}
	else if (Flag == ABILITY_FLAG_DOUBLE_JUMP)
	{
		this->abilities.DoubleJump = true;
	}
}

void BgCharacter::MainCharacter::DisableAbility(CHARACTER_ABILITY_FLAG Flag)
{
	if (Flag == ABILITY_FLAG_MOVE)
	{
		this->abilities.move = false;
	}
	else if (Flag == ABILITY_FLAG_JUMP)
	{
		this->abilities.jump = false;
	}
	else if (Flag == ABILITY_FLAG_SPRINT)
	{
		this->abilities.sprint = false;
	}
	else if (Flag == ABILITY_FLAG_DOUBLE_JUMP)
	{
		this->abilities.DoubleJump = false;
	}
}

void BgCharacter::MainCharacter::Move()
{
	float dt = GetFrameTime();
	static bool Slowing;
	static int TempMaxSpeed = this->Data.MaxSpeed;
	this->acceleration = 16.0f;
	
	if (this->abilities.move)
	{
		if (IsKeyDown(KEY_LEFT_SHIFT) && this->abilities.sprint)
		{
			TempMaxSpeed = this->Data.MaxSpeed * 2;
		}

		if ((IsKeyReleased(KEY_A)) || IsKeyReleased(KEY_D) || (IsKeyReleased(KEY_LEFT_SHIFT) && this->abilities.sprint))
		{
			Slowing = true;
			TempMaxSpeed = this->Data.MaxSpeed;
		}


		if (IsKeyDown(KEY_D))
		{

			/*if (object.Data.collision.IsColliding && object.Data.collision.direction == RIGHT)
			{
				this->Data.pos.x = object.Data.pos.x - this->Data.rec.width;
				this->Data.CurrentSpeed.x = 0;
				this->MovingStatus = IDLE;
			}

			else
			{
			*/	if (this->Data.CurrentSpeed <= this->Data.MaxSpeed)
			{
				Slowing = false;
			}

			if (!Slowing)
			{
				this->MovingStatus = MOVING_FRONT;
				if (this->Data.CurrentSpeed.x < 0)
					this->Data.CurrentSpeed.x += acceleration * 6;

				if (this->Data.CurrentSpeed.x <= TempMaxSpeed)
					this->Data.CurrentSpeed.x += acceleration;

				this->Data.pos.x += this->Data.CurrentSpeed.x * dt;
			}

		}


		else if (IsKeyDown(KEY_A))
		{

			/*if ((object.Data.collision.IsColliding && object.Data.collision.direction == RIGHT))
			{
				this->Data.pos.x = object.Data.pos.x + object.Data.rec.width;
				this->Data.CurrentSpeed.x = 0;
				this->MovingStatus = IDLE;
			}

			else
			{
			*/
			if (this->Data.CurrentSpeed.x >= -this->Data.MaxSpeed)
				Slowing = false;
			if (!Slowing)
			{
				this->MovingStatus = MOVING_BACK;
				if (this->Data.CurrentSpeed.x > 0)
					this->Data.CurrentSpeed.x -= acceleration * 6;

				if (this->Data.CurrentSpeed.x >= -TempMaxSpeed)
					this->Data.CurrentSpeed.x -= acceleration;

				this->Data.pos.x += this->Data.CurrentSpeed.x * dt;
			}

		}

		if (Slowing)
		{

			if (this->Data.CurrentSpeed.x > 0)
			{

				if (this->Data.CurrentSpeed.x >= this->Data.MaxSpeed)
					this->Data.CurrentSpeed.x -= acceleration * 2;

				else
					this->Data.CurrentSpeed.x -= acceleration * 10;

				this->Data.pos.x += this->Data.CurrentSpeed.x * dt;

				if (this->Data.CurrentSpeed.x <= 0)
				{
					this->MovingStatus = IDLE;
					this->Data.CurrentSpeed.x = 0;
					Slowing = false;
				}
			}

			else if (this->Data.CurrentSpeed.x < 0)
			{
				if (this->Data.CurrentSpeed.x <= -this->Data.MaxSpeed)
					this->Data.CurrentSpeed.x += acceleration * 2;

				else
					this->Data.CurrentSpeed.x += acceleration * 10;

				this->Data.pos.x += this->Data.CurrentSpeed.x * dt;

				if (this->Data.CurrentSpeed.x >= 0)
				{
					this->MovingStatus = IDLE;
					this->Data.CurrentSpeed.x = 0;
					Slowing = false;
				}
			}

		}

	}
}

	

void BgCharacter::MainCharacter::Jump()
{
	float dt = GetFrameTime();
			
	static float MaxJumpHeight = this->Data.pos.y - 100;
	this->CurrentHigh = this->Data.pos.y;
	this->Data.pos.y += this->Data.CurrentSpeed.y*dt;

	static bool IsMaxHeightReached = false;
	static bool IsPlayerJumped = false;
	static bool IsplayerJumpedDouble = false;
	
	
	object.Data.collision.IsOnTop = this->Data.pos.y + this->Data.rec.height > GROUND ? true : false;
	
	if(this->abilities.jump)
	{
		if(object.Data.collision.IsOnTop && IsKeyDown(KEY_SPACE))
		{	
			this->Data.CurrentSpeed.y = 0;
		}

		else if(object.Data.collision.IsOnTop)
		{
			MaxJumpHeight = this->Data.pos.y - 100;
			this->Data.CurrentSpeed.y = 0;
			IsMaxHeightReached = false;
			IsplayerJumpedDouble = false;
			IsPlayerJumped = false;
		}
			
		else 
		{
			this->Data.CurrentSpeed.y += gravity * dt;
		}
		
		if(MaxJumpHeight >= this->CurrentHigh)
		{
			IsMaxHeightReached = true;
		}

		if(IsKeyDown(KEY_SPACE) && !IsMaxHeightReached) 
		{
			this->Data.CurrentSpeed.y = -300;
		}

		else if(IsKeyReleased(KEY_SPACE))
		{
			IsMaxHeightReached = true;
			IsPlayerJumped = true;
		}

		else if (this->abilities.DoubleJump && !IsplayerJumpedDouble && IsKeyDown(KEY_SPACE) && IsPlayerJumped)
		{
			IsplayerJumpedDouble = true;
			IsMaxHeightReached = false;
			MaxJumpHeight = this->Data.pos.y - 100;
			IsPlayerJumped = false;	
		}
		

	}







}