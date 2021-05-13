# CBalloons
A program that displays ascending balloons.


## Nice balloons to show to your mother.
Baloons made up of different characters that fly up your terminal. Yess niceness in the terminal.
This program can be piped into other programs (e.g. lolcat to give it nice colors). It resizes with the terminal if an amount of columns is not manually specified. This resizing behaviour does not work when piping it into another program.
Tested on Linux. Maybe compatible with POSIX compatible OSes. Certainly not compatible with windows (unless you use WSL of course).




## Flags:
  - -b:   Set characters the balloons are made up of. This string can contain multiple characters. A random one will be picked for each balloon.
  - -s:   The -b flag but this one sets what characters the strings on the bottom of the balloons are made up off.
  - -m:   Set the minimum radius of the balloons.
  - -r:   Set the maximum radius of the balloons.
  - -t:   Set the minimum string length (The strings on the bottom of the balloons, not programming strings).
  - -l:   Set the maximum string length.
  - -w:   The amount of seconds the program waits each line. This is a floating point number.
  - -c:   The amount of columns in the output. This is handy when the program outputs to a pipe.


## Building it
No-one said it was going to be normie friendly. But I think people with a little bit of Linux terminal experience will be able to follow along.
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
