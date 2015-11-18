/**      1         2         3         4         5         6         7         8
 * 45678901234567890123456789012345678901234567890123456789012345678901234567890
 * 
 * pacman-banner: Silly test implementation of cprintf, v.0.0.1
 *    Johnathan Graham McKnight <akoimeexx@gmail.com>
 * 
 * compile: make
 * 
 *
 * Output (using #define PBRUSH[_EX], GBRUSH[_EX]):
     #####           #####           #####           #####           #####     
  ###########     ###########     ###########     ###########     ###########  
 ##########      ### ##### ###   ### ##### ###   ### ##### ###   ### ##### ### 
#########        ###  ####  ##   ###  ####  ##   ###  ####  ##   ###  ####  ## 
#######         ####  ####  ### ####  ####  ### ####  ####  ### ####  ####  ###
#########       ############### ############### ############### ###############
 ##########     ############### ############### ############### ###############
  ###########   ### ### ### ### ### ### ### ### ### ### ### ### ### ### ### ###
     #####      ##   #   #   ## ##   #   #   ## ##   #   #   ## ##   #   #   ##
 *
 * 
 * 
 * Copyright (c) 2015, Johnathan Graham McKnight
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE 1
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 201304L
#endif

#define AUTHOR "Johnathan Graham McKnight <akoimeexx@gmail.com>"
#define VERSION "0.0.1"
#define PBRUSH "#"
#define GBRUSH PBRUSH
#define PBRUSH_EX "■"
#define GBRUSH_EX PBRUSH_EX

#include <stdio.h>
#include <stdlib.h>
/* Command line option parsing made "easy" */
#include <getopt.h>
/* basename, et. al. */
#include <libgen.h>


/* Developer-made libraries */
#include "./functions.h"

