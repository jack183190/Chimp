#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"

namespace Chimp {
	struct TransformComponent {
		TransformComponent() = default;
		TransformComponent(const Vector3f& translation, const Vector3f& rotation, const Vector3f& scale)
			: m_Transform{ translation, rotation, scale} {
			UpdateTransform();
		}
		TransformComponent(const Transform& transform)
			: m_Transform(transform) {
			UpdateTransform();
		}

		const Vector3f& GetTranslation() const { return m_Transform.Translation; }
		const Vector3f& GetRotation() const { return m_Transform.Rotation; }
		const Vector3f& GetScale() const { return m_Transform.Scale; }

		void SetTranslation(const Vector3f& translation) { 
			m_Transform.Translation = translation;
			UpdateTransform();
		}
		// yaw, pitch, roll in radians
		void SetRotation(const Vector3f& rotation) {
			m_Transform.Rotation = rotation;
			UpdateTransform();
		}
		void SetScale(const Vector3f& scale) { 
			m_Transform.Scale = scale;
			UpdateTransform();
		}

		const Matrix& GetTransformMatrix() const { return m_TransformMatrix; }
		const Transform& GetTransform() const { return m_Transform; } // Can't be changed since it wouldn't update our matrix

	private:
		void UpdateTransform() {
			m_TransformMatrix = m_Transform.CreateTransformMatrix();
		}

	private:
		Transform m_Transform;
		Matrix m_TransformMatrix = CreateIdentityMatrix();
	};
}