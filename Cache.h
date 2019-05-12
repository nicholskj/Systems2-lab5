

class Cache
{
   class Line         
   {
      public:
         int64_t tag;  //address tag
         bool valid;   //is tag valid
         Line();
   };
   private:
      Line ** lines;           //cache is a 2D array of lines
      int32_t numSets;         //number of cache sets (number of rows)
      int32_t associativity;   //set associativity (number of columns)
      int32_t blockOffsetBits; //number of bits of the address for block offset
      int32_t setIndexBits;    //number of bits of the address for set index
      bool verbose;            //verbose mode of execution
   public:
      Cache(int32_t associativity, int32_t blockOffsetBits, 
            int32_t setIndexBits, bool verbose);
      long getBits(int start, int end, unsigned long num);
      bool checkData(unsigned long addr);
      bool setFull(unsigned long addr);
      void updateCache(bool present, bool full, unsigned long addr);
};
