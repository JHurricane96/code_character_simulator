/**
 * @file tower.h
 * Defines the Tower class
 */

#ifndef STATE_ACTOR_TOWER_H
#define STATE_ACTOR_TOWER_H

#include <cstdint>
#include "actor/actor.h"
#include "actor/fire_ball.h"
#include "state_export.h"

namespace state {

/**
 * Describes the possible owners of the tower
 */
enum class TowerOwner{
	PLAYER1,
	PLAYER2,
	UNOWNED,
};

/**
 * Class for Towers
 */
class STATE_EXPORT Tower: public Actor {
private:
	/**
	 * The tower's owner
	 */
	TowerOwner tower_owner;
	/**
	 * The tower's previous owner
	 */
	TowerOwner prev_tower_owner;
	/**
	 * Radius around tower that units need to be present to contend for
	 * tower ownership
	 * Measured in coordinates
	 */
	int64_t contention_radius;
	/**
	 * The contention score a player must reach to attain tower ownership
	 * Calculated as sum of difference in number of units over time
	 */
	int64_t max_contention_score;
	/**
	 * The present contention score
	 * Varies between -max_contention_score and max_contention_score
	 */
	float contention_score;
	/**
	 * Speed of a fire_ball fired from the tower
	 */
	int64_t fire_ball_speed;
	/**
	 * Time to live for the fire_balls fired
	 */
	int64_t fire_ball_ttl;
	/**
	 * Size of the fire_balls fired
	 */
	int64_t fire_ball_size;
	/**
	 * True if ready to fire fire_ball, false otherwise
	 */
	bool ready_to_attack;
public:
	Tower (
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
	);
	/**
	 * The tower's attack function
	 * Sets the ready_to_attack bool to true
	 */
	void Attack() override;
	/**
	 * The Tower's StopAttack function
	 *
	 * Is called when target is dead
	 * Sets ready_to_attack bool to false
	 */
	void StopAttack() override;
	/**
	 * The Tower's death method
	 *
	 * Similar to Actor's death method except
	 * - position isn't reset
	 * - contention_time_elapsed is set to 0
	 */
	void Die() override;
	/**
	 * Determines if ready to attack.
	 * Getter for ready_to_attack
	 *
	 * @return     True if ready to attack, False otherwise.
	 */
	bool IsReadyToAttack();
	/**
	 * Gets a fire_ball's lifetime
	 *
	 * @return     The time to live for the fire_ball
	 */
	int64_t GetFireBallTtl();
	/**
	 * Gets the tower's owner
	 *
	 * @return     The tower's owner
	 */
	TowerOwner GetTowerOwner();
	/**
	 * Gets the tower's previous owner
	 *
	 * @return     The tower's previous owner
	 */
	TowerOwner GetPrevTowerOwner();
	/**
	 * Gets the speed of a fire_ball fired from the tower
	 *
	 * @return     The fire_ball speed
	 */
	int64_t GetFireBallSpeed();
	/**
	 * Gets the size of a fire_ball fired from the tower
	 *
	 * @return     The fire_ball size
	 */
	int64_t GetFireBallSize();
	/**
	 * Gets the contention score for the tower
	 *
	 * @return     The contention_score
	 */
	float GetContentionScore();
	/**
	 * Gets the maximum contention score for the tower
	 *
	 * @return     The max_contention_score
	 */
	int64_t GetMaxContentionScore();
	/**
	 * Gets the contention radius of the tower
	 *
	 * @return     The contention_radius
	 */
	int64_t GetContentionRadius();
	/**
	 * Sets ready_to_attack to false
	 */
	void SetReadyToAttackToFalse();
	/**
	 * Respawns Tower
	 *
	 * @param[in]  pid   The tower owner id
	 */
	void Respawn(TowerOwner pid);
	/**
	 * Is called when units are fighting over a tower after its health has
	 * been depleted to zero
	 *
	 * When one player's units completely occupy the vicinity of the tower,
	 * that player attains ownership of the tower
	 *
	 * @param[in]  actors  The actors owned by the players
	 *
	 * @return     True if new owner is settled, else false
	 */
	bool Contend(
		float delta_time,
		std::vector<std::vector<std::shared_ptr<Actor> > > actors
	);
	/**
	 * Update function to be called every tick
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	void Update(float delta_time) override;
	/**
	 * Merges this, a Tower in a player's state, with the corresponding
	 * Tower in the main state
	 *
	 * @param[in]  tower   The main state's Tower
	 */
	void MergeWithMain(const Tower * tower);
};

}

#endif
