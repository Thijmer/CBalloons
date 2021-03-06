#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h> //For commandline parameters
#include <sys/ioctl.h> //For detecting terminal columns
#include <unistd.h> //For detecting terminal columns

int columns = 75; //Terminal width
char custom_column_count = 0;
char running = 1;

int get_terminal_width() {
    struct winsize w;
    if (isatty(STDOUT_FILENO) && !custom_column_count) { //Dont resize when piping, because it will cause a zero devision.
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        columns = w.ws_col;
    }
    return columns;
}

int sq(int n) {
    return n*n;
}

void keyboard_interrupt(int dummy) {
    running = 0;
    printf("\n");
}

typedef struct balloons {
    int x;
    long y;
    int radius;
    int string_length;
    char balloon_char;
    char string_char;
} balloon;

void mainloop(int min_radius, int max_radius, int min_string_length, int max_string_length, char *balloon_chars, char* balloon_string_chars, char cycles_per_balloon, char nrofbloonchars, char nrofstringchars, float sleep_time) {
    long row = 0;
    char CharacterIsNTimesLongerInHeightThanInWidth = 3;
    int array_size;
    if (max_radius + max_string_length > 0) { // This if statement exists for the people who want to make balloons that don't exist. (Radius = 0 and string length = 0)
        array_size = (max_radius*CharacterIsNTimesLongerInHeightThanInWidth*2+max_string_length)/cycles_per_balloon;
    } else {
        array_size = 1;
    }
    balloon balloons[array_size];
    int creation_index = 0;
    {//Fill aray with balloons that aren't going to be rendered.
        balloon tempbloon;
        tempbloon.x = 0;
        tempbloon.y = 0;
        tempbloon.radius = 0;
        tempbloon.balloon_char = " "[0];
        tempbloon.string_char = " "[0];
        while (creation_index < array_size) {
            balloons[creation_index] = tempbloon;
            creation_index ++;
        }
    }
    creation_index = 0;
    while (running) {
        get_terminal_width();
        if (row % cycles_per_balloon == 0) {
            balloons[creation_index].x = random() % columns;
            balloons[creation_index].y = row + max_radius;
            int radius;
            if (max_radius > min_radius) {
                radius = min_radius + (random() % (max_radius - min_radius));
            } else {
                radius = max_radius;
            }
            
            balloons[creation_index].radius = radius;
            if (max_string_length > 0 && max_string_length > min_string_length) {
                balloons[creation_index].string_length = min_string_length + (random() % (max_string_length - min_string_length));
            } else {
                balloons[creation_index].string_length = max_string_length;
            }
            balloons[creation_index].balloon_char = balloon_chars[random() % nrofbloonchars];
            balloons[creation_index].string_char = balloon_string_chars[random() % nrofstringchars];
            creation_index ++;
            creation_index = creation_index % array_size;
        }

        char new_row [columns+1];
        for (int column = 0; column < columns; column++) {
            new_row[column] = *" ";
            for (int balloon_index = 0; balloon_index < array_size; balloon_index++) {
                balloon current_bloon = balloons[balloon_index];
                int distance_to_bloon = sqrt(sq(current_bloon.x-column)+sq(current_bloon.y-row)*CharacterIsNTimesLongerInHeightThanInWidth);
                char is_under_bloon = (current_bloon.x == column) && ((current_bloon.y+current_bloon.radius/CharacterIsNTimesLongerInHeightThanInWidth) < (row-1));
                if (is_under_bloon && ((row-1) - (current_bloon.y+current_bloon.radius/CharacterIsNTimesLongerInHeightThanInWidth)) < current_bloon.string_length+1) {
                    new_row[column] = current_bloon.string_char;
                    break;
                } else if (distance_to_bloon < current_bloon.radius) {
                    new_row[column] = current_bloon.balloon_char;
                    break;
                }
            }
        }
        new_row[columns] = *"\0";
        printf("%s\n", new_row);
        usleep(1000000*sleep_time);
        row ++;
    }
}

