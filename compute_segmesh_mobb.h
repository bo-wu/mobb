/*
 * =====================================================================================
 *
 *       Filename:  compute_segmesh_mobb.h      Created:  05/27/2015 04:19:03 PM
 *
 *    Description:  filter for mini volume obb
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */
#ifndef COMPUTE_SEGMESH_MOBB_H_
#define COMPUTE_SEGMESH_MOBB_H_
#include <vector>
#include "Eigen/Dense"
#include "MinOBB.h"
#include "SegMeshLoader.h"
#include "SurfaceMeshPlugins.h"
#include "CustomDrawObjects.h"

class Filter_mobb : public SurfaceMeshFilterPlugin
{
    Q_OBJECT
    Q_INTERFACES(FilterPlugin)
public:
    QString name() { return "Compute MOBB";}
    QString description() { return "Compute MOBB for segmented mesh"; }

    ~Filter_mobb()
    {
        qDeleteAll(mesh_segment_vec);
        mesh_segment_vec.clear();
        segment_mobb_vec.clear();
        //qDeleteAll(ps_vec);
    }
    void initParameters(RichParameterSet *pars);
    void applyFilter(RichParameterSet *pars);
    void compute_mobb();
private:
    bool is_computed;
    bool testing;
    SegMeshLoader segmesh_manager;
    QVector<SurfaceMeshModel*> mesh_segment_vec;
    QVector<Geom::MinOBB> segment_mobb_vec;
};

#endif

