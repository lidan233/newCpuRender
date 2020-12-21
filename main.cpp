#include "tgaimage.h"
#include "Point.h"
#include "Line.h"
#include "render.h"
#include "triangle.h"
#include "camera.h"
#include "pipline.h"

#include <glm/gtx/string_cast.hpp>

const int width = 800;
const int height = 800 ;
const int depth = 255  ;

//Vec3f light_dir(0,0,-1) ;
//Vec3f light_dir = Vec3f(1,-1,1).normalize();
Vec3f camera_pos(0,0,3) ;
Vec3f eye(1,1,3);
Vec3f center(0,0,0);
Vec3f up(0,1,0);
Vec3f light_dir = camera_pos - center ;

int main(int argc, char** argv )
{
    ObjLoader loader("C:\\Users\\lidan\\Desktop\\render\\newCpuRender\\testData\\cube.obj") ;
    ObjData objData = loader.getData() ;
    Vec3f centerM = loader.getCenter() ;

    float yaw = NYAW ;
    float pitch = NPITCH ;
    camera ca(camera_pos,up,center,yaw,pitch) ;

    TGAImage image(width, height, TGAImage::RGB);
    image.set(0, 0, red);
    TGAImage image1(width, height, TGAImage::RGB);
    image1.set(0, 0, red);

    Model model = Model(centerM) ;
    View view = View(ca) ;
    Project project = Project(ca,800,800,0.1,1000) ;
    ViewPort viewport = ViewPort(800,800) ;

//    Matrix44 viewport = manipulation::viewport(width/8,height/8,width*3/4,height*3/4,255) ;
    PipLine pipline = PipLine(model,view,project,viewport) ;
    pipline.change(objData) ;

    for(int i = 0 ; i < objData.verts_.size(); i++)
    {
        std::cout<<objData.verts_[i]<<std::endl;
    }

    ZBuffer* zBuffer = new ZBuffer(image.get_height(),image.get_width()) ;

    srand(time(NULL) ) ;
    int size = objData.faces_.size() ;
    for(int i = 0; i< size;i++)
    {
        Vec3i face = objData.faces_[i] ;
        vector<Vec3i> t4 ;
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

        int index3[3] = {0,1,2};
        Triangle d(index3) ;
        d.draw_vec3i(image1,*zBuffer, t4,texture,intensity,image1 ) ;
    }
    image1.flip_vertically();
    image1.write_tga_file("output1.tga");
    return 0 ;
}

int main1(int argc, char** argv) {
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