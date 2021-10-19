#include "Matrix.h"

namespace Engine::Math
{
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
}