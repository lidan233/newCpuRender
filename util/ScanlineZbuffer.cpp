//
// Created by lidan on 2021/1/9.
//

#include "ScanlineZbuffer.h"

ScanlineZbuffer::ScanlineZbuffer(int H, int W)
{
//    szbuffer = Lmatrix<Doub>(1,W) ;
//    idBuffer = Lmatrix<Int>(H,W) ;
    width = W ;
    height = H ;
}

void ScanlineZbuffer::build(ObjData& objData,Vec3f LightView)
{
    polygonTable.clear();
    edgeTable.clear();
    polygonTable.resize(height);
    edgeTable.resize(height);
//    if(szbuffer.mrows()==1) szbuffer.resize(0,0) ;
//    if(idBuffer.mrows()!=height) idBuffer.resize(0,0) ;

    szbuffer = Lmatrix<Doub>(1,width) ;
    idBuffer = Lmatrix<Int>(height,width) ;

    for(int i = 0 ; i < objData.faces_.size() ; i++)
    {
        Vec3i face = objData.faces_[i] ;
        Poly_TriNode polyTriNode ;
        polyTriNode.pid = i ;
//        Vec3f normal ;
        float range_y_max = std::numeric_limits<float>::min() ;
        float range_y_min = std::numeric_limits<float>::max() ;

        Vec3f nn = objData.norms[objData.idxNorm_[i][0]] + objData.norms[objData.idxNorm_[i][0]]+objData.norms[objData.idxNorm_[i][0]] ;
        polyTriNode.norm = nn * 0.3333333 ;

        Vec3f tt1 = objData.verts_[face[0]] ;
        Vec3f tt2 = objData.verts_[face[1]] ;
        Vec3f tt3 = objData.verts_[face[2]] ;

        if(tt1[0]<0 || tt2[0]<0 || tt3[0] < 0 ) continue;
        if(tt1[0]>= height-0.5 || tt2[0]>= height-0.5 || tt3[0]>= height-0.5 ) continue;
        if(tt1[1]<0 || tt2[1]<0 || tt3[1] < 0 ) continue;
        if(tt1[1]>=width-0.5 || tt2[1]>=width-0.5 || tt3[1]>=width-0.5 ) continue;


        // build edge table
        for(int j = 0 ; j < 3 ; j++)
        {
            Vec3f p1 = objData.verts_[face[j]] ;
            Vec3f p2 = objData.verts_[face[(j+1)%3]] ;
            if(p2[1] > p1[1]) swap<3,float>(p1,p2) ;
            if(p1[1] <= 0 || p2[1] >= height-1 ) continue;

            if(p1[1]> height-1)
            {
                double t = double(height- 1- p2[1]) / double(p1[1]-p2[1]) ;
                p1[0] = t * (p1[0]-p2[0]) + p2[0] ;
                p1[1] = t * (p1[1]-p2[1]) + p2[1] ;
                p1[2] = t * (p1[2]-p2[2]) + p2[2] ;
            }

            EdgeNode edge;
            edge.dy = round(p1[1]) - round(p2[1]);
            if (edge.dy <= 0)  continue; // 太短的不算

            edge.left_x = p1[0];
            edge.poly_id = polyTriNode.pid;
            edge.dx = -(p1[0] - p2[0]) / (p1[1] - p2[1]) ;

            edgeTable[round(p1[1])].push_back(edge) ;

            range_y_min = min(p2[1], range_y_min) ;
            range_y_max = max(p1[1], range_y_max) ;
        }

        polyTriNode.linesize = round(range_y_max) - round(range_y_min) ;
//        Vec3f v1 = objData.verts_[face[0]] ;
//        Vec3f v2 = objData.verts_[face[1]] ;
//        Vec3f v3 = objData.verts_[face[2]] ;
//        normal = getNormal(v1,v2,v3) ;

        if(polyTriNode.linesize > 0 && range_y_max >=0 && range_y_min<height)
        {
            Vec3f t = objData.verts_[face[0]];
            polyTriNode.a = polyTriNode.norm.x ;
            polyTriNode.b  = polyTriNode.norm.y ;
            polyTriNode.c = polyTriNode.norm.z ;

            polyTriNode.d = -(polyTriNode.a * t.x + polyTriNode.b * t.y + polyTriNode.c * t.z ) ;
            polygonTable[round(range_y_max)].push_back(polyTriNode) ;
//            std::cout<<"input triangle "<<polyTriNode.norm<<std::endl ;

        }
    }
}

void ScanlineZbuffer::release()
{
    polygonTable.clear();
    edgeTable.clear();
    polygonTable.clear() ;
    edgeTable.clear() ;
//    if(szbuffer.mrows()==1) szbuffer.resize(0,0) ;
//    if(idBuffer.mrows()!=height) idBuffer.resize(0,0) ;

    szbuffer = Lmatrix<Doub>(0,0) ;
    idBuffer = Lmatrix<Int>(0,0) ;
}