static int nocolor_flag = 0;
static int scared_flag = 0;
static struct option long_options[] = {
    {"basic", no_argument, NULL, 'b'}, 
    {"ghost", required_argument, NULL, 'g'}, 
    {"nocolor", no_argument, NULL, 'n'}, 
    {"pacman", required_argument, NULL, 'p'}, 
    {"scared", no_argument, NULL, 's'}, 
    {"version", no_argument, NULL, 'v'}, 
    /**
     * Magic numbers to denote option struct array termination. Reference: 
     * http://www.gnu.org/software/libc/manual/html_node/Getopt.html
     */
    {0, 0, 0, 0}
};
// Art sets
char pacman[9][15] = {
    { 0,0,0,0,0,1,1,1,1,1,0,0,0,0,0 },
    { 0,0,1,1,1,1,1,1,1,1,1,1,1,0,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,0,0,0,0 },
    { 1,1,1,1,1,1,1,1,1,0,0,0,0,0,0 },
    { 1,1,1,1,1,1,1,0,0,0,0,0,0,0,0 },
    { 1,1,1,1,1,1,1,1,1,0,0,0,0,0,0 },
    { 0,1,1,1,1,1,1,1,1,1,1,0,0,0,0 },
    { 0,0,1,1,1,1,1,1,1,1,1,1,1,0,0 },
    { 0,0,0,0,0,1,1,1,1,1,0,0,0,0,0 },
};
char ghost[9][15] = {
    { 0,0,0,0,0,1,1,1,1,1,0,0,0,0,0 },
    { 0,0,1,1,1,1,1,1,1,1,1,1,1,0,0 },
    { 0,1,1,1,2,3,1,1,1,1,2,3,1,1,0 },
    { 0,1,1,1,2,3,1,1,1,1,2,3,1,1,0 },
    { 1,1,1,1,2,3,1,1,1,1,2,3,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    { 1,1,1,0,1,1,1,0,1,1,1,0,1,1,1 },
    { 1,1,0,0,0,1,1,0,1,1,0,0,0,1,1 },
};
char scared[9][15] = {
    { 0,0,0,0,0,1,1,1,1,1,0,0,0,0,0 },
    { 0,0,1,1,1,1,1,1,1,1,1,1,1,0,0 },
    { 0,1,1,1,2,2,1,1,1,2,2,1,1,1,0 }, 
    { 0,1,1,1,2,2,1,1,1,2,2,1,1,1,0 }, 
    { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }, 
    { 1,1,2,2,1,1,2,2,2,1,1,2,2,1,1 }, 
    { 1,2,1,1,2,2,1,1,1,2,2,1,1,2,1 }, 
    { 1,1,1,0,1,1,1,0,1,1,1,0,1,1,1 }, 
    { 1,1,0,0,0,1,1,0,1,1,0,0,0,1,1 }, 
};

void draw(char *pacbrush, char *ghostbrush) {
    int pigment;
    for(int rows=0; rows<9; rows++) {
        if(scared_flag == 0) {
            // The Ghosts: Red, Pink, Cyan, Orange [mimicked with 
            //             Yellow and regular text], white eyes, blue irises
            // 0 - Blinky, 1 - Pinky, 2 - Inky, 3 - Clyde
            char colors[4] = { FG_YELLOW, FG_CYAN, FG_MAGENTA, FG_RED };
            for(int ghosts=0; ghosts<sizeof(colors); ghosts++) {
                pigment=colors[ghosts];
                for (int chars=0; chars<15; chars++) {
                    char t_format = TXT_BOLD;
                    if(colors[ghosts] == FG_YELLOW) { t_format = TXT_NULL; }
                    if(ghost[rows][chars] == 1 && nocolor_flag == 0) {
                        cprintf(pigment, BG_NULL, t_format, ghostbrush);
                    } else if(ghost[rows][chars] == 2 && nocolor_flag == 0) {
                        cprintf(FG_WHITE, BG_NULL, TXT_BOLD, ghostbrush);
                    } else if(ghost[rows][chars] == 3 && nocolor_flag == 0) {
                        cprintf(FG_BLUE, BG_NULL, TXT_BOLD, ghostbrush);
                    } else if(ghost[rows][chars] == 1) {
                        cprintf(0, BG_NULL, t_format, ghostbrush);
                    } else {
                        printf(" ");
                    }
                }
                printf(" ");
            }
        }
        // Pac-Man: Yellow, Bold, no defining color differences
        for (int chars=0; chars<15; chars++) {
            if(pacman[rows][chars] == 1) {
                pigment = FG_YELLOW;
                if(nocolor_flag == 1) { pigment = 0; }
                cprintf(pigment, BG_NULL, TXT_BOLD, pacbrush);
            } else {
                printf(" ");
            }
        }
        if(scared_flag != 0) {
            printf(" ");
            for(int ghosts=0; ghosts<4; ghosts++) {
                for (int chars=0; chars<15; chars++) {
                    if(scared[rows][chars] == 1 && nocolor_flag == 0) {
                        cprintf(FG_BLUE, BG_NULL, TXT_NULL, ghostbrush);
                    } else if(scared[rows][chars] == 2 && nocolor_flag == 0) {
                        cprintf(FG_WHITE, BG_NULL, TXT_BOLD, ghostbrush);
                    } else if(scared[rows][chars] == 1) {
                        cprintf(0, BG_NULL, TXT_NULL, ghostbrush);
                    } else {
                        printf(" ");
                    }
                }
                printf(" ");
            }
        }
        printf("\n");
    }
}
int main(int argc, char *argv[]) {
    int optc=0, optind=0;
    char *bin, *pb, *gb;
    
    pb = PBRUSH_EX;
    gb = GBRUSH_EX;
    // If invoked with no arguments, don't continue past this point
    if(argc != 1) {
        // Option-checking loop
        while((optc = getopt_long(argc, argv, ":bg:np:sv", long_options, &optind
        )) != -1) {
            switch (optc) {
                case 'b':
                    pb = PBRUSH;
                    gb = GBRUSH;
                    break;
                case 'g':
                    gb = optarg;
                    break;
                case 'n':
                    nocolor_flag = 1;
                    break;
                case 'p':
                    pb = optarg;
                    break;
                case 's':
                    scared_flag = 1;
                    break;
                case 'v':
                    bin = basename(argv[0]);
                    printf("%s, v.%s\n  written by %s\n", bin, VERSION, AUTHOR);
                    exit(EXIT_SUCCESS);
                default:
                    bin = basename(argv[0]);
                    printf(
"usage: %s [--nocolor][--scared][--ghost=ascii][--pacman=ascii]\n", bin);
                    printf("       %s [--basic][--nocolor][--scared]\n", bin);
                    printf("       %s [-ns][-g #][-p #]\n", bin);
                    printf("       %s [-bns]\n", bin);
                    printf("       %s [-v]\n", bin);
                    exit(EXIT_FAILURE);
                    break;
            }
        }
    }
    draw(pb, gb);
    exit(EXIT_SUCCESS);
}
