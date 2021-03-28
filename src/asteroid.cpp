// asteroid.cpp:					MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#include "asteroid.h"

// -------------------- public --------------------

asteroid::asteroid(int pos_x, int pos_y, double degree, int type) : base{ pos_x, pos_y, degree } {
	wxImage::AddHandler(new wxPNGHandler);
	if (type == 0) {
		m_image.LoadFile(wxT(".\\images\\asteroid_big.png"), wxBITMAP_TYPE_PNG);
		m_image.Rescale(asteroid_big_size, asteroid_big_size);
		m_hitbox = asteroid_big_hitbox;
	}
	else {
		m_image.LoadFile(wxT(".\\images\\asteroid_small.png"), wxBITMAP_TYPE_PNG);
		m_image.Rescale(asteroid_small_size, asteroid_small_size);
		m_hitbox = asteroid_small_hitbox;
	}
	m_speed = asteroid_forward_move_speed;
}
