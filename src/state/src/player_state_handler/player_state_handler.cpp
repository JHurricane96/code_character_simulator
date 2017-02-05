#include <algorithm>
#include "player_state_handler/player_state_handler.h"

namespace state {

PlayerStateHandler::PlayerStateHandler(
	State * state, PlayerId player_id):
	state(state),
	player_id(player_id) {}

TerrainElementView::TerrainElementView() {}

TerrainElementView::TerrainElementView(
	TerrainElement* te,
	PlayerId player_id
	):
	position(te->GetPosition()),
	size(te->GetSize()),
	los_type(te->GetLos(player_id)) {
		if (los_type == UNEXPLORED) {
			terrain_type = UNDEFINED;
		}
		else {
			terrain_type = te->GetTerrainType();
		}
	}

list_act_id_t PlayerStateHandler::GetPlayerUnitIds() {
	return state->GetPlayerUnitIds(player_id);
}

list_act_id_t PlayerStateHandler::GetPlayerEnemyIds() {
	return state->GetPlayerEnemyIds(player_id);
}

std::vector<MagicianView> PlayerStateHandler::GetMagicians() {
	auto magicians = state->GetMagicians(player_id);
	std::vector<MagicianView> magician_views;
	for (auto magician : magicians)
		magician_views.push_back(MagicianView(magician.get()));
	return magician_views;
}

std::vector<EnemyMagicianView> PlayerStateHandler::GetEnemyMagicians() {
	auto magicians = state->GetEnemyMagicians(player_id);
	std::vector<EnemyMagicianView> magician_views;
	for (auto magician : magicians)
		magician_views.push_back(EnemyUnitView(magician.get()));
	return magician_views;
}

std::vector<ScoutView> PlayerStateHandler::GetScouts() {
	auto scouts = state->GetScouts(player_id);
	std::vector<ScoutView> scout_views;
	for (auto scout : scouts)
		scout_views.push_back(UnitView(scout.get()));
	return scout_views;
}

std::vector<EnemyScoutView> PlayerStateHandler::GetEnemyScouts() {
	auto scouts = state->GetEnemyScouts(player_id);
	std::vector<EnemyScoutView> scout_views;
	for (auto scout : scouts)
		scout_views.push_back(EnemyUnitView(scout.get()));
	return scout_views;
}

std::vector<SwordsmanView> PlayerStateHandler::GetSwordsmen() {
	auto swordsmen = state->GetSwordsmen(player_id);
	std::vector<SwordsmanView> swordsman_views;
	for (auto swordsman : swordsmen) {
		swordsman_views.push_back(SwordsmanView(swordsman.get()));
	}
	return swordsman_views;
}

std::vector<EnemySwordsmanView>
PlayerStateHandler::GetEnemySwordsmen() {
	auto swordsmen = state->GetEnemySwordsmen(player_id);
	std::vector<EnemySwordsmanView> swordsman_views;
	for (auto swordsman : swordsmen) {
		swordsman_views.push_back(EnemySwordsmanView(swordsman.get()));
	}
	return swordsman_views;
}

std::vector<TowerView> PlayerStateHandler::GetTowers() {
	auto towers = state->GetTowers(player_id);
	std::vector<TowerView> tower_views;
	for (auto tower : towers)
		tower_views.push_back(TowerView(tower.get()));
	return tower_views;
}

std::vector<EnemyTowerView> PlayerStateHandler::GetEnemyTowers() {
	auto towers = state->GetEnemyTowers(player_id);
	std::vector<EnemyTowerView> tower_views;
	for (auto tower : towers)
		tower_views.push_back(EnemyTowerView(tower.get()));
	return tower_views;
}

FlagView PlayerStateHandler::GetFlag() {
	return FlagView(state->GetFlag(player_id).get());
}

EnemyFlagView PlayerStateHandler::GetEnemyFlag() {
	return EnemyFlagView(state->GetEnemyFlag(player_id).get());
}

BaseView PlayerStateHandler::GetBase() {
	return BaseView(state->GetBase(player_id).get());
}

EnemyBaseView PlayerStateHandler::GetEnemyBase() {
	return EnemyBaseView(state->GetEnemyBase(player_id).get());
}

KingView PlayerStateHandler::GetKing() {
	return KingView(state->GetKing(player_id).get());
}

EnemyKingView PlayerStateHandler::GetEnemyKing(int * success) {
	auto enemy_king = state->GetEnemyKing(player_id, success);
	if (enemy_king != nullptr)
		return EnemyKingView(enemy_king.get());
	return EnemyKingView();
}

TerrainElementView PlayerStateHandler::CoordinateToTerrainElement(
	physics::Vector2D position,
	int * success
) {
	auto bounds = state->GetTerrain().GetSize();
	if (position.x < 0 || position.y < 0 ||
		position.x >= bounds.x || position.y >= bounds.y ) {
		if (success) *success = 0;
		return TerrainElementView();
	}

	TerrainElement te = state->GetTerrain()
							 .CoordinateToTerrainElement(position);
	if (success) *success = 1;
	return TerrainElementView(&te, player_id);
}

TerrainElementView PlayerStateHandler::OffsetToTerrainElement(
	physics::Vector2D offset,
	int * success
) {
	auto rows = state->GetTerrain().GetRows();
	if (offset.x < 0 || offset.y < 0 ||
		offset.x >= rows || offset.y >= rows ) {
		if (success) *success = 0;
		return TerrainElementView();
	}

	TerrainElement te = state->GetTerrain()
							 .OffsetToTerrainElement(offset);
	if (success) *success = 1;
	return TerrainElementView(&te, player_id);
}

int64_t PlayerStateHandler::GetTerrainRows() {
	return state->GetTerrain().GetRows();
}

UnitView PlayerStateHandler::GetUnitFromId(act_id_t actor_id, int * success) {
	auto actor = state->GetActorFromId(player_id, actor_id, success);
	if (actor != nullptr)
		return UnitView(actor.get());
	return UnitView();
}

EnemyUnitView
PlayerStateHandler::GetEnemyUnitFromId(act_id_t actor_id, int * success) {
	PlayerId enemy_player_id =
		static_cast<PlayerId>((player_id + 1) % (LAST_PLAYER + 1));
	auto actor = state->GetActorFromId(enemy_player_id, actor_id, success);
	if (actor == nullptr)
		return EnemyUnitView();

	auto enemies = state->GetPlayerEnemyIds(player_id);

	if (std::find(enemies.begin(), enemies.end(), actor_id) == enemies.end()) {
		if (success) {
			*success = -2;
		}
		return EnemyUnitView();
	}

	return EnemyUnitView(actor.get());
}

int64_t PlayerStateHandler::GetScore() {
	return state->GetScores()[player_id];
}

list_act_id_t PlayerStateHandler::GetRespawnables() {
	return state->GetRespawnables(player_id);
}

void PlayerStateHandler::MoveUnits(
	list_act_id_t unit_ids,
	physics::Vector2D destination,
	FormationMaker * formation_maker,
	std::vector<int64_t> terrain_weights,
	std::vector<physics::Vector2D> &path,
	int * success
) {
	state->MoveUnits(
		player_id,
		unit_ids,
		destination,
		formation_maker,
		terrain_weights,
		path,
		success
	);
}

void PlayerStateHandler::MoveUnits(
	list_act_id_t unit_ids,
	std::vector<physics::Vector2D> destinations,
	FormationMaker * formation_maker,
	int * success
) {
	state->MoveUnits(
		player_id,
		unit_ids,
		destinations,
		formation_maker,
		success
	);
}

void PlayerStateHandler::AttackUnit(
	list_act_id_t attacker_ids,
	act_id_t attack_target_id,
	int * success
) {
	state->AttackUnit(
		player_id,
		attacker_ids,
		attack_target_id,
		success
	);
}

void PlayerStateHandler::FlagCapture(int * success) {
	state->FlagCapture(player_id, success);
}

void PlayerStateHandler::FlagDrop(int * success) {
	state->FlagDrop(player_id, success);
}

float PlayerStateHandler::PlanPath(
	physics::Vector2D start,
	physics::Vector2D destination,
	std::vector<int64_t> terrain_weights,
	std::vector<physics::Vector2D> &path,
	int * success
) {
	return state->PlanPath(
		start,
		destination,
		terrain_weights,
		path,
		success
	);
}

void PlayerStateHandler::RespawnUnit(
	act_id_t actor_id,
	act_id_t respawn_location,
	int * success
) {
	state->RespawnUnit(
		player_id,
		actor_id,
		respawn_location,
		success
	);
}

}
