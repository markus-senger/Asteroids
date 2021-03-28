// types.h:													MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include <ml5/ml5.h>

// general
constexpr int	window_init_width	= 800;
constexpr int	window_init_height	= 500;
constexpr int	window_buffer		= 100;
constexpr int	refresh_interval	= 100000;
constexpr int	hitarea_corners		= 8;
constexpr bool	show_hitarea		= false;
constexpr bool	game_over_is_aktiv	= true;
constexpr bool	show_score			= true;

// auxiliary functions
double	degree_to_rad		(double degree);
int		get_random_uniform	(int const l, int const u);

// controls
bool fly_foward_key			(int key);
bool rotation_right_key		(int key);
bool rotation_left_key		(int key);
bool shoot_key				(int key);

// spaceship
constexpr int ship_size						= 60;
constexpr int ship_hitbox					= 60;
constexpr int ship_forward_move_speed		= 20;
constexpr int ship_rotation_speed			= 17;
constexpr int ship_engine_on_time			= 3;

// enemy spaceship
constexpr int enemy_ship_size				= 60;
constexpr int enemy_ship_hitbox				= 60;
constexpr int enemy_ship_forward_move_speed	= 20;
constexpr int enemy_ship_rotation_speed		= 15;
constexpr int enemy_ship_spawn_cooldown		= 40;
constexpr int enemy_ship_action_cooldown	= 5;
constexpr int enemy_delay_offset			= 5;

// rocket
constexpr int rocket_size					= 20;
constexpr int rocket_hitbox					= 20;
constexpr int rocket_forward_move_speed		= 40;
constexpr int rocket_cooldown				= 5;
constexpr int rocket_delay_offset			= 10;

// asteroid
constexpr int asteroid_big_size				= 80;
constexpr int asteroid_big_hitbox			= 85;
constexpr int asteroid_small_size			= 40;
constexpr int asteroid_small_hitbox			= 45;
constexpr int asteroid_forward_move_speed	= 6;
constexpr int asteroid_cooldown				= 30;
constexpr int asteroid_delay_offset			= 30;

