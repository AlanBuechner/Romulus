#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"

namespace Engine::Math
{
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;

	using Mat2 = glm::mat2;
	using Mat3 = glm::mat3;
	using Mat4 = glm::mat4;

	using Quaternion = glm::quat;
	
	// trig
	float Sin(float x);
	float Cos(float x);
	float Tan(float x);
	float Cot(float x);
	float Sec(float x);
	float Csc(float x);

	float Asin(float x);
	float Acos(float x);
	float Atan(float x);

	float Atan2(float y, float x);

	float Sinh(float x);
	float Cosh(float x);
	float Tanh(float x);

	float Degrees(float x);
	float Radians(float x);

	// float functions
	float Sqrt(float x);
	float Pow(float x, float y);
	float Exp(float x);

	float Log(float x);

	float Floor(float x);
	float Ceil(float x);
	float Abs(float x);
	float Clamp(float x, float min, float max);
	float Min(float x, float y);
	float Max(float x, float y);
	float Mod(float x, float y);

	// Vector
	float Length(const Vector2& v);
	float Length(const Vector3& v);
	float Length(const Vector4& v);
	float LengthSqr(const Vector2& v);
	float LengthSqr(const Vector3& v);
	float LengthSqr(const Vector4& v);

	float Distance(const Vector2& v1, const Vector2& v2);
	float Distance(const Vector3& v1, const Vector3& v2);
	float Distance(const Vector4& v1, const Vector4& v2);
	float DistanceSqr(const Vector2& v1, const Vector2& v2);
	float DistanceSqr(const Vector3& v1, const Vector3& v2);
	float DistanceSqr(const Vector4& v1, const Vector4& v2);

	float Dot(const Vector2& v1, const Vector2& v2);
	float Dot(const Vector3& v1, const Vector3& v2);
	float Dot(const Vector4& v1, const Vector4& v2);

	Vector3 Cross(const Vector3& v1, const Vector3& v2);

	Vector2 Normalized(const Vector2& v);
	Vector3 Normalized(const Vector3& v);
	Vector4 Normalized(const Vector4& v);
	Vector2& Normalize(Vector2& v);
	Vector3& Normalize(Vector3& v);
	Vector4& Normalize(Vector4& v);

	Vector2 Lerp(const Vector2& v1, const Vector2& v2, float a);
	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float a);
	Vector4 Lerp(const Vector4& v1, const Vector4& v2, float a);

	// Matrix
	Mat4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	Mat4 Perspective(float fovy, float aspect, float zNear, float zFar);
	Mat4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	Mat4 Rotate(float angle, const Vector3& axis);
	Mat4 Scale(const Vector3& scale);
	Mat4 Translate(const Vector3& position);

	Mat2 Inverse(const Mat2& mat);
	Mat3 Inverse(const Mat3& mat);
	Mat4 Inverse(const Mat4& mat);

	// Random
	float Perlin(float p);
	float Perlin(const Vector2& p);
	float Perlin(const Vector3& p);
	float Perlin(const Vector4& p);

	float Perlin(float p, float rep);
	float Perlin(const Vector2& p, const Vector2& rep);
	float Perlin(const Vector3& p, const Vector3& rep);
	float Perlin(const Vector4& p, const Vector4& rep);

	float Simplex(float p);
	float Simplex(const Vector2& p);
	float Simplex(const Vector3& p);
	float Simplex(const Vector4& p);

	// Quaternion
	float Angle(const Quaternion& q);
	float Length(const Quaternion& q);
	Vector3 Axis(const Quaternion& q);
	Quaternion AngleAxis(float angle, float x, float y, float z);
	Quaternion AngleAxis(float angle, const Vector3& axis);
	Quaternion Inverse(const Quaternion& q);
	Quaternion Conjugate(const Quaternion& q);
	float Dot(const Quaternion& q1, const Quaternion& q2);
	Vector3 EulerAngles(const Quaternion& q);
	Quaternion Mix(const Quaternion& q1, const Quaternion& q2, float a);
	Quaternion Normalize(const Quaternion& q);
	Quaternion Rotate(const Quaternion& q, float angle, const Vector3& axis);
	float Pitch(const Quaternion& q);
	float Roll(const Quaternion& q);
	float Yaw(const Quaternion& q);
	Mat3 Mat3Cast(const Quaternion& q);
	Mat4 Mat4Cast(const Quaternion& q);
	Quaternion QuatCast(const Mat3& mat);
	Quaternion QuatCast(const Mat4& mat);
	Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float a);
	Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float a);



}