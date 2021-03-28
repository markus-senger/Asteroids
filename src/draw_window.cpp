// draw_window.cpp:                                         MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#include "draw_window.h"
#include <fstream>

// -------------------- public --------------------

draw_window::draw_window() {
	set_prop_initial_size({ window_init_width, window_init_height });
	set_prop_background_brush(*wxBLACK_BRUSH);
	
}

void draw_window::on_init() {
	start_timer(ml5::duration_t(refresh_interval));
	get_highscore_from_file();
	srand(time(NULL));
}

void draw_window::on_exit() {
	wxImage::CleanUpHandlers();
}

void draw_window::on_timer(ml5::timer_event const& event) {
	static int rocket_cooldown_counter = 0;
	static int asteroid_cooldown_counter = m_level;
	static int enemy_ship_spawn_cooldown_counter = m_level;
	static int enemy_ship_action_cooldown_counter = 0;
	rocket_cooldown_counter++;
	asteroid_cooldown_counter++;
	if (m_enemies.size() < 2)	enemy_ship_spawn_cooldown_counter++;
	if (!m_enemies.empty())		enemy_ship_action_cooldown_counter++;

	if (rocket_cooldown_counter == rocket_cooldown) {
		m_ship.set_shoot_ready(true);
		for (auto& enemy_obj : m_enemies) {
			enemy_obj->set_shoot_ready(true);
		}
		rocket_cooldown_counter = 0;
	}

	if (asteroid_cooldown_counter == asteroid_cooldown) {
		int direction = 0;
		int x = 0;
		int y = 0;

		calc_spawnpoint(&x, &y, &direction);
		
		m_asteroids.add(std::make_unique<asteroid>(x, y, direction, 0));
		asteroid_cooldown_counter = m_level;
	}

	if (enemy_ship_spawn_cooldown_counter == enemy_ship_spawn_cooldown && m_enemies.size() <= 1) {
		int direction = 0;
		int x = 0;
		int y = 0;

		calc_spawnpoint(&x, &y, &direction);

		m_enemies.add(std::make_unique<enemy_ship>(x, y, direction));
		enemy_ship_spawn_cooldown_counter = m_level;
	}

	if (enemy_ship_action_cooldown_counter == enemy_ship_action_cooldown) {
		handle_enemy_ship();
		enemy_ship_action_cooldown_counter = 0;
	}

	ml5::duration_t time(refresh_interval);
	restart_timer(time);
	refresh();
}

void draw_window::on_key(ml5::key_event const& event) {
	int key = event.get_key_code();

	if (fly_foward_key(key))		m_ship.fly_forward();
	else if (rotation_right_key(key))	m_ship.rotate_right();
	else if (rotation_left_key(key))	m_ship.rotate_left();
	else if (shoot_key(key)) {
		if (m_ship.get_shoot_ready()) {
			int tmp_x = round(m_ship.get_pos_x() + (ship_size / 2 - rocket_size / 2) + 
						abs(sin(degree_to_rad(m_ship.get_degree()))) * abs(cos(degree_to_rad(m_ship.get_degree()))) * 10);
			int tmp_y = round(m_ship.get_pos_y() + (ship_size / 2 - rocket_size / 2) + 
						abs(sin(degree_to_rad(m_ship.get_degree()))) * abs(cos(degree_to_rad(m_ship.get_degree()))) * 10);
			m_rockets.add(std::make_unique<rocket>(tmp_x, tmp_y, m_ship.get_degree(), 0));
			m_ship.set_shoot_ready(false);
		}
	}
}

