// OPERATIONS NEEDED TO BE IMPLEMENTED:
// Greater Than - Done
// Less Than - Done
// Equal To - Done
// Addition - Ongoing
// Subtraction - Ongoing
// Greatet Than Equal To - Not yet started
// Less Than Equal To - Not yet started
// Increment - Not yet started
// Decrement - Not yet started
// Multiplication - Not yet started
// Division - Not yet started
// Modulus - Not yet started
// Exponents - Not yet started

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INT_LENGTH 10000

typedef enum {
    positive = 0,
    negative = 1
} INT_SIGN;

typedef struct {
    // Integer will be in reverse for easier arithmetic
    int digits[MAX_INT_LENGTH];
    // Number of digits in integer
    int length;
    // Sign will be 0 (positive) or 1 (negative), following the binary sign bit convention.
    INT_SIGN sign;
} Bignum;

Bignum initBignum() {
    // Initialize Bignum values. This is needed some of the arithmetic function need to know if the Bignum has already been set, and get rid of garbage values.
    Bignum num;
    num.length = 0;
    num.sign = positive;
    return num;
}

void setBignum(Bignum *numStruct, char numStr[], INT_SIGN sign) {
    int temp;

    // Store sign enum (1 = negative || 0 = positive)
    numStruct->sign = sign;

    // Store number length
    numStruct->length = strlen(numStr);

    // Load numbers into Bignum.digits
    for (int i = 0; i < numStruct->length; i++) {
        numStruct->digits[i] = numStr[i] - '0';
    }

    // Reverse Bignum.digits for easier operations to be done
    for (int i = 0; i < numStruct->length / 2; i++) {
        temp = numStruct->digits[i];
        numStruct->digits[i] = numStruct->digits[numStruct->length - i - 1];
        numStruct->digits[numStruct->length - i - 1] = temp;
    }
}

void intToBignum(Bignum *numStruct, unsigned long long int integer) {
    // Use a counter to track indexes and length. Int is used as the unsigned longlong int data type's max digit count is 20
    int count = 0;

    // Push remainder of the modulo of the last digit of the integer to the Bignum struct, then divide the integer by 10 to shift the integer. Do this untill all digits have been pushed.
    while(integer > 0) {
        numStruct->digits[count] = integer % 10;
        integer = integer / 10;
        count++;
    }

    // Store the count in Bignum.length
    numStruct->length = count;
}


int isGreaterThanBignum(Bignum *num1, Bignum *num2) {
    if (num1->sign == negative && num2->sign == positive) {
        return 0;
    }
    if (num1->sign == positive && num2->sign == negative) {
        return 1;
    }

    if (num1->length > num2->length) {
        return 1;
    }
    if (num1->length < num2->length) {
        return 0;
    }

    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] > num2->digits[i]) {
            return 1;
        }

        if (num1->digits[i] < num2->digits[i]) {
            return 0;
        }
    }

    return 0;
}

int isLessThanBignum(Bignum *num1, Bignum *num2) {
    if (num1->sign == negative && num2->sign == positive) {
        return 1;
    }
    if (num1->sign == positive && num2->sign == negative) {
        return 0;
    }

    if (num1->length > num2->length) {
        return 0;
    }
    if (num1->length < num2->length) {
        return 1;
    }

    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] < num2->digits[i]) {
            return 1;
        }

        if (num1->digits[i] > num2->digits[i]) {
            return 0;
        }
    }

    return 0;
}

int isEqualToBignum(Bignum *num1, Bignum *num2) {
    if (num1->sign != num2->sign) {
        return 0;
    }

    if (num1->length != num2->length) {
        return 0;
    }

    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] != num2->digits[i]) {
            return 0;
        }
    }

    return 1;
}

void trimBignum(Bignum *num) {
    unsigned long long int numOfZeros = 0;

    // Start from the most significant digit, looking for 0s, and keep track of the number of 0s found.
    for (int i = num->length - 1; i >= 0; i--) {
        if (num->digits[i] == 0) {
            numOfZeros++;
        } else {
            break;
        }
    }

    // If 0s were found, trim the Bignum by adjusting the length.
    if (numOfZeros != 0) {
        num->length = num->length - numOfZeros;
    }
}

