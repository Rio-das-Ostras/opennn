//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   Q U A S I - N E W T O N   M E T H O D    C L A S S   H E A D E R      
//
//   Artificial Intelligence Techniques SL
//   artelnics@artelnics.com

#ifndef QUASINEWTONMETHOD_H
#define QUASINEWTONMETHOD_H

// System includes

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <limits>
#include <cmath>
#include <ctime>

// OpenNN includes

#include "config.h"

#include "loss_index.h"

#include "optimization_algorithm.h"
#include "learning_rate_algorithm.h"

// Eigen Includes

#include "../eigen/unsupported/Eigen/KroneckerProduct"

using Eigen::MatrixXd;

namespace OpenNN
{

struct QuasiNewtonMehtodData;

/// Class of optimization algorithm based on Newton's method.
/// An approximate Hessian matrix is computed at each iteration of the algorithm based on the gradients.

/// This concrete class represents a quasi-Newton training algorithm[1], used to minimize loss function.
///
/// \cite 1  Neural Designer "5 Algorithms to Train a Neural Network."
/// \ref https://www.neuraldesigner.com/blog/5_algorithms_to_train_a_neural_network

class QuasiNewtonMethod : public OptimizationAlgorithm
{

public:

   // Enumerations

   /// Enumeration of the available training operators for obtaining the approximation to the inverse hessian.

   enum class InverseHessianApproximationMethod{DFP, BFGS};

   // Constructors

   explicit QuasiNewtonMethod();

   explicit QuasiNewtonMethod(LossIndex*);

   // Destructor

   virtual ~QuasiNewtonMethod();

   // Get methods

   const LearningRateAlgorithm& get_learning_rate_algorithm() const;
   LearningRateAlgorithm* get_learning_rate_algorithm_pointer();

   const InverseHessianApproximationMethod& get_inverse_hessian_approximation_method() const;
   string write_inverse_hessian_approximation_method() const;

   const Index& get_epochs_number() const;

   // Stopping criteria

   const type& get_minimum_loss_decrease() const;
   const type& get_loss_goal() const;

   const Index& get_maximum_selection_failures() const;

   const Index& get_maximum_epochs_number() const;
   const type& get_maximum_time() const;

   // Set methods

   void set_loss_index_pointer(LossIndex*);

   void set_inverse_hessian_approximation_method(const InverseHessianApproximationMethod&);
   void set_inverse_hessian_approximation_method(const string&);

   void set_display(const bool&);

   void set_default();

   // Stopping criteria



   void set_minimum_loss_decrease(const type&);
   void set_loss_goal(const type&);

   void set_maximum_selection_failures(const Index&);

   void set_maximum_epochs_number(const Index&);
   void set_maximum_time(const type&);

   // Training methods

   void calculate_DFP_inverse_hessian(QuasiNewtonMehtodData&) const;

   void calculate_BFGS_inverse_hessian(QuasiNewtonMehtodData&) const;

   void initialize_inverse_hessian_approximation(QuasiNewtonMehtodData&) const;
   void calculate_inverse_hessian_approximation(QuasiNewtonMehtodData&) const;

   const Tensor<type, 2> kronecker_product(Tensor<type, 2>&, Tensor<type, 2>&) const;
   const Tensor<type, 2> kronecker_product(Tensor<type, 1>&, Tensor<type, 1>&) const;

   void update_parameters(
           const DataSetBatch& batch,
           NeuralNetworkForwardPropagation& forward_propagation,
           LossIndexBackPropagation& back_propagation,
           QuasiNewtonMehtodData& optimization_data);

   TrainingResults perform_training();

   string write_optimization_algorithm_type() const;

   // Serialization methods
   
   void from_XML(const tinyxml2::XMLDocument&);

   void write_XML(tinyxml2::XMLPrinter&) const;
   
   Tensor<string, 2> to_string_matrix() const;

private: 

   /// Learning rate algorithm object.
   /// It calculates the step for a given training direction.

   LearningRateAlgorithm learning_rate_algorithm;

   /// Variable containing the actual method used to obtain a suitable learning rate.

   InverseHessianApproximationMethod inverse_hessian_approximation_method;

   type first_learning_rate = static_cast<type>(0.01);

   // Stopping criteria

   /// Minimum loss improvement between two successive epochs. It is used as a stopping criterion.

   type minimum_loss_decrease;

   /// Goal value for the loss. It is used as a stopping criterion.

   type training_loss_goal;

   /// Maximum number of epochs at which the selection error increases.
   /// This is an early stopping method for improving selection.

   Index maximum_selection_failures;

   /// Maximum number of epochs to perform_training. It is used as a stopping criterion.

   Index maximum_epochs_number;

   /// Maximum training time. It is used as a stopping criterion.

   type maximum_time;
};


struct QuasiNewtonMehtodData : public OptimizationAlgorithmData
{
    /// Default constructor.

    explicit QuasiNewtonMehtodData()
    {
    }

    explicit QuasiNewtonMehtodData(QuasiNewtonMethod* new_quasi_newton_method_pointer)
    {
        set(new_quasi_newton_method_pointer);
    }

    virtual ~QuasiNewtonMehtodData() {}

    void set(QuasiNewtonMethod* new_quasi_newton_method_pointer)
    {
        quasi_newton_method_pointer = new_quasi_newton_method_pointer;

        LossIndex* loss_index_pointer = quasi_newton_method_pointer->get_loss_index_pointer();

        NeuralNetwork* neural_network_pointer = loss_index_pointer->get_neural_network_pointer();

        const Index parameters_number = neural_network_pointer->get_parameters_number();

        // Neural network data

        old_parameters.resize(parameters_number);

        parameters_difference.resize(parameters_number);

        potential_parameters.resize(parameters_number);
        parameters_increment.resize(parameters_number);

        // Loss index data

        old_gradient.resize(parameters_number);
        old_gradient.setZero();

        gradient_difference.resize(parameters_number);

        inverse_hessian.resize(parameters_number, parameters_number);
        inverse_hessian.setZero();

        old_inverse_hessian.resize(parameters_number, parameters_number);
        old_inverse_hessian.setZero();

        // Optimization algorithm data

        training_direction.resize(parameters_number);

        old_inverse_hessian_dot_gradient_difference.resize(parameters_number);
    }

    void print() const
    {
        cout << "Training Direction:" << endl;
        cout << training_direction << endl;

        cout << "Learning rate:" << endl;
        cout << learning_rate << endl;
    }

    QuasiNewtonMethod* quasi_newton_method_pointer = nullptr;

    // Neural network data

    Tensor<type, 1> old_parameters;
    Tensor<type, 1> parameters_difference;

    Tensor<type, 1> parameters_increment;

    // Loss index data

//    type old_loss = 0;

    Tensor<type, 1> old_gradient;
    Tensor<type, 1> gradient_difference;

    Tensor<type, 2> inverse_hessian;
    Tensor<type, 2> old_inverse_hessian;

    Tensor<type, 1> old_inverse_hessian_dot_gradient_difference;

    // Optimization algorithm data

    Index epoch = 0;

    Tensor<type, 0> training_slope;

    type learning_rate = type(0);
    type old_learning_rate = type(0);
};

}

#endif


// OpenNN: Open Neural Networks Library.
// Copyright(C) 2005-2021 Artificial Intelligence Techniques, SL.
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
