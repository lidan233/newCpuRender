//
// Created by 李源 on 2019-11-22.
//

#include "triangle.h"


Triangle::Triangle(int index[3]) {
    points_i[0] = index[0] ;
    points_i[1] = index[1] ;
    points_i[2] = index[2] ;
}


void Triangle::draw_vec3f(TGAImage &image, vector<Vec3f>& points, TGAColor &color)
{
    Point* t1 =new Point(points[points_i[0]].x,points[points_i[0]].y) ;
    Point* t2 =  new Point(points[points_i[1]].x,points[points_i[1]].y) ;
    Point* t3 = new Point(points[points_i[2]].x,points[points_i[2]].y) ;

    if(t1->getY()>t2->getY()) {std::swap(t1,t2) ;}
    if(t1->getY()>t3->getY()) {std::swap(t1,t3) ;}
    if(t2->getY()>t3->getY()) {std::swap(t2,t3) ;}


    //t2>t1>t0 ;
    int total_height = t3->getY()-t1->getY() ;
//    for(int i = t1->getY();i<t2->getY();i++)
//    {
//        int segment_height = t2->getY()-t1->getY()+1 ;
//        float alpha = (float)(i-t1->getY())/total_height ;
//        float beta = (float)(i-t1->getY())/segment_height ;
//
//        Vec2i A = Vec2i(t1->getX(),t1->getY())+
//                    (Vec2i(t3->getX(),t3->getY())-Vec2i(t1->getX(),t1->getY()))*alpha ;
//        Vec2i B = Vec2i(t1->getX(),t1->getY())+
//                  (Vec2i(t2->getX(),t2->getY())-Vec2i(t1->getX(),t1->getY()))*beta ;
////        image.set(A.x,i,red) ;
////        image.set(B.x,i,green ) ;
//        if(A.x>B.x)
//        {
//            std::swap(A,B) ;
//        }
//        //now the A is the left of B ;
//        //so from A to B ;
//
//        for(int j = A.x ;j< B.x;j++)
//        {
//            image.set(j,i,color) ;
//        }
//    }
//
//    for(int y = t2->getY();y<t3->getY() ;y++)
//    {
//        int segment_height  = t3->getY()-t2->getY()+1 ;
//        float alpha = (float)(y-t1->getY())/total_height ;
//        float beta = (float)(y-t2->getY())/segment_height ;
//        Vec2i A = Vec2i(t1->getX(),t1->getY())+
//                  (Vec2i(t3->getX(),t3->getY())-Vec2i(t1->getX(),t1->getY()))*alpha ;
//        Vec2i B = Vec2i(t2->getX(),t2->getY())+
//                  (Vec2i(t3->getX(),t3->getY())-Vec2i(t2->getX(),t2->getY()))*beta ;
//        if(A.x>B.x)
//        {
//            std::swap(A,B) ;
//        }
//        for(int j = A.x;j<=B.x;j++)
//        {
//            image.set(j,y,color) ;
//        }
//    }

    for(int i = 0 ;i< total_height;i++)
    {
        bool second_half = i>(t2->getY()-t1->getY())|| t1->getY()== t2->getY() ;
        int segment_height = second_half?t3->getY()-t2->getY():t2->getY()-t1->getY() ;
        float alpha = (float) i/total_height ;
        float beta =(float)(i-(second_half?t2->getY()-t1->getY():0))/segment_height ;

        Vec2i A = Vec2i(t1->getX(),t1->getY()) +
                  Vec2i(t3->getX()-t1->getX(),t3->getY()-t1->getY())*alpha ;
        Vec2i B = second_half?( Vec2i(t2->getX(),t2->getY())+(Vec2i(t3->getX(),t3->getY())-Vec2i(t2->getX(),t2->getY()))*beta)
                             :(Vec2i(t1->getX(),t1->getY())+(Vec2i(t2->getX(),t2->getY())-Vec2i(t1->getX(),t1->getY()))*beta) ;
        if(A.x>B.x) std::swap(A,B) ;


        for(int j =A.x;j<=B.x;j++)
        {
            image.set(j,t1->getY()+i,color) ;
        }


    }
//    Line(t1,t2).drawLine_remove_float_plus(image,color) ;
//    Line(t2,t3).drawLine_remove_float_plus(image,color) ;
//    Line(t3,t1).drawLine_remove_float_plus(image,green) ;
}



