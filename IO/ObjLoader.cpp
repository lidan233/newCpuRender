//
// Created by lidan on 19/12/2020.
//

#include "ObjLoader.h"
ObjLoader::ObjLoader(std::string path)
{
    std::ifstream in;
    in.open (path, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            objdata.verts_.push_back(v);
            center = center + v ;
            pmin = Vec::min(v,pmin) ;
            pmax = Vec::min(v,pmax) ;
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