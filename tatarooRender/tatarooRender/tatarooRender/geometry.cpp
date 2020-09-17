#include "geometry.h"

template <> template <> vec<3,int>  ::vec(const vec<3,float> &v) : x(int(v.x+.5f)),y(int(v.y+.5f)),z(int(v.z+.5f)) {}
template <> template <> vec<3,float>::vec(const vec<3,int> &v)   : x(v.x),y(v.y),z(v.z) {}
template <> template <> vec<2,int>  ::vec(const vec<2,float> &v) : x(int(v.x+.5f)),y(int(v.y+.5f)) {}
template <> template <> vec<2,float>::vec(const vec<2,int> &v)   : x(v.x),y(v.y) {}

mat<4, 1, float> v2m(Vec3f v)
{
    mat<4, 1, float> temp;
    temp[0][0] = v.x;
    temp[1][0] = v.y;
    temp[2][0] = v.z;
    temp[3][0] = 1;
    return temp;
}

Vec3f m2v(mat<4, 1, float> v){
    Vec3f k(v[0][0] / v[3][0], v[1][0] / v[3][0], v[2][0] / v[3][0]);
    return k;
}

Vec3f m2v(Vec4f v){
    Vec3f k(v[0] / v[3], v[1] / v[3], v[2] / v[3]);
    return k;
}
