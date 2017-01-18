#include "path_planner/formation.h"

namespace state {

Formation::Formation(
	PlayerId player_id,
	int64_t formation_id,
	std::vector<std::shared_ptr<Actor> > units,
	std::shared_ptr<FormationMaker> formation_maker,
	std::vector<physics::Vector2D> destinations
) :
player_id(player_id),
formation_id(formation_id),
units(units),
formation_maker(formation_maker),
destinations(destinations),
is_finished(false) {
	auto formation_positions =
		formation_maker->ReturnFormation(units.size());
    leader = units[0];
	units[0]->GetPathPlannerHelper()->SetPath(
		formation_id,
		formation_positions[0],
		true,
		NULL
	);

	for (int64_t i = 1; i < units.size(); ++i) {
		units[i]->GetPathPlannerHelper()->SetPath(
			formation_id,
			formation_positions[i],
			false,
			units[0]
		);
	}
}

bool Formation::IsFinished() {
	return is_finished;
}

void Formation::Finish() {
	is_finished = true;
}

enum LeaderState {
	LEADER_EXISTS,
	LEADER_MISSING,
	LEADER_FOUND,
};

void Formation::Update(
	std::vector<std::shared_ptr<Actor> > &sorted_allies,
	std::vector<std::shared_ptr<Actor> > &sorted_enemies
) {
	LeaderState leader_state = LEADER_EXISTS;
	std::vector<std::shared_ptr<Actor> > new_units;
	std::shared_ptr<Actor> new_leader;
	if (!leader->GetPathPlannerHelper()->IsPathPlanning() ||
		leader->GetPathPlannerHelper()->GetFormationId() != formation_id) {
		leader_state = LEADER_MISSING;
	}

	for (auto unit : units) {
		if (unit->GetPathPlannerHelper()->IsPathPlanning() &&
			unit->GetPathPlannerHelper()->GetFormationId() == formation_id) {
			new_units.push_back(unit);
			if (leader_state == LEADER_MISSING) {
				leader_state = LEADER_FOUND;
				new_leader = unit;
				unit->GetPathPlannerHelper()->MakeLeader();
			}
			else if (leader_state == LEADER_FOUND) {
				unit->GetPathPlannerHelper()->UpdateLeader(new_leader);
			}
		}
	}
	int64_t old_no_units = units.size();
	units = new_units;

	if (units.empty()) {
		Finish();
		return;
	}

	if (old_no_units != units.size()) {
		if (!IsValidFormation(formation_maker.get(), units.size())) {
			for (auto unit : units) {
				unit->GetPathPlannerHelper()->FinishPath();
			}
			Finish();
			return;
		}

		auto formation_positions =
			formation_maker->ReturnFormation(units.size());

		for (int64_t i = 0; i < units.size(); ++i) {
			units[i]->GetPathPlannerHelper()->
			UpdateRelativePosition(formation_positions[i]);
		}
	}

	for (auto unit : units) {
		int64_t unit_size = unit->GetSize();
		if (unit->GetPosition().distance(destinations.back()) <=
			unit_size * 3) {
			destinations.pop_back();
			break;
		}
	}

	if (destinations.empty()) {
		Finish();
		for (auto unit : units) {
			unit->GetPathPlannerHelper()->FinishPath();
		}
		return;
	}

	int64_t no_units_in_formation = 0;
	for (auto unit : units) {
		unit->GetPathPlannerHelper()->Update(
			sorted_allies,
			sorted_enemies,
			destinations.back()
		);
		if (unit->GetPathPlannerHelper()->IsInFormation()) {
			no_units_in_formation++;
		}
	}

	if (no_units_in_formation == units.size()) {
		for (auto unit : units) {
			unit->GetPathPlannerHelper()->SpeedUp();
		}
	}
	else {
		for (auto unit : units) {
			if (!unit->GetPathPlannerHelper()->IsInFormation()) {
				unit->GetPathPlannerHelper()->SpeedUp();
			}
		}
	}
}

void Formation::MergeWithBuffer(
	const Formation& formation,
	std::vector<std::shared_ptr<Actor> > actors
) {
	for (int64_t i = 0; i < units.size(); ++i) {
		units[i] = actors[units[i]->GetId()];
	}
	leader = actors[leader->GetId()];
}

}
