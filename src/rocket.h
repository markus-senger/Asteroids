// rocket.h:												MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include "space_object.h"

class rocket : public space_object {

	using base = space_object;
	using context_t = ml5::paint_event::context_t;

public:

	rocket	(int pos_x, int pos_y, double degree, int type = 0);
	~rocket	() = default;

	int get_type() const { return m_type; }
	int get_time() const { return m_time; }

	void set_time(int time) { m_time = time; }

private:

	int m_type	{ 0 };
	int m_time	{ 0 };
};