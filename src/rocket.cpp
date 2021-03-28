// rocket.cpp:												MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#include "rocket.h"

// -------------------- public --------------------

rocket::rocket(int pos_x, int pos_y, double degree, int type) : base{ pos_x, pos_y, degree } {
	wxImage::AddHandler(new wxPNGHandler);
	if (type == 0) {
		m_image.LoadFile(wxT(".\\images\\rocket.png"), wxBITMAP_TYPE_PNG);
		m_type = 0;
	}
	else {
		m_image.LoadFile(wxT(".\\images\\enemy_rocket.png"), wxBITMAP_TYPE_PNG);
		m_type = 1;
	}
	m_time = 1;
	m_image.Rescale(rocket_size, rocket_size);
	m_hitbox = rocket_hitbox;
	m_speed = rocket_forward_move_speed;
}