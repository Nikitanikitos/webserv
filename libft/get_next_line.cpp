# include "libft.hpp"

int				get_next_line(int fd, std::string &line)
{
	static std::string	surplus = std::string();
	char				buffer[BUF_SIZE];
	size_t 				bytes;

	if (surplus.empty()) {
		while ((bytes = read(fd, buffer, BUF_SIZE)) > 0)
			surplus += buffer;
		if (!bytes && surplus.empty())
			return 0;
		surplus += '\0';
	}
	std::string::size_type pos = surplus.find("\n", 0);
	if (pos == std::string::npos) {
		line = surplus.substr(0);
		surplus.erase();
		return 1;
	}
	line = surplus.substr(0, pos);
	surplus = surplus.substr(pos + 1);
	return 1;
}
