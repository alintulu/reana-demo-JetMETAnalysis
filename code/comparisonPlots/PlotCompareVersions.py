import numpy as np
import matplotlib
matplotlib.use('Agg') # Matplotlib chooses Xwindows backend by default. You need to set matplotlib to not use the Xwindows backend
import matplotlib.pyplot as plt
import argparse

# Set window full size
#manager = plt.get_current_fig_manager()
#manager.resize(*manager.window.maxsize())

# Set text size for plots
plt.rcParams.update({'font.size': 12})

parser = argparse.ArgumentParser()
parser.add_argument('--Dependent')
parser.add_argument('--OutputPath', default=".")
parser.add_argument('--Versions', nargs='+')
parser.add_argument('--Levels', nargs='+')
args = parser.parse_args()

print("Plotting comparison plots with {} dependent".format(args.Dependent))

# Number of versions
NVersions = len(args.Versions)
# Number of levels
NLevels = len(args.Levels)

def plotCorrection(x, y, c, *argv, **kwargs):
    ax = kwargs.pop("ax", None)
    label = kwargs.pop("label", None)
    if not ax:
        _, ax = plt.subplots()
    if label:
        return ax.plot(x, y, label=label, c=c)
    else:
        return ax.plot(x, y, c=c)

def plotUncertanityShading(x, y, dy, *argv, **kwargs):
    ax = kwargs.pop("ax", None)
    if not ax:
        _, ax = plt.subplots()
    ax.fill_between(x, y-dy, y+dy, alpha=0.3)
    return ax

def plotRatio(x, y, y0, c, *argv, **kwargs):
    ax = kwargs.pop("ax", None)
    if not ax:
        _, ax = plt.subplots()
    return ax.plot(x, y0/y, c=c)

def addLabel(ax, *argv, **kwargs):
    title = kwargs.pop("title", "")
    xlabel = kwargs.pop("xlabel", "")
    ylabel = kwargs.pop("ylabel", "")
    ax.set_title(title)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    return ax

def setMode(Dependent, data):
    if (Dependent == "Eta"):
        FixOneLabel = "PT"
        FixTwoLabel = "Rho"
    elif (Dependent == "Rho"):
        FixOneLabel = "PT"
        FixTwoLabel = "Eta"
    elif (Dependent == "PT"):
        FixOneLabel = "Eta"
        FixTwoLabel = "Rho"
    return (FixOneLabel, data[0,1], FixTwoLabel, data[0,2])

fileName = args.Dependent + "Dependent.txt"
# Read in data
data = np.loadtxt(fileName, delimiter=' ')
# Number of values per plot
N = int(data[0,0]+1)

# Loop over versions (like "Autumn18_V19_MC Autumn18_V13h_MC Summer16_07Aug2017_V11_MC Summer16_07Aug2017_V20_MC" etc..)
for l in range(NLevels):
    r = N * NVersions + 1
    dataSubRange = data[l * r:(l+1) * r,:]
    FixOneLabel, FixOne, FixTwoLabel, FixTwo = setMode(args.Dependent, dataSubRange)
    dataSubRange = dataSubRange[1:,:]

    # Set color of plots
    color=iter(plt.cm.rainbow(np.linspace(0, 1, NVersions)))

    # Corrections figure
    corr_fig, (corr_ax_main, corr_ax_sub) = plt.subplots(2, 1, gridspec_kw={'height_ratios': [3, 1]}, sharex=True)
    corr_fig.set_size_inches(32, 18)
    addLabel(corr_ax_main, ylabel="Correction", title="{} - {} dependent ({}: {} {}: {})".format(args.Levels[l], args.Dependent, FixOneLabel, FixOne, FixTwoLabel, FixTwo))
    addLabel(corr_ax_sub, xlabel=args.Dependent, ylabel="{} / Published data".format(args.Versions[0]))

    # Uncertanity figure
    unc_fig, (unc_ax_main, unc_ax_sub) = plt.subplots(2, 1, gridspec_kw={'height_ratios': [3, 1]}, sharex=True)
    unc_fig.set_size_inches(32, 18)
    addLabel(unc_ax_main, ylabel="Uncertanity", title="{} - {} dependent ({}: {} {}: {})".format(args.Levels[l], args.Dependent, FixOneLabel, FixOne, FixTwoLabel, FixTwo))
    addLabel(unc_ax_sub, xlabel=args.Dependent)

    # Loop over levels (like "L1FastJet L2L3Residual" etc...)
    for v in range(NVersions):

        print("  Plotting version {}".format(args.Versions[v]))

        # Set colors for plots
        c=next(color)
        
        # Corrections
        x = dataSubRange[v*N:(v+1)*N,0]
        y = dataSubRange[v*N:(v+1)*N,1]
        y0 = dataSubRange[:N,1]
        # Uncertanity
        dy = dataSubRange[v*N:(v+1)*N,2]
        dy0 = dataSubRange[:N,0]

        # Plot corrections
        plotCorrection(x, y, c, ax=corr_ax_main, label=args.Versions[v])
        if (v !=0): plotRatio(x, y, y0, c, ax=corr_ax_sub)

        # Plot uncertanity
        plotCorrection(x, dy, c, ax=unc_ax_main, label=args.Versions[v])
        if (v !=0): plotRatio(x, dy, dy0, c, ax=unc_ax_sub)
        
    # Shrink current axis's height by 10% on the bottom
    # box = corr_ax_main.get_position()
    # corr_ax_main.set_position([box.x0, box.y0 + box.height * 0.05,
    #                 box.width, box.height * 0.95])

    # unc_ax_main.set_position([box.x0, box.y0 + box.height * 0.05,
    #                 box.width, box.height * 0.95])

    # Put a legend below current axis
    corr_ax_main.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05),
            fancybox=True, shadow=True, ncol=5)
    unc_ax_main.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05),
            fancybox=True, shadow=True, ncol=5)

    #plt.show()
    corr_fig_name = '{}/JECChart_Correction_{}_{}_{}Dependent_{}{}_{}{}.png'.format(args.OutputPath.rstrip('\\'), args.Versions[0], args.Levels[l], args.Dependent, FixOneLabel, FixOne, FixTwoLabel, FixTwo)
    unc_fig_name = '{}/JECChart_Uncertanity_{}_{}_{}Dependent_{}{}_{}{}.png'.format(args.OutputPath.rstrip('\\'), args.Versions[0], args.Levels[l], args.Dependent, FixOneLabel, FixOne, FixTwoLabel, FixTwo)
    corr_fig.savefig(corr_fig_name, bbox_inches='tight', dpi=100)
    print("Saved correction plot as {}".format(corr_fig_name))
    unc_fig.savefig(unc_fig_name, bbox_inches='tight', dpi=100)
    print("Saved uncertanity plot as {}".format(unc_fig_name))
