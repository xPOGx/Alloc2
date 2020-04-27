#pragma once 

struct LBlockDescriptor {
	size_t nextFreeBlock;//shift
};

struct PageDescriptor {
	size_t next; // ������ ������� � ��������� ������	
	LBlockDescriptor* firstFree; // ������ ���� � ���� 1
	size_t bsize; // ����� lBlock � ���� 1 ��� ������� ����� � ���� 2
	char state; // 0-������, 1-lblock, 2-mblock	
};

class Alloc2 {
public:
	Alloc2(const size_t ms, const size_t ps);

	// ������� addr �� ������� ��������� ����� ��� NULL
	void* alloc_addr(size_t size);

	// ������� addr �� ������� ��������� ����� ��� NULL
	void* alloc_ReAddr(void* addr, size_t size);

	// ������� ���� �� ���� �������
	void release_addr(void* addr);

	// ���� � �������
	void OUTPUT();

private:

	// ������� ������������� ����������
	size_t* begin;

	// ������� ������� � ���'��
	size_t* pagesBegin;
	size_t size;
	size_t pages;
	size_t pageSize;

	// ����� ��� ����������� �������
	PageDescriptor* pageDescriptors;

	// ����� ������� � ������ 1
	size_t* lBlocks;

	size_t lBlocksLength;

	size_t firstFreePage;

	// ��������, ���� ��� ����� ������� ��� ����� ������
	PageDescriptor defineCategory(size_t s);

	// ���������� s �� �������� ��������� ��������� ������ 2
	size_t defineBlockSize(size_t s);

	// �������� �� ������� �� ����
	void initPages();

	// ��������� ������ ���� � size bs
	size_t* getFreeLBlock(size_t bs);

	// ������� index ��� ������ lBlocks
	size_t getIndex(size_t s);

	size_t createLBlockPage(size_t bs);

	//size_t getLBlockPage(size_t bs);
	size_t getFreePage();

	// ������ ���������� ������� � index ������� ������
	size_t* getAbsolutePageAddr(size_t index);

	// ���������� �� ����� �� ������� � ������� ����
	void setAllFree(PageDescriptor pd);

	// ��������� ������� ���� � length ps*pageSize
	size_t* getFreeMBlock(size_t ps);

	// ��������, �� ��� ���� � ����� ������ �� ��� �������
	bool freeLBlockIsLast(PageDescriptor pd);

	// ������� ������� ������� � addr
	size_t findPageByAddress(size_t* addr);

	// ������� ������� ����� � addr
	size_t findBlockByAddress(size_t* addr, size_t bs);

	// ��������� ����� � ����� ����糿 � ����
	void copyData(size_t* from, size_t* to, size_t length);
};
