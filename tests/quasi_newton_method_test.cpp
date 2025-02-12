//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   Q U A S I - N E W T O N   M E T H O D   T E S T   C L A S S           
//
//   Artificial Intelligence Techniques SL
//   artelnics@artelnics.com

#include "quasi_newton_method_test.h"


QuasiNewtonMethodTest::QuasiNewtonMethodTest() : UnitTesting() 
{
    sum_squared_error.set(&neural_network, &data_set);

    quasi_newton_method.set_loss_index_pointer(&sum_squared_error);
}


QuasiNewtonMethodTest::~QuasiNewtonMethodTest()
{

}


void QuasiNewtonMethodTest::test_constructor()
{
    cout << "test_constructor\n";

    // Default constructor

    QuasiNewtonMethod quasi_newton_method_1;

    assert_true(!quasi_newton_method_1.has_loss_index(), LOG);

    // Loss index constructor

    QuasiNewtonMethod quasi_newton_method_2(&sum_squared_error);
    assert_true(quasi_newton_method_2.has_loss_index(), LOG);
}


void QuasiNewtonMethodTest::test_destructor()
{
    cout << "test_destructor\n";

    QuasiNewtonMethod* quasi_newton_method = new QuasiNewtonMethod;
    delete quasi_newton_method;
}


void QuasiNewtonMethodTest::test_set_inverse_hessian_approximation_method()
{
    cout << "test_set_training_direction_method\n";

    quasi_newton_method.set_inverse_hessian_approximation_method(
                QuasiNewtonMethod::InverseHessianApproximationMethod::BFGS);

    assert_true(
                quasi_newton_method.get_inverse_hessian_approximation_method()
                == QuasiNewtonMethod::InverseHessianApproximationMethod::BFGS, LOG);
}


void QuasiNewtonMethodTest::test_calculate_DFP_inverse_hessian_approximation()
{
    cout << "test_calculate_DFP_inverse_hessian_approximation\n";

    samples_number = 1;
    inputs_number = 1;
    targets_number = 1;
    neurons_number = 1;

    // Test

    data_set.set(samples_number, inputs_number, targets_number);
    data_set.set_data_random();

    neural_network.set(NeuralNetwork::ProjectType::Approximation, {inputs_number, targets_number});

    // Test

    neural_network.set_parameters_constant(type(1));

    quasi_newton_method_data.set(&quasi_newton_method);

    quasi_newton_method.calculate_DFP_inverse_hessian(quasi_newton_method_data);

//    assert_true(are_equal(quasi_newton_method_data.inverse_hessian, inverse_hessian, type(1e-4)), LOG);
}


// @todo
void QuasiNewtonMethodTest::test_calculate_BFGS_inverse_hessian_approximation()
{
    cout << "test_calculate_BFGS_inverse_hessian_approximation\n";

    samples_number = 1;
    inputs_number = 1;
    targets_number = 1;
    neurons_number = 1;

    neural_network.set(NeuralNetwork::ProjectType::Approximation, {inputs_number, targets_number});
    neural_network.set_parameters_constant(type(1));

    sum_squared_error.set_regularization_method(LossIndex::RegularizationMethod::L2);

    quasi_newton_method.calculate_BFGS_inverse_hessian(quasi_newton_method_data);

//    assert_true(are_equal(BFGS_inverse_hessian ,inverse_hessian, type(1e-4)), LOG);
}


// @todo
void QuasiNewtonMethodTest::test_calculate_inverse_hessian_approximation()
{
    cout << "test_calculate_inverse_hessian_approximation\n";

    Tensor<type, 1> old_parameters;
    Tensor<type, 1> old_gradient;
    Tensor<type, 2> old_inverse_hessian;

    Tensor<type, 1> parameters;
    Tensor<type, 1> gradient;
    Tensor<type, 2> inverse_hessian;

    Tensor<type, 2> inverse_hessian_approximation;

    // Test

    samples_number = 1;
    inputs_number = 1;
    targets_number = 1;

    data_set.set(samples_number, inputs_number, targets_number);
    data_set.set_data_random();

    neural_network.set(NeuralNetwork::ProjectType::Approximation, {inputs_number, targets_number});

    quasi_newton_method.set_inverse_hessian_approximation_method(QuasiNewtonMethod::InverseHessianApproximationMethod::DFP);

    neural_network.set_parameters_constant(type(1));

    quasi_newton_method.calculate_inverse_hessian_approximation(quasi_newton_method_data);

//    assert_true(inverse_hessian_approximation == inverse_hessian, LOG);

    quasi_newton_method.set_inverse_hessian_approximation_method(QuasiNewtonMethod::InverseHessianApproximationMethod::DFP);

    neural_network.set_parameters_constant(type(1));

    neural_network.set_parameters_constant(type(-0.5));

    quasi_newton_method.calculate_inverse_hessian_approximation(quasi_newton_method_data);

//    assert_true(inverse_hessian_approximation == inverse_hessian, LOG);

    // Test

    quasi_newton_method.calculate_inverse_hessian_approximation(quasi_newton_method_data);
}


