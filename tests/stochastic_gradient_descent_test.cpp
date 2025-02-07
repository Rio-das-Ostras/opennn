//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   S T O C H A S T I C   G R A D I E N T   D E S C E N T   T E S T   C L A S S    
//
//   Artificial Intelligence Techniques SL
//   artelnics@artelnics.com

#include "stochastic_gradient_descent_test.h"

StochasticGradientDescentTest::StochasticGradientDescentTest() : UnitTesting()
{
    sum_squared_error.set(&neural_network, &data_set);

    stochastic_gradient_descent.set_loss_index_pointer(&sum_squared_error);

    stochastic_gradient_descent.set_display(false);
}


StochasticGradientDescentTest::~StochasticGradientDescentTest()
{
}


void StochasticGradientDescentTest::test_constructor()
{
    cout << "test_constructor\n";

    // Default constructor

    StochasticGradientDescent stochastic_gradient_descent_1;
    assert_true(!stochastic_gradient_descent_1.has_loss_index(), LOG);

    // Loss index constructor

    StochasticGradientDescent stochastic_gradient_descent_2(&sum_squared_error);
    assert_true(stochastic_gradient_descent_2.has_loss_index(), LOG);
}


void StochasticGradientDescentTest::test_destructor()
{
    cout << "test_destructor\n";

    StochasticGradientDescent* stochastic_gradient_descent = new StochasticGradientDescent;
    delete stochastic_gradient_descent;
}


void StochasticGradientDescentTest::test_perform_training()
{
    cout << "test_perform_training\n";

    Index samples_number;
    Index inputs_number;
    Index targets_number;

    TrainingResults training_results;

    // Test

    samples_number = 1;
    inputs_number = 1;
    targets_number = 1;

    data_set.set(samples_number, inputs_number, targets_number);
    data_set.set_data_random();

    neural_network.set(NeuralNetwork::ProjectType::Approximation, {inputs_number, targets_number});
    neural_network.set_parameters_random();

    stochastic_gradient_descent.set_maximum_epochs_number(1);

    training_results = stochastic_gradient_descent.perform_training();

    // Minimum parameters increment norm

    neural_network.set_parameters_constant(type(-1.0));

    stochastic_gradient_descent.set_loss_goal(type(0.0));
    stochastic_gradient_descent.set_maximum_epochs_number(1000);
    stochastic_gradient_descent.set_maximum_time(type(1000.0));

    training_results = stochastic_gradient_descent.perform_training();

    // Loss goal

    neural_network.set_parameters_constant(type(-1.0));

    type training_loss_goal = type(0.1);

    stochastic_gradient_descent.set_loss_goal(training_loss_goal);
    stochastic_gradient_descent.set_maximum_epochs_number(1000);
    stochastic_gradient_descent.set_maximum_time(type(1000.0));

    training_results = stochastic_gradient_descent.perform_training();

    // Minimum loss increase

    neural_network.set_parameters_constant(type(-1));

    stochastic_gradient_descent.set_loss_goal(type(0));
    stochastic_gradient_descent.set_maximum_epochs_number(1000);
    stochastic_gradient_descent.set_maximum_time(type(1000.0));

    training_results = stochastic_gradient_descent.perform_training();

    // Gradient norm goal

    neural_network.set_parameters_constant(type(-1));

    stochastic_gradient_descent.set_loss_goal(type(0));
    stochastic_gradient_descent.set_maximum_epochs_number(1000);
    stochastic_gradient_descent.set_maximum_time(type(1000.0));

    training_results = stochastic_gradient_descent.perform_training();
}


void StochasticGradientDescentTest::test_to_XML()
{
    cout << "test_to_XML\n";

    tinyxml2::XMLPrinter file_stream;

    stochastic_gradient_descent.write_XML(file_stream);
}


void StochasticGradientDescentTest::run_test_case()
{
    cout << "Running stochastic gradient descent test case...\n";

    // Constructor and destructor methods

    test_constructor();

    test_destructor();

    // Training methods

    test_perform_training();

    // Serialization methods

    test_to_XML();

    cout << "End of stochastic gradient descent test case.\n\n";
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
