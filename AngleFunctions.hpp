#ifndef ANGLE_FUNCTIONS_HPP
#define ANGLE_FUNCTIONS_HPP

#include <cmath>
#include <iostream>

namespace angle {
	/**
	 * @brief determines if current_angle is betwen min_angle and max_angle
	 */
	inline bool angleBetween(float min_angle, float max_angle, float current_angle) {
		if(max_angle >= min_angle) {
			return (current_angle > min_angle && current_angle < max_angle);
		} else {
			return !(current_angle > max_angle && current_angle < min_angle);
		}
	}

	/**
	 * @brief wraps an angle to a number in the range 0 to 360 degrees
	 * @return wrapped angle
	 */
	inline float wrapTo360(float angle) {
		if (angle < 0) {
			do {
				angle += 360;
			} while (angle < 0);
		} else if (angle >= 360) {
			do {
				angle -= 360;
			} while (angle >= 360);
		}
		return angle;
	}

	inline float wrapTo180(float angle) {
		if (angle < 0) {
			do {
				angle += 180;
			} while (angle < 0);
		} else if (angle >= 180) {
			do {
				angle -= 180;
			} while (angle >= 180);
		}
		return angle;
	}

	/**
	 * @brief calculates difference between two angles
	 * @param angle1 first angle in range 0-360
	 * @param angle2 second angle in range 0-360
	 * @return returns difference in range 0-180
	 */
	inline float angleDiffDegrees(float angle1, float angle2) {
		float diff = angle1 - angle2;
		std::cout << "angles: " << angle1 << '\t' << angle2 << '\n';
		std::cout << "diff: " << diff << '\n';
		return wrapTo180(std::fabs(diff));
	}

}


#endif /* ANGLE_FUNCTIONS_HPP */