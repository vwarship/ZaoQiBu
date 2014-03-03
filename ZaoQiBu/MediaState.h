#pragma once

typedef enum MediaState
{
	NothingSpecial = 0,
	Opening,
	Buffering,
	Playing,
	Paused,
	Stopped,
	Ended,
	Error
} MediaState;
