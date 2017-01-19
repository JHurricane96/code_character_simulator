#ifndef DRIVER_H
#define DRIVER_H

#include "../../state/include/state.h"
#include "../../state/include/player_state_handler/player_state_handler.h"

namespace DRIVERS {

class MainDriver {
private:
    /**
    * State objects state, p1_buffer, p2_buffer are the main game state, 
    * player 1 update buffer and player 2 update buffer respectively
    */
    state::State state, p1_buffer, p2_buffer;
    /**
    * PlayerStateHandler object state_view is the restricted view of the game
    * state given to the players
    */
    state::PlayerStateHandler state_view(&state);
    /**
    * bool variables is_p1_done and is_p2_done are simple objects which tells
    * whether the modification in the respective buffer is complete and whether
    * it is ready for merging
    */
    bool is_p1_done, is_p2_done;
    PlayerDriver p1_driver(&state_view, &p1_buffer, &is_p1_done);
    PlayerDriver p2_driver(&state_view, &p2_buffer, &is_p2_done);
    /**
    * GlobalUpdate is an infinite while loop handling the main game updates and
    * also the updates from the player AI
    * note that this function will merge buffers to the main state only if
    * their respective is_done flags are true and after merge this function
    * clears the flag for further updates
    */
    void GlobalUpdate();
    /**
    * MergeStates is a function which is called to merge two states
    */
    void MergeStates(state::State *main_state, state::State *to_merge_state);
    /**
    * Log is a function to monitor usage and efficieny of player AI and their
    * threads
    */
    void Log();
public:
    MainDriver();
    /**
    * Creates a thread whose handler function is the GlobalUpdate
    */
    void Run();
};

class PlayerDriver {
private:
    /**
    * PlayerStateHandler object view_state is the restricted game state view
    * for the players to create their AI
    */
    state::PlayerStateHandler *view_state;
    /**
    * State object modify_state is the buffer in which the player AI writes all
    * its updates
    */
    state::State *modify_state;
    /**
    * bool is_done if true says that an update is already in queue and to be
    * cleared by the GlobalUpdate and if false says that update can be done
    */
    bool *is_done;
public:
    PlayerDriver(state::PlayerStateHandler *handler, state::State *buffer, bool *done);
    /**
    * update can only be done is is_done is false
    * all AI updates are supposed to be done here
    * is_done is set to true at the end of update
    */
    void Update();
    /**
    * creates a thread whose handler function is the Update
    */
    void Run();
};

}

#endif
