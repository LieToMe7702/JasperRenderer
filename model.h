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
    TGAImage normalmap{};          // normal map texture
    TGAImage normal_tangent_map{}; // normal map texture in tangent space
    TGAImage specularmap{};        // specular map texture
    void load_texture(const std::string filename, const std::string suffix, TGAImage& img);
public:
	Model(const std::string fileName);
    int nverts() const;
    int nfaces() const;
    vec3 vert(const int i) const;
    vec3 vert(const int iface, const int nthvert) const;
    vec2 uv(const int iface, const int nthvert) const;
    vec3 normal(const int iface, const int nthvert) const;
    vec3 normal(const vec2& uv) const;                     // fetch the normal vector from the normal map texture
    vec3 normal_tangent(const vec2& uv) const;
    const TGAImage& diffuse()  const { return diffusemap; }
    const TGAImage& specular() const { return specularmap; }
    const TGAColor& diffuse(double u, double v);
    const TGAColor& spec(double u, double v);

};
