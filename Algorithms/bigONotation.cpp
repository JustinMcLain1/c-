#include <iostream>
#include <math.h>
#include <string.h>
#include <cstddef>     
#include <valarray> 

/// because this is a matrix and you perform n*n the values are squaring 
//so that means this functions is O(n^2) 
void Squares( int number) {

    int n = 4;

    for(int i = 0; i < number; i++){   // this is O(n)
        for(int j = 0; j < number; j++) { // this is O(n)
            std::cout << i << " " << j << std::endl;
        }      
    } 
}


// this function is O(n^3) this has an x y and z axis and you're cubing the value with
// multiple for loops
void cubes(int number) {

    for(int i = 0; i < number; i++){   // this is O(n)
        for(int j = 0; j < number; j++) { // this is O(n)
            for(int k = 0; k < number; k++) { // this is O(n)
             std::cout << i << " " << j << " " << k << std::endl;
            }
        }
    }
}

//----------------------------------------------------------------
//O(LOG N)

//LOGARITHMS DEAL IN BASE 2 
// THIS MEANS THAT 2^X = Y so we just need to find whatever
// the value of llog(base 2) of 8 (ex) = 3 

//RECURSIVE EXAMPLE
//-----------------------------------------------------------------

int logFunction(int number) {

if (number == 0) {
    const char heck = '4';
    return heck;
}
else {
    number = log(number/2);
        return logFunction(number);
}
}

//------------------------------------------------------------
// this is O(LOG N)
// THIS EXAMPLE IS the
//
// ITERATIVE EXAMPLE
//------------------------------------------------------------
void logn(int number) {
    while (number > 1) {
        number = log(number/2); // n will be halved for each iteration // whikle n > 1
    }
    // n = 8 /2 = 4 // in online example math.floor was used, it floors the divison
    // this means it will just round down
}


/// must be an ordered array
// this is for O(LOG N)
int binarySearch(int array[], int size, int searchVal) {

    int low = 0;
    int mid;
    int high = size - 1; 

    while(low <= high) {
        mid = (low + high)/2;
        if(mid == array[mid]) {
            return mid;
        }
        else if (searchVal > array[mid]){
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return -1;
}
//----------------------------------------------------------------
//O(N LOG N)
//----------------------------------------------------------------
void nLogNFunction(int number) {
    int y = number; //set n = 4 we have to store the original n in y to find the iteration
    while(number > 1) { 
        number = log(number/2); // this will divide 4/2 <-- this top level loop is log base 2 of 4 = 2 so its log n
        for (int i = 1; i <= y; i++) { // this will loop 4 times
           std::cout << i << std::endl;
        }
    }
}


//----------------------------------------------------------------
//MERGE SORT 
// THis sort functions is O(n log n)
//----------------------------------------------------------------

// void merge(int leftArr, int rightArr) {
//     //divide and concure
//     //keep splitting and cutting in half 

//     std::string resultArr[] = {};
//     int leftIndex = 0;
//     int rightIndex = 0;

//     //compare elements 
//     while ( leftIndex < leftArr.length && rightIndex < rightArr.length) {  
//         if(leftArr[leftIndex] < rightArr[rightIndex]) {
//             resultArr.push(leftArr[leftIndex]);
//             leftIndex += 1; // as elements are pushed to the array the index incremements
//         }
//         else {
//             resultArr.push(rightArr[rightIndex]);
//             rightIndex += 1;
//         }
//         // if you have two arrays [3, 12, 16] (left array) and [1, 6, 15] (right array)
//         //this will check for the values inside the left and right array and compare them
//         // so 1 is less than three so incremement the right array
//         // then 3 in the left array is less than 6 in the right array and so put 3 into the array
//         // and incremement the left index then repeat so 6 is smaller than 12 so put 6 in the array 
//         //and incremement the right index and so on

//         //the while loop will terminate when the index is greater then the array length but there is still 
//         // a 16 in the right array so fix this by :
//     }
//     return resultArr.concat(leftArr.slice(leftIndex)).concat(rightArr.slice(rightIndex)); // this will tack on slices of the left over array 

// }

// int mergeSort(int arr) {  THIS IS O(LOG N)
//     if (arr.length < 2) {
//         return arr;
//     }

//     int middleIndex = log(arr.length / 2); // this is done to keep the indexes non decimal numbers
//     int leftArr = arr.slice(0, middleIndex); // we want the left side of the array so we slice it
//     int rightArr = arr.slice(middleIndex, arr.length); //slice -- up to end but doesnt include end 
//     //ex: // [1,2,3] it will slice up to end but doesnt include end so it will go to three

//     //recursive part
//     return merge(mergeSort(leftArr),mergeSort(rightArr));   // THIS IS O(N)
// }



//0----------------------------------------------------------------
// 2^n these deal with recursive functions 
//-----------------------------------------------------------------

int fib (int n) { //this is O(2^n-1) -> O(2*4-1) = O(2^3) but we ignore constants so its just 2^n instead of 2*n-1
    if(n == 0) { 
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return fib(n - 1) + (n-2); //ex: fib(4) = fib(3) + fib(2) -> fib(3-1) -> fib(2) + fib(1) -> fib(1) fib(0) -> fib(2-1)+ fib(2-2) (all base cases acheived)
    //
    //this structure has 3 levels when drawn in a tree
    //for level 1 its called 2 times -> 2*1
    //level 2 its called 4 times -> 2^2
}


//----------------------------------------------------------------
// O(N!)
//----------------------------------------------------------------

int f( int number ) {

    if(number==0) {
        std::cout << "***************"<< std::endl;
        return 0;
    }
    for (int i = 0; i < number; i++) { // the for loop makes it factorial
        f(number -1);
    }
    //ex: f(3) =0? no so go to for loop
    // from 0 to 3 call the function - 1
    //so its recursively calling the function in the function
}

int main() {

    Squares(4);
    cubes(4);
    logFunction(4);
    nLogNFunction(4);
    f(3);

//THIS IS FOR BINARY SEARCH
    int a[] = { 12, 23, 34, 65, 73, 79, 84, 93};

    int userVal;
    std::cout << "Enter an integer: " << std::endl;
    std::cin >> userVal;

    int result = binarySearch(a, 8, userVal);

    if (result >= 0) {
        std::cout << "the number is " << a[result] << " was found at the element with index " << result << std::endl;
    }
    else {
        std::cout << "the number is " << userVal << " was not found within this index " << std::endl;
    }
//------------------------------------------------


    return 0;
}