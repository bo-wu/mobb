/*
 * =====================================================================================
 *
 *       Filename:  compute_segmesh_mobb.h      Created:  05/27/2015 04:19:03 PM
 *
 *    Description:  
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */
#ifndef COMPUTE_SEGMESH_MOBB_H_
#define COMPUTE_SEGMESH_MOBB_H_
#include "MinOBB.h"
#include "SegMeshLoader.h"
#include "SurfaceMeshPlugins.h"

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
        qDeleteAll(segment_mobb_vec);
        segment_mobb_vec.clear();
    }
    void initParameters(RichParameterSet *pars);
    void applyFilter(RichParameterSet *pars);
    void draw_mobb(QColor);
private:
    bool is_computed;
    SegMeshLoader segmesh_manager;
    QVector<SurfaceMeshModel*> mesh_segment_vec;
    QVector<Geom::MinOBB*> segment_mobb_vec;
};

#endif

