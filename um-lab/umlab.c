/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <uarray.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

static inline Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction instruction = 0;
        instruction = Bitpack_newu(instruction,4,28,op);
        instruction = Bitpack_newu(instruction,3,6,ra);
        instruction = Bitpack_newu(instruction,3,3,rb);
        instruction = Bitpack_newu(instruction,3,0,rc);
        return instruction;
}
static inline Um_instruction loadval(unsigned ra, unsigned val)
{
        uint64_t instruction = 0;

        /* put bits in locations specified */
        instruction = Bitpack_newu(instruction, 4, 28, 13);
        instruction = Bitpack_newu(instruction, 3, 25, ra);
        instruction = Bitpack_newu(instruction, 25, 0, val);

        return (uint32_t)instruction;
}


/* Wrapper functions for each of the instructions */



typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction cmov(Um_register a, Um_register b, Um_register c) 
{
        return three_register(CMOV, a, b, c);
}
static inline Um_instruction segl(Um_register a, Um_register b, Um_register c) 
{
        return three_register(SLOAD, a, b, c);
}
static inline Um_instruction segs(Um_register a, Um_register b, Um_register c) 
{
        return three_register(SSTORE, a, b, c);
}
static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}
static inline Um_instruction mul(Um_register a, Um_register b, Um_register c) 
{
        return three_register(MUL, a, b, c);
}
static inline Um_instruction divide(Um_register a, Um_register b, Um_register c) 
{
        return three_register(DIV, a, b, c);
}
static inline Um_instruction nand(Um_register a, Um_register b, Um_register c) 
{
        return three_register(NAND, a, b, c);
}
static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}
static inline Um_instruction map(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}static inline Um_instruction unmap(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}
static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}
static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}
static inline Um_instruction loadP(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}



/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}


/* Unit tests for the UM */

void test_halt(Seq_T stream)
{
        append(stream, halt());
}

void test_halt_verbose(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

void test_out(Seq_T stream)
{
        append(stream, loadval(r0, 'H'));
        append(stream, output(r0));
        append(stream, loadval(r0, 'e'));
        append(stream, output(r0));
        append(stream, loadval(r0, 'l'));
        append(stream, output(r0));
        append(stream, loadval(r0, 'l'));
        append(stream, output(r0));
        append(stream, loadval(r0, 'o'));
        append(stream, output(r0));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_add(Seq_T stream)
{

        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_mult(Seq_T stream)
{
        append(stream, loadval(r1, 3));
        append(stream, loadval(r2, 16));
        append(stream, mul(r3, r1, r2));
        append(stream, output(r3));
        append(stream, loadval(r1, 0x1111112));
        append(stream, loadval(r2, 240));
        append(stream, mul(r3, r1, r2));
        append(stream, loadval(r2, 2));
        append(stream, divide(r3, r3, r2));
        append(stream, output(r3));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_add_edge(Seq_T stream)
{

        append(stream, loadval(r1, 0x1ffffff));
        append(stream, loadval(r2, 128));
        append(stream, mul(r3, r1, r2));
        append(stream, loadval(r1, 176));
        append(stream, add(r3, r3, r1));
        append(stream, output(r3));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_div(Seq_T stream)
{

        append(stream, loadval(r1, 96));
        append(stream, loadval(r2, 2));
        append(stream, divide(r3, r1, r2));
        append(stream, output(r3));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_mov(Seq_T stream)
{

        append(stream, loadval(r1, 1));
        append(stream, loadval(r2, 49));
        append(stream, loadval(r3, 48));
        append(stream, loadval(r4, 0));
        append(stream, cmov(r3, r2,r4));
        append(stream, output(r3));
        append(stream, cmov(r3, r2, r1));
        append(stream, output(r3));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_nand(Seq_T stream)
{
        append(stream, loadval(r1, 0xcd));
        append(stream, loadval(r2, 0xff));
        append(stream, nand(r1, r3, r2));
        append(stream, loadval(r2, 0x800000));
        append(stream, mul(r3, r3,r2));
        append(stream, divide(r3, r3,r2));
        append(stream, output(r3));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_in(Seq_T stream)
{
        append(stream, input(r0));
        append(stream, input(r1));
        append(stream, input(r2));
        append(stream, input(r3));
        append(stream, input(r4));
        append(stream, output(r0));
        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, output(r4));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_segl(Seq_T stream)
{
        append(stream, loadval(r1, 8));
        append(stream, loadval(r2, 0x1fffff8));
        append(stream, add(r2, r2, r1));
        append(stream, segl(r1, r3,r1));
        append(stream, divide(r3, r1,r2));
        append(stream, output(r3));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}
void test_segs(Seq_T stream)
{
        append(stream, loadval(r2, 0x1fffffa));
        append(stream, loadval(r1, 6));
        append(stream, add(r2, r2, r1));
        append(stream, loadval(r1, 'V'));
        append(stream, mul(r2, r2, r1));
        append(stream, segs(r3, r1,r2));
        append(stream, add(r2, r2, r1));
}

void test_unmap(Seq_T stream)
{
        append(stream, loadval(r1, 1));
        append(stream, map(r1, r1));
        append(stream, unmap(r1));
        append(stream, halt());
}

void test_map(Seq_T stream)
{
        append(stream, loadval(r1, 1));
        append(stream, map(r1, r1));
        append(stream, loadval(r2, 50));
        append(stream, segs(r1, r3,r2));
        append(stream, segl(r1, r1,r3));
        append(stream, output(r1));
        append(stream, loadval(r0, '\n'));
        append(stream, output(r0));
        append(stream, halt());
}

void test_loadp(Seq_T stream)
{
        append(stream, loadval(r0, 3));
        append(stream, loadP(r1, r0));
        append(stream, halt());
        append(stream, loadval(r2, 50));
        append(stream, map(r2, r2));
        append(stream, loadval(r3, 31));
        append(stream, loadval(r4, 2));
        append(stream, segl(r5, r5,r4));
        append(stream, segs(r2, r3,r5));
        append(stream, loadP(r2, r3));
}