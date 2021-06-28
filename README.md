# CRC-Digital-Communcations-Exercise

The goal is to create an array D out of random bits (array size = k) as well as create the array T and FCS
(T = 2^(n+k) * D + F, F = remainder of division between 2^(n+k) * D and P).
Afterwards, known errors are created randomly using the current BER, and lastly, an error check is made using CRC implementation (see Receiver() and end of main()).
The division is a XOR comparison between the bits of any 2 given arrays.
The results are counted and printed after initiating the above process MAX_TESTS times.

The above code uses the variables given by the user: 
k (number of bits for data to be processed),
BER (precision for creating known error)
P (a bit continuation to divide the data)

The original data created is random. They are saved onto an array with a size of k.
for the given exercise, the above variables are already set, but can be easily changed by the programmer.
Current set:
k = 20, BER = 1e-3, P = 110101

The MAX_TESTS constant can also be changed, as the current value (10 million) takes quite a bit of time to execute (about 8 seconds).

The randomizer used is a pseudo randomizer that is essentially the same for a concurrent run.
Actual randomness implementation doesn't exist in the program, but another implementation can be added by the programmer easily if needed.