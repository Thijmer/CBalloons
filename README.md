# CBalloons
A terminal program that displays ascending balloons.


## Nice balloons.
This program displays balloons that are made up of different characters and fly up your terminal.
This program can be piped into other programs (e.g. lolcat to give it nice colors). It resizes with the terminal if an amount of columns is not manually specified. This automatic size behaviour does not work when piping it into another program.
Tested on Linux. Probably compatible with POSIX compatible OSes. Certainly not compatible with windows (unless you use WSL of course).




## Flags:
  - -b:   Set characters the balloons are made up of. This string can contain multiple characters. A random one will be picked for each balloon.
  - -s:   The -b flag but this one sets what characters the strings on the bottom of the balloons are made up off.
  - -m:   Set the minimum radius of the balloons.
  - -r:   Set the maximum radius of the balloons.
  - -t:   Set the minimum string length (The strings on the bottom of the balloons, not programming strings).
  - -l:   Set the maximum string length.
  - -w:   The amount of seconds the program waits each line. This is a floating point number.
  - -c:   The amount of columns in the output. This is handy when the program outputs to a pipe.
  - -f:   The amount of rows that are between the balloons. Increase this number for less balloons. Defaults to 1.


## Building it
No-one said it was going to be normie friendly. But I think people with a little bit of experience with the terminal will be able to follow along.
#### Step one: download
Download the 'CBalloons.c' file.
#### Step two: Build
Open the terminal and go to the directory the 'CBalloons.c' file is in. Then type the following command to build it:
```shell
gcc CBalloons.c -o CBalloons -lm
```
The code above assumes you are using gcc as your compiler. This will be the case on almost every Linux distro.
#### Run it
```shell
./CBalloons
```
#### (Optional) Make CBalloons easily executable
If you want to make CBalloons executable from every directory, just by typing CBalloons, you'll have to move it to a directory like /usr/local/bin where Linux looks for executables when you type a command. You can do that by:
```shell
sudo mv CBalloons /usr/local/bin
```
Now you can start it from anywhere using 'CBalloons'.

## Cool tricks
This program is so much cooler if you combine it with other programs. This program is able to output to a [pipe](https://en.wikipedia.org/wiki/Pipeline_(Unix)). If you pipe it, you'll have to set the amount of columns manually using the -c flag or else it will default to 75. This is because pipes don't have a width, like ttys do.

#### With lolcat:
You'll have to install lolcat first for this example. Lolcat gives nice rainbow colors to text.
```shell
./CBalloons -c $(tput cols) | lolcat
```

#### Custom colors:
If you're not really into the rainbow stuff, you could of course give each balloon it's own color.
```shell

./CBalloons -b QJV -c $(tput cols) | sed "s/Q/$(tput setaf 1)Q$(tput sgr0)/g; s/J/$(tput setaf 2)J$(tput sgr0)/g; s/V/$(tput setaf 4)J$(tput sgr0)/g"
```
Here I use the -b flag here to specify the letters the balloons are gonna be made out of and I pipe the output into 'sed' (which pretty much every Linux distro will have because it is a POSIX standard). 'sed' replaces each of the letters with a color code, the letter itself and then the reset code. I substituted the 'tput setaf' command here because I think it is much clearer than with the color code itself.

#### Filled balloons:
You can also set both the foreground and the background of the balloons, using the same technique as the previous command. In this example, I didn't do it in one line, because the line would get too long for me to comprehend.
```shell
red=$(tput setaf 1; tput setab 1)
green=$(tput setaf 2; tput setab 2)
blue=$(tput setaf 4; tput setab 4)
reset=$(tput sgr0)
./CBalloons -c $(tput cols) -b TGV | sed "s/T/${red}T${reset}/g; s/G/${green}G${reset}/g; s/V/${blue}V${reset}/g"
```
The explaination of this one is pretty much the same as the previous one. The difference is that in this one, we saved the color codes that 'tput setaf' and 'tput setab' produced to variables before running the command itself. And the other difference is that we used both 'tput setab' (for the background) and 'tput setaf', while the first one only used 'tput setaf'.

#### Make your own combination:
You can do it!


## Contributing:
I'm open to contributions (=code & ideas)! Do you see a way to make the program more efficient? Please make sure to tell about it! This is my second C project, and I would love to get good at writing really efficient C code.

