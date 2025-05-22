#pragma once
#include "Vectors.h"

namespace CameraTransformations {

	inline Vector2Int screenToWorldPoint(Vector2Float screenPosition, Vector2Int screenDimensions, float cameraAngle, float cameraScale, Vector2Int cameraPosition) {
		Vector2Int screenCenter = screenDimensions / 2.0f;
		Vector2Float offset = screenPosition;//- Vectors::toVector2Float(screenCenter);

		Vector2Float rotated = offset.rotate(-cameraAngle);       // Un-rotate
		Vector2Int scaled = Vectors::toVector2Int(rotated * cameraScale);               // Scale up to world space

		Vector2Int worldPoint = scaled.scaleToWorldPosition() + cameraPosition;

		return worldPoint;  // Or use rounding if needed
	}

	inline Vector2Float worldToScreenPoint(Vector2Int worldPosition, Vector2Int screenDimensions, float cameraAngle, float cameraScale, Vector2Int cameraPos) {
		Vector2Float screenCenter = Vectors::toVector2Float(screenDimensions) / 2.0f;

		Vector2Float offset = Vectors::toVector2Float(worldPosition - cameraPos);
		Vector2Float scaled = (offset / cameraScale).scaleToScreenPosition();                // Scale down to screen space
		Vector2Float rotated = scaled.rotate(cameraAngle);         // Rotate to camera view

		Vector2Float screenFloat = rotated;

		return screenFloat;
	}
}
