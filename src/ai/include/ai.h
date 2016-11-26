#ifndef AI_AI_H
#define AI_AI_H

#include <memory>
#include <map>
#include "player_ai_helper.h"
#include "ai_export.h"

namespace ai {

/**
 * Forward declaration for class group.
 */
class Group;

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
public:

	/**
	 * Constructor for AI.
	 */
	AI();

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
};

}

#endif //AI_AI_H