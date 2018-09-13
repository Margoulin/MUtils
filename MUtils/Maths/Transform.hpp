#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

class Transform
{
public:
	Transform();
	Transform(Vector3F const& posValue, Quaternion const& rotValue, Vector3F const& scaleValue = Vector3F::one);
	Transform(Matrix4x4F const& transformationMatrix);
	~Transform() = default;

	auto	Translate(Vector3F const& value) -> void { Position += value; }
	auto	Rotate(Quaternion const& value) -> void { Rotation = Rotation * value; }

	auto	SetPosition(Vector3F const& value) -> void { Position = value; }
	auto	SetScale(Vector3F const& value) -> void { Scale = value; }
	auto	SetRotation(Vector3F const& value) -> void { Rotation = Quaternion::Euler(value); }
	auto	SetRotation(Quaternion const& value) -> void { Rotation = value; }

	auto	GetLocalMatrix() const -> Matrix4x4F;
	auto	GetPosition() const -> Vector3F { return Position; }
	auto	GetScale() const -> Vector3F { return Scale; }
	auto	GetRotation() const -> Vector3F { return Rotation.GetEulerAngles(); }
	auto	GetQuaternionRotation() const -> Quaternion { return Rotation; }

	auto	operator*(Transform const& other) const -> Transform;

	auto	operator=(const Transform&) -> Transform&;

protected:

private:
	Vector3F	Position;
	Vector3F	Scale;
	Quaternion	Rotation;
};


#endif /*__TRANSFORM_HPP__*/