include($$[STARLAB])
include($$[SURFACEMESH])
StarlabTemplate(plugin)

DEPENDPATH += $$PWD
CONFIG(debug, debug|release) {
    CFG = debug
} else {
    CFG = release
}

LIB += -L$$PWD/../../UtilityLib/$$CFG/lib -lUtilityLib
INCLUDEPATH += $$PWD/../../UtilityLib

LIBS += -L$$PWD/../../GeometryLib/$$CFG/lib -lGeometryLib
INCLUDEPATH += $$PWD/../../GeometryLib

LIB += -L$$PWD/../../StructureLib/$$CFG/lib -lStructureLib
INCLUDEPATH += $$PWD/../../StructureLib

LIBS += -L$$PWD/../../MeshUtilityLib/$$CFG/lib -lMeshUtilityLib
INCLUDEPATH += $$PWD/../../MeshUtilityLib

INCLUDEPATH += $$PWD/../../../libigl/include/
INCLUDEPATH += $$PWD/../../../libigl/include/igl/cgal/
INCLUDEPATH += /usr/local/include/eigen3/
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lpugixml
LIBS += -L/home/wubo/workspace/libigl/lib -ligl -liglcgal

HEADERS += \ 
    compute_segmesh_mobb.h \
    ../../UtilityLib/CustomDrawObjects.h \
    skeleton.h \
    box_assist.h
SOURCES += \
    compute_segmesh_mobb.cpp \
    skeleton.cpp \
    box_assist.cpp
