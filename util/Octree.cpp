//
// Created by 李源 on 2020-12-12.
//

#include "Octree.h"



void split(ObjData& data, std::vector<int>& fatherfaces,int splitDimension, float splitValue, std::vector<int>& face1, std::vector<int>& face2)
{

    for(int i = 0 ; i < fatherfaces.size();i++)
    {
        Vec3i face = data.faces_[fatherfaces[i]] ;

        if( data.verts_[face[0]][splitDimension] < splitValue
                || data.verts_[face[1]][splitDimension] < splitValue
                || data.verts_[face[2]][splitDimension] < splitValue )
        {
            face1.push_back(fatherfaces[i]) ;
        }

        if( data.verts_[face[0]][splitDimension] >= splitValue
            || data.verts_[face[1]][splitDimension] >= splitValue
            || data.verts_[face[2]][splitDimension] >= splitValue )
        {
            face2.push_back(fatherfaces[i]) ;
        }
    }

}

std::vector<int>*  SplitFace(ObjData& data, OcNode* node,std::vector<OcNode*>& nodes, int minisize,std::vector<int>& allfaces)
{
//    std::vector<int> allfaces = node->getFaces() ;
    std::cout<<" my node has "<<allfaces.size()<<std::endl ;
    if(allfaces.size() < minisize) return &allfaces;
    std::unique_ptr<BoundingBox> rootbox(node->getBox()) ;
    Vec3f pmin = rootbox->getPmin() ;
    Vec3f pmax = rootbox->getPmax() ;
    Vec3f pmid = (pmin+pmax)/2.0 ;
    Vec3f size = pmax - pmin ;


    std::vector<int> firstSplit_L ;
    std::vector<int> firstSplit_R ;
    split(data, allfaces, 0, pmid[0], firstSplit_L, firstSplit_R) ;

    std::vector<int> secondSplit_LL ;
    std::vector<int> secondSplit_LR ;
    std::vector<int> secondSplit_RL ;
    std::vector<int> secondSplit_RR ;
    split(data, firstSplit_L, 1, pmid[1], secondSplit_LL, secondSplit_LR) ;
    split(data, firstSplit_R, 1, pmid[1], secondSplit_RL, secondSplit_RR) ;


    std::vector<int> thirdSplit_LLL ;
    std::vector<int> thirdSplit_LLR ;
    std::vector<int> thirdSplit_LRL ;
    std::vector<int> thirdSplit_LRR ;
    std::vector<int> thirdSplit_RLL ;
    std::vector<int> thirdSplit_RLR ;
    std::vector<int> thirdSplit_RRL ;
    std::vector<int> thirdSplit_RRR ;
    split(data, secondSplit_LL, 2, pmid[2], thirdSplit_LLL, thirdSplit_LLR) ;
    split(data, secondSplit_LR, 2, pmid[2], thirdSplit_LRL, thirdSplit_LRR) ;
    split(data, secondSplit_RL, 2, pmid[2], thirdSplit_RLL, thirdSplit_RLR) ;
    split(data, secondSplit_RR, 2, pmid[2], thirdSplit_RRL, thirdSplit_RRR) ;


    Vec3f beginLLL = pmin ;
    Vec3f beginRLL = pmin + Vec3f(pmid[0],0,0) ;
    Vec3f beginLRL = pmin + Vec3f(0,pmid[1],0) ;
    Vec3f beginLLR = pmin + Vec3f(0,0,pmid[2]) ;
    Vec3f beginRRL = pmin + Vec3f(pmid[0],pmid[1],0) ;
    Vec3f beginRLR = pmin + Vec3f(pmid[0],0,pmid[2]) ;
    Vec3f beginLRR = pmin + Vec3f(0,pmid[1],pmid[2] ) ;
    Vec3f beginRRR = pmin + Vec3f(pmid[0],pmid[1],pmid[2]) ;

    Vec3f vol = pmid - pmin ;

    BoundingBox* boxLLL ;
    BoundingBox* boxRLL ;
    BoundingBox* boxLRL ;
    BoundingBox* boxLLR ;
    BoundingBox* boxRRL ;
    BoundingBox* boxRLR ;
    BoundingBox* boxLRR ;
    BoundingBox* boxRRR ;

    boxLLL = new BoundingBox(beginLLL,beginLLL+vol) ;
    std::unique_ptr<BoundingBox> child1(boxLLL) ;
    OcNode* node1 = new OcNode(nodes.size(),child1) ;
    node->setChild(node1,0) ;
    nodes.push_back(node1) ;
    node1->setFaces(SplitFace(data,node1,nodes,minisize,thirdSplit_LLL) );

    boxRLL = new BoundingBox(beginRLL,beginRLL+vol) ;
    std::unique_ptr<BoundingBox> child2(boxRLL) ;
    OcNode* node2 = new OcNode(nodes.size(),child2) ;
    node->setChild(node2,1) ;
    nodes.push_back(node2) ;
    node2->setFaces(SplitFace(data,node2,nodes,minisize,thirdSplit_LLR) );

    boxLRL = new BoundingBox(beginLRL,beginLRL+vol) ;
    std::unique_ptr<BoundingBox> child3(boxLRL) ;
    OcNode* node3 = new OcNode(nodes.size(),child3) ;
    node->setChild(node3,2) ;
    nodes.push_back(node3) ;
    node3->setFaces(SplitFace(data,node3,nodes,minisize,thirdSplit_LRL) ) ;

    boxLLR = new BoundingBox(beginLLR,beginLLR+vol) ;
    std::unique_ptr<BoundingBox> child4(boxLLR) ;
    OcNode* node4 = new OcNode(nodes.size(),child4) ;
    node->setChild(node4,3) ;
    nodes.push_back(node4) ;
    node4->setFaces(SplitFace(data,node4,nodes,minisize,thirdSplit_LLR) ) ;

    boxRRL = new BoundingBox(beginRRL,beginRRL+vol) ;
    std::unique_ptr<BoundingBox> child5(boxRRL) ;
    OcNode* node5 = new OcNode(nodes.size(),child5) ;
    node->setChild(node5,4) ;
    nodes.push_back(node5) ;
    node5->setFaces(SplitFace(data,node5,nodes,minisize,thirdSplit_RRL) ) ;

    boxRLR = new BoundingBox(beginRLR,beginRLR+vol) ;
    std::unique_ptr<BoundingBox> child6(boxRLR) ;
    OcNode* node6 = new OcNode(nodes.size(),child6) ;
    node->setChild(node6,5) ;
    nodes.push_back(node6) ;
    node6->setFaces(SplitFace(data,node6,nodes,minisize,thirdSplit_RLR) ) ;

    boxLRR = new BoundingBox(beginLRR,beginLRR+vol) ;
    std::unique_ptr<BoundingBox> child7(boxLRR) ;
    OcNode* node7 = new OcNode(nodes.size(),child7) ;
    node->setChild(node7,6) ;
    nodes.push_back(node7) ;
    node7->setFaces(SplitFace(data,node7,nodes,minisize,thirdSplit_LRR) );

    boxRRR = new BoundingBox(beginRRR,beginRRR+vol) ;
    std::unique_ptr<BoundingBox> child8(boxRRR) ;
    OcNode* node8 = new OcNode(nodes.size(),child8) ;
    node->setChild(node8,7) ;
    nodes.push_back(node8) ;
    node7->setFaces(SplitFace(data,node8,nodes,minisize,thirdSplit_RRR) ) ;

    return nullptr ;
}


void Octree::Build(const BoundingBox& box,ObjData& objdata,int minsize)
{
    Vec3f allmin = box.getPmin() ;
    Vec3f allmax = box.getPmax() ;
    this->minisize = minsize ;
    std::unique_ptr<BoundingBox> rootbox(new BoundingBox(allmin,allmax)) ;
    std::vector<int> allfaces ;
    for(int i = 0 ; i < objdata.faces_.size(); i++)
    {
        allfaces.push_back(i) ;
    }

    root = new OcNode(nodes.size(),rootbox) ;
    nodes.push_back(root) ;

    SplitFace(objdata, root, nodes,this->minisize,allfaces) ;
}


OcNode* OcNode::getNext(int index, Vec3f viewdir)
{
    if(index <0 || index>=8 ) { return nullptr ; }
    if(isLeaf) { return nullptr ; }
    int t[8] = {0,1,2,3,4,5,6,7} ;
    OcNode** children_s = children ;
    // from big to small
    std::sort(&t[0],&t[1],[children_s,viewdir](int i, int j){
        float v1 = children_s[i]->box->getPmid() * viewdir  ;
        float v2 = children_s[j]->box->getPmid() * viewdir  ;
        return v1 > v2 ;
    }) ;
    return children[t[index]] ;
}