void draw_window::on_paint(ml5::paint_event const& event) {
	check_collision();

	m_ship.draw(event.get_context());
	for (auto& asteroids_obj : m_asteroids) {
		if (asteroids_obj != nullptr) {
			asteroids_obj->draw(event.get_context());
			asteroids_obj->fly_forward();
		}
	}
	Sleep(asteroid_delay_offset - m_asteroids.size());
	for (auto& rocket_obj : m_rockets) {
		if (rocket_obj != nullptr) {
			rocket_obj->draw(event.get_context());
			rocket_obj->fly_forward();
			if (rocket_obj->get_time() >= 0) rocket_obj->set_time(rocket_obj->get_time() - 1);
		}
	}
	Sleep(rocket_delay_offset - m_rockets.size());
	for (auto& enemy_obj : m_enemies) {
		if (enemy_obj != nullptr) {
			enemy_obj->draw(event.get_context());
		}
	}
	Sleep(enemy_delay_offset - m_enemies.size());

	set_status_text("Hits: " + std::to_string(m_hits) + " | Points: " + std::to_string(m_points) + " | Highscore: " + m_highscore + " (points)");

	if (show_score) {
		event.get_context().SetTextForeground(*wxWHITE);
		wxFont font;
		font.SetPixelSize(wxSize{ 13, 13 });
		event.get_context().SetFont(font);
		if (m_is_game_over) {
			event.get_context().DrawText("Game Over!\nHits: " + std::to_string(m_hits) + " | Points: " + std::to_string(m_points), wxPoint{ 0, 0 });
			if (stoi(m_highscore) < m_points) {
				save_highscore_in_file();
				set_status_text("Hits: " + std::to_string(m_hits) + " | Points: " + std::to_string(m_points) + " | Highscore: **new Highscore**");
			}
		}
		else {
			event.get_context().DrawText("Hits: " + std::to_string(m_hits) + " | Points: " + std::to_string(m_points), wxPoint{ 0, 0 });
		}
	}
}

void draw_window::game_over() {
	m_ship.set_destroy(true);
	m_is_game_over = true;

	m_rockets.clear();
	m_asteroids.clear();
	m_enemies.clear();

	refresh();
	stop_timer();
}


// -------------------- private --------------------

void draw_window::get_highscore_from_file() {
	std::ifstream file("..\\asteroids\\src\\highscore.txt");
	if (file) {
		std::getline(file, m_highscore);
		file.close();
	}
}

void draw_window::save_highscore_in_file() {
	std::ofstream file("..\\asteroids\\src\\highscore.txt");
	if (file) {
		file << m_points;
		file.close();
	}
}

void draw_window::calc_spawnpoint(int* x, int* y, int* direction) {
	int spawnpoint = rand() % 4;

	if (spawnpoint == 0) {
		*x = rand() % get_width();
		*y = get_height();
		*direction = 50 + rand() % (310 - 50 + 1) + 180;
	}
	else if (spawnpoint == 1) {
		*x = rand() % get_width();
		*y = -70;
		*direction = 110 + rand() % (250 - 110 + 1);
	}
	else if (spawnpoint == 2) {
		*x = get_width();
		*y = rand() % get_height();
		*direction = 20 + rand() % (160 - 20 + 1);
	}
	else if (spawnpoint == 3) {
		*x = -70;
		*y = rand() % get_height();
		*direction = 200 + (rand() % (340 - 200 + 1));
	}
}

bool draw_window::check_out_of_border_with_buffer(int x, int y) {
	return	((x > get_width() + window_buffer) || (x < 0 - window_buffer) ||
			 (y > get_height() + window_buffer) || (y < 0 - window_buffer));
}

bool draw_window::check_out_of_border(int x, int y) {
	return	((x > get_width()) || (x < 0) || (y > get_height()) || (y < 0));
}

