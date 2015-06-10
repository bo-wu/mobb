/*
 * =====================================================================================
 *
 *       Filename:  box_assist.h      Created:  06/10/2015 11:21:33 PM
 *
 *    Description:  
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */
#ifndef BOX_ASSIST_H_
#define BOX_ASSIST_H_
#include <Eigen/Dense>
#include "Box.h"
// test if point inside box
bool point_inside_box(const Eigen::Vector3d &point, const Geom::Box &box);

#endif

