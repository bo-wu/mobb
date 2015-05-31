/*
 * =====================================================================================
 *
 *       Filename:  compute_segmesh_mobb.cpp      Created:  05/27/2015 04:19:09 PM
 *
 *    Description:  compute segment mini obb
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */

#include "CustomDrawObjects.h"
#include "compute_segmesh_mobb.h"
#include "XmlWriter.h"
//#include "draw_boxes.h"
#define GUI_DRAWING 1

void Filter_mobb::initParameters(RichParameterSet *pars)
{
    pars->addParam(new RichBool("display_mobb", true, "visualize mini obb"));
    pars->addParam(new RichBool("display_mesh", true, "visualize object mesh"));
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
    QVector<Vector3> all_vertex;
    for (auto part : mesh_segment_vec)
    {
        vertex_soup.clear();
        Vector3VertexProperty points = part->vertex_coordinates();
        foreach(Vertex v, part->vertices())
        {
            vertex_soup.push_back(points[v]);
            all_vertex.push_back(points[v]);
        }
        //Geom::MinOBB temp_mobb(vertex_soup, false);
        Geom::MinOBB temp_mobb(vertex_soup, true);
        segment_mobb_vec.push_back(temp_mobb);
    }
    global_mobb = Geom::MinOBB(all_vertex, true);

    write_mobb_xml();
}

// write mobb into xml file
void Filter_mobb::write_mobb_xml()
{
    auto *model = document()->selectedModel();
    auto name = (model->path).section(".", 0, 0);
    //qDebug()<<qPrintable(name.append(".xml"));
    name.append(".xml");
    QFile file(name);
    if(!file.open(QIODevice::WriteOnly)) 
    {
        qDebug()<<"cannot open file for writing xml";
        return;
    }
    XmlWriter xw(&file);
    xw.setAutoNewLine(true);
    xw.writeRaw("<!--?xml Version = \"1.0\"?-->\n");
    global_mobb.mMinBox.write(xw);
    foreach(auto mobb, segment_mobb_vec)
    {
        mobb.mMinBox.write(xw);
    }
    file.close();
}


void Filter_mobb::applyFilter(RichParameterSet *pars)
{
    if (!is_computed)
    {
        compute_mobb();
        is_computed = true;
    }

    QColor color = Qt::cyan;
    color.setAlphaF(0.3);
#ifdef GUI_DRAWING
    drawArea()->clear();
    if(pars && pars->getBool("display_mobb"))
    {
        /*  
        int seg_idx = pars->getInt("segment_index");
        if (seg_idx >= segment_mobb_vec.size())
        {
            seg_idx = segment_mobb_vec.size() - 1;
        }
        qDebug()<<"segment index "<<seg_idx;

        PolygonSoup *ps = new PolygonSoup;
        for(QVector<Vector3> f : segment_mobb_vec[seg_idx].mMinBox.getFacePoints())
        {
            ps->addPoly(f, color);
        }
        drawArea()->addRenderObject(ps);
        */

        for (auto mobb : segment_mobb_vec)
        {
            PolygonSoup *ps = new PolygonSoup; 
            for(QVector<Vector3> f : mobb.mMinBox.getFacePoints())
            {
                ps->addPoly(f, color);
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
