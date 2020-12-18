//
// Created by 李源 on 2019-11-21.
//

#ifndef TINYRENDER_LINE_H
#define TINYRENDER_LINE_H


#include "Point.h"
#include "tgaimage.h"
#include "calculate.h"

#include <math.h>

class Line {
private:
    Point* p_b = nullptr ;
    Point* p_e = nullptr ;

public:
    Line(Point* pb,Point*  pe ) {
        if(pb->getX()>pe->getX())
        {
            p_b = pe ;
            p_e = pb ;
        }else{
            p_b = pb ;
            p_e = pe ;
        }
    }
    void drawLine_cal_much(TGAImage &image, TGAColor color) ;
    void drawLine_with_bilv(TGAImage &image,TGAColor color) ;
    void drawLine_change_all(TGAImage &image,TGAColor color) ;
    void drawLine_remove_float_plus(TGAImage &image,TGAColor color) ;
};


#endif //TINYRENDER_LINE_H
