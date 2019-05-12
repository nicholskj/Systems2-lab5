#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
#include "Cache.h"
#include "Simulate.h"

/*
 * Authors Kyle Nichols and Nick Smith
 */

/* Simulate
 * Initializes the hits, misses, evictions, and verbose
 * data members.
 * Create the cache object that will be used in the
 * simulation.
 */
Simulate::Simulate(int32_t associativity, int32_t blockOffsetBits, 
                   int32_t setIndexBits, bool verbose)
{
   //This method is complete
   hits = misses = evictions = 0;
   cacheP = new Cache(associativity, blockOffsetBits, setIndexBits, verbose);
   this->verbose = verbose;
}

/*
 * Destructor
 */
Simulate::~Simulate() {
    free(cacheP);
}

/*
 * run
 * Opens the file of addresses and uses each address of a data item
 * to perform a cache access.  The addresses are formatted as follows:
I 0400d7d4,8
 M 0421c7f0,4
 L 04f6b868,8
 S 7ff0005c8,8

Each line denotes one or two memory accesses. The format of each line is

[space]operation address,size

The operation field denotes the type of memory access: 
I denotes an instruction load, L a data load, S a data store, and M a 
data modify (i.e., a data load  followed by a data store, thus two 
sequential accesses to cache). There is never a space before an I. 
There is always a space before each M, L, and S. The address field 
specifies a 64-bit hexadecimal memory address. The size field specifies 
the number of bytes accessed by the operation.  The size
field and instruction (I) addresses can be ignored.

This method will use an address to access the cache
and update the hits, misses, and evictions data members.
*/
void Simulate::run(std::string filename)
{
   //You will need to write this.
   //It will call methods in the Cache class that you
   //will also have to write.

   //Your cache class will need a method that returns true or false
   //depending upon whether the desired data is already in the cache.
   //This can be used to increment hits or misses

   //In addition, your Cache class will need a method that determines
   //whether a set is full.  If a miss occurs and the set is full,
   //then evictions incremented.

   //Your Cache class will also need a method that updates a cache set.

   //You should divide the work of this into two methods.  The run
   //method can read and parse the address.  A second method can take the
   //address and access the cache with it (updating hits or misses and
   //possibly evictions).
  std::string line;
  std::ifstream file(filename);
  std::string addrLine;
  std::string sizeLine;
  int endAddr;
  int startAddr = 3;
  while (std::getline(file, line))
  {
    if(line[0] == ' ')
    {
        endAddr = line.find(',');
        addrLine = line.substr(startAddr,endAddr);
        unsigned long addr = std::stoul(addrLine,0,16);
        switch(line[1])
        {
            case 'M':
                if(verbose)
                    printf("%s ",line.c_str());
                callCache(addr);
                callCache(addr);
                if(verbose)
                    printf("\n");
                break;
            case 'L':
                if(verbose)
                    printf("%s ",line.c_str());
                callCache(addr);
                if(verbose)
                    printf("\n");
                break;
            case 'S':
                if(verbose)
                    printf("%s ",line.c_str());
                callCache(addr);
                if(verbose)
                    printf("\n");
                break;
            default:
                break;
       }

    }
  }
}

/*
 * callCache
 *
 * calls code from Cache.C and updates hits and misses
 *
 * param - addr (address of the line to be stored
 *
 */

void Simulate::callCache(unsigned long addr)
{
    bool isPres = cacheP->Cache::checkData(addr);
    bool isFull = cacheP->Cache::setFull(addr);
    if(!isPres){
        if(verbose)
            printf("miss ");
        misses++;
    }
    else{ 
        if(verbose)
            printf("hit ");
        hits++;
    }
    if(isFull && !isPres){
        if(verbose)
            printf("eviction ");
        evictions++;
    }

    cacheP->Cache::updateCache(isPres, isFull, addr);

}


/*
 * printSummary
 * Prints the number of hits, misses, and evictions both
 * to stdout and to a file to support automatic testing
 */
void Simulate::printSummary()
{
   //This method is complete
   std::fstream fs;
   printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
   fs.open(".csim_results", std::fstream::out);
   assert(fs.is_open());
   fs << hits << " " << misses << " " << evictions <<"\n";;
   fs.close();
}

