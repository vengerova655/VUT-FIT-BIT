/**
 * Implementace My MALloc
 * Demonstracni priklad pro 1. ukol IPS/2020
 * Ales Smrcka
 * Vypracovala -> Veronika Vengerova (xvenge01)
 * @author xvenge01
 */


#include "mmal.h"
#include <sys/mman.h> // mmap
#include <stdbool.h> // bool
#include <assert.h> // assert
#include <string.h> // memcpy

#ifdef NDEBUG
/**
 * The structure header encapsulates data of a single memory block.
 *   ---+------+----------------------------+---
 *      |Header|DDD not_free DDDDD...free...|
 *   ---+------+-----------------+----------+---
 *             |-- Header.asize -|
 *             |-- Header.size -------------|
 */
typedef struct header Header;
struct header {

    /**
     * Pointer to the next header. Cyclic list. If there is no other block,
     * points to itself.
     */
    Header *next;

    /// size of the block
    size_t size;

    /**
     * Size of block in bytes allocated for program. asize=0 means the block 
     * is not used by a program.
     */
    size_t asize;
};

/**
 * The arena structure.
 *   /--- arena metadata
 *   |     /---- header of the first block
 *   v     v
 *   +-----+------+-----------------------------+
 *   |Arena|Header|.............................|
 *   +-----+------+-----------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
typedef struct arena Arena;
struct arena {

    /**
     * Pointer to the next arena. Single-linked list.
     */
    Arena *next;

    /// Arena size.
    size_t size;
};

#define PAGE_SIZE (128*1024)

#endif // NDEBUG

Arena *first_arena = NULL;

/**
 * Return size alligned to PAGE_SIZE
 */
static
size_t allign_page(size_t size)
{
    size = ((size + ( PAGE_SIZE-1 )) / PAGE_SIZE) * PAGE_SIZE;
    return size;
}

/**
 * Allocate a new arena using mmap.
 * @param req_size requested size in bytes. Should be alligned to PAGE_SIZE.
 * @return pointer to a new arena, if successfull. NULL if error.
 * @pre req_size > sizeof(Arena) + sizeof(Header)
 */

/**
 *   +-----+------------------------------------+
 *   |Arena|....................................|
 *   +-----+------------------------------------+
 *
 *   |--------------- Arena.size ---------------|
 */
static
Arena *arena_alloc(size_t req_size)
{
    //allignes to PAGE_SIZE
    req_size = allign_page(req_size);

    //allocates memory
    Arena *a = mmap(0,req_size,PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS,0,0);

    //failed allocation
    if (a == MAP_FAILED){
        return NULL;
    } 

    a->size = req_size;
    a->next = NULL;
    return a;
}

/**
 * Appends a new arena to the end of the arena list.
 * @param a     already allocated arena
 */
static
void arena_append(Arena *a)
{
    if ( first_arena == NULL ) {
        first_arena = a;
        first_arena->next = NULL;
    }
    else {
        Arena *ptr = first_arena;
        
        //arenas -> linked list, ends with NULL
        while(ptr->next != NULL){
            ptr = ptr->next;
        } 

        //appending to the end of arena list
        ptr->next = a;
        ptr->next->next = NULL;
    }
}

/**
 * Header structure constructor (alone, not used block).
 * @param hdr       pointer to block metadata.
 * @param size      size of free block
 * @pre size > 0
 */
/**
 *   +-----+------+------------------------+----+
 *   | ... |Header|........................| ...|
 *   +-----+------+------------------------+----+
 *
 *                |-- Header.size ---------|
 */
static
void hdr_ctor(Header *hdr, size_t size)
{
    //free header
    hdr->asize = 0;
    hdr->size = size;
    hdr->next = NULL;
}

/**
 * Checks if the given free block should be split in two separate blocks.
 * @param hdr       header of the free block
 * @param size      requested size of data
 * @return true if the block should be split
 * @pre hdr->asize == 0
 * @pre size > 0
 */
static
bool hdr_should_split(Header *hdr, size_t size)
{
    //allign to header size
    size_t req_size = ( (size + ( sizeof(Header)-1 ) )/sizeof(Header) * sizeof(Header));

    //efficient split only if size is at least 2 * sizeof(Header) bigger
    req_size += 2 * sizeof(Header);

    //sets correct size
    if(hdr->size >= req_size){
        return true;
    }
    return false;
}

/**
 * Splits one block in two.
 * @param hdr       pointer to header of the big block
 * @param req_size  requested size of data in the (left) block.
 * @return pointer to the new (right) block header.
 * @pre   (hdr->size >= req_size + 2*sizeof(Header))
 */
/**
 * Before:        |---- hdr->size ---------|
 *
 *    -----+------+------------------------+----
 *         |Header|........................|
 *    -----+------+------------------------+----
 *            \----hdr->next---------------^
 */
/**
 * After:         |- req_size -|
 *
 *    -----+------+------------+------+----+----
 *     ... |Header|............|Header|....|
 *    -----+------+------------+------+----+----
 *             \---next--------^  \--next--^
 */
static
Header *hdr_split(Header *hdr, size_t req_size)
{
    //gets pointer to the second header
    Header * hdr2 = &hdr[1 + req_size/sizeof(Header)] ;
    //creates second header
    hdr_ctor(hdr2,(hdr->size - req_size - sizeof(Header)));

    hdr2->next = hdr->next;
    hdr->next = hdr2;
    hdr->size = req_size;
    return hdr2;
}

/**
 * Detect if two adjacent blocks could be merged.
 * @param left      left block
 * @param right     right block
 * @return true if two block are free and adjacent in the same arena.
 * @pre left->next == right
 * @pre left != right
 */
