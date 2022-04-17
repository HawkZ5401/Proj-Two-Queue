//Queue.cpp

#include <iostream>
#include <fstream>

using namespace std;

struct job
{
    char jobType;
    int arrTime;
    int processTime;
};
const int jobSize = sizeof(job);

struct CPU
{
private:
    int totalProcessTime;
    int totalIdleTime;

    bool hasJob;
    int workTime; //this will match the process time of the job its working on

public:
    int runTime = 0;
    int idleTime = 1;

    void setWorkTime(job x) { workTime = x.processTime; }

    void toggle() //Run Time and Idle Time reset to 0 when a processor toggles from run to idle or idle to run
    {
        runTime = 0;
        idleTime = 0;
    }

    void addTotProc() { totalProcessTime++; }

    void addTotIdle() { totalIdleTime++; }

    bool getStatus() { return hasJob; }
};

class Queue
{
private:
    int front;
    int rear;
    job arr[5570];
    int amount;

public:
    

    Queue()
    {
        front = -1;
        rear = -1;
    }

    job peek(int pos)
    {
        if (isEmpty())
        {
            cout << "Queue is empty" << endl;
        }
        else
        {
            return arr[pos];
        }
    }

    bool isEmpty()
    {
        if (front == -1 && rear == -1) //-1 is default value
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool isFull() //i dont think the queue should ever be full
    {
        if (rear == 5570)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void enqueue(job val)
    {
        if (isFull())
        {
            cout << "Queue is full" << endl;
            return;
        }
        else if (isEmpty())
        {
            rear = 0;
            front = 0;
            arr[rear] = val;
            amount++;
        }
        else
        {
            rear++;
            arr[rear] = val;
            amount++;
        }
    }

    job dequeue()
    {
        job x;

        if (isEmpty())
        {
            cout << "Queue is empty" << endl;
            return;
        }
        else if (front == rear)
        {
            x = arr[front];
            arr[front] = { 'X', 0, 0 };
            front = -1;
            rear = -1;
            amount--;

            return x;
        }
        else
        {
            x = arr[front];
            arr[front] = { 'X', 0, 0 };
            front++;
            amount--;

            return x;
        }
    }

    int getAmount() { return amount; }
};

int main()
{
    int numOfCPUs = 1; //number of CPUs for test
    CPU cpus[1];

    fstream jobFile("org.dat", ios::in | ios::out | ios::trunc | ios::binary); //org.dat is the organized file from the other program

    if (!jobFile)
    {
        cerr << "org.dat could not be opened" << endl;
        return -1;
    }

    job* jobList = new job[5570]; //lots of mem, so we use heap
    Queue jobQ;

    for (int i = 0; i < 5570; i++) //read file contents into array
    {
        jobFile.seekg((static_cast<std::basic_istream<char, std::char_traits<char>>::off_type>(i) * jobSize), ios::beg);
        jobFile.read((char*)&jobList[i], jobSize);
    }

    //for all the "cout" you see here, make a log file and write to there instead. cout was for debugging
    for (int time = 1; time < 10000; time++) //run for 10000 time units
    {
        static int i = 0; //where we are in array

        /* CHECK FOR ARRIVALS */
        static int numOfJobs = 0, Ajobs = 0, Bjobs = 0, Cjobs = 0, Djobs = 0; //Bjobs LOOOOL

        while (jobList[i].arrTime == time) //accounts for multiple jobs with same arrival time
        {
            jobQ.enqueue(jobList[i]);

            if (jobList[i].jobType == 'A')
            {
                cout << "Time: " << time << "  Arrival: Overall Job: " << numOfJobs << ", Job " << jobList[i].jobType << ":" << Ajobs << ", Processing Time " << jobList[i].processTime << endl;
                Ajobs++;
            }
            else if (jobList[i].jobType == 'B')
            {
                cout << "Time: " << time << "  Arrival: Overall Job: " << numOfJobs << ", Job " << jobList[i].jobType << ":" << Bjobs << ", Processing Time " << jobList[i].processTime << endl;
                Bjobs++;
            }
            else if (jobList[i].jobType == 'C')
            {
                cout << "Time: " << time << "  Arrival: Overall Job: " << numOfJobs << ", Job " << jobList[i].jobType << ":" << Cjobs << ", Processing Time " << jobList[i].processTime << endl;
                Cjobs++;
            }
            else if (jobList[i].jobType == 'D')
            {
                cout << "Time: " << time << "  Arrival: Overall Job: " << numOfJobs << ", Job " << jobList[i].jobType << ":" << Djobs << ", Processing Time " << jobList[i].processTime << endl;
                Djobs++;
            }

            numOfJobs++;
            i++;
        }

        /* CHECK IF JOBS FINISH */


        /* CHECK IF CPU IS IDLE AND IF NECESSARY GIVE IT A JOB */
        int work = 0;
        for (int k = 0; k < numOfCPUs; k++)
        {
            if (jobQ.isEmpty() != true) //work to do
            {
                cpus[k].toggle();
                cout << "Time:" << time << "- Begin Processing Job" << work << ", Job " << jobQ.peek(work).jobType << ":1 in CPU" << k << endl;
            }

        }

        /* WRAP UP FOR THE DAY */ //"the status statement showing queue and CPU status is the last thing done at any time unit"
        //display time
        cout << "Time: " << time;

        //display how many jobs in q
        if (jobQ.isEmpty())
        {
            cout << "  Queue: Empty;";
        }
        else
        {
            cout << "  Queue: " << jobQ.getAmount();
        }

        //display idle/run times
        cout << "Time: " << time;

        for (int j = 0; j < numOfCPUs; j++)
        {
            if (cpus[1].getStatus() == false) //if has no job
            {
                cout << " CPU " << j << " Idle Time: " << cpus[j].idleTime << ";";
            }
            else if (cpus[1].getStatus() == true) //if has job
            {
                cout << " CPU " << j << " Run Time: " << cpus[j].runTime << ";";
            }
        }
        cout << endl;
    }











	return 0;
}