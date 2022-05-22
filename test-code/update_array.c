#include<stdio.h>

/* This can be used for D1 implementation.
- Update: How I can update values for difference equation
- Sum: How I can use values in the difference equation */

void update_array(int*,int);

int array[4] = {10,20,30,40};

void update_array(int* arrPtr,int len_arr) {
    // Print array info
    printf("Length of array: %i\n",len_arr);

    // Loop through elements of array
    for (int i = 0; i < len_arr; i++) {
        // Print incoming
        printf("\tElement %i is %i",i,*arrPtr);
        // Update elements
        int j = i%4;
        if (j != 3) {
            *arrPtr  = *(arrPtr+1);
        } else {
            *arrPtr = *arrPtr + 10;
        }
        // Print outgoing
        printf("\t\tElement %i is now %i\n",i,*arrPtr);
        // Add to pointer
        arrPtr++;
    }
}

int sum_array(int arr[],int arr_len) {
    // Loop through array elements and get sum
    int sum = 0;
    for (int i = 0; i < arr_len; i++) {
        sum = sum + arr[i];
    }
    return sum;
}


int main() {

    // Length of array
    int length_array = sizeof(array)/sizeof(int);
    printf("Size of array: %i\n",length_array);

    // Loop through array elements and print
    for (int i = 0; i < length_array; i++) {
        printf("\t%i\n",array[i]);
    }

    // Loop for some iterations
    int num_iterations = 6;
    for (int i = 0; i < num_iterations; i++) {
        printf("i = %i\n",i);
        // Update array elements
        update_array(&array[0],length_array);
        // Do something will all array elements (sum)
        printf("Sum of array elements: %i\n",sum_array(array,length_array));
    }
    
    return 0;
}