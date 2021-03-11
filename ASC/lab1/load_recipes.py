"""
	Bonus task: load all the available coffee recipes from the folder 'recipes/'
	File format:
		first line: coffee name
		next lines: resource=percentage

	info and examples for handling files:
		http://cs.curs.pub.ro/wiki/asc/asc:lab1:index#operatii_cu_fisiere
		https://docs.python.org/3/library/io.html
		https://docs.python.org/3/library/os.path.html
"""

# Coffee examples
ESPRESSO = "espresso"
AMERICANO = "americano"
CAPPUCCINO = "cappuccino"

# Resources examples
ALL = "all"
WATER = "water"
COFFEE = "coffee"
MILK = "milk"

RECIPES_FOLDER = "recipes/"

#Get recipies from files
def get_recipie(new_coffee_type):
	file_name = RECIPES_FOLDER + new_coffee_type + ".txt"
	new_coffee_recipie = {}

	with open(file_name) as recipie_file:
		recipie_file.readline().strip('\n')
		for line in recipie_file:
			tokens = line.split('=')
			new_coffee_recipie[tokens[0]] = int(tokens[1])

	return new_coffee_recipie