void Triangle::draw_vec3i(TGAImage& image,Buffer& zbuffer ,vector<Vec3i>& points,vector<Vec2i>& colorsPosition,Vec3f intensity,TGAImage& textImage)
{
    Vec3i t1 = points[0];
    Vec3i t2 = points[1] ;
    Vec3i t3 = points[2];


    float in1 = intensity[0] ;
    float in2 = intensity[1] ;
    float in3 = intensity[2] ;


    Vec2i c1 = colorsPosition[0] ;
    Vec2i c2 = colorsPosition[1] ;
    Vec2i c3 = colorsPosition[2] ;

    if(t2.y < t1.y) {std::swap(t1,t2) ; std::swap(c1,c2) ; std::swap(in1,in2);}
    if(t1.y > t3.y) {std::swap(t1,t3) ; std::swap(c1,c3) ; std::swap(in1,in3);}
    if(t2.y > t3.y) {std::swap(t2,t3) ; std::swap(c2,c3) ; std::swap(in2,in3);}



    int total_height = t3.y-t1.y ;
    for(int i = 0 ;i< total_height;i++)
    {
        bool second_half = i>(t2.y-t1.y)|| t1.y== t2.y ;
        int segment_height = second_half?t3.y-t2.y:t2.y-t1.y ;
        float alpha = (float) i/total_height ;
        float beta =(float)(i-(second_half?t2.y-t1.y:0))/segment_height ;

        Vec3i A = t1+Vec3f(t3-t1)*alpha ;
        Vec3i B = second_half? t2+Vec3f(t3-t2)*beta:t1+Vec3f(t2-t1)*beta ;

        Vec2i uvA = c1+Vec2i(c3-c1)*alpha ;
        Vec2i uvB = second_half?c2+Vec2i(c3-c2)*beta:c1+Vec2i(c2-c1)*beta ;

        float inA = in1+(in3-in1)*alpha ;
        float inB = second_half?in2+(in3-in2)*beta:in1+(in2-in1)*beta ;


        if(A.x>B.x){ std::swap(A,B) ; std::swap(uvA,uvB) ;std::swap(inA,inB) ;}



        for(int j =A.x;j<=B.x;j++)
        {
            float phi = B.x==A.x?1.0f:float(j-A.x)/float(B.x-A.x) ;
            Vec3i P = Vec3f(A)+Vec3f(B-A)*phi ;
            Vec2i uvP = uvA + (uvB-uvA)*phi ;
            float inP = inA+(inB-inA)*phi ;
//            int idx =  P.x+P.y*image.get_width() ;
            if(P.x>=image.get_width()||P.y>=image.get_height()||P.x<0||P.y<0) continue ;
            if(zbuffer[P.x][P.y]<P.z)
            {
                zbuffer[P.x][P.y] = P.z ;
                TGAColor color = textImage.get(uvP.x,uvP.y) ;

                image.set(P.x,P.y,color*inP) ;
//                std::cout<<inP ;
//                std::cout<<P.x<<" "<<P.y<<" "<<int(color.bgra[0]*inP)<<int(color.bgra[1]*inP)<<int(color.bgra[2]*inP)<<std::endl ;
//                std::cout<<P.x<<" "<<P.y<<" "<<int(color.bgra[0])<<int(color.bgra[1])<<int(color.bgra[2])<<std::endl ;
//                image.set(P.x,P.y,TGAColor((unsigned char)(color.r),(unsigned char)(color.g),(unsigned char)(color.b))) ;
//                image.set(P.x,P.y,TGAColor((unsigned char)(255*intensity),(unsigned char)(255*intensity),(unsigned char)(255*intensity))) ;
            }
        }


    }
}



void Triangle::draw(TGAImage& image,Buffer& zbuffer ,vector<Vec3f>& points,vector<Vec2i>& colorsPosition,float intensity ,TGAImage& textImage)
{
    std::pair<Vec2f,Vec2f> bound = getMBR( image,points_i,points) ;
    Vec2f xmin = bound.first ;
    Vec2f xmax = bound.second ;

    vector<Point*> temp ;
    temp.push_back(new Point(points[points_i[0]].x,points[points_i[0]].y)) ;
    temp.push_back(new Point(points[points_i[1]].x,points[points_i[1]].y)) ;
    temp.push_back(new Point(points[points_i[2]].x,points[points_i[2]].y)) ;


    Vec3f p ;
    for(p.x = xmin.x;p.x<=xmax.x ;p.x++)
    {
        for(p.y=xmin.y;p.y<=xmax.y;p.y++)
        {
            Vec3f bscreen = barycentric(temp,new Point(p.x,p.y));
            if(bscreen.x<0||bscreen.y<0||bscreen.z<0) continue ;

            vector<TGAColor> colors  ;
            for(int i = 0 ;i<colorsPosition.size();i++)
            {
                colors.push_back(textImage.get(colorsPosition[i][0],colorsPosition[i][1])) ;
            }
            TGAColor temp = interpolateColor(colors,bscreen) ;

            p.z = 0 ;
            for(int i = 0; i<3 ;i++) p.z += points[points_i[i]].z*bscreen[i];
            if(zbuffer[int(p.x)][int(p.y)]<p.z)
            {
                zbuffer[int(p.x)][int(p.y)] = p.z ;
                image.set(p.x,p.y,multiply(temp,intensity)) ;
            }

        }
    }

}