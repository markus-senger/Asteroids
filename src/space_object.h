// space_object.h:											MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include <ml5/ml5.h>
#include "types.h"

class space_object : public ml5::object, MI5_DERIVE(space_object, ml5::object) {
	MI5_INJECT(space_object)

	using context_t = ml5::paint_event::context_t;

public:

	space_object	(int pos_x, int pos_y, double degree) : m_pos{ pos_x, pos_y }, m_degree{ degree } {}
	~space_object	() = default;

	virtual void	draw			(context_t& context);
	virtual void	fly_forward		();
	void			calc_hit_area	(wxPoint* corners);

	int			get_pos_x		()	const { return m_pos.x; }
	int			get_pos_y		()	const { return m_pos.y; }
	double		get_degree		()	const { return m_degree; }
	int			get_hitbox		()	const { return m_hitbox; }
	int			get_speed		()	const { return m_speed; }
	bool		get_deletable	()	const { return m_deletable; }
	wxRegion	get_hit_area	();

	void		set_deletable	(bool status) { m_deletable = status; }


protected:

	wxImage		m_image;

	wxPoint		m_pos		{ 0,0 };
	double		m_degree	{ 0 };
	int			m_hitbox	{ 0 };
	int			m_speed		{ 0 };
	bool		m_deletable	{ false };
};
