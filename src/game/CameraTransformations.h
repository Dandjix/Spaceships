#pragma once
#include "../math/Vectors.h"

namespace CameraTransformations {

	/// <summary>
	/// this one generally works. Sometimes you have to use the other one idk why please someone put me out of my misery
	/// </summary>
	/// <param name="screenPosition"></param>
	/// <param name="screenDimensions"></param>
	/// <param name="cameraAngle"></param>
	/// <param name="cameraScale"></param>
	/// <param name="cameraPosition"></param>
	/// <returns></returns>
	inline Vector2Int screenToWorldPoint(Vector2Float screenPosition, Vector2Int screenDimensions, float cameraAngle, float cameraScale, Vector2Int cameraPosition) {
		
		Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2;
		Vector2Float offsetScreenPosition = screenPosition - halfScreenDim;

		Vector2Float rotated = offsetScreenPosition.rotate(-cameraAngle);
		Vector2Float scaled = (rotated) * cameraScale;

		Vector2Int worldPoint = Vectors::toVector2Int(scaled).scaleToWorldPosition() + cameraPosition;

		return worldPoint;
	}

	/// <summary>
	/// works fine actually i think maybe
	/// </summary>
	/// <param name="worldPosition"></param>
	/// <param name="screenDimensions"></param>
	/// <param name="cameraAngle"></param>
	/// <param name="cameraScale"></param>
	/// <param name="cameraPos"></param>
	/// <returns></returns>
	inline Vector2Float worldToScreenPoint(Vector2Int worldPosition, Vector2Int screenDimensions, float cameraAngle, float cameraScale, Vector2Int cameraPos)
	{
			// 1. Get half screen dim
			Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2.0f;

			// 2. Undo "+ cameraPosition"
			Vector2Float local = Vectors::toVector2Float(worldPosition - cameraPos);

			// 3. Undo scaleToWorldPosition() -> return to the "scaled" grid space
			Vector2Float grid = local.scaleToScreenPosition();

			// 4. Undo "* cameraScale"
			Vector2Float unscaled = grid / cameraScale;

			// 5. Undo rotate(-angle) → rotate(+angle)
			Vector2Float unrotated = unscaled.rotate(cameraAngle);

			// 6. Undo "screenPosition - halfScreenDim"
			Vector2Float screenPos = unrotated + halfScreenDim;

			return screenPos;
		}
}