void addBignum(Bignum *result, Bignum *num1, Bignum *num2) {
    int sum;
    int carry = 0;
    int resultLength = 0;
    // maxLength will cap out at around 18,446,744,073,709,551,615 (ref: C docs). Therefore the Bignum num1 and num2 can only have a maximum of 18,446,744,073,709,551,615 digits.
    unsigned long long int maxLength;

    // Find the longest length
    if (num1->length > num2->length) {
        maxLength = num1->length;
    } else if (num2->length > num1->length) {
        maxLength = num2->length;
    } else {
        maxLength = num1->length;
    }

    // Perform addition
    for (int i = 0; i < maxLength; i++) {
        sum = carry;
        sum += num1->digits[i] + num2->digits[i];
        carry = sum >= 10 ? 1 : 0;
        sum = sum % 10;

        result->digits[i] = sum;

        resultLength++;
    }

    // If there is a remaining carry, append to result. e.g. 8 + 9 = 17 NOT 8 + 9 = 7 (the carry in the buffer was not added)
    if (carry == 1) {
        result->digits[resultLength] = carry;
        resultLength++;
    }

    // Store result digit length
    result->length = resultLength;
}

void subtractBignum(Bignum *result, Bignum *num1, Bignum *num2) {
    // Compare the 2 integers to determine whether to add or subract (subraction rules) and determine the sign of result.

    // Check signs | If signs are different, add the two numbers.
    if (num1->sign == positive && num2->sign == negative) {
        printf("\nThey have different signs! Will perform addition on them...");
        addBignum(result, num1, num2);
        return;
    }
    if (num1->sign == negative && num2->sign == positive) {
        printf("\nThey have different signs! Will perform addition on them...");
        addBignum(result, num1, num2);
        result->sign = negative;
        return;
    }

    // Find minuend and subtrahend | Store in a temp Bignum as minuend's Bignum.digits will be manipulated due to carries. This is also needed as minuend can be either of the two parameters(num1 or num2); If minuend is found. the other number will be the subtrahend.
    Bignum minuend = initBignum();
    Bignum subtrahend = initBignum();

    // Check length | Longer length will automatically be set to the minuend and shorter will be the subtrahend.
    if (num1->length > num2->length) {
        printf("\nNum 1 is longer! It will be set to the minuend...");

        minuend.length = num1->length;
        subtrahend.length = num2->length;

        memcpy(&minuend.digits, num1->digits, sizeof(int) * num1->length);
        memcpy(&subtrahend.digits, num2->digits, sizeof(int) * num2->length);

        result->sign = num1->sign;
    } else if (num1->length < num2->length) {
        printf("\nNum 2 is longer! It will be set to the minuend...");

        minuend.length = num2->length;
        subtrahend.length = num1->length;

        memcpy(&minuend.digits, num2->digits, sizeof(int) * num2->length);
        memcpy(&subtrahend.digits, num1->digits, sizeof(int) * num1->length);

        // If num1 - num2, and bot Bignums have the same sign, and num2 is longer than num1, the result's sign will be the inverse of the sign of the 2 Bignums.
        // E.g.: (+7) - (+10) = -3 or (-7) - (-10) = 3 
        if (num1->sign == positive) {
            result->sign = negative;
        } else if (num1->sign == negative) {
            result->sign = positive;
        }
    }

    // Compare two Bignums | If minuend and subtrahend was not found in the previous conditions, i.e. they're of the same sign and length. Compare the two Bignums.
    if (isGreaterThanBignum(num1, num2) && minuend.length == 0) {
        printf("\nThey have the same length and sign, but num 1 is bigger!...");

        minuend.length = num1->length;
        subtrahend.length = num2->length;

        memcpy(&minuend.digits, num1->digits, sizeof(int) * num1->length);
        memcpy(&subtrahend.digits, num2->digits, sizeof(int) * num2->length);

        result->sign = num1->sign;
    }else if (isLessThanBignum(num1, num2) && minuend.length == 0) {
        printf("\nThey have the same length and sign, but num 2 is bigger!...");

        minuend.length = num2->length;
        subtrahend.length = num1->length;

        memcpy(&minuend.digits, num2->digits, sizeof(int) * num2->length);
        memcpy(&subtrahend.digits, num1->digits, sizeof(int) * num1->length);

        // If num1 - num2, and both Bignums have the same sign,  num2 is greater than num1, the result's sign will be the inverse of the sign of the 2 Bignums.
        // E.g.: (+30) - (+70) = -40 or (-30) - (-70) = 40 
        if (num1->sign == positive) {
            result->sign = negative;
        } else if (num1->sign == negative) {
            result->sign = positive;
        }
    } else if (isEqualToBignum(num1, num2)) {
        printf("\nThey have the same sign, length, and is equal to each other. Result will be 0...\n");

        result->digits[0] = 0;
        result->length = 1;
        result->sign = positive;

        return;
    }

    // Print Minuend and Subtrahend.
    printf("\n\nlen: %d | Minuend: ", minuend.length);
    for (int i = minuend.length - 1; i >= 0; i--) {
        printf("%d", minuend.digits[i]);
    }
    printf("\nlen: %d | Subtrahend: ", subtrahend.length);
    for (int i = subtrahend.length - 1; i >= 0; i--) {
        printf("%d", subtrahend.digits[i]);
    }

    // Once miunend and subtrahends are found, perform subtraction.
    printf("\n");
    int difference;
    unsigned long long int resultLength = 0;

    // Start at the least significant digit (index 0). Then iterate through till the min length (subtrahend length).
    for (int i = 0; i < subtrahend.length; i++) {
        // If current minuend digit is greater than the current subtrahend digit: No need to borrow.
        if (minuend.digits[i] > subtrahend.digits[i]) {
            // Get difference of the current minuend and subtrahend digits and increment the result's digit length counter.
            result->digits[i] = minuend.digits[i] - subtrahend.digits[i];
            resultLength++;

            printf("\nNot Borrowing... : ");
            printf("%d - %d = %d", minuend.digits[i], subtrahend.digits[i], result->digits[i]);
        } else if (minuend.digits[i] < subtrahend.digits[i]) {
            // If borrowing is needed, start from the next digit after the current index, and traverse through (till the last digit of the minuend) until you find a digit that can give you a borrow (anything greater than 0).
            for (int j = i + 1; j < minuend.length; j++) {
                printf("\n    Looking for borrow...");
                if (minuend.digits[j] > 0) {
                    // If a digit that can give a borrow is found. Decrement that digit, then add 10 to the current index that needs a borrow.
                    minuend.digits[j] = minuend.digits[j] - 1;
                    minuend.digits[i] = minuend.digits[i] + 10;

                    // Since a borrow was found. Exit out of loop.
                    // Since a borrow was found. Exit out of loop.
                    break;
                } else if (minuend.digits[j] == 0) {
                    // If the digit is a 0, replace that 0 with 9 and move to the next digit looking for a digit that can give a borrow. We can immediately give it a value of 9, since it is bound to be a 9 as it will be a 10 once a borrow is found, then it will be decremented as it needs to give a borrow to the digit to its right.
                    //
                    // E.g.:
                    // modified minuend:   1,2,9,9,11
                    // original minuend:   1,3,0,0, 1
                    //       subtrahend: -  , , ,2, 3
                    //                    ------------
                    //           result:    1,2,9,7,8
                    minuend.digits[j] = 9;
                }
            }

            // Get difference of the current minuend and subtrahend digits and increment the result's digit length counter.
            result->digits[i] = minuend.digits[i] - subtrahend.digits[i];
            resultLength++;

            printf("\nBorrowed... : ");
            printf("%d - %d = %d", minuend.digits[i], subtrahend.digits[i], result->digits[i]);
        } else if (minuend.digits[i] == subtrahend.digits[i]) {
            // When minuend's and subtrahend's current iteration digit is equal. Set result's current iteration digit to 0.
            result->digits[i] = 0;
            resultLength++;
        }
    }

    printf("\n\nlen: %d | Mdf Minuend: ", minuend.length);
    for (int i = minuend.length - 1; i >= 0; i--) {
        printf("%d|", minuend.digits[i]);
    }

    // If subtrahend is shorter than the minuend, and all neccessary subtractions are finished. Drop down/copy the remaining values of the minuend.
    if (minuend.length > subtrahend.length) {
        printf("\n\nSubtrahend is shorter. Dropping the rest of the numbers... : ");
        for (int i = subtrahend.length; i < minuend.length; i++) {
            result->digits[i] = minuend.digits[i];
            resultLength++;
        }
    }

    printf("\n");

    result->length = resultLength;
}

int main(void) {
    Bignum num1 = initBignum(); 
    Bignum num2 = initBignum(); 
    Bignum result = initBignum();
    
    setBignum(&num1, "700", positive);
    setBignum(&num2, "100", positive);

    // subtractBignum(&result, &num1, &num2);

    printf("\nsgn: %d | len: %d | num 1: ", num1.sign, num1.length);
    for (int i = num1.length - 1; i >= 0 ; i--) {
        printf("%d", num1.digits[i]);
    }
    printf("\nsgn: %d | len: %d | num 2: ", num2.sign, num2.length);
    for (int i = num2.length - 1; i >= 0 ; i--) {
        printf("%d", num2.digits[i]);
    }
    printf("\nsgn: %d | len: %d | result: ", result.sign, result.length);
    for (int i = result.length - 1; i >= 0 ; i--) {
        printf("%d", result.digits[i]);
    }
    
    printf("\n\nnum 1 > num 2 = %d", isGreaterThanBignum(&num1, &num2));
    printf("\nnum 1 < num 2 = %d", isLessThanBignum(&num1, &num2));
    printf("\nnum 1 == num 2 = %d", isEqualToBignum(&num1, &num2));
    
    printf("\n\n");

    return 0;
}