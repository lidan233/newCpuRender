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

#include <vector>

class PipLine{

private:

    Matrix44 model_ ;
    Matrix44 view_ ;
    Matrix44 project_ ;
    Matrix44 viewport_ ;

public:
    PipLine( Model& model, View& view, Project& project, ViewPort& viewPort)
    {
        model_ = model.getMatrix() ;
        view_ = view.getMatrix() ;
        project_ = project.getMatrix() ;
        viewport_ = viewPort.getMatrix() ;
    }

    void change(ObjData& objData)
    {

        Matrix44 result = viewport_*project_*view_*model_ ;
        std::cout<<"model "<<model_ ;
        std::cout<<"view "<<view_ ;
        std::cout<<"project "<<project_ ;
        std::cout<<"project "<<viewport_ ;
        std::cout<<"result "<<result ;

        for(int i = 0 ;i< objData.verts_.size();i++)
        {
            std::cout<<" change point from"<<objData.verts_[i]<<std::endl ;
            Matrix41 screen = manipulation::v2m(objData.verts_[i]) ;
            objData.verts_[i] = manipulation::m2v(result*screen) ;
            std::cout<<" change point to"<<objData.verts_[i]<<std::endl ;
        }
    }


};
#endif //NEWCPURENDER_PIPLINE_H
