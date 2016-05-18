/*******************************************************************************
 *  Project: libopencad
 *  Purpose: OpenSource CAD formats support library
 *  Author: Alexandr Borzykh, mush3d at gmail.com
 *  Author: Dmitry Baryshnikov, bishop.dev@gmail.com
 *  Language: C++
 *******************************************************************************
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2016 Alexandr Borzykh
 *  Copyright (c) 2016 NextGIS, <info@nextgis.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *******************************************************************************/

#ifndef CADGEOMETRIES_H
#define CADGEOMETRIES_H

#include <vector>
#include <string>
#include <stdint.h>
#include "cadobjects.h"
#include "cadfile.h"
#include "simpledatatypes.h"

/**
 * @brief Base CAD geometry class
 */
class CADGeometry
{
 public:
    /**
     * @brief The CAD geometry types enum
     */
    enum CADGeometryType
    {
        POINT = 1,
        CIRCLE,
        LWPOLYLINE,
        ELLIPSE,
        LINE,
        POLYLINE3D,
        TEXT,
        ARC,
        SPLINE
    };

public: //TODO: protected
    CADGeometryType stGeometryType;
    double          dfThickness;
};

/* TODO: Point3D should be named Point, but because of possible redefenitions
 * its named Point3D
 * */
/**
 * @brief Geometry class which a single Point
 */
class Point3D : public CADGeometry
{
public:
    Point3D () : dfXAxisAng (0.0f)
    {
        stGeometryType = POINT;
    }

    Vertex3D vertPosition;
    Vector3D vectExtrusion;
    double dfXAxisAng;
};

/**
 * @brief Geometry class which represents a simple Line
 */
class Line : public CADGeometry
{
public:
    Line()
    {
        stGeometryType = LINE;
    }

    Vertex3D vertStart;
    Vertex3D vertEnd;
    Vector3D vectExtrusion;
};

/**
 * @brief Geometry class which represents Lwpolyline
 */
class LWPolyline : public CADGeometry
{
public:
    LWPolyline () : dfConstWidth(0.0f),
                    dfElevation(0.0f)
    {
        stGeometryType = LWPOLYLINE;
    }

    double dfConstWidth;
    double dfElevation;
    Vector3D vectExtrusion;
    std::vector< Vertex2D > vertexes;
    std::vector< double > bulges;
    std::vector< int16_t > vertexes_id;
    std::vector< std::pair< double, double > > widths; // start, end.
};

/**
 * @brief Geometry class which represents Polyline 3D
 */
class Polyline3D : public CADGeometry
{
public:
    Polyline3D()
    {
        stGeometryType = POLYLINE3D;
    }

    std::vector< Vertex3D > vertexes;
};

/**
 * @brief Geometry class which represents Circle
 */
class Circle : public CADGeometry
{
public:
    Circle () : dfRadius (0.0f)
    {
        stGeometryType = CIRCLE;
    }

    Vertex3D vertPosition;
    Vector3D vectExtrusion;
    double dfRadius;
};

/**
 * @brief Geometry class which represents Ellipse
 */
class Ellipse : public CADGeometry
{
public:
    Ellipse() : dfAxisRatio(0.0f),
                dfStartingAngle(0.0f),
                dfEndingAngle(0.0f)
    {
        stGeometryType = ELLIPSE;
    }

    Vertex3D vertPosition;
    Vector3D vectWCS;
    Vector3D vectExtrusion;
    double dfAxisRatio;
    double dfStartingAngle;
    double dfEndingAngle;
};

/**
 * @brief Geometry class which represents Text
 */
class Text : public CADGeometry
{
public:
    Text() : dfElevation(0.0f),
             dfObliqueAngle(0.0f),
             dfRotationAngle(0.0f),
             dfHeight(0.0f),
             dfWidthFactor(0.0f),
             dGeneration(0),
             dHorizontalAlignment(0),
             dVerticalAlignment(0)
    {
        stGeometryType = TEXT;
    }

    double dfElevation;
    Vertex2D vertInsertion;
    Vertex2D vertAlignment;
    Vector3D vectExtrusion;
    double dfObliqueAngle;
    double dfRotationAngle;
    double dfHeight;
    double dfWidthFactor;
    std::string strTextValue;
    int16_t dGeneration;
    int16_t dHorizontalAlignment;
    int16_t dVerticalAlignment;
};

/**
 * @brief Geometry class which represents Arc
 */
class Arc : public CADGeometry
{
public:
    Arc() : dfRadius(0.0f),
            dfStartingAngle(0.0f),
            dfEndingAngle(0.0f)
    {
        stGeometryType = ARC;
    }

    Vertex3D vertPosition;
    double dfRadius;
    Vector3D vectExtrusion;
    double dfStartingAngle;
    double dfEndingAngle;
};

/**
 * @brief Geometry class which represents Spline
 */
class Spline : public CADGeometry
{
public:
    Spline()
    {
        stGeometryType = SPLINE;
    }

    long dScenario;
    bool bRational;
    bool bClosed;
    bool bPeriodic;
    bool bWeight;

    long   dDegree;
    double dfFitTol;
    double dfKnotTol;
    double dfCtrlTol;
    Vector3D vectBegTangDir;
    Vector3D vectEndTangDir;

    std::vector < double > adfKnots;
    std::vector < double > adfCtrlPointsWeight;
    std::vector < Vertex3D > avertCtrlPoints;
    std::vector < Vertex3D > averFitPoints;
};

class EXTERN Layer
{
public:
    Layer(CADFile * pCADFile)
    {
        pstCADFile_m = pCADFile;
    }

    size_t dLayerID;

    std::string sLayerName;
    bool bFrozen;
    bool bOn;
    bool bFrozenByDefaultInNewVPORT;
    bool bLocked;
    bool bPlottingFlag;
    int16_t dLineWeight;
    int16_t dColor;

    CADFile * pstCADFile_m;

    size_t GetGeometriesCount() { return astAttachedGeometries.size(); }

    CADGeometry * GetGeometry( size_t index )
    {
        return pstCADFile_m->GetGeometry ( dLayerID, index );
    }

    std::vector < std::pair < long long, short > > astAttachedGeometries;
};


#endif // CADGEOMETRIES_H