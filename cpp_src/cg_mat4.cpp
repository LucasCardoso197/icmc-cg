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

/* row,col => index
		(col*4)+r
	index => row, col
		row = i%4
		col = i/4 */
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


void Mat4::translate(float tx, float ty, float tz){
	const float aux[16] = {1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							tx, ty, tz, 1.0f};

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

void Mat4::scale(float Sx, float Sy, float Sz){
	const float aux[16] = {Sx, 0.0f, 0.0f, 0.0f,
                           0.0f, Sy, 0.0f, 0.0f,
                           0.0f, 0.0f, Sz, 0.0f,
                           0.2f, 0.2f, 0.0f, 1.0f};

    for(int i=0; i<16; i++)
        setData(i%4, i/4, aux[i]);
}

void Mat4::identity(){
    const float aux[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.2f, 0.2f, 0.0f, 1.0f};

    for(int i=0; i<16; i++)
        setData(i%4, i/4, aux[i]);
}
}