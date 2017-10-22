#include <new>
#include <iostream>
//#include <vector>

class IntVector
{
public:
	IntVector()
		:m_size(0),
		m_capacity(1)
	{
		m_memory = new int[m_capacity];
	}

	int operator[](size_t i)
	{
		return *(static_cast<int*>(m_memory) + i);
	}

	void push_back(int i)
	{
		if (m_capacity < m_size + 1)
		{
			resize(m_capacity * 2);
		}

		int* tmp = new(static_cast<int*>(m_memory) + m_size) int;
		*tmp = i;
		m_size++;
	}

	size_t size()
	{
		return m_size;
	}

	void resize(size_t s)
	{
		size_t newCapacity = s;
		void* newMemory = new int[newCapacity];

		for (size_t i = 0; i < m_capacity; i++)
		{
			int* tmp = new(static_cast<int*>(newMemory) + i) int;
			*tmp = *(static_cast<int*>(m_memory) + i);
		}

		delete[] m_memory;
		m_memory = newMemory;
		m_capacity = newCapacity;
	}

	~IntVector()
	{
		delete[] m_memory;
	}

private:
	void* m_memory;
	size_t m_capacity;
	size_t m_size;
};

template <class T>
class vector
{
public:
	vector()
	{
		m_size = 0;
		m_capacity = 1;
		m_memory = new T[m_capacity];
	};

	T operator[](size_t i)
	{
		return *(static_cast<T*>(m_memory) + i * sizeof(T*));
	};

	void push_back(const T& i)
	{
		if (m_size >= m_capacity)
		{
			reserve(m_capacity * 2);
		}

		T* tmp = new(static_cast<T*>(m_memory) + m_size * sizeof(T*)) T;
		*tmp = i;
		m_size++;
	};

	size_t capacity()
	{
		return m_capacity;
	};

	size_t size()
	{
		return m_size;
	};

	void resize(size_t s, T val = T())
	{
		if (s < m_size)
		{
			for (size_t i = s; i < m_size; i++)
			{
				(static_cast<T*>(m_memory) + i * sizeof(T*))->~T();
			}
			m_size = s;
		}
		else if (s > m_size)
		{
			if (s > m_capacity)
			{
				size_t newCapacity = s;
				void* newMemory = new T[newCapacity];

				//for (size_t i = 0; i < s; i++)
				//{
				//	T* tmp = new(static_cast<T*>(newMemory) + i * sizeof(T*)) T;

				//	if (i < m_size)
				//		*tmp = *(static_cast<T*>(m_memory) + i * sizeof(T*));//copy old object
				//	else
				//		*tmp = val;//create default
				//}

				delete[] m_memory;
				m_memory = newMemory;
				m_capacity = newCapacity;
				m_size = newCapacity;
			}
			else {
				for (size_t i = m_size; i < s; i++)
				{
					T* tmp = new(static_cast<T*>(m_memory) + i * sizeof(T*)) T;
					*tmp = val;
				}
				m_size = s;
			}
		}
	};

	void reserve(size_t s)
	{
		size_t newCapacity = s;
		void* newMemory = new T[newCapacity];

		for (size_t i = 0; i < m_size; i++)
		{
			T* tmp = new(static_cast<T*>(newMemory) + i * sizeof(T*)) T;
			*tmp = *(static_cast<T*>(m_memory) + i * sizeof(T*));
		}

		m_capacity = newCapacity;
	};

	~vector()
	{
		delete[] m_memory;
	};

private:
	void* m_memory;
	size_t m_capacity;
	size_t m_size;
};

int main()
{
	vector<double>* ownIntVec = new vector<double>();

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;

	for (int i = 0; i < 10; i++)
		ownIntVec->push_back(0.125 * i);

	for (int i = 0; i < 10; i++)
		std::cout << (*ownIntVec)[i] << std::endl;

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;
	std::cin.ignore();

	delete ownIntVec;
/*
	IntVector intVec;
	for(int i = 0; i < 10; i++)
		intVec.push_back(i);

	for (int i = 0; i < 10; i++)
		std::cout << intVec[i] << std::endl;

	std::cin.ignore();*/
}