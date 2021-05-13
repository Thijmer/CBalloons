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

void keyboard_interrupt(int dumy) {
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
    char CharacterIsNTimesLongerInHeightThanInWidth = 2;
    int array_size = (max_radius*2+max_string_length)/cycles_per_balloon;
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
            balloons[creation_index].string_length = min_string_length + (random() % (max_string_length - min_string_length));
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
                char is_under_bloon = current_bloon.x == column && (current_bloon.y+current_bloon.radius/CharacterIsNTimesLongerInHeightThanInWidth) < row;
                if (is_under_bloon && (distance_to_bloon-current_bloon.radius/CharacterIsNTimesLongerInHeightThanInWidth) < current_bloon.string_length) {
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
    while((opt = getopt(argc, argv, ":b:s:m:r:l:t:w:c:")) != -1) {
        switch(opt) {
            case 'b':
                nrofbloonchars = strlen(optarg);
                strcpy(balloon_chars, optarg);
                break;
            case 's':
                nrofstringchars = strlen(optarg);
                strcpy(balloon_string_chars, optarg);
                break;
            case 'm':
                min_radius = atoi(optarg);
                break;
            case 'r':
                max_radius = atoi(optarg);
                break;
            case 'l':
                max_string_length = atoi(optarg);
                break;
            case 't':
                min_string_length = atoi(optarg);
                break;
            case 'w':
                sleep_time = (float)strtod(optarg, NULL);
                break;
            case 'c':
                columns = atoi(optarg);
                custom_column_count = 1;
                if (columns == 0) {
                    fprintf(stderr, "The width of the output cannot be set to zero.\n");
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