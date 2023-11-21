#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);


extern void test_halt(Seq_T stream);
extern void test_halt_verbose(Seq_T stream);
extern void test_out(Seq_T stream);
extern void test_add(Seq_T stream);
extern void test_mult(Seq_T stream);
extern void test_add_edge(Seq_T stream);
extern void test_div(Seq_T stream);
extern void test_mov(Seq_T stream);
extern void test_nand(Seq_T stream);
extern void test_segl(Seq_T stream);
extern void test_segs(Seq_T stream);
extern void test_unmap(Seq_T stream);
extern void test_loadp(Seq_T stream);
extern void test_map(Seq_T stream);

typedef struct Mem_T {
        Seq_T seg_mem;
        Seq_T unmapped;
        uint32_t maxID;
} *Mem_T;

/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes
        flo instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
        {"test_halt", NULL, "", test_halt},
        {"test_halt_verbose", NULL, "", test_halt_verbose},
        {"test_out", NULL, "Hello\n", test_out},
        {"test_add", NULL, "6\n", test_add},
        {"test_mult", NULL, "0à\n", test_mult},
        {"test_add_edge", NULL, "0\n", test_add_edge},
        {"test_div", NULL, "0\n", test_div},
        {"test_mov", NULL, "01\n", test_mov},
        {"test_nand", NULL, "01\n", test_nand},
        {"test_segl", NULL, "V\n", test_segl},
};

  
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("../test/%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("../test/%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("../test/%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
