//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   W E I G T H E D   S Q U A R E D   E R R O R   C L A S S               
//
//   Artificial Intelligence Techniques SL
//   artelnics@artelnics.com

#include "weighted_squared_error.h"

namespace OpenNN
{

/// Default constructor. 
/// It creates a weighted squared error term not associated to any
/// neural network and not measured on any data set.
/// It also initializes all the rest of class members to their default values.

WeightedSquaredError::WeightedSquaredError() : LossIndex()
{
    set_default();
}


/// Neural network constructor. 
/// It creates a weighted squared error term object associated to a
/// neural network object but not measured on any data set object.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.

WeightedSquaredError::WeightedSquaredError(NeuralNetwork* new_neural_network_pointer)
    : LossIndex(new_neural_network_pointer)
{
    set_default();
}


/// Data set constructor. 
/// It creates a weighted squared error term not associated to any
/// neural network but to be measured on a given data set object.
/// It also initializes all the rest of class members to their default values.
/// @param new_data_set_pointer Pointer to a data set object.

WeightedSquaredError::WeightedSquaredError(DataSet* new_data_set_pointer)
    : LossIndex(new_data_set_pointer)
{
    set_default();
}


/// Neural network and data set constructor. 
/// It creates a weighted squared error term object associated to a
/// neural network and measured on a data set.
/// It also initializes all the rest of class members to their default values.
/// @param new_neural_network_pointer Pointer to a neural network object.
/// @param new_data_set_pointer Pointer to a data set object.

WeightedSquaredError::WeightedSquaredError(NeuralNetwork* new_neural_network_pointer, DataSet* new_data_set_pointer)
    : LossIndex(new_neural_network_pointer, new_data_set_pointer)
{
    set_default();
}


/// XML constructor. 
/// It creates a weighted squared error object with all pointers set to nullptr.
/// The object members are loaded by means of a XML document.
/// Please be careful with the format of that file, which is specified in the OpenNN manual.
/// @param weighted_squared_error_document TinyXML document with the weighted squared error elements.

WeightedSquaredError::WeightedSquaredError(const tinyxml2::XMLDocument& weighted_squared_error_document)
    : LossIndex(weighted_squared_error_document)
{
    set_default();

    from_XML(weighted_squared_error_document);
}


/// Copy constructor. 
/// It creates a copy of an existing weighted squared error object.
/// @param other_weighted_squared_error Weighted squared error object to be copied.

WeightedSquaredError::WeightedSquaredError(const WeightedSquaredError& other_weighted_squared_error)
    : LossIndex(other_weighted_squared_error)
{
    negatives_weight = other_weighted_squared_error.negatives_weight;
    positives_weight = other_weighted_squared_error.positives_weight;

    training_normalization_coefficient = other_weighted_squared_error.training_normalization_coefficient;
    selection_normalization_coefficient = other_weighted_squared_error.selection_normalization_coefficient;
}


/// Destructor.

WeightedSquaredError::~WeightedSquaredError()
{
}


/// Returns the weight of the positives.

type WeightedSquaredError::get_positives_weight() const
{
    return(positives_weight);
}


/// Returns the weight of the negatives.

type WeightedSquaredError::get_negatives_weight() const
{
    return(negatives_weight);
}


/// Returns the normalization coefficient.

type WeightedSquaredError::get_training_normalization_coefficient() const
{
    return(training_normalization_coefficient);
}


/// Set the default values for the object.

void WeightedSquaredError::set_default()
{
    if(has_data_set() && data_set_pointer->has_data())
    {
        set_weights();

        set_training_normalization_coefficient();

        set_selection_normalization_coefficient();
    }
    else
    {
        negatives_weight = 1.0;
        positives_weight = 1.0;

        training_normalization_coefficient = 1.0;
        selection_normalization_coefficient = 1.0;
    }
}


/// Set a new weight for the positives values.
/// @param new_positives_weight New weight for the positives.

void WeightedSquaredError::set_positives_weight(const type& new_positives_weight)
{
    positives_weight = new_positives_weight;
}


/// Set a new weight for the negatives values.
/// @param new_negatives_weight New weight for the negatives.

void WeightedSquaredError::set_negatives_weight(const type& new_negatives_weight)
{
    negatives_weight = new_negatives_weight;
}


/// Set a new normalization coefficient.
/// @param new_training_normalization_coefficient New normalization coefficient.

void WeightedSquaredError::set_training_normalization_coefficient(const type& new_training_normalization_coefficient)
{
    training_normalization_coefficient = new_training_normalization_coefficient;
}


/// Set new weights for the positives and negatives values.
/// @param new_positives_weight New weight for the positives.
/// @param new_negatives_weight New weight for the negatives.

void WeightedSquaredError::set_weights(const type& new_positives_weight, const type& new_negatives_weight)
{
    positives_weight = new_positives_weight;
    negatives_weight = new_negatives_weight;
}


/// Calculates of the weights for the positives and negatives values with the data of the data set.

void WeightedSquaredError::set_weights()
{
    // Control sentence

#ifdef __OPENNN_DEBUG__

    check();

#endif

    const Tensor<Index, 1> target_distribution = data_set_pointer->calculate_target_distribution();

    const Index negatives = target_distribution[0];
    const Index positives = target_distribution[1];

    if(positives == 0 || negatives == 0)
    {
        positives_weight = 1.0;
        negatives_weight = 1.0;

        return;
    }

    negatives_weight = 1.0;
    positives_weight = static_cast<type>(negatives)/static_cast<type>(positives);
}


/// Calculates of the normalization coefficient with the data of the data set.

void WeightedSquaredError::set_training_normalization_coefficient()
{
    // Control sentence

#ifdef __OPENNN_DEBUG__

    check();

#endif

    const Tensor<Index, 1> target_variables_indices = data_set_pointer->get_target_variables_indices();

    const Index negatives = data_set_pointer->calculate_training_negatives(target_variables_indices[0]);

    training_normalization_coefficient = negatives*negatives_weight*static_cast<type>(0.5);
}


/// Calculates of the selection normalization coefficient with the data of the data set.

void WeightedSquaredError::set_selection_normalization_coefficient()
{
    // Control sentence

#ifdef __OPENNN_DEBUG__

    check();

#endif

    const Tensor<Index, 1> target_variables_indices = data_set_pointer->get_target_variables_indices();

    const Index negatives = data_set_pointer->calculate_selection_negatives(target_variables_indices[0]);

    selection_normalization_coefficient = negatives*negatives_weight*static_cast<type>(0.5);
}


/// This method calculates the error term gradient for batch instances.
/// It is used for optimization of parameters during training.
/// Returns the value of the error term gradient.
/// @param batch_indices Indices of the batch instances corresponding to the dataset.

type WeightedSquaredError::calculate_batch_error(const Tensor<Index, 1>& batch_indices) const
{
    #ifdef __OPENNN_DEBUG__

        check();

    #endif

    // Data set

    const Tensor<type, 2> inputs = data_set_pointer->get_input_data(batch_indices);

    const Tensor<type, 2> targets = data_set_pointer->get_target_data(batch_indices);

    const Tensor<type, 2> outputs = neural_network_pointer->calculate_trainable_outputs(inputs);
/*
    const type batch_error = weighted_sum_squared_error(outputs, targets, positives_weight, negatives_weight);

    return batch_error / training_normalization_coefficient;
*/

    return 0.0;
}


type WeightedSquaredError::calculate_batch_error(const Tensor<Index, 1>& batch_indices,
                                                   const Tensor<type, 1>& parameters) const
{
    #ifdef __OPENNN_DEBUG__

        check();

    #endif

    // Data set

    const Tensor<type, 2> inputs = data_set_pointer->get_input_data(batch_indices);

    const Tensor<type, 2> targets = data_set_pointer->get_target_data(batch_indices);

    const Tensor<type, 2> outputs = neural_network_pointer->calculate_trainable_outputs(inputs, parameters);
/*
    const type batch_error = weighted_sum_squared_error(outputs, targets, positives_weight, negatives_weight);

    return batch_error / training_normalization_coefficient;
*/
    return 0.0;

}


/// This method calculates the error term gradient for training instances.
/// It is used for optimization of parameters during training.
/// Returns the value of the error term gradient.

Tensor<type, 1> WeightedSquaredError::calculate_training_error_gradient() const
{

#ifdef __OPENNN_DEBUG__

check();

#endif

    // Neural network

    const Index layers_number = neural_network_pointer->get_trainable_layers_number();

    const Index parameters_number = neural_network_pointer->get_parameters_number();

     bool is_forecasting = false;

    if(neural_network_pointer->has_long_short_term_memory_layer() || neural_network_pointer->has_recurrent_layer()) is_forecasting = true;

    // Data set

    const Tensor<Index, 2> training_batches = data_set_pointer->get_training_batches(!is_forecasting);

    const Index batches_number = training_batches.size();

    // Loss index

    Tensor<type, 1> training_error_gradient(parameters_number);
    training_error_gradient.setZero();

     #pragma omp parallel for

    for(Index i = 0; i < batches_number; i++)
    {
        const Tensor<type, 2> inputs = data_set_pointer->get_input_data(training_batches.chip(i,0));
        const Tensor<type, 2> targets = data_set_pointer->get_target_data(training_batches.chip(i,0));
/*
        const Tensor<Layer::ForwardPropagation, 1> forward_propagation = neural_network_pointer->calculate_forward_propagation(inputs);

        const Tensor<type, 2> output_gradient
                = calculate_output_gradient(forward_propagation[layers_number-1].activations, targets);

        const Tensor<Tensor<type, 2>, 1> layers_delta = calculate_layers_delta(forward_propagation, output_gradient);

        const Tensor<type, 1> batch_gradient = calculate_error_gradient(inputs, forward_propagation, layers_delta);

        #pragma omp critical

        training_error_gradient += batch_gradient;
*/
    }

    return training_error_gradient * static_cast<type>(2.0) / training_normalization_coefficient;
}


/// This method calculates the first order loss.
/// It is used for optimization of parameters during training.
/// Returns a first order terms loss structure, which contains the values and the Jacobian of the error terms function.

LossIndex::BackPropagation WeightedSquaredError::calculate_first_order_loss() const
{
#ifdef __OPENNN_DEBUG__

check();

#endif

    // Neural network

    const Index layers_number = neural_network_pointer->get_trainable_layers_number();

    const Index parameters_number = neural_network_pointer->get_parameters_number();

     bool is_forecasting = false;

    if(neural_network_pointer->has_long_short_term_memory_layer() || neural_network_pointer->has_recurrent_layer()) is_forecasting = true;

    // Data set

    const Tensor<Index, 2> training_batches = data_set_pointer->get_training_batches(!is_forecasting);

    const Index batches_number = training_batches.size();

    BackPropagation first_order_loss(this);

    // Eigen stuff

     #pragma omp parallel for

    for(Index i = 0; i < batches_number; i++)
    {
        const Tensor<type, 2> inputs = data_set_pointer->get_input_data(training_batches.chip(i,0));
        const Tensor<type, 2> targets = data_set_pointer->get_target_data(training_batches.chip(i,0));

        const Tensor<Layer::ForwardPropagation, 1> forward_propagation = neural_network_pointer->calculate_forward_propagation(inputs);
/*
        const Tensor<type, 1> error_terms = calculate_training_error_terms(forward_propagation[layers_number-1].activations, targets);

        const Tensor<type, 2> output_gradient = (forward_propagation[layers_number-1].activations - targets).divide(error_terms, 0);

        const Tensor<Tensor<type, 2>, 1> layers_delta = calculate_layers_delta(forward_propagation, output_gradient);

        const Tensor<type, 2> error_terms_Jacobian
                = calculate_error_terms_Jacobian(inputs, forward_propagation, layers_delta);

//        const Tensor<type, 2> error_terms_Jacobian_transpose = error_terms_Jacobian.calculate_transpose();

        const Tensor<type, 0> loss = error_terms.contract(error_terms,product_vector_vector);//dot(error_terms, error_terms);

        const Tensor<type, 1> gradient = error_terms_Jacobian.contract(error_terms,product_matrix_transpose_vector);//dot(error_terms_Jacobian_transpose, error_terms);

          #pragma omp critical
        {
            first_order_loss.loss += loss(0);
            first_order_loss.gradient += gradient;
         }
*/
    }

    first_order_loss.loss /= training_normalization_coefficient;
    first_order_loss.gradient = (2.0/training_normalization_coefficient)*first_order_loss.gradient;

    if(regularization_method != RegularizationMethod::NoRegularization)
    {
        first_order_loss.loss += regularization_weight*calculate_regularization();
        first_order_loss.gradient += calculate_regularization_gradient()*regularization_weight;
    }

    return first_order_loss;
}


/// This method calculates the first order loss for the selected batch.
/// Returns a first order terms loss structure, which contains the values and the Jacobian of the error terms function.
/// @param batch_indices Indices of the batch instances corresponding to the dataset.

LossIndex::BackPropagation WeightedSquaredError::calculate_first_order_loss(const DataSet::Batch& batch) const
{
#ifdef __OPENNN_DEBUG__

check();

#endif

    // Neural network

    const Index layers_number = neural_network_pointer->get_trainable_layers_number();

    BackPropagation first_order_loss(this);
/*
    const Tensor<Layer::ForwardPropagation, 1> forward_propagation = neural_network_pointer->calculate_forward_propagation(batch.inputs_2d);

    const Tensor<type, 2> output_gradient
            = calculate_output_gradient(forward_propagation[layers_number-1].activations, batch.targets_2d)/training_normalization_coefficient;

    const Tensor<Tensor<type, 2>, 1> layers_delta = calculate_layers_delta(forward_propagation, output_gradient);

    const Tensor<type, 1> batch_gradient
            = calculate_error_gradient(batch.inputs_2d, forward_propagation, layers_delta);

    const type batch_error = sum_squared_error(forward_propagation[layers_number-1].activations, batch.targets_2d);

    first_order_loss.loss = batch_error / training_normalization_coefficient;
    first_order_loss.gradient += batch_gradient;

    // Regularization

    if(regularization_method != RegularizationMethod::NoRegularization)
    {
        first_order_loss.loss += regularization_weight*calculate_regularization();
        first_order_loss.gradient += calculate_regularization_gradient()*regularization_weight;
    }
*/
    return first_order_loss;
}


/// Calculates the loss output gradient by means of the back-propagation algorithm,
/// and returns it in a single vector of size the number of neural network parameters.
/// @param outputs Tensor    of the outputs of the model.
/// @param targets Tensor of targets of the data set.

Tensor<type, 2> WeightedSquaredError::calculate_output_gradient(const Tensor<type, 2>& outputs, const Tensor<type, 2>& targets) const
{
#ifdef __OPENNN_DEBUG__

check();

#endif

    return (outputs-targets)*((targets-static_cast<type>(1.0))*static_cast<type>(-1.0)*negatives_weight + targets*positives_weight);
}


/// Returns loss vector of the error terms function for the weighted squared error.
/// It uses the error back-propagation method.
/// @param outputs Output data.
/// @param targets Target data.

Tensor<type, 1> WeightedSquaredError::calculate_training_error_terms(const Tensor<type, 2>& outputs, const Tensor<type, 2>& targets) const
{
    // Control sentence

    #ifdef __OPENNN_DEBUG__

    check();

    #endif
/*
    return weighted_error_rows(outputs, targets, positives_weight, negatives_weight);
*/
    return Tensor<type, 1>();

}


/// Returns loss vector of the error terms function for the weighted squared error for a given set of parameters.
/// It uses the error back-propagation method.
/// @param parameters Parameters of the neural network

Tensor<type, 1> WeightedSquaredError::calculate_training_error_terms(const Tensor<type, 1>& parameters) const
{
    // Control sentence

    #ifdef __OPENNN_DEBUG__

    check();

    #endif

    const Tensor<type, 2> inputs = data_set_pointer->get_training_input_data();

    const Tensor<type, 2> targets = data_set_pointer->get_training_target_data();

    const Tensor<type, 2> outputs = neural_network_pointer->calculate_trainable_outputs(inputs, parameters);
/*
    return weighted_error_rows(outputs, targets, positives_weight, negatives_weight);
*/
    return Tensor<type, 1>();

}


/// This method calculates the second order loss.
/// It is used for optimization of parameters during training.
/// Returns a second order terms loss structure, which contains the values and the Hessian of the error terms function.

LossIndex::SecondOrderLoss WeightedSquaredError::calculate_terms_second_order_loss() const
{
#ifdef __OPENNN_DEBUG__

check();

#endif

    // Neural network

    const Index layers_number = neural_network_pointer->get_trainable_layers_number();

    const Index parameters_number = neural_network_pointer->get_parameters_number();

     bool is_forecasting = false;

    if(neural_network_pointer->has_long_short_term_memory_layer() || neural_network_pointer->has_recurrent_layer()) is_forecasting = true;

    // Data set

    const Tensor<Index, 2> training_batches = data_set_pointer->get_training_batches(!is_forecasting);

    const Index batches_number = training_batches.size();

    SecondOrderLoss terms_second_order_loss(parameters_number);

    // Eigen stuff

     #pragma omp parallel for

    for(Index i = 0; i < batches_number; i++)
    {
        const Tensor<type, 2> inputs = data_set_pointer->get_input_data(training_batches.chip(i,0));
        const Tensor<type, 2> targets = data_set_pointer->get_target_data(training_batches.chip(i,0));

        const Tensor<Layer::ForwardPropagation, 1> forward_propagation = neural_network_pointer->calculate_forward_propagation(inputs);
/*
        const Tensor<type, 1> error_terms
                = calculate_training_error_terms(forward_propagation[layers_number-1].activations, targets);

        const Tensor<type, 2> output_gradient = (forward_propagation[layers_number-1].activations - targets).divide(error_terms, 0);

        const Tensor<Tensor<type, 2>, 1> layers_delta = calculate_layers_delta(forward_propagation, output_gradient);

        const Tensor<type, 2> error_terms_Jacobian = calculate_error_terms_Jacobian(inputs, forward_propagation, layers_delta);

//        const Tensor<type, 2> error_terms_Jacobian_transpose = error_terms_Jacobian.calculate_transpose();

        const Tensor<type, 0> loss = error_terms.contract(error_terms, product_vector_vector);//dot(error_terms, error_terms);

        const Tensor<type, 1> gradient = error_terms_Jacobian.contract(error_terms, product_matrix_transpose_vector);//dot(error_terms_Jacobian_transpose, error_terms);

        Tensor<type, 2> hessian_approximation = error_terms_Jacobian.contract(error_terms_Jacobian, product_matrix_transpose_matrix);
        //hessian_approximation.dot(error_terms_Jacobian_transpose, error_terms_Jacobian);

          #pragma omp critical
        {
            terms_second_order_loss.loss += loss(0);
            terms_second_order_loss.gradient += gradient;
            terms_second_order_loss.hessian += hessian_approximation;
        }
*/
    }

    terms_second_order_loss.loss /= training_normalization_coefficient;
    terms_second_order_loss.gradient = (static_cast<type>(2.0)/training_normalization_coefficient)*terms_second_order_loss.gradient;
    terms_second_order_loss.hessian = (static_cast<type>(2.0)/training_normalization_coefficient)*terms_second_order_loss.hessian;

    if(regularization_method != RegularizationMethod::NoRegularization)
    {
        terms_second_order_loss.loss += calculate_regularization();
        terms_second_order_loss.gradient += calculate_regularization_gradient();
        terms_second_order_loss.hessian += calculate_regularization_hessian();
    }

    return terms_second_order_loss;
}


/// Returns a string with the name of the weighted squared error loss type, "WEIGHTED_SQUARED_ERROR".

string WeightedSquaredError::get_error_type() const
{
    return "WEIGHTED_SQUARED_ERROR";
}


/// Returns a string with the name of the weighted squared error loss type in text format.

string WeightedSquaredError::get_error_type_text() const
{
    return "Weighted squared error";
}


/// Serializes the weighted squared error object into a XML document of the TinyXML library.
/// See the OpenNN manual for more information about the format of this document-> 

tinyxml2::XMLDocument* WeightedSquaredError::to_XML() const
{
    ostringstream buffer;

    tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument;

    // Weighted squared error

    tinyxml2::XMLElement* weighted_squared_error_element = document->NewElement("WeightedSquaredError");

    document->InsertFirstChild(weighted_squared_error_element);

    // Positives weight
    {
    tinyxml2::XMLElement* element = document->NewElement("PositivesWeight");
    weighted_squared_error_element->LinkEndChild(element);

    buffer.str("");
    buffer << positives_weight;

    tinyxml2::XMLText* text = document->NewText(buffer.str().c_str());
    element->LinkEndChild(text);
    }

    // Negatives weight
    {
    tinyxml2::XMLElement* element = document->NewElement("NegativesWeight");
    weighted_squared_error_element->LinkEndChild(element);

    buffer.str("");
    buffer << negatives_weight;

    tinyxml2::XMLText* text = document->NewText(buffer.str().c_str());
    element->LinkEndChild(text);
    }

    // Display
    //   {
    //      tinyxml2::XMLElement* element = document->NewElement("Display");
    //      weighted_squared_error_element->LinkEndChild(element);

    //      buffer.str("");
    //      buffer << display;

    //      tinyxml2::XMLText* text = document->NewText(buffer.str().c_str());
    //      element->LinkEndChild(text);
    //   }

    return document;
}


/// Serializes the cross entropy error object into a XML document of the TinyXML library without keep the DOM tree in memory.
/// See the OpenNN manual for more information about the format of this document.
/// @param file_stream

void WeightedSquaredError::write_XML(tinyxml2::XMLPrinter& file_stream) const
{
    ostringstream buffer;

    // Error type

    file_stream.OpenElement("Error");

    file_stream.PushAttribute("Type", "WEIGHTED_SQUARED_ERROR");

    // Positives weight

    file_stream.OpenElement("PositivesWeight");

    buffer.str("");
    buffer << positives_weight;

    file_stream.PushText(buffer.str().c_str());

    file_stream.CloseElement();

    // Negatives weight

    file_stream.OpenElement("NegativesWeight");

    buffer.str("");
    buffer << negatives_weight;

    file_stream.PushText(buffer.str().c_str());

    file_stream.CloseElement();

    // Close error

    file_stream.CloseElement();

    // Regularization

    write_regularization_XML(file_stream);
}


/// Loads a weighted squared error object from a XML document.
/// @param document Pointer to a TinyXML document with the object data.

void WeightedSquaredError::from_XML(const tinyxml2::XMLDocument& document)
{
   const tinyxml2::XMLElement* root_element = document.FirstChildElement("WeightedSquaredError");

   if(!root_element)
   {
       ostringstream buffer;

       buffer << "OpenNN Exception: WeightedSquaredError class.\n"
              << "void from_XML(const tinyxml2::XMLDocument&) method.\n"
              << "Weighted squared element is nullptr.\n";

       throw logic_error(buffer.str());
   }

   // Positives weight

   const tinyxml2::XMLElement* error_element = root_element->FirstChildElement("Error");

   const tinyxml2::XMLElement* positives_weight_element = error_element->FirstChildElement("PositivesWeight");

   if(positives_weight_element)
   {
      const string string = positives_weight_element->GetText();

      try
      {
         set_positives_weight(atof(string.c_str()));
      }
      catch(const logic_error& e)
      {
         cerr << e.what() << endl;
      }
   }

   // Negatives weight

   const tinyxml2::XMLElement* negatives_weight_element = error_element->FirstChildElement("NegativesWeight");

   if(negatives_weight_element)
   {
      const string string = negatives_weight_element->GetText();

      try
      {
         set_negatives_weight(atof(string.c_str()));
      }
      catch(const logic_error& e)
      {
         cerr << e.what() << endl;
      }
   }

   // Regularization

   tinyxml2::XMLDocument regularization_document;
   tinyxml2::XMLNode* element_clone;

   const tinyxml2::XMLElement* regularization_element = root_element->FirstChildElement("Regularization");

   element_clone = regularization_element->DeepClone(&regularization_document);

   regularization_document.InsertFirstChild(element_clone);

   regularization_from_XML(regularization_document);
}


/// Returns the model in string format.

string WeightedSquaredError::object_to_string() const
{
    ostringstream buffer;

    buffer << "Weighted squared error.\n"
           << "Positives weight: " << positives_weight << "\n"
           << "Negatives weight: " << negatives_weight << endl;

    return buffer.str();
}

}

// OpenNN: Open Neural Networks Library.
// Copyright(C) 2005-2020 Artificial Intelligence Techniques, SL.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
