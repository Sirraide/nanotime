#include <chrono>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <sys/wait.h>
#include <unistd.h>

#define R "\033[31m"
#define G "\033[32m"
#define Y "\033[33m"
#define Q "\033[m"

using namespace std::literals;
namespace chrono = std::chrono;

int main(int argc, char** argv) {
    if (argc < 2) {
        fmt::print(stderr, R "Error: Missing argument\n" Q);
        std::exit(1);
    }

    auto start = chrono::high_resolution_clock::now();

    auto pid = vfork();
    if (pid < 0) {
        std::perror(R "vfork()");
        fmt::print(stderr, Q);
        std::exit(1);
    }

    if (pid == 0) {
        ::execvp(argv[1], argv + 1);
        std::perror(R "execvp()");
        fmt::print(Q);
        std::exit(1);
    }

    int status{};
    do waitpid(pid, &status, 0);
    while (not WIFEXITED(status) and not WIFSIGNALED(status));

    auto end = chrono::high_resolution_clock::now();

    if (WIFSIGNALED(status)) fmt::print(R "Process terminated by signal {}\n" Q, WTERMSIG(status));

    fmt::print(Y "Started:" G "  {:%F %T UTC}\n", start);
    fmt::print(Y "Finished:" G " {:%F %T UTC}\n", end);
    fmt::print(
        Y "s:" G " {}" Y ", " G "ms:" Y " {}" Y ", " G "us:" Y " {}" Y ", " G "ns:" Y " {}\n",
        chrono::duration_cast<chrono::seconds>(end - start).count(),
        chrono::duration_cast<chrono::milliseconds>(end - start).count() % 1000,
        chrono::duration_cast<chrono::microseconds>(end - start).count() % 1000,
        chrono::duration_cast<chrono::nanoseconds>(end - start).count() % 1000
    );

    if (WIFSIGNALED(status)) std::exit(128 + WTERMSIG(status));
    if (WEXITSTATUS(status) != 0) std::exit(WEXITSTATUS(status));
}
