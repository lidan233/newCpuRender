//
// Created by 李源 on 2020-12-12.
//

#ifndef HIERARCHYZBUFFER_BOUNDINGBOX_H
#define HIERARCHYZBUFFER_BOUNDINGBOX_H

#include "Vec.h"


#include <limits.h>
#include <memory>



class BoundingBox {
private:
    Vec3f _pmin = Vec3f(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
    Vec3f _pmax = Vec3f(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());
public:


    BoundingBox(Vec3f pmin, Vec3f pmax)
    {
        _pmin = pmin ;
        _pmax = pmax ;
    }

    Vec3f getPmin() const { return _pmin; }
    Vec3f getPmax() const { return _pmax; }
    Vec3f getPmid() const { return (_pmin + _pmax)/2.0 ;}

    double getsize()
    {
        Vec3f size = _pmax - _pmin ;
        return int(size[0]) * int(size[1]) * int(size[2]) ;
    }

    void merge(const BoundingBox& box)

    {
        _pmin = min(_pmin,box._pmin) ;
        _pmax = max(_pmax,box._pmax) ;
    }

    bool isInteract(const BoundingBox& box)
    {
        Vec3f newpmin = max(box._pmin,_pmin) ;
        Vec3f newpmax = min(box._pmax,_pmax) ;

        Vec3f vol = newpmax - newpmin ;
        if(vol[0]>=0 && vol[1]>=0 && vol[2]>=0)
        {
            return true ;
        }

        return false ;
    }


    std::unique_ptr<BoundingBox> interact(const BoundingBox& box)
    {
        if(this->isInteract(box))
        {
            Vec3f newpmin = max(box._pmin,_pmin) ;
            Vec3f newpmax = min(box._pmax,_pmax) ;

            BoundingBox* res1 = new BoundingBox(newpmin,newpmax) ;
            std::unique_ptr<BoundingBox> res(res1) ;
            return res ;
        }

        return nullptr ;
    }

};


#endif //HIERARCHYZBUFFER_BOUNDINGBOX_H
