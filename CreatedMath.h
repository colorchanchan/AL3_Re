#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

Vector3 Add(Vector3 a, Vector3 b);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(float scalar, const Vector3& v);
float Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);

float Dot(const Vector3& v1, const Vector3& v2);
Vector3 Lerp(float t, const Vector3& s, const Vector3& e);
Vector3 Slerp(float t, const Vector3& s, const Vector3& e);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// inline Vector3 Add(const Vector3& v1, const Vector3& v2) {
//	Vector3 result;
//	result.x = v1.x + v2.x;
//	result.y = v1.y + v2.y;
//	result.z = v1.z + v2.z;
//	return result;
// }

Matrix4x4 Inverse(const Matrix4x4& m);
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4
    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

 Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

 Matrix4x4 MakeViewPortMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);