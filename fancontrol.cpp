#include "fancontrol.h"

using namespace std;
char out[32];
char fspeed[8];

int main(int argc, char **argv)
{
    int cpu0_temp, cpu1_temp, avg_temp, speedarg;
    int restarting = 0;

    string host, user, pass;

    // populate credentials from .env
    FILE *fp;
    ifstream envFile;
    envFile.open(".env");
    if (envFile.fail())
    {
        throw ".env file could not be opened!";
    }
    string line;
    while (getline(envFile, line))
    {
        cout << line << "<\n";
        if (line.length() == 0)
        {
            continue;
        }
        if (line.find("HOST") != string::npos)
        {
            host = line.substr(line.find("=") + 1);
            continue;
        }
        if (line.find("USER") != string::npos)
        {
            user = line.substr(line.find("=") + 1);
            continue;
        }
        if (line.find("PASS") != string::npos)
        {
            pass = line.substr(line.find("=") + 1);
            continue;
        }
    }
    envFile.close();

    printf("DELL iDRAC FAN CONTROL\n");
    if (argc == 2)
    {
        speedarg = atoi(argv[1]);
        if (!(speedarg >= 0 || speedarg <= 100))
        {
            printf("Zero/Invalid Params.\nSetting default fan speed\n");
            set_speed(DEFAULT_SPEED);
            printf("Fan speed set to %i%%\n", DEFAULT_SPEED);
        }
        else
        {
            set_speed(speedarg);
            printf("Fan speed set to %i%%\n", speedarg);
        }
    }
    else
    {
        printf("Zero/Invalid Params.\nSetting default fan speed\n");
        set_speed(DEFAULT_SPEED);
        printf("Fan speed set to %i%%\n", DEFAULT_SPEED);
    }

    string com;
    while (1)
    {
        // Store current cpu temps
        fp = popen("sensors -u coretemp-isa-0000 | grep temp1_input: | cut -c16-17", "r");
        cpu0_temp = get_temp(fp);

        fp = popen("sensors -u coretemp-isa-0001 | grep temp1_input: | cut -c16-17", "r");
        cpu1_temp = get_temp(fp);

        avg_temp = (cpu0_temp + cpu1_temp) / 2;

        printf("Avergage: %i deg C\n", avg_temp);
        if (avg_temp > TEMP_LIMIT)
        {
            printf("too hot: %i deg C\n", avg_temp);

            // set to automatic control
            com = "ipmitool -I lanplus -H " + host + " -U " + user + " -P " + pass + " raw 0x30 0x30 0x02 0xff 0x0F > /dev/null 2>&1";
            system(com.c_str());
        }
        else
        {
            printf("idle: %i deg C\n", avg_temp);

            // set to manual control
            com = "ipmitool -I lanplus -H " + host + " -U " + user + " -P " + pass + " raw 0x30 0x30 0x01 0x00 > /dev/null 2>&1";
            system(com.c_str());

            // set fan speed
            com = "ipmitool -I lanplus -H " + host + " -U " + user + " -P " + pass + " raw 0x30 0x30 0x02 0xff " + fspeed + " > /dev/null 2>&1";
            system(com.c_str());
        }

        // Connectivity check
        if (NETSTAT)
        {
            restarting--;
            if (restarting < 0)
            {
                // wait for NetworkManager to start again
                restarting = 3;
                fp = popen("ping -c 2 8.8.8.8 || systemctl restart NetworkManager", "r");
                cout << "checking internet: " << to_string(get_temp(fp));
            }
        }
        sleep(3);
    }
}

int get_temp(FILE *fp)
{
    while (fgets(out, sizeof(out), fp) != NULL)
    {
    }
    pclose(fp);
    return atoi(out);
}

void set_speed(int speed)
{
    sprintf(fspeed, "0x%x", speed);
    return;
}
