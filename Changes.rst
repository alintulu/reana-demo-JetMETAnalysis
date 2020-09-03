Changes
=======

03-08-2020
---------------------------

- Plot comparisons of corrections and uncertainties between different runs
- Update submodule JetMETAnalysis to version with pt-clipping as well as fit function "ak4" for L1 and for L2L3
  ```
  -l2ppfit Standard+Gaussian \
  -ptclipfit true
  ```

20-07-2020
---------------------------

- Compile analysis code when creating docker image, as opposed to compiling independently at every step
- Update submodule JetMETAnalysis to version that uses MiniAOD

01-07-2020
---------------------------

- Create descriptive README