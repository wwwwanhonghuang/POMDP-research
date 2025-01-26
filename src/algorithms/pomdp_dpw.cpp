#include "algorithms/pomcp_dpw.hpp"
double k_o = 1;
double gamma = 1;
double alpha_o = 1;

double simulate_pomcp_dpw(State s, History h, int d){
    // Base case: if depth d == 0, return 0 (end of recursion).
    if (d == 0) return 0;
    
    // Step 1: Sample an action based on the current history.
    Action a = action_prog_widen(h); // This function decides the action to take based on the history 'h'.
    
    // Step 2: Append the selected action to the history.
    History appended_a_history = h | a;  // Concatenate the action 'a' to the history.

    // Step 3: Check if the history-action pair has been explored sufficiently.
    // This comparison seems to use a threshold based on the number of visits (N) and a scaling factor (alpha_o).
    if (C(appended_a_history).size() <= k_o * std::pow(N(appended_a_history), alpha_o)){
        
        // Step 4: If not enough exploration has been done, generate a new state, observation, and reward.
        GenerativeResult generative_result = G(s, a);  // The generative model G returns the new state (s'), observation (o), and reward (r).
        
        // Step 5: Update the history with the generated observation.
        History appended_a_o_history = appended_a_history | generative_result.o;  // Append the observation 'o' to history.

        // Step 6: Update the counts for the action-observation pair.
        C(appended_a_history) = C(appended_a_history) + {generative_result.o};  // Update the count of the observation.
        M(appended_a_o_history) = M(appended_a_o_history) + 1;  // Update the count of the action-observation pair.
        
        // Step 7: Append the belief state (new state) to the belief store.
        B(appended_a_o_history).append(generative_result.s_tilde);  // Append the new state (s_tilde) to the belief.

        // Step 8: Check if this observation has appeared before.
        if (M(h | a | generative_result.o) == 1) {
            // If this observation has not been encountered before, perform a rollout using the generative model's result.
            total = generative_result.r + gamma * rollout(generative_result.s_tilde, appended_a_o_history, d - 1);
        } else {
            // If the observation has been seen before, recursively simulate with the new state.
            total = generative_result.r + gamma * simulate_pomcp_dpw(generative_result.s_tilde, appended_a_o_history, d - 1);
        }
    } else {
        // Step 9: If the exploration condition is not met, reject the observation and select a new one.
        Observation o = select_wp_free_observation(C(appended_a_history), 
            [](Observation obs) -> double { 
                return M(appended_a_history | obs);  // Select observation based on how often it has been encountered.
                // M_sum_o can be used here to adjust the observation selection.
            });

        // Step 10: Sample a new belief state.
        State s_tilde = select_wp_free_observation(B(appended_a_history), 
            [](Observation obs) -> double { return 1; }  // This function samples from the belief.
        );

        // Step 11: Calculate the reward for the new state, action, and sampled belief state.
        double r = R(s, a, s_tilde);  // Compute reward based on current state, action, and sampled state.

        // Step 12: Continue the simulation with the new belief and observation.
        total = r + gamma * simulate_pomcp_dpw(s_tilde, appended_a_o_history | o, d - 1);
    }
    
    // Step 13: Update the visit counts for the history and action-history pair.
    N(h) = N(h) + 1;  // Increment the count for the original history.
    N(appended_a_history) = N(appended_a_history) + 1;  // Increment the count for the history-action pair.
    
    // Step 14: Update the Q-value (expected reward) for the action-history pair.
    Q(appended_a_history) = Q(appended_a_history) + (total - Q(appended_a_history)) / N(appended_a_history);
    
    // Step 15: Return the total reward computed in this iteration.
    return total;
}
