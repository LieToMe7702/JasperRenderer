#pragma once
#include <string>
#include <vector>

#include "geometry.h"
#include "tgaimage.h"

class Model
{
    std::vector<vec3> verts{};     // array of vertices
    std::vector<vec2> tex_coord{}; // per-vertex array of tex coords
    std::vector<vec3> norms{};     // per-vertex array of normal vectors
    std::vector<int> facet_vrt{};
    std::vector<int> facet_tex{};  // per-triangle indices in the above arrays
    std::vector<int> facet_nrm{};
    TGAImage diffusemap{};
    void load_texture(const std::string filename, const std::string suffix, TGAImage& img);
public:
	Model(const std::string fileName);
    int nverts() const;
    int nfaces() const;
    vec3 vert(const int i) const;
    vec3 vert(const int iface, const int nthvert) const;
    vec2 uv(const int iface, const int nthvert) const;
    const TGAImage& diffuse()  const { return diffusemap; }
};
