#include <vector>
#include "player1.h"
#include "vector2d.h"
#include "path_planner/path_planner_helper.h"

#include <iostream>


namespace player1 {

list_act_id_t l({ 6, 7 });

class MyFormation : public state::FormationMaker {
public:
	MyFormation()
		: FormationMaker()
	{
	}
	std::vector<physics::Vector2D> ReturnFormation(int64_t formation_size)
	{
		std::vector<physics::Vector2D> f(formation_size);
		// if (formation_size == 1) {
		// 	std::cout << "LOL IN HERE!\n";
		// 	f[0] = physics::Vector2D(1, 0);
		// 	return f;
		// }
		for (int i = 0; i < formation_size; i++)
			f[i] = physics::Vector2D(i * 30, i);
		return f;
	}
};

void Player1::Update(std::shared_ptr<state::PlayerStateHandler> state) {
	// MyFormation f;
	std::shared_ptr<MyFormation> f(new MyFormation());
	// std::cout << "lol\n";
	std::unique_ptr<int> success(new int());
	state->AttackUnit(l, 3, success.get());
	// std::cout << "tower attack success: " << *success << "\n";
	auto king = state->GetKing();
	std::vector<physics::Vector2D> crap;
	if (king.GetHp() < king.GetMaxHp()) {
		state->MoveUnits(list_act_id_t({king.GetId()}), physics::Vector2D(0, 0), f.get(), std::vector<int64_t>(3, 1), crap, NULL);
	}
}

}
