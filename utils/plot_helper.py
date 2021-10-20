import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack
from scipy import signal
import matplotlib

def get_figsize(columnwidth=555, wf=0.5, hf=(5.**0.5-1.0)/2.0, ):
    """Parameters:
    - wf [float]:  width fraction in columnwidth units
    - hf [float]:  height fraction in columnwidth units.
    Set by default to golden ratio.
    - columnwidth [float]: width of the column in latex. Get this from LaTeX 
    using \showthe\columnwidth
    Returns:  [fig_width,fig_height]: that should be given to matplotlib
    """

    fig_width_pt = columnwidth
    inches_per_pt = 1.0/72.27               # Convert pt to inch
    fig_width = fig_width_pt*inches_per_pt  # width in inches
    fig_height = fig_width*hf      # height in inches

    params = {'backend': 'ps',
    'axes.labelsize': 10,
    'font.size': 10,
    'legend.fontsize': 10,
    #'text.usetex': True,

    'figure.figsize':[fig_width, fig_height] }
    matplotlib.rcParams.update(params)

    return [fig_width, fig_height]

def makeFFT(x, y):

    if(np.size(np.shape(x))):
        x = x[:,0]
    if(np.size(np.shape(y))):
        y = y[:,0]

    T = (x[x.size-1] - x[0])
    #print(T)
    N = x.size
    #print(N)
    fs = int(1/(T/N))
    #print(fs)

    window = signal.windows.flattop(N)
    Aw=np.size(window)/np.sum(window)

    x = np.linspace(0.0, N*T, N)
    xf = np.linspace(0.0, int(fs/2), int(N/2))

    yf = scipy.fftpack.fft(y*window)*Aw
    yfreq = 2.0/N * np.abs(yf[:N//2])
    yfreqdB = 20*np.log10(yfreq)

    upper_freq = int(((96000)/fs)*xf.size)

    # fig, ax = plt.subplots(figsize=(20,10))
    fig, ax = plt.subplots( figsize=get_figsize(), dpi=100)
    xf = xf[0:upper_freq]
    yfreqdB = yfreqdB[0:upper_freq]
    ax.plot(xf, yfreqdB)
    ax.set_xscale('log')
    ax.set_title("FFT")
    ax.set_xlabel("Frequency")
    ax.set_ylabel("dB")
    #ax.set_ylim([-150,0])
    ax.set_xlim([20,upper_freq])
    ax.grid(True, which="both")


    return xf, yfreq, yfreqdB, yfreq.size

def plot_gp(mu, cov, X, X_train=None, Y_train=None, samples=[]):
    X = X.ravel()
    mu = mu.ravel()
    uncertainty = 1.96 * np.sqrt(np.diag(cov))
    
    plt.fill_between(X, mu + uncertainty, mu - uncertainty, alpha=0.2)
    plt.plot(X, mu, label='Mean')
    for i, sample in enumerate(samples):
        plt.plot(X, sample, lw=1, ls='--', label=f'Sample {i+1}')
    if X_train is not None:
        plt.plot(X_train, Y_train, 'rx')