// asteroid.h:												MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include "space_object.h"

class asteroid : public space_object {

	using base = space_object;
	using context_t = ml5::paint_event::context_t;

public:

	asteroid	(int pos_x, int pos_y, double degree, int type);
	~asteroid	() = default;
};