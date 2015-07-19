
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
  // Add your code here
  maxNumber = 500;
  currentNumber = 0;
  
  array = new ArrayDictionaryNode[maxNumber];
}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
        // Add your code here
        	
	for (int i = 0; i < currentNumber; i++) {
		if (strcmp(array[i].key, key) == 0)
		{
			array[i].data = (DataType*)record;
			return false;
		}
	}
	
	
	if(maxNumber == currentNumber) {
		int oldSize = maxNumber;
		maxNumber *= 2;
		
		ArrayDictionaryNode *newArray = new ArrayDictionaryNode[maxNumber];
		
		for(int j = 0; j < oldSize; j++) {
		
			newArray[j].key = (char*)malloc(sizeof(char)*100);
			
			strcpy(newArray[j].key, array[j].key);
			
			newArray[j].data = new DataType;
			newArray[j].data = (DataType*)array[j].data;
		}
		
		delete[] array;
		array = newArray;
	}
	currentNumber++;
	
	array[currentNumber -1].key = (char*)malloc(100);
	array[currentNumber -1].data = new DataType;
	
	array[currentNumber -1].key = strdup(key);
	array[currentNumber -1].data = (DataType*)record;
	
	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
        // add your code here
	for(int i = 0; i < currentNumber; i++) {
		if(strcmp(array[i].key,key) == 0) {
			return (DataType)array[i].data;
		}
	}
	
	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
        // Add your code here
        if(findRecord(key) == NULL) return false;
        
        ArrayDictionaryNode *newArray = new ArrayDictionaryNode[maxNumber];
        int j = 0;
        for(int i = 0; i <  currentNumber; i++) {
        	if(strcmp(array[j].key,key) != 0) {
        	
        		newArray[i].key = (char*)malloc(100);
				newArray[i].data = new DataType;
	
				newArray[i].key = strdup(array[i].key);
				newArray[i].data = (DataType*)array[i].data;	
				j++;
        	}
        	j++;
        }
        delete[] array;
        array = newArray;
	return true;
	
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
        // Add yoru code here
	return NULL;
}
