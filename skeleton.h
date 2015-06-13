/*
 * =====================================================================================
 *
 *       Filename:  skeleton.h      Created:  06/10/2015 09:56:46 AM
 *
 *    Description:  read skeleton
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */
#ifndef SKELETON_H_
#define SKELETON_H_
#include <Eigen/Dense>
#include <vector>
#include <string>

class Skeleton
{
public:
    Skeleton(){}
    std::vector<Eigen::Vector3d> vertices;
    std::vector<Eigen::Vector2i> edges;
    void clear_skel();
    void read_skel(const std::string);
};

#endif

