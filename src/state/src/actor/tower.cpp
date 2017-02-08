#include "actor/tower.h"
#include <algorithm>

namespace state {

Tower::Tower(
	act_id_t id,
	PlayerId player_id,
	int64_t attack,
	int64_t hp,
	int64_t max_hp,
	int64_t max_speed,
	int64_t size,
	int64_t total_respawn_time,
	float time_to_respawn,
	int64_t time_spent_near_base,
	physics::Vector2D position,
	physics::Vector2D velocity,
	int64_t los_radius,
	int64_t attack_speed,
	int64_t contention_radius,
	int64_t max_contention_score,
	int64_t range,
	int64_t fire_ball_speed,
	int64_t fire_ball_ttl,
	int64_t fire_ball_size
	) :
	Actor(
		id,
		player_id,
		attack,
		hp,
		max_hp,
		max_speed,
		size,
		total_respawn_time,
		time_to_respawn,
		time_spent_near_base,
		position,
		velocity,
		los_radius,
		attack_speed,
		range,
		ActorType::TOWER,
		true,
		false
	),
	tower_owner(static_cast<TowerOwner>(player_id)),
	prev_tower_owner(static_cast<TowerOwner>(player_id)),
	contention_radius(contention_radius),
	max_contention_score(max_contention_score),
	contention_score(0),
	fire_ball_speed(fire_ball_speed),
	fire_ball_ttl(fire_ball_ttl),
	fire_ball_size(fire_ball_size),
	ready_to_attack(false) {}

bool Tower::Contend(
	float delta_time,
	std::vector<std::vector<std::shared_ptr<Actor> > > actors
) {
	std::vector<int64_t> count(LAST_PLAYER + 1, 0);
	for (int pid = 0; pid <= LAST_PLAYER; pid++) {
		for (auto actor : actors[pid])
			if (actor->GetId() != id &&
				actor->GetPosition().distance(position) < contention_radius)
				count[pid]++;
	}
	contention_score += (count[0] - count[1]) * delta_time;
	if (std::abs(contention_score) >= max_contention_score) {
		prev_tower_owner = tower_owner;
		Respawn(static_cast<TowerOwner>((contention_score < 0)));
		return true;
	}
	return false;
}

void Tower::Die() {
	is_dead = true;
	time_to_respawn = 0;
	hp = 0;
	time_spent_near_base = 0;
	respawn_location = nullptr;
	attack_target = nullptr;
	contention_score = 0;
}

void Tower::Attack() {
	ready_to_attack = true;
}

void Tower::StopAttack() {
	ready_to_attack = false;
	attack_target = nullptr;
}

bool Tower::IsReadyToAttack() {
	return ready_to_attack;
}

int64_t Tower::GetFireBallTtl() {
	return fire_ball_ttl;
}

TowerOwner Tower::GetTowerOwner() {
	return tower_owner;
}

TowerOwner Tower::GetPrevTowerOwner() {
	return prev_tower_owner;
}

int64_t Tower::GetFireBallSize() {
	return fire_ball_size;
}

int64_t Tower::GetFireBallSpeed() {
	return fire_ball_speed;
}

int64_t Tower::GetContentionRadius() {
	return contention_radius;
}

float Tower::GetContentionScore() {
	return contention_score;
}

int64_t Tower::GetMaxContentionScore() {
	return max_contention_score;
}

void Tower::SetReadyToAttackToFalse() {
	ready_to_attack = false;
}

void Tower::Respawn(TowerOwner pid) {
	is_dead = false;
	hp = max_hp;
	tower_owner = pid;
	player_id = static_cast<PlayerId>(pid);
	contention_score = 0;
}

void Tower::Update(float delta_time) {
	DecideState(delta_time);
};

void Tower::MergeWithMain(const Tower * tower) {
	tower_owner = tower->tower_owner;
	prev_tower_owner = tower->prev_tower_owner;
	contention_score = tower->contention_score;
	ready_to_attack = tower->ready_to_attack;
}

}
