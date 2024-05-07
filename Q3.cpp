// Filename: Q3.cpp
// Author: Erin M. McIsaac

#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

struct requestStructure 
{
    int request_id;
    string ip_address;
    string page_requested;
};

queue<requestStructure> msg_queue;
mutex mtx;
condition_variable cv;
int counter = 1;

string webPages[10] = {"google.com", "yahoo.com", "example.com", "stackoverflow.com", "github.com", "wikipedia.org", "amazon.com", "bing.com", "linkedin.com", "netflix.com"};

void listen() 
{
    while (true) 
    {
        this_thread::sleep_for(chrono::seconds(rand() % 3 + 1)); // Sleep for 1-3 seconds
        unique_lock<mutex> lck(mtx);
        requestStructure req;
        req.request_id = counter++;
        req.ip_address = ""; // For simplicity, we leave the IP address empty
        req.page_requested = webPages[rand() % 10]; // Randomly select a web page
        msg_queue.push(req);
        cv.notify_one();
    }
}

void do_request(int thread_id) 
{
    while (true) {
        unique_lock<mutex> lck(mtx);
        cv.wait(lck, []{ return !msg_queue.empty(); });
        requestStructure req = msg_queue.front();
        msg_queue.pop();
        lck.unlock();

        cout << "Thread " << thread_id << " completed request " << req.request_id
             << " requesting webpage " << req.page_requested << endl;
    }
}

int main() 
{
    thread listener(listen);
    const int n = 5; // Number of consumer threads
    thread consumers[n];
    for (int i = 0; i < n; ++i) {
        consumers[i] = thread(do_request, i+1);
    }

    listener.join();
    for (int i = 0; i < n; ++i) {
        consumers[i].join();
    }

    return 0;
}