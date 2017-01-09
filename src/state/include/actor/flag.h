/**
 * @file flag.h
 * Defines the Flag Class
 */

#ifndef STATE_ACTOR_FLAG_H
#define STATE_ACTOR_FLAG_H

#include <cstdint>
#include "actor/actor.h"
#include "actor/flag.fwd.h"
#include "actor/king.fwd.h"
#include "state_export.h"

namespace state {
/**
 * Class for Flags that the players try to capture
 */
class STATE_EXPORT Flag: public Actor {
private:
	/**
	 * King that carries this flag
	 *
	 * nullptr if the flag isn't being carried
	 */
	King * king;
public:
	Flag(
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
		int64_t attack_speed
	);
	/**
	 * Assigns king to this->king
	 *
	 * @param      king  The king
	 */
	void Capture(King * king);
	/**
	 * Sets the flag's king to nullptr
	 */
	void Drop();
	/**
	 * Determines if this flag has been captured
	 *
	 * @return     false if king is nullptr, true otherwise
	 */
	bool IsCaptured();
	/**
	 * Update function to be called every tick
	 *
	 * @param[in]  delta_time  The difference in time between the
	 *                         previous and current Update calls
	 */
	void Update(int64_t delta_time);
	/**
	 * Merges this, a Flag in the main state, with the corresponding
	 * Flag in a player's state
	 *
	 * @param[in]  flag    The player's Flag
	 * @param[in]  actors  The player's state's Actors. Any
	 *                     references to the other state's Actors
	 *                     are replaced by this state's Actors
	 */
	void MergeWithBuffer(
		const Flag * flag,
		std::vector<std::shared_ptr<Actor> > actors
	);
	/**
	 * Merges this, a Flag in a player's state, with the corresponding
	 * Flag in the main state
	 *
	 * @param[in]  flag    The main state's Flag
	 * @param[in]  actors  The player's state's Actors. Any
	 *                     references to the other state's Actors
	 *                     are replaced by this state's Actors
	 */
	void MergeWithMain(
		const Flag * flag,
		std::vector<std::shared_ptr<Actor> > actors
	);
};

}


#endif
