#pragma once
#include "../math/Vectors.h"

namespace CameraTransformations {

	struct CameraInfo {
	public:
		/// <summary>
		/// camera position in world units
		/// </summary>
		const Vector2Int cameraPosition;
		const float cameraAngle;
		const Vector2Int screenDimensions;
		/**
		 * scale of the camera. 1 is regular camera zoom, while 0 would be infinitely zoomed in and inf infinitely zoomed out
		 */
		const float cameraScale;

		/// <summary>
		/// works all the time 50% of the time
		/// </summary>
		/// <param name="screenPosition"></param>
		/// <param name="screenDimensions"></param>
		/// <param name="cameraAngle"></param>
		/// <param name="cameraScale"></param>
		/// <param name="cameraPosition"></param>
		/// <returns></returns>
		[[nodiscard]] Vector2Int screenToWorldPoint(Vector2Float screenPosition) const {

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
		[[nodiscard]] Vector2Float worldToScreenPoint(Vector2Int worldPosition) const
		{
				// 1. Get half screen dim
				Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2.0f;

				// 2. Undo "+ cameraPosition"
				Vector2Float local = Vectors::toVector2Float(worldPosition - cameraPosition);

				// 3. Undo scaleToWorldPosition() -> return to the "scaled" grid space
				Vector2Float grid = local.scaleToScreenPosition();

				// 4. Undo "* cameraScale"
				Vector2Float unscaled = grid / cameraScale;

				// 5. Undo rotate(-angle) → rotate(+angle)
				Vector2Float unrotated = unscaled.rotate(cameraAngle);

				// 6. Undo "screenPosition - halfScreenDim"
				Vector2Float screenPosition = unrotated + halfScreenDim;

				return screenPosition;
		}
	};
}
