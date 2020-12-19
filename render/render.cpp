//
// Created by lidan on 18/12/2020.
//

#include "render.h"
#include "Line.h"
using namespace Vec ;

Render::Render(const string path)
{
    ObjLoader loader(path) ;
    objData = loader.getData() ;
}
Render::~Render()
{

}
int Render::getVertexSize(){return objData.verts_.size() ;}
int Render::getFacesSize() {return objData.faces_.size() ;}
Vec3f Render::getVertex(int n) {return objData.verts_[n] ;}


Vec3i Render::getFace(int idx){return objData.faces_[idx] ;}
Vec3i Render::getFaceText(int idx){ return objData.idxTex_[idx] ;}
Vec3i Render::getFaceVertexNormIndex(int idx){ return objData.idxNorm_[idx] ;}

void Render::drawAll(TGAImage& image ,vector<TGAColor> v_c,vector<TGAColor> f_c,Vec3f light_dir)
{
    zBuffer = new ZBuffer(image.get_height(),image.get_width()) ;

    srand(time(NULL) ) ;
    int size = getFacesSize() ;
    for(int i = 0; i< size;i++)
    {
        Vec3i face = getFace(i) ;
        isnormal = false ;
        vector<Vec3i> t4 ;
        Vec3f intensity;
        vector<Vec3f> norms = getFaceVectexNorm(i) ;

        for(int i = 0; i< 3;i++)
        {
            t4.push_back(normal_v[face[i]]);
            intensity[i] = norms[i]*light_dir ;

        }
        vector<Vec2i> T_Texts  = getFaceVertexTexturePosition(i);
        int index3[3] = {0,1,2};
        Triangle d(index3) ;
        d.draw_vec3i(image,*zBuffer, t4,T_Texts,intensity,text ) ;
    }
}


void Render::normal(int height,int width,Vec3f center,Vec3f eye,Vec3f up)
{
    isnormal = true ;
    for(int i = 0; i<objData.verts_.size();i++)
    {
        Vec3i temp ;
        temp[0] = objData.verts_[i][0] ;
        temp[1] = objData.verts_[i][1] ;
        temp[2] = objData.verts_[i][2] ;
        normal_v.push_back(temp) ;
    }
    normalVertexs(objData.verts_,normal_v,height,width,camera,eye,center,up) ;
}

void Render::addTexture(string path)
{
    text.read_tga_file(path.c_str()) ;
    text.flip_vertically();
    T_width = text.get_width() ;
    T_height = text.get_height() ;
    useTexture = true ;
}

TGAColor Render::getTextureColor(Vec2i point )
{
    Vec2i p ;
    p[0] = point[0]*T_width ;
    p[1] = point[1]*T_height ;

    return text.get(p[0],p[1]) ;
}

vector<Vec2i> Render::getFaceVertexTexturePosition(int face_i )
{
    assert(face_i<getFacesSize()) ;
    Vec3i idx_v = getFaceText(face_i) ;
    vector<Vec2i> result  ;
    for(int i = 0; i <3 ;i++)
    {
        Vec2i temp = Vec2i(objData.texture[idx_v[i]][0]*text.get_width(),objData.texture[idx_v[i]][1]*text.get_height()) ;
        result.push_back(temp) ;
    }
    return result ;
}

vector<Vec3f> Render::getFaceVectexNorm(int face_i)
{
    assert(face_i<getFacesSize()) ;
    Vec3i idx_v = getFaceVertexNormIndex(face_i);
    vector<Vec3f> norms ;
    for(int i =0 ;i <3 ;i++)
    {
        Vec3f temp = objData.norms[idx_v[i]] ;
        norms.push_back(temp.normalize()) ;
    }
    return norms;
}

void Render::setcamera(Vec3f camera)
{
    this->camera = camera ;
}
