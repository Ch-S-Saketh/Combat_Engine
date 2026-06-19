#pragma once
#include <vector>
#include <string>

class NeuralNet {
public:
    NeuralNet(const std::vector<int>& layer_sizes);
    
    std::vector<float> forward(const std::vector<float>& input);
    void backward(const std::vector<float>& target, float learning_rate);
    
    void save(const std::string& filename) const;
    void load(const std::string& filename);

    // Get layer activations for visualization
    const std::vector<std::vector<float>>& get_activations() const { return a; }
    const std::vector<int>& get_layer_sizes() const { return layer_sizes; }

private:
    std::vector<int> layer_sizes;
    std::vector<std::vector<std::vector<float>>> weights; // weights[layer][node_in][node_out]
    std::vector<std::vector<float>> biases; // biases[layer][node]
    
    // Cache for backprop
    std::vector<std::vector<float>> z; // Pre-activations
    std::vector<std::vector<float>> a; // Post-activations
    
    float relu(float x);
    float relu_deriv(float x);
};
