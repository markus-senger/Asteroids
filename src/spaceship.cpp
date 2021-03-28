// spaceship.cpp:											MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#include "spaceship.h"

// -------------------- public --------------------

spaceship::spaceship(int pos_x, int pos_y, double degree) : base{ pos_x, pos_y, degree } {
	wxImage::AddHandler(new wxPNGHandler);
	m_image.LoadFile(wxT(".\\images\\spaceship.png"), wxBITMAP_TYPE_PNG);
	m_image.Rescale(ship_size, ship_size);

    m_space_ship_forward_image.LoadFile(wxT(".\\images\\spaceship_move_forward.png"), wxBITMAP_TYPE_PNG);
	m_space_ship_forward_image.Rescale(ship_size, ship_size);

	m_space_ship_right_image.LoadFile(wxT(".\\images\\spaceship_move_right.png"), wxBITMAP_TYPE_PNG);
	m_space_ship_right_image.Rescale(ship_size, ship_size);

	m_space_ship_left_image.LoadFile(wxT(".\\images\\spaceship_move_left.png"), wxBITMAP_TYPE_PNG);
	m_space_ship_left_image.Rescale(ship_size, ship_size);

	m_space_ship_destroyed_image.LoadFile(wxT(".\\images\\spaceship_destroyed.png"), wxBITMAP_TYPE_PNG);
	m_space_ship_destroyed_image.Rescale(ship_size, ship_size);

	m_hitbox = ship_hitbox;
	m_speed = ship_forward_move_speed;
}

void spaceship::fly_forward() {
	m_pos.x -= sin(degree_to_rad(m_degree)) * ship_forward_move_speed;
	m_pos.y -= cos(degree_to_rad(m_degree)) * ship_forward_move_speed;

	if (!m_engine_on_left && !m_engine_on_right) {
		m_engine_time = ship_engine_on_time;
		m_engine_on_main = true;
	}
}

void spaceship::rotate_right() {
	m_degree -= ship_rotation_speed;
	if (m_degree <= -360) {
		m_degree += 360;
	}

	if (!m_engine_on_left && !m_engine_on_main) {
		m_engine_time = ship_engine_on_time;
		m_engine_on_right = true;
	}
}

void spaceship::rotate_left() {
	m_degree += ship_rotation_speed;
	if (m_degree >= 360) {
		m_degree -= 360;
	}

	if (!m_engine_on_right && !m_engine_on_main) {
		m_engine_time = ship_engine_on_time;
		m_engine_on_left = true;
	}

}

void spaceship::draw(context_t& context) {
	if (show_hitarea) {
		context.SetPen(*wxRED_PEN);
		context.SetBrush(*wxRED_BRUSH);
		wxPoint p[8];
		calc_hit_area(p);
		context.DrawPolygon(8, p);
	}

	wxImage space_ship_image_buffer;
	if (m_destroyed) {
		space_ship_image_buffer = m_space_ship_destroyed_image.Rotate(degree_to_rad(m_degree), m_pos);
	}
	else if (m_engine_time > 0) {
		if (m_engine_on_main)		space_ship_image_buffer = m_space_ship_forward_image.Rotate(degree_to_rad(m_degree), m_pos);
		else if (m_engine_on_left)	space_ship_image_buffer = m_space_ship_left_image.Rotate(degree_to_rad(m_degree), m_pos);
		else if (m_engine_on_right)	space_ship_image_buffer = m_space_ship_right_image.Rotate(degree_to_rad(m_degree), m_pos);
		m_engine_time--;
	}
	else {
		space_ship_image_buffer = m_image.Rotate(degree_to_rad(m_degree), m_pos);
		m_engine_on_main = false;
		m_engine_on_left = false;
		m_engine_on_right = false;
	}
	context.DrawBitmap(space_ship_image_buffer, m_pos);
}