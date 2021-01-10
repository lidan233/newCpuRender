#include "tgaimage.h"
#include "Point.h"
#include "Line.h"
#include "render.h"
#include "triangle.h"
#include "camera.h"
#include "pipline.h"
#include "window.h"
#include "HierachyZBuffer.h"
#include "Octree.h"
#include "Timer.h"
#include "ScanlineZbuffer.h"

#include <glm/gtx/string_cast.hpp>
#include <stack>
#include <omp.h>


const int width = 800;
const int height = 800 ;
const int depth = 255  ;

//Vec3f light_dir(0,0,-1) ;
//Vec3f light_dir = Vec3f(1,-1,1).normalize();
Vec3f camera_pos(0,0,50) ;
Vec3f eye(1,1,3);
Vec3f center(0,0,0);
Vec3f up(0,1,0);
Vec3f light_dir = camera_pos - center ;
float yaw = NYAW ;
float pitch = NPITCH ;
camera ca(camera_pos,up,center,yaw,pitch) ;


#define NOLOG


void renderToImageHZ(ObjData& objData, TGAImage& renderimage , TGAImage& textureImage, int i, HierachyZBuffer* hzBuffer)
{
    Vec3i face = objData.faces_[i] ;
    vector<Vec3f> t4 ;
    vector<Vec2i> texture ;
    Vec3f intensity;

    Vec3i idx_v = objData.idxNorm_[i] ;
    vector<Vec3f> norms ;
    for(int j =0 ;j <3 ;j++)
    {
        Vec3f temp = objData.norms[idx_v[j]] ;
        norms.push_back(temp.normalize()) ;
    }

    for(int i = 0; i< 3;i++)
    {
#ifdef LOG
        std::cout<<" Triangle has verts "<<face[i]<<" :"<<objData.verts_[face[i]]<<std::endl ;
#endif
        t4.push_back(objData.verts_[face[i]]);
        texture.push_back(Vec2i(0,0)) ;
        intensity[i] = -(norms[i]*ca.getViewDir()) ;

    }

#ifdef LOG
    std::cout<<"face "<< face[0]<<" " << face[1]<<" " <<face[2]<<" " << light_dir<<" " << intensity<<" " <<std::endl ;
#endif
    int index3[3] = {0,1,2};
    Triangle d(index3) ;

    if(intensity[0]+intensity[1]+intensity[2]>0)
//            d.draw(image1,*zBuffer, t4,texture,intensity[0]+intensity[1]+intensity[2],image ) ;
        d.draw_hierachy_zbuffer(renderimage,*hzBuffer, t4,texture,intensity[0]+intensity[1]+intensity[2],textureImage) ;
//    d.draw_hierachy_zbuffer(renderimage,*hzBuffer, t4,texture,1,textureImage) ;

}


void renderToImage(ObjData& objData, TGAImage& renderimage , TGAImage& textureImage, int i, ZBuffer* zBuffer)
{
    Vec3i face = objData.faces_[i] ;
    vector<Vec3f> t4 ;
    vector<Vec2i> texture ;
    Vec3f intensity;

    Vec3i idx_v = objData.idxNorm_[i] ;
    vector<Vec3f> norms ;
    for(int j =0 ;j <3 ;j++)
    {
        Vec3f temp = objData.norms[idx_v[j]] ;
        norms.push_back(temp.normalize()) ;
    }

    for(int i = 0; i< 3;i++)
    {
        t4.push_back(objData.verts_[face[i]]);
        texture.push_back(Vec2i(0,0)) ;
        intensity[i] = norms[i]*light_dir ;

    }

#ifdef LOG
    std::cout<<"face "<< face[0]<<" " << face[1]<<" " <<face[2]<<" " << light_dir<<" " << intensity<<" " <<std::endl ;
#endif
    int index3[3] = {0,1,2};
    Triangle d(index3) ;

//            d.draw(image1,*zBuffer, t4,texture,intensity[0]+intensity[1]+intensity[2],image ) ;
    d.draw(renderimage,*zBuffer, t4,texture,intensity[0]+intensity[1]+intensity[2],textureImage) ;

}

