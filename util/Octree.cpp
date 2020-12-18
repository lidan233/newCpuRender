//
// Created by 李源 on 2020-12-12.
//

#include "Octree.h"



void SplitBuild(OcNode* node,int minsize,std::vector<OcNode*>& nodes)
{
    std::unique_ptr<BoundingBox>& rootbox = node->box ;
    glm::dvec3 pmin = rootbox->getPmin() ;
    glm::dvec3 pmax = rootbox->getPmax() ;
    glm::dvec3 pmid = (pmin+pmax)/2.0 ;
    glm::dvec3 size = pmax - pmin ;

    if(size[0]>minsize&&size[1]>minsize&&size[2]>minsize)
    {
        return ;
    }

    node->isLeaf = false ;

    glm::dvec3 begin1 = pmin ;
    glm::dvec3 begin2 = pmin + glm::dvec3(pmid[0],0,0) ;
    glm::dvec3 begin3 = pmin + glm::dvec3(0,pmid[1],0) ;
    glm::dvec3 begin4 = pmin + glm::dvec3(0,0,pmid[2]) ;
    glm::dvec3 begin5 = pmin + glm::dvec3(pmid[0],pmid[1],0) ;
    glm::dvec3 begin6 = pmin + glm::dvec3(pmid[0],0,pmid[2]) ;
    glm::dvec3 begin7 = pmin + glm::dvec3(0,pmid[1],pmid[2] ) ;
    glm::dvec3 begin8 = pmin + glm::dvec3(pmid[0],pmid[1],pmid[2]) ;

    glm::dvec3 vol = pmid - pmin ;


    BoundingBox* box1 ;
    BoundingBox* box2 ;
    BoundingBox* box3 ;
    BoundingBox* box4 ;
    BoundingBox* box5 ;
    BoundingBox* box6 ;
    BoundingBox* box7 ;
    BoundingBox* box8 ;


//    if(size[0]>minsize)
//    {
        box1 = new BoundingBox(begin1,begin1+vol) ;
        std::unique_ptr<BoundingBox> child1(box1) ;
        OcNode* node1 = new OcNode(nodes.size(),child1) ;
        node1->children[0] = node1 ;
        nodes.push_back(node1) ;
        SplitBuild(node1,minsize,nodes) ;
//    }
//    if(size[0]>minsize) {
        box2 = new BoundingBox(begin2,begin2+vol) ;
        std::unique_ptr<BoundingBox> child2(box2) ;
        OcNode* node2 = new OcNode(nodes.size(),child2) ;
        node2->children[1] = node2 ;
        nodes.push_back(node2) ;
        SplitBuild(node2,minsize,nodes) ;
//    }

//    if(size[1]>minsize) {
        box3 = new BoundingBox(begin3,begin3+vol) ;
        std::unique_ptr<BoundingBox> child3(box3) ;
        OcNode* node3 = new OcNode(nodes.size(),child3) ;
        node3->children[2] = node3 ;
        nodes.push_back(node3) ;
        SplitBuild(node3,minsize,nodes) ;
//    }
//    if(size[2]>minsize) {
        box4 = new BoundingBox(begin4,begin4+vol) ;
        std::unique_ptr<BoundingBox> child4(box4) ;
        OcNode* node4 = new OcNode(nodes.size(),child4) ;
        node4->children[3] = node4 ;
        nodes.push_back(node4) ;
        SplitBuild(node4,minsize,nodes) ;
//    }
//    if(size[0]>minsize||size[1]>minsize) {
        box5 = new BoundingBox(begin5,begin5+vol) ;
        std::unique_ptr<BoundingBox> child5(box5) ;
        OcNode* node5 = new OcNode(nodes.size(),child5) ;
        node5->children[4] = node5 ;
        nodes.push_back(node5) ;
        SplitBuild(node5,minsize,nodes) ;
//    }
//    if(size[0]>minsize||size[2]>minsize) {
        box6 = new BoundingBox(begin6,begin6+vol) ;
        std::unique_ptr<BoundingBox> child6(box6) ;
        OcNode* node6 = new OcNode(nodes.size(),child6) ;
        node6->children[5] = node6 ;
        nodes.push_back(node6) ;
        SplitBuild(node6,minsize,nodes) ;
//    }
//    if(size[1]>minsize||size[2]>minsize) {
        box7 = new BoundingBox(begin7,begin7+vol) ;
        std::unique_ptr<BoundingBox> child7(box7) ;
        OcNode* node7 = new OcNode(nodes.size(),child7) ;
        node7->children[6] = node7 ;
        nodes.push_back(node7) ;
        SplitBuild(node7,minsize,nodes) ;
//    }
//    if(size[0]>minsize||size[1]>minsize||size[2]>minsize) {
        box8 = new BoundingBox(begin8,begin8+vol) ;
        std::unique_ptr<BoundingBox> child8(box8) ;
        OcNode* node8 = new OcNode(nodes.size(),child8) ;
        node8->children[7] = node8 ;
        nodes.push_back(node8) ;
        SplitBuild(node8,minsize,nodes) ;
//    }

}

void Octree::Build(const BoundingBox& box,int minsize)
{
    glm::dvec3 allmin = box.getPmin() ;
    glm::dvec3 allmax = box.getPmax() ;
    this->minsize = minsize ;

    std::unique_ptr<BoundingBox> rootbox(new BoundingBox(allmin,allmax)) ;
    root = new OcNode(nodes.size(),rootbox) ;

    SplitBuild(root,minsize,nodes) ;
}
