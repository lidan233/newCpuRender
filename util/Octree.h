//
// Created by 李源 on 2020-12-12.
//

#ifndef HIERARCHYZBUFFER_OCTREE_H
#define HIERARCHYZBUFFER_OCTREE_H
#include <vector>

#include "BoundingBox.h"

struct OcNode{
    int isLeaf = true ;
    int id = -1 ;
    OcNode* children[8] = {nullptr} ;
    std::unique_ptr<BoundingBox> box = nullptr ;

    OcNode() {}
    OcNode(int id, std::unique_ptr<BoundingBox>& b)
    {
        this->id = id ;
        this->box = std::move(b) ;
    }
};

class Octree {
private:
    OcNode* root = nullptr;
    std::vector<OcNode*> nodes ;
    int minsize = -1 ;

    void Build(const BoundingBox& box,int minsize);
public:
    Octree(const BoundingBox& allvol, int minsize)
    {
        Build(allvol,minsize);
    }

};


#endif //HIERARCHYZBUFFER_OCTREE_H
