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

#include <pugixml.hpp>
#include <iostream>
#include <sstream>
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


std::vector<Geom::Box> Filter_mobb::parse_xml(QString fname)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(qPrintable(fname));
    std::cout<<"Load "<<qPrintable(fname)<<" results: "<<result.description()<<std::endl;
    std::vector<Geom::Box> model_boxes;
    //read boxes for model: should be fix format xml
    std::vector<double> param_value;
    for(pugi::xml_node box : doc.children("box"))
    {
        Eigen::Vector3d c;
        QVector<Eigen::Vector3d> axis;
        Eigen::Vector3d ext;
        param_value.clear();
        for(pugi::xml_node param : box.children())
        {
            std::string value_string(param.text().get());
            std::istringstream ss(value_string);
            double temp;
            while(ss>>temp)
            {
                param_value.push_back(temp);
            }
        }
        c << param_value.at(0), param_value.at(1), param_value.at(2);
        ext << param_value.at(12), param_value.at(13), param_value.at(14);
        Eigen::Vector3d temp;
        for(int i=1; i<=3; ++i)
        {
            temp << param_value.at(i*3+0), param_value.at(i*3+1), param_value.at(i*3+2);
            axis.push_back(temp);
        }
        model_boxes.push_back(Geom::Box(c, axis, ext));
    }
    return model_boxes;
}


void Filter_mobb::read_mobb_xml()
{

}


void Filter_mobb::applyFilter(RichParameterSet *pars)
{
    
    if (!is_computed)
    {
        //compute_mobb();
        auto *model = document()->selectedModel();
        auto xml_name = (model->path).section(".", 0, 0);
        xml_name.append(".xml");
        box_vec = parse_xml(xml_name);
        is_computed = true;
    }

    QColor color = Qt::cyan;
    color.setAlphaF(0.3);
#ifdef GUI_DRAWING
    drawArea()->clear();
    if(pars && pars->getBool("display_mobb"))
    {
        /*  //draw each index 
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
        //draw each box from xml
        int seg_idx = pars->getInt("segment_index");
        if(seg_idx >= box_vec.size()-1)
            seg_idx = box_vec.size()-2;
        PolygonSoup *ps = new PolygonSoup;
        for(QVector<Vector3> f : box_vec[seg_idx+1].getFacePoints())
        {
            ps->addPoly(f, color);
        }
        drawArea()->addRenderObject(ps);
        /* //draw all boxes
        for (auto mobb : segment_mobb_vec)
        {
            PolygonSoup *ps = new PolygonSoup; 
            for(QVector<Vector3> f : mobb.mMinBox.getFacePoints())
            {
                ps->addPoly(f, color);
            }
            drawArea()->addRenderObject(ps);
        }
        */
        /*  
        int i = 0;
        for (auto mobb : box_vec)
        {
            if (i == 0)
            {
                ++i;
                continue;
            }
            PolygonSoup *ps = new PolygonSoup; 
            for(QVector<Vector3> f : mobb.getFacePoints())
            {
                ps->addPoly(f, color);
            }
            drawArea()->addRenderObject(ps);
        }
        drawArea()->drawAllRenderObjects();
        */
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
