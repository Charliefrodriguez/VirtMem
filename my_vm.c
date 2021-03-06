#include "my_vm.h"
#include<math.h>
#include<string.h>
char* physMem = NULL;
char * physBM; // null terminator has all bits set to zero
char * virtBM;
pde_t* pageDir; // ptr to front of pgdir
struct tlb TLB[TLB_ENTRIES];
pthread_mutex_t lock; 
int TLB_misess = 0; 
int TLB_calls = 0;
int TLB_hits = 0;
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
	
//	pthread_mutex_lock(&lock);
	for(i = 0; i < p; ++i) {
		physBM[i] = '\0';
	}
	for(i = 0; i < v; ++i) {
		virtBM[i] = '\0';
	} 
//intialize the TLB values in the array
	for(int i = 0; i < TLB_ENTRIES; ++i) {
				TLB[i].empty = 0;
				TLB[i].dirIndex = 0;
				TLB[i].page_frame_pos=0;
				TLB[i].tableIndex = 0;
				TLB[i].pfn = 0;
	}



//	pthread_mutex_unlock(&lock);	
	pageDir = (pde_t*)(physMem);
}


/*
 * Part 2: Add a virtual to physical page translation to the TLB.
 * Feel free to extend the function arguments or return type.
 */
int add_TLB(void *va, void *pa)
{
 unsigned int pfn = ((unsigned int)pa) >> (unsigned int)log2(PGSIZE);
	
 

	for(int i = 0; i < TLB_ENTRIES; ++i) {
		if(TLB[i].empty == 0) {
				TLB[i].empty = 1;
				TLB[i].dirIndex = get_top_10_bits(((unsigned int)va));
				TLB[i].tableIndex = get_mid_10_bits((unsigned int)va);
				TLB[i].page_frame_pos = get_last_12_bits((unsigned int)va);
				TLB[i].pfn = pfn;
				return 1;
		}
	}

	// if all entires are full, selec a random entry to boot from tlb and insert new value 
	
	int kick_out =  rand() % 512; 
	
				TLB[kick_out].dirIndex = get_top_10_bits((unsigned int)va);
				TLB[kick_out].tableIndex = get_mid_10_bits((unsigned int)va);
				TLB[kick_out].page_frame_pos = get_last_12_bits((unsigned int)va);
				TLB[kick_out].pfn = pfn;


	//didnt work somehow
	return -1;


/*Part 2 HINT: Add a virtual to physical page translation to the TLB */



	
}
 

 	


/*
 * Part 2: Check TLB for a valid translation.
 * Returns the physical page address.
 * Feel free to extend this function and change the return type.
 */
pte_t *
check_TLB(void *va) {
	

	/* Part 2: TLB lookup code here */
	TLB_calls++;


unsigned int addr = (unsigned int )va;
	unsigned int top = get_top_10_bits(addr);
	unsigned int mid = get_mid_10_bits(addr);
	unsigned int bot = get_last_12_bits(addr);

	for(int i = 0; i < TLB_ENTRIES; ++i) { 
		if(TLB[i].dirIndex == top && TLB[i].tableIndex == mid && TLB[i].page_frame_pos == bot ) {
					TLB_hits++;
					return (pte_t *)TLB[i].pfn; // return pfn
		}
	} 
	
	TLB_misess++; // increment TLB misses

	pte_t * phys_addr = translate((pde_t *)physMem, va); // get the physical addr
	add_TLB(va, (void *)phys_addr); // add new entry to TLB
	
	
	return NULL;


}


/*
 * Part 2: Print TLB miss rate.
 * Feel free to extend the function arguments or return type.
 */
	void
