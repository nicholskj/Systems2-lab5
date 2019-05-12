
class Simulate
{
   private:
      Cache * cacheP;      //pointer to the cache object
      int32_t hits;        //number of hits
      int32_t misses;      //number of misses
      int32_t evictions;   //number of evictions
      bool verbose;        //for verbose output
   public:
      Simulate(int32_t associativity, int32_t blockOffsetBits, 
               int32_t setIndexBits, bool verbose);
      void printSummary();
      void run(std::string filename);
      void callCache(unsigned long addr);
      ~Simulate();
};
