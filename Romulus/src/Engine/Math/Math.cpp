#include "pch.h"
#include "Math.h"
#include <glm\gtc\noise.hpp>
#include <math.h>

namespace Engine::Math
{
	// trig
	float Sin(float x)
	{
		return glm::sin(x);
	}

	float Cos(float x)
	{
		return glm::cos(x);
	}

	float Tan(float x)
	{
		return glm::tan(x);
	}

	float Cot(float x)
	{
		return 1/glm::tan(x);
	}

	float Sec(float x)
	{
		return 1.0f / glm::cos(x);
	}

	float Csc(float x)
	{
		return 1.0f / glm::sin(x);
	}

	float Asin(float x)
	{
		return glm::asin(x);
	}

	float Acos(float x)
	{
		return glm::acos(x);
	}

	float Atan(float x)
	{
		return glm::atan(x);
	}

	float Atan2(float y, float x)
	{
		return glm::atan(y/x);
	}

	float Sinh(float x)
	{
		return glm::sinh(x);
	}

	float Cosh(float x)
	{
		return glm::cosh(x);
	}

	float Tanh(float x)
	{
		return glm::tanh(x);
	}

	float Degrees(float x)
	{
		return glm::degrees(x);
	}

	float Radians(float x)
	{
		return glm::radians(x);
	}

	// float functions
	float Sqrt(float x)
	{
		return glm::sqrt(x);
	}

	float Pow(float x, float y)
	{
		return glm::pow(x, y);
	}

	float Exp(float x)
	{
		return glm::exp(x);
	}

	float Log(float x)
	{
		return glm::log(x);
	}

	float Floor(float x)
	{
		return glm::floor(x);
	}

	float Ceil(float x)
	{
		return glm::ceil(x);
	}

	float Abs(float x)
	{
		return glm::abs(x);
	}

	float Clamp(float x, float min, float max)
	{
		return glm::clamp(x, min, max);
	}

	float Min(float x, float y)
	{
		return glm::min(x, y);
	}

	float Max(float x, float y)
	{
		return glm::max(x, y);
	}

	float Mod(float x, float y)
	{
		return glm::mod(x, y);
	}

	// Vectors
	float Length(const Vector2& v)
	{
		return glm::length(v);
	}

	float Length(const Vector3& v)
	{
		return glm::length(v);
	}

	float Length(const Vector4& v)
	{
		return glm::length(v);
	}

	float LengthSqr(const Vector2& v)
	{
		return (v.x * v.x) + (v.y * v.y);
	}

	float LengthSqr(const Vector3& v)
	{
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	}

	float LengthSqr(const Vector4& v)
	{
		return (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
	}

	float Distance(const Vector2& v1, const Vector2& v2)
	{
		return Length(v2 - v1);
	}

	float Distance(const Vector3& v1, const Vector3& v2)
	{
		return Length(v2 - v1);
	}

	float Distance(const Vector4& v1, const Vector4& v2)
	{
		return Length(v2 - v1);
	}

	float DistanceSqr(const Vector2& v1, const Vector2& v2)
	{
		return LengthSqr(v2 - v1);
	}

	float DistanceSqr(const Vector3& v1, const Vector3& v2)
	{
		return LengthSqr(v2 - v1);
	}

	float DistanceSqr(const Vector4& v1, const Vector4& v2)
	{
		return LengthSqr(v2 - v1);
	}

	float Dot(const Vector2& v1, const Vector2& v2)
	{
		return glm::dot(v1, v2);
	}

	float Dot(const Vector3& v1, const Vector3& v2)
	{
		return glm::dot(v1, v2);
	}

	float Dot(const Vector4& v1, const Vector4& v2)
	{
		return glm::dot(v1, v2);
	}

	Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return glm::cross(v1, v2);
	}

	Vector2 Normalized(const Vector2& v)
	{
		return glm::normalize(v);
	}

	Vector3 Normalized(const Vector3& v)
	{
		return glm::normalize(v);
	}

	Vector4 Normalized(const Vector4& v)
	{
		return glm::normalize(v);
	}

	Vector2& Normalize(Vector2& v)
	{
		return v = glm::normalize(v);
	}

	Vector3& Normalize(Vector3& v)
	{
		return v = glm::normalize(v);
	}

	Vector4& Normalize(Vector4& v)
	{
		return v = glm::normalize(v);
	}

