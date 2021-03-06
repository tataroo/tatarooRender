#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "geometry.h"
#include "tgaimage.h"
#include "map"
using namespace  std;
class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
    std::vector<Vec3f> norms_;
    std::vector<Vec2f> uv_;
    static map<const char*, Model*> modelMap;
    void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
    Model(const char *filename);
    Model();
    int nverts() const;
    int nfaces() const;
    Vec3f normal(const int iface, const int nthvert) const;
    Vec3f normal(const Vec2f &uv) const;
    Vec3f vert(const int i) const;
    Vec3f vert(const int iface, const int nthvert) const;
    Vec2f uv(const int iface, const int nthvert) const;
    TGAColor diffuse(const Vec2f &uv) const;
    float specular(const Vec2f &uv) const;
    std::vector<int> face(const int idx) const;
    static void addModel(const char *filename);
    static map<const char*, Model*> getModels();
public:
    TGAImage diffusemap_;
    TGAImage normalmap_;
    TGAImage specularmap_;
};
#endif //__MODEL_H__

