# Parent image CentOS7 with CVMFS embedded
# CMSSW: 10_6_12
# SCRAM ARCH: slc7_amd64_gcc700
FROM clelange/cmssw:10_6_12

USER cmsusr

# Add code
ADD /code/utils $HOME/utils
ADD /code/JetMETAnalysis $HOME/JetMETAnalysis

# Set up CMSSW area and compile code
RUN source $HOME/utils/compile_code.sh

ENTRYPOINT []
    


