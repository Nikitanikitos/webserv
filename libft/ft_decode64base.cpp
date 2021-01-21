# include "libft.hpp"

uint32_t			get_index_64base(uint8_t chr) {
	if ('A' <= chr && chr <= 'Z')
		return (chr - 65);
	else if ('a' <= chr && chr <= 'z')
		return (chr - 71);
	else if ('0' <= chr && chr <= '9')
		return (chr + 4);
	else if (chr == '+')
		return (62);
	else if (chr == '/')
		return (63);
	else
		return (0);
}

std::string		ft_decode64base(const std::string& data) {
	std::string		result;
	uint32_t		bytes;
	char			chars[4];
	const char*		data_ptr;

	data_ptr = data.c_str();
	chars[3] = 0;
	for (size_t i = 0; i < data.size(); i += 4) {
		bytes = 0;
		bytes |= ((get_index_64base(data_ptr[i]) & 0b00111111) << 26) | ((get_index_64base(data_ptr[i + 1]) & 0b00111111) << 20) |
				((get_index_64base(data_ptr[i + 2]) & 0b00111111) << 14) | ((get_index_64base(data_ptr[i + 3]) & 0b00111111) << 8);
		chars[0] = bytes >> 24;
		chars[1] = bytes >> 16;
		chars[2] = bytes >> 8;
		result.append(chars);
	}
	return (result);
}
