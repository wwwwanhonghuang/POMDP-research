#include "algorithms/pomdp_cpow.hpp"
double simulate_pomcp_cpow(State s, History h, int d){
    if(d == 0) return 0;
    Action a = action_prog_widen(h);
    GenerativeResult generative_result = G(s, a);
    if(C(ha).size() <= k_o * std::pow(N(ha), alpha_o)){
        M(hao) = M(hao) + 1
    } else {
        o = select_wp(C(ha), M(hao)/sum_o(M(hao)))
    }
    B(hao).append(generative_result.s_tilde);
    W(hao).append(Z(o|s,a,s_tilde));
    if(!C(ha).contains(o)){ // new_node
        C(ha) = C(ha) + {o};
        total = r + gamma * rollout(generative_result.s_tilde, hao, d - 1);
    }
    N(h) = N(h) + 1;
    N(ha) = N(ha) + 1;
    Q(ha) = Q(ha) + (total - Q(ha) / N(ha));
    return total;
}