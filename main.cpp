//Fischer Thomas, Was Viktor, Winkler Roman

#include <new>
#include <iostream>

//all specifications from http://www.cplusplus.com/reference/vector/vector/
template <class T>
class vector
{
public:
	vector()
		:m_size(0),
		m_capacity(0),
		m_memory(new T[m_capacity])
	{};

	T& operator[](size_t i)
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

	size_t capacity() const
	{
		return m_capacity;
	};

	size_t size() const
	{
		return m_size;
	};

	void resize(size_t s, const T& val = T())
	{
		T* memoryPtr = static_cast<T*>(m_memory);

		if (s < m_size)
		{
			for (size_t i = s; i < m_size; i++)
			{
				(memoryPtr + i)->~T();
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
						*tmp = *(memoryPtr + i);
					else
						*tmp = val;//would "T(val)" be better?
				}

				delete[] m_memory;
				m_memory = newMemory;
				m_capacity = newCapacity;
				m_size = newCapacity;
			}
			else {
				for (size_t i = m_size; i < s; i++)
				{
					T* tmp = new(memoryPtr + i) T;
					*tmp = val;//same question as above
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
				*tmp = *(static_cast<T*>(m_memory) + i);//see resize
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

		size_t offset = end - start;

		if (offset == 0)
			offset++;

		for (size_t i = start; i < m_size; i++)
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

	size_t erase_by_swap(size_t p)
	{
		T* data = static_cast<T*>(m_memory);
		(data + p)->~T();

		T* tmp = new(data + p) T;
		*tmp = *(data + m_size - 1);

		m_size--;

		return p;
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

	testClass(int v1, double v2)
	{
		a = v1;
		b = v2;
	}

	testClass()
		:testClass(0, 0.0)
	{}
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
		/*
		size: 10
		size: 9
		size: 6
		myvector contains: 4 5 7 8 9 10
		*/
		vector<int> myvector;

		// set some values (from 1 to 10)
		for (int i = 1; i <= 10; i++)
			myvector.push_back(i);

		std::cout << "size: " << myvector.size() << std::endl;
		// erase the 6th element
		myvector.erase(5);
		std::cout << "size: " << myvector.size() << std::endl;

		// erase the first 3 elements:
		myvector.erase(0, 3);
		std::cout << "size: " << myvector.size() << std::endl;

		std::cout << "myvector contains:";
		for (unsigned i = 0; i < myvector.size(); ++i)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
		std::cout << "erase test finished" << std::endl;
	}
	//*** et

	//erase_by_swap test
		std::cout << std::endl;
		//expected output
		/*
			size: 5
			size: 4
			myvector contains: 1 5 3 4
		*/
		vector<int> myvector;

		// set some values (from 1 to 10)
		for (int i = 1; i <= 5; i++)
			myvector.push_back(i);
		std::cout << "size: " << myvector.size() << std::endl;

		// erase the 2nd element
		myvector.erase_by_swap(1);
		std::cout << "size: " << myvector.size() << std::endl;

		std::cout << "myvector contains:";
		for (unsigned i = 0; i < myvector.size(); ++i)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
		std::cout << "erase_by_swap test finished" << std::endl;
	//*** ebst
#endif

	//own test
	std::cout << std::endl;
	vector<testClass>* ownIntVec = new vector<testClass>();
	testClass tC;

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;

	for (int i = 0; i < 20; i++)
	{
		tC.a = i;
		tC.b = 0.125 * i;
		ownIntVec->push_back(tC);
	}

	for (int i = 0; i < ownIntVec->size(); i++)
		std::cout << (*ownIntVec)[i].a << "|" << (*ownIntVec)[i].b << std::endl;

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;

	ownIntVec->resize(10);//no allocation
	ownIntVec->resize(20);//no allocation and elements 10-19 are (0|0.0)
	ownIntVec->reserve(50);//allocate memory here but size is still 20
	ownIntVec->resize(25, testClass(1337, 13.37));//no allocation should happen
	ownIntVec->erase(5);
	ownIntVec->erase(1, 4);
	ownIntVec->push_back(testClass(666, 66.6));
	ownIntVec->erase_by_swap(0);//first element is the one from one line above
	//vector has a size of 21 and a capacity of 50 here

	(*ownIntVec)[1].a = 10;
	(*ownIntVec)[1].b = 10.101;

	for (size_t i = 0; i < ownIntVec->size(); i++)
		std::cout << (*ownIntVec)[i].a << "|" << (*ownIntVec)[i].b << std::endl;

	std::cout << "size:" << ownIntVec->size() << "/" << ownIntVec->capacity() << std::endl;

	delete ownIntVec;

	std::cout << "press ENTER to exit\n";
	std::cin.ignore();
	//*** own test
}