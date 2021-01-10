//
// Created by lidan on 20/12/2020.
//

#ifndef NEWCPURENDER_PIPLINE_H
#define NEWCPURENDER_PIPLINE_H

#include "Model.h"
#include "View.h"
#include "Project.h"
#include "ViewPort.h"
#include "Vec.h"
#include "config.h"

#include <vector>
#include <limits>
#include <omp.h>


class PipLine{

private:

    Matrix44 model_ ;
    Matrix44 view_ ;
    Matrix44 project_ ;
    Matrix44 viewport_ ;
    Matrix44 result ;

public:
    PipLine( Model& model, View& view, Project& project, ViewPort& viewPort)
    {
        model_ = model.getMatrix() ;
        view_ = view.getMatrix() ;
        project_ = project.getMatrix() ;
        viewport_ = viewPort.getMatrix() ;
        result = viewport_*project_*view_*model_ ;
    }



    BoundingBox* change(BoundingBox& boundingBox)
    {
        Vec3f r_min = Vec3f(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
        Vec3f r_max = Vec3f(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());

        Vec3f min = boundingBox.getPmin() ;
        Vec3f max = boundingBox.getPmax() ;

        Matrix41 t1 = manipulation::v2m(min) ;
        Vec3f LLL = manipulation::m2v(result*t1);
        r_min = Vec::min(r_min,LLL) ;
        r_max = Vec::max(r_max,LLL) ;


        Matrix41 t2 = manipulation::v2m(Vec3f(min[0],min[1],max[2])) ;
        Vec3f LLR = manipulation::m2v(result*t2);
        r_min = Vec::min(r_min,LLR) ;
        r_max = Vec::max(r_max,LLR) ;

        Matrix41 t3 = manipulation::v2m(Vec3f(min[0],max[1],min[2])) ;
        Vec3f LRL = manipulation::m2v(result*t3) ;
        r_min = Vec::min(r_min,LRL) ;
        r_max = Vec::max(r_max,LRL) ;

        Matrix41 t4 = manipulation::v2m(Vec3f(max[0],min[1],max[2])) ;
        Vec3f RLR = manipulation::m2v(result*t4) ;
        r_min = Vec::min(r_min,RLR) ;
        r_max = Vec::max(r_max,RLR) ;

        Matrix41 t5 = manipulation::v2m(Vec3f(max[0],min[1],min[2])) ;
        Vec3f RLL = manipulation::m2v(result*t5) ;
        r_min = Vec::min(r_min,RLL) ;
        r_max = Vec::max(r_max,RLL) ;

        Matrix41 t6 = manipulation::v2m(Vec3f(min[0],max[1],max[2])) ;
        Vec3f LRR = manipulation::m2v(result*t6) ;
        r_min = Vec::min(r_min,LRR) ;
        r_max = Vec::max(r_max,LRR) ;

        Matrix41 t7 = manipulation::v2m(Vec3f(min[0],max[1],max[2])) ;
        Vec3f RRL = manipulation::m2v(result*t7) ;
        r_min = Vec::min(r_min,RRL) ;
        r_max = Vec::max(r_max,RRL) ;

        Matrix41 t8 = manipulation::v2m(Vec3f(max)) ;
        Vec3f RRR = manipulation::m2v(result*t8) ;
        r_min = Vec::min(r_min,RRR) ;
        r_max = Vec::max(r_max,RRR) ;

        BoundingBox* result = new BoundingBox(r_min, r_max) ;
        return result ;
    }

    void change(ObjData& objData)
    {

//        Matrix44 result = project_*view_*model_ ;
//#ifndef NOLOG
//        std::cout<<"model "<<model_ ;
//        std::cout<<"view "<<view_ ;
//        std::cout<<"project "<<project_ ;
//        std::cout<<"project "<<viewport_ ;
//        std::cout<<"result "<<result ;
//#endif

        omp_set_num_threads(processor_N);
#pragma omp parallel for
        for(int i = 0 ;i< objData.verts_.size();i++)
        {

//            std::cout<<" change point from"<<objData.verts_[i]<<std::endl ;
            Matrix41 screen = manipulation::v2m(objData.verts_[i]) ;
            objData.verts_[i] = manipulation::m2v(result*screen) ;
//            std::cout<<" change point to"<<objData.verts_[i]<<std::endl ;
        }
    }


};
#endif //NEWCPURENDER_PIPLINE_H