print_TLB_missrate()
{
	float miss_rate = 0;	

	/*Part 2 Code here to calculate and print the TLB miss rate*/

	miss_rate = ((float)TLB_misess)/((float)TLB_calls);

		fprintf(stderr, "TLB hits  %d, TLB misses %d TLB calls total %d \n", TLB_hits, TLB_misess,TLB_calls  );

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
	pthread_mutex_lock(&lock);
	unsigned int addr = (unsigned int)va;
	unsigned long top = get_top_10_bits(addr);
	unsigned long mid = get_mid_10_bits(addr);
	unsigned long bot = get_last_12_bits(addr);
	

	pde_t * dir_ptr = pgdir; 

	dir_ptr += top; // takes to proper page directory entry 

	pte_t * pg_ptr = (pte_t *)(*dir_ptr); // derref page dir entry, that should be a pte_t ptr 

	pg_ptr += mid; 


	unsigned long pfn = (unsigned long)(*pg_ptr); 

	pte_t * output = (pte_t *)((pfn  << (unsigned int)log2(PGSIZE) | bot));
	 pthread_mutex_unlock(&lock);
 	
	return output;
//	return (pte_t *)((pfn  << 12) | bot);

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
	
	if(physMem == NULL ){ // checks if physMem is initalized or not 
		pthread_mutex_lock(&lock);

			set_physical_mem();
			

			unsigned int i  = 1; 
	
			while(i<800){ // find first availible physical frame
						if( (i+1) % 8 != 0 && get_bit_at_index(physBM , i) == 0 ){
									set_bit_at_index(physBM, i);
									break; 
						}
						i++; 
		
			}
			
				unsigned int pfn = i; //the page that we will link the addr too 
				// assuming page dir starts at front of phy_mem 

	
				char * ptr = physMem; 
				ptr = physMem + MEMSIZE-1 - PGSIZE; // decrement backwards to create frist physical frame 
				
				unsigned long offset = 0; 

				unsigned long j = 1;  
				
				unsigned long top_bound = pow(2,32-log2(PGSIZE));
				for(j;j<top_bound;j++){
				if(get_bit_at_index(virtBM, j) == 0) { 
								set_bit_at_index(virtBM, j);
							break;
						}
					} 
					
					
					/* 
					decompose number val into indicies i,j , where i<= total # pde entries, and j <= total #pte entries per page
					Thus, the total number of different values would be i*j total pde entires * total pte entires = # virt addresses total
					*/
					unsigned pte_per_page = PGSIZE/4; // page table entries per page
					unsigned int top = floor(j/pte_per_page); 
					unsigned int mid =  j % pte_per_page; 
					
					unsigned int bot = (unsigned int)log2(PGSIZE); 
					pde_t * dir_ptr = (pde_t *)physMem; // we point dir_ptr to start of page dir 
					unsigned int top_bits = floor((32-bot)/2);
					pte_t * page_table_i = (pte_t *)(dir_ptr + (long unsigned int )pow(2,top) +1); // we then jump 2^(top bits)+1 entries to the first spot in physical mem after page dir
					
					*(page_table_i+mid) = (pte_t)(pfn); // we then iterate into this space which is the first page table, and store the pfn here
				
					*(dir_ptr+top) = (pde_t)page_table_i;			  // we then store a ptr to the strat of the page table piece containing the pf number
				
					unsigned int mid_bit = floor((32-bot)/2) + (32-bot)%2 ; 
	
					void * output =   (void*)( (((top << mid_bit) | mid) << bot) | offset );

					pthread_mutex_unlock(&lock);
					return output;
			//	return (void *)((((top << 10) | mid) << 12) | offset); // we then return the virt addr
	} 
	else{ 
		
				pthread_mutex_lock(&lock);

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
				ptr = physMem + MEMSIZE-1 - i*PGSIZE;  // decrement backwards to next open physical frame 
				
				unsigned long offset = 0; 
				// find first open slot in first availible page table
			
				unsigned long j = 1;  
				
				unsigned long top_bound = pow(2,32-log2(PGSIZE));
				for(j;j<top_bound;j++){
				if(get_bit_at_index(virtBM, j) == 0) { 
								set_bit_at_index(virtBM, j);
							break;
						}
					} 
								// decompose number into array matrix i,j values
					unsigned pte_per_page = PGSIZE/4; // page table entries per page
					unsigned int top = floor(j/pte_per_page); 
					unsigned int mid =  j % pte_per_page; 
					
					pde_t * dir_ptr = (pde_t *)physMem; 
					
					pte_t * page_table_i = (pte_t *)(dir_ptr + (long unsigned int ) pow(2,top) +1);// increments to where first page table starts

/* 
top represents the rows we need to jump, where each row represents some porition of the page table 
governed by a page directory entry, we jump in blocks of 2^12 because that is how many enties we can fit per page

*/
					page_table_i += PGSIZE*top; 
					
					*(page_table_i+mid) = (pte_t)(pfn); // store pfn value in the page table entry 
				
					*(dir_ptr+top) = (pde_t)page_table_i;			  // store pointer to page_table_i in dir ptr
						unsigned int bot = (unsigned int)log2(PGSIZE); 
					unsigned int mid_bit = floor((32-bot)/2) + (32-bot)%2 ; 
	
					void * output =   (void*)( (((top << mid_bit) | mid) << bot) | offset );

					//void * output =   (void*)( (((top << 10) | mid) << 12) | offset );
					pthread_mutex_unlock(&lock);

				return output;
				//return (void*)( (((top << 10) | mid) << 12) | offset );





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

	unsigned long phys_addr = (unsigned long)(translate( (pde_t *) physMem , va)); 
		
	pthread_mutex_lock(&lock);
	
	unsigned long addr = (unsigned long)va;
	unsigned long top = get_top_10_bits(addr);
	unsigned long mid = get_mid_10_bits(addr);
	unsigned long bot = get_last_12_bits(addr);



	unsigned long pfn =  phys_addr >> (unsigned int)log2(PGSIZE); 



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

	
				unsigned long val = top*PGSIZE + mid; // invert i,j to get corresponding value in bitmap

				int j = 1;  
				// find first open slot in first availible page table
				for(j;j<524288;j++){	
						if(j == val ) { 
								set_free_bit_at_index(virtBM, j);
							break;
						}
					} 
				
	
	pthread_mutex_unlock(&lock);




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

	unsigned int addr = (unsigned int)va;
	unsigned long top = get_top_10_bits(addr);
	unsigned long mid = get_mid_10_bits(addr);
	unsigned long bot = get_last_12_bits(addr);

unsigned long phys_addr = (unsigned long)(translate( (pde_t *) physMem , va)); 
		pthread_mutex_lock(&lock);

	unsigned long pfn =  phys_addr >> (unsigned int)log2(PGSIZE); 


				char * ptr = physMem; 
				ptr = physMem + MEMSIZE-1 - pfn * PGSIZE +bot; // decrement backwards in crements of pgsize * pfn 
				
				char * input = (char *)val;
				if(size < PGSIZE){ 
				 
				 	int i =0; 
			 
					memcpy((void *)ptr , val,size); // need to use this function here, and we are allowed to!
				}
pthread_mutex_unlock(&lock);

}


/*Given a virtual address, this function copies the contents of the page to val*/
void get_value(void *va, void *val, int size) {

	/* HINT: put the values pointed to by "va" inside the physical memory at given
	 * "val" address. Assume you can access "val" directly by derefencing them.
	 */  
		unsigned int addr = (unsigned int)va;
	unsigned long top = get_top_10_bits(addr);
	unsigned long mid = get_mid_10_bits(addr);
	unsigned long bot = get_last_12_bits(addr);


	unsigned long pfn;	
	pte_t * output = check_TLB(va);
	
	if(output != NULL){ 
		pfn = (unsigned long)output;		
	} 
	else{
	unsigned long phys_addr = (unsigned long)(translate( (pde_t *) physMem , va)); 
//	pthread_mutex_lock(&lock);
	pfn =  phys_addr >> (unsigned int)log2(PGSIZE); 
	}

				char * ptr = physMem; 
				ptr = physMem + MEMSIZE-1 - pfn * PGSIZE + bot; // decrement backwards in crements of pgsize * pfn 
				
				char * input = (char *)val;
				if(size < PGSIZE){ 
				 
				 	int i =0; 
			 
					memcpy(val,(void *)ptr , size); // need to use this function here, and we are allowed to!
				}
		
	//				pthread_mutex_unlock(&lock);
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
//	int ans_indx = 0;
	int matrix_1 = 0; 
	int matrix_2 = 0;
	//int i,j,k; 
		//int x = 1;
    int y, z;
    int i =0, j=0,k=0;
    int address_a = 0, address_b = 0;
    int address_c = 0;
    //printf("Fetching matrix elements stored in the arrays\n");

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            int x  = 0;
						address_c = (unsigned int)answer + ((i * size * sizeof(int))) + (j * sizeof(int)); 
											
						for(k=0;k<size;k++){
						address_a = (unsigned int)mat1 + ((i * size * sizeof(int))) + (k * sizeof(int));
            address_b = (unsigned int)mat2 + ((k * size * sizeof(int))) + (j * sizeof(int));

						get_value((void *)address_a, &y, sizeof(int));
            get_value( (void *)address_b, &z, sizeof(int));
						
						pthread_mutex_lock(&lock);            
						x+= y*z;
						pthread_mutex_unlock(&lock);

//				printf("%d ", y*z); 
						} 
						put_value((void *)address_c,&x,sizeof(int)); 

					        }
  //      printf("\n");
    } 




}

static unsigned int get_top_10_bits(unsigned int value){   
    unsigned int bot = (unsigned int)log2(PGSIZE); 
		unsigned int back = floor((32-bot)/2) + (32-bot)%2 + bot; 
		return (value >> back);
		//return (value >> 22);
}

static unsigned int get_mid_10_bits(unsigned int value){
   unsigned int mid_bits_value = 0;   
   //value =    value >> 12; 
   unsigned int bot = (unsigned int)log2(PGSIZE); 
	 unsigned int mid = floor((32-bot)/2) + (32-bot)%2 ; 
	 value = value >> (unsigned int)log2(PGSIZE);
	 unsigned int outer_bits_mask =   (1 << mid);  
   outer_bits_mask = outer_bits_mask-1;
   mid_bits_value =  value &  outer_bits_mask;
   return mid_bits_value;

}

static unsigned int get_last_12_bits(unsigned int value){
   unsigned int mid_bits_value = 0;
   value = value >> 0; 

   unsigned int outer_bits_mask =  (1 << (unsigned int)log2(PGSIZE));
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



/*
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
}*/
