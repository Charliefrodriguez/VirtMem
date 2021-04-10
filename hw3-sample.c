/******************************************************************************

 Sample code for 

1) Extracting "N" top order bits (outer bits) from a 32 bit number

2) Extracting "K" middle order bits from a 32 bit number

3) Setting "Kth bit" value in a character array

4) Reading "Kth bit" value in a character array.

*******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Example 1 EXTRACTING OUTER (TOP-ORDER) BITS

static unsigned int get_top_bits(unsigned int value,  int num_bits)
{
    //Assume you would require just the higher order (outer)  bits, 
    //that is first few bits from a number (e.g., virtual address) 
    //So given an  unsigned int value, to extract just the higher order (outer)  “num_bits”
    int num_bits_to_prune = 32 - num_bits; //32 assuming we are using 32-bit address 
    return (value >> num_bits_to_prune);
}


//Example 2 EXTRACTING BITS FROM THE MIDDLE
//Now to extract some bits from the middle from a 32 bit number, 
//assuming you know the number of lower_bits (for example, offset bits in a virtual address)

static unsigned int get_mid_bits (unsigned int value, int num_middle_bits, int num_lower_bits)
{

   //value corresponding to middle order bits we will returning.
   unsigned int mid_bits_value = 0;   
    
   // First you need to remove the lower order bits (e.g. PAGE offset bits). 
   value =    value >> num_lower_bits; 


   // Next, you need to build a mask to prune the outer bits. How do we build a mask?   

   // Step1: First, take a power of 2 for “num_middle_bits”  or simply,  a left shift of number 1.  
   // You could try this in your calculator too.
   unsigned int outer_bits_mask =   (1 << num_middle_bits);  

   // Step 2: Now subtract 1, which would set a total of  “num_middle_bits”  to 1 
   outer_bits_mask = outer_bits_mask-1;

   // Now time to get rid of the outer bits too. Because we have already set all the bits corresponding 
   // to middle order bits to 1, simply perform an AND operation. 
   mid_bits_value =  value &  outer_bits_mask;

  return mid_bits_value;

}

//Example 3
//Function to set a bit at "index"
// bitmap is a region where were store bitmap 
static void set_bit_at_index(char *bitmap, int index)
{
    // We first find the location in the bitmap array where we want to set a bit
    // Because each character can store 8 bits, using the "index", we find which 
    // location in the character array should we set the bit to.
    char *region = ((char *) bitmap) + (index / 8);
    
    // Now, we cannot just write one bit, but we can only write one character. 
    // So, when we set the bit, we should not distrub other bits. 
    // So, we create a mask and OR with existing values
    char bit = 1 << (index % 8);

    // just set the bit to 1. NOTE: If we want to free a bit (*bitmap_region &= ~bit;)
    *region |= bit;
   
    return;
}


//Example 3
//Function to get a bit at "index"
static int get_bit_at_index(char *bitmap, int index)
{
    //Same as example 3, get to the location in the character bitmap array
    char *region = ((char *) bitmap) + (index / 8);
    
    //Create a value mask that we are going to check if bit is set or not
    char bit = 1 << (index % 8);
    
    return (int)(*region >> (index % 8)) & 0x1;
}



int main () {

    unsigned int myaddress = 4026544704;   // Binary  would be 11110000000000000011001001000000

    //Example 1: Finding value of top order (outer) bits

    //Now let’s say we need to extract just the top (outer) 4 bits (1111), which is decimal 15  
    unsigned int outer_bits_value = get_top_bits (myaddress , 4);
    printf("Example 1: Outer bits value %u \n", outer_bits_value); 
    printf("\n");

    //Example 2: Finding value for middle bits
    
    //Now let’s say we need to extract just the middle 20 bits (00000000000000110010), which is decimal 50  
    //In this example, the number of offset bits is  01000000 (8-bits), which is decimal 64
    unsigned int mid_bits_value = get_mid_bits (myaddress, 20, 8);

    printf("Example 2: Middle bits value %u \n", mid_bits_value);
    printf("\n");

    //Example 3: Checking if a bit is set or not
    char *bitmap = (char *)malloc(4);  //We can store 32 bits (4*8-bit per character)
    memset(bitmap,0, 4); //clear everything
    
    //Let's try to set the 17th bit 
    set_bit_at_index(bitmap, 17);
    
    //Let's try to set the 4th bit 
    set_bit_at_index(bitmap, 4);
    
    //Let's try to read 17th bit)
    printf("Example 3: The value at 17th location %d\n", 
            get_bit_at_index(bitmap, 17));
            
    printf("Example 3: The value at 4th location %d\n", 
            get_bit_at_index(bitmap, 4));
            
    printf("Example 3: The value at 3rd location %d\n", 
            get_bit_at_index(bitmap, 3));

    return 0;
}

