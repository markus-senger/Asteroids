// enemy_ship.h:											MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include "spaceship_object.h"

class enemy_ship : public spaceship_object {

	using base = spaceship_object;
	using context_t = ml5::paint_event::context_t;

public:

	enemy_ship(int pos_x, int pos_y, double degree);
	~enemy_ship	() = default;
};
