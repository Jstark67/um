#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"

extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void build_halt_test(Seq_T instructions);
extern void build_verbose_halt_test(Seq_T instructions);
extern void add_two_numbers(Seq_T stream);
extern void print_add(Seq_T stream);

typedef struct Mem_T {
        Seq_T seg_mem;
        Seq_T unmapped;
        uint32_t maxID;
} *Mem_T

/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes
        flo instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
        { "halt",         NULL, "", build_halt_test },
        { "halt-verbose", NULL, "", build_verbose_halt_test },
        { "add", NULL, "", add_two_numbers},
        { "print-six", NULL, "", print_add}
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
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
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

/* Tests the basic functionalities of map and unmap, will require more tests since this is just a prototype */
void umemory_map_unmap_test(Mem_T mem)
{
        /* check whether mapping returns consecutive ids and allocated correct space*/
        uint32_t id;
        uint32_t size = 64; /* random number to test with */
        for (int i = 0; i < 1000; i++) {
                id = Umemory_map(mem, size);
                assert(id == i + 1);
                assert(UArray_size(Seq_get(mem->seg_mem, id)) == size);
        }
        /* call unmap with an invalid id */
        uint32_t unmap_id = 114514;
        Umemory_unmap(mem, unmap_id); /* Should fail the program */
        /* unmap a segment in the middle and call map again, check whether the two segment indices are the same */
        for (int i = 250; i < 500; i++) {
                unmap_id = i;
                Umemory_unmap(mem, unmap_id);
                id = Umemory_map(mem, size);
                assert(unmap_id == id);
        }
}

/* Tests the basic functionalities of load and store, will require more tests since this is just a prototype */
void umemory_load_store_test(Mem_T mem)
{
        uint32_t size = 64; /* random number to test with */
        UArray_T cur_program = Seq_get(mem->seg_mem, 0);
        /* assuming 10 consecutive slots in memory has been mapped */
        
        for (uint32_t i = 1; i <= 10; i++) {
                for (uint32_t j = 0; j < size; j++) {
                        /* test whether value stored by umemory_store can be fetched by umemory_load independently */
                        uint32_t random = generate_random_uint32(); /* some helper function that generates a random uint32 value */
                        Umemory_store(mem, i, j, random);
                        uint32_t load_value = Umemory_load(mem, i, j);
                        assert(load_value == random);

                        /* test whether Umemory_loadprogram successfully replaces the program Uarray */
                        UArray_T temp = Umemory_loadprogram(mem, i, j);
                        assert(temp != cur_program);
                }

        }
        
        
        
}


