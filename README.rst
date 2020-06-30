============================
 REANA example - MC JEC
============================


About
=====

This `REANA <http://reanahub.io/>`_ reproducible analysis example computes Monte Carlo (MC) jet energy corrections. It involves creating and analysing L1FastJet, L2Relative and L3Absolute MC truth jet energy corrections. The code is used by the CMS Jet Energy Resolution and Corrections (JERC) subgroup. The main goal of the analysis is to use MC truth information and jet matching techniques to create proper jet energy calibrations.

Analysis structure
==================

Making a research data analysis reproducible basically means to provide
"runnable recipes" addressing (1) where is the input data, (2) what software was
used to analyse the data, (3) which computing environments were used to run the
software and (4) which computational workflow steps were taken to run the
analysis. This will permit to instantiate the analysis on the computational
cloud and run the analysis to obtain (5) output results.

1. Input data
-------------

The analysis takes QCD dijet events with and without added pileup as input. The file format is currently AODSIM but will soon migrate to MINIAOD and likely thereafter to NANOAOD.


2. Analysis code
----------------

This anaylsis consist of multiple stages. Each stage uses C++ code, and the CMSSW framework to compile and provide information necessary for jet response and resolution measurements.

The first part of the analysis prepares the input for which the analysis will run on. To enable scattering over the input, to speed up the process and save on memory, the first part prepares a list of pileup files and no pileup files containing the same lumisections. This is achieved by creating a list of event/run/lumi for all Ntuple files and then matches the files that has the same information.

`ListRunLumi.cpp <code/utils/ListRunLumi.cpp>`_ - List lumisections

This script takes one file as input and returns a table with three columns; lumisection, run number and absolute path to file. The table is sorted with the lumisections in increasing order. 

The script has the command line interface

  :code:`./RunListRunLumi {input_file} {algorithm} >> {output_file}`

`MatchFiles.cpp <code/utils/MatchFiles.cpp>`_ - Match lumisection to pileup and no pileup file

The output from previous step in used to return a list of matched lumisections to corresponding pileup and no pileup file. 

The script has the command line interface

  :code:`cat {lumi_file_noPU} {lumi_file_PU} | ./RunMatchFiles > MatchedFiles`

`RunPrepareMatching.cpp <code/utils/RunPrepareMatching.cpp>`_ - Format the matched files

This script takes the list of lumisections matched to files and returns a YAML file with one value containing `{batch_size}` number of paths to pileup files as well as every no pileup file that contains they same lumisections as the batch of pileup files.

As a result we have successfully clustered the pileup files into batches while simultaneously storing a list of all no pileup files with the same lumisections.

The script has the command line interface

  :code:`./RunPrepareMatching MatchedFiles {batch_size} >> {output_file}`
  
The second part of the analysis is when the deriving of the MC truth corrections starts.

`jet_synchtest_x <code/>`_ - Match jets

This script matches events between two samples and then matches the reconstructed jets between those two samples based on particle level. The primary reason for this is to calcualte the difference in pT between a jet that is in an environment where pileup was simulated and the same exact jet when there is no pileup. This tells us the offset or in other words the amount of pileup added to the jet.

The script takes several parameters as input, they can all be found at

`jet_synchfit_x <code/>`_ - Compute L1FastJet

This script is responsible for computing the L1FastJet jet energy corrections. The goal is to determine how offset over area changes with jet pT and rho. The computed offset can then be removed. A function is fitted to a TGraph2D and the fit becomes a single line in the output text file.

The script takes several parameters as input, they can all be found at

`deriveL1.C <code/>`_ - Compute L1FastJet

This script is due to experimenting with different L1 parameterization. It rederives the L1FastJet following the another L1 parameterization.

The script has the command line interface

  ``root -b -q -l `printf "deriveL1.C+(\"{output_path}\",\"{algorithm}\",\"{algo}{cone_size}{jet_type}\",\"{era}\")"``
  
`jet_apply_jec_x <code/>`_ - Apply corrections
 
This script allows you to apply a set of jet energy corrections. Every uncorrected jet collection in the input tree(s) will be corrected according to the levels parameter, while corrected jet collections will be skipped. The parameter levels is a list of correction levels to be applied, such as L1(FastJet), L2(Relative) and L3(Absolute).
 
The script takes several parameters as input, they can all be found at

`jet_response_analyzer_x <code/>`_ - Creates histograms
 
This script creates histograms out of the ROOT trees with applied L1 corrections.
 
The script takes several parameters as input, they can all be found at
 
`jet_l2_correction_x <code/>`_ - Compute L2Relative and L3Absolute
  
This script is responsible for computing the L2Relative and L3Absolute jet energy corrections. For L2 it computes the relative energy correction, w.r.t. the barrel, as a function of jet pT in each eta bin. The barrel refers to the central eta region configured in previous step. For L3 it computes the absolute response as a function of reference pT and correction as a function of jet pT in the barrel.

The script takes several parameters as input, they can all be found at
 
`jet_correction_analyzer_x <code/>`_ - Compute closure
 
This script produces the closure histograms necessary to plot the computed closure.
 
The script takes several parameters as input, they can all be found at
 
`jet_draw_closure_x <code/>`_ - Plot closure
 
This script takes the output from the previous step and makes properly formated response and closure plots compliant with the TDR style. It is able to make response vs pT and response vs eta plots. Further it is also able to plot the ratio between the closure from multiple eras and files.

The script takes several parameters as input, they can all be found at

3. Compute environment
----------------------

In order to be able to rerun the analysis even several years in the future, we
need to "encapsulate the current compute environment", for example to freeze the
ROOT version our analysis is using. We shall achieve this by preparing a `Docker
<https://www.docker.com/>`_ container image for our analysis steps.

