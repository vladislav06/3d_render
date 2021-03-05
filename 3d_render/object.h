#pragma once
#include <math.h>
#include <SDL.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <gmtl/gmtl.h>

using namespace::gmtl;
#define PI 3.14159265;
class object
{
public:

    object(std::string fname);
    void move(float x, float y, float z);
    void rotate(float x, float y, float z);
    void draw(SDL_Renderer* _render_);


private:

    struct _point_ {
        float c[3];
    };

    struct _triangle_ {
        gmtl::Vec3f p[3];
        gmtl::Vec3f midpoint;//middle point of triangle
        gmtl::Vec3f normVect; //normal vector of triangle
        float l; //longest side of triangle
        bool rasterize = true;
    };

    gmtl::Vec3f _abs_rot;
  //  _triangle_ _tmp[12];
    _triangle_* _mesh;

    //mesh which we draw
    _triangle_* _draw_mesh;

    //temporary mesh
    _triangle_* _mesh2;

    // _triangle_* _mesh3;
    gmtl::Vec3f _midPoint = gmtl::Vec3f();

    uint32_t _nTri;

    //moving just mesh, not midpoint
    void moveMesh(float x, float y, float z);

    //sorting
    void swap(_triangle_* a, _triangle_* b);
    int partition(_triangle_ arr[], int start, int end);
    void quickSort(_triangle_ arr[], int start, int end);

    //drawing
    void rasterize(SDL_Renderer* _render_, _triangle_ tri);

    gmtl::Vec3f light = gmtl::Vec3f(-5, -5, 0.1);


};

