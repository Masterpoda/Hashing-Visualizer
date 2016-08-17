/*!
 * @file
 */
 
#ifndef CONSTANTS__H__
#define CONSTANTS__H__

/**
 * @brief Array of constants for table sizes
 */

const unsigned long long tableSizes[30] = {

    13,
    53,
    97,
    193,
    389,
    769,
    1543,
    3079,
    6151,
    12289,
    24593,
    49157,
    98317,
    196613,
    393241,
    786433,
    1572869,
    3145739,
    6291469,
    12582917,
    25165843,
    50331653,
    100663319,
    201326611,
    402653189,
    805306457,
    1610612741,
    3221225533,
    6442451077,


};


#endif

#if defined __UINT32_MAX__ or UINT32_MAX
  #include <inttypes.h>
  #else
  typedef unsigned char uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned long uint32_t;
  typedef unsigned long long uint64_t;
  #endif

#ifdef __GNUC__
#define FORCE_INLINE __attribute__((always_inline)) inline
#else
#define FORCE_INLINE inline
#endif

static FORCE_INLINE uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}

static FORCE_INLINE uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

#define getblock(p, i) (p[i])

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

static FORCE_INLINE uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

//----------

static FORCE_INLINE uint64_t fmix64 ( uint64_t k )
{
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;

  return k;
}


#include <vector>
//#include "murmur3.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>


#ifndef __HASH_H
#define __HASH_H


typedef long long ll;

using namespace std;


/***************************************************************************//**
 * @author Zac Christy, Kenneth Petry
 *
 * @par Description:
 * A class used to operate and implement the Hash table data structure
 *
 * @param[in] init_size
 * @param[in] argv - a pointer array full of the character strings signifying
 * the files and function calls needed by the user
 *
 * @returns 0 program ran successful.
 * @returns 1 the program failed or there was an error.
 ******************************************************************************/

template<class K, class V>
class Hash
{
   public:
      Hash();                                    // zero parameter constructor
      Hash(size_t init_size);                    // one parameter constructor
     ~Hash();                                    // destructor

      V& operator[](const K& Key);               // overloaded []
      V& insert(const K& Key, V& Value);         // insert
      V find(const K& Key);                      // find a key
      size_t size();                             // current size
      double loadFactor();                       // load factor
      int hashCodeGenerated(int hashCode);       // count of time hashCode was generated
      int* bucketProbed();                       // frequency of a hash code
      void remove(const K& Key);                 // remove a key from hash

      void print_table();                        // Prints entire table for testing purposes
      K getKey(int index);
      int contents();

   private:
   
        string formatKey(const K& Key, size_t &len); //takes a key and formats it into char*
        void rehash(); //rehashes the key into the next appropriate table size
        int tIndex = 0;
        unsigned long long tSize;
        size_t len = 0;
        uint32_t hash[1] = {0};
        double maxLF = 0.39;
        void MurmurHash3_x86_32 (const void *key, int len, uint32_t seed, void *out);


        long long counter = 0;
        
        struct bucket
        {
            K key;
            V value;
            bool filled = false;
            bool deleted = false;
            int probed = 0;
        };
        
        vector < bucket > table;

	
};


/***************************************************************************//**
 * @author Kenneth Petry
 *
 * @par Description:
 * A Constructor to initialize the size of the hash table when no size is
 * specialized in the instantiation of the Hash Table
 *
 ******************************************************************************/

template<typename K, typename V>
Hash<K,V>::Hash()
{
    tSize = tableSizes[5];
    table.resize ( tSize );
    tIndex = 5;

}

/***************************************************************************//**
 * @author Kenneth Petry
 *
 * @par Description:
 * A Constructor to initialize the size of the hash table when a size is
 * specified by the instantiation.
 *
 * @param[in] init_size - the minimum size of the table specified by
 *                        instantiation.
 ******************************************************************************/

template<typename K, typename V>
Hash<K,V>::Hash(size_t init_size)
{
    int i = 0;
    while(tableSizes[i]< init_size && (i < 28))
        i++;

    tIndex = i;
    tSize = tableSizes[i];
    table.resize ( tableSizes[i] );

}

/***************************************************************************//**
 * @author Kenneth Petry
 *
 * @par Description:
 * A deconstructor for the Hash Table class
 *
 ******************************************************************************/

template<typename K, typename V>
Hash<K,V>::~Hash()
{

}


/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * A function describing the overloaded [] operator functionality
 *
 * @param[in] Key - They key value of the bucket being searched for by the
 *                  [] operator.
 * @param[out] value - the value of the bucket with specified key value,
 *                     passed by reference
 *
 * @returns &value of the bucket with specified key
 ******************************************************************************/

