#ifndef BELIEF_HPP
#define BELIEF_HPP
#include <vector>
#include <stdexcept>
#include <random> 

struct Belief {
    virtual ~Belief() = default;
}

struct GaussianBelief : Belief {

}

struct ParticleFilterBelief: Belief {
    std::vector<State> particles;
    std::vector<double> weights;
    
    static int sample_from_particles(const std::vector<double>& weights){
        double weight_sum = 0.0;
        for (double w : weights) {
            weight_sum += w;
        }
        
        // Avoid division by zero if the sum of weights is zero
        if (weight_sum == 0.0) {
            throw std::invalid_argument("Sum of particle weights cannot be zero.");
        }

        // Normalize weights to sum to 1
        std::vector<double> normalized_weights(weights.size());
        for (size_t i = 0; i < weights.size(); ++i) {
            normalized_weights[i] = weights[i] / weight_sum;
        }

         // Step 2: Compute the cumulative distribution
        std::vector<double> cumulative_distribution(weights.size());
        cumulative_distribution[0] = normalized_weights[0];
        for (size_t i = 1; i < weights.size(); ++i) {
            cumulative_distribution[i] = cumulative_distribution[i - 1] + normalized_weights[i];
        }

        // Step 3: Generate a random number between 0 and 1
        double random_value = static_cast<double>(rand()) / RAND_MAX;  // Random value between 0 and 1

        // Step 4: Find the corresponding particle
        for (size_t i = 0; i < cumulative_distribution.size(); ++i) {
            if (random_value < cumulative_distribution[i]) {
                return i;  // Return the index of the selected particle
            }
        }

        // If no particle is selected (should not happen unless cumulative distribution is incorrect)
        return weights.size() - 1;  // Return the last particle as a fallback
    }
    
    static State sample_from_belief(ParticleFilterBelief belief){
        int index = sample_from_particles(belief.weights);
        return belief.particles[index];
    }
}
#endif