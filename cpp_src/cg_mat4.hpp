#ifndef _CG_MAT4_
#define _CG_MAT4_

#include <iostream>
#include <cmath>

namespace cgicmc{
class Mat4 {
public:
	Mat4();
	~Mat4() {};

	float* getData();
	float getData(int r, int c) const;
	void setData(int r, int c, float d);
	void rotateZAxis(float angle);
	void translate(float tx, float ty, float tz);
	void scale(float Sx, float Sy, float Sz);
	void identity();
	Mat4 operator *(const Mat4& M) const;
	void print() const;
private:
	float matrix[16];
};
}

#endif