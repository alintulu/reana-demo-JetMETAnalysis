#!/bin/bash

rawPath=https://raw.githubusercontent.com/cms-jet/JECDatabase/master/textFiles
ERA='REANA_vCMS-JET_AK4' #'Summer19UL18_V2_MC' #'REANA_vILIASPATCH' #'Splines' #'REANA'
VERSIONS='REANA_vILIAS_AK4 Summer19UL18_V2_MC' #REANA_vILIAS_AK4_OFFICIALL1'REANA_vILIASPATCH REANA_vCMS-JET' #REANA_vILIASPATCH_OFFICIALL1' #Summer19UL18_V5_MC'
ALGO='AK4PFchs'
LEVELS='L1FastJet'  #L2Relative' #'L1FastJet'
DEPENDENT='Eta'
FIX_PT='100' #'30 100 300'
FIX_RHO='20' #'20 40 60'
FIX_ETA='0' # '1.5 2.3 3.0'
OUTPUT_PATH='.'
#OUTPUT_PATH='/eos/user/a/adlintul/www/plots/JEC/Ilias-patch/16_11_2020'

mkdir -p $OUTPUT_PATH
echo "INFO | Writing to $OUTPUT_PATH"

#mkdir -p JECDatabase/$ERA
#cp $1 JECDatabase/$ERA/$ERA\_L1FastJet_$ALGO.txt
#cp $2 JECDatabase/$ERA/$ERA\_L2Relative_$ALGO.txt

#mkdir -p JECDatabase/$VERSIONS
#cp $1 JECDatabase/$VERSIONS/$VERSIONS\_L1FastJet_$ALGO.txt
#cp $2 JECDatabase/$VERSIONS/$VERSIONS\_L2Relative_$ALGO.txt

echo "INFO | Downloading corrections from Github.."
for version in $ERA $VERSIONS; do
    mkdir -p JECDatabase/$version
    for level in $LEVELS; do
        if  wget -q -N $rawPath/$version/$version\_$level\_$ALGO.txt -P JECDatabase/$version; then
	   echo "INFO |" Downloading $rawPath/$version/$version\_$level\_$ALGO.txt into JECDatabase/$version
	else
	   if [ -f JECDatabase/$version/$version\_$level\_$ALGO.txt ]; then
	      echo "INFO |" $version\_$level\_$ALGO.txt already exist in JECDatabase/$version
	   else
	      echo "WARNING |" $version\_$level\_$ALGO.txt does not exist in JECDatabase/$version
	      echo "WARNING | Aborting.."
	      return
	   fi 
	fi
    done
    #wget $rawPath/$version/$version\_"Uncertainty"_$ALGO.txt -P JECDatabase/$version
done

echo

versions=$(echo "$ERA $VERSIONS" | sed -e 's/\s\+/,/g')

if [[ $DEPENDENT == *"all"* ]] || [[ $DEPENDENT == *"Eta"* ]]; then
   echo "INFO | Computing Eta dependent comparisons.."
   for pt in $FIX_PT; do
       echo "    PT" $pt
       for rho in $FIX_RHO; do  
          echo "      Rho" $rho
          for level in $LEVELS; do
             echo "         Level" $level
             ./Execute --Version $versions --Algorithm $ALGO --Level $level --Dependent Eta --Min -4.18 --Max 5.18 --NBin -1 --FixPT $pt --FixRho $rho >> EtaDependent.txt 
          done
          python PlotCompareVersions.py --Dependent Eta --Levels $LEVELS --Versions $ERA $VERSIONS --OutputPath $OUTPUT_PATH --Algorithm $ALGO
          rm EtaDependent.txt
       done
   done

fi

if [[ $DEPENDENT == *"all"* ]] || [[ $DEPENDENT == *"PT"* ]]; then
   echo "INFO | Computing PT dependent comparisons.."
   for eta in $FIX_ETA; do
      echo "    Eta" $eta
      for rho in $FIX_RHO; do
         echo "      Rho" $rho
         for level in $LEVELS; do
            echo "         Level" $level
            ./Execute --Version $versions --Algorithm $ALGO --Level $level --Dependent PT --Min 1 --Max 7000 --NBin 100 --FixEta $eta --FixRho $rho >> PTDependent.txt
         done
         python PlotCompareVersions.py --Dependent PT --Levels $LEVELS --Versions $ERA $VERSIONS --OutputPath $OUTPUT_PATH --Algorithm $ALGO
         rm PTDependent.txt
      done
   done
fi

if [[ $DEPENDENT == *"all"* ]] || [[ $DEPENDENT == *"Rho"* ]]; then
   echo "INFO | Computing Rho dependent comparisons.."
   for eta in $FIX_ETA; do
      echo "    Eta" $eta
      for pt in $FIX_PT; do
         echo "      PT" $pt
         for level in $LEVELS; do
            echo "         Level" $level
            ./Execute --Version $versions --Algorithm $ALGO --Level $level --Dependent Rho --Min 0 --Max 70 --NBin 50 --FixPT $pt --FixEta $eta >> RhoDependent.txt
         done
         python PlotCompareVersions.py --Dependent Rho --Levels $LEVELS --Versions $ERA $VERSIONS --OutputPath $OUTPUT_PATH --Algorithm $ALGO
         rm RhoDependent.txt
      done
   done
fi
