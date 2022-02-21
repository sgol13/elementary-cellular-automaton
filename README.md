# Elementary Cellular Automaton
This project is an implementation of an [elementary cellular automaton](https://en.wikipedia.org/wiki/Elementary_cellular_automaton). I've used [Allegro](https://liballeg.org/) library for graphic visualization and [argparse](https://github.com/cofyc/argparse) for command line argument parsing. The program allows you to experiment with various transition rules and other initial parameters which affect the automaton. In the images below you can see the exemplary results for transition rules, respectively: 105, 22, 125, 129.

<img src="https://i.postimg.cc/CM6LTXPp/example.png" width="1000">

## Compilation
If you want to compile the program, you need to have Allegro library installed. You can download it [here](https://liballeg.org/download.html). 
Then you can build the program using a prepared bash script. Just type:
```
./build
```
The executable `cellular_automaton` will appear in your current directory.

## Usage
There are four initial parameters for the cellular automaton. Two of them must be specified for each simulation:
* **rule** - the number 0-255 which determines how the cell's state depends on its previous state and the previous states of its two immediate neighbours. Here you can find the list of rules which are notable from mathematical point of view: [transition rules](https://en.wikipedia.org/wiki/Elementary_cellular_automaton#Single_1_histories).
* **population size** - the number of cells in the first iteration which are set to 1. They are selected randomly.

The other two parameters are optional because they have default values:
* **iterations** - the number of simulation iterations. (default 50)
* **columns** - the number of columns in a simulation. This is also the maximum size of a population. (default 80)


The simulation can be started like in the example below.
```
./cellular_automaton 167 20
```

The full manual (can be accessed also by using `-h` option):
```
Usage: ./cellular_automaton RULE POPULATION [options]

Visual simulation of an elementary cellular automaton.


positional arguments:
    RULE                      transition rule, [0, 255]
    POPULATION                size of an initial population, [0, columns]

optional arguments:
    -i, --iterations=<int>    number of simulation iterations, [10, 80], default 50
    -c, --columns=<int>       number of columns, [30, 150], default 80
    -h, --help                show this help message and exit
```

## License
This project is under MIT [license](LICENSE).
