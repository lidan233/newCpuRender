//
// Created by 李源 on 2019-11-22.
//

#ifndef COMMON
#define COMMON


#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "manipulation.h"
#include "tgaimage.h"
#include "triangle.h"
#include "Vec.h"

#include <tuple>

using namespace std;


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,255,0,255) ;
const TGAColor blue = TGAColor(0,0,255,255) ;


const TGAColor colors[] = {white,red,green,blue};



inline string ltrim(string data) {
    string::iterator p = find_if(data.begin(), data.end(), not1(ptr_fun<int, int>(isspace)));
    data.erase(data.begin(), p);
    return data;
}

inline string rtrim(string data)
{
    string::reverse_iterator p = find_if(data.rbegin(),data.rend(),not1(ptr_fun<int,int>(isspace))) ;
    data.erase(p.base(),data.end()) ;
    return data ;
}

inline string trim(string data)
{
    return ltrim(rtrim(data)) ;
}

inline vector<string> getstringObj(string data )
{
    vector<string> result ;
    data = trim(data) ;
    stringstream ss ;
    ss<<data.c_str()[0] ;
    result.push_back(ss.str()) ;
    ss.str("") ;

    for(int i = 1; i<data.size();i++)
    {
        while(data.c_str()[i]==' '||data.c_str()[i]=='\t'){i++ ;}
        if(data.c_str()[i-1]==' '||data.c_str()[i-1]=='\t')
        {
            string temp = ss.str() ;
            if(ss.str()!="")
                result.push_back(temp) ;
            ss.str("");
        }
        ss<<data.c_str()[i] ;
    }
    result.push_back(ss.str()) ;
    return result ;
}

class ObjData{
public:
    std::vector<Vec3f> verts_;
    std::vector<Vec3i> faces_;
    std::vector<Vec3i> idxTex_ ;
    std::vector<Vec2f> texture ;
    std::vector<Vec3f> norms ;
    std::vector<Vec3i> idxNorm_ ;
};

inline void readObj(const string filename,vector<Vec3f>& verts_,vector<vector<int> >& faces_,vector<vector<int> >& idxTex_,vector<Vec2f>& texture,vector<Vec3f>& norms,vector<vector<int> >& idxNorm_) {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            verts_.push_back(v);
        }else if(!line.compare(0,3,"vt "))
        {
            Vec2f t;
            float temp ;
            iss >> trash>>trash ;
            for(int i = 0; i <2 ;i++)
            {
                iss>>temp;
                t[i]= temp ;
            }
            texture.push_back(t) ;
        }else if(!line.compare(0,3,"vn "))
        {
            Vec3f n;
            float temp ;
            iss >> trash>>trash ;
            for(int i = 0; i <3 ;i++)
            {
                iss>>temp;
                n[i]= temp ;
            }
            norms.push_back(n) ;
        }else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            std::vector<int> idxTex ;
            std::vector<int> idxNorm ;
            //we not find trash
            int iTex,iNorm, idx;
            iss >> trash;
            //vertex/uv/normal
            while (iss >> idx >> trash >> iTex >> trash >> iNorm) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back(idx);
                idxTex.push_back(--iTex) ;
                idxNorm.push_back(--iNorm) ;
            }
            faces_.push_back(f);
            idxTex_.push_back(idxTex) ;
            idxNorm_.push_back(idxNorm) ;
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

inline void normalVertexs(vector<Vec3f>& s_verts ,vector<Vec3i>& verts_,int height,int width,Vec3f camera,Vec3f eye,Vec3f center,Vec3f up)
{
    // 平移+旋转 平移到center 对应的verts_此时还是在-1到1之间的local坐标  然后我们转换一下坐标系
    Matrix44 lookat = manipulation::lookAt(eye,center,up) ;
    Matrix44 viewport = manipulation::viewport(width/8,height/8,width*3/4,height*3/4,255) ;
    Matrix44 project = manipulation::projection((eye-center).norm()) ;

    std::cerr<<lookat<<endl ;
    std::cerr<<viewport<<endl ;
    std::cerr<<project<<endl ;

    Matrix44 fuck = viewport*project*lookat ;
    std::cerr<<fuck<<endl ;
    for(int i = 0 ;i< verts_.size();i++)
    {
        Matrix41 screen = manipulation::v2m(s_verts[i]) ;
        verts_[i] = manipulation::m2v(viewport*project*lookat*screen) ;
    }

}

inline double round(double src,int precise) {
    stringstream ss;
    ss << std::fixed << setprecision(precise) << src;
    ss >> src;
    return src ;
}
inline Vec3f barycentric(vector<Point*> points,Point* p)
{
    Vec3f all_x = Vec3f(points[2]->getX()-points[0]->getX(),
                        points[1]->getX()-points[0]->getX(),
                        points[0]->getX()-p->getX()) ;
    Vec3f all_y = Vec3f(points[2]->getY()-points[0]->getY(),
                        points[1]->getY()-points[0]->getY(),
                        points[0]->getY()-p->getY()) ;
    Vec3f result = cross(all_x,all_y) ;

    //根据重心坐标系推出0存在 xy拆分两个方程 然后=0 定义为垂直两个向量 X乘
    //这里结果中如果两个接近共线则认为是0 则三角形被认为是退化三角形
    if(abs(result[2])>1e-2)
    {
        return Vec3f(1.f-(result.x+result.y)/result.z,result.y/result.z,result.x/result.z) ;
    }
    return Vec3f(-1,1,1);
}
inline std::pair<Vec2f,Vec2f> getMBR(TGAImage& image,int* points_i,vector<Vec3f> points)
{
    Vec2f xmin(std::numeric_limits<float>::max(),std::numeric_limits<float>::max()) ;
    Vec2f xmax(-std::numeric_limits<float>::max(),-std::numeric_limits<float>::max()) ;
    Vec2f clamp(image.get_width()-1,image.get_height()-1) ;

    for(int i = 0;i<3;i++)
    {

        xmin.x = std::max(0.0f,std::min(points[points_i[i]].x,(float)xmin[0]))  ;
        xmin.y = std::max(0.0f,std::min(points[points_i[i]].y,(float)xmin[1])) ;
        xmax.x = std::min((float)clamp[0],std::max((float)xmax[0],points[points_i[i]].x)) ;
        xmax.y = std::min((float)clamp[1],std::max((float)xmax[1],points[points_i[i]].y)) ;
    }


    return make_pair(xmin,xmax) ;
}

inline TGAColor& multiply(TGAColor& from ,const float val)
{
    TGAColor to ;
    for(int i = 0;i< 3;i++)
    {

        to.bgra[i] = (char)(((float)from.bgra[i])*val) ;
    }
    to.bgra[3] = 255 ;
    to.bytespp = 3;
    return to ;
}
inline TGAColor& interpolateColor(vector<TGAColor> colors,Vec3f baryCoordinates )
{
    TGAColor result ;
    for(int i =0 ;i < 4;i++)
    {
        for(int j = 0; j<3 ;j++)
        {
            result.bgra[i] +=  colors[j].bgra[i]*baryCoordinates[j] ;
        }
    }
    return result ;
}

#endif