#ifdef __cplusplus
extern "C"
#endif
int usingScanlineBuffer(int argc,ObjLoader& loader )
{
    ObjData objData = loader.getData() ;
    ObjData objData1 = objData ;
    Vec3f centerM = loader.getCenter() ;

    TGAImage image1(width, height, TGAImage::RGBA);
    image1.set(0, 0, red);

    Window window(height,width,reinterpret_cast<Uint32*>(image1.buffer())) ;
    ScanlineZbuffer slzBuffer(height,width) ;

    while(1)
    {
        Timer tc = Timer();
        Model model = Model(centerM);
        View view = View(ca);
        Project project = Project(ca, 800, 800, 0.1, 1000);
        ViewPort viewport = ViewPort(800, 800);
        PipLine pipline = PipLine(model,view,project,viewport) ;
        pipline.change(objData) ;

        if(window.render(reinterpret_cast<Uint32*>(image1.buffer()))<0) break ;
        Vec2f cursorOffset = window.getOffset() ;
        ca.ProcessMouseMovement(cursorOffset[0],cursorOffset[1],true );

        slzBuffer.build(objData) ;
        slzBuffer.run(objData,ca.getViewDir(),image1) ;

        objData = objData1 ;
        image1.clear();
        std::cout<<tc.elapsed()<<std::endl ;
    }

    window.endrender() ;
    return 0 ;
}

// Using
#ifdef __cplusplus
extern "C"
#endif
int usingHZandOctree(int argc,ObjLoader& loader )
{

//    srand(time(NULL)) ;
//    ObjLoader loader(path) ;
//    loader.randomCopy(argc) ;
    ObjData objData = loader.getData() ;

    ObjData objData1 = objData ;
    Vec3f centerM = loader.getCenter() ;
    BoundingBox allbox = BoundingBox(loader.getMin(), loader.getMax()) ;

    Octree* octree = new Octree(allbox,objData,30) ;
    OcNode* root = octree->getRoot() ;
    std::cout<< "all octree depth is "<<octree->getDepth(root)<<std::endl ;


    TGAImage image(width, height, TGAImage::RGB);
    image.set(0, 0, red);
    TGAImage image1(width, height, TGAImage::RGBA);
    image1.set(0, 0, red);

    Window window(height,width,reinterpret_cast<Uint32*>(image1.buffer())) ;
    ZBuffer* zBuffer = new ZBuffer(image.get_height(),image.get_width()) ;
    HierachyZBuffer* hzBuffer = new HierachyZBuffer(image.get_height(),image.get_width()) ;




    while(1)
    {
        Timer tc = Timer() ;
        Model model = Model(centerM) ;
        View view = View(ca) ;
        Project project = Project(ca,800,800,0.1,1000) ;
        ViewPort viewport = ViewPort(800,800) ;

#ifdef LOG
    for(int i = 0 ; i < objData.verts_.size(); i++)
    {
        std::cout<<"this vertex i "<<objData.verts_[i]<<std::endl;
    }
#endif

        PipLine pipline = PipLine(model,view,project,viewport) ;
        pipline.change(objData) ;

#ifdef LOG
    for(int i = 0 ; i < objData.verts_.size(); i++)
    {
        std::cout<<"this vertex i "<<objData.verts_[i]<<std::endl;
    }
#endif

        std::stack<std::pair<OcNode*,int>> stk ;
        stk.push(std::pair<OcNode*,int>(root,0)) ;
        while(stk.size()!=0)
        {
            std::pair<OcNode*,int>& t = stk.top() ;

            OcNode* newNode = nullptr ;
            if(t.first->isLeafNode())
            {
                newNode = t.first ;
                stk.pop() ;
            }else{
                newNode = t.first->getNext(t.second,ca.getViewDir()) ;
                t.second++ ;

                if(newNode==nullptr)  {
                    stk.pop() ;
//                std::cout<<"pop"<<t.first->getId()<<std::endl;
                    continue ;
                }
            }

            // if this node is a leaf node, so we decided to render, else push as the iterator
            if(newNode->isLeafNode())
            {
                if(newNode->getFaces_size()<=0) continue ;
                BoundingBox* t = newNode->getBox() ;
                BoundingBox* t1 = pipline.change(*t) ;

                if(!hzBuffer->canRejectBox(t1->getPmin(),t1->getPmax(),t1->getPmin()[2]))
                {
                    std::vector<int>& allData = *(newNode->getFaces());
                    for(int i = 0 ; i < allData.size() ; i++)
                    {
//                        std::cout<<" render "<<allData[i]<<" triangle"<<std::endl ;
                        renderToImageHZ(objData,image1,image,allData[i],hzBuffer) ;
                    }
                }else{
#ifdef LOG
                    Vec3f t2 = t1->getPmin() ;
                    Vec3f t3 = t1->getPmax() ;
                    std::cout<<"reject "<<t2<<" "<<t3<<std::endl ;
#endif
                }

            }else{

                BoundingBox* t = newNode->getBox() ;
                BoundingBox* t1 = pipline.change(*t) ;
                if(newNode->getFaceSize()>0 && !hzBuffer->canRejectBox(t1->getPmin(),t1->getPmax(),t1->getPmin()[2]))
                {
                    std::pair<OcNode*,int> tt = std::pair<OcNode*,int>(newNode,0) ;
                    stk.push(tt) ;
//                std::cout<<"push" <<newNode->getId()<<std::endl;
                }else{
//                    std::cout<<"reject"<<std::endl ;
                }
            }
        }

        std::cout<<std::endl ;

        srand(time(NULL) ) ;
        long starttime = time(NULL) ;
        if(window.render(reinterpret_cast<Uint32*>(image1.buffer()))<0)
            break ;
        Vec2f cursorOffset = window.getOffset() ;
        ca.ProcessMouseMovement(cursorOffset[0],cursorOffset[1],true );

        objData = objData1 ;
        zBuffer->clear();
        hzBuffer->clear() ;
        image1.clear();
        std::cout<<tc.elapsed()<<std::endl ;
    }

    window.endrender() ;
    return 0 ;
}



