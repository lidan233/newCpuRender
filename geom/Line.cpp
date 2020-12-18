//
// Created by 李源 on 2019-11-21.
//

#include "Line.h"
void Line::drawLine_cal_much(TGAImage &image, TGAColor color)
{
    // this cal much because this althgrim contains * and /
    double k = (p_e->getY()-p_b->getY())/(p_e->getX()-p_b->getX()) ;
    for(int i = p_b->getX();i<=p_e->getX();i++ )
    {
        image.set(i,(int)(i*k)+p_b->getY(),color) ;
    }
}

void Line::drawLine_with_bilv(TGAImage &image,TGAColor color)
{
    for(int i = p_b->getX();i<=p_e->getX();i++)
    {
        double bilv = (i-p_b->getX())/(p_e->getX()-p_b->getX()) ;
        image.set(i,p_e->getY()*bilv+(1-bilv)*p_b->getY(),color) ;
    }
}

void Line::drawLine_change_all(TGAImage &image,TGAColor color)
{
    bool steep = false ;
    double x0 =  p_b->getX() ;
    double x1 =  p_e->getX() ;
    double y0 =  p_b->getY() ;
    double y1 =  p_e->getY() ;


    if(abs(p_b->getX()-p_e->getX())<abs(p_b->getY()-p_e->getY()))
    {
        std::swap(x0,y0) ;
        std::swap(x1,y1) ;
        steep = true ;
    }

    if(x0>x1)
    {
        swap(x0,x1) ;
        swap(y0,y1) ;
    }

    double dy = y1-y0 ;
    double dx = x1-x0 ;
    double derror = abs(dy/dx)  ;
    double error =  0 ;
    int y = y0 ;

    for(int i = x0;i<=x1 ;i++)
    {
        if(steep)
        {
            image.set(y,i,color) ;
        }else
        {
            image.set(i,y,color) ;
        }
        error += derror ;

        if(error>0.5)
        {
            y+=y1>y0?1:-1 ;
            error -= 1 ;
        }
    }
}

void Line::drawLine_remove_float_plus(TGAImage &image,TGAColor color)
{
    bool steep = false ;
    int x0 = p_b->getX() ;
    int y0 = p_b->getY() ;
    int x1 = p_e->getX() ;
    int y1 = p_e->getY() ;

    if(abs(x0-x1)<abs(y0-y1))
    {
        steep = true ;
        swap(x0,y0) ;
        swap(x1,y1) ;
    }

    if(x0>x1)
    {
        swap(x0,x1) ;
        swap(y0,y1) ;
    }

    int dy = y1-y0 ;
    int dx = x1-x0 ;
    int derror2 = abs(dy)*2 ;
    int error2 =  0 ;
    int y = y0 ;

    for(int i = x0; i < x1;i++ )
    {
        if(steep)
        {
            image.set(y,i,color) ;
        }else
        {
            image.set(i,y,color) ;
        }
        error2+=derror2 ;//error2*dx = 2k*dx  ; 两边都扩大
        if(error2>dx)
        {
            y += (y1>y0)?1:-1 ;
            error2 -= dx*2 ;
        }
    }

}

