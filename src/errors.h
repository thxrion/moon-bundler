#include <stdio.h>
#include <errno.h>

void print_getcwd_errors(void) {
    printf("An error occurred in the getcwd() call.\n");

    switch (errno) {
        case EACCES: {
            printf ("The process did not have read or search permission on some component of the working directory's path name.\n");
            break;
        }
        case EINVAL: {
            printf ("Size is less than or equal to zero.\n");
            break;
        }
        case EIO: {
            printf ("An input/output error occurred.\n");
            break;
        }
        case ENOENT: {
            printf ("A component of the working directory's path name does not exist.\n");
            break;
        }
        case ENOTDIR: {
            printf ("A directory component of the working directory's path name is not really a directory.\n");
            break;
        }
        case ERANGE: {
            printf ("size is greater than 0, but less than the length of the working directory's path name, plus 1 for the terminating NULL.\n");
            break;
        }
    }

    return;
}

void print_target_not_found_error(void) {
    printf("Target not specified, please use --target <name>\n");
    return;
}