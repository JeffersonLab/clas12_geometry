#include <cstddef>

#include "geometry/constants.hpp"
#include "clas12/geometry/central_tracker.hpp"

namespace clas12
{
namespace geometry
{
namespace central_tracker
{
namespace barrel_svt
{
using std::cos;
using std::sin;
using std::tan;
using ::geometry::euclid_vector;
using ::geometry::line_segment;

/**
 * \brief constructor.
 *
 * This forces the layer to have a parent Sector
 * object pointer.
 *
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this layer in vector<Layer>
 *             held by parent Sector.
 **/
Layer::Layer(const Sector* sector, const size_t& idx)
: _sector(sector)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the layer to have a parent Sector
 * object pointer.
 *
 * \param [in] that the Layer being copied
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this layer in vector<Layer>
 *             held by parent Sector.
 **/
Layer::Layer(const Layer& that, const Sector* sector, const size_t& idx)
: _activeSenLen(that._activeSenLen)
, _activeSenWid(that._activeSenWid)
, _deadZnSenLen1(that._deadZnSenLen1)
, _deadZnSenLen2(that._deadZnSenLen2)
, _deadZnSenLen3(that._deadZnSenLen3)
, _deadZnSenWid(that._deadZnSenWid)
, _endAngle(that._endAngle)
, _idx(idx)
, _physSenLen(that._physSenLen)
, _physSenWid(that._physSenLen)
, _readoutPitch(that._readoutPitch)
, _sector(sector)
, _siliconWidth(that._siliconWidth)
, _startAngle(that._startAngle)
, _strips(that._strips)

{
}

/**
 * \brief radius to this layer
 * \return the region's radius plus any neccessary layer gap.
 **/
double Layer::radius() const
{
    if (_idx == 0)
    {
        return  _sector->region().radius() - 0.5 * _siliconWidth;
    }
    else
    {
        return  _sector->region().radius() + 0.5 * _siliconWidth;
    }
}

/**
 * Calculate the endpoints of silicon strips:
 * Different algorithm for the top and bottom layers
 * Top layer: as seen from the outside of the detector (_idx =1)
 * Bottom layer: underneath the top layer
 * Strip number 1 in the bottom layer is parallel to the bottom of the sector
 * Calculation is done in the local coordinate system:
 * Origin is defined to be at the hybrid sensor implant point of the first strip on the top layer
 * Local_z is in the direction of strip number 1, along the length of the layer
 * From the edge of the hybrid sensor pointing toward the far sensor (local_z is parallel to LAB z)
 * Local_x is along the width of the layer
 * Equation for a strip is a straight line  x = mz+b
**/



/**
 * \brief End point at the hybrid sensor
 * \param [is] is the strip index (starting from zero) in this layer
 *             negative strip index counts from the end.
 * \return (x,y,z) position in sector coordinate system (mm)
 **/
euclid_vector<double,3> Layer::stripFirstPoint(const int& is) const
{
    double b = this->strip_index(is) * _readoutPitch;
    if (_idx == 0)
    {
        b = _activeSenWid - b;
    }

    return euclid_vector<double,3>{b,0,0};
}

/**
 * \brief End point at the far sensor
 * \param [in] is the strip index (starting from zero) in this layer
 * \param [in] is the layer index (0 for the bottom, and 1 for the top layer)
 * \return (x,y,z) position in sector coordinate system (mm)
 **/
euclid_vector<double,3> Layer::stripSecondPoint(const int& is) const
{
    static const double deg2rad = 180./3.1451926536;
    double xval;
    double zval;

    double stripAngle   = this->strip_index(is)*_endAngle/(this->nstrips()-1);
    double stripInterX  = this->strip_index(is)*_readoutPitch;
    double centerLength = 3*_activeSenLen+_deadZnSenLen2+_deadZnSenLen3;
    if (_idx == 1) //top
    {
        double m = tan(stripAngle*deg2rad);
        double b = stripInterX;
        if( (centerLength*m+b) > _activeSenWid)
        {
            xval =  double(_activeSenWid);
            zval = (xval-b)/m;
        }
        else
        {
            zval = centerLength;
            xval = m*zval+b;
        }
    }
    else if (_idx == 0) // bottom
    {
        double m = -tan(stripAngle * deg2rad);
        double b = _activeSenWid - stripInterX;
        if(centerLength*m+b < 0)
        {
            xval = 0;
            zval = -b/m;
        }
        else
        {
            zval = centerLength;
            xval = m*zval+b;
        }
    }

    clog << _idx<< "  "<<is<<"  "<<xval<<"  "<<zval<<endl;
    return euclid_vector<double,3>{xval,0,zval};
}


/**
     * \brief Rotate and translate the end points to the LAB frame
**/
euclid_vector<double,3> Layer::TransformToLAB(const euclid_vector<double,3>& point , const double& gapfactor) const
{

  //reset local X-coord to the middle of the hybrid sensor
   double local_x  = point.x()-0.5*_activeSenWid;


// global rotation angle
   double sector2lab_angle = _sector->phi() + 0.5 *3.14159265359 ;

// gap between the layers
   double gap = _idx * (_sector->layergap() + _siliconWidth);

//  translation parameters
   double tx = (this->radius()+ gap*gapfactor)*cos(_sector->phi());
   double ty = (this->radius()+ gap*gapfactor)*sin(_sector->phi());
   double tz = _sector->region().zstart()+0.5*_deadZnSenLen2;

// Endpoints in the LAB frame
   double xval =   local_x*cos(sector2lab_angle) - point.y()*sin(sector2lab_angle) +tx;
   double yval =   local_x*sin(sector2lab_angle) + point.y()*cos(sector2lab_angle) +ty;
   double zval =   point.z() + tz;
   euclid_vector<double,3> v {xval,yval,zval};

   clog<<" rotated "<< xval<< "  "<<yval<<"  "<<zval<<endl;

   //euclid_vector<double,3> tv {tx,ty,tz};
   //euclid_vector<double,3> v = point;
   //v.phi(v.phi() + sector2lab_angle) += tv;


   return v;

}




/**
 * \brief 3D line segment representing a silicon strip (in the LAB frame)
 * \param [in] is the strip index (starting from zero) in this layer
 * \return line segment with end-points in sector coordinate system (mm)
 **/
line_segment<double,3> Layer::siliconStrip(const int& is) const
{

    //return line_segment<double,3>{
      //  this->stripFirstPoint(is),
       // this->stripSecondPoint(is) };

    return line_segment<double,3>{
        this->TransformToLAB(stripFirstPoint(is),0),

        this->TransformToLAB(stripSecondPoint(is),0)};


}

vector<line_segment<double,3>> Layer::siliconStrips() const
{
    vector<line_segment<double,3>> ret;
    for (int i=0; i<this->nstrips(); i++)
    {
        ret.push_back(this->siliconStrip(i));
    }
    return ret;
}





} /* namespace clas12::geometry::central_tracker::barrel_svt */
} /* namespace clas12::geometry::central_tracker */
} /* namespace clas12::geometry */
} /* namespace clas12 */
