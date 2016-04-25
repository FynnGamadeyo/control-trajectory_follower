#ifndef TRAJECTORY_FOLLOWER_MOTION2D
#define TRAJECTORY_FOLLOWER_MOTION2D

#include <base/Eigen.hpp>
#include <base/commands/Motion2D.hpp>
#include <base/Angle.hpp>

namespace trajectory_follower {

enum DriveMode {
    ModeAckermann,
    ModeTurnOnTheSpot,
    ModeLateral
};

struct Motion2D {
    double translation;
    double rotation;
    double heading;     // relative to current robot heading
                        // zero means no variation of heading
    
    Motion2D()
        : translation(0.)
        , rotation(0.)
        , heading(0.)
    {
    }

    Motion2D(const Eigen::Vector2d &translation, double rotation)
        : translation((Eigen::Vector2d(translation.x(), 0) + Eigen::Vector2d(0, translation.y())).norm())
        , rotation(rotation)
    {
        this->heading = atan2(translation.y(), translation.x());
    }

    Motion2D(double translation, double rotation, double orientation)
        : translation(translation)
        , rotation(rotation)
        , heading(orientation)
    {
    }
    
    DriveMode getDriveMode()
    {
        if (translation == 0 && rotation != 0)
        {
            return ModeTurnOnTheSpot;
        }
        else if (heading != 0 && rotation == 0)
        {
            return ModeLateral;
        }
        else
        {
            return ModeAckermann;
        }
    }
    
    base::commands::Motion2D toBaseMotion2D()
    {
        base::commands::Motion2D cmd;
        cmd.rotation = this->rotation;
        cmd.translation = this->translation;
        return cmd;
    }
};

}

#endif