static
bool hdr_can_merge(Header *left, Header *right)
{
    //both blocks are free
    if (left->asize == 0 && right->asize == 0) {
        //right is in the same arena next to the left block
        if (right == (&left[1 + left->size/sizeof(Header)])) {
            return true;
        }
    }
    return false;
}

/**
 * Merge two adjacent free blocks.
 * @param left      left block
 * @param right     right block
 * @pre left->next == right
 * @pre left != right
 */
static
void hdr_merge(Header *left, Header *right)
{
    //next items in list will not be lost
    left->next = right->next;
    
    //size = previous size + size of right block + size of Header metadata
    left->size = left->size + right->size + sizeof(Header);
}

/**
 * Finds the first free block that fits to the requested size.
 * @param size      requested size
 * @return pointer to the header of the block or NULL if no block is available.
 * @pre size > 0
 */
static
Header *first_fit(size_t size)
{

    Header* ptr = (Header *) first_arena; 
    
    //arena allocated
    if(ptr != NULL) {
        //pointer to first header(metadata)
        ptr = (Header *)(&first_arena[1]);
        
        do
        {
            //size is free and fits to the space allocated to header
            if ( ptr->asize == 0 && ptr->size >= size ){
                return ptr;
            }
            else{
                //move to the next header
                ptr = ptr->next;
            }
        //circular linked list -> last points to first header
        } while (ptr != (Header *)(&first_arena[1]));
    }
    return NULL;
}

/**
 * Search the header which is the predecessor to the hdr. Note that if 
 * @param hdr       successor of the search header
 * @return pointer to predecessor, hdr if there is just one header.
 * @pre first_arena != NULL
 * @post predecessor->next == hdr
 */
static
Header *hdr_get_prev(Header *hdr)
{
    //first header in arena
    Header* ptr = (Header *)(&first_arena[1]);

    //first header returns itself
    while (ptr->next != hdr)
    {
        ptr = ptr->next;
    }
    
    return ptr;
}

/**
 * Allocate memory. Use first-fit search of available block.
 * @param size      requested size for program
 * @return pointer to allocated data or NULL if error or size = 0.
 */
void *mmalloc(size_t size)
{
    //no need to alloc size = 0
    if( size == 0 ){
        return NULL;
    }

    //available block
    Header *hdrptr = first_fit(size);

    //did not find available block
    if(hdrptr == NULL){
        //size for program(already checked not equal to zero), plus we need space for metadata
        Arena* a = arena_alloc( size + sizeof(Arena) + sizeof(Header) );

        //error while allocating
        if (a == NULL) {
            return NULL;
        }
        //appends arena to the list of arenas
        arena_append(a);

        //pointer to first header in arena we need to create
        hdrptr = ( Header * )( &a[1] );

        //data block is smaller ( - Header and Arena metadata)
        size_t sizeal = a->size - sizeof(Arena) - sizeof(Header);
        if(sizeal > 0){
            hdr_ctor(hdrptr,sizeal);
        }

        //Headers -> circular linked list -> points to first header
        hdrptr->next = (Header *)(&first_arena[1]);
        
        //returns last header in circular linked list -> to which we want to append new header
        if (first_arena != NULL){
            Header *tmp = hdr_get_prev((Header *)(&first_arena[1]));
            tmp->next = hdrptr;
        }
    }
    
    //error while allocating
    if(first_arena == NULL){
        return NULL;
    }
    
    //should split only if free
    if( hdrptr->asize == 0 && size > 0 ){
        if(hdr_should_split(hdrptr,size)) {
            size_t req_size = (((size + ( sizeof(Header)-1 )) / sizeof(Header)) * sizeof(Header));
            
            //efficient only if bigger than requested size
            if(hdrptr->size >= req_size + 2*sizeof(Header)){
                hdr_split(hdrptr,req_size);
            }
        }
    }
    hdrptr->asize = size;
    return &hdrptr[1];
}

/**
 * Free memory block.
 * @param ptr       pointer to previously allocated data
 * @pre ptr != NULL
 */
void mfree(void *ptr)
{
    //pointer to header metadata of the data
    Header *hdptr = (&((Header *)ptr)[-1]);
    
    //freeing whole space
    hdptr->asize = 0;

    //pointer to first header in arena (already alllocated since ptr!=NULL)
    Header *tmp = (Header *)&first_arena[1];

    do
    {
        //cannot merge one header
        if(tmp != tmp->next){
            //merge together all arenas that can be merged
            while(hdr_can_merge(tmp,tmp->next))
            {
                hdr_merge(tmp,tmp->next);
            }
        }
        //next header
        tmp = tmp->next;


    //header = circular linked list, last points to first
    }while (tmp != (Header *)&first_arena[1]);
}

/**
 * Reallocate previously allocated block.
 * @param ptr       pointer to previously allocated data
 * @param size      a new requested size. Size can be greater, equal, or less
 * then size of previously allocated block.
 * @return pointer to reallocated space or NULL if size equals to 0.
 * @post header_of(return pointer)->size == size
 */
void *mrealloc(void *ptr, size_t size)
{
    if(size != 0){
        //pointer to header metadata
        Header * hdr = (&((Header *)ptr)[-1]);
       
        //in block not enough space 
        if((hdr->size) < size){
            //allocating new space
            hdr = mmalloc(size);

            
            //copying data to newly allocated space
            memcpy(hdr,(&((Header *)ptr)[-1]),(&((Header *)ptr)[-1])->asize);
        }
        else
        {
            hdr->asize = size;
        }
        //return pointer to DATA
        return &hdr[1]; 
    }
    //return value if size == 0
    return NULL;
}