	Vector2 Lerp(const Vector2& v1, const Vector2& v2, float a)
	{
		return (v1 * a) + v2 * (1.f - a);
	}

	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float a)
	{
		return (v1 * a) + v2 * (1.f - a);
	}

	Vector4 Lerp(const Vector4& v1, const Vector4& v2, float a)
	{
		return (v1 * a) + v2 * (1.f - a);
	}

	// Matrix
	Mat4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return glm::ortho(left, right, bottom, top, zNear, zFar);
	}

	Mat4 Perspective(float fovy, float aspect, float zNear, float zFar)
	{
		return glm::perspective(fovy, aspect, zNear, zFar);
	}

	Mat4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
	{
		return glm::lookAt(eye, center, up);
	}

	Mat4 Rotate(float angle, const Vector3& axis)
	{
		return glm::rotate(glm::mat4(1.0f), angle, axis);
	}

	Mat4 Scale(const Vector3& scale)
	{
		return glm::scale(glm::mat4(1.0f), scale);
	}

	Mat4 Translate(const Vector3& position)
	{
		return glm::translate(glm::mat4(1.0f), position);
	}

	Mat2 Inverse(const Mat2& mat)
	{
		return glm::inverse(mat);
	}

	Mat3 Inverse(const Mat3& mat)
	{
		return glm::inverse(mat);
	}

	Mat4 Inverse(const Mat4& mat)
	{
		return glm::inverse(mat);
	}



	// Random
	float Perlin(float p)
	{
		return glm::perlin(glm::vec1(p));
	}

	float Perlin(const Vector2& p)
	{
		return glm::perlin(p);
	}

	float Perlin(const Vector3& p)
	{
		return glm::perlin(p);
	}

	float Perlin(const Vector4& p)
	{
		return glm::perlin(p);
	}

	float Perlin(float p, float rep)
	{
		return glm::perlin(glm::vec1(p), glm::vec1(rep));
	}

	float Perlin(const Vector2& p, const Vector2& rep)
	{
		return glm::perlin(p, rep);
	}

	float Perlin(const Vector3& p, const Vector3& rep)
	{
		return glm::perlin(p, rep);
	}

	float Perlin(const Vector4& p, const Vector4& rep)
	{
		return glm::perlin(p, rep);
	}

	float Simplex(float p)
	{
		return glm::simplex(glm::vec1(p));
	}

	float Simplex(const Vector2& p)
	{
		return glm::simplex(p);
	}

	float Simplex(const Vector3& p)
	{
		return glm::simplex(p);
	}

	float Simplex(const Vector4& p)
	{
		return glm::simplex(p);
	}

	float Angle(const Quaternion& x)
	{
		return glm::angle(x);
	}

	float Length(const Quaternion& q)
	{
		return 0.0f;
	}



	// Quaternion
	Vector3 Axis(const Quaternion& q)
	{
		return glm::axis(q);
	}

	Quaternion AngleAxis(float angle, float x, float y, float z)
	{
		return glm::angleAxis(angle, Vector3{ x,y,z });
	}

	Quaternion AngleAxis(float angle, const Vector3& axis)
	{
		return glm::angleAxis(angle, axis);
	}

	Quaternion Inverse(const Quaternion& q)
	{
		return glm::inverse(q);
	}

	Quaternion Conjugate(const Quaternion& q)
	{
		return glm::conjugate(q);
	}

	float Dot(const Quaternion& q1, const Quaternion& q2)
	{
		return glm::dot(q1, q2);
	}

	Vector3 EulerAngles(const Quaternion& q)
	{
		return glm::eulerAngles(q);
	}

	Quaternion Mix(const Quaternion& q1, const Quaternion& q2, float a)
	{
		return glm::mix(q1, q2, a);
	}

	Quaternion Normalize(const Quaternion& q)
	{
		return glm::normalize(q);
	}

	Quaternion Rotate(const Quaternion& q, float angle, const Vector3& axis)
	{
		return glm::rotate(q, angle, axis);
	}

	float Pitch(const Quaternion& q)
	{
		return glm::pitch(q);
	}

	float Roll(const Quaternion& q)
	{
		return glm::roll(q);
	}

	float Yaw(const Quaternion& q)
	{
		return glm::yaw(q);
	}

	Mat3 Mat3Cast(const Quaternion& q)
	{
		return glm::mat3_cast(q);
	}

	Mat4 Mat4Cast(const Quaternion& q)
	{
		return glm::mat4_cast(q);
	}

	Quaternion QuatCast(const Mat3& mat)
	{
		return glm::quat_cast(mat);
	}

	Quaternion QuatCast(const Mat4& mat)
	{
		return glm::quat_cast(mat);
	}

	Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float a)
	{
		return glm::lerp(q1, q2, a);
	}

	Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float a)
	{
		return glm::slerp(q1, q2, a);
	}

}