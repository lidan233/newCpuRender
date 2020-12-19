#include "tgaimage.h"
#include "Point.h"
#include "Line.h"
#include "render.h"
#include "triangle.h"


const int width = 800;
const int height = 800 ;
const int depth = 255  ;

//Vec3f light_dir(0,0,-1) ;
Vec3f light_dir = Vec3f(1,-1,1).normalize();
Vec3f camera(0,0,3) ;
Vec3f eye(1,1,3);
Vec3f center(0,0,0);
Vec3f up(0,1,0);

int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);
    image.set(0, 0, red);

//	// i want to have the origin at the left bottom corner of the image
//
//
//	Point* be = new Point(1,10) ;
//	Point* end = new Point(10,1) ;
//
//    Line* ll1 = new Line(new Point(13, 20), new Point(80, 40));
//    ll1->drawLine_with_bilv( image, white) ;
//
//    Line* ll2 = new Line(new Point(20, 13),new Point(100,40)) ;
//    ll2->drawLine_with_bilv(image, red);
//
//    Line* ll3 = new Line(new Point(80, 40),new Point( 13, 20));
//    ll3->drawLine_with_bilv( image, red);
//
//	Line* ll = new Line(be,end) ;
////	ll->drawLine_cal_much(image,white) ;
//	ll->drawLine_with_bilv(image,red) ;




//
//    ModelAndView a = ModelAndView("/Users/lidan/Desktop/软光栅/tinyrender/testData/lidan.obj") ;
//    vector<TGAColor> one ;
//    vector<TGAColor> two ;
//    a.normal(800,800);
//    a.drawAll(image, one ,two) ;


//    vector<Vec3f*> t1 ;
//    t1.push_back(new Vec3f(10,70,10));
//    t1.push_back(new Vec3f(50,160,10));
//    t1.push_back(new Vec3f(70, 500,10));
//
//
//    float* z_buffer = new float[image.get_width()*image.get_height()] ;
//    for(int i = image.get_width()*image.get_height();i--;z_buffer[i] = -std::numeric_limits<float>::max()) ;
//
//    int index[3] = {0,1,2};
//    Triangle a(index) ;
//    a.draw(image, z_buffer,t1, const_cast<TGAColor &>(red)) ;

/*
    vector<Vec3f> t2 ;
    t2.push_back(Vec3f(140,140,10));
    t2.push_back(Vec3f(50,160,10));
    t2.push_back(Vec3f(70, 80,10));
    int index1[3] = {0,1,2};
    Triangle b(index1) ;
    b.draw(image, t2, const_cast<TGAColor &>(white)) ;
    vector<Vec3f> t3 ;
    t3.push_back(Vec3f(180,140,10));
    t3.push_back(Vec3f(70,160,10));
    t3.push_back(Vec3f(70, 80,10));
    int index2[3] = {0,1,2};
    Triangle c(index2) ;
    c.draw(image, t3, const_cast<TGAColor &>(green)) ;
    vector<Vec3f*> t4 ;
    t4.push_back(new Vec3f(160,160,10));
    t4.push_back(new Vec3f(270,160,10));
    t4.push_back(new Vec3f(160, 80,10));
    int index3[3] = {0,1,2};
    Triangle d(index3) ;
    c.draw(image, t4, const_cast<TGAColor &>(green)) ;
*/



    Render a = Render("C:\\Users\\lidan\\Desktop\\render\\newCpuRender\\testData\\obj\\african_head\\african_head.obj") ;
    a.addTexture("C:\\Users\\lidan\\Desktop\\render\\newCpuRender\\testData\\obj\\african_head\\african_head_diffuse.tga") ;
    vector<TGAColor> one ;
    vector<TGAColor> two ;
    a.setcamera(camera) ;
    a.normal(800,800,center,eye,up);
    a.drawAll(image, one ,two,light_dir) ;


    image.flip_vertically();

    image.write_tga_file("output.tga");
    return 0;
}