static bool edgeSortCmp(const ActiveEdgeNode& lEdge, const ActiveEdgeNode& rEdge)
{
    if (round(lEdge.lx) < round(rEdge.lx))  return true;
    else if (round(lEdge.lx) == round(rEdge.lx))
    {
        if (round(lEdge.ldx) < round(rEdge.ldx))
            return true;
    }
    return false;
}

void ScanlineZbuffer::addEdge(int y, Poly_TriNode* active_polygon)
{
//    std::cout<<y<<" "<<edgeTable[y].size()<<std::endl  ;
    for(auto it= edgeTable[y].begin(); it != edgeTable[y].end(); )
    {
        if (it->poly_id != active_polygon->pid)
        {
            ++it;
            continue;
        }

        ActiveEdgeNode activeEdgeNode ;
        activeEdgeNode.lx = it->left_x ;
        activeEdgeNode.ldx  = it->dx ;
        activeEdgeNode.ldy  = it->dy ;

        if (active_polygon->c == 0)
        {
            activeEdgeNode.z = 0;
            activeEdgeNode.dzx = 0;
            activeEdgeNode.dzy = 0;
        } else
        {
            activeEdgeNode.z   = -(active_polygon->d + active_polygon->a*it->left_x + active_polygon->b*y) / active_polygon->c;
            activeEdgeNode.dzx = -(active_polygon->a / active_polygon->c) ;
            activeEdgeNode.dzy = active_polygon->b / active_polygon->c ;
        }
        active_polygon->activeEdgeTable.push_back(activeEdgeNode);
        it->poly_id = -1;
    }
    std::sort(active_polygon->activeEdgeTable.begin(), active_polygon->activeEdgeTable.end(), edgeSortCmp);

}

void ScanlineZbuffer::run(ObjData& objData,Vec3f LightView, TGAImage& image)
{
    clock_t t = clock();
    activePolygonTable.clear();
    for(int line = height-1; line >=0 ; line--)
    {
        memset(idBuffer[line], -1, sizeof(int)*width) ;
        szbuffer.fill(0,0,1,width,std::numeric_limits<double>::max()) ;

        for (list<Poly_TriNode>::iterator it = polygonTable[line].begin(), end = polygonTable[line].end(); it != end; ++it)
            activePolygonTable.push_back(*it);

        for (int i = 0; i < activePolygonTable.size(); ++i)
        {
            //添加这个polygon的活化edge 在这个扫描线上的
            Poly_TriNode& active_polygon = activePolygonTable[i];
//            std::cout<<"using active polygon"<<active_polygon.norm<<std::endl ;

            addEdge(line, &active_polygon);

            vector<ActiveEdgeNode>& active_edge_table = active_polygon.activeEdgeTable;
            assert(active_edge_table.size() % 2 == 0) ;


            int aesize = active_edge_table.size();
            // 所有活化边开始更新
            for (auto ac = active_edge_table.begin(); ac != active_edge_table.end(); ++ac)
            {

                ActiveEdgeNode& edge1 = *ac;
                ActiveEdgeNode& edge2 = *(++ac);

                float zx = edge1.z;
                int beginx = round(edge1.lx)>=0? round(edge1.lx): 0 ;
                beginx =  beginx>=width? width-1: beginx ;

                int endx = round (edge2.lx) >=0? round(edge2.lx) : 0 ;
                endx = endx>=width?width-1:endx ;

                for (int x = beginx; x < endx; ++x)
                {
//                    assert(x < width) ;

                    if (zx < szbuffer[0][x])
                    {
                        szbuffer[0][x] = zx;
                        idBuffer[line][x] = active_polygon.pid;
                        float t = -(active_polygon.norm*LightView) ;
                        TGAColor color1 = TGAColor(0,0,255)  ;
                        image.set(line,x,color1) ;
//                        std::cout<<active_polygon.norm<<" "<<LightView<<" input color "<<color1<<std::endl ;
                    }
                    zx += edge1.dzx;
                }
                --edge1.ldy;
                --edge2.ldy;
                edge1.lx += edge1.ldx;
                edge2.lx += edge2.ldx;
                edge1.z += edge1.dzx*edge1.ldx + edge1.dzy;
                edge2.z += edge2.dzx*edge2.ldx + edge2.dzy;
            }


            int last = 0;
            for (int j = 0; j < active_edge_table.size(); ++j, ++last)
            {
                while (active_edge_table[j].ldy <= 0)
                {
                    ++j;
                    if (j >= aesize) break;
                }
                if (j >= aesize) break;
                active_edge_table[last] = active_edge_table[j];
            }
            active_edge_table.resize(last);

            --active_polygon.linesize;

        }


        int last = 0;
        for (int i = 0, len = activePolygonTable.size(); i < len; ++i, ++last)
        {
            while (activePolygonTable[i].linesize < 0)
            {
                ++i;
                if (i >= len) break;
            }
            if (i >= len) break;

            activePolygonTable[last] = activePolygonTable[i];
        }
        activePolygonTable.resize(last);
    }
    cout << "ScanLineZbuffer :" << float((clock() - t)) << "ms" << endl;
}
