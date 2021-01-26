#include <cstddef>

void			ft_memcopy(const void* src, void* dst, size_t size) {
	unsigned char*	b_src = (unsigned char*)src;
	unsigned char*	b_dst = (unsigned char*)dst;

	unsigned int*	w_src = (unsigned int*)b_src;
	unsigned int*	w_dst = (unsigned int*)b_dst;

	while (size >= 4) {
		*w_dst++ = *w_src++;
		size -= 4;
	}

	if (size) {
		b_dst = (unsigned char*)w_dst;
		b_src = (unsigned char*)w_src;
		while (size > 0)
		{
			*b_dst++ = *b_src++;
			size--;
		}
	}
}
