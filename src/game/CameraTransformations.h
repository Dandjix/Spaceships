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
	/// sometimes this one does the trick kill me
	/// </summary>
	/// <param name="screenPosition"></param>
	/// <param name="screenDimensions"></param>
	/// <param name="cameraAngle"></param>
	/// <param name="cameraScale"></param>
	/// <param name="cameraPosition"></param>
	/// <returns></returns>
	inline Vector2Int screenToWorldPointPerfectInverse(Vector2Float screenPosition, Vector2Int screenDimensions, float cameraAngle, float cameraScale, Vector2Int cameraPosition) {

		Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2;
		Vector2Float offsetScreenPosition = screenPosition - halfScreenDim;

		Vector2Float rotated = offsetScreenPosition.rotate(-cameraAngle);
		Vector2Float scaled = (rotated + halfScreenDim)*cameraScale;

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
	inline Vector2Float worldToScreenPoint(Vector2Int worldPosition, Vector2Int screenDimensions, float cameraAngle, float cameraScale, Vector2Int cameraPos) {
		Vector2Float floatPosition = Vectors::toVector2Float(worldPosition);

		Vector2Float floatCameraPosition = Vectors::toVector2Float(cameraPos);
		Vector2Float worldCenter = floatPosition - floatCameraPosition;
		Vector2Float scaledWorldCenter = ((worldCenter) / cameraScale).scaleToScreenPosition();

		Vector2Float screenCenter = Vectors::toVector2Float(screenDimensions) / 2;
		Vector2Float diff = (screenCenter - scaledWorldCenter).rotate(cameraAngle);
		scaledWorldCenter = screenCenter - diff;

		return scaledWorldCenter;
	}
}
