//
// Created by 李源 on 2020-12-12.
//

#ifndef HIERARCHYZBUFFER_BOUNDINGBOX_H
#define HIERARCHYZBUFFER_BOUNDINGBOX_H

#include <glm/glm.hpp>
#include <limits.h>
#include <memory>



class BoundingBox {
private:
    glm::dvec3 _pmin = glm::dvec3(std::numeric_limits<float>::max());
    glm::dvec3 _pmax = glm::dvec3(std::numeric_limits<float>::min());
public:

    BoundingBox() {}

    BoundingBox(glm::dvec3 pmin, glm::dvec3 pmax)
    {
        _pmin = pmin ;
        _pmax = pmax ;
    }

    glm::dvec3 getPmin() const { return _pmin; }
    glm::dvec3 getPmax() const { return _pmax; }

    double getsize()
    {
        glm::dvec3 size = _pmax - _pmin ;
        return int(size[0]) * int(size[1]) * int(size[2]) ;
    }

    void merge(const BoundingBox& box)

    {
        _pmin = glm::min(_pmin,box._pmin) ;
        _pmax = glm::max(_pmax,box._pmax) ;
    }

    bool isInteract(const BoundingBox& box)
    {
        glm::dvec3 newpmin = glm::max(box._pmin,_pmin) ;
        glm::dvec3 newpmax = glm::min(box._pmax,_pmax) ;

        glm::dvec3 vol = newpmax - newpmin ;
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
            glm::dvec3 newpmin = glm::max(box._pmin,_pmin) ;
            glm::dvec3 newpmax = glm::min(box._pmax,_pmax) ;

            BoundingBox* res1 = new BoundingBox(newpmin,newpmax) ;
            std::unique_ptr<BoundingBox> res(res1) ;
            return res ;
        }

        return nullptr ;
    }
};


#endif //HIERARCHYZBUFFER_BOUNDINGBOX_H
