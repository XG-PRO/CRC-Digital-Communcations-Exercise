//
// Created by XG-PRO on 6/16/2021.
//
#include <bits/stdc++.h>
#include <random>

#define MAX_TESTS 1e+7

using namespace std;

int XOR(int a, int b){
    //A simple tool that returns the XOR result of 2 bits
    if (a==b)
        return 0;
    return 1;
}

int** XOR_Bit_Division(const int* data,int data_length, int* divider, int divider_length, bool shift_five){
    int* remainder;
    int* shifted_data;
    int remainder_length, shifted_data_length;

    bool flag1;
    bool flag2=false;
    int current_pos=0;

    //If need be, we shift the original data five times to the left, adding 5 zeroes at its end
    if (shift_five)
    {
        shifted_data_length = data_length + divider_length - 1;
        shifted_data = new int[shifted_data_length];
        for (int i = 0; i < shifted_data_length; i++)
        {
            if (i < data_length)
                shifted_data[i] = data[i];
            else
                shifted_data[i] = 0;
        }
    }
    //Otherwise the data remains unchanged
    else
    {
        shifted_data_length = data_length;
        shifted_data = new int[shifted_data_length];
        for (int i = 0; i < shifted_data_length; i++)
            shifted_data[i] = data[i];
    }



    //The loop of "division" occurs until the final data (or the remainder) is less than the divider and cannot be divided any further
    while(!flag2)
    {

        //At the start of each loop, find the first bit of data that is non-zero (as division eliminates bits afterwards without changing the original length)
        //This occurs at the start of the loop since the position will be needed before the loop ends to process the remainder
        flag1=false;
        for (int i = 0; i < shifted_data_length  && flag1!=1; i++)
        {
            current_pos = i;
            if (shifted_data[i] != 0)
            {
                flag1 = true;

            }
        }

        //If the data can no longer be divided, end the loop
        if ((shifted_data_length-current_pos)<divider_length)
        {
            flag2 = true;
            remainder_length = shifted_data_length-current_pos;

            //As this is the finishing process, the last remaining data is added to the remainder to be returned
            if (remainder_length>0)
            {
                remainder = new int[remainder_length];
                for (int j = 0; j < remainder_length; j++)
                    remainder[j] = shifted_data[current_pos + j];
            }
            //If the original data is divided perfectly, a pseudo-remainder is created for ease of access on checks
            else
            {
                remainder = new int[1];
                remainder[0]=0;
            }
        }


        //If the loop has not ended and there are data to be divided, the division starts
        if (flag1 && !flag2)
        {
            //For every bit that is in the length of the divider, a XOR comparison occurs for each bit and the result is saved
            for (int i=current_pos;i<current_pos+divider_length;i++)
                shifted_data[i] = XOR(shifted_data[i],divider[i-current_pos]);
        }

    }


    //Since we need both the data of the remainder and its length, we return 2 values in the form of a 2-cell array
    int** end_ar = new int*[2];
    end_ar[0] = remainder;
    end_ar[1] = new int[1];
    end_ar[1][0]=remainder_length;
    return end_ar;


}

int Receiver(int* data, int data_length, int* divider, int divider_length){
    //A XOR division but without bit shifting on the original data
    //If at least 1 bit is found as an error (the remainder is not 0), then it is counted as an error

    int* remainder = XOR_Bit_Division(data,data_length,divider,divider_length,false)[0];
    if (remainder[0]==0)
        return 0;
    else
        return 1;
}


int main(){

    int k, P_length, T_length, known_error_counter =0, crc_error_counter=0;
    int *P;
    int *D;
    int *T;
    double BER;

    //Set default starting numbers for the exercise
    k = 20;
    P_length = 6;
    P = new int[P_length];
    P[0] = 1;
    P[1] = 1;
    P[2] = 0;
    P[3] = 1;
    P[4] = 0;
    P[5] = 1;

    BER = 1e-3;


    //Randomness Generator for bits
    random_device defEngine;
    uniform_real_distribution<double> intDistro(0,1);



    for (int t=0;t<MAX_TESTS;t++)
    {

        //Random data gathering for the initial tests
        D = new int[k];
        for (int i=0;i<k;i++)
            D[i] = (int)intDistro(defEngine);

        //The remainder collected by shifting the original data 5 bits to the left
        int** R = XOR_Bit_Division(D,k,P, P_length,true);
        int* F = R[0];      //The FCS continuation
        int F_length = R[1][0];

        //The T continuation which consists of D*2^(n-k) + F data
        T_length = P_length + k - 1;
        T = new int[T_length];
        for (int i=0;i<T_length;i++)
        {
            //Copy the full original data at the start
            if (i<k)
                T[i]=D[i];
            //Copy the rest of the data from the remainder
            else
            {
                if (T_length - i > F_length)
                    T[i]=0;
                else
                    T[i]=F[i + F_length - T_length];
            }
        }


        //Known Error check using BER
        bool flag=false;
        for (int i=0;i<T_length;i++)
        {
            //Take random numbers that could be within BER precision to alter the final data (this represents a possible error)
            if (intDistro(defEngine)<BER)
            {
                if (T[i]==1)
                    T[i]=0;
                else
                    T[i]=1;
                flag=true;
            }
        }
        //If at least 1 error has been found in the continuation, it is counted
        if (flag)
            known_error_counter++;

        //CRC Error check using a Receiver
        crc_error_counter += Receiver(T,T_length,P,P_length);

    }

    //Results
    cout<<"The number of Known Errors is: "<<known_error_counter<<" or "<<((((double)known_error_counter / (double)MAX_TESTS )) * 100)<<"% of all cases"<<endl;
    cout<<"The number of CRC Found Errors is: "<<crc_error_counter<<" or "<<((((double)crc_error_counter/ (double)MAX_TESTS )) * 100)<<"% of all cases"<<endl;
    cout<<"The number of errors not identified by CRC is: "<<known_error_counter-crc_error_counter<<" or "<<((((double)known_error_counter - (double)crc_error_counter) / (double)crc_error_counter) * 100)<<"% of known cases"<<endl;

}

