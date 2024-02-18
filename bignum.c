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

void initBignum(Bignum *numStruct, char numStr[], INT_SIGN sign) {
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
    if (num2->length > num1->length) {
        return 0;
    }

    for (int i = num1->length - 1; i >= 0; i--) {
        if (num1->digits[i] > num2->digits[i]) {
            return 1;
        }

        if (num2->digits[i] > num1->digits[i]) {
            return 0;
        }
    }

    return 0;
}

void subtractBignum(Bignum *result, Bignum *num1, Bignum *num2) {
    // Compare the 2 integers to determine whether to add or subract (subraction rules) and determine the sign of result.

    // Check signs | If signs are different, add the two numbers
    if (num1->sign == positive && num2->sign == negative) {
        printf("Subtracting...\n");
        addBignum(result, num1, num2);
        return;
    }
    if (num1->sign == negative && num2->sign == positive) {
        printf("Subtracting...\n");
        addBignum(result, num1, num2);
        result->sign = negative;
        return;
    }

    // Find minuend | Store in a temp Bignum as array elements will be manipulated due to carries
    unsigned long long int minLength;
    Bignum minuend;

    // Check length | Longer length will automatically be set to the minuend
    if (num1->length > num2->length) {
        minLength = num2->length;

        minuend.length = num1->length;
        memcpy(&minuend.digits, num1->digits, sizeof(int) * num1->length);
    } else if (num2->length > num1->length) {
        minLength = num1->length;

        minuend.length = num2->length;
        memcpy(&minuend.digits, num2->digits, sizeof(int) * num2->length);
    }

    if (isGreaterThanBignum(num1, num2)) {
        printf("They have the same length and sign, but num 1 is bigger!");
        minLength = num1->length;
        minuend.length = num1->length;
        memcpy(&minuend.digits, num1->digits, sizeof(int) * num1->length);
    } else if (isGreaterThanBignum(num2, num1)) {
        printf("They have the same length and sign, but num 2 is bigger!");
        minLength = num2->length;
        minuend.length = num2->length;
        memcpy(&minuend.digits, num2->digits, sizeof(int) * num2->length);
    } else {
        printf("They have the same length, sign, and is equal!");
    }

    printf("\nMinuend: ");
    for (int i = minuend.length - 1; i >= 0; i--) {
        printf("%d", minuend.digits[i]);
    }
    printf("\n");
}

int main(void) {
    Bignum num1, num2, result;
    
    initBignum(&num1, "48001", positive);
    initBignum(&num2, "48001", positive);

    subtractBignum(&result, &num1, &num2);

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
    
    printf("\nnum1 > num2 = %d", isGreaterThanBignum(&num1, &num2));
    
    printf("\n\n");

    return 0;
}