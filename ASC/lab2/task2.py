"""
    Basic thread handling exercise:

    Use the Thread class to create and run more than 10 threads which print their name and a random
    number they receive as argument. The number of threads must be received from the command line.

    e.g. Hello, I'm Thread-96 and I received the number 42

"""

from threading import Thread
import random

class MyThread(Thread):
    """ Clasa care incapsuleaza codul nostru concurent """
    def __init__(self, id, nr):
        Thread.__init__(self)
        self.id = id
        self.nr = nr
 
    def run(self):
        print ("Hello, I'm Thread", self.id, "and I received the number", self.nr)

if __name__=="__main__":
    threads = []
    threads_no = int(input("Enter number of threads:\n"))
    for i in range(threads_no):
        thread = MyThread(i, random.random())
        threads.append(thread)
        thread.start()

    for i, thread in enumerate(threads):
        thread.join()