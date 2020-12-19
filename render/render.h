//
// Created by lidan on 18/12/2020.
//

#ifndef NEWCPURENDER_RENDER_H
#define NEWCPURENDER_RENDER_H

#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

#include "Vec.h"
#include "common.h"
#include "tgaimage.h"
#include "manipulation.h"
#include "ObjLoader.h"

using namespace std;

class Render {

private:
    //normals of vertex
    ObjData objData ;
    std::vector<Vec3i> normal_v;
    TGAImage text ;

    bool isnormal = false  ;
    bool useTexture = false ;

//    ZBuffer* zBuffer ;
    Buffer* zBuffer ;

    int T_width ;
    int T_height ;

    Vec3f camera ;


public:
    Render(const string path) ;
    Render(vector<Vec3f> points,vector< vector<int> > faces) ;
    ~Render() ;
    int getVertexSize() ;
    int getFacesSize() ;
    Vec3f getVertex(int n) ;
    Vec3i getFace(int idx) ;
    Vec3i getFaceText(int idx) ;
    Vec3i getFaceVertexNormIndex(int idx);
    void normal(int height,int width,Vec3f center,Vec3f eye,Vec3f up);
    void drawAll(TGAImage& image ,vector<TGAColor> v_c,vector<TGAColor> f_c,Vec3f light_dir) ;
    void addTexture(string path) ;
    TGAColor getTextureColor(Vec2i point ) ;
    vector<Vec2i> getFaceVertexTexturePosition(int face_i ) ;
    vector<Vec3f> getFaceVectexNorm(int face_i) ;
    void setcamera(Vec3f camera);



};


#endif //NEWCPURENDER_RENDER_H
