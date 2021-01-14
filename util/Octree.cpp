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

void  SplitFace(ObjData& data, OcNode* node,std::vector<OcNode*>& nodes, int minisize,std::vector<int>& allfaces,int layer)
{
//    std::vector<int> allfaces = node->getFaces() ;
//    std::cout<<" my node has "<<allfaces.size()<<std::endl ;

    node->setFaceSize(allfaces.size()) ;



    if(allfaces.size() <= minisize || layer>=3){
        node->setLeafNode() ;
        node->setFaces(&allfaces) ;
        return ;
    }

    std::unique_ptr<BoundingBox> rootbox(node->getBox()) ;
    Vec3f pmin = rootbox->getPmin() ;
    Vec3f pmax = rootbox->getPmax() ;
    Vec3f pmid = (pmin+pmax)/2.0 ;
    Vec3f size = pmax - pmin ;

    if(size[0] * size[1] * size[2] < 1)
    {
        node->setLeafNode() ;
        node->setFaces(&allfaces) ;
        return ;
    }

#ifdef LOG
    std::cout<<"split from "<<pmin<<" to "<<pmax<<" by "<<pmid <<std::endl ;
#endif

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
    Vec3f beginRLL = Vec3f(pmid[0],pmin[1],pmin[2]) ;
    Vec3f beginLRL = Vec3f(pmin[0],pmid[1],pmin[2]) ;
    Vec3f beginLLR = Vec3f(pmin[0],pmin[1],pmid[2]) ;
    Vec3f beginRRL = Vec3f(pmid[0],pmid[1],pmin[2]) ;
    Vec3f beginRLR = Vec3f(pmid[0],pmin[1],pmid[2]) ;
    Vec3f beginLRR = Vec3f(pmin[0],pmid[1],pmid[2]) ;
    Vec3f beginRRR = pmid ;

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
    SplitFace(data,node1,nodes,minisize,thirdSplit_LLL,layer+1)  ;


    boxRLL = new BoundingBox(beginRLL,beginRLL+vol) ;
    std::unique_ptr<BoundingBox> child2(boxRLL) ;
    OcNode* node2 = new OcNode(nodes.size(),child2) ;
    node->setChild(node2,1) ;
    nodes.push_back(node2) ;
    SplitFace(data,node2,nodes,minisize,thirdSplit_RLL,layer+1) ;

    boxLRL = new BoundingBox(beginLRL,beginLRL+vol) ;
    std::unique_ptr<BoundingBox> child3(boxLRL) ;
    OcNode* node3 = new OcNode(nodes.size(),child3) ;
    node->setChild(node3,2) ;
    nodes.push_back(node3) ;
    SplitFace(data,node3,nodes,minisize,thirdSplit_LRL,layer+1) ;

    boxLLR = new BoundingBox(beginLLR,beginLLR+vol) ;
    std::unique_ptr<BoundingBox> child4(boxLLR) ;
    OcNode* node4 = new OcNode(nodes.size(),child4) ;
    node->setChild(node4,3) ;
    nodes.push_back(node4) ;
    SplitFace(data,node4,nodes,minisize,thirdSplit_LLR,layer+1) ;

    boxRRL = new BoundingBox(beginRRL,beginRRL+vol) ;
    std::unique_ptr<BoundingBox> child5(boxRRL) ;
    OcNode* node5 = new OcNode(nodes.size(),child5) ;
    node->setChild(node5,4) ;
    nodes.push_back(node5) ;
    SplitFace(data,node5,nodes,minisize,thirdSplit_RRL,layer+1) ;

    boxRLR = new BoundingBox(beginRLR,beginRLR+vol) ;
    std::unique_ptr<BoundingBox> child6(boxRLR) ;
    OcNode* node6 = new OcNode(nodes.size(),child6) ;
    node->setChild(node6,5) ;
    nodes.push_back(node6) ;
    SplitFace(data,node6,nodes,minisize,thirdSplit_RLR,layer+1) ;

    boxLRR = new BoundingBox(beginLRR,beginLRR+vol) ;
    std::unique_ptr<BoundingBox> child7(boxLRR) ;
    OcNode* node7 = new OcNode(nodes.size(),child7) ;
    node->setChild(node7,6) ;
    nodes.push_back(node7) ;
    SplitFace(data,node7,nodes,minisize,thirdSplit_LRR,layer+1) ;

    boxRRR = new BoundingBox(beginRRR,beginRRR+vol) ;
    std::unique_ptr<BoundingBox> child8(boxRRR) ;
    OcNode* node8 = new OcNode(nodes.size(),child8) ;
    node->setChild(node8,7) ;
    nodes.push_back(node8) ;
    SplitFace(data,node8,nodes,minisize,thirdSplit_RRR,layer+1) ;

    node->setFaces(nullptr) ;

}


int Octree::getDepth(OcNode* node)
{
    if(node==nullptr) return 0 ;
    int res = 0 ;
    if(node == nullptr) node = root ;
    for(int i = 0 ; i <8; i++)
    {
        int t  = getDepth(node->getChild(i))+1 ;
        res = res>t?res:t ;
    }
    return res ;
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

    SplitFace(objdata, root, nodes,this->minisize,allfaces,0) ;
}


OcNode* OcNode::getNext(int index, Vec3f viewdir)
{
    if(index <0 || index>=8 ) { return nullptr ; }
    if(isLeaf) { return nullptr ; }


    OcNode** children_s = children ;
    // from big to small
    if(index==0)
    {
        lastView = viewdir ;
        std::sort(&t[0],(&t[7])+1,[children_s,viewdir](int i, int j){
            if(children_s[i]== nullptr) return false ;
            if(children_s[j]==nullptr) return true ;
            float v1 = children_s[i]->box->getPmid() * viewdir  ;
            float v2 = children_s[j]->box->getPmid() * viewdir  ;
            return v1 < v2 ;
        }) ;
    }

    if((lastView - viewdir).norm()>0.2)
    {
        lastView = viewdir ;
        std::sort(&t[0],(&t[7])+1,[children_s,viewdir](int i, int j){
            if(children_s[i]== nullptr) return false ;
            if(children_s[j]==nullptr) return true ;
            float v1 = children_s[i]->box->getPmid() * viewdir  ;
            float v2 = children_s[j]->box->getPmid() * viewdir  ;
            return v1 < v2 ;
        }) ;
    }

//    std::cout<<"choose "<<t[index]<<"by "<<a<<" "<<b<<" :"<<viewdir<< std::endl ;

    return children[t[index]] ;
}