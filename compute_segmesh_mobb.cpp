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
#include "QuickMeshDraw.h"
#include "compute_segmesh_mobb.h"
//#include "draw_box.h"

void Filter_mobb::initParameters(RichParameterSet *pars)
{
    pars->addParam(new RichInt("segmesh_index", 0, "Part Index"));
    pars->addParam(new RichBool("display_mobb", true, "visualize mini obb"));
    pars->addParam(new RichBool("display_mesh", true, "visualize object mesh"));
    is_computed = false;
}

void Filter_mobb::compute_mobb()
{
    //Starlab::Model *model = document()->selectedModel();
    segmesh_manager = SegMeshLoader(mesh());
    mesh_segment_vec = segmesh_manager.getSegMeshes();

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
    int segmesh_index = pars->getInt("segmesh_index");
    if (!is_computed)
    {
        compute_mobb();
        is_computed = true;
    }
    if (segmesh_index >= mesh_segment_vec.size() )
        segmesh_index %= mesh_segment_vec.size();

    drawArea()->clear();
    DrawBox *db = new DrawBox;
    if(pars && pars->getBool("display_mobb"))
    {
        for(auto mesh_seg : mesh_segment_vec)
        {
            db->mesh = mesh_seg;
            drawArea()->addRenderObject(db);
        }
        drawArea()->drawAllRenderObjects();
    }
    if(pars && pars->getBool("display_mesh"))
    {
        //qDebug()<<"mesh visible "<<mesh()->isVisible;
     //   mesh()->isVisible = !mesh()->isVisible;
    }
}

Q_EXPORT_PLUGIN(Filter_mobb)
