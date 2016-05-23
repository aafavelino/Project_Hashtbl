/*!
 *  @file hashtbl.inl
 *  @brief Implementacao da hashtable
 *
 *  Implementacao da hashtable
 */

#include <iostream>
#include <functional>
#include <exception>
#include <forward_list>
#include <list>
#include <cmath>
#include "hashtbl.h"
using namespace std;

//Verificar primo
bool eh_primo(int x)
{
    if (x != 1)
        return false;
    for (int i = 2; i < x; i++)
        if(x % i == 0)
            return false;
    return true;
}
// Encontrar o proximo primo
int proximo(int x)
{
    while (eh_primo(x) == false)
    {
        return x;
    }
}


//! MyHashTable namespace encapsulates all class related to a simple hash function definition.
namespace MyHashTable {

    //----------------------------------------------------------------------------------------
    //! Default construtor.
    /*! Creates a hash table of the required capacity, which uses an external hash function
     *  that maps keys to unsigned long integers.
     *  If no external hash function is provided, an \r UndefinedHashFunctionException is generated.
     *  \param _initSize Required hash table capacity.
     *  \param _pfHF Pointer to an external hash function that does the first hashing and returns an unsigned long int.
     *  \throw UndefinedHashFunctionException if no external hash function is provided.
    */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    HashTbl< KeyType, DataType, KeyHash, KeyEqual >::HashTbl ( int _initSize )
        : mSize( _initSize ), mCount( 0u )
    {
        std::unique_ptr<std::forward_list<Entry>[]> slave(new std::forward_list<Entry>[mSize]);
        mpDataTable = std::move(slave);

    }

    //----------------------------------------------------------------------------------------
    //! Destrutor that just frees the table memory, clearing all collision lists.
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    HashTbl< KeyType, DataType, KeyHash, KeyEqual >::~HashTbl ()
    {
 
        for (int i = 0; i < mSize; ++i)
        {
            mpDataTable[i].~forward_list();
        }
        mSize = 0;
    }

    //----------------------------------------------------------------------------------------
    //! Inserts data into the hash table.
    /*! For an insertion to occur, the client code should provide a key and the data itself
     *  If the data is already stored in the table, the function updates the data with the
     *  new information provided.
     *  \param _newKey Key associated with the data, used to get to the stored information.
     *  \param _newDataItem Data to be stored or updated, in case the information is already stored in the hash table.
     *  \return true if the data is already stored in the table and it is updated; false, otherwise.
     *  \throw std::bad_alloc In case no memory is available for dynamic allocation required in the insertion procedure.
    */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    bool HashTbl< KeyType, DataType, KeyHash, KeyEqual >::insert ( const KeyType & _key, const DataType & _DataItem ) throw ( std::bad_alloc )
    {

            KeyHash hashFunc;
            KeyEqual equalFunc;
            Entry newEntry ( _key, _DataItem ); 
            if(mCount + 1 > mSize)
                rehash();

            
            auto end = hashFunc( _key ) % mSize;
            auto i = mpDataTable[end].begin();
            auto it = mpDataTable[end].before_begin();
            for (; i != mpDataTable[end].end(); i++)
            {
                if(true == equalFunc( (*i).mKey, _key)){
                    (*i).mData = _DataItem;
                    return false;
                }
                it++;
            }

            mCount++;
            mpDataTable[end].emplace_after(it,_key,_DataItem);
            return true;
        // Flag used to indicate whether the data has been found or not.
        //auto bDataFound( false );
        //return bDataFound;
    }


    //----------------------------------------------------------------------------------------
    //! Removes data from the hash table.
    /*! Removse a data item from the table, based on the key associated with the data.
     *  If the data cannot be found, false is returned; otherwise, true is returned instead.
     *  \param _searchKey Data key to search for in the table.
     *  \return true if the data item is found; false, otherwise.
    */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    bool HashTbl< KeyType, DataType, KeyHash, KeyEqual >::remove ( const KeyType & _searchKey )
    {
            KeyHash hashFunc;
            KeyEqual equalFunc;

            // Apply double hashing method, one functor and the other with modulo function. 
            auto end = hashFunc( _searchKey ) % mSize;
            auto i = mpDataTable[end].begin();
            auto it = mpDataTable[end].before_begin();
            for (; i != mpDataTable[end].end(); i++)
            {
                if(true == equalFunc( (*i).mKey, _searchKey)){
                it++;
                   mpDataTable[end].erase_after(it, i);
                   mCount--;
                   return true;
                }
                i++;
            }
           
            return false;
    }

    //----------------------------------------------------------------------------------------
    //! Retrieves data from the table.
    /*! Retrieves a data item from the table, based on the key associated with the data.
     *  If the data cannot be found, false is returned; otherwise, true is returned instead.
     *  \param _searchKey Data key to search for in the table.
     *  \param _dataItem Data record to be filled in when data item is found.
     *  \return true if the data item is found; false, otherwise.
    */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    bool HashTbl< KeyType, DataType, KeyHash, KeyEqual >::retrieve ( const KeyType & _searchKey, DataType & _dataItem ) const
    {
            KeyHash hashFunc;
            KeyEqual equalFunc;

            // Apply double hashing method, one functor and the other with modulo function. 
            auto end = (hashFunc( _searchKey ) % mSize);
            auto i = mpDataTable[end].begin();
            for (auto i = mpDataTable[end].begin(); i != mpDataTable[end].end(); i++)
            {
                if(true == equalFunc( (*i).mKey, _searchKey)){
                   _dataItem = (*i).mData;
                   return true;
                }
                i++;
            }
           
            return false;      
    }

    //! Clears the data table.
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    void HashTbl< KeyType, DataType, KeyHash, KeyEqual >::clear ()
    {
            for (int i = mCount; i > 0; i--)
            {
                (&mpDataTable[i])->~forward_list();
            }
    }

    //! Tests whether the table is empty.
    /*!
     * \return true is table is empty, false otherwise.
     */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    bool HashTbl< KeyType, DataType, KeyHash, KeyEqual >::isEmpty () const
    {
        return ( mCount == 0 );
    }

    //! Counts the number of elements currently stored in the table.
    /*!
     * \return The current number of elements in the table.
     */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    unsigned long int HashTbl< KeyType, DataType, KeyHash, KeyEqual >::count () const
    {
        return mCount;
    }

    //! Prints out the hash table content.
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    void HashTbl< KeyType, DataType, KeyHash, KeyEqual >::showStructure () const
    {
        KeyHash hashFn;

        // Traverse the list associated with the based address (idx), calculated before.
        for( auto i(0) ; i < mSize; ++i )
        {
            std::cout << i << " :{ key=";
            for( auto & e : mpDataTable[ i ] )
            {
                std::cout << hashFn( e.mKey ) << " ; " << e.mData << " " ;
            }
            std::cout << "}\n";
        }
    }

template <typename KeyType, typename DataType, typename KeyHash, typename KeyEqual>
void HashTbl<KeyType, DataType, KeyHash, KeyEqual>::rehash(void) {

    auto newSize = proximo(mSize);
    std::unique_ptr<std::forward_list<Entry>[]> slave(new std::forward_list<Entry>[newSize]);
    KeyHash hashFunc;
    for (auto i(0u); i < mSize; i++) {
        
        for (auto it = mpDataTable[i].begin(); it != mpDataTable[i].end(); it++){
            Entry temp((*it).mKey, (*it).mData);
            auto end(hashFunc(temp.mKey)%newSize);
            slave[end].push_front(temp);
        }
    }

    this->~HashTbl();
    mpDataTable = std::move(slave);
    mSize = newSize;

}
} // namespace MyHashTable
