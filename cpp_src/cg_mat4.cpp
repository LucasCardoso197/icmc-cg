#include <cg_mat4.hpp>

namespace cgicmc{
Mat4::Mat4() {
	for(int i=0; i<16; i++)
		matrix[i] = 0.0f;
}

float *Mat4::getData(){
	return matrix;
}

float Mat4::getData(int r, int c) const{
	return matrix[(c*4)+r];
}

void Mat4::setData(int r, int c, float d) {
	if((r >= 0|| r < 4)&&(c >= 0 || c < 4))
		matrix[(c*4)+r] = d;
}

Mat4 Mat4::operator *(const Mat4& M) const{
	Mat4 result;
	float product;

	for(int r=0; r<4; r++){
		for(int c=0; c<4; c++){
			product = 0;
			for(int i=0; i<4; i++)
				product += matrix[(i*4)+r] * M.getData(i, c);
			result.setData(r, c, product);
		}
	}

	return result;
}

void Mat4::print() const{
	int r, c;
	for(r=0; r<4; r++){
		for(c=0; c<3; c++)
			std::cout << matrix[(c*4)+r] << " ";
		std::cout << matrix[(c*4)+r] << std::endl;
	}
    std::cout << std::endl;
};

// Transformações
void Mat4::translate(float tx, float ty, float tz){
	const float aux[16] = {1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							tx, ty, tz, 1.0f};

	for(int i=0; i<16; i++)
		setData(i%4, i/4, aux[i]);
}

void Mat4::rotateXAxis(float angle){
	float s = sin(angle*M_PI/180);
	float c = cos(angle*M_PI/180);

	const float aux[16] = {1.0f, 0.0f, 0.0f, 0.0f,
						   0.0f, c, s, 0.0f,
						   0.0f, -s, c, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f};

	for(int i=0; i<16; i++)
		setData(i%4, i/4, aux[i]);
}

void Mat4::rotateYAxis(float angle){
	float s = sin(angle*M_PI/180);
	float c = cos(angle*M_PI/180);

	const float aux[16] = {c, 0.0f, s, 0.0f,
						   0.0f, 1.0f, 0.0f, 0.0f,
						   -s, 0.0f, c, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f};

	for(int i=0; i<16; i++)
		setData(i%4, i/4, aux[i]);
}

void Mat4::rotateZAxis(float angle){
	float s = sin(angle*M_PI/180);
	float c = cos(angle*M_PI/180);

	const float aux[16] = {c, s, 0.0f, 0.0f,
						   -s, c, 0.0f, 0.0f,
						   0.0f, 0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f};

	for(int i=0; i<16; i++)
		setData(i%4, i/4, aux[i]);
}

void Mat4::rotate(float *xAxisVersor, float *yAxisVersor, float *zAxisVersor){
	const float aux[16] = {xAxisVersor[0], xAxisVersor[1], xAxisVersor[2], 0.0f,
						   yAxisVersor[0], yAxisVersor[1], yAxisVersor[2], 0.0f,
						   zAxisVersor[0], zAxisVersor[1], zAxisVersor[2], 0.0f,
						   0.0f, 0.0f, 0.0f, 1.0f};

	for(int i=0; i<16; i++)
		setData(i%4, i/4, aux[i]);
}

void Mat4::scale(float Sx, float Sy, float Sz){
	const float aux[16] = {Sx, 0.0f, 0.0f, 0.0f,
                           0.0f, Sy, 0.0f, 0.0f,
                           0.0f, 0.0f, Sz, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f};

    for(int i=0; i<16; i++)
        setData(i%4, i/4, aux[i]);
}

void Mat4::identity(){
    const float aux[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f};

    for(int i=0; i<16; i++)
        setData(i%4, i/4, aux[i]);
}

void Mat4::view(float *cameraPos, float *lookAtPoint, float *viewUpVec){
	float u[3], v[3], n[3];
	for(int i=0; i<16; i++)
		matrix[i] = 0.0f;
	for(int i=0; i<3; i++)
		n[i] = cameraPos[i] - lookAtPoint[i];
	normalizeVector(n);

	crossProduct(n, viewUpVec, u);
	normalizeVector(u);
	crossProduct(u, n, v);

	const float aux[16] = {u[0], v[0], n[0], 0.0f,
						   u[1], v[1], n[1], 0.0f,
						   u[2], v[2], n[2], 0.0f,
						   -dotProduct(cameraPos, u), -dotProduct(cameraPos, v), -dotProduct(cameraPos, n), 1.0f};

    for(int i=0; i<16; i++)
        setData(i%4, i/4, aux[i]);
}

void Mat4::perspetiveProjection(int height, int width, float fov, float near, float far){
	float t = tan(fov/2.0*M_PI/180);
	float a = (float) width / (float) height;
	for(int i=0; i<16; i++)
		matrix[i] = 0.0f;
	setData(0, 0, 1/(a*t));
	setData(1, 1, 1/t);
	setData(2, 2, (far+near)/(near-far));
	setData(3, 2, -1.0);
	setData(2, 3, (2*far*near)/(near-far));
}

void Mat4::ortogonalProjection(float *boxMax, float *boxMin){
	for(int i=0; i<16; i++)
		matrix[i] = 0.0f;
	setData(0, 0, 2/(boxMax[0]-boxMin[0]));
	setData(1, 1, 2/(boxMax[1]-boxMin[1]));
	setData(2, 2, -2/(boxMax[2]-boxMin[2]));
	setData(0, 3, -(boxMax[0]+boxMin[0]) / (boxMax[0]-boxMin[0]));
	setData(1, 3, -(boxMax[1]+boxMin[1]) / (boxMax[1]-boxMin[1]));
	setData(2, 3, -(boxMax[2]+boxMin[2]) / (boxMax[2]-boxMin[2]));
	setData(3, 3, 1.0);
}

} // Fim namespace cgicmc