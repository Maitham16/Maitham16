// Matrix4 header class
#ifndef MATRIX4_H
#define MATRIX4_H

#include <array>
#include <cmath>

#include "Vector4.h"

class Matrix4
{
public:
    std::array<std::array<float, 4>, 4> mat;

    Matrix4()
    {
        identity();
    }

    // this function from chat gpt to make the matrix identity without assigning each element
    void identity()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    static Matrix4 translate(float x, float y, float z)
    {
        Matrix4 result;
        result.mat[0][3] = x;
        result.mat[1][3] = y;
        result.mat[2][3] = z;
        return result;
    }

    static Matrix4 scale(float x, float y, float z)
    {
        Matrix4 result;
        result.mat[0][0] = x;
        result.mat[1][1] = y;
        result.mat[2][2] = z;
        return result;
    }

    static Matrix4 rotate(float angle, float x, float y, float z)
    {
        float s = std::sin(angle);
        float c = std::cos(angle);
        float oc = 1.0 - c;

        Matrix4 result;
        result.mat[0][0] = x * oc + c;
        result.mat[0][1] = y * x * oc + z * s;
        result.mat[0][2] = x * z * oc - y * s;
        result.mat[1][0] = x * y * oc - z * s;
        result.mat[1][1] = y * oc + c;
        result.mat[1][2] = y * z * oc + x * s;
        result.mat[2][0] = x * z * oc + y * s;
        result.mat[2][1] = y * z * oc - x * s;
        result.mat[2][2] = z * oc + c;
        return result;
    }

    Matrix4 operator*(const Matrix4 &rhs) const
    {
        Matrix4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                result.mat[i][j] = 0;
                for (int k = 0; k < 4; k++)
                    result.mat[i][j] += this->mat[i][k] * rhs.mat[k][j];
            }
        return result;
    }

    void makeTranslation(double x, double y, double z)
    {
        identity();

        mat[0][3] = x;
        mat[1][3] = y;
        mat[2][3] = z;
    }

    void makeRotationFromBasis(const Vector3 &right, const Vector3 &up, const Vector3 &forward)
    {
        mat[0][0] = right.x;
        mat[1][0] = right.y;
        mat[2][0] = right.z;
        mat[3][0] = 0.0;

        mat[0][1] = up.x;
        mat[1][1] = up.y;
        mat[2][1] = up.z;
        mat[3][1] = 0.0;

        mat[0][2] = -forward.x;
        mat[1][2] = -forward.y;
        mat[2][2] = -forward.z;
        mat[3][2] = 0.0;

        mat[0][3] = 0.0;
        mat[1][3] = 0.0;
        mat[2][3] = 0.0;
        mat[3][3] = 1.0;
    }

    void makePerspective(double fovY, double aspectRatio, double nearClip, double farClip)
    {
        double theta = fovY * 0.5;
        double range = farClip - nearClip;
        double invtan = 1. / tan(theta);

        identity();

        mat[0][0] = invtan / aspectRatio;
        mat[1][1] = invtan;
        mat[2][2] = -(nearClip + farClip) / range;
        mat[2][3] = -1;
        mat[3][2] = -2 * nearClip * farClip / range;
        mat[3][3] = 0;
    }

    Vector4 operator*(const Vector4 &v) const
    {
        Vector4 result;
        result.x = mat[0][0] * v.x + mat[0][1] * v.y + mat[0][2] * v.z + mat[0][3] * v.w;
        result.y = mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z + mat[1][3] * v.w;
        result.z = mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z + mat[2][3] * v.w;
        result.w = mat[3][0] * v.x + mat[3][1] * v.y + mat[3][2] * v.z + mat[3][3] * v.w;
        return result;
    }

    // operand types are: const Matrix4 * Vector3
    Vector3 operator*(const Vector3 &v) const
    {
        Vector3 result;
        result.y = mat[1][0] * v.x + mat[1][1] * v.y + mat[1][2] * v.z;
        result.z = mat[2][0] * v.x + mat[2][1] * v.y + mat[2][2] * v.z;
        return result;
    }
};

#endif
