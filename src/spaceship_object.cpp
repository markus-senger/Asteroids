// spaceship_object.cpp:									MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#include "spaceship_object.h"

// -------------------- public --------------------

void spaceship_object::fly_forward() {
	m_pos.x -= sin(degree_to_rad(m_degree)) * ship_forward_move_speed;
	m_pos.y -= cos(degree_to_rad(m_degree)) * ship_forward_move_speed;
}

void spaceship_object::rotate_right() {
	m_degree -= ship_rotation_speed;
	if (m_degree <= -360) {
		m_degree += 360;
	}
}

void spaceship_object::rotate_left() {
	m_degree += ship_rotation_speed;
	if (m_degree >= 360) {
		m_degree -= 360;
	}
}

void spaceship_object::draw(context_t& context) {
	if (show_hitarea) {
		context.SetPen(*wxRED_PEN);
		context.SetBrush(*wxRED_BRUSH);
		wxPoint p[8];
		calc_hit_area(p);
		context.DrawPolygon(8, p);
	}

	wxImage space_ship_image_buffer;
	space_ship_image_buffer = m_image.Rotate(degree_to_rad(m_degree), m_pos);

	context.DrawBitmap(space_ship_image_buffer, m_pos);
}