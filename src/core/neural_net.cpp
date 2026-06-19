#include "neural_net.h"
#include <cmath>
#include <random>
#include <fstream>

NeuralNet::NeuralNet(const std::vector<int>& sizes) : layer_sizes(sizes) {
    std::mt19937 gen(42); 
    
    int num_layers = sizes.size();
    weights.resize(num_layers - 1);
    biases.resize(num_layers - 1);
    z.resize(num_layers);
    a.resize(num_layers);
    
    for (int i = 0; i < num_layers; ++i) {
        z[i].resize(sizes[i], 0.0f);
        a[i].resize(sizes[i], 0.0f);
    }
    
    for (int i = 0; i < num_layers - 1; ++i) {
        int in_size = sizes[i];
        int out_size = sizes[i + 1];
        
        std::normal_distribution<float> d(0.0f, std::sqrt(2.0f / in_size)); // He Init
        
        weights[i].resize(in_size, std::vector<float>(out_size));
        for (int j = 0; j < in_size; ++j) {
            for (int k = 0; k < out_size; ++k) {
                weights[i][j][k] = d(gen);
            }
        }
        biases[i].assign(out_size, 0.01f);
    }
}

float NeuralNet::relu(float x) { return x > 0.0f ? x : 0.01f * x; }
float NeuralNet::relu_deriv(float x) { return x > 0.0f ? 1.0f : 0.01f; }

std::vector<float> NeuralNet::forward(const std::vector<float>& input) {
    a[0] = input;
    z[0] = input;
    
    for (size_t i = 0; i < weights.size(); ++i) {
        int in_size = layer_sizes[i];
        int out_size = layer_sizes[i + 1];
        
        z[i+1].assign(out_size, 0.0f);
        for (int k = 0; k < out_size; ++k) {
            float sum = biases[i][k];
            for (int j = 0; j < in_size; ++j) {
                sum += a[i][j] * weights[i][j][k];
            }
            z[i+1][k] = sum;
        }
        
        a[i+1].resize(out_size);
        for (int k = 0; k < out_size; ++k) {
            if (i == weights.size() - 1) {
                a[i+1][k] = z[i+1][k]; // Linear activation for output
            } else {
                a[i+1][k] = relu(z[i+1][k]);
            }
        }
    }
    return a.back();
}

void NeuralNet::backward(const std::vector<float>& target, float lr) {
    int num_layers = layer_sizes.size();
    std::vector<std::vector<float>> deltas(num_layers);
    
    int out_size = layer_sizes.back();
    deltas.back().resize(out_size);
    for (int i = 0; i < out_size; ++i) {
        deltas.back()[i] = a.back()[i] - target[i];
    }
    
    for (int l = num_layers - 2; l >= 0; --l) {
        int layer_size = layer_sizes[l];
        int next_size = layer_sizes[l + 1];
        
        deltas[l].assign(layer_size, 0.0f);
        for (int j = 0; j < layer_size; ++j) {
            float sum = 0.0f;
            for (int k = 0; k < next_size; ++k) {
                sum += weights[l][j][k] * deltas[l + 1][k];
            }
            deltas[l][j] = sum * relu_deriv(z[l][j]);
        }
    }
    
    for (int l = 0; l < num_layers - 1; ++l) {
        int in_size = layer_sizes[l];
        int out_size = layer_sizes[l + 1];
        
        for (int k = 0; k < out_size; ++k) {
            biases[l][k] -= lr * deltas[l + 1][k];
            for (int j = 0; j < in_size; ++j) {
                weights[l][j][k] -= lr * a[l][j] * deltas[l + 1][k];
            }
        }
    }
}

void NeuralNet::save(const std::string& filename) const {
    std::ofstream out(filename, std::ios::binary);
    for (size_t i = 0; i < weights.size(); ++i) {
        for (size_t j = 0; j < weights[i].size(); ++j) {
            out.write(reinterpret_cast<const char*>(weights[i][j].data()), weights[i][j].size() * sizeof(float));
        }
        out.write(reinterpret_cast<const char*>(biases[i].data()), biases[i].size() * sizeof(float));
    }
}

void NeuralNet::load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return;
    for (size_t i = 0; i < weights.size(); ++i) {
        for (size_t j = 0; j < weights[i].size(); ++j) {
            in.read(reinterpret_cast<char*>(weights[i][j].data()), weights[i][j].size() * sizeof(float));
        }
        in.read(reinterpret_cast<char*>(biases[i].data()), biases[i].size() * sizeof(float));
    }
}
