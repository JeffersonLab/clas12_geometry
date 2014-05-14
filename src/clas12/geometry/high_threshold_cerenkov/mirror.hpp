#ifndef CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_MIRROR_HPP
#define CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_MIRROR_HPP

#include <cstddef>
#include <vector>

#include "geometry/euclid_vector.hpp"
#include "geometry/line_segment.hpp"
#include "geometry/plane.hpp"

namespace clas12
{
namespace geometry
{

class HighThresholdCerenkov;

namespace high_threshold_cerenkov
{

using std::vector;

using ::geometry::euclid_vector;
using ::geometry::line_segment;
using ::geometry::plane;

class Sector;

/**
 **/
class Mirror
{
  public:
    // inline methods
    const Sector& sector() const;

  private:
    Mirror(const Sector* sector, size_t idx);
    Mirror(const Mirror& that, const Sector* sector, size_t idx);

    /// \brief a pointer to the parent sector
    const Sector* _sector;

    /// \brief the index of the mirror in the vector<Mirror>
    /// object held by the Sector parent class
    size_t _idx;

    // inline methods
    /// \brief deleted copy constructor
    Mirror(const Mirror&) = delete;

    /// \brief deleted assignment operator
    Mirror& operator=(const Mirror&) = delete;

    /// \brief the top-level HTCC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::HighThresholdCerenkov;

    /// \brief the "parent" class Sector takes care of
    /// copying this class when the top-level HTCC class's
    /// copy constructor is called.
    friend class Sector;
};

/**
 * \brief get the parent sector object
 * \return constant reference to Mirror::_sector
 **/
inline
const Sector& Mirror::sector() const
{
    return *_sector;
}

} // namespace clas12::geometry::high_threshold_cerenkov
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_MIRROR_HPP

