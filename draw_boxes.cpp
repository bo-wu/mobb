/*
 * =====================================================================================
 *
 *       Filename:  draw_boxes.cpp      Created:  05/29/2015 11:28:57 PM
 *
 *    Description:  draw segments boxes
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */
#include "draw_boxes.h"

DrawBoxes::DrawBoxes(QVector<Geom::MinOBB*> _mobb_vec) : mobb_vec(_mobb_vec)
{

}

std::vector<PolygonSoup> DrawBoxes::fill_polygonsoup_vec(QColor color)
{
    for(auto mobb : mobb_vec)
    {
        ps_vec.push_back(mobb.mMinBox.get_PolygonSoup(color));
    }
    return ps_vec;
}

// copy from PolygonSoup
BoxSoup::draw()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);

    //drawTris();
    draw_quads();

    glDisable(GL_LIGHTING);

    // Draw borders as lines and points to force drawing something
    glLineWidth(2.0f);
    glColor4d(0,0,0,1);
    glBegin(GL_LINES);
    for (QVector<QVector3> poly : polys){
        for(int i = 0; i < (int) poly.size(); i++){
            glVertQt(poly[i]);
            glVertQt(poly[(i+1) % poly.size()]);
        }
    }
    glEnd();

    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (QVector<QVector3> poly : polys){
        for(int i = 0; i < (int) poly.size(); i++)
            glVertQt(poly[i]);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void BoxSoup::draw_quads(bool isColored = true){
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    for(int i = 0; i < (int) polys.size(); i++)
    {
        if(polys[i].size() != 4) continue;
        else{
            if(isColored) glColorQt(polys_colors[i]);
            for(int p = 0; p < 4; p++) glVertQt(polys[i][p]);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

