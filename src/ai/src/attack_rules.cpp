#include "player_state_handler/player_state_handler.h"
#include "attack_rules.h"
#include "ai_utilities.h"
#include "ipc.h"

#include <bits/stdc++.h>

#define INF 10000
#define TINY 0.00001
#define SWORDHIT 100
#define MAGEHIT 150
namespace ai {

AttackRules::AttackRules() {
	formation = new DefaultFormation();
}

/**
 * Strategy to effectively attack enemies whilst in attack state
 *
 * @param[in]  unitId  The unit identifier
 * @param[in]  state   The state
 */
void AttackRules::Strategy (
	state::act_id_t unitId,
	std::shared_ptr<state::PlayerStateHandler> state,
	std::vector<std::pair<int64_t, int>> sortedEnemies
)	{
		if (state->GetUnitFromId(unitId, nullptr).GetPathPlannerHelper().IsPathPlanning()
			|| ( !state->GetUnitFromId(unitId, nullptr).CanAttack()
			&& state->GetUnitFromId(unitId, nullptr).GetActorType() != state::ActorType::KING )
			|| !state->GetUnitFromId(unitId, nullptr).CanPathPlan()
			|| IsAttackingEnemy(state, unitId)
		) {
			if(state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::KING)
				// ipc::Logger::Instance().SetLogs("Returning King");
				std::cerr << "Returning King\n";
			return;
		}

		std::vector<int64_t> terrain_weights(3);
		if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::MAGICIAN) {
			terrain_weights[state::MOUNTAIN] = 1;
			terrain_weights[state::PLAIN] = 1;
			terrain_weights[state::FOREST] = 1;
		}
		else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::SWORDSMAN) {
			terrain_weights[state::MOUNTAIN] = 1;
			terrain_weights[state::PLAIN] = 1;
			terrain_weights[state::FOREST] = 1;
		}
		else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::KING) {
			terrain_weights[state::MOUNTAIN] = 1;
			terrain_weights[state::PLAIN] = 1;
			terrain_weights[state::FOREST] = 1;
		}
		else if (state->GetUnitFromId(unitId, nullptr).GetActorType() == state::ActorType::SCOUT) {
			terrain_weights[state::MOUNTAIN] = 1;
			terrain_weights[state::PLAIN] = 1;
			terrain_weights[state::FOREST] = 1;
		}

		auto unitView = state->GetUnitFromId(unitId, nullptr);
		state::list_act_id_t traveller;
		std::vector<physics::Vector2D> tempPath;

		if(unitView.GetActorType() == state::ActorType::KING) {
			// std::unique_ptr<int> flagCaptureStatus(new int());
			int flagCaptureStatus;
			std::unique_ptr<int> flagDropStatus(new int());

			state->FlagCapture(&flagCaptureStatus);
			switch(flagCaptureStatus) {
				case -1:
					traveller.push_back(unitId);
					state->MoveUnits (
						traveller,
						std::vector<physics::Vector2D>({state->GetEnemyBase().GetPosition()}),
						formation,
						nullptr);
					break;
				case -2:
					state->FlagDrop(flagDropStatus.get());
					switch(*flagDropStatus) {
						case -1:
							traveller.push_back(unitId);
							state->MoveUnits (
								traveller,
								std::vector<physics::Vector2D>({state->GetBase().GetPosition()}),
								formation,
								nullptr);
							break;
					}
				case 1:
					traveller.push_back(unitId);
					state->MoveUnits (
						traveller,
						std::vector<physics::Vector2D>({state->GetBase().GetPosition()}),
						formation,
						nullptr);
					break;
			}
		}
}

}