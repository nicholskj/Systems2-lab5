#include <iostream>
#include <string>
#include <assert.h>
#include "Cache.h"

/*
 * Author - Nick Smith and Kyle Nichols
 */

/*
 * Cache constructor
 * Creates the twoD array of lines and initializes the
 * associativity, numSets, blockOffsetBits, setIndexBits,
 * and verbose data members.
 */
Cache::Cache(int32_t associativity, int32_t blockOffsetBits, 
             int32_t setIndexBits, bool verbose)
{
   //You will need to write the missing code
   numSets = 1;
   numSets <<= setIndexBits;
   this->associativity = associativity;
   this->blockOffsetBits = blockOffsetBits;
   this->setIndexBits = setIndexBits;
   this->verbose = verbose;
   lines = new Line*[numSets];
   for (int i = 0; i < numSets; i++) {
        lines[i] = new Line[associativity];
   }
}

/*
 * Line constructor
 * Sets the tag to 0 and the valid bit to false (tag not valid)
 */
Cache::Line::Line()
{
   //This constructor is complete.
   tag = 0;
   valid = false;
}

/* You will need to determine what other methods you need and
 * add them. For example,
 * 
 * a method that returns true or false depending upon whether the 
 * desired data is already in the cache.
 *         
 * a method that determines whether a set is full.  
 *                  
 * a method that updates a cache set.
 *
 * ....
 *
 */

/*
 * GetBits
 * Returns bits of the disired range from number passed by
 * user
 *
 * param: start - start bit
 * param: end - end bit
 * param: num - number (must be a long) to select bits from
 * 
 *
 * return: bits from num
 *
 */
long Cache::getBits(int start, int end, unsigned long num) {
    assert(start >= 0 && start < 64 && end >= 0 && end < 64);
    num <<= start;
    num >>= end;
    return num;
}

/*
 * checkData
 *
 * Searches cache to determine if data is present
 *
 * param: addr - address
 *
 * return: true - if data is present
 * return: false - if data is absent
 */
bool Cache::checkData(unsigned long addr) {
    int tag = getBits(0, setIndexBits + blockOffsetBits, addr);
    int set = getBits(64 - setIndexBits - blockOffsetBits, 64 - setIndexBits, addr);
    for (int i = 0; i < associativity; i++) {
        if (lines[set][i].tag == tag && lines[set][i].valid)
            return true;    
    }
    return false;
}

/*
 * setFull
 *
 * Determines if set is full
 *
 * param:: addr - address
 * 
 *
 * return: true - set full
 * return: false - set not full
 */
bool Cache::setFull(unsigned long addr) {
    int set = getBits(64 - setIndexBits - blockOffsetBits, 64 - setIndexBits, addr);
    for (int i = 0; i < associativity; i++) {
        if (!lines[set][i].valid)
            return false;
    }
    return true;
}

/*
 * updateCache
 *
 * Updates cache using LRU eviction policy when a load or store is performed
 *
 * param: present - data present?
 * param: full - set full?
 * param: addr - address
 * 
 * */

void Cache::updateCache(bool present, bool full, unsigned long addr) {
    int index = 0;
    int set = getBits(64 - setIndexBits - blockOffsetBits,64 - setIndexBits, addr);
    int tag = getBits(0, setIndexBits + blockOffsetBits, addr);
    if (full)
        index = associativity - 1;
    else {
        for (int i = 0; i < associativity; i++) {
            if (present && lines[set][i].tag == tag) {
                    index = i;
                    break;
            }
            else if (!lines[set][i].valid) {
                index = i;
                break;
            }
        }
    }
    for (int i = index; i > 0; i--)
    {
        lines[set][i] = lines[set][i-1];
    }
    lines[set][0].tag = tag;
    lines[set][0].valid = 1;
}
