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

enum class OBJECT_TYPE
{
	DEFAULT,
	PLATFORM,
	PLAYER,
	FOREBLOCK,
	FOREGROUND,
	FIN
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

enum class COLLIDER_STATE
{
	ENTER,
	STAY,
	EXIT
};

enum class PLAYER_STATE
{
	APPEAR,
	IDLE,
	RUN,
	JUMP
};