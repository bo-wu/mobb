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

std::vector DrawBoxes::fill_polygonsoup_vec(QColor color)
{
    for(auto mobb : mobb_vec)
    {
        ps_vec.push_back(mobb->mMinBox.get_PolygonSoup(color));
    }
    return ps_vec;
}
