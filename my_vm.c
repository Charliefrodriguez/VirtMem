#include "my_vm.h"
#include<math.h>
#include<string.h>
char* physMem = NULL;
char * physBM; // null terminator has all bits set to zero
char * virtBM;
pde_t* pageDir; // ptr to front of pgdir

/*
Function responsible for allocating and setting your physical memory 
*/
void set_physical_mem() {

    //Allocate physical memory using mmap or malloc; this is the total size of
    //your memory you are simulating

    
    //HINT: Also calculate the number of physical and virtual pages and allocate
    //virtual and physical bitmaps and initialize them
	physMem = (char*)malloc(sizeof(char) * MEMSIZE);	
	int p = 32 - (log2(PGSIZE)) ;
	int v = MAX_MEMSIZE / PGSIZE;
	int i;
	
	physBM = (char *)malloc(sizeof(char)*p);
	virtBM = (char *)malloc(sizeof(char)*v);

	for(i = 0; i < p; ++i) {
		physBM[i] = '\0';
	}
	for(i = 0; i < v; ++i) {
		virtBM[i] = '\0';
	}
	pageDir = (pde_t*)(physMem);
}


/*
 * Part 2: Add a virtual to physical page translation to the TLB.
 * Feel free to extend the function arguments or return type.
 */
int add_TLB(void *va, void *pa)
{

	/*Part 2 HINT: Add a virtual to physical page translation to the TLB */

	return -1;
}


/*
 * Part 2: Check TLB for a valid translation.
 * Returns the physical page address.
 * Feel free to extend this function and change the return type.
 */
pte_t *
check_TLB(void *va) {

	/* Part 2: TLB lookup code here */

}


/*
 * Part 2: Print TLB miss rate.
 * Feel free to extend the function arguments or return type.
 */
	void
print_TLB_missrate()
{
	double miss_rate = 0;	

	/*Part 2 Code here to calculate and print the TLB miss rate*/




	fprintf(stderr, "TLB miss rate %lf \n", miss_rate);
}



/*
   The function takes a virtual address and page directories starting address and
   performs translation to return the physical address
 */
pte_t *translate(pde_t *pgdir, void *va) {
	/* Part 1 HINT: Get the Page directory index (1st level) Then get the
	 * 2nd-level-page table index using the virtual address.  Using the page
	 * directory index and page table index get the physical address.
	 *
	 * Part 2 HINT: Check the TLB before performing the translation. If
	 * translation exists, then you can return physical address from the TLB.
	 */
	unsigned long addr = (unsigned long)va;
	unsigned long top = get_top_10_bits(addr);
	unsigned long mid = get_mid_10_bits(addr);
	unsigned long bot = get_last_12_bits(addr);
	

	pde_t * dir_ptr = pgdir; 

	dir_ptr += top; // takes to proper page directory entry 

	pte_t * pg_ptr = (pte_t *)(*dir_ptr); // derref page dir entry, that should be a pte_t ptr 

	pg_ptr += mid; 


	unsigned long pfn = (unsigned long)(*pg_ptr); 

	return (pte_t *)((pfn  << 12) | bot);

	//If translation not successfull
	///return NULL; 
}


/*
   The function takes a page directory address, virtual address, physical address
   as an argument, and sets a page table entry. This function will walk the page
   directory to see if there is an existing mapping for a virtual address. If the
   virtual address is not present, then a new entry will be added
 */
	int
page_map(pde_t *pgdir, void *va, void *pa)
{

	/*HINT: Similar to translate(), find the page directory (1st level)
	  and page table (2nd-level) indices. If no mapping exists, set the
	  virtual to physical mapping */

	return -1;
}


/*Function that gets the next available page
 */
void *get_next_avail(int num_pages) {

	//Use virtual address bitmap to find the next free page
}


/* Function responsible for allocating pages
   and used by the benchmark
 */
