import numpy as np
import matplotlib.pyplot as plt

# Read in data
data = np.loadtxt("testData.txt", delimiter=' ')
# Number of values per plot
N = int(data[0,0]+1)
# Number of plots
NPlots = len(data[1:,0]) / N

# Produced data
myData = data[1:(N+1),:]
# Published data to compare produced with
publishedData = data[(N+1):,:]

print(N)
print(NPlots)
print(data.shape)
print(len(data[:,0]))
print(myData.shape)
print(publishedData.shape)

# Create main and subplot
fig, (ax_main, ax_sub) = plt.subplots(2, 1, gridspec_kw={'height_ratios': [3, 1]}, sharex=True)
ax_main.grid()
ax_sub.grid()
ax_sub.set_xlabel("Eta")
ax_main.set_ylabel("Correction")
ax_sub.set_ylabel("Produced data/Published data")

# Create colors for plots
color=iter(plt.cm.rainbow(np.linspace(0,1,NPlots)))
c=next(color)

# Draw main plot
ax_main.plot(myData[:,0], myData[:,1],c=c)
ax_main.fill_between(myData[:,0], myData[:,1]-myData[:,2], myData[:,1]+myData[:,2], alpha=0.3, facecolor=c)
ax_main.axvline(x=0, c="black")
ax_sub.axvline(x=0, c="black")

# Draw published data plots
for i in range(NPlots-1):
    c=next(color)
    ax_main.plot(publishedData[i:(N*(i+1)),0], publishedData[i:(N*(i+1)),1],c=c)
    ax_main.fill_between(publishedData[i:(N*(i+1)),0], publishedData[i:(N*(i+1)),1]-publishedData[i:(N*(i+1)),2], publishedData[i:(N*(i+1)),1]+publishedData[i:(N*(i+1)),2], alpha=0.3, facecolor=c)

    # Draw sub plot of ratio
    ax_sub.plot(myData[:,0], myData[:,1]/publishedData[i:(N*(i+1)),1], c=c)


print(myData[:,1])
print(publishedData[i:(N*(i+1)),1])
print(myData[:,0], myData[:,1]/publishedData[i:(N*(i+1)),1])

plt.show()