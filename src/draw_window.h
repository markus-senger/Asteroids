// draw_window.h:											MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include <ml5/ml5.h>
#include "types.h"
#include "spaceship.h"
#include "enemy_ship.h"
#include "rocket.h"
#include "asteroid.h"

class draw_window final : public ml5::window, MI5_DERIVE(draw_window, ml5::window) {
	MI5_INJECT(draw_window)

public:

	draw_window();

	void on_init	()								override;
	void on_exit	()								override;
	void on_timer	(ml5::timer_event const& event) override;
	void on_key		(ml5::key_event const& event)	override;
	void on_paint	(ml5::paint_event const& event) override;
	void game_over	();


private:

	spaceship									m_ship{ window_init_width / 2 - ship_size / 2, window_init_height / 2 - ship_size / 2, 0.0 };
	ml5::vector<std::unique_ptr<rocket>>		m_rockets;
	ml5::vector<std::unique_ptr<asteroid>>		m_asteroids;
	ml5::vector<std::unique_ptr<enemy_ship>>	m_enemies;
	size_t		m_hits			{ 0 };
	size_t		m_points		{ 0 };
	std::string	m_highscore		{ "0" };
	bool		m_is_game_over	{ false };
	size_t		m_level			{ 0 };

	void get_highscore_from_file					();
	void save_highscore_in_file						();
	void calc_spawnpoint							(int* x, int* y, int* direction);
	bool check_out_of_border						(int x, int y);
	bool check_out_of_border_with_buffer			(int x, int y);
	void check_collision							();
	bool check_collision_asteroids_asteroids		(std::unique_ptr<asteroid>& asteroid_obj);
	bool check_collision_asteroids_rockets			(std::unique_ptr<asteroid>& asteroid_obj);
	bool check_collision_asteroids_enemy_ships		(std::unique_ptr<asteroid>& asteroid_obj);
	bool check_collision_rockets_enemy_ships		(std::unique_ptr<rocket>& rocket_obj);
	bool check_collision_enemy_ships_other_ships	(std::unique_ptr<enemy_ship>& enemy_obj);
	void create_small_asteroids						(int x, int y);
	void handle_enemy_ship							();
}; 