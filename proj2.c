#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Created by Vladislav Pastushenko
// xpastu04
// 12.01.2019

#define I0 powl(10,-12)
#define Ut 0.0258563

// Function "f"
// -------------------
// Return the difference between the results of the equations
double f(double Up, double U0, double R) {
    return I0 * (exp(Up / Ut) - 1) - (U0 - Up) / R;
}

// Function "amperage"
// -------------------
// It gets voltage and returns amperage
// Calculates with Shockley equation
long double calcAmp(long double Up) {
    return I0 * exp(Up / Ut) - I0;
}


// Function "diode"
// -------------------
// It gets voltage "U0", resistance "R" and accuracy "EPS"
// Returns voltage of diode
double diode(double U0, double R, double EPS) {

    long double a = 0;
    long double b = U0;

    long double mid = b / 2;
    long double fmid = f(mid, U0, R);

    for (int breaker = 0; fabsl(b - a) > EPS; breaker++)
    {
        // If Up is correct:
        // I0 * exp(Up/Ut) - I0 = (U0 - Up)/R
        // It means, that than the difference between the results of the equations closer to zero,
        // that value of U0 is more correct
        // Both equations are growing, but I0 * exp(Up/Ut) - I0 grows faster. 
        // It means, that if fmid > 0 => a < U0 < mid, if fmid < 0 => mid < U0 < b
        if (fmid > 0) {
            b = mid;
        }
        else
            a = mid;

        mid = (a + b) / 2;
        fmid = f(mid, U0, R);

        if (breaker == 52) break; // Infinite cycle checking 
        //(The IEEE 754 standard specifies a binary64 as having significand precision: 53 bits (52 explicitly stored) 
        //(https://en.wikipedia.org/wiki/Double-precision_floating-point_format)
    }
    return mid;
}

int main(int argc, char* argv[]) {

    // Check quantity of arguments 
    if (argc < 4) {
        fprintf(stderr, "error: Too few arguments to the program \nCorrect format\n$ ./proj2 U0 R EPS\n");
        return 1;
    }

    double U0 = atof(argv[1]);

    // Check voltage
    if (U0 < 0) {
        fprintf(stderr, "error: U has to be positive number or 0\n");
        return 1;
    }

    double R = atof(argv[2]);

    // Check resistance
    if (R <= 0) {
        fprintf(stderr, "error: R has to be positive number\n");
        return 1;
    }

    double EPS = atof(argv[3]);

    // Check EPS
    if (EPS < 0) {
        fprintf(stderr, "error: EPS has to be positive number\n");
        return 1;
    }

    long double Up = diode(U0, R, EPS);

    printf("Up=%Lg V\n", Up);
    printf("Ip=%Lg A\n", (U0 - Up) / R); // Prints Id, calculated with Kirchhoff's law

    return 0;
}