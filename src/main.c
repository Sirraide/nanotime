#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define __USE_POSIX199309
#include <time.h>

#define R "\033[31m"
#define G "\033[32m"
#define Y "\033[33m"
#define Q "\033[m"

#define SHRINK(a, b)      \
	do                    \
		if (a > 999) {    \
			b = a / 1000; \
			a %= 1000;    \
		}                 \
	while (0)

void print_time(const char *restrict format, time_t time) {
	static char buf[2048];

	size_t written = strftime(buf, sizeof buf, format, gmtime(&time));
	buf[written]   = 0;
	printf("%s\n", buf);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, R "Error: Missing argument\n" Q);
		exit(1);
	}

	struct timespec start, end;
	time_t			program_start = time(NULL);
	int				start_ret, end_ret;

	int pid = fork();
	if (pid < 0) {
		perror(R "fork()");
		printf("%s", Q);
		exit(1);
	}

	if (pid == 0) {
		execvp(argv[1], argv + 1);
		perror(R "execvp()");
		printf("%s", Q);
		exit(1);
	} else start_ret = clock_gettime(CLOCK_MONOTONIC, &start);

	int wstatus, pid_ret;
	do pid_ret = waitpid(pid, &wstatus, 0);
	while (!WIFEXITED(wstatus));

	end_ret = clock_gettime(CLOCK_MONOTONIC, &end);

	if (start_ret < 0 || end_ret < 0) {
		perror(R "clock_gettime()");
		printf("%s", Q);
		exit(1);
	}

	if (pid_ret != pid) {
		perror(R "waitpid() returned the wrong pid");
		printf("%s", Q);
		exit(1);
	}

	time_t program_end = time(NULL);

	size_t ns, us = 0, ms = 0, s = 0;

	ns = (size_t) (end.tv_nsec - start.tv_nsec);
	SHRINK(ns, us);
	SHRINK(us, ms);
	SHRINK(ms, s);
	s += (size_t) (end.tv_sec - start.tv_sec);

	print_time(Y "Started:" G "  %c (%s)", program_start);
	print_time(Y "Finished:" G " %c (%s)", program_end);
	printf(Y "s:" G " %zu" Y ", ms:" G " %zu" Y ", us:" G " %zu" Y ", ns:" G " %zu\n",
		s, ms, us, ns);
}
