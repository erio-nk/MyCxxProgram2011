#if 0
///// うーｎうまくつくれん
template <class T, int SIZE>
class MemoryPool
{
	struct ControlBlock
	{
		int available;	// 1=OK 0=NG
		unsigned int size;
	};

	static MemoryPool<T, SIZE> inst;

	char* pPool;

public:
	MemoryPool()
	{
		pPool = malloc(SIZE);
		assert(pPool);
		memset(pPool, 0, SIZE);

		// 最初のブロック
		((ControlBlock*)pPool)->available = 1;
	}

	~MemoryPool()
	{
		free(pPool);
	}

	T* Alloc(short size)
	{
		if (!pPool) {
			pPool = malloc(SIZE);
			assert(pPool);
		}

		int s = 0;
		ControlBlock* pBlock = (ControlBlock*)pPool;
		while (s < SIZE) {
			if (pBlock->available) {
				if (pBlock->size == 0 || pBlock->size >= size) {

				}
			}
		}

		return NULL;
	}

	void Free(T* p)
	{
	}
};

char* template <class T, int SIZE> class MemoryPool::inst;

#endif

