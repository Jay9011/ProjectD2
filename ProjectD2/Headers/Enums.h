#pragma once

enum class GAME_STATE
{
	PLAY,
	PAUSE,
	EXIT
};

enum class OBJECT_STATE
{
	DEAD,
	ACTIVE,
	INACTIVE
};

enum class ANIM_PLAY_TYPE
{
	ONCE,
	LOOP,
	PINGPONG
};

enum class COLLIDER_TYPE
{
	LINE,
	AARECT,
	CIRCLE
};

enum class OBJECT_TYPE
{
	DEFAULT,

	FIN = 32
};

enum class PLAYER_STATE
{
	APPEAR,
	IDLE,
	RUN,
	JUMP
};