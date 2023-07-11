// perlin noise header class
// reference: the book on the next weekend
#ifndef PERLIN_H
#define PERLIN_H

#include "Vec3.h"

class Perlin
{
public:
    Perlin()
    {
        random_vec = new Vec3[point_count];
        for (int i = 0; i < point_count; i++)
            random_vec[i] = unit_vector((-1 + (1 - (-1))) * drand48());

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~Perlin()
    {
        delete[] random_vec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double noise(const Vec3 &p) const
    {
        double u = p.x() - floor(p.x());
        double v = p.y() - floor(p.y());
        double w = p.z() - floor(p.z());

        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());

        Vec3 c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = random_vec[
                        perm_x[(i + di) & 255] ^
                        perm_y[(j + dj) & 255] ^
                        perm_z[(k + dk) & 255]];

        return trilinear_interp(c, u, v, w);
    }

    double turb(const Vec3 &p, int depth = 7) const
    {
        double accum = 0;
        Vec3 temp_p = p;
        double weight = 1.0;

        for (int i = 0; i < depth; i++)
        {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return fabs(accum);
    }

private:
    static const int point_count = 256;
    Vec3 *random_vec;
    int *perm_x;
    int *perm_y;
    int *perm_z;

    static int *perlin_generate_perm()
    {
        int *p = new int[point_count];

        for (int i = 0; i < Perlin::point_count; i++)
            p[i] = i;

        permute(p, point_count);

        return p;
    }

    static void permute(int *p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double trilinear_interp(Vec3 c[2][2][2], double u, double v, double w)
    {
        double uu = u * u * (3 - 2 * u);
        double vv = v * v * (3 - 2 * v);
        double ww = w * w * (3 - 2 * w);
        double accum = 0;

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                {
                    Vec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) *
                             (j * vv + (1 - j) * (1 - vv)) *
                             (k * ww + (1 - k) * (1 - ww)) *
                             dot(c[i][j][k], weight_v);
                }

        return accum;
    }
};

#endif