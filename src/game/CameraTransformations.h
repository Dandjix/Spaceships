#pragma once
#include "../math/Vectors.h"
#include "math/Matrix.h"

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

    private:
        Matrix::Mat3<> screenToWorldMatrix() const {
            Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2.0f;

            // Build transformation matrix (applied right to left):
            // 5. Translate by camera position
            Matrix::Mat3 translateCamera = Matrix::Mat3<>::translation(
                cameraPosition.x, cameraPosition.y
            );

            // 4. Scale to world position
            // Based on your commented code: scaleToWorldPosition()
            // You'll need to determine this scale factor from your Vector2Int/Float classes
            // For now, assuming it's defined somewhere as a constant
            Matrix::Mat3 scaleToWorld = Matrix::Mat3<>::scale(Vectors::getFactor());

            // 3. Scale by camera scale
            Matrix::Mat3 scaleCam = Matrix::Mat3<>::scale(cameraScale);

            // 2. Rotate by -cameraAngle
            Matrix::Mat3 rotate = Matrix::Mat3<>::rotation(-cameraAngle);

            // 1. Center the screen (translate by -halfScreen)
            Matrix::Mat3 centerScreen = Matrix::Mat3<>::translation(
                -halfScreenDim.x, -halfScreenDim.y
            );

            // Combine: applied right to left
            return translateCamera * scaleToWorld * scaleCam * rotate * centerScreen;
        }

        Matrix::Mat3<> worldToScreenMatrix() const {
            Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2.0f;

            // Inverse operations in reverse order:
            // 1. Translate by -camera position
            Matrix::Mat3 translateCamera = Matrix::Mat3<>::translation(
                -cameraPosition.x, -cameraPosition.y
            );

            // 2. Scale to screen position (inverse of scaleToWorld)
            Matrix::Mat3 scaleToScreen = Matrix::Mat3<>::scale(1.0f / Vectors::getFactor());

            // 3. Unscale by camera scale
            Matrix::Mat3 unscaleCam = Matrix::Mat3<>::scale(1.0f / cameraScale);

            // 4. Rotate by +cameraAngle (inverse rotation)
            Matrix::Mat3 rotate = Matrix::Mat3<>::rotation(cameraAngle);

            // 5. Uncenter the screen (translate by +halfScreen)
            Matrix::Mat3 uncenterScreen = Matrix::Mat3<>::translation(
                halfScreenDim.x, halfScreenDim.y
            );

            // Combine: applied right to left
            return uncenterScreen * rotate * unscaleCam * scaleToScreen * translateCamera;
        }

    public:
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
            auto w = screenToWorldMatrix() * screenPosition;
            return Vectors::toVector2Int(w);

            // Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2;
            // Vector2Float offsetScreenPosition = screenPosition - halfScreenDim;
            //
            // Vector2Float rotated = offsetScreenPosition.rotate(-cameraAngle);
            // Vector2Float scaled = (rotated) * cameraScale;
            //
            // Vector2Int worldPoint = Vectors::toVector2Int(scaled).scaleToWorldPosition() + cameraPosition;
            //
            // return worldPoint;
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
        [[nodiscard]] Vector2Float worldToScreenPoint(Vector2Int worldPosition) const {
            auto w = worldToScreenMatrix() * Vectors::toVector2Float(worldPosition);
            return w;
            //
            // // 1. Get half screen dim
            // Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2.0f;
            //
            // // 2. Undo "+ cameraPosition"
            // Vector2Float local = Vectors::toVector2Float(worldPosition - cameraPosition);
            //
            // // 3. Undo scaleToWorldPosition() -> return to the "scaled" grid space
            // Vector2Float grid = local.scaleToScreenPosition();
            //
            // // 4. Undo "* cameraScale"
            // Vector2Float unscaled = grid / cameraScale;
            //
            // // 5. Undo rotate(-angle) → rotate(+angle)
            // Vector2Float unrotated = unscaled.rotate(cameraAngle);
            //
            // // 6. Undo "screenPosition - halfScreenDim"
            // Vector2Float screenPosition = unrotated + halfScreenDim;
            //
            // return screenPosition;
        }
    };

    //this one doesnt work at all
    struct ExteriorCameraInfo {
    public:
        /// <summary>
        /// camera position in world units (relative to ship top left)
        /// </summary>
        const Vector2Int cameraPosition;

        /**
         * The top left of the spaceship the camera is on
         */
        const Vector2Int spaceshipPosition;
        /**
         * The center of the spaceship the camera is on
         */
        const Vector2Int spaceshipCenter;
        const float cameraAngle;
        const float spaceshipAngle;
        const Vector2Int screenDimensions;
        /**
         * scale of the camera. 1 is regular camera zoom, while 0 would be infinitely zoomed in and inf infinitely zoomed out
         */
        const float cameraScale;

        [[nodiscard]] Vector2Int screenToWorldPoint(Vector2Float screen) const {
            Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2.0f;

            // Transformation chain (applied right to left):
            // 1. Center screen coordinates
            Matrix::Mat3 centerScreen = Matrix::Mat3<>::translation(-halfScreenDim.x, -halfScreenDim.y);

            // 2. Rotate by -cameraAngle (undo camera rotation)
            Matrix::Mat3 rotateCam = Matrix::Mat3<>::rotation(-cameraAngle);

            // 3. Scale by cameraScale (undo zoom)
            Matrix::Mat3 scaleCam = Matrix::Mat3<>::scale(cameraScale);

            // 4. Scale to world position
            Matrix::Mat3 scaleToWorld = Matrix::Mat3<>::scale(Vectors::getFactor());

            // 5. Translate by camera position (relative to ship)
            Matrix::Mat3 translateCam = Matrix::Mat3<>::translation(
                cameraPosition.x, cameraPosition.y
            );

            // 6. Rotate by spaceship angle (ship's local to world rotation)
            Matrix::Mat3 rotateShip = Matrix::Mat3<>::rotation(spaceshipAngle);

            // 7. Translate by spaceship position (ship local to world position)
            Matrix::Mat3 translateShip = Matrix::Mat3<>::translation(
                spaceshipPosition.x, spaceshipPosition.y
            );

            Matrix::Mat3 transform = translateShip * rotateShip * translateCam * scaleToWorld * scaleCam * rotateCam *
                                     centerScreen;

            Vector2Float worldFloat = transform * screen;
            return Vectors::toVector2Int(worldFloat);
        }

        [[nodiscard]] Vector2Float worldToScreenPoint(Vector2Int world) const {
            Vector2Float halfScreenDim = Vectors::toVector2Float(screenDimensions) / 2.0f;

            // Inverse operations in reverse order:
            // 1. Translate by -spaceship position
            Matrix::Mat3 translateShip = Matrix::Mat3<>::translation(
                -spaceshipPosition.x, -spaceshipPosition.y
            );

            // 2. Rotate by -spaceship angle
            Matrix::Mat3 rotateShip = Matrix::Mat3<>::rotation(-spaceshipAngle);

            // 3. Translate by -camera position
            Matrix::Mat3 translateCam = Matrix::Mat3<>::translation(
                -cameraPosition.x, -cameraPosition.y
            );

            // 4. Scale to screen position (inverse of scaleToWorld)
            Matrix::Mat3 scaleToScreen = Matrix::Mat3<>::scale(1.0f / Vectors::getFactor());

            // 5. Unscale by camera scale
            Matrix::Mat3 unscaleCam = Matrix::Mat3<>::scale(1.0f / cameraScale);

            // 6. Rotate by +cameraAngle
            Matrix::Mat3 rotateCam = Matrix::Mat3<>::rotation(cameraAngle);

            // 7. Uncenter screen
            Matrix::Mat3 uncenterScreen = Matrix::Mat3<>::translation(
                halfScreenDim.x, halfScreenDim.y
            );

            Matrix::Mat3 transform = uncenterScreen * rotateCam * unscaleCam * scaleToScreen * translateCam * rotateShip
                                     * translateShip;

            return Vectors::toVector2Float(transform * world);
        }

        [[nodiscard]] float getScreenObjectAngle(float world_angle) const {
            // An object in world space needs to:
            // 1. Have spaceship rotation subtracted (since camera rotates with ship)
            // 2. Have camera rotation added (since camera can rotate relative to ship)
            return world_angle - spaceshipAngle + cameraAngle;
        }
    };
}
