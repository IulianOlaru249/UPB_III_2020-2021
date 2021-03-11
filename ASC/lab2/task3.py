"""
Coffee Factory: A multiple producer - multiple consumer approach

Generate a base class Coffee which knows only the coffee name
Create the Espresso, Americano and Cappuccino classes which inherit the base class knowing that
each coffee type has a predetermined size.
Each of these classes have a get message method

Create 3 additional classes as following:
    * Distributor - A shared space where the producers puts coffees and the consumers takes them
    * CoffeeFactory - An infinite loop, which always sends coffees to the distributor
    * User - Another infinite loop, which always takes coffees from the distributor

The scope of this exercise is to correctly use threads, classes and synchronization objects.
The size of the coffee (ex. small, medium, large) is chosen randomly everytime.
The coffee type is chosen randomly everytime.

Example of output:

Consumer 65 consumed espresso
Factory 7 produced a nice small espresso
Consumer 87 consumed cappuccino
Factory 9 produced an italian medium cappuccino
Consumer 90 consumed americano
Consumer 84 consumed espresso
Factory 8 produced a strong medium americano
Consumer 135 consumed cappuccino
Consumer 94 consumed americano
"""

# NOTE: For testing purposes, there are only 3 producers and 3 consumers.
#       each making/consuming only 2 coffes.
#       The program in not dependant on the number of producers and consumers,
#       or the number of coffes they make/take.
#       To test full behaviout: * uncomment 'while true' loops.
#                               * change the variables consumer_number/ producer_number

from threading import Semaphore, Lock, Thread
import enum
import random

#Diffrent coffee sizes
class Sizes(enum.Enum):
   Small = 1
   Medium = 2
   Large = 3

#Diffrent coffee types
class Coffee:
    """ Base class """
    def __init__(self, name):
        self.name = name

    def get_name(self):
        """ Returns the coffee name """
        raise NotImplementedError

    def get_size(self):
        """ Returns the coffee size """
        raise NotImplementedError

class Espresso(Coffee):
    def __init__(self, size):
        super().__init__("espresso")
        self.size = size

    def get_name(self):
        return self.name

    def get_size(self):
        return self.size

class Americano(Coffee):
    def __init__(self, size):
        super().__init__("americano")
        self.size = size
    
    def get_name(self):
        return self.name

    def get_size(self):
        return self.size

class Cappuccino(Coffee):
    def __init__(self, size):
        super().__init__("cappuccino")
        self.size = size

    def get_name(self):
        return self.name

    def get_size(self):
        return self.size

#Buffer
class Distributor():
    def __init__(self, inventory_size, inventory):
        self.inventory = inventory
        self.inventory_size = inventory_size
        self.empty = Semaphore(value=inventory_size)
        self.full = Semaphore(value=0)

    def add_to_inventory(self, coffee):
        self.empty.acquire()
        self.inventory.append(coffee)
        self.full.release()

    def take_from_inventory(self):
        self.full.acquire()
        aux = self.inventory.pop()
        self.empty.release()
        return aux

#Producer
class CoffeeFactory(Thread):
    def __init__(self, id, distributor):
        Thread.__init__(self)
        self.id = id
        self.distributor = distributor

    def run(self):
        self.produce_coffees()

    def produce_random_coffee(self):
        coffee_choice = random.randint(0, 2)
        size_choice = random.randint(0, 2)
        new_coffee_size = ""

        if coffee_choice == 0:
            new_coffee = Espresso(size_choice)
        
        if coffee_choice == 1:
            new_coffee = Americano(size_choice)

        if coffee_choice == 2:
            new_coffee = Cappuccino(size_choice)

        if new_coffee.get_size() == 0:
            new_coffee_size = "small"

        if new_coffee.get_size() == 1:
            new_coffee_size = "medium"

        if new_coffee.get_size() == 2:
            new_coffee_size = "large"

        print("Factory", self.id, "produced a", new_coffee_size, new_coffee.get_name())
        self.distributor.add_to_inventory(new_coffee)

    def produce_coffees(self):
        for i in range(2):
        #while True:
            self.produce_random_coffee()

#Consumer
class User(Thread):
    def __init__(self, id, distributor):
        Thread.__init__(self)
        self.id = id
        self.distributor = distributor

    def run(self):
        self.consume_coffees()

    def consume_coffees(self):
        for i in range(2):
        #while True:
            coffee = self.distributor.take_from_inventory()
            print("Consumer", self.id, "consumed", coffee.get_name())


def main():
    # Common coffee distributor. It can hold N coffees at a time
    distributor_inventory_size = 10
    distributor = Distributor(distributor_inventory_size, [])

    #Start threads
    producer_number = 3
    producer_threads = []
    for i in range(producer_number):
        producer_thread = CoffeeFactory(i, distributor)
        producer_threads.append(producer_thread)
        producer_thread.start()

    consumer_number = 3
    consumer_threads = []
    for i in range(consumer_number):
        consumer_thread = User(i, distributor)
        consumer_threads.append(consumer_thread)
        consumer_thread.start()

    for i, thread in enumerate(consumer_threads):
        thread.join()

    for i, thread in enumerate(producer_threads):
        thread.join()
    

if __name__ == '__main__':
    main()
