

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



static int get_bit_at_index(char *bitmap, int index)
{
    //Same as example 3, get to the location in the character bitmap array
    char *region = ((char *) bitmap) + (index / 8);
    
    //Create a value mask that we are going to check if bit is set or not
    char bit = 1 << (index % 8);
    
    return (int)(*region >> (index % 8)) & 0x1;
}
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
    *region &= ~bit;
   
    return;
}



int main () {

    //Example 3: Checking if a bit is set or not
    char *bitmap = (char *)malloc(4);  //We can store 32 bits (4*8-bit per character)
		*bitmap = 'A'; 
		*(bitmap+1) = 'G'; 
		*(bitmap+2) = 'C'; 
		*(bitmap+3) = 'D'; 

/* 
We want to skip the 8th bit since that won't be set in the case of ascii valued chars
the value for chars ranges between 33 and 126. Those values are going to be binary values
that aren't 8 bits long but 7. So, the 8th bit will not be used. Thus, we skip it to stay within each entry

*/


//    memset(bitmap,0, 4); //clear everything
/*    
    //Let's try to set the 17th bit 
    set_bit_at_index(bitmap, 17);
    
    //Let's try to set the 4th bit 
    set_bit_at_index(bitmap, 4);
  */  
    //Let's try to read 17th bit)
    printf("Example 3: The value at 17th location %d\n", get_bit_at_index(bitmap, 0)); 
		int i =0;
		set_bit_at_index(bitmap, 0);
		for(i;i<31;i++){ 
			if( (i+1) % 8 != 0){
			printf("%d \n",get_bit_at_index(bitmap, i)); 
			} 
			else{ 
			printf("\n");
			}
		}
    

		return 0;

}

