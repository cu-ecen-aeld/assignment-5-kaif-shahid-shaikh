#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    openlog("kaifs-writer", LOG_PID, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "Usage: writer <file_path> <string_to_write>");
        closelog();
        return EXIT_FAILURE;
    }

    const char *filepath = argv[1];
    const char *text     = argv[2];

    syslog(LOG_DEBUG, "Writing %s to %s", text, filepath);

    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        syslog(LOG_ERR, "Error opening %s: %m", filepath);
        closelog();
        return EXIT_FAILURE;
    }

    if (fprintf(fp, "%s", text) < 0) {
        syslog(LOG_ERR, "Error writing to %s: %m", filepath);
        fclose(fp);
        closelog();
        return EXIT_FAILURE;
    }

    if (fclose(fp) != 0) {
        syslog(LOG_ERR, "Error closing %s: %m", filepath);
        closelog();
        return EXIT_FAILURE;
    }

    closelog();
    return EXIT_SUCCESS;
}

