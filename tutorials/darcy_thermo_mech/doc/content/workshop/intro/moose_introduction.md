# MOOSE Introduction

!style halign=center
Multi-physics Object Oriented Simulation Environment

!---

!media darcy_thermo_mech/moose_intro.png style=width:75%;margin-left:auto;margin-right:auto;display:block;

!---

## History and Purpose

- Development started in 2008

- Open-sourced in 2014

- Designed to solve computational engineering problems and
  reduce the expense and time required to develop new =applications= by:

  - being easily extended and maintained
  - working efficiently on a few and many processors
  - providing an object-oriented, extensible system for creating all aspects of a simulation tool

!---

## MOOSE Team

!media darcy_thermo_mech/moose_team.png style=width:85%;margin-left:auto;margin-right:auto;display:block;

!---

!media darcy_thermo_mech/moose_design.png style=width:75%;margin-left:auto;margin-right:auto;display:block;

!---

## General Capabilities

- Continuous and Discontinuous Galerkin FEM
- Finite Volume
- Fully coupled, fully implicit (and explicit)
- Automatic differentiation (AD)
- Unstructured mesh with FEM shapes
- Higher order geometry
- Mesh adaptivity (refinement and coarsening)
- Massively parallel (MPI and threads)
- User code agnostic of dimension, parallelism, shape functions, etc.

!---

## Object-oriented, pluggable system

!media darcy_thermo_mech/moose_systems.png style=width:75%;margin-left:auto;margin-right:auto;display:block;

!---

## Example Code

!media darcy_thermo_mech/moose_code.png style=width:100%;margin-left:auto;margin-right:auto;display:block;box-shadow:none;

!---

## Software Quality

- MOOSE follows an Nuclear Quality Assurance Level 1 (NQA-1) development process
- All commits undergo review using GitHub Pull Requests and must pass a set of application
  regression tests before they are available to our users
- MOOSE includes a test suite and documentation system to allow for agile development while
  maintaining a NQA-1 process
- Utilizes the Continuous Integration Environment for Verification, Enhancement, and Testing (CIVET)

!---

## Development Process

!media darcy_thermo_mech/civet_flow.png style=width:100%;margin-left:auto;margin-right:auto;display:block;

!---

## Community

###### https://github.com/idaholab/moose/discussions

!media darcy_thermo_mech/moose_users.png style=width:50%;margin-left:auto;margin-right:auto;display:block;

!---

!media darcy_thermo_mech/moose_contributors.png style=width:80%;margin-left:auto;margin-right:auto;display:block;background:white;

!---

!media darcy_thermo_mech/moose_add_del.png style=width:80%;margin-left:auto;margin-right:auto;display:block;background:white;