void *a_malloc(unsigned int num_bytes) {
	// does each malloc call get it's own entire physical page? That would seem pretty dumb but might have enough mem to pull off
	if(physMem == NULL ){ 
			set_physical_mem();
			

			unsigned int i  = 1; 

			while(i<800){ // find open physical page
						if( (i+1) % 8 != 0 && get_bit_at_index(physBM , i) == 0 ){
									set_bit_at_index(physBM, i); // set physical page bitmap  
									break; 
						}
						i++; 
					// initialize physical page 					
		
			}
				
				unsigned int pfn = i; //the page that we will link the addr too 
				// assuming page dir starts at front of phy_mem 

	
				char * ptr = physMem; 
				ptr = physMem + MEMSIZE-1 - 4096; // decrement backwards to create frist physical frame 
				
				unsigned long offset = 0; 
				int j = 1;  
				// find first open slot in first availible page table
				for(j;j<524288;j++){	
						if(get_bit_at_index(virtBM, j) == 0) { 
								set_bit_at_index(virtBM, j);
							break;
						}
					} 
					// decompose number into array matrix i,j values
					unsigned int top = floor(j/4096); 
					unsigned int mid =  j % 4096; 
				
					pde_t * dir_ptr = (pde_t *)physMem; 
					
					pte_t * page_table_i = (pte_t *)(dir_ptr + 1025);
					
					*(page_table_i+mid) = (pte_t)(pfn);
				
					*(dir_ptr+top) = (pde_t)page_table_i;			  // where does new page table begin
	

				return (void *)((((top << 10) | mid) << 12) | offset);
	} 
	else{ 
		

			unsigned int i  = 1; 

			while(i<800){ // find open physical page
						if( (i+1) % 8 != 0 && get_bit_at_index(physBM , i) == 0 ){
									set_bit_at_index(physBM, i); // set physical page bitmap  
									break; 
						}
						i++; 
					// initialize physical page 					
		
			}
				
				unsigned int pfn = i; //the page that we will link the addr too 

				char * ptr = physMem; 
				ptr = physMem + MEMSIZE-1 - i*4096;  // decrement backwards to next open physical frame 
				
				unsigned long offset = 0; 
				int j = 1;  
				// find first open slot in first availible page table
				for(j;j<524288;j++){	
						if(get_bit_at_index(virtBM, j) == 0) { 
								set_bit_at_index(virtBM, j);
							break;
						}
					} 
					// decompose number into array matrix i,j values
					unsigned int top = floor(j/4096); 
					unsigned int mid =  j % 4096; 
					
					pde_t * dir_ptr = (pde_t *)physMem; 
					
					pte_t * page_table_i = (pte_t *)(dir_ptr + 1025);// increments to where first page table starts

/* 
top represents the rows we need to jump, where each row represents some porition of the page table 
governed by a page directory entry, we jump in blocks of 2^12 because that is how many enties we can fit per page

*/
					page_table_i += 4096*top; 
					
					*(page_table_i+mid) = (pte_t)(pfn); // store pfn value in the page table entry 
				
					*(dir_ptr+top) = (pde_t)page_table_i;			  // store pointer to page_table_i in dir ptr
	


				return (void*)( (((top << 10) | mid) << 12) | offset );





	}
	
	
	/* 
	 * HINT: If the physical memory is not yet initialized, then allocate and initialize.
	 */

	/* 
	 * HINT: If the page directory is not initialized, then initialize the
	 * page directory. Next, using get_next_avail(), check if there are free pages. If
	 * free pages are available, set the bitmaps and map a new page. Note, you will 
	 * have to mark which physical pages are used. 
	 */

//	return NULL;
}

/* Responsible for releasing one or more memory pages using virtual address (va)
 */
void a_free(void *va, int size) {
	
	
	unsigned long addr = (unsigned long)va;
	unsigned long top = get_top_10_bits(addr);
	unsigned long mid = get_mid_10_bits(addr);
	unsigned long bot = get_last_12_bits(addr);

	unsigned long phys_addr = (unsigned long)(translate( (pde_t *) physMem , va)); 
	
	unsigned long pfn =  phys_addr >> 12; 



// just loop through bitmaps and make sure the assocaited values at the indicies are set back to 0
			 int i  = 1; 

			while(i<800){ // find open physical page
						if(i == pfn ){
									set_free_bit_at_index(physBM, i); // set physical page bitmap  
									break; 
						}
						i++; 
					// initialize physical page 					
		
			}
				
	
				unsigned long val = top*4096 + mid; // invert i,j to get corresponding value in bitmap
				
				int j = 1;  
				// find first open slot in first availible page table
				for(j;j<524288;j++){	
						if(j == val ) { 
								set_free_bit_at_index(virtBM, j);
							break;
						}
					} 
				
	




	/* Part 1: Free the page table entries starting from this virtual address
	 * (va). Also mark the pages free in the bitmap. Perform free only if the 
	 * memory from "va" to va+size is valid.
	 *
	 * Part 2: Also, remove the translation from the TLB
	 */


}


/* The function copies data pointed by "val" to physical
 * memory pages using virtual address (va)
 */