// Using ZBuffer and hierachyZBuffer for buffers.
#ifdef __cplusplus
extern "C"
#endif
int usingHZ(int argc, ObjLoader& loader)
{
//    srand(time(NULL)) ;
//    ObjLoader loader("../testData/cube.obj") ;
//    loader.randomCopy(argc) ;
    ObjData objData = loader.getData() ;
    ObjData objData1 = objData ;
    Vec3f centerM = loader.getCenter() ;


    TGAImage image(width, height, TGAImage::RGB);
    image.set(0, 0, red);
    TGAImage image1(width, height, TGAImage::RGBA);
    image1.set(0, 0, red);

    Window window(height,width,reinterpret_cast<Uint32*>(image1.buffer())) ;
    ZBuffer* zBuffer = new ZBuffer(image.get_height(),image.get_width()) ;
    HierachyZBuffer* hzBuffer = new HierachyZBuffer(image.get_height(),image.get_width()) ;

    while(1)
    {
        Timer tc = Timer() ;
        Model model = Model(centerM) ;
        View view = View(ca) ;
        Project project = Project(ca,800,800,0.1,1000) ;
        ViewPort viewport = ViewPort(800,800) ;

#ifdef LOG
        for(int i = 0 ; i < objData.verts_.size(); i++)
    {
        std::cout<<"this vertex i "<<objData.verts_[i]<<std::endl;
    }
#endif

        PipLine pipline = PipLine(model,view,project,viewport) ;
        pipline.change(objData) ;

#ifdef LOG
        for(int i = 0 ; i < objData.verts_.size(); i++)
    {
        std::cout<<"this vertex i "<<objData.verts_[i]<<std::endl;
    }
#endif

        srand(time(NULL) ) ;
        int size = objData.faces_.size() ;
        for(int i = 0; i< size;i++)
        {

//            renderToImage(objData,image1,image,i,zBuffer) ;
            renderToImageHZ(objData,image1,image,i,hzBuffer) ;
        }
//        image1.flip_vertically();
//        image1.write_tga_file("output1.tga");
//        long starttime = time(NULL) ;
        if(window.render(reinterpret_cast<Uint32*>(image1.buffer()))<0)
            break ;
        Vec2f cursorOffset = window.getOffset() ;
        ca.ProcessMouseMovement(cursorOffset[0],cursorOffset[1],true );

        objData = objData1 ;
        zBuffer->clear();
        hzBuffer->clear() ;
        image1.clear();
        std::cout<<tc.elapsed()<<std::endl ;
    }

    window.endrender() ;
    return 0 ;
}


int main(int argc, char** argv)
{
    srand(time(NULL)) ;
//    ObjLoader loader("../testData/lidan.obj") ;
    ObjLoader loader("../testData/cube.obj") ;
    loader.randomCopy(1000) ;
//    usingHZ(1000,loader) ;
    usingScanlineBuffer(1000,loader) ;
//    usingHZandOctree(1000,loader) ;
}


// using test some class: HierachyZBuffer
int main1(int argc, char** argv)
{
    HierachyZBuffer buffer = HierachyZBuffer(800,800) ;
    buffer.cover(1,2,100) ;
    return 0 ;
}

int main4(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);
    image.set(0, 0, red);

    Render a = Render("C:\\Users\\lidan\\Desktop\\newCpuRender\\testData\\obj\\african_head\\african_head.obj") ;
    a.addTexture("C:\\Users\\lidan\\Desktop\\newCpuRender\\testData\\obj\\african_head\\african_head_diffuse.tga") ;
    vector<TGAColor> one ;
    vector<TGAColor> two ;
    a.setcamera(camera_pos) ;
    a.normal(800,800,center,eye,up);
    a.drawAll(image, one ,two,light_dir) ;

    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}