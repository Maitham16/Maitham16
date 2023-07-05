// camera header class
#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Ray.h"
#include <cmath>

class Camera
{
public:
    Vector3 position;
    Vector3 lookAt;
    Vector3 up;
    double fov;
    double aperture;
    double focusDistance;
    int imgWidth;
    int imgHeight;
    int maxBounce;
    Matrix4 transform;
    bool isTransform;
    bool dof;

    // Constructor
    Camera()
    {
        position = Vector3(0, 0, 0);
        lookAt = Vector3(0, 0, 1);
        up = Vector3(0, 1, 0);
        fov = 60;
        aperture = 0;
        focusDistance = 1;
        imgWidth = 640;
        imgHeight = 480;
        maxBounce = 5;
        isTransform = false;
        dof = false;
    }

    //  Constructor with parameters
    Camera(Vector3 position, Vector3 lookAt, Vector3 up, double fov, double aperture, double focusDistance, int imgWidth, int imgHeight, int maxBounce, bool isTransform, bool dof)
    {
        this->position = position;
        this->lookAt = lookAt;
        this->up = up;
        this->fov = fov;
        this->aperture = aperture;
        this->focusDistance = focusDistance;
        this->imgWidth = imgWidth;
        this->imgHeight = imgHeight;
        this->maxBounce = maxBounce;
        this->isTransform = isTransform;
        this->dof = dof;
    }

    // Generate ray function
    Ray generateRay(double u, double v) const
    {
        Vector3 lensPosition;

        // Check if depth of field is enabled (filter of camera lens)
        if (dof)
        {
            double lensRadius = aperture / 1.0;
            double r1 = drand48(), r2 = drand48();
            double lensX = lensRadius * sqrt(r1) * cos(2 * M_PI * r2);
            double lensY = lensRadius * sqrt(r1) * sin(2 * M_PI * r2);
            lensPosition = Vector3(lensX, lensY, 0);

            // Check if transformation is enabled
            if (isTransform)
            {
                // parameters for transformation
                Vector3 forward = (this->lookAt - this->position).normalize();
                Vector3 right = forward.cross(this->up).normalize();
                Vector3 upNew = right.cross(forward).normalize();

                // Compute transformation matrix (translation and rotation)
                Matrix4 translationMatrix;
                translationMatrix.makeTranslation(-position.x, -position.y, -position.z);

                Matrix4 rotationMatrix;
                rotationMatrix.makeRotationFromBasis(right, upNew, -forward);

                // Generate ray in camera space
                double theta = fov;
                double halfWidth = std::tan(theta / 2.0);
                double halfHeight = halfWidth * (imgHeight / static_cast<double>(imgWidth));
                Vector3 rayOriginCameraSpace = Vector3(0, 0, 0);
                Vector3 rayDirectionCameraSpace = Vector3(2.0 * halfWidth * (u - 0.5), 2.0 * halfHeight * (v - 0.5), -1.0).normalize();

                // Transform to world space
                Vector4 rayOriginWorldSpace = transform * Vector4(rayOriginCameraSpace, 1.0);
                Vector4 rayDirectionWorldSpace = transform * Vector4(rayDirectionCameraSpace, 0.0);

                Vector3 offset = lensPosition * focusDistance;
                Vector3 target = Vector3(rayDirectionWorldSpace.x, rayDirectionWorldSpace.y, rayDirectionWorldSpace.z) * focusDistance;
                return Ray(Vector3(rayOriginWorldSpace.x, rayOriginWorldSpace.y, rayOriginWorldSpace.z) + offset, (target - offset).normalize());
            }
            else
            {
                // parameters for the direction vector based on the camera's position, lookAt, and up vectors
                Vector3 direction = (lookAt - position).normalize();

                // parameter of the right vector
                Vector3 right = direction.cross(up).normalize();

                // parameter of the up vector
                Vector3 camUp = right.cross(direction).normalize();

                // parameters of the field of view half-angles
                double theta = fov;
                double halfWidth = std::tan(theta / 2.0);
                double halfHeight = halfWidth * (imgHeight / static_cast<double>(imgWidth));

                // Calculate the ray direction
                Vector3 rayDirection = (direction + (right * (2.0 * halfWidth * (u - 0.5))) + (camUp * (2.0 * halfHeight * (v - 0.5)))).normalize();

                Vector3 offset = lensPosition * focusDistance;
                Vector3 target = rayDirection * focusDistance;
                return Ray(position + offset, (target - offset).normalize());
            }
        }

        // Check if transformation is enabled without camera lens
        if (isTransform)
        {
            // parameters for transformation
            Vector3 forward = (this->lookAt - this->position).normalize();
            Vector3 right = forward.cross(this->up).normalize();
            Vector3 upNew = right.cross(forward).normalize();

            // Compute transformation matrix (translation and rotation)
            Matrix4 translationMatrix;
            translationMatrix.makeTranslation(-position.x, -position.y, -position.z);

            Matrix4 rotationMatrix;
            rotationMatrix.makeRotationFromBasis(right, upNew, -forward);

            // Generate ray in camera space
            double theta = fov;
            double halfWidth = std::tan(theta / 1.0);
            double halfHeight = halfWidth * (imgHeight / static_cast<double>(imgWidth));
            Vector3 rayOriginCameraSpace = Vector3(0, 0, 0);
            Vector3 rayDirectionCameraSpace = Vector3(2.0 * halfWidth * (u - 0.5), 2.0 * halfHeight * (v - 0.5), -1.0).normalize();

            // Transform to world space
            Vector4 rayOriginWorldSpace = transform * Vector4(rayOriginCameraSpace, 1.0);
            Vector4 rayDirectionWorldSpace = transform * Vector4(rayDirectionCameraSpace, 0.0);

            // Create and return the ray
            return Ray(Vector3(rayOriginWorldSpace.x, rayOriginWorldSpace.y, rayOriginWorldSpace.z), Vector3(rayDirectionWorldSpace.x, rayDirectionWorldSpace.y, rayDirectionWorldSpace.z).normalize());
        }
        else
        {

            //  parameters for the direction vector based on the camera's position, lookAt, and up vectors
            Vector3 direction = (lookAt - position).normalize();

            //  parameter of the right vector
            Vector3 right = direction.cross(up).normalize();

            //  parameter of the up vector
            Vector3 camUp = right.cross(direction).normalize();

            //  parameters of the field of view half-angles
            double theta = fov;
            double halfWidth = std::tan(theta / 1.0);
            double halfHeight = halfWidth * (imgHeight / static_cast<double>(imgWidth));

            //  Calculate the ray direction
            Vector3 rayDirection = (direction + (right * (2.0 * halfWidth * (u - 0.5))) + (camUp * (2.0 * halfHeight * (v - 0.5)))).normalize();

            // Create and return the ray
            return Ray(position, rayDirection);
        }
    }
};

#endif
