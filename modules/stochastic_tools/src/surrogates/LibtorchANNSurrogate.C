//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "LibtorchANNSurrogate.h"

registerMooseObject("StochasticToolsApp", LibtorchANNSurrogate);

InputParameters
LibtorchANNSurrogate::validParams()
{
  InputParameters params = SurrogateModel::validParams();
  params.addClassDescription("Surrogate that evaluates a feedforward artificial neural net. ");
  return params;
}

LibtorchANNSurrogate::LibtorchANNSurrogate(const InputParameters & parameters)
  : SurrogateModel(parameters),
    _v_means(getModelData<std::vector<Real>>("means")),
    _v_stddevs(getModelData<std::vector<Real>>("stddevs"))
#ifdef LIBTORCH_ENABLED
    ,
    _nn(getModelData<std::shared_ptr<Moose::LibtorchArtificialNeuralNet>>("nn"))
#endif
{
  // We check if MOOSE is compiled with torch, if not this throws an error
  StochasticToolsApp::requiresTorch(*this);
}

Real
LibtorchANNSurrogate::evaluate(const std::vector<Real> &
#ifdef LIBTORCH_ENABLED
                                   x
#endif
) const
{
  Real val(0.0);

#ifdef LIBTORCH_ENABLED
  // Check whether input point has same dimensionality as training data
  mooseAssert(_nn->numInputs() == x.size(),
              "Input point does not match dimensionality of training data.");

  torch::Tensor x_tf = torch::tensor(torch::ArrayRef<Real>(x.data(), x.size())).to(at::kDouble);

  // Center-scale - if this is disabled, this call does not change x_tf
  // Load into tensors. don't copy response moments.
  torch::Tensor t_means = torch::tensor(torch::ArrayRef<Real>(_v_means.data(), _v_means.size() - 1)).to(at::kDouble);
  torch::Tensor t_stddevs = torch::tensor(torch::ArrayRef<Real>(_v_stddevs.data(), _v_stddevs.size() - 1)).to(at::kDouble);
  x_tf = x_tf.sub(t_means).div(t_stddevs);

  // Compute prediction
  val = _nn->forward(x_tf).item<double>();

  // Undo center-scaling.
  val = _v_stddevs.back() * val + _v_means.back();
#endif

  return val;
}
