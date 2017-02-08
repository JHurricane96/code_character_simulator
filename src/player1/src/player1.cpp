#include "player1.h"
#include <climits>


namespace player1 {

// Here are some functions to help you get started!

/**
 * Formation where all units move together
 */
class TogetherFormation : public state::FormationMaker {
	std::vector<physics::Vector2D> ReturnFormation(int64_t size) override {
		return std::vector<physics::Vector2D>(size, physics::Vector2D(0, 0));
	}
};

/**
 * Filters through the given units and returns a list of the alive ones
 *
 * @param[in]  state     The state
 * @param      unit_ids  The unit IDs
 *
 * @return     The alive units
 */
state::list_act_id_t GetAliveUnits(
	std::shared_ptr<state::PlayerStateHandler> state,
	state::list_act_id_t& unit_ids
) {
	state::list_act_id_t alive_ids;
	for (auto id : unit_ids) {
		auto unit = state->GetUnitFromId(id, NULL);
		if (unit.GetHp() > 0) {
			alive_ids.push_back(id);
		}
	}

	return alive_ids;
}


/**
 * Gets the nearest enemy of the given unit within a certain threshold
 *
 * @param[in]  state      The state
 * @param[in]  unit_id    The unit ID
 * @param[in]  threshold  The threshold
 *
 * @return     The nearest enemy
 */
state::act_id_t GetNearestEnemy(
	std::shared_ptr<state::PlayerStateHandler> state,
	state::act_id_t unit_id,
	int threshold = INT_MAX
) {
	auto unit = state->GetUnitFromId(unit_id, NULL);
	auto enemies = state->GetPlayerEnemyIds();

	state::act_id_t nearest_enemy_id = -1;
	float min_dist = 0;

	for (auto en_id : enemies) {
		auto enemy = state->GetEnemyUnitFromId(en_id, NULL);

		auto cur_dist = enemy.GetPosition().distance(unit.GetPosition());

		if (cur_dist <= threshold) {
			if (nearest_enemy_id == -1 || cur_dist < min_dist) {
				nearest_enemy_id = en_id;
				min_dist = cur_dist;
			}
		}
	}

	return nearest_enemy_id;
}

/**
 * Iterates through each attacker, moving it towards the enemy if
 * too far, and attacking if near enough
 *
 * @param[in]  state               The state
 * @param[in]  attacking_unit_ids  The attacking unit IDs
 * @param[in]  attack_target_id    The attack target ID
 * @param[in]  formation_maker     The formation maker
 * @param[in]  threshold           The minimum distance within which
 *                                 the target must be in
 *
 * @return     true if the enemy is dead/out of LOS, false otherwise
 */
bool AttackUnit(
	std::shared_ptr<state::PlayerStateHandler> state,
	state::list_act_id_t attacking_unit_ids,
	state::act_id_t attack_target_id,
	state::FormationMaker * formation_maker,
	int threshold = INT_MAX
) {
	int is_target_valid;
	auto attack_target = state->GetEnemyUnitFromId(attack_target_id, &is_target_valid);

	if (is_target_valid != 1) {
		return true;
	}

	bool within_range = false;
	for (auto attacking_unit_id : attacking_unit_ids) {
		auto attacking_unit = state->GetUnitFromId(attacking_unit_id, NULL);
		if (attack_target.GetPosition().distance(attacking_unit.GetPosition()) <= threshold) {
			within_range = true;
			break;
		}
	}

	if (!within_range) {
		return true;
	}

	for (auto attacking_unit_id : attacking_unit_ids) {
		auto attacking_unit = state->GetUnitFromId(attacking_unit_id, NULL);
		state::list_act_id_t attacking_list({attacking_unit_id});

		int range;
		if (attacking_unit.GetAttackTarget(NULL)) {
			range = attacking_unit.GetAttackRange();
		}
		else {
			range = attacking_unit.GetAttackRange() / 3;
		}

		if (attack_target.GetPosition().distance(attacking_unit.GetPosition()) <
			range) {
			int success;
			state->AttackUnit(attacking_list, attack_target_id, &success);
		}
		else {
			state->MoveUnits(
				attacking_list,
				std::vector<physics::Vector2D>({attack_target.GetPosition()}),
				formation_maker,
				NULL
			);
		}
	}

	return false;
}

void Player1::Update(std::shared_ptr<state::PlayerStateHandler> state) {
	//Initialize stuff here
	if (!init) {
		init = true;

		together_formation = new TogetherFormation();

		terrain_elt_size = state->OffsetToTerrainElement(physics::Vector2D(0, 0), NULL).size;

		auto swordsmen = state->GetSwordsmen();
		auto mages = state->GetMagicians();
		state::list_act_id_t sw_ids, mg_ids;
		for (auto swordsman : swordsmen) {
			sw_ids.push_back(swordsman.GetId());
		}
		for (auto mage : mages) {
			mg_ids.push_back(mage.GetId());
		}
		// Initializing defender IDs...
		def_ids = state::list_act_id_t(sw_ids.begin(), sw_ids.begin() + sw_ids.size() / 2 + 1);
		def_ids.insert(def_ids.end(), mg_ids.begin(), mg_ids.begin() + mg_ids.size() / 2 + 1);

		auto base = state->GetBase();
		auto sw_size = swordsmen.begin()->GetSize();

		// Initializing path that defenders patron along
		std::vector<physics::Vector2D> offsets({
			physics::Vector2D(0, -base.GetSize() - sw_size - terrain_elt_size * 5),
			physics::Vector2D(base.GetSize() + sw_size + terrain_elt_size * 5, 0),
			physics::Vector2D(0, base.GetSize() + sw_size + terrain_elt_size * 5),
			physics::Vector2D(-base.GetSize() - sw_size - terrain_elt_size * 5, 0)
		});
		for (auto offset : offsets) {
			auto point = base.GetPosition() + offset;
			point.x = std::max(std::min(
				(double) state->GetTerrainRows() * terrain_elt_size, point.x), 0.0);
			point.y = std::max(std::min(
				(double) state->GetTerrainRows() * terrain_elt_size, point.y), 0.0);
			patrol_path.push_back(point);
		}
	}

	auto first_unit = state->GetUnitFromId(*def_ids.begin(), NULL);
	int prev_def_mode;

	do {
		prev_def_mode = def_mode;

		// def_mode 0 is for patrolling
		if (def_mode == 0) {
			auto target_id = GetNearestEnemy(state, first_unit.GetId(), terrain_elt_size * 3);

			if (target_id != -1) {
				def_mode = 1;
				def_att_target = target_id;
			}
			else if (!first_unit.GetPathPlannerHelper().IsPathPlanning()) {
				int success;
				state->MoveUnits(def_ids, std::vector<physics::Vector2D>({patrol_path[cur_patrol_pnt]}), together_formation, &success);
				cur_patrol_pnt = (cur_patrol_pnt + 1) % patrol_path.size();
			}
		}
		// def_mode 1 is for attacking guys who dare to approach us B|
		else if (def_mode == 1) {
			auto alive_def_ids = GetAliveUnits(state, def_ids);
			if (AttackUnit(
				state,
				alive_def_ids,
				def_att_target,
				together_formation,
				terrain_elt_size * 3
			)) {
				def_mode = 0;
			}
		}
	} while (prev_def_mode != def_mode);
}

}
