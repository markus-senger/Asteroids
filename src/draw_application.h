// draw_application.h:										MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#pragma once

#include <ml5/ml5.h>
#include "draw_window.h"

class draw_application : public ml5::application, MI5_DERIVE(draw_application, ml5::application)  {
	MI5_INJECT(draw_application)

public:

	std::unique_ptr<ml5::window> make_window() const override {
		return std::make_unique<draw_window>();
	}
};