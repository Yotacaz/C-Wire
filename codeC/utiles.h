#pragma once

#define RED "\x1B[0;31m"
#define GRN "\x1B[0;32m"
#define YEL "\x1B[0;33m"
#define BLU "\x1B[0;34m"
#define MAG "\x1B[0;35m"
#define CYN "\x1B[0;36m"
#define WHT "\x1B[0;37m"

// background colors
#define BG_RED "\x1B[41m"
#define BG_GRN "\x1B[42m"
#define BG_YEL "\x1B[43m"
#define BG_BLU "\x1B[44m"
#define BG_MAG "\x1B[45m"
#define BG_CYN "\x1B[46m"
#define BG_WHT "\x1B[47m"

// bold text colors
#define BOLD_RED "\x1B[1;31m"
#define BOLD_GRN "\x1B[1;32m"
#define BOLD_YEL "\x1B[1;33m"
#define BOLD_BLU "\x1B[1;34m"
#define BOLD_MAG "\x1B[1;35m"
#define BOLD_CYN "\x1B[1;36m"
#define BOLD_WHT "\x1B[1;37m"

// underline
#define UNDERLINE "\x1B[4m"

#define RESET "\x1B[0m"
void test();
int min2(int a, int b);
int max2(int a, int b);
int max3(int a, int b, int c);
int min3(int a, int b, int c);

void verifier(bool condition, char * msg_err);
