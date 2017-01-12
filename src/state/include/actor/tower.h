/**
 * @file tower.h
 * Defines the Tower class
 */

#ifndef STATE_ACTOR_TOWER_H
#define STATE_ACTOR_TOWER_H

#include <cstdint>
#include "actor/actor.h"
#include "actor/arrow.h"
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
	int64_t contention_score;
	/**
	 * Speed of an arrow fired from the tower
	 */
	int64_t arrow_speed;
	/**
	 * Time to live for the arrows fired
	 */
	int64_t arrow_ttl;
	/**
	 * Size of the arrows fired
	 */
	int64_t arrow_size;
	/**
	 * True if ready to fire arrow, false otherwise
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
	    int64_t time_to_respawn,
	    int64_t time_spent_near_base,
	    physics::Vector2D position,
	    physics::Vector2D velocity,
	    int64_t los_radius,
	    int64_t attack_speed,
	    int64_t contention_radius,
	    int64_t max_contention_score,
	    int64_t range,
	    int64_t arrow_speed,
	    int64_t arrow_ttl,
	    int64_t arrow_size
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
	 * Gets an arrow's lifetime
	 *
	 * @return     The timt to live for the arrow
	 */
	int64_t GetArrowTtl();
	/**
	 * Gets the tower's owner
	 *
	 * @return     The tower's owner
	 */
	TowerOwner GetTowerOwner();
	/**
	 * Gets the speed of an arrow fired from the tower
	 *
	 * @return     The arrow speed
	 */
	int64_t GetArrowSpeed();
	/**
	 * Gets the size of an arrow fired from the tower
	 *
	 * @return     The arrow size
	 */
	int64_t GetArrowSize();
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
		int64_t delta_time,
		std::vector<std::vector<std::shared_ptr<Actor> > > actors
	);
	/**
	 * Update function to be called every tick
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	void Update(int64_t delta_time);
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
