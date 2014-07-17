#ifndef CLAS12_GEOMETRY_OUTPUT_EC_SECTOR_VOLUMES_HPP
#define CLAS12_GEOMETRY_OUTPUT_EC_SECTOR_VOLUMES_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <algorithm>
#include <cstddef>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pugixml.hpp"

#include "geometry.hpp"
#include "geometry/constants.hpp"

#include "clas12/geometry/electromagnetic_cal.hpp"

namespace clas12
{
namespace geometry
{
namespace output
{

using std::map;
using std::string;
using std::copy;
using std::inserter;

using pugi::xml_document;
using pugi::xml_node;
using pugi::node_pcdata;

typedef map<string, map<string,string> > volmap_t;


/**
 * \brief generate the volumes of an EC Sector for input into gemc/geant4
 *
 * The numbers for the mother volumes are calculated following Geant4's G4TRAP constructor:
 *
 *     pDz     Half-length along the z-axis
 *     pTheta  Polar angle of the line joining the centres of the faces at -/+pDz
 *     pPhi    Azimuthal angle of the line joing the centre of the face at -pDz to the centre *             of the face at+pDz
 *     pDy1    Half-length along y of the face at -pDz
 *     pDx1    Half-length along x of the side at y=-pDy1 of the face at -pDz
 *     pDx2    Half-length along x of the side at y=+pDy1 of the face at -pDz
 *     pAlp1   Angle with respect to the y axis from the centre of the side at y=-pDy1 to the *              centre at y=+pDy1 of the face at -pDz
 *     pDy2    Half-length along y of the face at +pDz
 *     pDx3    Half-length along x of the side at y=-pDy2 of the face at +pDz
 *     pDx4    Half-length along x of the side at y=+pDy2 of the face at +pDz
 *     pAlp2   Angle with respect to the y axis from the centre of the side at y=-pDy2  *         to the centre aty=+pDy2 of the face at +pDz
 * \return map of map of strings: ret[volume_name][param_name] = value
**/


volmap_t ec_volumes_map(const ElectromagneticCal& ec)
{
    using namespace std;
    using namespace ::geometry;

    static const double double_tolerance = 1.e-8;

    // everything in clas12/geometry is in radians, but gemc
    // seems to want degrees.
    static const double rad2deg = 180. / cons::pi<double>();
    static const double pi      = cons::pi<double>();
    static const double halfpi  = 0.5* cons::pi<double>();

    double theta,phi;
    double alp1,alp2;
    double dx1_mom;
    double dx2_mom;
    double dx3_mom;
    double dx4_mom;
    double dy1_mom;
    double dy2_mom;
    double dz_mom;


    volmap_t vols;
    // constructing the mother volume, 1 for each sector
    for (size_t sec=0; sec<ec.sectors().size(); sec++)
    {
        const electromagnetic_cal::Sector& sector = ec.sector(sec);

        stringstream sector_name_ss;
        sector_name_ss << "sec" << sector.index()+1;
        string ec_sector_name = sector_name_ss.str();

        stringstream ec_desc;
        ec_desc << "Electromagnetic Calorimeter"
                    << " Sector " << sector.index()+1;


        //define some variables for convenience
        size_t total_layers  =  sector.nlayers()*sector.layer(0).nviews();
        const size_t Nstrips = sector.layer(0).view(0).nstrips();

        double alpha     = sector.layer(0).view_angle();
        double two_alpha = 2*sector.layer(0).view_angle();
        double gamma     =  tan(0.5*(1./tan(pi-two_alpha)-1/tan(alpha)));


        double ec_thickness = total_layers * sector.layer(0).strip_thick()
                       + (total_layers-1)    * sector.layer(0).lead_thick()
                       + (total_layers*2)    * sector.layer(0).wrapper_thick();




        dx1_mom = 0.00001;
        dy1_mom = sector.a1()*(total_layers-1)+
                  sector.a3()*(total_layers-1)+sector.a2();
        dx2_mom = 2*(sector.a3()*(total_layers-1)+sector.a2()) / tan(alpha);
        dz_mom  = 0.5 * ec_thickness;
        dx3_mom = dx1;
        dx4_mom = dx2_mom;
        dy2_mom = dy1_mom;
        theta   = 0.;
        phi     = 0.;
        alp1    = 0.;
        alp2    = 0.;

        /** L -vector from the target center to the front face of the first scintillator
            and  perpendicular to the face             **/
         euclid_vector<double,3> Lvec{ 0,
                                        sector.dist2tgt()* sin(sector.thtilt()),
                                        sector.dist2tgt()* cos(sector.thtilt())
                                     };

        /**  S - vector from the perpendicular point at L to geometric center of
             CLAS12 EC  layer 1          **/
        euclid_vector<double,3> Svec{ 0,
                                     -sector.dist2cnt()* cos(sector.thtilt()),
                                      sector.dist2cnt()* sin(sector.thtilt())
                                    };

        /** vector from center of front face to the midpoint in the z-direction **/
        euclid_vector<double,3> Dvec{ 0,
                                      ec_thickness* sin(sector.thtilt()),
                                      ec_thickness* cos(sector.thtilt())
                                    };
        /** vector from target to center of the mother volume with origin and the
            at the center of the front face  **/
        euclid_vector<double,3> ec_center_sector {Lvec.x()+Svec.x()+Dvec.x(),
                                                  Lvec.y()+Svec.y()+Dvec.y(),
                                                  Lvec.z()+Svec.z()+Dvec.z()-500.0};



        stringstream ec_pos;
        ec_pos << ec_center_sector.x() << "*cm "
                 << ec_center_sector.y() << "*cm "
                 << ec_center_sector.z() << "*cm";

        stringstream ec_rot;
        ec_rot << "ordered: zxy"
                  << " " << -90. - 60.*sector.index() << "*deg"
                  << " " << -90. - sector.thtilt()*rad2deg << "*deg"
                  << " " << 0 << "*deg";

        stringstream ec_dim;
        ec_dim  <<   dz_mom << "*cm "
                      << theta * rad2deg << "*deg "
                      << phi * rad2deg << "*deg "
                      << dy1_mom << "*cm "
                      << dx1_mom << "*cm "
                      << dx2_mom << "*cm "
                      << alp1 * rad2deg << "*deg "
                      << dy2_mom << "*cm "
                      << dx3_mom << "*cm "
                      << dx4_mom << "*cm "
                      << alp2 * rad2deg << "*deg";

        // The EC mother volume
        vols[ec_sector_name] = {
                {"mother", "root"},
                {"description", ec_desc.str()},
                {"pos", ec_pos.str()},
                {"rotation",  ec_rot.str()},
                {"color", "ff1111"},
                {"type", "G4Trap"},
                {"dimensions", ec_dim.str()},
                {"material", "G4_AIR"},
                {"mfield", "no"},
                {"ncopy", "1"},
                {"pMany", "1"},
                {"exist", "1"},
                {"visible", "0"},
                {"style", "0"},
                {"sensitivity", "no"},
                {"hit_type", ""},
                {"identifiers", ""}
        };


        for (size_t lyr=0; lyr<sector.nlayers()-1; lyr++)
        {
        /** make lead volumes (one for each layer), following  the G4Trap constructor:
            pDz     Half-length along the z-axis
            pTheta  Polar angle of the line joining the centres of the faces at -/+pDz
            pPhi    Azimuthal angle of the line joing the centre of the face at -pDz to the centre of the face at+pDz
            pDy1    Half-length along y of the face at -pDz
            pDx1    Half-length along x of the side at y=-pDy1 of the face at -pDz
            pDx2    Half-length along x of the side at y=+pDy1 of the face at -pDz
            pAlp1   Angle with respect to the y axis from the centre of the side at y=-pDy1 to the centre at y=+pDy1 of the face at -pDz
            pDy2    Half-length along y of the face at +pDz
            pDx3    Half-length along x of the side at y=-pDy2 of the face at +pDz
            pDx4    Half-length along x of the side at y=+pDy2 of the face at +pDz
            pAlp2   Angle with respect to the y axis from the centre of the side at y=-pDy2 to the centre at y=+pDy2 of the face at +pDz
        **/

            //Some lead layers are truncated at the small angle vertex (mm)
            vector<double> LeadVertexCut(nlayers-1,0.00001);
            LeadVertexCut(2)  = 1.440;
            LeadVertexCut(5)  = 1.448;
            LeadVertexCut(8)  = 1.445;
            LeadVertexCut(11) = 1.462;

            for (size_t l=14; l<sector.nlayers()-1; l+=3)
            {
                LeadVertexCut(l) = 1.0;
            }



            double dz;
            double theta;
            double phi;
            double dy1;
            double dx1;
            double dx2;
            double alp1;
            double dy2;
            double dx3;
            double dx4;
            double alp2;

            dz    = 0.5*(sector.layer(0).lead_thick());
            theta = 0;
            phi   = 0;
            dy1   = sector.a3()*(double(lyr)) + sector.a2() - LeadVertexCut(lyr);
            dx1   = LeadVertexCut(lyr)*tan(0.5*(pi-two_alpha));
            dx2   = dx2_mom;
            dy2   = dy_mom;
            dx3   = dx1_mom;
            dx4   = dx2_mom;
            alp1  = 0;
            alp2  = 0;


            // posz = position of the lead layer volume relative to the mother volume (EC mom)
            double z = - ec_thickness/2.
                         + sector.layer(0).lead_thick()* double(lyr+0.5)
                         + sector.layer(0).strip_thick()*double((lyr+1.))
                         + sector.layer(0).wrapper_thick()*double((2*(lyr+1.)));

            double ylead  = sector.a1()*(double(lyr)) + 0.5 * LeadVertexCut(lyr); // taken from Mike's code

            euclid_vector<> posz = {0,ylead,z};

            stringstream leadlayer_name_ss;
            leadlayer_name_ss << "sec" << sector.index()+1 << "_lead_layer" << lyr+1;
            string leadlayer_name = leadlayer_name_ss.str();

            stringstream leadlayer_desc;
            leadlayer_desc << "Electromagnetic Calorimeter"<< " Sector " << sector.index()+1 << " lead, Layer " << lyr +1;

            stringstream leadlayer_pos;
            leadlayer_pos << posz.x() << "*cm "<< posz.y() << "*cm "<< posz.z() << "*cm";

            stringstream leadlayer_rot;
            leadlayer_rot << "0*deg 0*deg 0*deg";

            stringstream leadlayer_dim;
            leadlayer_dim << dz << "*cm "
                      << theta * rad2deg << "*deg "
                      << phi * rad2deg << "*deg "
                      << dy1 << "*cm "
                      << dx1 << "*cm "
                      << dx2 << "*cm "
                      << alp1 * rad2deg << "*deg "
                      << dy2 << "*cm "
                      << dx3 << "*cm "
                      << dx4 << "*cm "
                      << alp2 * rad2deg << "*deg";

            stringstream leadlayer_ids;
            leadlayer_ids    << "sector ncopy 0 lead manual , layer " << lyr+1 ;

            // The Lead Layer volume
            vols[leadlayer_name] = {
              {"mother", ec_sector_name},
              {"description", leadlayer_desc.str()},
              {"pos", leadlayer_pos.str()},
              {"rotation", leadlayer_rot.str()},
              {"color", "ff6633"},
              {"type", "G4Trap"},
              {"dimensions", leadlayer_dim.str()},
              {"material", "Lead"},
              {"mfield", "no"},
              {"ncopy", "1"},
              {"pMany", "1"},
              {"exist", "1"},
              {"visible", "1"},
              {"style", "1"},
              {"sensitivity", "no"},
              {"hit_type", ""},
              {"identifiers", leadlayer_ids.str()}
            };
        }

        for (size_t lyr=0; lyr<sector.nlayers(); lyr++)
        {
      /* make U mother volumes (one for each layer), following  the G4Trap constructor*/



          double dz;
          double theta;
          double phi;
          double dy1;
          double dx1;
          double dx2;
          double alp1;
          double dy2;
          double dx3;
          double dx4;
          double alp2;

          dz    = (sector.layer(0).strip_thick()+sector.layer(0).wrapper_thick()*2)/2.;
          theta = 0;
          phi   = 0;
          dy1   = dy_mom;
          dx1   = dx1_mom;
          dx2   = dx2_mom;
          dy2   = dy_mom;
          dx3   = dx1_mom;
          dx4   = dx2_mom;
          alp1  = 0;
          alp2  = 0;

          // posz = position of the  U layer volume relative to the mother volume (PCAL mom)
          double z = - ec_thick_no_windows/2.
                     + sector.layer(0).lead_thick()* double(lyr*3.)
                     + sector.layer(0).strip_thick()*double((3.*(lyr+1)-2.5))
                     + sector.layer(0).wrapper_thick()*double((6*(lyr+1)-5)) ;



          euclid_vector<> posz = {0,0,z};

          stringstream ulayer_name_ss;
          ulayer_name_ss << "sec" << sector.index()+1 << "_U_view_layer" << lyr+1;
          string ulayer_name = ulayer_name_ss.str();

          stringstream ulayer_desc;
          ulayer_desc << "Electromagnetic Calorimeter"<< " Sector " << sector.index()+1 << " U view, Layer " << lyr +1;

          stringstream ulayer_pos;
          ulayer_pos << posz.x() << "*cm "<< posz.y() << "*cm "<< posz.z() << "*cm";

          stringstream ulayer_rot;
          ulayer_rot << "0*deg 0*deg 0*deg";

          stringstream ulayer_dim;
          ulayer_dim << dz << "*cm "
                     << theta * rad2deg << "*deg "
                     << phi * rad2deg << "*deg "
                     << dy1 << "*cm "
                     << dx1 << "*cm "
                     << dx2 << "*cm "
                     << alp1 * rad2deg << "*deg "
                     << dy2 << "*cm "
                     << dx3 << "*cm "
                     << dx4 << "*cm "
                     << alp2 * rad2deg << "*deg";

           stringstream ulayer_ids;
           ulayer_ids    << "sector ncopy 0 U View manual , layer " << lyr+1 ;

            // The (U) Layer volume
           vols[ulayer_name] = {
              {"mother", ec_sector_name},
              {"description", ulayer_desc.str()},
              {"pos", ulayer_pos.str()},
              {"rotation", ulayer_rot.str()},
              {"color", "ff6633"},
              {"type", "G4Trap"},
              {"dimensions", ulayer_dim.str()},
              {"material", "G4_TITANIUM_DIOXIDE"},
              {"mfield", "no"},
              {"ncopy", "1"},
              {"pMany", "1"},
              {"exist", "1"},
              {"visible", "1"},
              {"style", "1"},
              {"sensitivity", "no"},
              {"hit_type", "PCAL"},
              {"identifiers", ulayer_ids.str()}
           };

           for (size_t is=1; is<=sector.layer(0).view(0).nstrips(); is++)
           {
           /* make U view strip volumes (84 of them), following  the G4Trap constructor.*/

            dz    = 0.5*(sector.layer(0).strip_thick());
            theta = 0;
            phi   = 0;
            dy1   = 0.5*(sector.layer(0).strip_width());
            dy2   = dy1;

            dx2   = 0.5*sector.layer(0).view(0).strip_length(is);
            dx4   = dx2;
            /*dx1   = 0.5*(sector.layer(0).view(0).strip_length(is)
                         -2*(sector.layer(0).strip_width()+2*sector.layer(0).wrapper_thick())
                         *tan(halfpi-sector.layer(0).view_angle())
                         +2*sector.layer(0).wrapper_thick());
            */
            dx1   = 0.5*(sector.layer(0).view(0).strip_length(is)
                         -2*(sector.layer(0).strip_width())*tan(halfpi-alpha));


            dx3   = dx1;
            alp1  = 0;
            alp2  = 0;


            /* posy = position of the strip relative to its mother volume (U view)*/
            double y = -0.5*ec_active_uheight + (is-0.5)*sector.layer(0).strip_width();
                                        + (2*is-1)*sector.layer(0).wrapper_thick();

            euclid_vector<> posy = {0,y,0};

            stringstream strip_name_ss;
            strip_name_ss << "sec" << sector.index()+1 << "_U_view_layer" << lyr+1 << "_strip"<<is;
            string strip_name = strip_name_ss.str();

            stringstream strip_desc;
            ulayer_desc << "Electromagnetic Calorimeter"<< " Sector " << sector.index()+1
                        << " U view, Layer " << lyr +1 <<" strip "<< is;

            stringstream strip_pos;
            strip_pos << posy.x() << "*cm "<< posy.y() << "*cm "<< posy.z() << "*cm";

            stringstream strip_rot;
            ulayer_rot << "0*deg 0*deg 0*deg";

            stringstream strip_dim;
            strip_dim << dz << "*cm "
                      << theta * rad2deg << "*deg "
                      << phi * rad2deg << "*deg "
                      << dy1 << "*cm "
                      << dx1 << "*cm "
                      << dx2 << "*cm "
                      << alp1 * rad2deg << "*deg "
                      << dy2 << "*cm "
                      << dx3 << "*cm "
                      << dx4 << "*cm "
                      << alp2 * rad2deg << "*deg";

            stringstream strip_ids;
            strip_ids    << "sector ncopy 0 U View strip manual" << is ;

            // The Uview strip volumes
            vols[strip_name] = {
              {"mother", ulayer_name},
              {"description", strip_desc.str()},
              {"pos", strip_pos.str()},
              {"rotation", strip_rot.str()},
              {"color", "ff6633"},
              {"type", "G4Trap"},
              {"dimensions", strip_dim.str()},
              {"material", "Scintillator"},
              {"mfield", "no"},
              {"ncopy", "1"},
              {"pMany", "1"},
              {"exist", "1"},
              {"visible", "1"},
              {"style", "0"},
              {"sensitivity", "PCAL"},
              {"hit_type", "PCAL"},
              {"identifiers", strip_ids.str()}
            };

         } // loop over  strips in U view
        }   //loop over 5 layers of U view

        for (size_t lyr=0; lyr<sector.nlayers(); lyr++)
        {
      /* make V mother volumes (one for each layer), following  the G4Trap constructor*/

          double dz;
          double theta;
          double phi;
          double dy1;
          double dx1;
          double dx2;
          double alp1;
          double dy2;
          double dx3;
          double dx4;
          double alp2;


          dz    = 0.5*(sector.layer(0).strip_thick()+2*sector.layer(0).wrapper_thick());
          theta = 0;
          phi   = 0;
          dy1   = 0.5*ec_vw_height;
          dx1   = 0.5*(Vmaxlength +2*sector.layer(0).wrapper_thick());
          dx2   = 0.0000001;
          dy2   = dy1;
          dx3   = dx1;
          dx4   = dx2;
          alp1  = -gamma;
          alp2  = alp1;

          // posz = position of the  V layer volume relative to the mother volume (PCAL mom)
          double z = - ec_thick_no_windows/2.
                     + sector.layer(0).lead_thick()* double(lyr*3.+1.)
                     + sector.layer(0).strip_thick()*double((3.*lyr+1.5))
                     + sector.layer(0).wrapper_thick()*double((6*lyr+3));
          double x =  0.25*Umaxlength*cos(two_alpha);
          double y =  0.25*Umaxlength*sin(two_alpha);

          euclid_vector<> posz = {x,y,z};

          stringstream vlayer_name_ss;
          vlayer_name_ss << "sec" << sector.index()+1 << "_V_view_layer" << lyr+1;
          string vlayer_name = vlayer_name_ss.str();

          stringstream vlayer_desc;
          vlayer_desc << "Electromagnetic Calorimeter"<< " Sector " << sector.index()+1 << " V view, Layer " << lyr +1;

          stringstream vlayer_pos;
          vlayer_pos << posz.x() << "*cm "<< posz.y() << "*cm "<< posz.z() << "*cm";

          stringstream vlayer_rot;
          vlayer_rot << "0*deg 0*deg 0*deg";

          stringstream vlayer_dim;
          vlayer_dim << dz << "*cm "
                     << theta * rad2deg << "*deg "
                     << phi * rad2deg << "*deg "
                     << dy1 << "*cm "
                     << dx1 << "*cm "
                     << dx2 << "*cm "
                     << alp1 * rad2deg << "*deg "
                     << dy2 << "*cm "
                     << dx3 << "*cm "
                     << dx4 << "*cm "
                     << alp2 * rad2deg << "*deg";

           stringstream vlayer_ids;
           vlayer_ids    << "sector ncopy 0 V View manual , layer " << lyr+1 ;

            // The (V) Layer volume
           vols[vlayer_name] = {
              {"mother", ec_sector_name},
              {"description", vlayer_desc.str()},
              {"pos", vlayer_pos.str()},
              {"rotation", vlayer_rot.str()},
              {"color", "ff6633"},
              {"type", "G4Trap"},
              {"dimensions", vlayer_dim.str()},
              {"material", "G4_TITANIUM_DIOXIDE"},
              {"mfield", "no"},
              {"ncopy", "1"},
              {"pMany", "1"},
              {"exist", "1"},
              {"visible", "1"},
              {"style", "1"},
              {"sensitivity", "no"},
              {"hit_type", "PCAL"},
              {"identifiers", vlayer_ids.str()}
           };

           for (size_t is=1; is<=sector.layer(0).view(1).nstrips(); is++)
           {
           /* make V view strip volumes (77 of them), following  the G4Trap constructor.*/

            dz    = (sector.layer(0).strip_thick())/2.;
            theta = 0;
            phi   = 0;
            dy1   = 0.5*(sector.layer(0).strip_width());
            dy2   = dy1;

            dx2   = 0.5*(sector.layer(0).view(1).strip_length(is)
                         -(sector.layer(0).strip_width())*(tan(halfpi-alpha)+tan(two_alpha-halfpi)));
            dx4   = dx2;
            dx1   = 0.5*(sector.layer(0).view(1).strip_length(is));

            dx3   = dx1;
            alp1  = -gamma;
            alp2  = alp1;


            /* posy = position of the strip relative to its mother volume (V view)*/
            double y = 0.5*ec_vw_height - (is-0.5)*sector.layer(0).strip_width();
                                        - (2*is-1)*sector.layer(0).wrapper_thick();

            double x = -y*tan(gamma);
            euclid_vector<> posy = {x,y,0};

            stringstream strip_name_ss;
            strip_name_ss << "sec" << sector.index()+1 << "_V_view_layer" << lyr+1 << "_strip"<<is;
            string strip_name = strip_name_ss.str();

            stringstream strip_desc;
            vlayer_desc << "Electromagnetic Calorimeter"<< " Sector " << sector.index()+1
                        << " V view, Layer " << lyr +1 <<" strip "<< is;

            stringstream strip_pos;
            strip_pos << posy.x() << "*cm "<< posy.y() << "*cm "<< posy.z() << "*cm";

            stringstream strip_rot;
            vlayer_rot << "0*deg 0*deg 0*deg";

            stringstream strip_dim;
            strip_dim << dz << "*cm "
                      << theta * rad2deg << "*deg "
                      << phi * rad2deg << "*deg "
                      << dy1 << "*cm "
                      << dx1 << "*cm "
                      << dx2 << "*cm "
                      << alp1 * rad2deg << "*deg "
                      << dy2 << "*cm "
                      << dx3 << "*cm "
                      << dx4 << "*cm "
                      << alp2 * rad2deg << "*deg";

            stringstream strip_ids;
            strip_ids    << "sector ncopy 0 V View strip manual" << is ;

            // The Vview strip volumes
            vols[strip_name] = {
              {"mother", vlayer_name},
              {"description", strip_desc.str()},
              {"pos", strip_pos.str()},
              {"rotation", strip_rot.str()},
              {"color", "ff6633"},
              {"type", "G4Trap"},
              {"dimensions", strip_dim.str()},
              {"material", "Scintillator"},
              {"mfield", "no"},
              {"ncopy", "1"},
              {"pMany", "1"},
              {"exist", "1"},
              {"visible", "1"},
              {"style", "0"},
              {"sensitivity", "PCAL"},
              {"hit_type", "PCAL"},
              {"identifiers", strip_ids.str()}
            };

         } // loop over  strips in V view
        }   //loop over 5 layers of V view

        for (size_t lyr=0; lyr<sector.nlayers(); lyr++)
        {
      /* make W mother volumes (one for each layer), following  the G4Trap constructor*/

          double dz;
          double theta;
          double phi;
          double dy1;
          double dx1;
          double dx2;
          double alp1;
          double dy2;
          double dx3;
          double dx4;
          double alp2;


          dz    = 0.5*(sector.layer(0).strip_thick()+2*sector.layer(0).wrapper_thick());
          theta = 0;
          phi   = 0;
          dy1   = 0.5*ec_vw_height;
          dx1   = 0.5*(Vmaxlength + 2*sector.layer(0).wrapper_thick());
          dx2   = 0.0000001;
          dy2   = dy1;
          dx3   = dx1;
          dx4   = dx2;
          alp1  = -gamma;
          alp2  = alp1;

          // posz = position of the  W layer volume relative to the mother volume (PCAL mom)
          double z = - ec_thick_no_windows/2.
                     + sector.layer(0).lead_thick()* double(lyr*3.+2.)
                     + sector.layer(0).strip_thick()*double((3.*lyr+2.5))
                     + sector.layer(0).wrapper_thick()*double((6*lyr+5));
          double x =  -0.25*Umaxlength*cos(two_alpha);
          double y =  0.25*Umaxlength*sin(two_alpha);

          euclid_vector<> posz = {x,y,z};

          stringstream wlayer_name_ss;
          wlayer_name_ss << "sec" << sector.index()+1 << "_W_view_layer" << lyr+1;
          string wlayer_name = wlayer_name_ss.str();

          stringstream wlayer_desc;
          wlayer_desc << "Electromagnetic Calorimeter"<< " Sector " << sector.index()+1 << " V view, Layer " << lyr +1;

          stringstream wlayer_pos;
          wlayer_pos << posz.x() << "*cm "<< posz.y() << "*cm "<< posz.z() << "*cm";

          stringstream wlayer_rot;
          wlayer_rot << "0*deg 0*deg 0*deg";

          stringstream wlayer_dim;
          wlayer_dim << dz << "*cm "
                     << theta * rad2deg << "*deg "
                     << phi * rad2deg << "*deg "
                     << dy1 << "*cm "
                     << dx1 << "*cm "
                     << dx2 << "*cm "
                     << alp1 * rad2deg << "*deg "
                     << dy2 << "*cm "
                     << dx3 << "*cm "
                     << dx4 << "*cm "
                     << alp2 * rad2deg << "*deg";

           stringstream wlayer_ids;
           wlayer_ids    << "sector ncopy 0 V View manual , layer " << lyr+1 ;

            // The W Layer volume
           vols[wlayer_name] = {
              {"mother", ec_sector_name},
              {"description", wlayer_desc.str()},
              {"pos", wlayer_pos.str()},
              {"rotation", wlayer_rot.str()},
              {"color", "ff6633"},
              {"type", "G4Trap"},
              {"dimensions", wlayer_dim.str()},
              {"material", "G4_TITANIUM_DIOXIDE"},
              {"mfield", "no"},
              {"ncopy", "1"},
              {"pMany", "1"},
              {"exist", "1"},
              {"visible", "1"},
              {"style", "1"},
              {"sensitivity", "no"},
              {"hit_type", "PCAL"},
              {"identifiers", wlayer_ids.str()}
           };

           for (size_t is=1; is<=sector.layer(0).view(2).nstrips(); is++)
           {
           /* make W view strip volumes (77 of them), following  the G4Trap constructor.*/

            dz    = (sector.layer(0).strip_thick())/2.;
            theta = 0;
            phi   = 0;
            dy1   = 0.5*(sector.layer(0).strip_width());
            dy2   = dy1;

            dx2   = 0.5*(sector.layer(0).view(1).strip_length(is)
                         -(sector.layer(0).strip_width())*(tan(halfpi-alpha)+tan(two_alpha-halfpi)));
            dx4   = dx2;
            dx1   = 0.5*(sector.layer(0).view(1).strip_length(is));

            dx3   = dx1;
            alp1  = -gamma;
            alp2  = alp1;


            /* posy = position of the strip relative to its mother volume (W view)*/
            double y = -0.5*ec_active_vheight + (is-0.5)*sector.layer(0).strip_width();
                                        + (2*is-1)*sector.layer(0).wrapper_thick();

            double x = -y*tan(gamma);


            euclid_vector<> posy = {x,y,0};

            stringstream strip_name_ss;
            strip_name_ss << "sec" << sector.index()+1 << "_W_view_layer" << lyr+1 << "_strip"<<is;
            string strip_name = strip_name_ss.str();

            stringstream strip_desc;
            wlayer_desc << "Electromagnetic Calorimeter"<< " Sector " << sector.index()+1
                        << " W view, Layer " << lyr +1 <<" strip "<< is;

            stringstream strip_pos;
            strip_pos << posy.x() << "*cm "<< posy.y() << "*cm "<< posy.z() << "*cm";

            stringstream strip_rot;
            wlayer_rot << "0*deg 0*deg 0*deg";

            stringstream strip_dim;
            strip_dim << dz << "*cm "
                      << theta * rad2deg << "*deg "
                      << phi * rad2deg << "*deg "
                      << dy1 << "*cm "
                      << dx1 << "*cm "
                      << dx2 << "*cm "
                      << alp1 * rad2deg << "*deg "
                      << dy2 << "*cm "
                      << dx3 << "*cm "
                      << dx4 << "*cm "
                      << alp2 * rad2deg << "*deg";

            stringstream strip_ids;
            strip_ids    << "sector ncopy 0 W View strip manual" << is ;

            // The Wview strip volumes
            vols[strip_name] = {
              {"mother", wlayer_name},
              {"description", strip_desc.str()},
              {"pos", strip_pos.str()},
              {"rotation", strip_rot.str()},
              {"color", "ff6633"},
              {"type", "G4Trap"},
              {"dimensions", strip_dim.str()},
              {"material", "Scintillator"},
              {"mfield", "no"},
              {"ncopy", "1"},
              {"pMany", "1"},
              {"exist", "1"},
              {"visible", "1"},
              {"style", "0"},
              {"sensitivity", "PCAL"},
              {"hit_type", "PCAL"},
              {"identifiers", strip_ids.str()}
            };

         } // loop over  strips in W view
        }   //loop over 5 layers of W view





    } // loop over sectors

    return vols;
}

void ec_volumes_xml(xml_document& doc, const ElectromagneticCal& ec)
{
    // start building up the XML document
    xml_node geom_node = doc.child("geometry");
    if (!geom_node)
    {
        geom_node = doc.append_child("geometry");
    }
    xml_node ec_node = geom_node.child("electromagnetic_cal");
    if (!ec_node)
    {
        ec_node = geom_node.append_child("electromagnetic_cal");
    }
    xml_node vol_node = ec_node.child("volumes");
    if (!vol_node)
    {
        vol_node = ec_node.append_child("volumes");
    }

    for (auto k1 : ec_volumes_map(ec))
    {
        xml_node n1 = vol_node.append_child(k1.first.c_str());

        for (auto k2 : k1.second)
        {
            xml_node n2 = n1.append_child(k2.first.c_str());
            n2.append_child(node_pcdata).set_value(k2.second.c_str());
        }
    }
}

} // namespace clas12::geometry::output
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_OUTPUT_PCAL_SECTOR_VOLUMES_HPP
