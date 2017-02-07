#ifndef AI_AI_H
#define AI_AI_H

#include <memory>
#include <map>
#include "player_ai_helper.h"
#include "ai_export.h"

namespace ai {

class Group;

class AI_EXPORT AI : public player::PlayerAiHelper {
private:
	bool init_groups;
	state::list_act_id_t all_actors;
	std::vector<std::pair<int64_t, int>> sortedEnemies;
	std::vector <Group*> groups;
public:
	AI();
	void Update(std::shared_ptr<state::PlayerStateHandler> state) override;
	void SetSortedEnemies(std::shared_ptr<state::PlayerStateHandler> state);
};

}
#endif //AI_AI_H
