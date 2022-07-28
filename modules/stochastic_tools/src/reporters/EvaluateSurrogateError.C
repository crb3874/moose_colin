//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

// Stocastic Tools Includes
#include "EvaluateSurrogateError.h"

registerMooseObject("StochasticToolsApp", EvaluateSurrogateError);

InputParameters
EvaluateSurrogateError::validParams()
{
  InputParameters params = EvaluateSurrogate::validParams();
  params.addClassDescription("Tool for evaluating surrogate model error for a set of known responses.");

  params.addRequiredParam<std::vector<ReporterName>>("response",
  "Reporter value of response results, can be vpp with <vpp_name>/<vector_name> or sampler "
  "column with 'sampler/col_<index>'.");

  params.addParam<bool>("compute_rmse", false, "Switch to true to compute RMSE from errors.");
  params.suppressParameter<MultiMooseEnum>("evaluate_std");
  return params;
}

EvaluateSurrogateError::EvaluateSurrogateError(const InputParameters & parameters)
  : EvaluateSurrogate(parameters),
  _responses(getParam<std::vector<ReporterName>>("response")),
  _compute_rmse(getParam<bool>("compute_rmse"))
{
  /// Get all required reporters
  _rvals.resize(_model.size(), nullptr);
  _rvecvals.resize(_model.size(), nullptr);

  for (const auto m : make_range(_model.size()))
  {
    const std::string rtype = _response_types.size() == 1 ? _response_types[0] : _response_types[m];
    if (rtype == "real")
      _rvals[m] = &getReporterValueByName<std::vector<Real>>(_responses[m]);
    else if (rtype == "vector_real")
      _rvecvals[m] = &getReporterValueByName<std::vector<std::vector<Real>>>(_responses[m]);
    else
      paramError("response_type", "Unknown response type ", _response_types[m]);
  }

  if (_compute_rmse)
    _rmse = &declareValueByName<std::vector<Real>>("rmse", std::vector<Real>(_model.size()));
}

void
EvaluateSurrogateError::execute()
{
  /// Call EvaluateSurrogate::execute(). This already creates reporter fields for the model
  /// evaluations - we'll come in afterwards and subtract true response values to get the error.
  EvaluateSurrogate::execute();

  /// Loop over samples and subtract true response
  for (const auto ind : make_range(_sampler.getNumberOfLocalRows()))
  {
    for (const auto m : make_range(_model.size()))
    {
      if (_real_values[m] && _rvals[m])
        (*_real_values[m])[ind] -= (*_rvals[m])[ind];
      else if (_vector_real_values[m] && _rvecvals[m])
        for (const auto k : make_range((*_rvecvals[m])[ind].size()))
          (*_vector_real_values[m])[ind][k] -= (*_rvecvals[m])[ind][k];
      else
        mooseError("Mismatched response types for model evaluation and response -",
                   " this shouldn't be possible, so yell at Colin if you hit this.");
    }
  }

  /// Compute RMSE from errors.
  if (_compute_rmse)
  {
    _rmse->resize(_model.size(), 0.0);
    unsigned int n = _sampler.getNumberOfRows();
    for (const auto ind : make_range(_sampler.getNumberOfLocalRows()))
    {
      for (const auto m : make_range(_model.size()))
      {
        if (_real_values[m] && _rvals[m])
          (*_rmse)[m] += MathUtils::pow((*_real_values[m])[ind], 2)/n;
        else if (_vector_real_values[m] && _rvecvals[m])
          for (const auto k : make_range((*_rvecvals[m])[ind].size()))
            (*_rmse)[m] += MathUtils::pow((*_vector_real_values[m])[ind][k], 2)/n;
      }
    }

    /// Gather RMSE from all processes and finalize.
    gatherSum(*_rmse);
    for (const auto m : make_range(_model.size()))
      (*_rmse)[m] = std::sqrt((*_rmse)[m]);
  }
}
