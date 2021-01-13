//
// Created by lidan on 19/12/2020.
//

#ifndef NEWCPURENDER_OBJLOADER_H
#define NEWCPURENDER_OBJLOADER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include "Vec.h"
#include "common.h"

class ObjLoader {
private:
    ObjData objdata ;
    Vec3f center = Vec3f(0.0,0.0,0.0);
    Vec3f pmin = Vec3f(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
    Vec3f pmax = Vec3f(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());
public:
    ObjLoader(std::string path) ;
    void randomCopy(int num,Vec3f begin,Vec3f Box) ;
    void Copy(int i,Vec3f begin,Vec3f Box) ;
    ObjData& getData() { return objdata; }
    Vec3f getCenter() { return center; }
    Vec3f getMin() { return pmin; }
    Vec3f getMax() { return pmax; }
};


#endif //NEWCPURENDER_OBJLOADER_H
