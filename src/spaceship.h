// spaceship.h:												MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include "spaceship_object.h"

class spaceship : public spaceship_object {

	using base = spaceship_object;
	using context_t = ml5::paint_event::context_t;

public:

	spaceship	(int pos_x, int pos_y, double degree);
	~spaceship	() = default;

	void fly_forward	()						override;
	void rotate_right	()						override;
	void rotate_left	()						override;
	void draw			(context_t& context)	override;

	void set_destroy		(bool status) { m_destroyed = status; }


private:

	wxImage		m_space_ship_forward_image;
	wxImage		m_space_ship_left_image;
	wxImage		m_space_ship_right_image;
	wxImage		m_space_ship_destroyed_image;

	int			m_engine_time			{ 0 };
	bool		m_engine_on_main		{ false };
	bool		m_engine_on_left		{ false };
	bool		m_engine_on_right		{ false };
	bool		m_destroyed				{ false };	
};