

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
		
		for(i;i<31;i++){ 
			if( (i+1) % 8 != 0){
			printf("%d \n",get_bit_at_index(bitmap, i)); 
			} 
			else{ 
			printf("\n");
			}
		}
    
		int bigggg = (((653 << 10) |558)<< 12) | 2613;
		unsigned int huge  = 2741168693;
		printf("%u \n", huge);


		return 0;

}

