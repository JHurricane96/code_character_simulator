#ifndef STATE_PATH_PLANNER_PATH_PLANNER_HELPER_H
#define STATE_PATH_PLANNER_PATH_PLANNER_HELPER_H

#include <vector>
#include <memory>
#include "vector2d.h"
#include "actor/actor.h"
#include "state_export.h"

namespace state {

class STATE_EXPORT Formation {
public:
    int64_t formation_id;
    virtual std::vector<physics::Vector2D> ReturnFormation(int64_t formation_sizeR);
};

class STATE_EXPORT PathPlannerHelper {
private:
	std::vector<physics::Vector2D> next_points;
	act_id_t leader_id;
	physics::Vector2D relative_position;
	bool is_in_formation;
public:
	bool IsPathPlanning();
	void SetPath(
		std::vector<physics::Vector2D> points,
		act_id_t leader_id,
		Point relative_position,
		bool is_in_formation
	);
	physics::Vector2D Update(physics::Vector2D current_position);
};

}

#endif
