# Homework #4
Do not cheat and good luck!

It will be great (and useful for you) if you will document your work in this README and write a sort of documentation for your homework. We may give few extra credits for good documentation.

PART 1

You will be able to run my part 1 using make part1 and then calling it using ./part1
Althought it might read a bit messy when you test part1, when you test for signal handling, it should print out a new -> on the same line

PART 2

I did not create a Makefile for part 2, but you can compile the code using gcc part2.c -o part2 and running it using ./part2
In part2.c I included the header file cse320_functions.h  and you can test the code by running it in the main.
The two arrays of addr_in_use and files_in_use are both called address and files respectively.
Another note is that for my files_in_use struct,  i added another field FILE* filePoint which i used to close and open a file
Before you run your tests. please call cse320_init.

cse320_init() is a function that initalizes both address[] and files[]. It initalizes it so that all 25 elements in both arrays has 0 for the ref_count initially. It doesnt return anthing.

Besides the that one fucntion i created, the rest of the functions what were needed in the spec sheet.

THIS IS WITH THE EXTRA CREDIT 2.1

void* cse320_malloc(int size)
for cse320_malloc, this function takes in a size as a parameter and calls malloc internally. It also checks if the the number of addresses is 25 or more. I do this check by the global variable numOfAddr. Everytime i malloc and add to the address[], i increment numOfAddr. In this function, i check for the first available spot in address[i].ref_count is zero. This means this address has not been used and i malloc, change the ref_count to 1, change the addr to the return address of malloc(size) and increment numOfAddr. The function will return an error if you request a new address but numOfAddr is already caped at 25. If the function will return a void* of malloc(size).

void cse320_free(void* pointer)
This function takes in an address and checks if its inside the address[]. If the address does not exist, it will return an error. If it found the address, but the ref_count is 0, it will also return an error. Otherwise, it sets the ref_count at the position of the address in the array to 0 and free that pointer passed in. 

THIS IS WITH THE EXTRA CREDIT 2.2

FILE* cse320_fopen(char* file, char* command)
This function returns a FILE* if sucessful. It will turn an error if we are trying to open more then 25 files. It does a similar pocess to the malloc function such that if it finds a spot in the array files[] where ref_count is zero, it will open the file and increment the ref_count and set the address/filename to the apporiate fields. This also checks in the array if you're trying to open a file that has already been open. if it does, it just increments the ref_count at that space by 1. It then returns the FILE* of file you're trying to open

int cse320_fclose(FILE* closeFile)
Very similar to the free function, but this time it takes in a FILE* closefile and tries to find it in the files[]. if it doesnt find it, it prints and returns an error. once we fine the position of the closefile in the files[] and the ref_count is already 0, we would print and return an error. If the ref_count is equal to 1, this would close the file and decrement the ref_count to zero and return 0 for sucessful. If the ref_count at that position is greater then 1, then it would just decrement the ref_count and return 1.

void cse320_clean()
This function clears both file[] and address[], setting all its ref_count fields to zero and all its other fields to NULL. 


PART 3
I included the functions needed for part 3 inside cse320_functions.h, so u can test using the same main function after compiling it like part 2

void cse320_settimer(int i)
this function sets the global variable setTimer to whatever the user passes in. This is used to decide how long it takes to reap a child.

void handler()
this handler helps reaps the child when called and calls it self again whenever SIGALRM is triggered to continiously reaping all childs and zombie childs.

pid_t cse320_fork()
this returns the pid when we call fork. this function also implicitly reaps all childs and zombie threads because in the parents process of this function, we change our SIGALRM to our handler function and trigger the alarm which helps reap our processes. This also pauses and recalls alarm  to continiously reap any zombie or child processes. this fucntion returns the childs pid.
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                                                                                                                                       
~                            
