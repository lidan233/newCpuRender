//
// Created by 李源 on 2020-12-12.
//

#ifndef HIERARCHYZBUFFER_HIERACHYZBUFFER_H
#define HIERARCHYZBUFFER_HIERACHYZBUFFER_H

#include "ZBuffer.h"
#include "BoundingBox.h"
#include "Vec.h"
#include "config.h"
#include "ZBuffer.h"

#include <math.h>
#include <assert.h>


class HierachyZBuffer : public Buffer{
private:

    int *begin_height, *begin_width ;
    int minlevel ;
    ZBuffer** buffer = nullptr ;

    bool canRejectBox_level(int level, Vec2f min, Vec2f max, float depth)
    {
        min = Vec::max(min,Vec2f(0.0,0.0)) ;
        max = Vec::max(max,Vec2f(0.0,0.0)) ;
        if(level==0) return false ;

        for(int i = min[0] ; i < max[0] ; i++)
        {
            for(int j = min[1] ; j< max[1]; j++)
            {
                // we can't reject
                if((*(buffer[level]))[i][j] > depth) return false ;
            }
        }
        //we can reject
        return true ;
    }

public:
    HierachyZBuffer(int height,int width)
    {
        int level_height = ceil(log(double(height))/log(2.0)) ;
        int level_width = ceil(log(double(width))/log(2.0)) ;

        minlevel = level_height > level_width? level_width : level_height ;
        buffer = new ZBuffer*[minlevel] ;
        begin_height = new int[minlevel] ;
        begin_width = new int[minlevel] ;

        begin_height[0] = pow(2,level_height) ;
        begin_width[0] = pow(2,level_width) ;

        for(int i = 0 ; i< minlevel ;i++)
        {
            buffer[i] = new ZBuffer(begin_height[i], begin_width[i]) ;
            for(int m = 0 ; m < begin_height[i]; m++)
            {
                for(int n = 0  ; n< begin_width[i] ; n++)
                {
                    (*buffer[i])[m][n] = std::numeric_limits<Doub>::max() ;
                }
            }
            if(i+1<minlevel)
            {
                begin_height[i+1] = begin_height[i]/2.0 ;
                begin_width[i+1] = begin_width[i]/2.0 ;
            }
        }

    }

    void clear()
    {
        for(int i = 0 ; i< minlevel ;i++)
        {
            buffer[i] = new ZBuffer(begin_height[i], begin_width[i]) ;
            for(int m = 0 ; m < begin_height[i]; m++)
            {
                for(int n = 0  ; n< begin_width[i] ; n++)
                {
                    (*buffer[i])[m][n] = std::numeric_limits<Doub>::max() ;
                }
            }
            if(i+1<minlevel)
            {
                begin_height[i+1] = begin_height[i]/2.0 ;
                begin_width[i+1] = begin_width[i]/2.0 ;
            }
        }
    }



    bool canCover(int x ,int y,float depth)
    {
        if((*(buffer[0]))[x][y] <= depth)
        {
            return false;
        }
        return true ;
    }


    void coverReset( int x, int y, int level)
    {
        if(level>=(minlevel-1)) { return; }
        int beginx = floor(x/2.0)*2 ;
        int beginy = floor(y/2.0)*2 ;
        double result = std::numeric_limits<Doub>::min() ;

        if((*(buffer[level]))[beginx][beginy] > result) { result = (*(buffer[level]))[beginx][beginy]; }
        if((*(buffer[level]))[beginx+1][beginy] > result) { result = (*(buffer[level]))[beginx+1][beginy]; }
        if((*(buffer[level]))[beginx][beginy+1] > result) { result = (*(buffer[level]))[beginx][beginy+1]; }
        if((*(buffer[level]))[beginx+1][beginy+1] > result) { result = (*(buffer[level]))[beginx+1][beginy+1]; }


        if(  (((*(buffer[level+1]))[beginx/2][beginy/2])!=result) ) {

#ifdef LOG
            std::cout<<(((*(buffer[level+1]))[beginx/2][beginy/2])!=result)<<std::endl ;
            std::cout<<(((*(buffer[level+1]))[beginx/2][beginy/2])==result)<<std::endl ;
            std::cout<<(*(buffer[level+1]))[beginx/2][beginy/2]<<result<<std::endl ;
#endif

            (*(buffer[level+1]))[beginx/2][beginy/2] = result ;
#ifdef  LOG
            std::cout<<"level "<<level+1<<"change "<<beginx/2<<" "<<beginy/2<<" using depth"<< result <<std::endl ;
#endif
            coverReset( beginx/2, beginy/2 , level+1) ;
        }
    }

    bool cover(int x,int y ,float depth)
    {
        if(canCover(x,y,depth)) {
            (*(buffer[0]))[x][y] = depth;
#ifdef LOG
            std::cout<<"level "<<0<<"change "<<x<<" "<<y<<" using depth"<< depth <<std::endl ;
#endif
            coverReset(x,y,0) ;
            return true ;
        }
        return false ;
    }


    bool canRejectBox(Vec3f pmin_, Vec3f pmax_, float mindepth)
    {
        int count = minlevel-1 ;
        if(pmax_[0]<0 || pmax_[1]<0  || pmin_[0]>begin_height[0] || pmin_[1]>begin_height[1] )
        {
            return true ;
        }

        Vec2f pmin = Vec2f(pmin_[0]/begin_height[0],pmin_[1]/begin_width[0]) ;
        Vec2f pmax = Vec2f(pmax_[0]/begin_height[0],pmax_[1]/begin_width[0]) ;


        for(int i = 0 ; i < minlevel-1 ; i++)
        {
            Vec2f p_min = Vec2f(pmin[0] * begin_height[count] , pmin[1] * begin_width[count] );
            Vec2f p_max = Vec2f(pmax[0] * begin_height[count] , pmax[1] * begin_width[count] );

            // If all buffer value is less than mindepth, so we can't write any value.
            if(canRejectBox_level(count,p_min,p_max,mindepth)==true)
            {
                return true;
            }
            // else we test another level
            count--;
        }
        // we can't reject box
        return false;
    }

};


#endif //HIERARCHYZBUFFER_HIERACHYZBUFFER_H
