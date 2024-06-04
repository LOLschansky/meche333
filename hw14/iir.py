import matplotlib.pyplot as plt
import numpy as np
import csv
import pandas as pd

#Optimal parameter values
#SigA: A = 0.997, B = 0.003
#SigB: A = 0.999, B = 0.001
#SigC: A = 0.985, B = 0.015
#SigD: A = 0.985, B = 0.015
#
#
#
def iir_filter(data, A, B):
    filtered_data = [data[0]]  # Initialize with the first data point
    for i in range(1, len(data)):
        new_value = A * filtered_data[-1] + B * data[i]
        filtered_data.append(new_value)
    return np.array(filtered_data)


def plot_data_and_fft(data, filtered_data, A, B, file_name, t):
    sr = len(t)/t[-1]
    print("sample rate", str(sr))
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
    plt.plot(t, filtered_data, label=f'Filtered Data (A={A}, B={B})', color='red')
    plt.title(f'Time Domain Data (A={A}, B={B})')
    plt.legend()

    # Plot frequency domain data
    plt.subplot(2, 1, 2)
    plt.loglog(frq, np.abs(Y), label='Original FFT', color='black')
    plt.loglog(frq, np.abs(Y_mav), label=f'Filtered FFT (A={A}, B={B})', color='red')
    plt.title('Frequency Domain Data (FFT)')
    plt.legend()

    # Save plot
    plt.tight_layout()
    plt.savefig(f'{file_name}_A{A}_B{B}.png')
    plt.show()

def main():
    t = [] # column 0
    data = [] # column 1S
    file_name = 'sigA.csv'  # Replace with your CSV file path

    with open(file_name) as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            data.append(float(row[1])) # second column
            #data2.append(float(row[2])) # third column
    
    AB_values = [(0.05, 0.95), (0.4, 0.6), (0.5, 0.5), (0.6, 0.4), (0.997, 0.003)]  # Try different A and B values
    for A, B in AB_values:
        filtered_data = iir_filter(data, A, B)
        plot_data_and_fft(data, filtered_data, A, B, file_name, t)

if __name__ == "__main__":
    main()