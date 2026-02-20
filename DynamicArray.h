#pragma once

#include <iostream>
using namespace std;

// WEEK 06 ADDITION: Template class for dynamic array container
// Replaces raw pointer array with type-safe, reusable container
template <typename T>
class DynamicArray
{
private:
	T* data;           // Dynamic array of type T
	int size;          // Current number of elements
	int capacity;      // Maximum capacity before resize needed

	// Helper function to resize array when full
	void resize()
	{
		int newCapacity = capacity * 2;
		T* newData = new T[newCapacity];

		// Copy existing elements
		for (int i = 0; i < size; i++)
		{
			newData[i] = data[i];
		}

		// Delete old array
		delete[] data;

		// Update to new array
		data = newData;
		capacity = newCapacity;
	}

public:
	// Default constructor
	DynamicArray()
	{
		capacity = 5;  // Default capacity
		size = 0;
		data = new T[capacity];
	}

	// Parameterized constructor
	DynamicArray(int initialCapacity)
	{
		capacity = (initialCapacity > 0) ? initialCapacity : 5;
		size = 0;
		data = new T[capacity];
	}

	// Destructor
	~DynamicArray()
	{
		delete[] data;
		data = nullptr;
	}

	// Copy constructor (deep copy)
	DynamicArray(const DynamicArray& other)
	{
		capacity = other.capacity;
		size = other.size;
		data = new T[capacity];

		// Deep copy elements
		for (int i = 0; i < size; i++)
		{
			data[i] = other.data[i];
		}
	}

	// Copy assignment operator (deep copy)
	DynamicArray& operator=(const DynamicArray& other)
	{
		if (this != &other)  // Self-assignment check
		{
			// Delete existing data
			delete[] data;

			// Copy new data
			capacity = other.capacity;
			size = other.size;
			data = new T[capacity];

			for (int i = 0; i < size; i++)
			{
				data[i] = other.data[i];
			}
		}
		return *this;
	}

	// WEEK 06 ADDITION: Operator[] for array access with bounds checking
	// Returns reference for read/write access
	T& operator[](int index)
	{
		// Bounds checking - return first element if out of range (safe fallback)
		if (index < 0 || index >= size)
		{
			cout << "# Warning: Index " << index << " out of bounds (size=" << size << "). Returning index 0. #" << endl;
			return data[0];  // Safe fallback (assumes size > 0)
		}
		return data[index];
	}

	// WEEK 06 ADDITION: Const version of operator[] for const objects
	const T& operator[](int index) const
	{
		if (index < 0 || index >= size)
		{
			cout << "# Warning: Index " << index << " out of bounds (size=" << size << "). Returning index 0. #" << endl;
			return data[0];
		}
		return data[index];
	}

	// WEEK 06 ADDITION: Operator+= to add element to array
	DynamicArray& operator+=(const T& element)
	{
		// Resize if needed
		if (size >= capacity)
		{
			resize();
		}

		// Add element
		data[size] = element;
		size++;

		return *this;  // Return reference for chaining
	}

	// WEEK 06 ADDITION: Operator-= to remove element by index
	// Removes element at index and shifts remaining elements left
	DynamicArray& operator-=(int index)
	{
		// Validate index
		if (index < 0 || index >= size)
		{
			cout << "# Warning: Cannot remove index " << index << " (out of bounds). #" << endl;
			return *this;
		}

		// Shift elements left to close the gap
		for (int i = index; i < size - 1; i++)
		{
			data[i] = data[i + 1];
		}

		size--;
		return *this;  // Return reference for chaining
	}

	// Get current size
	int getSize() const
	{
		return size;
	}

	// Get capacity
	int getCapacity() const
	{
		return capacity;
	}

	// Check if empty
	bool isEmpty() const
	{
		return size == 0;
	}

	// Clear all elements (doesn't deallocate)
	void clear()
	{
		size = 0;
	}

	// Get element at index (safe access with nullptr return for out of bounds)
	T* get(int index)
	{
		if (index < 0 || index >= size)
		{
			return nullptr;
		}
		return &data[index];
	}

	// Const version of get
	const T* get(int index) const
	{
		if (index < 0 || index >= size)
		{
			return nullptr;
		}
		return &data[index];
	}
};