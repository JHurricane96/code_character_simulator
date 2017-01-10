#include <vector>
#include "player2.h"
#include "vector2d.h"
#include "path_planner/path_planner_helper.h"

#include <iostream>

namespace player2 {

std::vector<act_id_t> sword_ids;

int soorya_is_a_slut = 0;
int kingthing = 0;

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
			f[i] = physics::Vector2D(0, 0);
		return f;
	}
};

Player2::Player2() : balls(0), f(new MyFormation()) {
	for (int i = 16; i < 20; ++i) {
		sword_ids.push_back(i);
	}
}

void Player2::Update(std::shared_ptr<state::PlayerStateHandler> state) {
	std::vector<physics::Vector2D> crap;
	auto magicians = state->GetMagicians();
	// std::cout << "Size: " << state->GetPlayerUnitIds().size() << "\n";
	auto sw = state->GetSwordsmen();
	list_act_id_t sws;
	for (auto s : sw) {
		sws.push_back(s.GetId());
	}

	list_act_id_t ids;
	for (auto magician : magicians) {
		ids.push_back(magician.GetId());
	}
	if (!soorya_is_a_slut) {
		soorya_is_a_slut = 1;
		return;
	}
	std::vector<physics::Vector2D> path;
	if (balls == 0) {
		// std::cout << "Moved stuff!\n";
		std::unique_ptr<int> success(new int());
		state->MoveUnits(list_act_id_t(sws.begin(), sws.begin() + 4), state->GetEnemyKing(NULL).GetPosition(), f.get(), std::vector<int64_t>(3, 1), crap, NULL);
		state->PlanPath(
			magicians.begin()->GetPosition(),
			physics::Vector2D(300, 300),
			std::vector<int64_t>({1, 1, 1}),
			path,
			NULL
		);
		// state->MoveUnits(ids, physics::Vector2D(300, 300), f.get(), std::vector<int64_t>(3, 1), path, success.get());
		state->MoveUnits(ids, path, f.get(), success.get());
		std::cout << "Success: " << *success << "\n";
		for (auto point : path) {
			std::cout << point.x << " " << point.y << ", ";
		}
		std::cout << "\n";
		// state->MoveUnits(list_act_id_t(sws.begin(), sws.begin() + 4), physics::Vector2D(40, 60), f.get(), std::vector<int64_t>(3, 1), NULL);
		// state->MoveUnits(sword_ids, physics::Vector2D(99 * 2, 99 * 2), f.get(), std::vector<int64_t>(3, 1), NULL);
		// std::cout << "Soorya is a cuck!: " << *(state->GetPlayerEnemyIds().begin()) << "\n";
		auto enemies = state->GetPlayerEnemyIds();
		// state->AttackUnit(sword_ids, *(state->GetPlayerEnemyIds().begin()), success.get());
		std::cout << "attack success: " << *success << "\n";
		// std::cout << "success: " << *success << "\n";
		balls = 1;
	}
	else if (balls == 1) {
		std::unique_ptr<int> success(new int());
		path.clear();
		// state->MoveUnits(list_act_id_t(ids.begin(), ids.begin() + 4), physics::Vector2D(300, 300), f.get(), std::vector<int64_t>({1, 1, 1000}), success.get());
		std::cout << "Success of move units 2: " << *success << "\n";
		std::cout << "success: " << *success << " Plan Path: " << state->PlanPath(
			physics::Vector2D(20, 20),
			physics::Vector2D(300, 300),
			std::vector<int64_t>({1, 1, 1}),
			crap,
			success.get()
		) << "\n";
		balls = 2;
	}
	else if (balls == 2) {
		if (!sw.begin()->GetPathPlannerHelper().IsPathPlanning()) {
			std::unique_ptr<int> success(new int());
			state->AttackUnit(list_act_id_t(sws.begin(), sws.begin() + 4), 2, success.get());
			std::cout << "LOLOLOL!: " << *success << "\n";
			balls = 3;
		}
	}
	// else if (balls == 1) {
	// 	std::unique_ptr<int> swordssuccess(new int());
	// 	state->AttackUnit(sword_ids, 8, swordssuccess.get());
	// 	// std::cout << "swordsmen attacking success: " << *swordssuccess << "\n";
	// 	// balls = 2;
	// }
	// if (kingthing == 0) {
	// 	state->MoveUnits(list_act_id_t({3}), state->GetEnemyFlag().GetPosition(), f.get(), std::vector<int64_t>(3, 1), NULL);
	// 	kingthing = 1;
	// }
	// else if (kingthing == 1 && !state->GetKing().GetPathPlannerHelper().IsPathPlanning()) {
	// 	state->FlagCapture(NULL);
	// 	state->MoveUnits(list_act_id_t({3}), state->GetBase().GetPosition(), f.get(), std::vector<int64_t>(3, 1), NULL);
	// 	kingthing = 2;
	// }
	// else if (kingthing == 2 && !state->GetKing().GetPathPlannerHelper().IsPathPlanning()) {
	// 	state->FlagDrop(NULL);
	// 	state->MoveUnits(list_act_id_t({3}), physics::Vector2D(200, 40), f.get(), std::vector<int64_t>(3, 1), NULL);
	// 	kingthing = 3;
	// }
	// if (state->GetKing().GetHp() <= 0) {
	// 	std::unique_ptr<int> kingsuccess(new int());
	// 	state->RespawnUnit(3, state->GetBase().GetId(), NULL);
	// }
}

}
