"""
A command-line controlled coffee maker.
"""

import sys
import load_recipes as recipes

"""
Implement the coffee maker's commands. Interact with the user via stdin and print to stdout.

Requirements:
    - use functions
    - use __main__ code block
    - access and modify dicts and/or lists
    - use at least once some string formatting (e.g. functions such as strip(), lower(),
    format()) and types of printing (e.g. "%s %s" % tuple(["a", "b"]) prints "a b"
    - BONUS: read the coffee recipes from a file, put the file-handling code in another module
    and import it (see the recipes/ folder)

There's a section in the lab with syntax and examples for each requirement.

Feel free to define more commands, other coffee types, more resources if you'd like and have time.
"""

"""
Tips:
*  Start by showing a message to the user to enter a command, remove our initial messages
*  Keep types of available coffees in a data structure such as a list or dict
e.g. a dict with coffee name as a key and another dict with resource mappings (resource:percent)
as value
"""

# Commands
EXIT = "exit"
LIST_COFFEES = "list"
MAKE_COFFEE = "make"  #!!! when making coffee you must first check that you have enough resources!
HELP = "help"
REFILL = "refill"
RESOURCE_STATUS = "status"
commands = [EXIT, LIST_COFFEES, MAKE_COFFEE, REFILL, RESOURCE_STATUS, HELP]

# Coffee examples
ESPRESSO = "espresso"
AMERICANO = "americano"
CAPPUCCINO = "cappuccino"
COFFEE_TYPES = [ESPRESSO, AMERICANO, CAPPUCCINO]

# Resources examples
ALL = "all"
WATER = "water"
COFFEE = "coffee"
MILK = "milk"

# Coffee maker's resources - the values represent the fill percents
RESOURCES = {WATER: 10, COFFEE: 10, MILK: 10}

#See all available commands
def get_help():
    print("Available Commands: \n" +
            "exit\n" +
            "list\n" +
            "make\n" +
            "help\n" +
            "refill\n" +
            "status")

#See all available resources
def get_status():
    for ingredient, amount in RESOURCES.items():
        print("%s: %d%%" % (ingredient, amount))

#See all available recipies
def list_coffees():
    for coffee in COFFEE_TYPES:
        print(str(coffee) + ", ", end="")
    print()

#Handle refill command
def refill():
    print("Which resource? Type 'all' for refilling everything")

    new_resource = sys.stdin.readline().strip("\n")
    if new_resource == ALL:
        for resource, amount in RESOURCES.items():
            if (amount != 100):
                RESOURCES[resource] = 100
    else:
        for resource, amount in RESOURCES.items():
            if (resource == new_resource and amount != 100):
                RESOURCES[resource] = 100

    get_status()


# Check if enough resources for the coffee
def enough_resources(recipie):
    for resource, amount in recipie.items():
        if (RESOURCES[resource] < amount):
            print ("Not enough resources. Needs refill")
            return False
    return True

#Handle make coffee command
def make_coffee():
    print("Which coffee?")
    finished = False
    found = False
    new_coffee_type = sys.stdin.readline().strip("\n")
    # Get the recipe for the coffee type
    for coffee_type in COFFEE_TYPES:
        if (new_coffee_type == coffee_type):
            found = True
            recipie = recipes.get_recipie(new_coffee_type)
            #Make the coffee
            if (enough_resources(recipie)):
                finished = True
                for resource, amount in recipie.items():
                    RESOURCES[resource] -= amount

    # Handle unknown recipies
    if (not found):
        print("Unknown recipie")
        list_coffees()

    if (finished):
        print("Here's your " + new_coffee_type)

"""
Example result/interactions:

I'm a smart coffee maker
Enter command:
list
americano, cappuccino, espresso
Enter command:
status
water: 100%
coffee: 100%
milk: 100%
Enter command:
make
Which coffee?
espresso
Here's your espresso!
Enter command:
refill
Which resource? Type 'all' for refilling everything
water
water: 100%
coffee: 90%
milk: 100%
Enter command:
exit
"""

def main():
    print("I'm a simple coffee maker")

    #Handle commands
    while True:
        print("Give Command:")
        command = sys.stdin.readline().strip("\n")
        if (command == EXIT):
            break
        elif (command == RESOURCE_STATUS):
            get_status()
        elif (command == LIST_COFFEES):
            list_coffees()
        elif (command == REFILL):
            refill()
        elif (command == MAKE_COFFEE):
             make_coffee()
        elif (command == HELP):
            get_help()
        else:
            print("Invalid Command")
            get_help()

    print("Bye ^_^")

if __name__ == "__main__":
    main()
