#ifndef PFT_DPW_HPP
#define PFT_DPW_HPP
PFGenerativeResult sample_uniform_from(){
    
}
double simulate_pft_dpw(Belief b, int d){
    if (d == 0){
        return 0;
    }
    Action a = action_prog_widen(b);
    if(C(ha).size() <= k_o * std::pow(N(ba), alpha_o)){
        PFGenerativeResult generative_result = G_pf_m(b, a);
        C(ba) = C(ba) + {(generative_result.b_tilde, generative_result.reward)};
        total = r + gamma * rollout(b_tilde, d - 1);
    } else {
        PFGenerativeResult generative_result = sample_uniform_from(C(ba));
        total = r + gamma * simulate_pft_dpw(generative_result.b_tilde, d - 1);
    }
    N(b) = N(b) + 1;
    N(ba) = N(ba) + 1;
    Q(ba) = Q(ba) + (total - Q(ba)) / (N(ba));
    return total;
}
#endif