//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   L A Y E R   C L A S S   H E A D E R
//
//   Artificial Intelligence Techniques SL
//   artelnics@artelnics.com

#ifndef LAYER_H
#define LAYER_H

// System includes

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>

// OpenNN includes

#include "vector.h"
#include "matrix.h"
#include "tensor.h"

#include "tinyxml2.h"

namespace OpenNN {

/// This abstract class represents the concept of layer of neurons in OpenNN.

/// Layer is a group of neurons having connections to the same inputs and sending outputs to the same destinations.
/// Also is used to store information about the layers of the different architectures of NeuralNetworks.

class Layer
{

public:

    // Enumerations

    /// This enumeration represents the possible types of layers.

    enum Type{Scaling, Convolutional, Perceptron, Pooling, Probabilistic, LongShortTermMemory,Recurrent, Unscaling, Bounding, PrincipalComponents};

    /// This structure represents the first order activaions of layers.

    struct ForwardPropagation
    {
        /// Default constructor.

        explicit ForwardPropagation()
        {
        }


        virtual ~ForwardPropagation()
        {
        }

        void print() const
        {
            cout << "Combinations:" << endl;
            cout << combinations << endl;

            cout << "Activations:" << endl;
            cout << activations << endl;

            cout << "Activation derivatives:" << endl;
            cout << activations_derivatives << endl;
        }

        Tensor<double> combinations;

        Tensor<double> activations;

        Tensor<double> activations_derivatives;
    };


    // Constructor

    explicit Layer() {}

    // Destructor

    virtual ~Layer() {}

    // Parameters initialization methods

    virtual void initialize_parameters(const double&);

    virtual void randomize_parameters_uniform(const double& = -1.0, const double& = 1.0);
    virtual void randomize_parameters_normal(const double& = 0.0, const double& = 1.0);

    // Architecture

    virtual Vector<double> get_parameters() const;
    virtual size_t get_parameters_number() const;

    virtual void set_parameters(const Vector<double>&);

    // Outputs

    virtual Tensor<double> calculate_outputs(const Tensor<double>&);
    virtual Tensor<double> calculate_outputs(const Tensor<double>&, const Vector<double>&);

    virtual Vector<double> calculate_error_gradient(const Tensor<double>&, const Layer::ForwardPropagation&, const Tensor<double>&);

    virtual ForwardPropagation calculate_forward_propagation(const Tensor<double>&);

    virtual void calculate_forward_propagation(const Tensor<double>&, ForwardPropagation&) {}

    // Deltas

    virtual Tensor<double> calculate_output_delta(const Tensor<double>&, const Tensor<double>&) const;

    virtual Tensor<double> calculate_hidden_delta(Layer*,
                                                  const Tensor<double>&,
                                                  const Tensor<double>&,
                                                  const Tensor<double>&) const;

    // Get neurons number

    virtual Vector<size_t> get_input_variables_dimensions() const;

    virtual size_t get_inputs_number() const;
    virtual size_t get_neurons_number() const;

    virtual void set_inputs_number(const size_t&);
    virtual void set_neurons_number(const size_t&);

    virtual string object_to_string() const;

    // Layer type

    Type get_type() const;

    string get_type_string() const;

protected:

        /// Layer type object.

        Type layer_type = Perceptron;
};
}

#endif // __LAYER_H