int main(int argc, char *argv[]) { //Parse parameters and start program
    int opt;
    float sleep_time = 0.05;
    int max_radius = 10;
    int min_radius = 1;
    int max_string_length = 10;
    int min_string_length = 1;
    char cycles_per_balloon = 1;
    char balloon_chars[100] = "0O8/:TB@$&+";
    char nrofbloonchars = 8;
    char balloon_string_chars[100] = "|";
    char nrofstringchars = 1;
    while((opt = getopt(argc, argv, ":b:s:m:r:l:t:w:c:f:")) != -1) {
        switch(opt) {
            case 'b':
                nrofbloonchars = strlen(optarg);
                if (nrofbloonchars == 0) {
                    fprintf(stderr, "Why are you running this program if you don't want to have any balloons?\n(Balloon text is set empty)\n");
                    return 128;
                }
                strcpy(balloon_chars, optarg);
                break;
            case 's':
                nrofstringchars = strlen(optarg);
                if (nrofstringchars == 0) {
                    fprintf(stderr, "Strings made from negative matter?\n(String text is set empty)\n");
                    return 128;
                }
                strcpy(balloon_string_chars, optarg);
                break;
            case 'm':
                min_radius = atoi(optarg);
                if (min_radius < 0) {fprintf(stderr, "A balloon can not have a negative radius. (unless it's made from dark matter of course)\n(m(=min radius) is %s)\n", optarg);return 128;}
                break;
            case 'r':
                max_radius = atoi(optarg);
                if (max_radius < 0) {fprintf(stderr, "A balloon can not have a negative radius. (unless it's made from dark matter of course)\n(r(=max radius) is %s)\n", optarg);return 128;}
                break;
            case 'l':
                max_string_length = atoi(optarg);
                if (max_string_length < 0) {fprintf(stderr, "A string can not have a negative length. (unless it's made from dark matter of course)\n(l(=max string length) is %s)\n", optarg);return 128;}
                break;
            case 't':
                min_string_length = atoi(optarg);
                if (min_string_length < 0) {fprintf(stderr, "A string can not have a negative length. (unless it's made from dark matter of course)\n(t(=min string length) is %s)\n", optarg);return 128;}
                break;
            case 'w':
                sleep_time = (float)strtod(optarg, NULL);
                if (sleep_time < 0) {fprintf(stderr, "Time travelling is not supported.\n(w(=tick duration in seconds) is %s)\n", optarg);return 128;}
                break;
            case 'f':
                cycles_per_balloon = atoi(optarg);
                if (cycles_per_balloon < 1) {fprintf(stderr, "This number has to be greater than 0.\n(f(=cycles per balloon) is %s)\n", optarg);return 128;}
                break;
            case 'c':
                columns = atoi(optarg);
                custom_column_count = 1;
                if (columns == 0) {
                    fprintf(stderr, "The width of the output cannot be set to zero.\n(c(=columns) is %s)\n", optarg);
                    return 128;
                } else if (columns < 0) {
                    fprintf(stderr, "The terminal width can not be negative. (unless your computer is made from dark matter of course)\n(c(=columns) is %s)\n", optarg);
                    return 128;
                }
                break;
            case ':':
                fprintf(stderr, "Option '%c' expects a value.\n", optopt);
                return 128;
                break;
            case '?':
                fprintf(stderr, "This program does not take an option %c.\n", optopt);
                return 128;
                break;
        }
    }
    
    get_terminal_width();
    if (!isatty(STDOUT_FILENO)) { //Make piping work
        setvbuf(stdout, NULL, _IONBF, 0);
    }
    signal(SIGINT, keyboard_interrupt);
    mainloop(min_radius, max_radius, min_string_length, max_string_length, balloon_chars, balloon_string_chars, cycles_per_balloon, nrofbloonchars, nrofstringchars, sleep_time);
    return 0;
}
