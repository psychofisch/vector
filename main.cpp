#include <new>
#include <iostream>
//#include <vector>

template <class T>
class vector
{
public:
	vector()
	{
		m_size = 0;
		m_capacity = 0;
		m_memory = new T[m_capacity];
	};

	T operator[](size_t i)
	{
		return *(static_cast<T*>(m_memory) + i);
	};

	void push_back(const T& i)
	{
		if (m_size >= m_capacity)
		{
			if (m_capacity == 0)
				reserve(1);
			else
				reserve(m_capacity * 2);
		}

		T* tmp = new(static_cast<T*>(m_memory) + m_size) T;
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
				(static_cast<T*>(m_memory) + i)->~T();
			}
			m_size = s;
		}
		else if (s > m_size)
		{
			if (s > m_capacity)
			{
				size_t newCapacity = s;
				void* newMemory = new T[newCapacity];

				for (size_t i = 0; i < s; i++)
				{
					T* tmp = new(static_cast<T*>(newMemory) + i) T;

					if (i < m_size)
						*tmp = *(static_cast<T*>(m_memory) + i);//copy old object
					else
						*tmp = val;//create default
				}

				delete[] m_memory;
				m_memory = newMemory;
				m_capacity = newCapacity;
				m_size = newCapacity;
			}
			else {
				for (size_t i = m_size; i < s; i++)
				{
					T* tmp = new(static_cast<T*>(m_memory) + i) T;
					*tmp = val;
				}
				m_size = s;
			}
		}
	};

	void reserve(size_t s)
	{
		if (s > m_capacity)
		{
			size_t newCapacity = s;
			void* newMemory = new T[newCapacity];

			for (size_t i = 0; i < m_size; i++)
			{
				T* tmp = new(static_cast<T*>(newMemory) + i) T;
				*tmp = *(static_cast<T*>(m_memory) + i);
			}

			delete[] m_memory;
			m_memory = newMemory;
			m_capacity = newCapacity;
		}
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
	//reserve test
		//expected output
		/*
		making foo grow :
		capacity changed : 1
			capacity changed : 2
			capacity changed : 4
			capacity changed : 8
			capacity changed : 16
			capacity changed : 32
			capacity changed : 64
			capacity changed : 128
			making bar grow :
		capacity changed : 100
		*/

	size_t sz;

	vector<int> foo;
	sz = foo.capacity();
	std::cout << "making foo grow:\n";
	for (int i = 0; i<100; ++i) {
		foo.push_back(i);
		if (sz != foo.capacity()) {
			sz = foo.capacity();
			std::cout << "capacity changed: " << sz << '\n';
		}
	}

	vector<int> bar;
	sz = bar.capacity();
	bar.reserve(100);   // this is the only difference with foo above
	std::cout << "making bar grow:\n";
	for (int i = 0; i<100; ++i) {
		bar.push_back(i);
		if (sz != bar.capacity()) {
			sz = bar.capacity();
			std::cout << "capacity changed: " << sz << '\n';
		}
	}
	
	std::cout << "reserve test finished" << std::endl;
	std::cin.ignore();
	//*** rt

	vector<double>* ownIntVec = new vector<double>();

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;

	for (int i = 0; i < 10; i++)
		ownIntVec->push_back(0.125 * i);

	for (int i = 0; i < 10; i++)
		std::cout << (*ownIntVec)[i] << std::endl;

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;
	std::cin.ignore();

	delete ownIntVec;
}