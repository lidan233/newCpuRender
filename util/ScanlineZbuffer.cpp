//
// Created by lidan on 2021/1/9.
//

#include "ScanlineZbuffer.h"

ScanlineZbuffer::ScanlineZbuffer(int H, int W, ObjData& objData)
{
    //clear
    polygonTable.clear();
    edgeTable.clear();
    polygonTable.resize(height);
    edgeTable.resize(height);

    for(int i = 0 ; i < objData.faces_.size() ; i++)
    {
        Vec3i face = objData.faces_[i] ;
        Poly_TriNode polyTriNode ;
        polyTriNode.pid = i ;
        Vec3f normal ;
        float range_y_max = std::numeric_limits<float>::min() ;
        float range_y_min = std::numeric_limits<float>::max() ;


        // build edge table
        for(int j = 0 ; j < 3 ; j++)
        {
            Vec3f p1 = objData.verts_[face[i]] ;
            Vec3f p2 = objData.verts_[face[(i+1)%3]] ;
            if(p2[1] > p1[1]) swap<3,float>(p1,p2) ;
            if(p1[1] <= 0 || p2[1] >= height ) continue;

            if(p1[1]> height)
            {
                double t = double(height-p2[1]) / double(p1[1]-p2[1]) ;
                p1[1] = t * (p1[0]-p2[0]) + p2[0] ;
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
        Vec3f v1 = objData.verts_[face[0]] ;
        Vec3f v2 = objData.verts_[face[1]] ;
        Vec3f v3 = objData.verts_[face[2]] ;
        normal = getNormal(v1,v2,v3) ;

        if(polyTriNode.linesize > 0 && range_y_max >0 && range_y_min<height)
        {
            Vec3f t = objData.verts_[face[0]];
            polyTriNode.a = normal.x ;
            polyTriNode.b  = normal.y  ;
            polyTriNode.c = normal.z ;
            polyTriNode.d = -(polyTriNode.a * normal.x + polyTriNode.b * normal.y + polyTriNode.c * normal.z ) ;
            polygonTable[round(range_y_max)].push_back(polyTriNode) ;
        }
    }
}

static bool edgeSortCmp(const ActiveEdgeNode& lEdge, const ActiveEdgeNode& rEdge)
{
    if (round(lEdge.lx) < round(rEdge.lx))return true;
    else if (round(lEdge.lx) == round(rEdge.lx))
    {
        if (round(lEdge.ldx) < round(rEdge.ldx))
            return true;
    }
    return false;
}


void ScanlineZbuffer::addEdge(int y, Poly_TriNode* active_polygon)
{
    for(list<EdgeNode>::iterator it= edgeTable[y].begin(); it != edgeTable[y].end(); it++)
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
            activeEdgeNode.z   = -(active_polygon->d + active_polygon->a*it->x + active_polygon->b*y) / active_polygon->c;
            activeEdgeNode.dzx = -(active_polygon->a / active_polygon->c) ;
            activeEdgeNode.dzy = active_polygon->b / active_polygon->c ;
        }
        active_polygon->activeEdgeTable.push_back(activeEdgeNode);
        it->poly_id = -1;
    }
    std::sort(active_polygon->activeEdgeTable.begin(), active_polygon->activeEdgeTable.end(), edgeSortCmp);
}

void ScanlineZbuffer::run(ObjData& objData)
{
    memset(idBuffer[y], -1, sizeof(int)*width);
    fill(zBuffer, zBuffer + width, -0xfffffff);


    //检查分类的多边形表，如果有新的多边形涉及该扫描线，则把它放入活化的多边形表中
    for (list<Polygon>::iterator it = polygonTable[y].begin(),
                 end = polygonTable[y].end(); it != end; ++it)
        activePolygonTable.push_back(*it);

    int apsize = activePolygonTable.size();
#pragma omp parallel for //num_threads(NUM_THREADS)
    for (int i = 0; i < apsize; ++i)
    {
        Polygon& active_polygon = activePolygonTable[i];
        addEdge(y, &active_polygon);

        vector<ActiveEdge>& active_edge_table = active_polygon.activeEdgeTable;
        assert(active_edge_table.size() % 2 == 0);//假设都是凸多边形

        int aesize = active_edge_table.size();
        //for (int j = 0; j < aesize; ++j)
        for (vector<ActiveEdge>::iterator ae_it = active_edge_table.begin(), end = active_edge_table.end();
             ae_it != end; ++ae_it)
        {
            //取出活化边对
            ActiveEdge& edge1 = *ae_it;
            ActiveEdge& edge2 = *(++ae_it);

            float zx = edge1.z;
            //更新z-buffer和id缓冲器
            for (int x = round(edge1.x), end = round(edge2.x); x < end; ++x)
            {
                if (zx > zBuffer[x])
                {
                    zBuffer[x] = zx;
                    idBuffer[y][x] = active_polygon.id;
                }
                zx += edge1.dzx;
            }
            //进入下一条扫描线，更新活化边参数
            --edge1.dy;
            --edge2.dy;
            edge1.x += edge1.dx;
            edge2.x += edge2.dx;
            edge1.z += edge1.dzx*edge1.dx + edge1.dzy;
            edge2.z += edge2.dzx*edge2.dx + edge2.dzy;
        }

        //移除已经结束的活化边
        int last = 0;
        aesize = active_edge_table.size();
        for (int j = 0; j < aesize; ++j, ++last)
        {
            while (active_edge_table[j].dy <= 0)
            {
                ++j;
                if (j >= aesize)break;
            }
            if (j >= aesize)break;
            active_edge_table[last] = active_edge_table[j];
        }
        active_edge_table.resize(last);

        --active_polygon.dy;//活化多边形扫描线向下移动
    }

    //移除已经完成的活化多边形
    int last = 0;
    for (int i = 0, len = activePolygonTable.size(); i < len; ++i, ++last)
    {
        while (activePolygonTable[i].dy < 0)
        {
            ++i;
            if (i >= len) break;
        }
        if (i >= len) break;

        activePolygonTable[last] = activePolygonTable[i];
    }
    activePolygonTable.resize(last);
}
