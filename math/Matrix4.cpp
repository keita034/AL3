#include "Matrix4.h"

void Matrix4::IdentityMatrix() {
	Matrix4 matZero;

	*this = matZero;

	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;
}
