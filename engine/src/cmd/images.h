#ifndef _IMAGES_H
#define _IMAGES_H

#include <string>
#include <vector>
#include "gfx/vec.h"
#include "container.h"
#include "../SharedPool.h"
#include "gfx/sprite.h"
#include "gfx/animation.h"
#include "cargo.h"

struct DockingPorts
{
    struct Type
    {
        enum Value
        {
            CATEGORY_CONNECTED = 10,
            CATEGORY_WAYPOINT = 20,

            // Unconnected types corresponds to the old true/false values
            OUTSIDE = 0,
            INSIDE = 1,

            CONNECTED_OUTSIDE = CATEGORY_CONNECTED + OUTSIDE,
            CONNECTED_INSIDE = CATEGORY_CONNECTED + INSIDE,

            WAYPOINT_OUTSIDE = CATEGORY_WAYPOINT + OUTSIDE,
            WAYPOINT_INSIDE = CATEGORY_WAYPOINT + INSIDE,

            DEFAULT = OUTSIDE
        };

        static bool IsConnected(const Value& type)
        {
            switch (type)
            {
            case OUTSIDE:
            case INSIDE:
                return false;
            default:
                return true;
            }
        }

        static bool IsInside(const Value& type)
        {
            switch (type)
            {
            case INSIDE:
            case CONNECTED_INSIDE:
            case WAYPOINT_INSIDE:
                return true;
            default:
                return false;
            }
        }

        static bool IsWaypoint(const Value& type)
        {
            switch (type)
            {
            case WAYPOINT_OUTSIDE:
            case WAYPOINT_INSIDE:
                return true;
            default:
                return false;
            }
        }
    };

    DockingPorts()
        : radius(0),
          isInside(false),
          isConnected(false),
          isWaypoint(false),
          isOccupied(false)
    {}

    DockingPorts(const Vector &center, float radius, float minradius, const Type::Value& type)
        : center(center),
          radius(radius),
          isInside(Type::IsInside(type)),
          isConnected(Type::IsConnected(type)),
          isWaypoint(Type::IsWaypoint(type)),
          isOccupied(isWaypoint) // Waypoints are always occupied
    {}

    DockingPorts(const Vector &min, const Vector &max, float minradius, const Type::Value& type)
        : center((min + max) / 2.0f),
          radius((max - min).Magnitude() / 2.0f),
          isInside(Type::IsInside(type)),
          isConnected(Type::IsConnected(type)),
          isWaypoint(Type::IsWaypoint(type)),
          isOccupied(isWaypoint) // Waypoints are always occupied
    {}

    float GetRadius() const { return radius; }

    const Vector& GetPosition() const { return center; }

    // Waypoints are always marked as occupied.
    bool IsOccupied() const { return isOccupied; }
    void Occupy(bool yes) { isOccupied = yes; }

    // Does port have connecting waypoints?
    bool IsConnected() const { return isConnected; }

    // Port is located inside or outside the station
    bool IsInside() const { return isInside; }

    bool IsDockable() const { return !isWaypoint; }

private:
    Vector center;
    float radius;
    bool isInside;
    bool isConnected;
    bool isWaypoint;
    bool isOccupied;
};

struct DockedUnits
{
    UnitContainer uc;
    unsigned int  whichdock;
    DockedUnits( Unit *un, unsigned int w ) : uc( un )
        , whichdock( w ) {}
};



class Box;
class VSSprite;
class Animation;

// TODO: why is this a template??
template < typename BOGUS >
//added by chuck starchaser, to try to break dependency to VSSprite in vegaserver
struct UnitImages
{
    UnitImages();

    virtual ~UnitImages();

    StringPool::Reference cockpitImage;
    StringPool::Reference explosion_type;
    Vector CockpitCenter;
    VSSprite     *pHudImage = nullptr;
    ///The explosion starts at null, when activated time explode is incremented and ends at null
    Animation    *pExplosion = nullptr;
    float timeexplode = 0;

    // TODO: use smart pointer
    float        *cockpit_damage;     //0 is radar, 1 to MAXVDU is vdus and >MAXVDU is gauges





    std::vector< string >destination;
    std::vector< DockingPorts >dockingports;
    ///warning unreliable pointer, never dereference!
    std::vector< Unit* > clearedunits;
    std::vector< DockedUnits* >dockedunits;
    UnitContainer DockedTo;
    float unitscale;     //for output
    class XMLSerializer *unitwriter = nullptr;


    enum GAUGES
    {
        //Image-based gauges
        ARMORF, ARMORB, ARMORR, ARMORL, ARMOR4, ARMOR5, ARMOR6, ARMOR7, FUEL, SHIELDF, SHIELDR, SHIELDL, SHIELDB, SHIELD4,
        SHIELD5, SHIELD6, SHIELD7,
        ENERGY, AUTOPILOT, COLLISION, EJECT, LOCK, MISSILELOCK, JUMP, ECM, HULL, WARPENERGY,
        //target gauges
        TARGETSHIELDF, TARGETSHIELDB, TARGETSHIELDR, TARGETSHIELDL,
        KPS,         //KEEP KPS HERE - it marks the start of text-based gauges
        SETKPS, COCKPIT_FPS, WARPFIELDSTRENGTH, MAXWARPFIELDSTRENGTH, MAXKPS, MAXCOMBATKPS, MAXCOMBATABKPS, MASSEFFECT,
        AUTOPILOT_MODAL,         //KEEP first multimodal gauge HERE -- it marks the start of multi-modal gauges
        SPEC_MODAL, FLIGHTCOMPUTER_MODAL, TURRETCONTROL_MODAL, ECM_MODAL, CLOAK_MODAL, TRAVELMODE_MODAL,
        RECIEVINGFIRE_MODAL, RECEIVINGMISSILES_MODAL, RECEIVINGMISSILELOCK_MODAL, RECEIVINGTARGETLOCK_MODAL,
        COLLISIONWARNING_MODAL, CANJUMP_MODAL, CANDOCK_MODAL,
        NUMGAUGES         //KEEP THIS LAST - obvious reasons, marks the end of all gauges
    };
    enum MODALGAUGEVALUES
    {
        OFF, ON, SWITCHING, ACTIVE, FAW, MANEUVER, TRAVEL, NOTAPPLICABLE, READY, NODRIVE, TOOFAR, NOTENOUGHENERGY, WARNING,
        NOMINAL, AUTOREADY
    };
};



//From star_system_jump.cpp
class StarSystem;
struct unorigdest
{
    UnitContainer un;
    UnitContainer jumppoint;
    StarSystem   *orig;
    StarSystem   *dest;
    float   delay;
    int     animation;
    bool    justloaded;
    bool    ready;
    QVector final_location;
    unorigdest( Unit *un,
                Unit *jumppoint,
                StarSystem *orig,
                StarSystem *dest,
                float delay,
                int ani,
                bool justloaded,
                QVector use_coordinates /*set to 0,0,0 for crap*/ ) : un( un )
        , jumppoint( jumppoint )
        , orig( orig )
        , dest( dest )
        , delay( delay )
        , animation( ani )
        , justloaded( justloaded )
        , ready( true )
        , final_location( use_coordinates ) {}
};

#endif
