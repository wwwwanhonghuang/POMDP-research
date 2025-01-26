#include "algorithms/pomcp_dpw.hpp"

double simulate_pomcp_dpw(State s, History h, int d){
    if (d == 0) return 0;
    Action a = action_prog_widen(h);
    if (C(ha).size() <= k_o * std::pow(N(ha), alpha_o)){
        GenerativeResult generative_result = G(s, a);
        C(ha) = C(ha) + {o};
        M(hao) = M(hao) + 1
        B(hap).append(generative_result.s_tilde);
        if (M(hao) == 1){
            total = generative_result.r + gamma * rollout(generative_result.s_tilde, hao, d - 1);
        } else {
            total = generative_result.r + gamma * simulate_pomcp_dpw(generative_result.s_tilde, hao, d - 1);
        }
    }else{ // observation is rejected
    // tree search is continued with an observation selected in proportion to the number of times
        Observation o = select_wp(C(ha), M(hao)/M_sum_o(hao)); // select observation, with respective to M
        State s_tilde =  select_wp(B(ha), 1/B_sum_o(hao)); // sample a belief
        r = R(s, a, s_tilde); // calculate reward
        total = r + gamma * simulate_pomcp_dpw(s_tilde, hao, d - 1);
         
        r = R(s, a, s_tilde);
    }
    N(h) = N(h) + 1;
    N(ha) = N(ha) + 1;
    Q(ha) = Q(ha) + (total - Q(ha) / N(ha));
}