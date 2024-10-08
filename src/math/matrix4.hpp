#ifndef HIRUKI_MATH_MATRIX_H
#define HIRUKI_MATH_MATRIX_H

#include "math/vector3.hpp"
#include "math/vector4.hpp"
#include <array>
#include <cmath>

namespace Hiruki {
	namespace Math {
		// Matrix of 4x4. Row major.
		class Matrix4 {
		public:
			constexpr Matrix4() : m{0} {};
			inline Matrix4(std::array<std::array<float, 4>, 4> matrix) : m{matrix} {}
			inline Matrix4(
				std::array<float, 4> row0,
				std::array<float, 4> row1,
				std::array<float, 4> row2,
				std::array<float, 4> row3) : m{row0, row1, row2, row3} {};
		
			// Identity matrix
			// ┌         ┐
			// │ 1 0 0 0 │
			// │ 0 1 0 0 │
			// │ 0 0 1 0 │
			// │ 0 0 0 1 │
			// └         ┘
			static constexpr Matrix4 identity() {
				Matrix4 matrix;
		
				matrix.m[0][0] = 1;
				matrix.m[1][1] = 1;
				matrix.m[2][2] = 1;
				matrix.m[3][3] = 1;
		
				return matrix;
			}
		
			// Scale matrix
			// ┌            ┐
			// │ sx 0  0  0 │
			// │ 0  sy 0  0 │
			// │ 0  0  sz 0 │
			// │ 0  0  0  1 │
			// └            ┘
			static inline Matrix4 scale(float scaleX, float scaleY, float scaleZ) {
				Matrix4 matrix;
		
				matrix[0][0] = scaleX;
				matrix[1][1] = scaleY;
				matrix[2][2] = scaleZ;
				matrix[3][3] = 1.0;
		
				return matrix;
			}
			static inline Matrix4 scale(const Vector3 &scaleVector) {
				return scale(scaleVector.x, scaleVector.y, scaleVector.z);
			}
		
			// Translation matrix
			// ┌          ┐
			// │ 1 0 0 tx │
			// │ 0 1 0 ty │
			// │ 0 0 1 tz │
			// │ 0 0 0 1  │
			// └          ┘
			static inline Matrix4 translate(float translateX, float translateY, float translateZ) {
				Matrix4 matrix = Matrix4::identity();
		
				matrix[0][3] = translateX;
				matrix[1][3] = translateY;
				matrix[2][3] = translateZ;
		
				return matrix;
			}
			static inline Matrix4 translate(const Vector3 &translateVector) {
				return translate(translateVector.x, translateVector.y, translateVector.z);
			}
		
			// Rotation matrix (x-axis)
			// ┌              ┐
			// │ 1 0   0    0 │
			// │ 0 cos -sin 0 │
			// │ 0 sin cos  0 │
			// │ 0 0   0    1 │
			// └              ┘
			static inline Matrix4 rotateX(float angle) {
				Matrix4 matrix = Matrix4::identity();
				angle *= M_PI / 180.0f;
		
				float cosElement = std::cos(angle);
				float sinElement = std::sin(angle);
		
				matrix[1][1] = cosElement;
				matrix[1][2] = -sinElement;
				matrix[2][1] = sinElement;
				matrix[2][2] = cosElement;
		
				return matrix;
			}
		
			// Rotation matrix (y-axis)
			// ┌              ┐
			// │ cos  0 sin 0 │
			// │ 0    1 0   0 │
			// │ -sin 0 cos 0 │
			// │ 0    0 0   1 │
			// └              ┘
			static inline Matrix4 rotateY(float angle) {
				Matrix4 matrix = Matrix4::identity();
				angle *= M_PI / 180.0f;
		
				float cosElement = std::cos(angle);
				float sinElement = std::sin(angle);
		
				matrix[0][0] = cosElement;
				matrix[0][2] = sinElement;
				matrix[2][0] = -sinElement;
				matrix[2][2] = cosElement;
		
				return matrix;
			}
		
