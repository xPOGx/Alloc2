#pragma once 

struct LBlockDescriptor {
	size_t nextFreeBlock;//shift
};

struct PageDescriptor {
	size_t next; // список сторінок з однаковим станом	
	LBlockDescriptor* firstFree; // вільний блок у стані 1
	size_t bsize; // розмір lBlock у стані 1 або кількість блоків у стані 2
	char state; // 0-вільний, 1-lblock, 2-mblock	
};

class Alloc2 {
public:
	Alloc2(const size_t ms, const size_t ps);

	// Повертає addr на початок виділеного блоку або NULL
	void* alloc_addr(size_t size);

	// Повертає addr на початок виділеного блоку або NULL
	void* alloc_ReAddr(void* addr, size_t size);

	// Звільняє блок за цією адресою
	void release_addr(void* addr);

	// Вивід в консоль
	void OUTPUT();

private:

	// Початок контролювання інформації
	size_t* begin;

	// Початок сторінок в пам'яті
	size_t* pagesBegin;
	size_t size;
	size_t pages;
	size_t pageSize;

	// Масив усіх дескрипторів сторінок
	PageDescriptor* pageDescriptors;

	// Масив сторінок зі станом 1
	size_t* lBlocks;

	size_t lBlocksLength;

	size_t firstFreePage;

	// Визначте, який тип права держави для цього розміру
	PageDescriptor defineCategory(size_t s);

	// Округлення s до мінімальної необхідної потужності розміру 2
	size_t defineBlockSize(size_t s);

	// Початкові всі сторінки які вільні
	void initPages();

	// Повернути вільний блок з size bs
	size_t* getFreeLBlock(size_t bs);

	// Повертає index для масиву lBlocks
	size_t getIndex(size_t s);

	size_t createLBlockPage(size_t bs);

	//size_t getLBlockPage(size_t bs);
	size_t getFreePage();

	// Адреса повернення сторінки з index сторінок масиву
	size_t* getAbsolutePageAddr(size_t index);

	// Встановити всі блоки на сторінці у вільному стані
	void setAllFree(PageDescriptor pd);

	// Повернути великий блок з length ps*pageSize
	size_t* getFreeMBlock(size_t ps);

	// Перевірте, чи цей блок є також вільний на цій сторінці
	bool freeLBlockIsLast(PageDescriptor pd);

	// Повертає кількість сторінок з addr
	size_t findPageByAddress(size_t* addr);

	// Повертає кількість блоків з addr
	size_t findBlockByAddress(size_t* addr, size_t bs);

	// Копіювання даних зі старої позизії в нову
	void copyData(size_t* from, size_t* to, size_t length);
};
