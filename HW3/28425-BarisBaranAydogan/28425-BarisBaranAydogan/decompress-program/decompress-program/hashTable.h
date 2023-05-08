#include <iostream>
#include <string>

using namespace std;

// template HashTable class
template <class T, class V>
class HashTable
{
private:
    // struct for the hash table
    struct HashNode
    {
        T key;
        V value;
        bool hasData;
        bool deleted;
    };
    // array of HashNodes(The hash table)
    HashNode *table;

    // hash function object
    hash<T> hf;

    // size of the table
    int size;

    // number of elements in the table
    int numElements;

    // hash function
    // takes in key and returns an index
    int hash(T key);

    // rehash function
    // rehashes the table when there is no way to resolve a collision
    void rehash();

public:
    // constructor
    HashTable(int size = 1024);
    // destructor
    ~HashTable();
    // insert function
    void insert(T key, V value);
    // remove function
    void erase(T key);
    // search function
    bool find(T key);

    // get the V value of a T key
    V getValue(T key);

    // overloaded [] operator to get the V value of a T key
    V &operator[](T key);

    // get the number of elements in the table
    int getNumElements();
};


// constructor
template <class T, class V>
HashTable<T, V>::HashTable(int size)
{
    // set the size of the table
    this->size = size;

    // set the number of elements to 0
    numElements = 0;

    // create the table with the given size
    table = new HashNode[size];

    // set all the hasData values to false
    // this is so we can tell if a spot in the table is empty
    for (int i = 0; i < size; i++)
    {
        table[i].hasData = false;
        table[i].deleted = false;
    }
}

// destructor
template <class T, class V>
HashTable<T, V>::~HashTable()
{
    // delete the table
    delete[] table;
}

// insert function
template <class T, class V>
void HashTable<T, V>::insert(T key, V value)
{
    // if the table is full
    if (numElements == size)
    {
        // rehash the table
        rehash();
    }

    // get the index of the key
    int index = hash(key);
    //cout << "index: " << index << endl;

    // if the spot in the table is empty
    if (!table[index].hasData)
    {
        // insert the key
        table[index].key = key;
        table[index].value = value;
        table[index].hasData = true;
        // increment the number of elements
        numElements++;
    }
    // if the spot in the table is not empty
    else
    {
        // if the key is already in the table
        if (table[index].key == key)
        {
            // do nothing
            return;
        }
        // if the key is not in the table
        else
        {
            // increment the index (linear probing)
            index++;
            // while the index is not out of bounds
            while (index < size)
            {
                // if the spot in the table is empty
                if (!table[index].hasData)
                {
                    // insert the key
                    table[index].key = key;
                    table[index].value = value;
                    table[index].hasData = true;
                    // increment the number of elements
                    numElements++;
                    // break out of the loop
                    break;
                }
                // if the spot in the table is not empty
                else
                {
                    // if the key is already in the table
                    if (table[index].key == key)
                    {
                        // update the value
                        table[index].value = value;
                        return;
                    }
                    // if the key is not in the table
                    else
                    {
                        // increment the index (linear probing)
                        index++;
                    }
                }
            }
            //the key should be in the table by now
        }
    }
}

// remove function
template <class T, class V>
void HashTable<T, V>::erase(T key)
{
    // get the index of the key
    int index = hash(key);

    // if the spot in the table is empty
    if (!table[index].hasData)
    {
        // do nothing
        return;
    }
    // if the spot in the table is not empty
    else
    {
        // if the key is in the table
        if (table[index].key == key)
        {
            // remove the key
            table[index].hasData = false;
            table[index].deleted = true;
            // decrement the number of elements
            numElements--;
        }
        // if the key is not in the table, search for it linearly
        else
        {
            int initialIndex = index;
            // increment the index (linear probing)
            index= (index + 1) % size;
            // while the index is not out of bounds
            while (index != initialIndex)
            {
                // if the spot in the table is empty
                if (!table[index].hasData)
                {
                    // do nothing
                    return;
                }
                // if the spot in the table is not empty
                else
                {
                    // if the key is in the table
                    if (table[index].key == key)
                    {
                        // remove the key
                        table[index].hasData = false;
                        table[index].deleted = true;
                        // decrement the number of elements
                        numElements--;
                        // break out of the loop
                        return;
                    }
                    // if the key is not in the table
                    else
                    {
                        // increment the index (linear probing)
                        index = (index + 1) % size;
                    }
                }
            }
        }
    }
}

