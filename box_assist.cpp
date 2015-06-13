/*
 * =====================================================================================
 *
 *       Filename:  box_assist.cpp      Created:  06/10/2015 11:54:52 PM
 *
 *    Description:  
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */
#include <cmath>
#include "box_assist.h"

bool point_inside_box(const Eigen::Vector3d &point, const Geom::Box &box)
{
    auto p2b = point - box.Center;
    bool result = true;
    for(int i=0; i < 3; ++i)
    {
        float dist = fabs( p2b.dot(box.Axis[i]) );
        if(dist > box.Extent(i))
        {
            result = false;
            break;
        }
    }
    return result;
}
