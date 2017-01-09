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
	terrain_type(te->GetTerrainType()),
	los_type(te->GetLos(player_id)) {}

list_act_id_t PlayerStateHandler::GetPlayerUnitIds() {
	return state->GetPlayerUnitIds(player_id);
}

list_act_id_t PlayerStateHandler::GetPlayerEnemyIds() {
	return state->GetPlayerEnemyIds(player_id);
}

std::vector<TowerView> PlayerStateHandler::GetTowers() {
	auto towers = state->GetTowers(player_id);
	std::vector<TowerView> tower_views;
	for (auto tower : towers)
		tower_views.push_back(UnitView(tower.get()));
	return tower_views;
}

std::vector<EnemyTowerView> PlayerStateHandler::GetEnemyTowers() {
	auto towers = state->GetEnemyTowers(player_id);
	std::vector<EnemyTowerView> tower_views;
	for (auto tower : towers)
		tower_views.push_back(EnemyUnitView(tower.get()));
	return tower_views;
}

UnitView PlayerStateHandler::GetFlag() {
	return UnitView(state->GetFlag(player_id).get());
}

EnemyUnitView PlayerStateHandler::GetEnemyFlag() {
	return EnemyUnitView(state->GetEnemyFlag(player_id).get());
}

UnitView PlayerStateHandler::GetBase() {
	return UnitView(state->GetBase(player_id).get());
}

EnemyUnitView PlayerStateHandler::GetEnemyBase() {
	return EnemyUnitView(state->GetEnemyBase(player_id).get());
}

UnitView PlayerStateHandler::GetKing() {
	return UnitView(state->GetKing(player_id).get());
}

EnemyUnitView PlayerStateHandler::GetEnemyKing(int * success) {
	auto enemy_king = state->GetEnemyKing(player_id, success);
	if (enemy_king != nullptr)
		return EnemyUnitView(enemy_king.get());
	return EnemyUnitView();
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
	if (actor != nullptr)
		return EnemyUnitView(actor.get());
	return EnemyUnitView();
}

list_act_id_t PlayerStateHandler::GetRespawnables() {
	return state->GetRespawnables(player_id);
}

void PlayerStateHandler::MoveUnits(
	list_act_id_t unit_ids,
	physics::Vector2D destination,
	std::shared_ptr<FormationMaker> formation_maker,
	std::vector<int64_t> terrain_weights,
	int * success
) {
	state->MoveUnits(
		player_id,
		unit_ids,
		destination,
		formation_maker,
		terrain_weights,
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

int64_t PlayerStateHandler::PlanPath(
	physics::Vector2D start,
	physics::Vector2D destination,
	std::vector<int64_t> terrain_weights
) {
	return state->PlanPath(
			   start,
			   destination,
			   terrain_weights
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
