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
#include "RenderObject.h"
#include "compute_segmesh_mobb.h"
#include "XmlWriter.h"
#define GUI_DRAWING 1

void Filter_mobb::initParameters(RichParameterSet *pars)
{
    pars->addParam(new RichBool("display_mobb", true, "visualize mini obb"));
    pars->addParam(new RichBool("display_mesh", false, "visualize object mesh"));
    pars->addParam(new RichBool("display_skel", true, "visualize box skeleton"));
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
    int i = -1;
    possibility.clear();
    for(pugi::xml_node box : doc.children("box"))
    {
        //first box is overall bounding box
        if (i < 0)
            i++;
        else
        {
            if (box.attribute("appearence"))
            {
                int app = box.attribute("appearence").as_int();
                int tot = box.attribute("total").as_int();
                qDebug()<<app<<"/"<<tot;
                if(tot > 0)
                {
                    possibility.push_back(double(app)/double(tot));
                }
                else
                    possibility.push_back(0.0);
            }
        }
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
        skeleton_to_box();
        is_computed = true;
    }
    
    if (possibility.size() != box_vec.size()-1)
    {
        std::cout<<"maybe error in file format\n";
    }
    QColor color; // = Qt::cyan;
    color.setAlphaF(0.3);
#ifdef GUI_DRAWING
    drawArea()->clear();
    if (pars && pars->getBool("display_skel"))
    {
        QVector3 q0, q1;
        LineSegments *ls = new LineSegments(3.0);
        QVector< QVector<Vector3> > box_skel_ponits;
        box_skel_ponits.clear();
        int i = -1;
        for(auto box : box_vec)
        {
            if(i < 0)
            {
                ++i;
                continue;
            }
            color = qtJetColorMap(possibility.at(i));
            for(auto line_segs :  box_edge_vec.at(i))
            {
                Vector3 temp_p0 = skel.vertices.at(line_segs(0));
                Vector3 temp_p1 = skel.vertices.at(line_segs(1));
                q0 = QVector3(temp_p0(0), temp_p0(1), temp_p0(2));
                q1 = QVector3(temp_p1(0), temp_p1(1), temp_p1(2));
                ls->addLine(q0, q1, color);
            }
            /*
            int aid;
            box.Extent.maxCoeff(&aid);
            auto s = box.getSkeleton(aid);
            auto pnts = s.getUniformSamples(20);
            box_skel_ponits.push_back(pnts);
            for(int n=0; n < pnts.size(); ++n)
                for(int m=n+1; m < pnts.size(); ++m)
                {
                    Vector3 temp_p0 = pnts.at(n);
                    Vector3 temp_p1 = pnts.at(m);
                    q0 = QVector3(temp_p0(0), temp_p0(1), temp_p0(2));
                    q1 = QVector3(temp_p1(0), temp_p1(1), temp_p1(2));
                    ls->addLine(q0, q1, color);
                }
                */
            /*  
            q0 = QVector3(s.P0(0), s.P0(1), s.P0(2));
            q1 = QVector3(s.P1(0), s.P1(1), s.P1(2));
            ls->addLine(q0, q1, color);
            */
            ++i;
        }
        drawArea()->addRenderObject(ls);
    }

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
        //draw each box from xml
        int seg_idx = pars->getInt("segment_index");
        if(seg_idx >= box_vec.size()-1)
            seg_idx = box_vec.size()-2;
        color = qtJetColorMap(possibility.at(seg_idx));
        color.setAlphaF(0.3);
        PolygonSoup *ps = new PolygonSoup;
        for(QVector<Vector3> f : box_vec[seg_idx+1].getFacePoints())
        {
            ps->addPoly(f, color);
        }
        drawArea()->addRenderObject(ps);
        */

          //draw all the box from xml
        int i = -1;
        for (auto mobb : box_vec)
        {
            if (i < 0)
            {
                ++i;
                continue;
            }
            color = qtJetColorMap(possibility.at(i));
            color.setAlphaF(0.8);
            PolygonSoup *ps = new PolygonSoup; 
            for(QVector <Vector3> f : mobb.getFacePoints())
            {
                ps->addPoly(f, color);
            }
            drawArea()->addRenderObject(ps);
            ++i;
        }
        drawArea()->drawAllRenderObjects();
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

QColor Filter_mobb::qtJetColorMap(double value, double min, double max)
{
    unsigned char rgb[3];
    unsigned char c1=144;
    float max4=(max-min)/4;
    value-=min;
    if(value==HUGE_VAL)
    {rgb[0]=rgb[1]=rgb[2]=255;}
    else if(value < 0)
    {rgb[0]=rgb[1]=rgb[2]=0;}
    else if(value < max4)
    {rgb[0]=0;rgb[1]=0;rgb[2]=c1+(unsigned char)((255-c1)*value/max4);}
    else if(value < 2*max4)
    {rgb[0]=0;rgb[1]=(unsigned char)(255*(value-max4)/max4);rgb[2]=255;}
    else if(value < 3*max4)
    {rgb[0]=(unsigned char)(255*(value-2*max4)/max4);rgb[1]=255;rgb[2]=255-rgb[0];}
    else if(value < max)
    {rgb[0]=255;rgb[1]=(unsigned char)(255-255*(value-3*max4)/max4);rgb[2]=0;}
    else {rgb[0]=255;rgb[1]=rgb[2]=0;}
    return QColor(rgb[0],rgb[1],rgb[2]);
}


std::vector<std::vector<Eigen::Vector2i> > Filter_mobb::skeleton_to_box()
{
    auto *model = document()->selectedModel();
    auto skel_name = (model->path).section(".", 0, 0);
    skel_name.append("_merge_ckel.cg");
    skel.clear_skel();
    skel.read_skel(qPrintable(skel_name));
    std::list<Eigen::Vector2i> edge_list(skel.edges.begin(), skel.edges.end());
    std::vector<std::vector<Eigen::Vector2i> > box_edge;
    box_edge.clear();
    int i = -1;
    for(auto box : box_vec)
    {
        if (i == -1)
        {
            ++i;
            continue;
        }
        std::vector<Vector2i> edge;
        edge.clear();
        std::list<Eigen::Vector2i>::iterator it = edge_list.begin();
        while(it != edge_list.end())
        {
            if( point_inside_box(skel.vertices.at((*it)(0)), box) && point_inside_box(skel.vertices.at((*it)(1)), box) )
            {
                edge.push_back(*it);
                it = edge_list.erase(it);
            }
            else
            {
                ++it;
            }
        }
        box_edge.push_back(edge);
    }
    box_edge_vec.clear();
    box_edge_vec = box_edge;
    return box_edge;
}


Q_EXPORT_PLUGIN(Filter_mobb)