void QuasiNewtonMethodTest::test_perform_training()
{
    cout << "test_perform_training\n";

    // Test

    samples_number = 1;
    inputs_number = 1;
    targets_number = 1;
    neurons_number = 1;

    data_set.set(samples_number, inputs_number, targets_number);
    data_set.set_data_random();
    neural_network.set(NeuralNetwork::ProjectType::Approximation, {inputs_number, neurons_number, targets_number});
    quasi_newton_method.set_inverse_hessian_approximation_method(QuasiNewtonMethod::InverseHessianApproximationMethod::DFP);

    // Test

    neural_network.set_parameters_constant(type(3.1415927));

//    type old_loss = sum_squared_error.calculate_training_loss();

    quasi_newton_method.set_maximum_epochs_number(2);
    quasi_newton_method.set_display(false);

    quasi_newton_method.perform_training();

//    type loss = sum_squared_error.calculate_training_loss();

//    assert_true(loss < old_loss, LOG);

    // Minimum parameters increment norm

    neural_network.set_parameters_constant(type(3.1415927));

    type minimum_parameters_increment_norm = type(0.1);

    quasi_newton_method.set_loss_goal(type(0));
    quasi_newton_method.set_minimum_loss_decrease(0.0);
    quasi_newton_method.set_maximum_epochs_number(10);
    quasi_newton_method.set_maximum_time(1000.0);

    quasi_newton_method.perform_training();

    // Loss goal

    neural_network.set_parameters_constant(3.1415927);

    type training_loss_goal = 100.0;

    quasi_newton_method.set_loss_goal(training_loss_goal);
    quasi_newton_method.set_minimum_loss_decrease(0.0);
    quasi_newton_method.set_maximum_epochs_number(10);
    quasi_newton_method.set_maximum_time(1000.0);

    quasi_newton_method.perform_training();

//    loss = sum_squared_error.calculate_training_loss();

//    assert_true(loss < training_loss_goal, LOG);

    // Minimum loss decrease

//    neural_network.set_parameters_constant(3.1415927);

    type minimum_loss_decrease = 100.0;

    quasi_newton_method.set_loss_goal(type(0));
    quasi_newton_method.set_minimum_loss_decrease(minimum_loss_decrease);
    quasi_newton_method.set_maximum_epochs_number(10);
    quasi_newton_method.set_maximum_time(1000.0);

    quasi_newton_method.perform_training();

    // Gradient norm goal

    neural_network.set_parameters_constant(type(3.1415927));

    quasi_newton_method.set_loss_goal(type(0));
    quasi_newton_method.set_minimum_loss_decrease(0.0);
    quasi_newton_method.set_maximum_epochs_number(10);
    quasi_newton_method.set_maximum_time(1000.0);

    quasi_newton_method.perform_training();

//    type gradient_norm = sum_squared_error.calculate_training_loss_gradient().calculate_norm();
//    assert_true(gradient_norm < gradient_norm_goal, LOG);
}


void QuasiNewtonMethodTest::run_test_case()
{
    cout << "Running quasi-Newton method test case...\n";

    // Constructor and destructor methods

    test_constructor();
    test_destructor();

    // Set methods

    test_set_inverse_hessian_approximation_method();

    // Training methods

    test_calculate_DFP_inverse_hessian_approximation();

    test_calculate_BFGS_inverse_hessian_approximation();

    test_calculate_inverse_hessian_approximation();

    test_perform_training();

    cout << "End of quasi-Newton method test case.\n\n";
}


// OpenNN: Open Neural Networks Library.
// Copyright (C) 2005-2021 Artificial Intelligence Techniques, SL.
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
