#define DELAY 1800
#define PERIOD_MIN 90
#define PERIOD_MAX 120
#define IN_INTERVAL_MIN 1
#define IN_INTERVAL_MAX 2
#define GROUP_NUM_MIN 1
#define GROUP_NUM_MAX 3

//*******************************************************************************

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

#define rand_int(min, max) rand()%(max - min + 1) + min

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

queue<string>
	squeue,
	temp_queue;

int
	delay_counter = DELAY,
	period_min = PERIOD_MIN,
	period_max = PERIOD_MAX,
	periodic_timeout = 0;
bool list_front = true;
bool active = true;

//*******************************************************************************

INPUT &GetInput()
{
	static INPUT input;
	memset(&input, 0, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	return input;
}
void SendInputString(string const &str, bool enter = true)
{
	static INPUT &input(GetInput());

	for (unsigned short i(0); i < str.size(); ++i)
	{
		input.ki.wVk = VkKeyScanA(str[i]);
		SendInput(1, &input, sizeof(INPUT));
	}

	if (enter)
	{
		input.ki.wVk = VK_RETURN;
		SendInput(1, &input, sizeof(INPUT));
	}
}
void SendInputStringEx(string const &str, bool enter = true)
{
	sleep_for(milliseconds(500));
	SendInputString(str, enter);
}

void manage_queue()
{
	while (!(squeue.empty()))
	{
		unsigned int sleep_duration(rand_int(IN_INTERVAL_MIN, IN_INTERVAL_MAX));
		if (sleep_duration)
			sleep_for(seconds(sleep_duration));

		if (active)
		{
			SendInputString(squeue.front());
			squeue.pop();
		}
	}

	sleep_for(milliseconds(50));
	manage_queue();
}

void periodic_handler()
{
	for (;;)
	{
		int period(rand_int(period_min, period_max));
		for (unsigned int elapsed(0); period > 0; --period)
		{
			sleep_for(seconds(1));
			if (elapsed++ > period_max)
				break;
		}

		if (active)
		{
			unsigned int mid(max(0, int(student_id.size()*0.5 - 0.5)));
			vector<string> temp_vec;

			if (temp_queue.empty())
			{
				for (unsigned int i(list_front ? 0 : mid); i < (list_front ? mid + 1 : student_id.size()); temp_vec.push_back(student_id[i++]));
				random_shuffle(temp_vec.begin(), temp_vec.end());
				for (unsigned int i(0); i < temp_vec.size(); ++i)
					temp_queue.push(temp_vec[i]);
				list_front = !list_front;
			}

			for (unsigned short n(rand_int(GROUP_NUM_MIN, GROUP_NUM_MAX)); n || temp_queue.empty(); --n)
			{
				squeue.push(temp_queue.front());
				temp_queue.pop();
			}
		}
	}
}

void on_delay_skip()
{
	while (delay_counter > 0)
		if ((GetKeyState(VK_CONTROL) & 0x8000) && (GetKeyState(VK_SHIFT) & 0x8000))
		{
			SendInputStringEx("A", false);
			delay_counter = 0;
			break;
		}
}

int main()
{
	srand(time(0));

	if (IsWindowVisible(GetConsoleWindow()))
		ShowWindow(GetConsoleWindow(), SW_HIDE);

	thread(on_delay_skip).detach();

	for (; delay_counter--; sleep_for(seconds(1)));

	thread(periodic_handler).detach();
	thread(manage_queue).detach();

	for (;;)
	{
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				SendInputStringEx((active) ? "B" : "C", false);
				active = !active;
			}
			else if (GetKeyState('T') & 0x8000)
			{
				/*
				*	decrease minimum period
				*/
				period_min = max(1, period_min - 10);
				SendInputStringEx(to_string(period_min), false);
			}
			else if (GetKeyState('Y') & 0x8000)
			{
				/*
				*	increase minimum period
				*/
				period_min = min(period_min + 10, period_max);
				SendInputStringEx(to_string(period_min), false);
			}
			else if (GetKeyState('U') & 0x8000)
			{
				/*
				*	decrease maximum period
				*/
				period_max = max(period_min, period_max - 10);
				SendInputStringEx(to_string(period_max), false);
			}
			else if (GetKeyState('I') & 0x8000)
			{
				/*
				*	increase maximum period
				*/
				period_max = period_max + 10;
				SendInputStringEx(to_string(period_max), false);
			}
		}
	}

	return 0;
}
