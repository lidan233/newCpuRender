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
#include <io.h>
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

    // help rasterizer
    float z ;
    float dzx ;
    float dzy ;
    int id ; // 交点对所在的多边形的编号
};

// 活化和非活化的polygonNode数据结构相同，所以这里合成为一个(我这里仅仅支持三角形Polygon)
struct Poly_TriNode{
    float a,b,c,d ;
    int pid ;
    int linesize  ;
    std::vector< ActiveEdgeNode> activeEdgeTable;
};


class ScanlineZbuffer {
private:
    Lmatrix<Doub> szbuffer ;
    int height = 0 , width = 0 ;
    std::vector<std::list<Poly_TriNode>> polygonTable; // Poly 表
    std::vector<std::list<EdgeNode>> edgeTable; // edge table
    std::vector<Poly_TriNode> activePolygonTable; //两个活化表

    int** idBuffer ;
    bool needUpdate ;

public:
    ScanlineZbuffer(int H, int W, ObjData& objData) ;
    void addEdge(int y, Poly_TriNode* active_polygon) ;
    void run(ObjData& objData) ;

};


#endif //NEWCPURENDER_SCANLINEZBUFFER_H