template<typename K, typename V>
V& Hash<K,V>::operator[](const K& Key)
{

    counter++;
    bucket holder;

    if(Hash<K,V>::loadFactor() >= maxLF)
    {
        rehash();
    }

    long long loc;
    const char * data = formatKey(Key, len).c_str();
    int seed = 0;

    do{
        MurmurHash3_x86_32(data, len, seed++, hash);
        loc = hash[0]%tSize;
        table[loc].probed++;
        holder = table[loc];

    }while(holder.key != Key && holder.filled && !holder.deleted);

    if(Key == holder.key && !holder.deleted)
        counter--;

    holder.key = Key;
    holder.filled = true;
    holder.deleted = false;

    table[loc] = holder;

    len = 0;
    return table[loc].value;
}


/***************************************************************************//**
 * @author Zac Christy Kenneth Petry
 *
 * @par Description:
 * A function that inserts the key/value pair into the Hash Table and
 * returns the value of that bucket by reference. The counter of filled buckets
 * is also incremented.
 *
 * @param[in] Key - a value denoting the number of arguments declared in the
 * command line
 * @param[in] Value - a pointer array full of the character strings signifying
 * the files and function calls needed by the user
 * @param[out] Value - The value of the bucket returned by reference
 *
 * @returns &value value of the bucket that was accessed by reference
 ******************************************************************************/

template<typename K, typename V>
V& Hash<K,V>::insert(const K& Key, V& Value)
{

    counter++;
    bucket holder;
    if(Hash<K,V>::loadFactor() >= maxLF)
    {
        rehash();
    }

    long long loc;
    const char * data = formatKey(Key, len).c_str();
    int seed = 0;


    do{
        MurmurHash3_x86_32(data, len, seed++, hash);
        loc = hash[0]%tSize;
        table[loc].probed++;
        holder = table[loc];

    }while(holder.key != Key && holder.filled && !holder.deleted);

    if(Key == holder.key && !holder.deleted)
        counter--;


    holder.value = Value;
    holder.key = Key;
    holder.filled = true;
    holder.deleted = false;

    table[loc] = holder;


    len = 0;
    return table[loc].value;
}

/***************************************************************************//**
 * @author Zachary Christy
 *
 * @par Description:
 * A function that searches through the Hash Table for the Bucket containing
 * the key given as its argument. It then returns the value of that bucket.
 *
 * @param[in] Key - The key value being searched for
 * @param[out] value - The value of the bucket being searched for
 *
 * @returns value the returned value of the bucket being searched for
 ******************************************************************************/

template<typename K, typename V>
V Hash<K,V>::find(const K& Key)
{
    long long loc;
    const char * data = formatKey(Key, len).c_str();
    int seed = 0;
    
    

    do{
        MurmurHash3_x86_32(data, len, seed++, hash);
        loc = hash[0]%tSize;
        table[loc].probed++;

    }while((table[loc].key != Key && table[loc].filled) || table[loc].deleted);

    len = 0;

    return table[loc].value;
 }


/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * A function that returns the size of the table.
 *
 * @param[out] tSize - The size of the table
 *
 * @returns tSize The size of the table
 ******************************************************************************/

template<typename K, typename V>
size_t Hash<K,V>::size()
{
    return tSize;
}


/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * A function that returns the load factor of the table.
 *
 * @param[in] tSize - The size of the table
 * @param[in] counter - The amount of filled buckets
 *
 * @returns counter/tSize Returns the load factor of the Table
 ******************************************************************************/
template<typename K, typename V>
double Hash<K,V>::loadFactor()
{
    return (double)counter/tSize;
}

template<typename K, typename V>
int Hash<K,V>::contents()
{
    return counter;
}


/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * Function returns how many times a hash code was generated. Since the
 * table handles collisions and searches by re-hashing, this is identical to
 * the amount of times a bucket has been probed.
 *
 * @param[in] hashCode - The hash code to be checked
 *
 * @returns probed How many times a hash code was generated
 ******************************************************************************/

template<typename K, typename V>
int Hash<K,V>::hashCodeGenerated(int hashCode)
{
    return table[hashCode%tSize].probed;
}

/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * A function that returns a pointer to an integer
 *
 * @param[out] probed - a pointer to an array of ints denoting how many
 *                      times a bucket was probed
 *
 * @returns probed A pointer to an array of ints denoting how many times a
 *                 bucket was probed.
 ******************************************************************************/

template<typename K, typename V>
int* Hash<K,V>::bucketProbed()
{
    int * probed = (int *)malloc(sizeof(int)*tSize);

    for(long long i = 0; i < tSize; i++)
    {
        probed[i] = table[i].probed;
    }

    return probed;
}

/***************************************************************************//**
 * @author Zac Christy Kenneth Petry
 *
 * @par Description:
 * A function that removes the key/value pair from the bucket with the
 * specified key, marking it as deleted so that it is considered empty when
 * inserting, and full when searching
 *
 * @param[in] Key - The key of the bucket to be marked as deleted
 *
 * @returns void
 ******************************************************************************/

