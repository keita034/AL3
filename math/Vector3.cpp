#include "Vector3.h"

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;

	}

	return *this;
}

float Vector3::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}
