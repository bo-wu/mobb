/*
 * =====================================================================================
 *
 *       Filename:  draw_box.h      Created:  05/29/2015 02:46:27 PM
 *
 *    Description: draw boxes 
 *
 *         Author:  Wu Bo (Robert), wubo.gfkd@gmail.com
 *		Copyright:	Copyright (c) 2015, Wu Bo
 *   Organization:  National University of Defense Technology
 *
 * =====================================================================================
 */
#ifndef DRAW_BOX_H_
#define DRAW_BOX_H_
#include <vector>
#include "RenderObject.h"
#include "CustomDrawObjects.h"
#include "Box.h"

class DrawBoxes
{
public:
    DrawBoxes(QVector<Geom::MinOBB*>);
    std::vector fill_polygonsoup_vec(QColor color=Qt::cyan);
private:
    QVector<Geom::MinOBB*> mobb_vec;
    std::vector<PolygonSoup> ps_vec;
};

/*
class DrawBoxes : public RenderObject::Base
{
public:
    DrawBoxes():RenderObject::Base(1, Qt::red){}
    DrawBoxes(QVector<Geom::Box> _box_vec) : RenderObject::Base(1, Qt::red)
    {
        box_vec = _box_vec;
    }
    virtual void draw(QGLWidget& widget)
    {
        Q_UNUSED(widget)

        //QuickMeshDraw::drawMeshSharpEdges(mesh, QColor(1,0,0), 6);
        //QuickMeshDraw::drawMeshSolid(mesh, QColor(1,0,0));
        //QuickMeshDraw::drawMeshWireFrame(mesh, QColor(1,0,0));
    }

    void add_box(const Geom::Box box, const QColor& c = Qt::blue)
    {
        box_vec.push_back(box);
    }
    void draw_sharp_edges();

private:
    QVector<Geom::Box> box_vec;

};
*/
#endif