void draw_window::check_collision() {
	if (check_out_of_border(m_ship.get_pos_x(), m_ship.get_pos_y()) && game_over_is_aktiv) {
		game_over();
	}

	for (auto& asteroid_obj : m_asteroids) {
		if (asteroid_obj != nullptr) {
			if (check_out_of_border_with_buffer(asteroid_obj->get_pos_x(), asteroid_obj->get_pos_y())) {
				m_asteroids.remove(asteroid_obj);
			}

			wxRegion hit_area_asteroid = asteroid_obj->get_hit_area();
			hit_area_asteroid.Intersect(m_ship.get_hit_area());
			if (!hit_area_asteroid.IsEmpty() && game_over_is_aktiv) {
				game_over();
				break;
			}
		
			if (check_collision_asteroids_asteroids(asteroid_obj) 
				|| check_collision_asteroids_rockets(asteroid_obj) 
				|| check_collision_asteroids_enemy_ships(asteroid_obj)) {
				break;
			}
		}
	}

	for (auto& rocket_obj : m_rockets) {
		if (rocket_obj != nullptr) {
			if (check_out_of_border_with_buffer(rocket_obj->get_pos_x(), rocket_obj->get_pos_y())) {
				m_rockets.remove(rocket_obj);
				break;
			}
			wxRegion hit_area_rocket = rocket_obj->get_hit_area();
			hit_area_rocket.Intersect(m_ship.get_hit_area());
			if (!hit_area_rocket.IsEmpty() && rocket_obj->get_time() < 0 && game_over_is_aktiv) {
				game_over();
				break;
			}

			if (check_collision_rockets_enemy_ships(rocket_obj)) {
				break;
			}
		}
	}

	for (auto& enemy_obj : m_enemies) {
		if (enemy_obj != nullptr) {
			if (check_out_of_border_with_buffer(enemy_obj->get_pos_x(), enemy_obj->get_pos_y())) {
				m_enemies.remove(enemy_obj);
				break;
			}
		}
		wxRegion hit_area_enemy = enemy_obj->get_hit_area();
		hit_area_enemy.Intersect(m_ship.get_hit_area());
		if (!hit_area_enemy.IsEmpty() && game_over_is_aktiv) {
			game_over();
			break;
		}
		
		if (check_collision_enemy_ships_other_ships(enemy_obj)) {
			break;
		}
	}
}

bool draw_window::check_collision_asteroids_asteroids(std::unique_ptr<asteroid>& asteroid_obj) {
	int x = 0;
	int y = 0;
	bool spawn_small = false;

	for (auto& asteroid_obj_sec : m_asteroids) {
		if ((asteroid_obj_sec != nullptr) && (asteroid_obj != nullptr) && (asteroid_obj_sec != asteroid_obj)) {
			wxRegion hit_area_asteroid_sec = asteroid_obj_sec->get_hit_area();
			hit_area_asteroid_sec.Intersect(asteroid_obj->get_hit_area());
			if (!hit_area_asteroid_sec.IsEmpty()) {
				if (asteroid_obj_sec->get_hitbox() == asteroid_big_hitbox || asteroid_obj->get_hitbox() == asteroid_big_hitbox) {
					spawn_small = true;
					x = asteroid_obj_sec->get_pos_x();
					y = asteroid_obj_sec->get_pos_y();
				}
				
				m_asteroids.remove(asteroid_obj_sec);
				break;
			}
		}
	}
	if (spawn_small) {
		create_small_asteroids(x, y);
		m_asteroids.remove(asteroid_obj);
	}
	return spawn_small;
}

bool draw_window::check_collision_asteroids_rockets(std::unique_ptr<asteroid>& asteroid_obj) {
	int x = 0;
	int y = 0;
	bool spawn_small = false;

	for (auto& rocket_obj : m_rockets) {
		if (rocket_obj != nullptr) {
			wxRegion hit_area_rocket = rocket_obj->get_hit_area();
			hit_area_rocket.Intersect(asteroid_obj->get_hit_area());
			if (!hit_area_rocket.IsEmpty()) {
				if (asteroid_obj->get_hitbox() == asteroid_big_hitbox) {
					spawn_small = true;
					x = asteroid_obj->get_pos_x();
					y = asteroid_obj->get_pos_y();
					if (rocket_obj->get_type() == 0) m_points += 1;
				}
				else {
					if (rocket_obj->get_type() == 0) m_points += 5;
				}
				if (rocket_obj->get_type() == 0) m_hits++;
				if (m_level < 20) m_level++;
				m_rockets.remove(rocket_obj);
				m_asteroids.remove(asteroid_obj);
				break;
			}
		}
	}
	if (spawn_small) {
		create_small_asteroids(x, y);
	}
	return spawn_small;
}

