//
// Created by 李源 on 2019-11-22.
//

#ifndef TINYRENDER_TRIANGLE_H
#define TINYRENDER_TRIANGLE_H

#include "Point.h"
#include "tgaimage.h"
#include "common.h"
#include "ZBuffer.h"

#include <vector>
#include <tuple>

using namespace std ;
class Triangle {
private:
    int  points_i[3] ;
public:
    Triangle(int index[3]);
    void draw_vec3f(TGAImage& image,vector<Vec3f>& points,TGAColor& color ) ;
    void draw(TGAImage& image,Buffer& zbuffer ,vector<Vec3f>& points,vector<Vec2i>& colorsPosition ,float intensity,TGAImage& textImage);
    void draw_vec3i(TGAImage &image, Buffer& zbuffer, vector<Vec3i>& points, vector<Vec2i>& colorsPosition, Vec3f intensity,
                    TGAImage& textImage);
};


#endif //TINYRENDER_TRIANGLE_H
