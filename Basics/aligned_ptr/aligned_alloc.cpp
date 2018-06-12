#include <stdlib.h>
#include <stdio.h>
#include <cstdint>

static const int ALIGNMENT = 4;
void *aligned_malloc(unsigned int size){
	void *ret;
	if(!size)
		return (void *)0;

	printf("size %d, newsize:%d\n", size, ALIGNMENT);
	void *p = malloc(size + sizeof(void *));
	printf("pointer allocated:%p\n", p);
	
	int value = (reinterpret_cast<char*>(p));
	int offset = (value % ALIGNMENT);
	int tail = ALIGNMENT - offset;
	ret = p + tail;
	*(reinterpret_cast<char *>(ret) - 1) = tail;
	printf("pointer allocated:%p, offset:%d, tail:%d, ret:%p\n", p, offset, *((char *)ret - 1), ret);
	return ret;
}

void aligned_free(void *p){
	int offset = *(reinterpret_cast<char *>(p) - 1);
	void *to_release = p - offset;
	printf("pointer received to release:%p, tail:%d, to_release:%p\n", p, offset, to_release);
	free (to_release);
}
int main(int argc, char **argv) {
	for (int i = 0; i < 1000; i++)
	{
		void *x = aligned_malloc(10);
		//printf("pointer received %p\n", x);
		aligned_free(x);
	}
	return 0;
}
