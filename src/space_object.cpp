// space_object.cpp:										MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#include "space_object.h"

// -------------------- public --------------------

void space_object::draw(context_t& context) {
	if (show_hitarea) {
		context.SetPen(*wxRED_PEN);
		context.SetBrush(*wxRED_BRUSH);
		wxPoint corners[hitarea_corners];
		calc_hit_area(corners);
		context.DrawPolygon(hitarea_corners, corners);
	}

	wxImage image_buffer;
	image_buffer = m_image.Rotate(degree_to_rad(m_degree), m_pos, true);
	context.DrawBitmap(image_buffer, m_pos);
}

void space_object::fly_forward() {
	m_pos.x -= sin(degree_to_rad(m_degree)) * m_speed;
	m_pos.y -= cos(degree_to_rad(m_degree)) * m_speed;
}

wxRegion space_object::get_hit_area() {
	wxPoint corners[hitarea_corners];
	calc_hit_area(corners);

	return wxRegion{ hitarea_corners, corners };
}

void space_object::calc_hit_area(wxPoint* corners) {
	int tmp_x = round(m_pos.x + (m_hitbox / 2) + abs(sin(degree_to_rad(m_degree))) * abs(cos(degree_to_rad(m_degree))) * 20);
	int tmp_y = round(m_pos.y + (m_hitbox / 2) + abs(sin(degree_to_rad(m_degree))) * abs(cos(degree_to_rad(m_degree))) * 20);

	int degree_steps = 360 / hitarea_corners;
	int degree = 0;
	for (int i = 0; i < hitarea_corners; i++) {
		corners[i] = { wxPoint{ (int)round(tmp_x + cos(degree_to_rad(degree)) * m_hitbox / 2), (int)round(tmp_y + sin(degree_to_rad(degree)) * m_hitbox / 2) } };
		degree += degree_steps;
	}
}