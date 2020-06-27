
#include <dirent.h>
#include <fcntl.h>
#include <map>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

float get_cpu_temperature()
{
	std::map<int, float> temps;
	std::vector<std::string> hwmons;
	std::string hwmon = "/sys/class/hwmon";

	{
		DIR *d = opendir(hwmon.c_str());
		if (d) {
			while (1) {
				dirent *e = readdir(d);
				if (!e) break;
				if (e->d_name[0] != '.') {
					std::string s = hwmon + "/" + e->d_name;
					hwmons.push_back(s);
				}
			}
			closedir(d);
		}
	}

	auto ReadLine = [](std::string const &path)->std::string{
		int n = 0;
		char tmp[100];
		int fd = open(path.c_str(), O_RDONLY);
		if (fd != -1) {
			n = read(fd, tmp, sizeof(tmp) - 1);
			close(fd);
			while (n > 0 && isspace((unsigned char)tmp[n - 1])) {
				n--;
			}
		}
		tmp[n] = 0;
		return tmp;
	};

	for (std::string const &dir : hwmons) {
		std::string path = dir + "/name";
		std::string name = ReadLine(path);
		if (name == "coretemp") {
			DIR *d = opendir(dir.c_str());
			if (d) {
				while (1) {
					dirent *e = readdir(d);
					if (!e) break;
					int number;
					if (sscanf(e->d_name, "temp%d_", &number) == 1) {
						char const *p = strchr(e->d_name, '_');
						if (p) {
							p++;
							if (strcmp(p, "label") == 0) {
								path = dir + '/' + e->d_name;
								std::string label = ReadLine(path);
								int package;
								if (sscanf(label.c_str(), "Package id %d", &package) == 1) {
									path = dir + "/temp" + std::to_string(number) + "_input";
									std::string input = ReadLine(path);
									int temp = strtol(input.c_str(), nullptr, 10);
									temps[package] = temp / 1000.0;
								}
							}
						}
					}
				}
				closedir(d);
			}
		}
	}

	float temp = 0;
	int count = 0;
	for (auto const &pair : temps) {
		temp += pair.second;
		count++;
	}
	if (count > 0) {
		temp = temp / count;
	}

	return temp;
}