// search function
template <class T, class V>
bool HashTable<T, V>::find(T key)
{
    // get the index of the key
    int index = hash(key);

    // if the spot in the table is empty
    if (!table[index].hasData)
    {
        // return false
        return false;
    }
    // if the spot in the table is not empty
    else
    {
        // if the key is in the table
        if (table[index].key == key)
        {
            // return true
            return true;
        }
        // if the key is not in the table, search for it linearly
        else
        {
            int initialIndex = index;
            // increment the index (linear probing)
            index = (index + 1) % size;
            // while the index is not out of bounds
            while (index != initialIndex)
            {
                // if the spot in the table is empty
                if (!table[index].hasData && !table[index].deleted)
                {
                    // return false
                    return false;
                }
                // if the spot in the table is not empty
                else
                {
                    // if the key is in the table
                    if (table[index].key == key)
                    {
                        // return true
                        return true;
                    }
                    // if the key is not in the table
                    else
                    {
                        // increment the index (linear probing)
                        index= (index + 1) % size;
                    }
                }
            }
        }
    }
    // if the key is not in the table
    return false;
}

// hash function
template <class T, class V>
int HashTable<T, V>::hash(T key)
{

    // return the index of the key
    return (hf(key) % size + size) % size;
}

// rehash function
template <class T, class V>
void HashTable<T, V>::rehash()
{
    // create a new table with double the size
    HashNode *newTable = new HashNode[size * 2];

    // set all the hasData values to false
    // this is so we can tell if a spot in the table is empty
    for (int i = 0; i < size * 2; i++)
    {
        newTable[i].hasData = false;
        newTable[i].deleted = false;
    }

    // set the size of the table
    size *= 2;

    // set the number of elements to 0
    numElements = 0;

    // swap the tables
    // the old table is now the new table
    // the new table is now the old table
    HashNode *temp = table;
    table = newTable;
    newTable = temp;

    // insert all the elements from the old table into the new table
    for (int i = 0; i < size / 2; i++)
    {
        if (newTable[i].hasData)
        {
            insert(newTable[i].key, newTable[i].value);
        }
    }

    // delete the old table
    delete[] newTable;

}

// get value function
template <class T, class V>
V HashTable<T, V>::getValue(T key)
{
    // get the index of the key
    int index = hash(key);

    // if the spot in the table is empty
    if (!table[index].hasData)
    {
        // return a default value
        return V();
    }
    // if the spot in the table is not empty
    else
    {
        // if the key is in the table
        if (table[index].key == key)
        {
            // return the value
            return table[index].value;
        }
        // if the key is not in the table, search for it linearly
        else
        {
            //initialIndex
            int initialIndex = index;
            // increment the index (linear probing)
            index= (index + 1) % size;
            // while the index is not out of bounds
            while (index != initialIndex)
            {
                // if the spot in the table is empty
                if (!table[index].hasData && !table[index].deleted)
                {
                    // return a default value
                    return V();
                }
                // if the spot in the table is not empty
                else if(!table[index].deleted)
                {
                    // if the key is in the table
                    if (table[index].key == key)
                    {
                        // return the value
                        return table[index].value;
                    }
                    // if the key is not in the table
                    else
                    {
                        // increment the index (linear probing)
                        index= (index + 1) % size;
                    }
                }
            }
        }
    }
    // if the key is not in the table
    return V();
}

// overloaded [] operator
// if the key is not in the table, it will be added
// then a reference to the value will be returned
template <class T, class V>
V &HashTable<T, V>::operator[](T key)
{
    // get the index of the key
    int index = hash(key);

    // if the spot in the table is empty
    if (!table[index].hasData)
    {
        // insert the key
        table[index].key = key;
        table[index].hasData = true;
        table[index].value = V();
        numElements++;

        // return a reference to the value
        return table[index].value;
    }
    // if the spot in the table is not empty
    else
    {
        // if the key is in the table
        if (table[index].key == key)
        {
            // return a reference to the value
            return table[index].value;
        }
        // if the key is not in the table, search for it linearly
        else
        {
            int initialIndex = index;
            // increment the index (linear probing)
            index= (index + 1) % size;
            // while the index is not out of bounds
            while (index != initialIndex)
            {
                // if the spot in the table is empty
                if (!table[index].hasData)
                {
                    // insert the key
                    table[index].key = key;
                    table[index].hasData = true;
                    table[index].value = V();
                    numElements++;
                    // return a reference to the value
                    return table[index].value;
                }
                // if the spot in the table is not empty
                else 
                {
                    // if the key is in the table
                    if (table[index].key == key)
                    {
                        // return a reference to the value
                        return table[index].value;
                    }
                    // if the key is not in the table
                    else
                    {
                        // increment the index (linear probing)
                        index= (index + 1) % size;
                    }
                }
            }
        }
    }
    // if the key is not in the table
    // insert the key
    insert(key, V());
    // return a reference to the value
    return (*this)[key];
}
