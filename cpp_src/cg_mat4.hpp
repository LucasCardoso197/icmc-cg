#ifndef _CG_MAT4_
#define _CG_MAT4_

#include <iostream>
#include <cmath>
#include <cg_utils.hpp>

namespace cgicmc{
class Mat4 {
public:
	Mat4();
	~Mat4() {};

	float* getData();
	float getData(int r, int c) const;
	void setData(int r, int c, float d);
	void rotateXAxis(float angle);
	void rotateYAxis(float angle);
	void rotateZAxis(float angle);
	void rotate(float *xVersor, float *yVersor, float *zVersor);
	void translate(float tx, float ty, float tz);
	void scale(float Sx, float Sy, float Sz);
	void view(float *cameraPos, float *lookAtPoint, float *viewUpVec);
	void perspetiveProjection(int height, int width, float fov, float near, float far);
	void ortogonalProjection(float *boxMax, float *boxMin);
	void identity();
	Mat4 operator *(const Mat4& M) const;
	void print() const;
private:
	float matrix[16];
};
}

#endif