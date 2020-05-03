// ***************************************************************************
// enemy
// ***************************************************************************

#include <vectrex.h>
#include "utils/sound.h"
#include "utils/utils.h"
#include "utils/vector.h"
#include "enemy.h"
#include "player.h"
#include "game.h"
#include "object.h"
#include "level.h"
#include "utils/math.h"
#include "utils/sound.h"

// ---------------------------------------------------------------------------

#undef SF
#define SF 1

#define DRAWING_SPEED 0x7f

const struct sound_explosion_t bang =
{
	0b00101010, SOUND_EXPL_RISE, SOUND_VOL_RISE, 1U
};

struct packet_t vectors_obstacles[] =
{
	{DRAW, { -4 * SF, 0 * SF}},
	{MOVE, { 0, 0}},
	{DRAW, {  0 * SF, 10 * SF}}, //width
	{DRAW, {  4 * SF,  0 * SF}},
	{MOVE, { 0, 0}},
	{STOP, { 0, 0}},
};

// ---------------------------------------------------------------------------

void draw_enemy(struct object_t* p)
{		
	long int temp = 0;
	Reset0Ref();				// reset beam to center of screen
	//TOP
	dp_VIA_t1_cnt_lo = 0x7f;	// set scaling factor for positioning
	Moveto_d(127, p->x);		// move beam to object coordinates
	dp_VIA_t1_cnt_lo = DRAWING_SPEED;	// set scalinf factor for drawing
	//calculate vectors
	temp = (127 - (long int)p->pos);
	if(temp > 127)
	{
		vectors_obstacles[1] = (struct packet_t){DRAW,{ -(int)(temp % 127) * SF, 0}};
		vectors_obstacles[4] = (struct packet_t){DRAW,{ (int)(temp % 127) * SF, 0}};
		temp = 127;
	}
	else 
	{
		vectors_obstacles[1] = (struct packet_t) {MOVE, { 0, 0}};
		vectors_obstacles[4] = (struct packet_t) {MOVE, { 0, 0}};
	}
	vectors_obstacles[0] = (struct packet_t){DRAW, { -(int)temp * SF, 0}}; //left top corner
	vectors_obstacles[3] = (struct packet_t){DRAW,{ (int)temp * SF, 0}};
	Draw_VLp(&vectors_obstacles);	// draw vector list
	
	//BOTTOM
	Reset0Ref();				// reset beam to center of screen
	dp_VIA_t1_cnt_lo = 0x7f;	// set scaling factor for positioning
	Moveto_d(-128, p->x);		// move beam to object coordinates
	dp_VIA_t1_cnt_lo = DRAWING_SPEED;	// set scalinf factor for drawing
	//calculate vectors
	temp = ((long int)p->pos + 57); //-70 (width of hole "pos" -> top edge) + 127
	if(temp > 127)
	{
		vectors_obstacles[1] = (struct packet_t){DRAW,{ (int)(temp % 127) * SF, 0}};
		vectors_obstacles[4] = (struct packet_t){DRAW,{ -(int)(temp % 127) * SF, 0}};
		temp = 127;
	}
	else 
	{
		vectors_obstacles[1] = (struct packet_t) {MOVE, { 0, 0}};
		vectors_obstacles[4] = (struct packet_t) {MOVE, { 0, 0}};
	}
	vectors_obstacles[0] = (struct packet_t){DRAW,{ (int)temp * SF, 0}}; //left top corner
	vectors_obstacles[3] = (struct packet_t){DRAW,{ -(int)temp * SF, 0}};
	Draw_VLp(&vectors_obstacles);	// draw vector list
}

// ---------------------------------------------------------------------------

void handle_enemies(void)
{
	static unsigned int iterator = OBSTACLES_COUNT-1;
	int i;
	
	//create new obstacle if time to do so
	if((current_level.frame % 65) == 0)
	{
		init_object(&obstacles[iterator]);
		if(iterator-- == 0) iterator = OBSTACLES_COUNT-1;
	}
	//handle all obstacles	
	for( i = 0; i < OBSTACLES_COUNT; i++)
	{ 
		if(obstacles[i].activ)
		{
			move_object(&obstacles[i]);
			draw_enemy(&obstacles[i]);
			if (check_collision(player.y, &obstacles[i]))
			{
			    play_explosion(&bang);
				player.status = DEAD;
				for( i = 0; i < OBSTACLES_COUNT; i++)
				{
					obstacles[i].activ = 0; 
				}
			}
		}
	}
}

// ***************************************************************************
// end of file
// ***************************************************************************

