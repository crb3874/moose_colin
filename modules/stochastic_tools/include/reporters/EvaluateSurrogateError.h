//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// MOOSE includes
#include "EvaluateSurrogate.h"

/**
 * A tool for evaluating surrogate model error relative to known response data.
 */
class EvaluateSurrogateError : public EvaluateSurrogate
{
public:
  static InputParameters validParams();

  EvaluateSurrogateError(const InputParameters & parameters);
  virtual void execute() override;

protected:
  /// Reporter names to use as responses.
  std::vector<ReporterName> _responses;
  /// Response values, Real type
  std::vector<const std::vector<Real> *> _rvals;
  /// Response values, Vector Real type.
  std::vector<const std::vector<std::vector<Real>> *>  _rvecvals;
  /// Switch to control whether RMSE should be computed from errors.
  const bool _compute_rmse;
  /// If we're computing RMSE, we'll put it in this reporter.
  std::vector<Real> * _rmse;
};
