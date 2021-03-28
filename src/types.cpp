// types.cpp:												MSe, 2021-02-28 
// v1.0
// FH Oberösterreich / Campus Hagenberg
// ----                                                                                                                    
// ========================================================================

#include "types.h"
#include <ml5/ml5.h>

// auxiliary functions
double degree_to_rad(double degree) {
	return (degree * M_PI / 180);
}

// controls
bool fly_foward_key(int key) {
	switch (key) {
	case 'w': return true;
	case 'W': return true;
	case 315: return true;
	}

	return false;
}

bool rotation_right_key(int key) {
	switch (key) {
	case 'd': return true;
	case 'D': return true;
	case 316: return true;
	}

	return false;
}

bool rotation_left_key(int key) {
	switch (key) {
	case 'a': return true;
	case 'A': return true;
	case 314: return true;
	}

	return false;
}

bool shoot_key(int key) {
	switch (key) {
	case ' ': return true;
	}

	return false;
}
