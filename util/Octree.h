//
// Created by 李源 on 2020-12-12.
//

#ifndef HIERARCHYZBUFFER_OCTREE_H
#define HIERARCHYZBUFFER_OCTREE_H
#include <vector>

#include "BoundingBox.h"
#include "common.h"

struct OcNode{
private:
    int isLeaf = false ;
    int id = -1 ;
    OcNode* children[8] = {nullptr} ;
    std::unique_ptr<BoundingBox> box = nullptr ;
    std::vector<int>* faceid = nullptr;
public:
    OcNode(int id, std::unique_ptr<BoundingBox>& b)
    {
        this->id = id ;
        this->box = std::move(b) ;
//        this->faceid = faceid ;
    }
    BoundingBox getBox() {
        return BoundingBox(box->getPmin(), box->getPmax());
    }

    std::vector<int>* getFaces() { return faceid ; }
    void setFaces(std::vector<int>* faces) {
        if(faces!=nullptr)
        {
            faceid = new std::vector<int>(*faces) ;
            isLeaf = true ;
        }
    }

    OcNode* getChild(int i) { return children[i] ; }
    void setChild(OcNode* child, int i) { children[i] = child; }

    bool isLeafNode() { return isLeaf; }
    OcNode* getNext(int i, Vec3f viewdir) ;
};

class Octree {
private:
    OcNode* root = nullptr;
    std::vector<OcNode*> nodes ;
    int minisize = 10 ;

    void Build(const BoundingBox& box,ObjData& objdata,int minsize);
public:
    Octree(const BoundingBox& allvol, ObjData& objData, int minsize)
    {
        Build(allvol,objData,minsize);
    }

    OcNode* getRoot() { return root ; }
};


#endif //HIERARCHYZBUFFER_OCTREE_H
