//
// Created by lidan on 2021/1/9.
//

#ifndef NEWCPURENDER_SCANLINEZBUFFER_H
#define NEWCPURENDER_SCANLINEZBUFFER_H
#include "data.h"
#include "ObjLoader.h"
#include "common.h"

#include <list>
#include <algorithm>

struct EdgeNode{
    float left_x ;
    float dx ;
    int dy ;
    int poly_id ;
};

struct ActiveEdgeNode{
    // help iterator
    float lx ;
    float ldx ; // the interect point of this edge
    int ldy ; // but the init value is the index of Scanline index.

//    float rx ;
//    float rdx ;
//    int rdy ;

    float z ;
    float dzx ;
    float dzy ;
    int id ; // 交点对所在的多边形的编号
};

struct Poly_TriNode{
    float a,b,c,d ;
    int pid ;
    int linesize  ;
    Vec3f norm ;
    std::vector< ActiveEdgeNode> activeEdgeTable;
};


class ScanlineZbuffer {
private:
    Lmatrix<Doub> szbuffer ;
    Lmatrix<Int> idBuffer ;
    int height = 0 , width = 0 ;
    std::vector<std::list<Poly_TriNode>> polygonTable;
    std::vector<std::list<EdgeNode>> edgeTable; // edge table
    std::vector<Poly_TriNode> activePolygonTable;

public:
    ScanlineZbuffer(int H, int W) ;
    void addEdge(int y, Poly_TriNode* active_polygon) ;
    void run(ObjData& objData,Vec3f LightView, TGAImage& image) ;
    void build(ObjData& objData) ;
    void release() ;

};


#endif //NEWCPURENDER_SCANLINEZBUFFER_H