			// Rotation matrix (z-axis)
			// ┌              ┐
			// │ cos -sin 0 0 │
			// │ sin cos  0 0 │
			// │ 0   0    1 0 │
			// │ 0   0    0 1 │
			// └              ┘
			static inline Matrix4 rotateZ(float angle) {
				Matrix4 matrix = Matrix4::identity();
				angle *= M_PI / 180.0f;
		
				float cosElement = std::cos(angle);
				float sinElement = std::sin(angle);
		
				matrix[0][0] = cosElement;
				matrix[0][1] = -sinElement;
				matrix[1][0] = sinElement;
				matrix[1][1] = cosElement;
		
				return matrix;
			}
		
			// Rotation matrix (all-axis)
			static inline Matrix4 rotateXYZ(Math::Vector3 rotationAngles) {
				return rotateXYZ(rotationAngles.x, rotationAngles.y, rotationAngles.z);
			}
			static inline Matrix4 rotateXYZ(float angleX, float angleY, float angleZ) {
				Matrix4 matrix = Matrix4::identity();
				matrix = matrix.mul(Matrix4::rotateX(angleX));
				matrix = matrix.mul(Matrix4::rotateY(angleY));
				matrix = matrix.mul(Matrix4::rotateZ(angleZ));
		
				return matrix;
			}
		
			// Perspective matrix
			// ┌                                                                                                          ┐
			// │ aspectY * 1 / tan(fovy / 2.0) 0                   0                     0                                │
			// │ 0                             1 / tan(fovy / 2.0) 0                     0                                │
			// │ 0                             0                   zFar / (zFar - zNear) (-zFar * zNear) / (zFar - zNear) │
			// │ 0                             0                   0                     1                                │
			// └                                                                                                          ┘
			static inline Matrix4 perspective(float windowWidth, float windowHeight, float fovyAngles, float zNear, float zFar) {
				float aspectY = windowHeight / windowWidth;
				float f = 1.0 / std::tan((fovyAngles * (M_PI / 180) / 2.0));
				float zRangeReciprocal = 1.0 / (zFar - zNear);
		
				Matrix4 matrix;
		
				matrix[0][0] = aspectY * f;
				matrix[1][1] = f;
				matrix[2][2] = zFar * zRangeReciprocal;
				matrix[2][3] = (-zFar * zNear) * zRangeReciprocal;
				matrix[3][2] = 1.0;
		
				return matrix;
			}
		
			inline Vector4 project(const Vector4 &vector) const {
				return this->mul(vector);
			}
		
			inline Matrix4 mul(const Matrix4 &that) const {
				Matrix4 matrix;
		
				for(int i = 0; i < 4; i++) {
					for(int j = 0; j < 4; j++) {
						matrix[i][j] = 
							(*this)[i][0] * that[0][j] + 
							(*this)[i][1] * that[1][j] + 
							(*this)[i][2] * that[2][j] + 
							(*this)[i][3] * that[3][j];
					}
				}
		
				return matrix;
			}
		
			inline Vector4 mul(const Vector4 &v) const {
				Vector4 newVector;
		
				const Matrix4 &m= *this;
				newVector.x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + v.w * m[0][3];
				newVector.y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + v.w * m[1][3];
				newVector.z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + v.w * m[2][3];
				newVector.w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + v.w * m[3][3];
		
				return newVector;
			}
		
			const std::array<float, 4> &operator[](std::size_t index) const {
				return m[index];
			}
		
			std::array<float, 4> &operator[](std::size_t index) {
				return m[index];
			}
		
			friend std::ostream& operator<<(std::ostream& os, const Matrix4 &matrix) {
				os << "Matrix4:" << std::endl;
		
				for(int i = 0; i < 4; i++) {
					os << "  (";
		
					for(int j = 0; j < 4; j++) {
						os << matrix[i][j];
						if(j != 3)
							os << ", ";
					}
		
					os << ")" << std::endl;
				}
				os << ")" << std::endl;
		
				return os;
			}
		
		private:
			std::array<std::array<float, 4>, 4> m;
		};
	}
}

#endif
