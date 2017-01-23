/**
 * @file vector2d.h
 * Defines 2D Vector class
 */

#ifndef PHYSICS_VECTOR2D_H
#define PHYSICS_VECTOR2D_H

#include <iostream>
#include "physics_export.h"

namespace physics {

/**
 * Class for 2D vectors
 */
class PHYSICS_EXPORT Vector2D {
public:
	Vector2D();
	Vector2D(double x, double y);

	/**
	 * Equal to operator for vectors
	 *
	 * @param[in]  rhs   The vector to be compared against
	 *
	 * @return     true if the vectors are equal, else false
	 */
	bool operator==(Vector2D rhs);

	/**
	 * Addition operator for vectors
	 *
	 * @param[in]  rhs   The vector to be added with
	 *
	 * @return     Sum of the vectors
	 */
	Vector2D operator+(Vector2D rhs);

	/**
	 * Minus operator for vectors
	 * Subtracts the paramater passed from the vector
	 *
	 * @param[in]  rhs   The vector that subtracts from
	 *
	 * @return     Difference of the vectors
	 */
	Vector2D operator-(Vector2D rhs);

	/**
	 * Scalar Addition operator
	 * vec.x + scalar, vec.y + scalar
	 *
	 * @param[in]  scalar  The scalar to add
	 *
	 * @return     The new vector
	 */
	Vector2D operator+(double scalar);

	/**
	 * Scalar Subtraction operator
	 * vec.x - scalar, vec.y - scalar
	 *
	 * @param[in]  scalar  The scalar to subtract
	 *
	 * @return     The new vector
	 */
	Vector2D operator-(double scalar);

	/**
	 * Scalar multiplication operator
	 *
	 * @param[in]  scalar  The factor to multiply by
	 *
	 * @return     The scaled vector
	 */
	Vector2D operator*(double scalar);

	/**
	 * Scalar division operator
	 *
	 * @param[in]  scalar  The factor to divide by
	 *
	 * @return     The scaled vector
	 */
	Vector2D operator/(double scalar);

	friend std::ostream& operator<<(
		std::ostream& ostream,
		const Vector2D& vector
	);
	/**
	 * Dot product of vectors
	 *
	 * @param[in]  rhs   The vector with which the operation is performed
	 *
	 * @return     The result of the dot product
	 */
	double dot(Vector2D rhs);

	/**
	 * The magnitude of the vector
	 *
	 * @return     The magnitude of the vector
	 */
	double magnitude();
	/**
	 * Distance between this and another vector
	 *
	 * @param[in]  other  The other vector
	 *
	 * @return     The distance between the two vectors
	 */
	double distance(Vector2D other);

	double x;
	double y;
};

}

#endif
