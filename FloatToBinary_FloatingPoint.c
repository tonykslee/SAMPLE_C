/*
Tony Lee
TCSS 333
Homework 7
03/10/2016

This program takes a user input float and converts it into a binary number
in IEEE 754 32-bit Floating point (single precision) format. It also prints
out the steps along the way.
*/
#include <stdio.h>
#include <string.h>

#define FRACTION_LENGTH 23
#define EXPONENT_LENGTH 8
#define SIGN_LENGTH 1
#define BIAS_OFFSET 127

//This struct holds the 3 parts of the floating point format.
//1 bit for the sign, 8 bits for exponent, and 23 bits for fraction.
typedef struct {
    unsigned int fraction : FRACTION_LENGTH;
    unsigned int exponent : EXPONENT_LENGTH;
    unsigned int sign : SIGN_LENGTH;
} floatBits;
    
//This union combines a normal float and the float-struct, so if a 
//float is stored into this union, it can be accessed as a float,
//or via the struct via its three parts.
typedef union {
    float theFloat;
    floatBits bits;
} floatUnion;

//This function creates the binary representation of the integer 'num' and 
//stores it to the char array 'binStr[]'. It then returns the string of the
//'num' in its binary form.
char *storeBinary(unsigned int num, int digits, char binStr[]) {
    int i;
    char *strPtr = binStr;
    
    for (i = digits; i >= 0; i--) {
        if (num >> i & 1) {
            strcat(binStr, "1");
        } else {
            strcat(binStr, "0");
        }
    }
    strcat(binStr, "\0");
    return strPtr;
}

//This function prints the steps of adding the fraction part of
//the floating point format.
float createFraction(char binFrac[]) {
    int i;
    double frac = .5;
    double currentNum = 1.0;
    
    printf("Creating the fraction:\n");
    for (i = 0; i <= FRACTION_LENGTH; i++) {
        printf("fraction = %lf, ", currentNum);
        
        printf("after ");
        if (binFrac[i] == '0') {
            printf("skipping ");
        } else {
            printf("adding ");
            currentNum += frac;
        }
        printf("%lf\n", frac);
        frac *= .5;
    }
    printf("\n");
    return currentNum;
}

//This function prints out the steps of adding the exponent to the
//floating point format.
float applyExponent(int trueExponent, float fracResult) {
    int i;
    printf("Applyint the exponent:\n");
    printf("unbiased exponent = %d\n", trueExponent); 
    if (trueExponent > 0) {
        for (i = 0; i < trueExponent; i++) {
            fracResult *= 2;
            printf("times 2 = %f\n", fracResult);
        }    
    } else {
        for (i = 0; i < (-1 * trueExponent); i++) {
            fracResult /= 2;
            printf("divided by 2 = %f\n", fracResult);
        }   
    }
    return fracResult;
}

//This is the main method that drives the program.
int main(void) {
    floatUnion theNumber;
    float fracResult;
    char *binExpPtr, *binFracPtr;
    char binExp[EXPONENT_LENGTH + 1] = "";
    char binFrac[FRACTION_LENGTH + 1] = "";
    
    printf("Enter a float : ");
    scanf("%f", &theNumber.theFloat);
    printf("Your float was read as: %f\n", theNumber.theFloat);
    
    binExpPtr = storeBinary(theNumber.bits.exponent, EXPONENT_LENGTH - 1, binExp);
    binFracPtr = storeBinary(theNumber.bits.fraction, FRACTION_LENGTH - 1, binFrac);
    
    printf("Your float in 32 bits is: %d%s%s\n", theNumber.bits.sign, binExpPtr, binFracPtr);
    printf("Sign: %d\n", theNumber.bits.sign);
    printf("Exponent: %s\n", binExpPtr);
    printf("Fraction: %s\n\n", binFracPtr);
    
    fracResult = createFraction(binFrac);
    fracResult = applyExponent(theNumber.bits.exponent - BIAS_OFFSET, fracResult);
    
    if (theNumber.bits.sign == 0) {
        printf("\nFinal Answer: ");
    } else {
        printf("\nFinal Answer: -");
    }
    printf("%f", fracResult);
}