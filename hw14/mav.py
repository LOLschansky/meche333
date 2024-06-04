import matplotlib.pyplot as plt
import numpy as np
import csv
import pandas as pd

def moving_average_filter(data, X):
    filtered_data = []
    for i in range(len(data)):
        if i < X:
            filtered_data.append(0)
        else:
            filtered_data.append(np.mean(data[i-X+1:i+1]))
    return filtered_data

def plot_data_and_fft(data, filtered_data, X, file_name, t):
    sr = len(t)/t[-1]
    Fs = sr # sample rate
    Ts = 1.0/Fs; # sampling interval
    ts = np.arange(0,t[-1],Ts) # time vector
    #y = data # the data to make the fft from
    n = len(data) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range

    Y = np.fft.fft(data)/n # fft computing and normalization
    Y_mav = np.fft.fft(filtered_data)/n # fft computing and normalization 

    Y = Y[range(int(n/2))]
    Y_mav = Y_mav[range(int(n/2))]


    # Plot time domain data
    plt.figure(figsize=(5, 5))

    plt.subplot(2, 1, 1)
    plt.plot(t, data, label='Original Data', color='black')
    plt.plot(t, filtered_data, label=f'Filtered Data (X={X})', color='red')
    plt.title(f'Time Domain Data (Averaged over {X} points)')
    plt.legend()

    # Plot frequency domain data
    plt.subplot(2, 1, 2)
    plt.loglog(frq, abs(Y), label='Original FFT', color='black')
    plt.loglog(frq, abs(Y_mav), label=f'Filtered FFT (X={X})', color='red')
    plt.title('Frequency Domain Data (FFT)')
    plt.legend()

    # Save plot
    plt.tight_layout()
    plt.savefig(f'{file_name}_X{X}.png')
    plt.show()

def main():
    t = [] # column 0
    data = [] # column 1S
    file_name = 'sigD.csv'  # Replace with your CSV file path

    with open(file_name) as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            data.append(float(row[1])) # second column
            #data2.append(float(row[2])) # third column
    
    X_values = [100, 500, 1000]  # Try different X values
    for X in X_values:
        filtered_data = moving_average_filter(data, X)
        plot_data_and_fft(data, filtered_data, X, file_name, t)

if __name__ == "__main__":
    main()