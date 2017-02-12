/**
 * @file ai.h
 * The AI class for the main unit update loop
 * and information retention between updates
*/
#ifndef AI_AI_H
#define AI_AI_H

#include <memory>
#include <map>
#include "player_ai_helper.h"
#include "group_state.h"
#include "ai_export.h"

namespace ai {

/**
 * Class for ai.
 */
class AI_EXPORT AI : public player::PlayerAiHelper {

private:

	/**
	 * Initial groups on start.
	 */
	bool init_groups;

	/**
	 * List of all actors recieved from state.
	 */
	state::list_act_id_t all_actors;

	/**
	 * Stores sortedEnemies every tick.
	 */
	std::vector<std::pair<int64_t, int>> sortedEnemies;

	/**
	 * Guards for the king
	 */
	std::vector<state::act_id_t> kingsGuard;

	/**
	 * Guards for the flag
	 */
	std::vector<state::act_id_t> flagsGuard;

	/**
	 * Vector of all groups
	 */
	std::vector <Group*> groups;

	/**
	 * Scales strength of AI according to level
	 */
	int aiLevel;

public:

	/**
	 * Constructor for AI.
	 */
	AI(int level);

	/**
	 * Updates the state.
	 *
	 * @param[in]  state  The state
	 */
	void Update(std::shared_ptr<state::PlayerStateHandler> state) override;

	/**
	 * Sets the sorted enemies every tick.
	 *
	 * @param[in]  state  The state
	 */
	void SetSortedEnemies(std::shared_ptr<state::PlayerStateHandler> state);

	/**
	 * @brief      Gets the optimal respawn location.
	 *
	 * @param[in]  state          The state
	 * @param[in]  to_respawn_id  To respawn identifier
	 *
	 * @return     The optimal respawn location.
	 */
	state::act_id_t GetOptimalRespawnLocation (
		std::shared_ptr<state::PlayerStateHandler> state,
		state::act_id_t to_respawn_id
	);
};

}

#endif //AI_AI_H
