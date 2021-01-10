//
// Created by lidan on 19/12/2020.
//

#include "ObjLoader.h"
#include <stdlib.h>
#include <time.h>

void ObjLoader::randomCopy(int num)
{
    srand(time(NULL)) ;
    int v_size = objdata.verts_.size() ;
    int f_size = objdata.faces_.size() ;
    int i_size = objdata.idxTex_.size() ;
    int in_size = objdata.idxNorm_.size() ;

    objdata.verts_.reserve(num*v_size) ;
    objdata.faces_.reserve(num*f_size) ;
    objdata.idxTex_.reserve(num*i_size) ;
    objdata.idxNorm_.reserve(num*in_size) ;



    for(int n =0 ; n < num ; n++)
    {
        std::vector<Vec3f> verts_;
        std::vector<Vec3i> faces_;
        std::vector<Vec3i> idxTex_ ;
        std::vector<Vec3i> idxNorm_ ;

        Vec3f tv = Vec3f((float(rand()%50)-25),(float(rand()%50-25)),(float(rand()%50-25))) ;
        for(int i = 0 ; i < v_size ; i++)
        {
            Vec3f t1 = objdata.verts_[i] + tv ;
            verts_.push_back(t1) ;
            this->pmin = min(this->pmin,t1);
            this->pmax = max(this->pmax,t1) ;

        }

        objdata.verts_.insert(objdata.verts_.end(),verts_.begin(),verts_.end()) ;

        int size_begin = objdata.verts_.size()  - v_size;
        for(int i = 0 ; i < f_size ; i++ )
        {
            Vec3i t1 =  objdata.faces_[i] + Vec3i(size_begin,size_begin,size_begin) ;
            faces_.push_back(t1) ;
        }
        objdata.faces_.insert(objdata.faces_.end(),faces_.begin(),faces_.end()) ;


        for(int i = 0 ; i < i_size ; i++ )
        {
            Vec3i t1 =  objdata.idxTex_[i] ;
            idxTex_.push_back(t1) ;
        }
        objdata.idxTex_.insert(objdata.idxTex_.end(),idxTex_.begin(),idxTex_.end()) ;


        for(int i = 0 ; i < in_size ; i++ )
        {
            Vec3i t1 =  objdata.idxNorm_[i] ;
            idxNorm_.push_back(t1) ;
        }
        objdata.idxNorm_.insert(objdata.idxNorm_.end(),idxNorm_.begin(),idxNorm_.end()) ;
    }

}


ObjLoader::ObjLoader(std::string path)
{
    std::ifstream in;
    in.open (path, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
//        std::cout<<line<<std::endl ;
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];


//            v = v*10.0 ;


            objdata.verts_.push_back(v);
            center = center + v ;
            pmin = Vec::min(v,pmin) ;
            pmax = Vec::max(v,pmax) ;
        }else if(!line.compare(0,3,"vt "))
        {
            Vec2f t;
            float temp ;
            iss >> trash>>trash ;
            for(int i = 0; i <2 ;i++)
            {
                iss>>temp;
                t[i]= temp ;
            }
            objdata.texture.push_back(t) ;
        }else if(!line.compare(0,3,"vn "))
        {
            Vec3f n;
            float temp ;
            iss >> trash>>trash ;
            for(int i = 0; i <3 ;i++)
            {
                iss>>temp;
                n[i]= temp ;
            }
            objdata.norms.push_back(n) ;
        }else if (!line.compare(0, 2, "f ")) {
            Vec3i f ;
            Vec3i idxTex ;
            Vec3i idxNorm ;
            int iTex,iNorm, idx, count = 0 ;
            iss >> trash;
            //vertex/uv/normal
            while (iss >> idx >> trash >> iTex >> trash >> iNorm) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f[count] = idx ;
                idxTex[count] = --iTex ;
                idxNorm[count] = --iNorm ;
                count++ ;
            }
            assert(count==3) ;
            objdata.faces_.push_back(f);
            objdata.idxTex_.push_back(idxTex) ;
            objdata.idxNorm_.push_back(idxNorm) ;
        }
    }
    center = center/objdata.verts_.size() ;
    std::cerr << "# v# " << objdata.verts_.size() << " f# "  << objdata.faces_.size() << std::endl;
}