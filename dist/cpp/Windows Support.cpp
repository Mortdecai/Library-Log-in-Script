#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

const unsigned int DELAY = 3600;
const unsigned int PERIOD_MIN = 90;
const unsigned int PERIOD_MAX = 120;

bool active = true;
std::vector<std::string>
	student_id
	{
		"2015-21039",
		"2014-00533",
		"2015-23057",
		"2015-19937",
		"2015-22687",
		"2015-21873",
		"2015-20154",
		"2015-19866",
		"2015-21965",
		"2015-19631",
		"2015-21549",
		"2014-15771",
		"2015-21261",
		"2015-20090",
		"2015-19648",
		"2014-14907",
		"2014-14901",
		"2015-21753",
		"2015-20170",
		"2015-22612",
		"2015-22181",
		"2015-20202",
		"2015-19987",
		"2015-19481",
		"2015-20974",
		"2015-21287",
		"2015-20099",
		"2015-20245",
		"2015-20096",
		"2015-19590",
		"2015-22980",
		"2015-20228",
		"2017-18178"
	};

#define RAND_INT(MIN, MAX) rand()%(MAX - MIN + 1) + MIN

void periodic_func()
{
	INPUT input;
	memset(&input, 0, sizeof(INPUT));

	input.type = INPUT_KEYBOARD;
	std::string str_id(student_id[RAND_INT(0, student_id.size() - 1)]);
	for(unsigned int i(0); i < str_id.size(); ++i)
	{
		input.ki.wVk = VkKeyScanA(str_id[i]);
		SendInput(1, &input, sizeof(INPUT));
	}

	input.ki.wVk = VK_RETURN;
	SendInput(1, &input, sizeof(INPUT));
}
void periodic_handler()
{
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(RAND_INT(PERIOD_MIN, PERIOD_MAX)));
		if (active)
			periodic_func();
	}
}

int main()
{
	srand(time(0));

	std::this_thread::sleep_for(std::chrono::seconds(DELAY));

	std::thread periodic(periodic_handler);
	periodic.detach();

	for (;;)
		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
		{
			active = !active;
			std::this_thread::sleep_for(std::chrono::milliseconds(1200));
		}

	return 0;
}
