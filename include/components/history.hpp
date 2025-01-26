#ifndef HISTORY_HPP
#define HISTORY_HPP

struct History {
    std::vector<State> states;         // Sequence of states in the history
    std::vector<Action> actions;       // Sequence of actions in the history
    std::vector<Observation> obs;      // Sequence of observations in the history
    
    // Concatenate a state to the history
    History operator|(const State& s) const {
        History new_history = *this;
        new_history.states.push_back(s); // Append state
        return new_history;
    }

    // Concatenate an action to the history
    History operator|(const Action& a) const {
        History new_history = *this;
        new_history.actions.push_back(a); // Append action
        return new_history;
    }

    // Concatenate an observation to the history
    History operator|(const Observation& o) const {
        History new_history = *this;
        new_history.obs.push_back(o); // Append observation
        return new_history;
    }

    // Print history for debugging
    void print() const {
        std::cout << "States: ";
        for (const auto& s : states) {
            std::cout << s << " "; // Assuming State has operator<< defined
        }
        std::cout << "\nActions: ";
        for (const auto& a : actions) {
            std::cout << a << " "; // Assuming Action has operator<< defined
        }
        std::cout << "\nObservations: ";
        for (const auto& o : obs) {
            std::cout << o << " "; // Assuming Observation has operator<< defined
        }
        std::cout << "\n";
    }
};

#endif