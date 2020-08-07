// ***************************************************************************
// player
// ***************************************************************************

#pragma once
#include <vectrex.h>

// ---------------------------------------------------------------------------

enum player_status_t
{
	DEAD,
	ALIVE,
};

typedef enum player_state_t
{
	INIT_FALL = 0,
	FALL,
	TIMEOUT,
	JUMP,
	BOOST
} player_state;
// ---------------------------------------------------------------------------

struct player_t
{
	enum player_status_t status;	// player status
    int y;
	int x;
	int timeout;
	int jump;
	enum player_state_t player_S;
};

// ---------------------------------------------------------------------------

extern struct player_t player;

// ---------------------------------------------------------------------------

void init_player(void);
void handle_player(void);

// ***************************************************************************
// end of file
// ***************************************************************************
