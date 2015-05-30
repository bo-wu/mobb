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
#define GUI_DRAWING 1

void Filter_mobb::initParameters(RichParameterSet *pars)
{
    pars->addParam(new RichBool("display_mobb", true, "visualize mini obb"));
    pars->addParam(new RichBool("display_mesh", false, "visualize object mesh"));
    pars->addParam(new RichInt("segment_index", 0, "visualize segment individually"));
    is_computed = false;
    testing = false;
}

void Filter_mobb::compute_mobb()
{
    //Starlab::Model *model = document()->selectedModel();
    segmesh_manager = SegMeshLoader(mesh());
    mesh_segment_vec = segmesh_manager.getSegMeshes();

    segment_mobb_vec.clear();
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
    
}


void Filter_mobb::applyFilter(RichParameterSet *pars)
{
    if (!is_computed)
    {
        compute_mobb();
        is_computed = true;
    }

#ifdef GUI_DRAWING
    if(pars && pars->getBool("display_mobb"))
    {
        for (auto mobb : segment_mobb_vec)
        {
            PolygonSoup *ps = new PolygonSoup; 
            for(QVector<Vector3> f : mobb.mMinBox.getFacePoints())
            {
                ps->addPoly(f, Qt::cyan);
            }
            drawArea()->addRenderObject(ps);
        }
        drawArea()->drawAllRenderObjects();
    }
    else
    {
        drawArea()->clear();
    }

    if(pars && pars->getBool("display_mesh"))
    {
        mesh()->isVisible = true;
    }
    else
    {
        mesh()->isVisible = false;
    }
#endif

}

Q_EXPORT_PLUGIN(Filter_mobb)
