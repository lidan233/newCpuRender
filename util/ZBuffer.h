//
// Created by 李源 on 2020-12-12.
//

#ifndef HIERARCHYZBUFFER_ZBUFFER_H
#define HIERARCHYZBUFFER_ZBUFFER_H

#include <data.h>
 class Buffer{
 public:
    virtual double* operator[](int H) { return 0 ;} ;
    virtual bool canCover(int H,int W,float depth) { return false; } ;
};

class ZBuffer: public Buffer {
private:
    Lmatrix<Doub> zbuffer ;

public:
    ZBuffer(int H, int W)
    {
        zbuffer = Lmatrix<Doub>(H,W);
        for(int i = 0 ; i < H ; i++)
        {
            for(int j = 0 ; j < W ; j++)
            {
                zbuffer[i][j] = std::numeric_limits<Doub>::min() ;
            }
        }
    }

    double* operator[](int H) { return zbuffer[H] ; }
    bool canCover(int H,int W,float depth) { if(depth<zbuffer[H][W]) return true ; }
};


#endif //HIERARCHYZBUFFER_ZBUFFER_H
