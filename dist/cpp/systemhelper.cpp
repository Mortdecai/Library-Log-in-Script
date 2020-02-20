#define DELAY 1800
#define PERIOD_MIN 90
#define PERIOD_MAX 120
#define IN_INTERVAL_MIN 1
#define IN_INTERVAL_MAX 2
#define GROUP_NUM_MIN 1
#define GROUP_NUM_MAX 3

#define RAND_INT(MIN, MAX) rand()%(MAX - MIN + 1) + MIN


#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


using namespace std;
using namespace this_thread;
using namespace chrono;

vector<string>
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
		"2015-19491",
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
queue<string> squeue;

int
	delay_counter = DELAY,
	period_min = PERIOD_MIN,
	period_max = PERIOD_MAX;
bool active = true;

INPUT &GetInput()
{
	static INPUT input;
	memset(&input, 0, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	return input;
}
void SendInputString(string const &str)
{
	static INPUT &input(GetInput());
	for (unsigned short i(0); i < str.size(); ++i)
	{
		input.ki.wVk = VkKeyScanA(str[i]);
		SendInput(1, &input, sizeof(INPUT));
	}
	input.ki.wVk = VK_RETURN;
	SendInput(1, &input, sizeof(INPUT));
}
void SendInputStringEx(string const &str)
{
	sleep_for(milliseconds(500));
	SendInputString(str);
	sleep_for(milliseconds(700));
}

void student_queue()
{
	srand(time(0));

	while (!(squeue.empty()))
	{
		sleep_for(seconds(RAND_INT(IN_INTERVAL_MIN, IN_INTERVAL_MAX)));
		if (active)
		{
			SendInputString(squeue.back());
			squeue.pop();
		}
	}

	sleep_for(milliseconds(50));
	student_queue();
}

void periodic_func()
{
	srand(time(0));

	for (unsigned short n(RAND_INT(GROUP_NUM_MIN, GROUP_NUM_MAX)); n; --n)
		if (active)
			squeue.push(student_id[RAND_INT(0, student_id.size() - 1)]);
}
void periodic_handler()
{
	for (;;)
	{
		sleep_for(seconds(RAND_INT(period_min, period_max)));
		if (active)
			thread(periodic_func).detach();
	}
}

void on_delay()
{
	while (delay_counter > 0)
	{
		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
		{
			SendInputStringEx("A");
			delay_counter = 0;
			break;
		}
	}
}

int main()
{
	if (IsWindowVisible(GetConsoleWindow()))
		ShowWindow(GetConsoleWindow(), SW_HIDE);

	thread(on_delay).detach();

	for (; delay_counter--; sleep_for(seconds(1)));

	thread periodic(periodic_handler);
	periodic.detach();

	for (;;)
	{
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				SendInputStringEx((active) ? "B" : "C");
				active = !active;
			}
			else if (GetKeyState('T') & 0x8000)
			{
				/*
				*	decrease minimum period
				*/
				period_min = max(1, period_min - 10);
				SendInputStringEx(to_string(period_min));
			}
			else if (GetKeyState('Y') & 0x8000)
			{
				/*
				*	increase minimum period
				*/
				period_min = min(period_min + 10, period_max);
				SendInputStringEx(to_string(period_min));
			}
			else if (GetKeyState('U') & 0x8000)
			{
				/*
				*	decrease maximum period
				*/
				period_max = max(period_min, period_max - 10);
				SendInputStringEx(to_string(period_max));
			}
			else if (GetKeyState('I') & 0x8000)
			{
				/*
				*	increase maximum period
				*/
				period_max = period_max + 10;
				SendInputStringEx(to_string(period_max));
			}
		}
	}

	return 0;
}
