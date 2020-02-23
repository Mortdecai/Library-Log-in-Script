#Windows Support?

import keyboard as kb;
import random as r;
import time;

id_number = ['2015-21039', '2014-00533', '2015-23057', '2015-19937', '2015-22687', '2015-21873', '2015-20154', '2015-19866', '2015-21965', '2015-19631', '2015-21549', '2014-15771', '2015-21261', '2015-20090', '2015-19648', '2014-14907', '2014-14901', '2015-21753', '2015-20170', '2015-22612', '2015-22181', '2015-20202', '2015-19987', '2015-19491', '2015-20974', '2015-21287', '2015-20099', '2015-20245', '2015-20096', '2015-19590', '2015-22980', '2015-20228', '2017-18178'];
# Len of id_nums = 33

switch = True;
wait_time = 1;
min_time = 10;
max_time = 10;

def switchOff(x):
    global min_time, max_time;
    for i in range(x):
        time.sleep(1);

        if kb.is_pressed('shift + `'):
            global switch;
            switch = False;
            kb.write("OFF");
            break;
        elif kb.is_pressed('shift + T'):
            if min_time > 5:
                min_time -= 5;
            kb.write(str(min_time));
        elif kb.is_pressed('shift + Y'):
            if min_time < max_time:
                min_time += 5;
            kb.write(str(min_time));
        elif kb.is_pressed('shift + U'):
            if max_time > min_time:
                max_time -= 5;
            kb.write(str(max_time));
        elif kb.is_pressed('shift + I'):
            max_time += 5;
            kb.write(str(max_time))



def switchOn():
    while True:
        time.sleep(1);
        if kb.is_pressed('shift + 1'):
            global switch;
            switch = True;
            kb.write("ON");
            break;

def wait():
    global wait_time;
    for i in range(wait_time):
        time.sleep(1);

        if kb.is_pressed('shift + Q'):
            wait_time = 1;
            kb.write("SKIP");
            break;

def main():
    global switch;5
    while switch:
        for i in range(r.randint(0, 4)):
            kb.write(id_number[r.randint(0, 32)]);
            kb.press_and_release('Enter');
        switchOff(r.randint(min_time, max_time)); #Controls the interval of log-ins

    while (switch == False):
        switchOn();

    main();

wait();
main();


