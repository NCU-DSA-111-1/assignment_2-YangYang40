# Shogi in C with replay mode
DS Assignment 2 (Files are at the "master" branch)


## Usage

## Compile & Run
```sh
# Compile
make all

# Run
To create new game:
cd bin
./shogi -n -s filename.txt

To replay a old game:
cd bin
./shogi -1 filename.txt
```

## -n -s(create new game)

### Blue play first

### Blue player move

1. Enter the coordinates of the piece you want to move.  
2. Enter the place you want to put the piece onto.

3. Enter what you want to do after this hand



```sh
# Blue
Player B play!
Please enter the coordinate of the piece that you want to move:
1 3

Please enter which coordinate you want this piece to move to 
1 4

Did you want to regret this hand? Or do you want to save the board right here?
To regret type 0,to continue playing type p, to save type s:
p
```

### Red player move

1. Enter the coordinates of the piece you want to move.  
2. Enter the place you want to put the piece onto.

3. Enter what you want to do after this hand




```sh
# Red
Player R play!
Please enter the coordinate of the piece that you want to move:
5 7

Please enter which coordinate you want this piece to move to 
5 6

Did you want to regret this hand? Or do you want to save the board right here?
To regret type 0,to continue playing type p, to save type s:
p
```

### Enter 0 - continuous regret mode
```sh
# To enter
Did you want to regret this hand? Or do you want to save the board right here?
To regret type 0,to continue playing type p, to save type s:
0

#type Y for yes N for no
Player '(player color)' do you want to regret this hand of yours?
N

#if type N(exit this mode) - choose your next step
Do you like to continue playing or save the board?(Type c for continue or s for save)
c
```

## -1(replay old game)

### Enter 'f' or 'b' to see next round or previous hand.
```sh
#Enter inputs you can put in
Enter 'f' to go to the next hand.
Enter 'b' to go to the previous hand.
Enter 'e' to exit.