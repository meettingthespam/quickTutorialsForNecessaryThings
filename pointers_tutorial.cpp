#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <numeric>


// POINTERS!
// basically a pointer is just going to store an address in memory 
// make sure the pointer is the same data type "to which it points"



// Passing the pointer to a function, allwing the function to directly change values
// this solves the global vs local variable (for specific problems)
// anytime you're receiving a pointer, it'll be an interger with the pointer * attached to it
void AssignAge3(int* pAge);

int main(){
	
	int age = 43;
	
	// declare a pointer with *
	// (NULL so it's not pointing some random location)
	int* pAge = NULL;
	
	// & reference operator
	// whenever you put it in front of a variable, 
	// it returns the address for that varaible
	
	// assigning the pointer's location to the variable 
	pAge = &age;


	std::cout << "Pointer referenced Address : " << pAge << "\n";
	
	
	// want to get the value at a specific address??
	// use the D-refernce operator
		// using the *pAge means grab the value that the pointer is pointing at
	
	std::cout << "Value at Address : " << *pAge << "\n" ;
	
	
	// create a pointer to an array and cycle through all the data in the array 
	// using ++ (forwards) -- (backwards)
	
	// initializing the array 
	int intArray[] = {1, 2, 3, 4};
	
	// initializing the pointer, 
		// NOTE for arrays you don't have to use the reference operator (the &)
	int* pIntArray = intArray;
	
	// cycling through array with d-reference operator 
	// and the address with just directly calling the pointer
	// and then going forward through it with the ++ after the address (where the pointer is pointing)
	std::cout << "1st " << *pIntArray << " @ Address " << pIntArray << "\n";
	pIntArray++;
	std::cout << "2nd " << *pIntArray << " @ Address " << pIntArray << "\n";
	pIntArray++;
	std::cout << "3rd " << *pIntArray << " @ Address " << pIntArray << "\n";
	pIntArray--;
	std::cout << "2nd " << *pIntArray << " @ Address " << pIntArray << "\n";	
	
	
	int age = 43;
	// & reference operator
	AssignAge3(&age);
	// new age is printed out
	std::cout << "Pointer Age " << age << "\n";


	
	
	
	return 0;
	
	
}


// FUNCTIONS

void AssignAge3(int* pAge){
	*pAge = 22;
}

// END FUNCTIONS 