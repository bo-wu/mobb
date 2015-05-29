/*
 * =====================================================================================
 *
 *       Filename:  compute_segmesh_mobb.cpp      Created:  05/27/2015 04:19:09 PM
 *
 *    Description:  
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */

#include "CustomDrawObjects.h"
#include "compute_segmesh_mobb.h"
#include "draw_boxes.h"

void Filter_mobb::initParameters(RichParameterSet *pars)
{
    pars->addParam(new RichBool("display_mobb", true, "visualize mini obb"));
    pars->addParam(new RichBool("display_mesh", true, "visualize object mesh"));
    is_computed = false;
}

void Filter_mobb::compute_mobb()
{
    //Starlab::Model *model = document()->selectedModel();
    segmesh_manager = SegMeshLoader(mesh());
    mesh_segment_vec = segmesh_manager.getSegMeshes();

    segment_mobb_vec.clear();
    ps_vec.clear();
    // compute mini volume obb
    QVector<Vector3> vertex_soup;
    for (auto part : mesh_segment_vec)
    {
        vertex_soup.clear();
        Vector3VertexProperty points = part->vertex_coordinates();
        foreach(Vertex v, part->vertices())
        {
            vertex_soup.push_back(points[v]);
        }
        Geom::MinOBB temp_mobb(vertex_soup, false);
        segment_mobb_vec.push_back(temp_mobb);
    }
    for (auto mobb : segment_mobb_vec)
    {
        mobb.mMinBox.get_PolygonSoup(Qt::cyan);
        PolygonSoup ps;
        ps = mobb.mMinBox.get_PolygonSoup(Qt::cyan);
        ps_vec.push_back(ps);
    }
    
}

void Filter_mobb::draw_mobb()
{
    drawArea()->clear();
    for(auto ps : ps_vec)
    {
        drawArea()->addRenderObject(&ps);
    }
    drawArea()->drawAllRenderObjects();
}


void Filter_mobb::applyFilter(RichParameterSet *pars)
{
    if (!is_computed)
    {
        compute_mobb();
        is_computed = true;
    }
    drawArea()->clear();
    if(pars && pars->getBool("display_mobb"))
    {
        draw_mobb();
    }
    if(pars && pars->getBool("display_mesh"))
    {
        //qDebug()<<"mesh visible "<<mesh()->isVisible;
        mesh()->isVisible = !mesh()->isVisible;
    }
}

Q_EXPORT_PLUGIN(Filter_mobb)
