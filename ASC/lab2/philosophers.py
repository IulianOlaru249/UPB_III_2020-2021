from threading import Thread, Lock
import time

class Philosofer(Thread):
    def __init__(self, id, left_fork, right_fork):
        Thread.__init__(self)
        self.id = id
        self.left_fork = left_fork
        self.right_fork = right_fork

        # In order to solve the problem one philosofer
        # must pick the forks in reverse order.
        # Inefficient approach but gets the job done
        # ¯\_( ͡° ͜ʖ ͡°)_/¯

        if (self.id == 0):
            self.left_fork = right_fork
            self.right_fork = left_fork

    def run(self):
        while True:
            with self.left_fork:
                time.sleep(0.5)
                self.right_fork.acquire()
                print("Philosopher", self.id, "is eating")
                break
        
        self.right_fork.release()


def main():
    #initiate philosophers and forks
    philosophers = []
    philosophers_no = 5
    forks = []
    for i in range(philosophers_no):
        forks.append(Lock())

    #start philosophers
    for i in range(philosophers_no):
        philosopher = Philosofer(i, forks[i - 1], forks[i])
        philosophers.append(philosopher)
        philosopher.start()

    #join philosophers
    for i, philosopher in enumerate(philosophers):
        philosopher.join()

if __name__=="__main__":
    main()