This analysis example runs within the `CMSSW <http://cms-sw.github.io/>`_
analysis framework that was packaged for Docker in

4. Analysis workflow
--------------------

This worfklow has mutliple steps, some steps scattered to run in parallel, later to be merged together. We shall use the `Yadage <https://github.com/yadage>`_ workflow engine to
express the computational steps in a declarative manner. The `workflow.yaml <workflow/workflow.yaml>`_ workflow defines the full pipeline.


.. code-block:: console

   +-------------------+
   | Ntuple production |   Running parallel
   +-------------------+
      |      |      |    
     +----+  |  +------+
     | PU |  |  | NoPU |
     +----+  |  +------+   
      |      |      |
      v      v      v
   +-------------------+
   | List lumisections |   Running parallel
   +-------------------+
      |      |      |    
     +----+  |  +------+
     | PU |  |  | NoPU |
     +----+  |  +------+   
      |      |      |
      v      v      v
   +--------------------+
   | Match lumisections |   Single process
   +--------------------+
            |
            |
            v
   +----------------+
   |   Match jets   |   Running parallel
   +----------------+
     |      |     
     |      |  
     v      v   <-- Merge
   +------------+ 
   | Compute L1 |   Single process
   +------------+
            |
            |
            v
   +----------+
   | Apply L1 |   Running parallel
   +----------+
     |      |     
     |      | 
     v      v  
   +-------------------+
   | Produce histograms|   Running parallel
   +-------------------+
     |      |      |    
     |      |      |
     v      v      v               
    +--------------+   
    | Compute L2L3 |   Single process
    +--------------+
           |
           |                             
           v                                                           
   +-----------------------+
   | Compute Closure files |   Running parallel
   +-----------------------+
     |      |      |    
     |      |      |
     v      v      v   <-- Merge
   +---------------------+
   |  Draw Closure plots |   Single process
   +---------------------+
           |
           |
           v
         DONE


5. Output results
-----------------

The interesting fragements generated by this result are the L1 and L2L3 corrections as well as the closure plots. One example plot is the following:

Running the example on REANA cloud
==================================

We start by creating a `reana.yaml <reana.yaml>`_ file describing the above
analysis structure with its inputs, code, runtime environment, computational
workflow steps and expected outputs. In this example we are using the Yadage
workflow specification, with its steps in the `workflow <workflow>`_ directory.


.. code-block:: yaml

    version: 0.6.0
    inputs:
      directories:
        - workflow
    workflow:
      type: yadage
      file: workflow/workflow.yaml
    outputs:
      files:
       - plot_closure/

We can now install the REANA command-line client, run the analysis and download the resulting plots:

.. code-block:: console

    $ # create new virtual environment
    $ virtualenv ~/.virtualenvs/myreana
    $ source ~/.virtualenvs/myreana/bin/activate
    $ # install REANA client
    $ pip install reana-client
    $ # connect to some REANA cloud instance
    $ export REANA_SERVER_URL=https://reana.cern.ch/
    $ export REANA_ACCESS_TOKEN=XXXXXXX
    $ # create new workflow
    $ reana-client create -n my-analysis
    $ export REANA_WORKON=my-analysis
    $ # upload input code and data to the workspace
    $ reana-client upload 
    $ # start computational workflow
    $ reana-client start
    $ # ... should be finished in about 15 minutes
    $ reana-client status
    $ # list output files
    $ reana-client ls
    $ # download generated plots
    $ reana-client download plot_closure/

Please see the `REANA-Client <https://reana-client.readthedocs.io/>`_
documentation for more detailed explanation of typical ``reana-client`` usage
scenarios.

Contributors
============

The list of contributors in alphabetical order:

- Adelina Lintuluoto
