//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   P E R C E P T R O N   L A Y E R   C L A S S
//
//   Artificial Intelligence Techniques SL
//   artelnics@artelnics.com

#include "perceptron_layer.h"

namespace OpenNN
{

/// Default constructor.
/// It creates a empty layer object, with no perceptrons.
/// This constructor also initializes the rest of class members to their default values.

PerceptronLayer::PerceptronLayer() : Layer()
{
    set();

    layer_type = Type::Perceptron;
}


/// Layer architecture constructor.
/// It creates a layer object with given numbers of inputs and perceptrons.
/// The parameters are initialized at random.
/// This constructor also initializes the rest of class members to their default values.
/// @param new_inputs_number Number of inputs in the layer.
/// @param new_neurons_number Number of perceptrons in the layer.

PerceptronLayer::PerceptronLayer(const Index& new_inputs_number, const Index& new_neurons_number,
                                 const PerceptronLayer::ActivationFunction& new_activation_function) : Layer()
{
    set(new_inputs_number, new_neurons_number, new_activation_function);

    layer_type = Type::Perceptron;

    layer_name = "perceptron_layer";
}


/// Destructor.
/// This destructor does not delete any pointer.

PerceptronLayer::~PerceptronLayer()
{
}


/// Returns the number of inputs to the layer.

Index PerceptronLayer::get_inputs_number() const
{
    return synaptic_weights.dimension(0);
}


/// Returns the number of neurons in the layer.

Index PerceptronLayer::get_neurons_number() const
{
    return biases.size();
}


Index PerceptronLayer::get_biases_number() const
{
    return biases.size();
}


/// Returns the number of layer's synaptic weights

Index PerceptronLayer::get_synaptic_weights_number() const
{
    return synaptic_weights.size();
}


/// Returns the number of parameters(biases and synaptic weights) of the layer.

Index PerceptronLayer::get_parameters_number() const
{
    return biases.size() + synaptic_weights.size();
}


/// Returns the biases from all the perceptrons in the layer.
/// The format is a vector of real values.
/// The size of this vector is the number of neurons in the layer.

const Tensor<type, 2>& PerceptronLayer::get_biases() const
{
    return biases;
}


/// Returns the synaptic weights from the perceptrons.
/// The format is a matrix of real values.
/// The number of rows is the number of neurons in the layer.
/// The number of columns is the number of inputs to the layer.

const Tensor<type, 2>& PerceptronLayer::get_synaptic_weights() const
{
    return synaptic_weights;
}


Tensor<type, 2> PerceptronLayer::get_synaptic_weights(const Tensor<type, 1>& parameters) const
{
    const Index inputs_number = get_inputs_number();

    const Index neurons_number = get_neurons_number();

    const Index synaptic_weights_number = get_synaptic_weights_number();

    const Index parameters_size = parameters.size();

    const Index start_synaptic_weights_number = (parameters_size - synaptic_weights_number);

    Tensor<type, 1> new_synaptic_weights = parameters.slice(Eigen::array<Eigen::Index, 1>({start_synaptic_weights_number}), Eigen::array<Eigen::Index, 1>({synaptic_weights_number}));

    Eigen::array<Index, 2> two_dim{{inputs_number, neurons_number}};

    return new_synaptic_weights.reshape(two_dim);
}


Tensor<type, 2> PerceptronLayer::get_biases(const Tensor<type, 1>& parameters) const
{
    const Index biases_number = biases.size();

    Tensor<type, 1> new_biases(biases_number);

    new_biases = parameters.slice(Eigen::array<Eigen::Index, 1>({0}), Eigen::array<Eigen::Index, 1>({biases_number}));

    Eigen::array<Index, 2> two_dim{{1, biases.dimension(1)}};

    return new_biases.reshape(two_dim);

}


/// Returns a single vector with all the layer parameters.
/// The format is a vector of real values.
/// The size is the number of parameters in the layer.

Tensor<type, 1> PerceptronLayer::get_parameters() const
{
    Tensor<type, 1> parameters(synaptic_weights.size() + biases.size());

    for(Index i = 0; i < biases.size(); i++)
    {
        fill_n(parameters.data()+i, 1, biases(i));
    }

    for(Index i = 0; i < synaptic_weights.size(); i++)
    {
        fill_n(parameters.data()+ biases.size() +i, 1, synaptic_weights(i));
    }

    return parameters;
}


/// Returns the activation function of the layer.
/// The activation function of a layer is the activation function of all perceptrons in it.

const PerceptronLayer::ActivationFunction& PerceptronLayer::get_activation_function() const
{
    return activation_function;
}


/// Returns a string with the name of the layer activation function.
/// This can be: Logistic, HyperbolicTangent, Threshold, SymmetricThreshold, Linear, RectifiedLinear, ScaledExponentialLinear.

string PerceptronLayer::write_activation_function() const
{
    switch(activation_function)
    {
    case ActivationFunction::Logistic:
        return "Logistic";

    case ActivationFunction::HyperbolicTangent:
        return "HyperbolicTangent";

    case ActivationFunction::Threshold:
        return "Threshold";

    case ActivationFunction::SymmetricThreshold:
        return "SymmetricThreshold";

    case ActivationFunction::Linear:
        return "Linear";

    case ActivationFunction::RectifiedLinear:
        return "RectifiedLinear";

    case ActivationFunction::ScaledExponentialLinear:
        return "ScaledExponentialLinear";

    case ActivationFunction::SoftPlus:
        return "SoftPlus";

    case ActivationFunction::SoftSign:
        return "SoftSign";

    case ActivationFunction::HardSigmoid:
        return "HardSigmoid";

    case ActivationFunction::ExponentialLinear:
        return "ExponentialLinear";
    }

    return string();
}


/// Returns true if messages from this class are to be displayed on the screen,
/// or false if messages from this class are not to be displayed on the screen.

const bool& PerceptronLayer::get_display() const
{
    return display;
}


/// Sets an empty layer, wihtout any perceptron.
/// It also sets the rest of members to their default values.

void PerceptronLayer::set()
{
    biases.resize(0, 0);

    synaptic_weights.resize(0, 0);

    set_default();
}


/// Sets new numbers of inputs and perceptrons in the layer.
/// It also sets the rest of members to their default values.
/// @param new_inputs_number Number of inputs.
/// @param new_neurons_number Number of perceptron neurons.

void PerceptronLayer::set(const Index& new_inputs_number, const Index& new_neurons_number,
                          const PerceptronLayer::ActivationFunction& new_activation_function)
{
    biases.resize(1, new_neurons_number);

    synaptic_weights.resize(new_inputs_number, new_neurons_number);

    set_parameters_random();

    activation_function = new_activation_function;

    set_default();
}


/// Sets those members not related to the vector of perceptrons to their default value.
/// <ul>
/// <li> Display: True.
/// <li> layer_type: Perceptron_Layer.
/// <li> trainable: True.
/// </ul>

void PerceptronLayer::set_default()
{
    layer_name = "perceptron_layer";

    display = true;

    layer_type = Type::Perceptron;
}


void PerceptronLayer::set_name(const string& new_layer_name)
{
    layer_name = new_layer_name;
}


/// Sets a new number of inputs in the layer.
/// The new synaptic weights are initialized at random.
/// @param new_inputs_number Number of layer inputs.

void PerceptronLayer::set_inputs_number(const Index& new_inputs_number)
{
    const Index neurons_number = get_neurons_number();

    biases.resize(1,neurons_number);

    synaptic_weights.resize(new_inputs_number, neurons_number);
}


/// Sets a new number perceptrons in the layer.
/// All the parameters are also initialized at random.
/// @param new_neurons_number New number of neurons in the layer.

void PerceptronLayer::set_neurons_number(const Index& new_neurons_number)
{
    const Index inputs_number = get_inputs_number();

    biases.resize(1, new_neurons_number);

    synaptic_weights.resize(inputs_number, new_neurons_number);
}


/// Sets the biases of all perceptrons in the layer from a single vector.
/// @param new_biases New set of biases in the layer.

void PerceptronLayer::set_biases(const Tensor<type, 2>& new_biases)
{
    biases = new_biases;
}


/// Sets the synaptic weights of this perceptron layer from a single matrix.
/// The format is a matrix of real numbers.
/// The number of rows is the number of neurons in the corresponding layer.
/// The number of columns is the number of inputs to the corresponding layer.
/// @param new_synaptic_weights New set of synaptic weights in that layer.

void PerceptronLayer::set_synaptic_weights(const Tensor<type, 2>& new_synaptic_weights)
{
    synaptic_weights = new_synaptic_weights;
}


/// Sets the parameters of this layer.

void PerceptronLayer::set_parameters(const Tensor<type, 1>& new_parameters, const Index& index)
{   
    const Index biases_number = get_biases_number();
    const Index synaptic_weights_number = get_synaptic_weights_number();

    memcpy(biases.data(),
           new_parameters.data() + index,
           static_cast<size_t>(biases_number)*sizeof(type));

    memcpy(synaptic_weights.data(),
           new_parameters.data() + biases_number + index,
           static_cast<size_t>(synaptic_weights_number)*sizeof(type));
}


/// This class sets a new activation(or transfer) function in a single layer.
/// @param new_activation_function Activation function for the layer.

void PerceptronLayer::set_activation_function(const PerceptronLayer::ActivationFunction& new_activation_function)
{
    activation_function = new_activation_function;
}


/// Sets a new activation(or transfer) function in a single layer.
/// The second argument is a string containing the name of the function("Logistic", "HyperbolicTangent", "Threshold", etc).
/// @param new_activation_function Activation function for that layer.

void PerceptronLayer::set_activation_function(const string& new_activation_function_name)
{
    if(new_activation_function_name == "Logistic")
    {
        activation_function = ActivationFunction::Logistic;
    }
    else if(new_activation_function_name == "HyperbolicTangent")
    {
        activation_function = ActivationFunction::HyperbolicTangent;
    }
    else if(new_activation_function_name == "Threshold")
    {
        activation_function = ActivationFunction::Threshold;
    }
    else if(new_activation_function_name == "SymmetricThreshold")
    {
        activation_function = ActivationFunction::SymmetricThreshold;
    }
    else if(new_activation_function_name == "Linear")
    {
        activation_function = ActivationFunction::Linear;
    }
    else if(new_activation_function_name == "RectifiedLinear")
    {
        activation_function = ActivationFunction::RectifiedLinear;
    }
    else if(new_activation_function_name == "ScaledExponentialLinear")
    {
        activation_function = ActivationFunction::ScaledExponentialLinear;
    }
    else if(new_activation_function_name == "SoftPlus")
    {
        activation_function = ActivationFunction::SoftPlus;
    }
    else if(new_activation_function_name == "SoftSign")
    {
        activation_function = ActivationFunction::SoftSign;
    }
    else if(new_activation_function_name == "HardSigmoid")
    {
        activation_function = ActivationFunction::HardSigmoid;
    }
    else if(new_activation_function_name == "ExponentialLinear")
    {
        activation_function = ActivationFunction::ExponentialLinear;
    }
    else
    {
        ostringstream buffer;

        buffer << "OpenNN Exception: PerceptronLayer class.\n"
               << "void set_activation_function(const string&) method.\n"
               << "Unknown activation function: " << new_activation_function_name << ".\n";

        throw logic_error(buffer.str());
    }
}


/// Sets a new display value.
/// If it is set to true messages from this class are to be displayed on the screen;
/// if it is set to false messages from this class are not to be displayed on the screen.
/// @param new_display Display value.

void PerceptronLayer::set_display(const bool& new_display)
{
    display = new_display;
}


/// Initializes the biases of all the perceptrons in the layer of perceptrons with a given value.
/// @param value Biases initialization value.

void PerceptronLayer::set_biases_constant(const type& value)
{
    biases.setConstant(value);
}


/// Initializes the synaptic weights of all the perceptrons in the layer of perceptrons with a given value.
/// @param value Synaptic weights initialization value.

void PerceptronLayer::set_synaptic_weights_constant(const type& value)
{
    synaptic_weights.setConstant(value);
}


/// Initializes all the biases and synaptic weights in the neural newtork with a given value.
/// @param value Parameters initialization value.

void PerceptronLayer::set_parameters_constant(const type& value)
{
    biases.setConstant(value);

    synaptic_weights.setConstant(value);
}


/// Initializes all the biases and synaptic weights in the neural newtork at random with values comprised
/// between -1 and +1.

void PerceptronLayer::set_parameters_random()
{
    const type minimum = type(-0.2);
    const type maximum = type(0.2);

    for(Index i = 0; i < biases.size(); i++)
    {
        const type random = static_cast<type>(rand()/(RAND_MAX+1.0));

        biases(i) = minimum + (maximum - minimum)*random;
    }

    for(Index i = 0; i < synaptic_weights.size(); i++)
    {
        const type random = static_cast<type>(rand()/(RAND_MAX+1.0));

        synaptic_weights(i) = minimum + (maximum - minimum)*random;
    }
}


void PerceptronLayer::calculate_combinations(const Tensor<type, 2>& inputs,
                                             const Tensor<type, 2>& biases,
                                             const Tensor<type, 2>& synaptic_weights,
                                             Tensor<type, 2>& combinations) const
{
#ifdef OPENNN_DEBUG
    check_columns_number(inputs, get_inputs_number(), LOG);
    //    check_dimensions(biases, 1, get_neurons_number(), LOG);
    check_dimensions(synaptic_weights, get_inputs_number(), get_neurons_number(), LOG);
    check_dimensions(combinations, inputs.dimension(0), get_neurons_number(), LOG);
#endif

    const Index batch_samples_number = inputs.dimension(0);
    const Index biases_number = get_biases_number();

    for(Index i = 0; i < biases_number; i++)
    {
        fill_n(combinations.data() + i*batch_samples_number, batch_samples_number, biases(i));
    }

    combinations.device(*thread_pool_device) += inputs.contract(synaptic_weights, A_B);
}


void PerceptronLayer::calculate_activations(const Tensor<type, 2>& combinations, Tensor<type, 2>& activations) const
{
#ifdef OPENNN_DEBUG
    check_columns_number(combinations, get_neurons_number(), LOG);
    check_dimensions(activations, combinations.dimension(0), get_neurons_number(), LOG);
#endif

    switch(activation_function)
    {
    case ActivationFunction::Linear: linear(combinations, activations); return;

    case ActivationFunction::Logistic: logistic(combinations, activations); return;

    case ActivationFunction::HyperbolicTangent: hyperbolic_tangent(combinations, activations); return;

    case ActivationFunction::Threshold: threshold(combinations, activations); return;

    case ActivationFunction::SymmetricThreshold: symmetric_threshold(combinations, activations); return;

    case ActivationFunction::RectifiedLinear: rectified_linear(combinations, activations); return;

    case ActivationFunction::ScaledExponentialLinear: scaled_exponential_linear(combinations, activations); return;

    case ActivationFunction::SoftPlus: soft_plus(combinations, activations); return;

    case ActivationFunction::SoftSign: soft_sign(combinations, activations); return;

    case ActivationFunction::HardSigmoid: hard_sigmoid(combinations, activations); return;

    case ActivationFunction::ExponentialLinear: exponential_linear(combinations, activations); return;
    }
}


void PerceptronLayer::calculate_activations_derivatives(const Tensor<type, 2>& combinations,
                                                        Tensor<type, 2>& activations,
                                                        Tensor<type, 2>& activations_derivatives) const
{
#ifdef OPENNN_DEBUG
    check_columns_number(combinations, get_neurons_number(), LOG);
    check_dimensions(activations, combinations.dimension(0), get_neurons_number(), LOG);
    check_dimensions(activations_derivatives, combinations.dimension(0), get_neurons_number(), LOG);
#endif

    switch(activation_function)
    {
    case ActivationFunction::Linear: linear_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::Logistic: logistic_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::HyperbolicTangent: hyperbolic_tangent_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::Threshold: threshold_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::SymmetricThreshold: symmetric_threshold_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::RectifiedLinear: rectified_linear_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::ScaledExponentialLinear: scaled_exponential_linear_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::SoftPlus: soft_plus_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::SoftSign: soft_sign_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::HardSigmoid: hard_sigmoid_derivatives(combinations, activations, activations_derivatives); return;

    case ActivationFunction::ExponentialLinear: exponential_linear_derivatives(combinations, activations, activations_derivatives); return;
    }
}


Tensor<type, 2> PerceptronLayer::calculate_outputs(const Tensor<type, 2>& inputs)
{
#ifdef OPENNN_DEBUG
    check_columns_number(inputs, get_inputs_number(), LOG);
#endif

    const Index batch_size = inputs.dimension(0);
    const Index outputs_number = get_neurons_number();

    Tensor<type, 2> outputs(batch_size, outputs_number);

    calculate_combinations(inputs, biases, synaptic_weights, outputs);

    calculate_activations(outputs, outputs);

    return outputs;
}


void PerceptronLayer::forward_propagate(const Tensor<type, 2>& inputs,
                                        LayerForwardPropagation* forward_propagation)
{
#ifdef OPENNN_DEBUG
    check_columns_number(inputs, get_inputs_number(), LOG);
#endif

    PerceptronLayerForwardPropagation* perceptron_layer_forward_propagation
            = static_cast<PerceptronLayerForwardPropagation*>(forward_propagation);

    calculate_combinations(inputs,
                           biases,
                           synaptic_weights,
                           perceptron_layer_forward_propagation->combinations);

    calculate_activations_derivatives(perceptron_layer_forward_propagation->combinations,
                                      perceptron_layer_forward_propagation->activations,
                                      perceptron_layer_forward_propagation->activations_derivatives);
}


void PerceptronLayer::forward_propagate(const Tensor<type, 2>& inputs,
                                        Tensor<type, 1> potential_parameters,
                                        LayerForwardPropagation* forward_propagation)
{
#ifdef OPENNN_DEBUG
    check_columns_number(inputs, get_inputs_number(), LOG);
    check_size(potential_parameters, get_parameters_number(), LOG);
#endif

    const Index neurons_number = get_neurons_number();

    const Index inputs_number = get_inputs_number();

    const TensorMap<Tensor<type, 2>> potential_biases(potential_parameters.data(), neurons_number, 1);

    const TensorMap<Tensor<type, 2>> potential_synaptic_weights(potential_parameters.data()+neurons_number, inputs_number, neurons_number);

    PerceptronLayerForwardPropagation* perceptron_layer_forward_propagation
            = static_cast<PerceptronLayerForwardPropagation*>(forward_propagation);

    calculate_combinations(inputs,
                           potential_biases,
                           potential_synaptic_weights,
                           perceptron_layer_forward_propagation->combinations);

    calculate_activations_derivatives(perceptron_layer_forward_propagation->combinations,
                                      perceptron_layer_forward_propagation->activations,
                                      perceptron_layer_forward_propagation->activations_derivatives);
}


void PerceptronLayer::calculate_hidden_delta(LayerForwardPropagation* next_layer_forward_propagation,
                                             LayerBackPropagation* next_layer_back_propagation,
                                             LayerBackPropagation* layer_back_propagation) const
{
    PerceptronLayerBackPropagation* perceptron_layer_back_propagation =
            static_cast<PerceptronLayerBackPropagation*>(layer_back_propagation);

    switch(next_layer_back_propagation->layer_pointer->get_type())
    {
    case Type::Perceptron:
    {
        PerceptronLayerForwardPropagation* next_perceptron_layer_forward_propagation =
                static_cast<PerceptronLayerForwardPropagation*>(next_layer_forward_propagation);

        PerceptronLayerBackPropagation* next_perceptron_layer_back_propagation =
                static_cast<PerceptronLayerBackPropagation*>(next_layer_back_propagation);

        calculate_hidden_delta_perceptron(next_perceptron_layer_forward_propagation,
                                          next_perceptron_layer_back_propagation,
                                          perceptron_layer_back_propagation);
    }
        break;

    case Type::Probabilistic:
    {
        ProbabilisticLayerForwardPropagation* next_probabilistic_layer_forward_propagation =
                static_cast<ProbabilisticLayerForwardPropagation*>(next_layer_forward_propagation);

        ProbabilisticLayerBackPropagation* next_probabilistic_layer_back_propagation =
                static_cast<ProbabilisticLayerBackPropagation*>(next_layer_back_propagation);

        calculate_hidden_delta_probabilistic(next_probabilistic_layer_forward_propagation,
                                             next_probabilistic_layer_back_propagation,
                                             perceptron_layer_back_propagation);
    }
        break;

    default: return;
    }
}


void PerceptronLayer::calculate_hidden_delta_perceptron(PerceptronLayerForwardPropagation* next_forward_propagation,
                                                        PerceptronLayerBackPropagation* next_back_propagation,
                                                        PerceptronLayerBackPropagation* back_propagation) const
{
    const Tensor<type, 2>& next_synaptic_weights = static_cast<PerceptronLayer*>(next_back_propagation->layer_pointer)->get_synaptic_weights();

    back_propagation->delta.device(*thread_pool_device) =
            (next_back_propagation->delta*next_forward_propagation->activations_derivatives).contract(next_synaptic_weights, A_BT);
}


void PerceptronLayer::calculate_hidden_delta_probabilistic(ProbabilisticLayerForwardPropagation* next_forward_propagation,
                                                           ProbabilisticLayerBackPropagation* next_back_propagation,
                                                           PerceptronLayerBackPropagation* back_propagation) const
{
    const ProbabilisticLayer* probabilistic_layer_pointer = static_cast<ProbabilisticLayer*>(next_back_propagation->layer_pointer);

    const Tensor<type, 2>& next_synaptic_weights = probabilistic_layer_pointer->get_synaptic_weights();

    const Index batch_samples_number = back_propagation->batch_samples_number;

    const Index next_neurons_number = probabilistic_layer_pointer->get_biases_number();

    if(probabilistic_layer_pointer->get_neurons_number() == 1) // Binary
    {
        TensorMap< Tensor<type, 2> > activations_derivatives_2d(next_forward_propagation->activations_derivatives.data(),
                                                                 batch_samples_number, next_neurons_number);
        back_propagation->delta.device(*thread_pool_device) =
                (next_back_propagation->delta*activations_derivatives_2d.reshape(Eigen::array<Index,2> {{activations_derivatives_2d.dimension(0),1}})).contract(next_synaptic_weights, A_BT);
    }
    else // Multiple
    {
        if(probabilistic_layer_pointer->get_activation_function() != ProbabilisticLayer::ActivationFunction::Softmax)
        {
            back_propagation->delta.device(*thread_pool_device) =
                    (next_back_propagation->delta*next_forward_propagation->activations_derivatives.reshape(Eigen::array<Index,2> {{next_forward_propagation->activations_derivatives.dimension(0),1}})).contract(next_synaptic_weights, A_BT);
        }
        else
        {
            const Index samples_number = next_back_propagation->delta.dimension(0);
            const Index outputs_number = next_back_propagation->delta.dimension(1);
            const Index next_layer_neurons_number = probabilistic_layer_pointer->get_neurons_number();

            if(outputs_number != next_layer_neurons_number)
            {
                ostringstream buffer;

                buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                       << "void calculate_hidden_delta_probabilistic(ProbabilisticLayerForwardPropagation*,ProbabilisticLayerBackPropagation*,PerceptronLayerBackPropagation*) const.\n"
                       << "Number of columns in delta (" << outputs_number << ") must be equal to number of neurons in probabilistic layer (" << next_layer_neurons_number << ").\n";

                throw logic_error(buffer.str());
            }

            if(next_forward_propagation->activations_derivatives.dimension(1) != next_layer_neurons_number)
            {
                ostringstream buffer;

                buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                       << "void calculate_hidden_delta_probabilistic(ProbabilisticLayerForwardPropagation*,ProbabilisticLayerBackPropagation*,PerceptronLayerBackPropagation*) const.\n"
                       << "Dimension 1 of activations derivatives (" << outputs_number << ") must be equal to number of neurons in probabilistic layer (" << next_layer_neurons_number << ").\n";

                throw logic_error(buffer.str());
            }

            if(next_forward_propagation->activations_derivatives.dimension(2) != next_layer_neurons_number)
            {
                ostringstream buffer;

                buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                       << "void calculate_hidden_delta_probabilistic(ProbabilisticLayerForwardPropagation*,ProbabilisticLayerBackPropagation*,PerceptronLayerBackPropagation*) const.\n"
                       << "Dimension 2 of activations derivatives (" << outputs_number << ") must be equal to number of neurons in probabilistic layer (" << next_layer_neurons_number << ").\n";

                throw logic_error(buffer.str());
            }

            const Index step = next_layer_neurons_number*next_layer_neurons_number;

            for(Index i = 0; i < samples_number; i++)
            {
                next_back_propagation->delta_row = next_back_propagation->delta.chip(i,0);

                TensorMap< Tensor<type, 2> > activations_derivatives_matrix(next_forward_propagation->activations_derivatives.data() + i*step,
                                                                            next_layer_neurons_number, next_layer_neurons_number);

                next_back_propagation->error_combinations_derivatives.chip(i,0) =
                        next_back_propagation->delta_row.contract(activations_derivatives_matrix, AT_B);
            }

            back_propagation->delta.device(*thread_pool_device) =
                    next_back_propagation->error_combinations_derivatives.contract(next_synaptic_weights, A_BT);
        }
    }
}


void PerceptronLayer::calculate_hidden_delta_lm(LayerForwardPropagation* next_layer_forward_propagation,
                                                LayerBackPropagationLM* next_layer_back_propagation,
                                                LayerBackPropagationLM* layer_back_propagation) const
{
    PerceptronLayerBackPropagationLM* perceptron_layer_back_propagation =
            static_cast<PerceptronLayerBackPropagationLM*>(layer_back_propagation);

    switch(next_layer_back_propagation->layer_pointer->get_type())
    {
    case Type::Perceptron:
    {
        PerceptronLayerForwardPropagation* next_perceptron_layer_forward_propagation =
                static_cast<PerceptronLayerForwardPropagation*>(next_layer_forward_propagation);

        PerceptronLayerBackPropagationLM* next_perceptron_layer_back_propagation =
                static_cast<PerceptronLayerBackPropagationLM*>(next_layer_back_propagation);

        calculate_hidden_delta_perceptron_lm(next_perceptron_layer_forward_propagation,
                                             next_perceptron_layer_back_propagation,
                                             perceptron_layer_back_propagation);
    }
        break;

    case Type::Probabilistic:
    {
        ProbabilisticLayerForwardPropagation* next_probabilistic_layer_forward_propagation =
                static_cast<ProbabilisticLayerForwardPropagation*>(next_layer_forward_propagation);

        ProbabilisticLayerBackPropagationLM* next_probabilistic_layer_back_propagation =
                static_cast<ProbabilisticLayerBackPropagationLM*>(next_layer_back_propagation);

        calculate_hidden_delta_probabilistic_lm(next_probabilistic_layer_forward_propagation,
                                                next_probabilistic_layer_back_propagation,
                                                perceptron_layer_back_propagation);
    }
        break;

    default: return;
    }
}


void PerceptronLayer::calculate_hidden_delta_perceptron_lm(PerceptronLayerForwardPropagation* next_forward_propagation,
                                                           PerceptronLayerBackPropagationLM* next_back_propagation,
                                                           PerceptronLayerBackPropagationLM* back_propagation) const
{
    const Tensor<type, 2>& next_synaptic_weights = static_cast<PerceptronLayer*>(next_back_propagation->layer_pointer)->get_synaptic_weights();

    back_propagation->delta.device(*thread_pool_device) =
            (next_back_propagation->delta*next_forward_propagation->activations_derivatives.reshape(Eigen::array<Index,2> {{next_forward_propagation->activations_derivatives.dimension(0),1}})).contract(next_synaptic_weights, A_BT);
}


void PerceptronLayer::calculate_hidden_delta_probabilistic_lm(ProbabilisticLayerForwardPropagation* next_forward_propagation,
                                                              ProbabilisticLayerBackPropagationLM* next_back_propagation,
                                                              PerceptronLayerBackPropagationLM* back_propagation) const
{           
    const ProbabilisticLayer* probabilistic_layer_pointer = static_cast<ProbabilisticLayer*>(next_back_propagation->layer_pointer);

    const Tensor<type, 2>& next_synaptic_weights = probabilistic_layer_pointer->get_synaptic_weights();

    if(probabilistic_layer_pointer->get_activation_function() == ProbabilisticLayer::ActivationFunction::Softmax)
    {
        const Index samples_number = next_back_propagation->delta.dimension(0);
        const Index outputs_number = next_back_propagation->delta.dimension(1);
        const Index next_layer_neurons_number = probabilistic_layer_pointer->get_neurons_number();

        if(outputs_number != next_layer_neurons_number)
        {
            ostringstream buffer;

            buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                   << "void calculate_hidden_delta_probabilistic(ProbabilisticLayerForwardPropagation*,ProbabilisticLayerBackPropagationLM*,PerceptronLayerBackPropagationLM*) const.\n"
                   << "Number of columns in delta (" << outputs_number << ") must be equal to number of neurons in probabilistic layer (" << next_layer_neurons_number << ").\n";

            throw logic_error(buffer.str());
        }

        if(next_forward_propagation->activations_derivatives.dimension(1) != next_layer_neurons_number)
        {
            ostringstream buffer;

            buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                   << "void calculate_hidden_delta_probabilistic(ProbabilisticLayerForwardPropagation*,ProbabilisticLayerBackPropagationLM*,PerceptronLayerBackPropagationLM*) const.\n"
                   << "Dimension 1 of activations derivatives (" << outputs_number << ") must be equal to number of neurons in probabilistic layer (" << next_layer_neurons_number << ").\n";

            throw logic_error(buffer.str());
        }

        if(next_forward_propagation->activations_derivatives.dimension(2) != next_layer_neurons_number)
        {
            ostringstream buffer;

            buffer << "OpenNN Exception: ProbabilisticLayer class.\n"
                   << "void calculate_hidden_delta_probabilistic(ProbabilisticLayerForwardPropagation*,ProbabilisticLayerBackPropagationLM*,PerceptronLayerBackPropagationLM*) const.\n"
                   << "Dimension 2 of activations derivatives (" << outputs_number << ") must be equal to number of neurons in probabilistic layer (" << next_layer_neurons_number << ").\n";

            throw logic_error(buffer.str());
        }

        const Index step = next_layer_neurons_number*next_layer_neurons_number;

        for(Index i = 0; i < samples_number; i++)
        {
            next_back_propagation->delta_row = next_back_propagation->delta.chip(i,0);

            TensorMap< Tensor<type, 2> > activations_derivatives_matrix(next_forward_propagation->activations_derivatives.data() + i*step,
                                                                        next_layer_neurons_number, next_layer_neurons_number);

            next_back_propagation->error_combinations_derivatives.chip(i,0) =
                    next_back_propagation->delta_row.contract(activations_derivatives_matrix, AT_B);
        }

        back_propagation->delta.device(*thread_pool_device) =
                (next_back_propagation->error_combinations_derivatives).contract(next_synaptic_weights, A_BT);
    }
    else
    {
        back_propagation->delta.device(*thread_pool_device) =
                (next_back_propagation->delta*next_forward_propagation->activations_derivatives.reshape(Eigen::array<Index,2> {{next_forward_propagation->activations_derivatives.dimension(0),1}})).contract(next_synaptic_weights, A_BT);
    }
}


void PerceptronLayer::calculate_squared_errors_Jacobian_lm(const Tensor<type, 2>& inputs,
                                                           LayerForwardPropagation* forward_propagation,
                                                           LayerBackPropagationLM* back_propagation)
{
    PerceptronLayerForwardPropagation* perceptron_layer_forward_propagation =
            static_cast<PerceptronLayerForwardPropagation*>(forward_propagation);

    PerceptronLayerBackPropagationLM* perceptron_layer_back_propagation_lm =
            static_cast<PerceptronLayerBackPropagationLM*>(back_propagation);

    const Index samples_number = inputs.dimension(0);

    const Index inputs_number = get_inputs_number();
    const Index neurons_number = get_neurons_number();

    Index parameter_index = 0;

    for(Index sample = 0; sample < samples_number; sample++)
    {
        parameter_index = 0;

        for(Index neuron = 0; neuron < neurons_number; neuron++)
        {
            for(Index input = 0; input <  inputs_number; input++)
            {
                perceptron_layer_back_propagation_lm->squared_errors_Jacobian(sample, neurons_number+parameter_index) =
                        perceptron_layer_back_propagation_lm->delta(sample, neuron) *
                        perceptron_layer_forward_propagation->activations_derivatives(sample, neuron) *
                        inputs(sample, input);

                parameter_index++;
            }

            perceptron_layer_back_propagation_lm->squared_errors_Jacobian(sample, neuron) =
                    perceptron_layer_back_propagation_lm->delta(sample, neuron) *
                    perceptron_layer_forward_propagation->activations_derivatives(sample, neuron);
        }
    }
}


void PerceptronLayer::insert_squared_errors_Jacobian_lm(LayerBackPropagationLM * back_propagation ,
                                                        const Index & index,
                                                        Tensor<type, 2>& squared_errors_Jacobian) const
{
    PerceptronLayerBackPropagationLM* perceptron_layer_back_propagation_lm =
            static_cast<PerceptronLayerBackPropagationLM*>(back_propagation);

    const Index batch_samples_number = perceptron_layer_back_propagation_lm->squared_errors_Jacobian.dimension(0);
    const Index layer_parameters_number = get_parameters_number();

    memcpy(squared_errors_Jacobian.data() + index,
           perceptron_layer_back_propagation_lm->squared_errors_Jacobian.data(),
           static_cast<size_t>(layer_parameters_number*batch_samples_number)*sizeof(type));
}


void PerceptronLayer::calculate_error_gradient(const Tensor<type, 2>& inputs,
                                               LayerForwardPropagation* forward_propagation,
                                               LayerBackPropagation* back_propagation) const
{
    PerceptronLayerForwardPropagation* perceptron_layer_forward_propagation =
            static_cast<PerceptronLayerForwardPropagation*>(forward_propagation);

    PerceptronLayerBackPropagation* perceptron_layer_back_propagation =
            static_cast<PerceptronLayerBackPropagation*>(back_propagation);

    perceptron_layer_back_propagation->biases_derivatives.device(*thread_pool_device) =
            (perceptron_layer_back_propagation->delta*perceptron_layer_forward_propagation->activations_derivatives).sum(Eigen::array<Index, 1>({0}));

    perceptron_layer_back_propagation->synaptic_weights_derivatives.device(*thread_pool_device) =
            inputs.contract(perceptron_layer_back_propagation->delta*perceptron_layer_forward_propagation->activations_derivatives, AT_B);
}


void PerceptronLayer::insert_gradient(LayerBackPropagation* back_propagation,
                                      const Index& index,
                                      Tensor<type, 1>& gradient) const
{
    PerceptronLayerBackPropagation* perceptron_layer_back_propagation =
            static_cast<PerceptronLayerBackPropagation*>(back_propagation);

    const Index biases_number = get_biases_number();
    const Index synaptic_weights_number = get_synaptic_weights_number();

    memcpy(gradient.data() + index,
           perceptron_layer_back_propagation->biases_derivatives.data(),
           static_cast<size_t>(biases_number)*sizeof(type));

    memcpy(gradient.data() + index + biases_number,
           perceptron_layer_back_propagation->synaptic_weights_derivatives.data(),
           static_cast<size_t>(synaptic_weights_number)*sizeof(type));
}


/// Returns a string with the expression of the inputs-outputs relationship of the layer.
/// @param inputs_names vector of strings with the name of the layer inputs.
/// @param outputs_names vector of strings with the name of the layer outputs.

string PerceptronLayer::write_expression(const Tensor<string, 1>& inputs_names, const Tensor<string, 1>& outputs_names) const
{
#ifdef OPENNN_DEBUG
    //    check_size(inputs_names, get_inputs_number(), LOG);
    //    check_size(outputs_names, get_neurons_number(), LOG);
#endif

    ostringstream buffer;

    for(Index j = 0; j < outputs_names.size(); j++)
    {
        const Tensor<type, 1> synaptic_weights_column =  synaptic_weights.chip(j,1);

        buffer << outputs_names[j] << " = " << write_activation_function_expression() << "( " << biases(0,j) << " +";

        for(Index i = 0; i < inputs_names.size() - 1; i++)
        {
            buffer << " (" << inputs_names[i] << "*" << synaptic_weights_column(i) << ") +";
        }

        buffer << " (" << inputs_names[inputs_names.size() - 1] << "*" << synaptic_weights_column[inputs_names.size() - 1] << ") );\n";
    }

    return buffer.str();
}


void PerceptronLayer::from_XML(const tinyxml2::XMLDocument& document)
{
    ostringstream buffer;

    // Perceptron layer

    const tinyxml2::XMLElement* perceptron_layer_element = document.FirstChildElement("PerceptronLayer");

    if(!perceptron_layer_element)
    {
        buffer << "OpenNN Exception: PerceptronLayer class.\n"
               << "void from_XML(const tinyxml2::XMLDocument&) method.\n"
               << "PerceptronLayer element is nullptr.\n";

        throw logic_error(buffer.str());
    }

    // Layer name

    const tinyxml2::XMLElement* layer_name_element = perceptron_layer_element->FirstChildElement("LayerName");

    if(!layer_name_element)
    {
        buffer << "OpenNN Exception: PerceptronLayer class.\n"
               << "void from_XML(const tinyxml2::XMLDocument&) method.\n"
               << "LayerName element is nullptr.\n";

        throw logic_error(buffer.str());
    }

    if(layer_name_element->GetText())
    {
        set_name(layer_name_element->GetText());
    }

    // Inputs number

    const tinyxml2::XMLElement* inputs_number_element = perceptron_layer_element->FirstChildElement("InputsNumber");

    if(!inputs_number_element)
    {
        buffer << "OpenNN Exception: PerceptronLayer class.\n"
               << "void from_XML(const tinyxml2::XMLDocument&) method.\n"
               << "InputsNumber element is nullptr.\n";

        throw logic_error(buffer.str());
    }

    if(inputs_number_element->GetText())
    {
        set_inputs_number(static_cast<Index>(stoi(inputs_number_element->GetText())));
    }

    // Neurons number

    const tinyxml2::XMLElement* neurons_number_element = perceptron_layer_element->FirstChildElement("NeuronsNumber");

    if(!neurons_number_element)
    {
        buffer << "OpenNN Exception: PerceptronLayer class.\n"
               << "void from_XML(const tinyxml2::XMLDocument&) method.\n"
               << "NeuronsNumber element is nullptr.\n";

        throw logic_error(buffer.str());
    }

    if(neurons_number_element->GetText())
    {
        set_neurons_number(static_cast<Index>(stoi(neurons_number_element->GetText())));
    }

    // Activation function

    const tinyxml2::XMLElement* activation_function_element = perceptron_layer_element->FirstChildElement("ActivationFunction");

    if(!activation_function_element)
    {
        buffer << "OpenNN Exception: PerceptronLayer class.\n"
               << "void from_XML(const tinyxml2::XMLDocument&) method.\n"
               << "ActivationFunction element is nullptr.\n";

        throw logic_error(buffer.str());
    }

    if(activation_function_element->GetText())
    {
        set_activation_function(activation_function_element->GetText());
    }

    // Parameters

    const tinyxml2::XMLElement* parameters_element = perceptron_layer_element->FirstChildElement("Parameters");

    if(!parameters_element)
    {
        buffer << "OpenNN Exception: PerceptronLayer class.\n"
               << "void from_XML(const tinyxml2::XMLDocument&) method.\n"
               << "Parameters element is nullptr.\n";

        throw logic_error(buffer.str());
    }

    if(parameters_element->GetText())
    {
        const string parameters_string = parameters_element->GetText();

        set_parameters(to_type_vector(parameters_string, ' '));
    }
}


void PerceptronLayer::write_XML(tinyxml2::XMLPrinter& file_stream) const
{
    ostringstream buffer;

    // Perceptron layer

    file_stream.OpenElement("PerceptronLayer");

    // Layer name
    file_stream.OpenElement("LayerName");
    buffer.str("");
    buffer << layer_name;
    file_stream.PushText(buffer.str().c_str());
    file_stream.CloseElement();

    // Inputs number
    file_stream.OpenElement("InputsNumber");

    buffer.str("");
    buffer << get_inputs_number();

    file_stream.PushText(buffer.str().c_str());

    file_stream.CloseElement();

    // Outputs number

    file_stream.OpenElement("NeuronsNumber");

    buffer.str("");
    buffer << get_neurons_number();

    file_stream.PushText(buffer.str().c_str());

    file_stream.CloseElement();

    // Activation function

    file_stream.OpenElement("ActivationFunction");

    file_stream.PushText(write_activation_function().c_str());

    file_stream.CloseElement();

    // Parameters

    file_stream.OpenElement("Parameters");

    buffer.str("");

    const Tensor<type, 1> parameters = get_parameters();
    const Index parameters_size = parameters.size();

    for(Index i = 0; i < parameters_size; i++)
    {
        buffer << parameters(i);

        if(i != (parameters_size-1)) buffer << " ";
    }

    file_stream.PushText(buffer.str().c_str());

    file_stream.CloseElement();

    // Peceptron layer (end tag)

    file_stream.CloseElement();
}


string PerceptronLayer::write_activation_function_expression() const
{
    switch(activation_function)
    {
    case ActivationFunction::Threshold:
        return "threshold";

    case ActivationFunction::SymmetricThreshold:
        return "symmetric_threshold";

    case ActivationFunction::Logistic:
        return "logistic";

    case ActivationFunction::HyperbolicTangent:
        return "tanh";

    case ActivationFunction::Linear:
        return string();

    case ActivationFunction::RectifiedLinear:
        return "ReLU";

    case ActivationFunction::ExponentialLinear:
        return "ELU";

    case ActivationFunction::ScaledExponentialLinear:
        return "SELU";

    case ActivationFunction::SoftPlus:
        return "soft_plus";

    case ActivationFunction::SoftSign:
        return "soft_sign";

    case ActivationFunction::HardSigmoid:
        return "hard_sigmoid";
    }

    return string();
}


string PerceptronLayer::write_combinations_c() const
{
    ostringstream buffer;

    const Index inputs_number = get_inputs_number();
    const Index neurons_number = get_neurons_number();

    buffer << "\tvector<float> combinations(" << neurons_number << ");\n" << endl;

    for(Index i = 0; i < neurons_number; i++)
    {
        buffer << "\tcombinations[" << i << "] = " << biases(i);

        for(Index j = 0; j < inputs_number; j++)
        {
            buffer << " +" << synaptic_weights(j, i) << "*inputs[" << j << "]";
        }

        buffer << ";" << endl;
    }

    return buffer.str();
}


string PerceptronLayer::write_activations_c() const
{
    ostringstream buffer;

    const Index neurons_number = get_neurons_number();

    buffer << "\n\tvector<float> activations(" << neurons_number << ");\n" << endl;

    for(Index i = 0; i < neurons_number; i++)
    {
        buffer << "\tactivations[" << i << "] = ";

        switch(activation_function)
        {
        case ActivationFunction::HyperbolicTangent:
            buffer << "tanh(combinations[" << i << "]);\n";
            break;

        case ActivationFunction::RectifiedLinear:
            buffer << "combinations[" << i << "] < 0.0 ? 0.0 : combinations[" << i << "];\n";
            break;

        case ActivationFunction::Logistic:
            buffer << "1.0/(1.0 + exp(-combinations[" << i << "]));\n";
            break;

        case ActivationFunction::Threshold:
            buffer << "combinations[" << i << "] >= 0.0 ? 1.0 : 0.0;\n";
            break;

        case ActivationFunction::SymmetricThreshold:
            buffer << "combinations[" << i << "] >= 0.0 ? 1.0 : -1.0;\n";
            break;

        case ActivationFunction::Linear:
            buffer << "combinations[" << i << "];\n";
            break;

        case ActivationFunction::ScaledExponentialLinear:
            buffer << "combinations[" << i << "] < 0.0 ? 1.0507*1.67326*(exp(combinations[" << i << "]) - 1.0) : 1.0507*combinations[" << i << "];\n";
            break;

        case ActivationFunction::SoftPlus:
            buffer << "log(1.0 + exp(combinations[" << i << "]));\n";
            break;

        case ActivationFunction::SoftSign:
            buffer << "combinations[" << i << "] < 0.0 ? combinations[" << i << "]/(1.0 - combinations[" << i << "] ) : combinations[" << i << "]/(1.0 + combinations[" << i << "] );\n";
            break;

        case ActivationFunction::ExponentialLinear:
            buffer << "combinations[" << i << "] < 0.0 ? 1.0*(exp(combinations[" << i << "]) - 1.0) : combinations[" << i << "];\n";
            break;

        case ActivationFunction::HardSigmoid:
            ///@todo
            break;
        }
    }

    return buffer.str();
}


string PerceptronLayer::write_combinations_python() const
{
    ostringstream buffer;

    const Index inputs_number = get_inputs_number();
    const Index neurons_number = get_neurons_number();

    buffer << "\t\tcombinations = [None] * "<<neurons_number<<"\n" << endl;

    for(Index i = 0; i < neurons_number; i++)
    {
        buffer << "\t\tcombinations[" << i << "] = " << biases(i);

        for(Index j = 0; j < inputs_number; j++)
        {
            buffer << " +" << synaptic_weights(j, i) << "*inputs[" << j << "]";
        }

        buffer << " " << endl;
    }

    buffer << "\t\t" << endl;

    return buffer.str();
}


string PerceptronLayer::write_activations_python() const
{
    ostringstream buffer;

    const Index neurons_number = get_neurons_number();

    buffer << "\t\tactivations = [None] * "<<neurons_number<<"\n" << endl;

    for(Index i = 0; i < neurons_number; i++)
    {
        buffer << "\t\tactivations[" << i << "] = ";

        switch(activation_function)
        {

        case ActivationFunction::HyperbolicTangent:
            buffer << "np.tanh(combinations[" << i << "])\n";
            break;

        case ActivationFunction::RectifiedLinear:
            buffer << "np.maximum(0.0, combinations[" << i << "])\n";
            break;

        case ActivationFunction::Logistic:
            buffer << "1.0/(1.0 + np.exp(-combinations[" << i << "]))\n";
            break;

        case ActivationFunction::Threshold:
            buffer << "1.0 if combinations[" << i << "] >= 0.0 else 0.0\n";
            break;

        case ActivationFunction::SymmetricThreshold:
            buffer << "1.0 if combinations[" << i << "] >= 0.0 else -1.0\n";
            break;

        case ActivationFunction::Linear:
            buffer << "combinations[" << i << "]\n";
            break;

        case ActivationFunction::ScaledExponentialLinear:
            buffer << "1.0507*1.67326*(np.exp(combinations[" << i << "]) - 1.0) if combinations[" << i << "] < 0.0 else 1.0507*combinations[" << i << "]\n";
            break;

        case ActivationFunction::SoftPlus:
            buffer << "np.log(1.0 + np.exp(combinations[" << i << "]))\n";
            break;

        case ActivationFunction::SoftSign:
            buffer << "combinations[" << i << "]/(1.0 - combinations[" << i << "] ) if combinations[" << i << "] < 0.0 else combinations[" << i << "]/(1.0 + combinations[" << i << "] )\n";
            break;

        case ActivationFunction::ExponentialLinear:
            buffer << "1.0*(np.exp(combinations[" << i << "]) - 1.0) if combinations[" << i << "] < 0.0 else combinations[" << i << "]\n";
            break;

        case ActivationFunction::HardSigmoid:
            ///@todo
            break;
        }
    }

    return buffer.str();
}


string PerceptronLayer::write_expression_c() const
{
    ostringstream buffer;

    buffer << "vector<float> " << layer_name << "(const vector<float>& inputs)\n{" << endl;

    buffer << write_combinations_c();

    buffer << write_activations_c();

    buffer << "\n\treturn activations;\n}" << endl;

    return buffer.str();
}


string PerceptronLayer::write_expression_python() const
{
    ostringstream buffer;

    buffer << "\tdef " << layer_name << "(self,inputs):\n" << endl;

    buffer << write_combinations_python();

    buffer << write_activations_python();

    buffer << "\n\t\treturn activations;\n" << endl;

    return buffer.str();
}

}

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
