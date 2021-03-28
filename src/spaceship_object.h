// spaceship_object.h:										MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include "space_object.h"

class spaceship_object : public space_object {

	using base = space_object;
	using context_t = ml5::paint_event::context_t;

public:

	spaceship_object	(int pos_x, int pos_y, double degree) : base{ pos_x, pos_y, degree } {}
	~spaceship_object	() = default;

	virtual void fly_forward	()			override;
	virtual void rotate_right	();
	virtual void rotate_left	();
	virtual void draw		(context_t& context)	override;

	bool get_shoot_ready	()	const { return m_shoot_ready; }

	void set_shoot_ready	(bool status) { m_shoot_ready = status; }


private:

	bool	m_shoot_ready{ false };
};
