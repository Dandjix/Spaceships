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

	//this one doesnt work at all
	struct ExteriorCameraInfo {
	public:
		/// <summary>
		/// camera position in world units
		/// </summary>
		const Vector2Int cameraPosition;

		/**
		 * The center of the spaceship the camera is on
		 */
		const Vector2Int spaceshipPosition;
		const Vector2Int spaceshipCenter;
		const float cameraAngle;
		const float spaceshipAngle;
		const Vector2Int screenDimensions;
		/**
		 * scale of the camera. 1 is regular camera zoom, while 0 would be infinitely zoomed in and inf infinitely zoomed out
		 */
		const float cameraScale;



		[[nodiscard]] Matrix::Mat3 screenToWorldMatrix() const {
			Vector2Float half = Vectors::toVector2Float(screenDimensions) / 2;

			Matrix::Mat3 T_center  = Matrix::Mat3::translation(-half.x, -half.y);
			Matrix::Mat3 R_cam     = Matrix::Mat3::rotation(-cameraAngle);
			Matrix::Mat3 S         = Matrix::Mat3::scale(cameraScale);

			// spaceship transform
			Matrix::Mat3 R_ship    = Matrix::Mat3::rotation(-spaceshipAngle);
			Matrix::Mat3 T_ship    = Matrix::Mat3::translation(spaceshipPosition.x, spaceshipPosition.y);

			Matrix::Mat3 T_cam     = Matrix::Mat3::translation(cameraPosition.x, cameraPosition.y);

			// world = Cam * Ship * Scale * RotCam * Offset
			return T_cam * T_ship * R_ship * S * R_cam * T_center;
		}

		[[nodiscard]] Matrix::Mat3 worldToScreenMatrix() const {
			Vector2Float half = Vectors::toVector2Float(screenDimensions) / 2;

			Matrix::Mat3 T_center  = Matrix::Mat3::translation(half.x, half.y);

			// invert order + invert each
			Matrix::Mat3 R_cam_inv   = Matrix::Mat3::rotation(cameraAngle);
			Matrix::Mat3 S_inv       = Matrix::Mat3::scale(1.0f / cameraScale);
			Matrix::Mat3 R_ship_inv  = Matrix::Mat3::rotation(spaceshipAngle);
			Matrix::Mat3 T_ship_inv  = Matrix::Mat3::translation(-spaceshipPosition.x, -spaceshipPosition.y);
			Matrix::Mat3 T_cam_inv   = Matrix::Mat3::translation(-cameraPosition.x, -cameraPosition.y);

			return T_center * R_cam_inv * S_inv * R_ship_inv * T_ship_inv * T_cam_inv;
		}

		[[nodiscard]] Vector2Int screenToWorldPoint(Vector2Float screen) const {
			auto w = screenToWorldMatrix() * screen;
			return Vectors::toVector2Int(w);
		}

		[[nodiscard]] Vector2Float worldToScreenPoint(Vector2Int world) const {
			return worldToScreenMatrix() * Vectors::toVector2Float(world);
		}

		[[nodiscard]] float getScreenObjectAngle(float world_angle) const {
			return world_angle - cameraAngle + spaceshipAngle;
		}
	};
}
