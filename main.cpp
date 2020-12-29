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

#include <glm/gtx/string_cast.hpp>
#include <stack>


const int width = 800;
const int height = 800 ;
const int depth = 255  ;

//Vec3f light_dir(0,0,-1) ;
//Vec3f light_dir = Vec3f(1,-1,1).normalize();
Vec3f camera_pos(2,2,3) ;
Vec3f eye(1,1,3);
Vec3f center(0,0,0);
Vec3f up(0,-3,2);
Vec3f light_dir = camera_pos - center ;

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
    d.draw_hierachy_zbuffer(renderimage,*hzBuffer, t4,texture,intensity[0]+intensity[1]+intensity[2],textureImage) ;

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



// Using
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv )
{
    srand(time(NULL)) ;
    ObjLoader loader("../testData/cube.obj") ;
    ObjData objData = loader.getData() ;
    ObjData objData1 = objData ;
    Vec3f centerM = loader.getCenter() ;
    BoundingBox allbox = BoundingBox(loader.getMin(), loader.getMax()) ;

    Octree* octree = new Octree(allbox,objData,10) ;
    OcNode* root = octree->getRoot() ;

    float yaw = NYAW ;
    float pitch = NPITCH ;
    camera ca(camera_pos,up,center,yaw,pitch) ;

    TGAImage image(width, height, TGAImage::RGB);
    image.set(0, 0, red);
    TGAImage image1(width, height, TGAImage::RGBA);
    image1.set(0, 0, red);

    Window window(height,width,reinterpret_cast<Uint32*>(image1.buffer())) ;
    ZBuffer* zBuffer = new ZBuffer(image.get_height(),image.get_width()) ;
    HierachyZBuffer* hzBuffer = new HierachyZBuffer(image.get_height(),image.get_width()) ;




    while(1)
    {
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
            OcNode* newNode = t.first->getNext(t.second,ca.getViewDir()) ;
            t.second++ ;
            if(newNode==nullptr)  {
                stk.pop() ;
                continue ;
            }

            // if this node is a leaf node, so we decided to render, else push as the iterator
            if(newNode->isLeafNode())
            {
                BoundingBox* t = newNode->getBox() ;
                BoundingBox* t1 = pipline.change(*t) ;

                if(!hzBuffer->canRejectBox(t1->getPmin(),t1->getPmax(),t1->getPmin()[2]))
                {

                    std::vector<int>& allData = *(newNode->getFaces());
                    for(int i = 0 ; i < allData.size() ; i++)
                    {
                        renderToImageHZ(objData,image1,image,allData[i],hzBuffer) ;
                    }
                }else{
                    std::cout<<"reject"<<std::endl ;
                }

            }else{
                std::pair<OcNode*,int> t1 = std::pair<OcNode*,int>(newNode,0) ;
                stk.push(t1) ;
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
    }

    window.endrender() ;
    return 0 ;
}



// Using ZBuffer and hierachyZBuffer for buffers.
#ifdef __cplusplus
extern "C"
#endif
int main2(int argc, char** argv )
{
    srand(time(NULL)) ;
    ObjLoader loader("../testData/cube.obj") ;
    ObjData objData = loader.getData() ;
    ObjData objData1 = objData ;
    Vec3f centerM = loader.getCenter() ;

    float yaw = NYAW ;
    float pitch = NPITCH ;
    camera ca(camera_pos,up,center,yaw,pitch) ;

    TGAImage image(width, height, TGAImage::RGB);
    image.set(0, 0, red);
    TGAImage image1(width, height, TGAImage::RGBA);
    image1.set(0, 0, red);

    Window window(height,width,reinterpret_cast<Uint32*>(image1.buffer())) ;
    ZBuffer* zBuffer = new ZBuffer(image.get_height(),image.get_width()) ;
    HierachyZBuffer* hzBuffer = new HierachyZBuffer(image.get_height(),image.get_width()) ;

    while(1)
    {
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
        long starttime = time(NULL) ;
        if(window.render(reinterpret_cast<Uint32*>(image1.buffer()))<0)
            break ;
        Vec2f cursorOffset = window.getOffset() ;
        ca.ProcessMouseMovement(cursorOffset[0],cursorOffset[1],true );

        objData = objData1 ;
        zBuffer->clear();
        hzBuffer->clear() ;
        image1.clear();
    }

    window.endrender() ;
    return 0 ;
}



// using test some class: HierachyZBuffer
int main1(int argc, char** argv)
{
    HierachyZBuffer buffer = HierachyZBuffer(800,800) ;
    buffer.cover(1,2,100) ;
}

int main3(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);
    image.set(0, 0, red);

    Render a = Render("C:\\Users\\lidan\\Desktop\\render\\newCpuRender\\testData\\obj\\african_head\\african_head.obj") ;
    a.addTexture("C:\\Users\\lidan\\Desktop\\render\\newCpuRender\\testData\\obj\\african_head\\african_head_diffuse.tga") ;
    vector<TGAColor> one ;
    vector<TGAColor> two ;
    a.setcamera(camera_pos) ;
    a.normal(800,800,center,eye,up);
    a.drawAll(image, one ,two,light_dir) ;

    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}