template<typename K, typename V>
void Hash<K,V>::remove(const K& Key)
{
    long long loc;
    const char * data = formatKey(Key, len).c_str();
    uint32_t seed = 0;

    do{
        MurmurHash3_x86_32(data, len, seed++, hash);
        loc = hash[0]%tSize;
        table[loc].probed++;

        ///loop will continue searching when:
        /// 1.) The bucket has been filled and
        /// 2.) The keys are not equal
    }while(table[loc].key != Key && table[loc].filled );



    ///The counter will be decremented if:
    /// 1.) The element we're deleting hasnt been deleted and
    /// 2.) Something has been inserted into it at some point
    if(!table[loc].deleted && table[loc].filled)
    {
        counter --;
    }

    V empty;

    table[loc].value = empty;

    table[loc].deleted = true;

}

/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * A function that takes a key of any data type and formats it for the hash
 * function
 *
 * @param[in] Key - The key to be formatted
 * @param[in/out] len - The length of the key to be formatted
 * @param[out] sData - a pointer to a character string to be formatted
 *
 * @returns sData A pointer to a character array for the hash function
 ******************************************************************************/

template<typename K, typename V>
string Hash<K,V>::formatKey(const K& Key, size_t &len)
{
    string sData = "";
    stringstream ss;
    ss << Key;
    ss >> sData;
    len = sData.length();
    return sData;//formats the key so that a char string can be extracted
}

/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * A function that rehashes the data in the hash table into a larger hash table
 *
 * @param[in] table - The original table to be rehashed
 *
 * @returns void
 ******************************************************************************/

template<typename K, typename V>
void Hash<K,V>::rehash()
{
    tIndex ++;

    cout <<fixed<< setprecision(2)<<endl;
    cout<<"... load factor "<< loadFactor() <<" (rehashing new size = "<< tableSizes[tIndex] << ")" <<endl;

    long long loc;
    unsigned long long oldSize = tSize;


    tSize = tableSizes[tIndex];//setting table to new size

    int seed = 0;
    vector<bucket> newTable;
    newTable.resize(tSize);
    bool reassign;
    bucket holder;
	const char * data;
    //iterate through and rehash table into new table;
    for(unsigned int i = 0; i < oldSize; i++)
    {
        holder = table[i];
        reassign = (holder.probed > 0  && !holder.deleted);


        if(reassign)
        {
            data = formatKey(holder.key, len).c_str();

            do{
                    MurmurHash3_x86_32(data, len, seed++, hash);
                    loc = hash[0]%tSize;

            }while(newTable[loc].filled);

            seed = 0;
            newTable[loc] = holder;
        }

    }

    table = newTable;

}

/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * A function that prints the contents of the table for data verification
 * purposes. Not recommended for large tables.
 *
 * @returns void
 ******************************************************************************/

template<typename K, typename V>
void Hash<K,V>::print_table()
{
    cout<<endl<<"Table Contents: "<<endl;
    cout<<"----------------"<<endl<<endl;
    for(unsigned int i = 0; i < tSize; i++)
    {
        if(table[i].filled && !table[i].deleted)
        {
            cout<<i<<".) "<<"Key: "<<table[i].key<<" ";
            cout<<"Val: "<<table[i].value<<" ";
            cout<<"Probed: "<<table[i].probed<<" "<<endl;
        }
        else
        {
            cout<<i<<".)"<<"EMPTY BUCKET"<<endl;
        }
    }
    cout<<endl;
}

/***************************************************************************//**
 * @author Zac Christy
 *
 * @par Description:
 * A function that returns the value of a key at a specific index
 *
 * @param[in] index - The index, or Hash Code of the bucket to be checked
 *
 * @returns key The key of the bucket to be searched
 ******************************************************************************/

template<typename K, typename V>
K Hash<K,V>::getKey(int index)
{
    return table[index].key;
}



/***************************************************************************//**
 * @author Austin Appleby
 *
 * @par Description:
 * Uses black magic to return a hash code with good phi distribution, good
 * Avalanche, and 1000 mana. "Murmur" is a juxtaposition of "Multiply, Rotate,
 * Multiply, Rotate" Which is probably the voodoo incantation used to summon
 * whatever eldritch monster that gives out the hash code. Probably not, but
 * just to be safe, dont say it into a mirror in a dark bathroom.
 *
 * @param[in] Key - A pointer to a character array of the key to be hashed
 * @param[in] len - The length in bytes of the key
 * @param[in] seed - Seed value of the hash
 * @param[out] out - Pointer to hash code
 *
 * @returns none
 ******************************************************************************/

template<typename K, typename V>
void Hash<K, V> :: MurmurHash3_x86_32 ( const void * key, int len,
                          uint32_t seed, void * out )
{
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 4;
  int i;

  uint32_t h1 = seed;
  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

  for(i = -nblocks; i; i++)
  {
    uint32_t k1 = getblock(blocks,i);

    k1 *= c1;
    k1 = ROTL32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1,13);
    h1 = h1*5+0xe6546b64;
  }

  //----------
  // tail

  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

  uint32_t k1 = 0;
  switch(len & 3)
  {
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization

  h1 ^= len;
  h1 = fmix32(h1);
  *(uint32_t*)out = h1;

}

#endif
