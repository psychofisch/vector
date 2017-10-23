//Fischer Thomas, Was Viktor, Winkler Roman

#include <new>
#include <iostream>

//all specifications from http://www.cplusplus.com/reference/vector/vector/
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

	void resize(size_t s, const T& val = T())
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
						*tmp = *(static_cast<T*>(m_memory) + i);
					else
						*tmp = T(val);
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

	size_t erase(size_t start, size_t end)
	{
		T* data = static_cast<T*>(m_memory);

		//no range check needed -> An invalid position or range causes undefined behavior.(http://www.cplusplus.com/reference/vector/vector/erase/)
		for (size_t i = start; i <= end; i++)
			(data + i)->~T();

		size_t offset = end - start + 1;

		for (size_t i = start; offset > 0 && i < m_size; i++)
		{
			T* tmp;
			if (i <= end)
				tmp = new(data + i) T;
			else
				tmp = (data + i);

			*tmp = *(data + i + offset);
		}
		m_size -= offset;

		return start;
	};

	size_t erase(size_t p)
	{
		return erase(p, p);
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

class testClass
{
public:
	int a;
	double b;
};

#define TESTS 1

int main()
{
#if TESTS
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
	{
		size_t sz;

		vector<int> foo;
		sz = foo.capacity();
		std::cout << "making foo grow:\n";
		for (int i = 0; i < 100; ++i) {
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
		for (int i = 0; i < 100; ++i) {
			bar.push_back(i);
			if (sz != bar.capacity()) {
				sz = bar.capacity();
				std::cout << "capacity changed: " << sz << '\n';
			}
		}

		std::cout << "reserve test finished" << std::endl;
	}
	//*** rt

	//resize test
	{
		std::cout << std::endl;
		//expected output
		//myvector contains: 1 2 3 4 5 100 100 100 0 0 0 0
		vector<int> myvector;

		// set some initial content:
		for (int i = 1; i < 10; i++) myvector.push_back(i);

		myvector.resize(5);
		myvector.resize(8, 100);
		myvector.resize(12);

		std::cout << "myvector contains:";
		for (int i = 0; i < myvector.size(); i++)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
		std::cout << "resize test finished" << std::endl;
	}
	//*** rt

	//erase test
	{
		std::cout << std::endl;
		//expected output
		//myvector contains: 4 5 7 8 9 10
		vector<int> myvector;

		// set some values (from 1 to 10)
		for (int i = 1; i <= 10; i++)
			myvector.push_back(i);

		// erase the 6th element
		myvector.erase(5);

		// erase the first 3 elements:
		myvector.erase(0, 3);

		std::cout << "myvector contains:";
		for (unsigned i = 0; i < myvector.size(); ++i)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
		std::cout << "erase test finished" << std::endl;
	}
	//*** et
#endif

	//own test
	std::cout << std::endl;
	vector<testClass>* ownIntVec = new vector<testClass>();
	testClass tC;

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;

	for (int i = 0; i < 10; i++)
	{
		tC.a = i;
		tC.b = 0.125 * i;
		ownIntVec->push_back(tC);
	}

	for (int i = 0; i < 10; i++)
		std::cout << (*ownIntVec)[i].a << "|" << (*ownIntVec)[i].b << std::endl;

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;

	delete ownIntVec;

	std::cout << "press ENTER to exit\n";
	std::cin.ignore();
	//*** own test
}