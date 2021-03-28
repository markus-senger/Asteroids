// enemy_ship.cpp:											MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#include "enemy_ship.h"

// -------------------- public --------------------

enemy_ship::enemy_ship(int pos_x, int pos_y, double degree) : base{ pos_x, pos_y, degree } {
	wxImage::AddHandler(new wxPNGHandler);
	m_image.LoadFile(wxT(".\\images\\enemy_ship.png"), wxBITMAP_TYPE_PNG);
	m_image.Rescale(enemy_ship_size, enemy_ship_size);
	m_hitbox = enemy_ship_hitbox;
	m_speed = enemy_ship_forward_move_speed;
}