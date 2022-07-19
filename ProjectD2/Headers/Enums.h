#pragma once

enum class GAME_STATE
{
	PLAY,
	PAUSE,
	EXIT
};

enum class OBJECT_TYPE
{
	DEFAULT,
	
	FIN = 32
};

enum class OBJECT_STATE
{
	DEAD,
	ACTIVE
};

enum class ANIM_PLAY_TYPE
{
	Once,
	Loop,
	PingPong
};

enum class PLAYER_STATE
{
	APPEAR,
	IDLE,
	RUN,
	JUMP
};

/*
enum class SCENE_TYPE
{
	TEST,
	TOOL,
	END
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,
	END
};

enum class COMMAND_TYPE
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	
	FIN
};
*/