void put_value(void *va, void *val, int size) {

	/* HINT: Using the virtual address and translate(), find the physical page. Copy
	 * the contents of "val" to a physical page. NOTE: The "size" value can be larger 
	 * than one page. Therefore, you may have to find multiple pages using translate()
	 * function.
	 */

	unsigned long phys_addr = (unsigned long)(translate( (pde_t *) physMem , va)); 
	
	unsigned long pfn =  phys_addr >> 12; 


				char * ptr = physMem; 
				ptr = physMem + MEMSIZE-1 - pfn * 4096; // decrement backwards in crements of pgsize * pfn 
				
				char * input = (char *)val;
				if(size < PGSIZE){ 
				 
				 	int i =0; 
			 
					memcpy((void *)ptr , val,size); // need to use this function here, and we are allowed to!
				}


}


/*Given a virtual address, this function copies the contents of the page to val*/
void get_value(void *va, void *val, int size) {

	/* HINT: put the values pointed to by "va" inside the physical memory at given
	 * "val" address. Assume you can access "val" directly by derefencing them.
	 */ 
	unsigned long phys_addr = (unsigned long)(translate( (pde_t *) physMem , va)); 
	
	unsigned long pfn =  phys_addr >> 12; 


				char * ptr = physMem; 
				ptr = physMem + MEMSIZE-1 - pfn * 4096; // decrement backwards in crements of pgsize * pfn 
				
				char * input = (char *)val;
				if(size < PGSIZE){ 
				 
				 	int i =0; 
			 
					memcpy(val,(void *)ptr , size); // need to use this function here, and we are allowed to!
				}
}



/*
   This function receives two matrices mat1 and mat2 as an argument with size
   argument representing the number of rows and columns. After performing matrix
   multiplication, copy the result to answer.
 */
void mat_mult(void *mat1, void *mat2, int size, void *answer) {

	/* Hint: You will index as [i * size + j] where  "i, j" are the indices of the
	 * matrix accessed. Similar to the code in test.c, you will use get_value() to
	 * load each element and perform multiplication. Take a look at test.c! In addition to 
	 * getting the values from two matrices, you will perform multiplication and 
	 * store the result to the "answer array"
	 */


}

static unsigned int get_top_10_bits(unsigned int value){   
    return (value >> 22);
}

static unsigned int get_mid_10_bits(unsigned int value){
   unsigned int mid_bits_value = 0;   
   value =    value >> 12; 
   unsigned int outer_bits_mask =   (1 << 10);  
   outer_bits_mask = outer_bits_mask-1;
   mid_bits_value =  value &  outer_bits_mask;
   return mid_bits_value;

}

static unsigned int get_last_12_bits(unsigned int value){
   unsigned int mid_bits_value = 0;
   value = value >> 0;
   unsigned int outer_bits_mask =  (1 << 12);
   outer_bits_mask = outer_bits_mask-1;
   mid_bits_value =  value &  outer_bits_mask;
   return mid_bits_value;

} 


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

static void set_free_bit_at_index(char *bitmap, int index)
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


//Function to get a bit at "index"
static int get_bit_at_index(char *bitmap, int index)
{
    //Same as example 3, get to the location in the character bitmap array
    char *region = ((char *) bitmap) + (index / 8);
    
    //Create a value mask that we are going to check if bit is set or not
    char bit = 1 << (index % 8);
    
    return (int)(*region >> (index % 8)) & 0x1;
}




int main() {

	int b = 27;
	int * ptr; 
	ptr = &b; 

	int a = 28;
	int * ptr2; 
	ptr2 = &a; 

	int c = 29;
	int * ptr3; 
	ptr3 = &c; 


	void * output = a_malloc(4);  
	
	put_value(output, (void *)ptr, sizeof(int *)); 
	
	int * ret =  (int *)malloc(sizeof(int)); 

	get_value(output,(void *)ret, sizeof(int *)); 
	
	printf("%d \n",*ret);
	
//	a_free(output, sizeof(int *)); 

	void * output2 =  a_malloc(4); 


	put_value(output2, (void *)ptr2, sizeof(int *)); 

	int * ret1 = (int *)malloc(sizeof(int)); // make sure that we malloc space, for what we need to store otherwise undefined behavior :( 

	get_value(output2, (void *)ret1, sizeof(int *)); 
	
	printf("%d \n",*ret1);


	void * output3  =  a_malloc(4);
	
	put_value(output3, (void *)ptr3, sizeof(int *)); 
	
		int * ret2 = (int *)malloc(sizeof(int)); 

	get_value(output3, (void *)ret2, sizeof(int *)); 
	
	printf("%d \n",*ret2); 



	return 1;
}