bool draw_window::check_collision_asteroids_enemy_ships(std::unique_ptr<asteroid>& asteroid_obj) {
	int x = 0;
	int y = 0;
	bool spawn_small = false;
	bool collision = false;

	for (auto& enemy_obj : m_enemies) {
		if (enemy_obj != nullptr) {
			if (asteroid_obj != nullptr) {
				wxRegion hit_area_enemy = enemy_obj->get_hit_area();
				hit_area_enemy.Intersect(asteroid_obj->get_hit_area());
				if (!hit_area_enemy.IsEmpty()) {
					if (asteroid_obj->get_hitbox() == asteroid_big_hitbox) {
						spawn_small = true;
						x = asteroid_obj->get_pos_x();
						y = asteroid_obj->get_pos_y();
					}
					m_enemies.remove(enemy_obj);
					m_asteroids.remove(asteroid_obj);
					collision = true;
					break;
				}
			}
		}
	}
	if (spawn_small) {
		create_small_asteroids(x, y);
	}
	return collision;
}

bool draw_window::check_collision_rockets_enemy_ships(std::unique_ptr<rocket>& rocket_obj) {
	for (auto& enemy_obj : m_enemies) {
		if (enemy_obj != nullptr && rocket_obj != nullptr && rocket_obj->get_time() < 0) {
			wxRegion hit_area_enemy = enemy_obj->get_hit_area();
			hit_area_enemy.Intersect(rocket_obj->get_hit_area());
			if (!hit_area_enemy.IsEmpty()) {
				if (rocket_obj->get_type() == 0) {
					m_hits++;
					m_points += 10;
					if (m_level < 20) m_level++;
				}

				m_rockets.remove(rocket_obj);
				m_enemies.remove(enemy_obj);
				return true;
			}
		}
	}
	return false;
}

bool draw_window::check_collision_enemy_ships_other_ships(std::unique_ptr<enemy_ship>& enemy_obj) {
	for (auto& enemy_obj_sec : m_enemies) {
		if (enemy_obj != nullptr && enemy_obj_sec != nullptr && enemy_obj != enemy_obj_sec) {
			wxRegion hit_area_enemy = enemy_obj->get_hit_area();
			hit_area_enemy.Intersect(enemy_obj_sec->get_hit_area());
			if (!hit_area_enemy.IsEmpty()) {
				m_enemies.remove(enemy_obj);
				m_enemies.remove(enemy_obj_sec);
				return true;
			}
		}
	}
	return false;
}

void draw_window::create_small_asteroids(int x, int y) {
	wxPoint planet1{ x - 25, y + 25 };
	wxPoint planet2{ x + 25, y + 25 };
	wxPoint planet3{ x, y - 20 };

	if (!check_out_of_border(planet1.x, planet1.y))	m_asteroids.add(std::make_unique<asteroid>(planet1.x, planet1.y, 40, 1));
	if (!check_out_of_border(planet2.x, planet2.y))	m_asteroids.add(std::make_unique<asteroid>(planet2.x, planet2.y, 220, 1));
	if (!check_out_of_border(planet3.x, planet3.y))	m_asteroids.add(std::make_unique<asteroid>(planet3.x, planet3.y, 320, 1));
}

void draw_window::handle_enemy_ship() {
	for (auto& enemy_obj : m_enemies) {
		if (enemy_obj != nullptr) {
			int action = rand() % 7;
			if (action >= 0 && action <= 3) {
				enemy_obj->fly_forward();
			}
			else if (action == 4) {
				enemy_obj->rotate_left();
			}
			else if (action == 5) {
				enemy_obj->rotate_right();
			}
			else if (action == 6) {
				if (enemy_obj->get_shoot_ready()) {
					int tmp_x = round(enemy_obj->get_pos_x() + (ship_size / 2 - rocket_size / 2) +
						abs(sin(degree_to_rad(enemy_obj->get_degree()))) * abs(cos(degree_to_rad(enemy_obj->get_degree()))) * 10);
					int tmp_y = round(enemy_obj->get_pos_y() + (ship_size / 2 - rocket_size / 2) +
						abs(sin(degree_to_rad(enemy_obj->get_degree()))) * abs(cos(degree_to_rad(enemy_obj->get_degree()))) * 10);
					m_rockets.add(std::make_unique<rocket>(tmp_x, tmp_y, enemy_obj->get_degree(), 1));
					enemy_obj->set_shoot_ready(false);
				}
			}
		}
	}
}
