#include "tgaimage.h"
#include "Point.h"
#include "Line.h"
#include "render.h"
#include "triangle.h"
#include "camera.h"
#include "pipline.h"
#include "window.h"
#include "HierachyZBuffer.h"

#include <glm/gtx/string_cast.hpp>

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

//    for(int i = 0 ; i < objData.verts_.size(); i++)
//    {
//        std::cout<<"this vertex i "<<objData.verts_[i]<<std::endl;
//    }

//    Matrix44 viewport = manipulation::viewport(width/8,height/8,width*3/4,height*3/4,255) ;
        PipLine pipline = PipLine(model,view,project,viewport) ;
        pipline.change(objData) ;

//    for(int i = 0 ; i < objData.verts_.size(); i++)
//    {
//        std::cout<<"this vertex i "<<objData.verts_[i]<<std::endl;
//    }



        srand(time(NULL) ) ;
        int size = objData.faces_.size() ;
        for(int i = 0; i< size;i++)
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
//        std::cout<<"face "<< face[0]<<" " << face[1]<<" " <<face[2]<<" " << light_dir<<" " << intensity<<" " <<std::endl ;

            int index3[3] = {0,1,2};
            Triangle d(index3) ;

//            d.draw(image1,*zBuffer, t4,texture,intensity[0]+intensity[1]+intensity[2],image ) ;
            d.draw_hierachy_zbuffer(image1,*hzBuffer, t4,texture,intensity[0]+intensity[1]+intensity[2],image) ;
        }
//        image1.flip_vertically();
//        image1.write_tga_file("output1.tga");
        long starttime = time(NULL) ;
        if(window.render(reinterpret_cast<Uint32*>(image1.buffer()))<0)
            break ;
        Vec2f cursorOffset = window.getOffset() ;
//        std::cout<<cursorOffset[0]<<" "<<cursorOffset[1]<<std::endl ;
        ca.ProcessMouseMovement(cursorOffset[0],cursorOffset[1],true );
//        std::cout<<cursorOffset[0]<<" "<<cursorOffset[1]<<std::endl ;

//        std::cout<<"using "<<time(NULL)<<" "<<starttime<<" FPS"<<std::endl ;
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

int main2(int argc, char** argv) {
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