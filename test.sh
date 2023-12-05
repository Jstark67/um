#!/bin/bash

make
use comp40
echo "test add normal"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_add.um > output.txt
um ./test/test_add.um > output1.txt
diff output.txt output1.txt


echo "test add edge"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_add_edge.um > output.txt
um ./test/test_add_edge.um > output1.txt
diff output.txt output1.txt

echo "test div"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_div.um > output.txt
um ./test/test_div.um > output1.txt
diff output.txt output1.txt

echo "test_halt_verbose"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_halt_verbose.um > output.txt
um ./test/test_halt_verbose.um > output1.txt
diff output.txt output1.txt

echo "test_halt"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_halt.um > output.txt
um ./test/test_halt.um > output1.txt
diff output.txt output1.txt


echo "test_mov"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_mov.um > output.txt
um ./test/test_mov.um > output1.txt
diff output.txt output1.txt


echo "test_mult"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_mult.um > output.txt
um ./test/test_mult.um > output1.txt
diff output.txt output1.txt


echo "test_nand"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_nand.um > output.txt
um ./test/test_nand.um > output1.txt
diff output.txt output1.txt


echo "test_out"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_out.um > output.txt
um ./test/test_out.um > output1.txt
diff output.txt output1.txt


echo "test_segl"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_segl.um > output.txt
um ./test/test_segl.um > output1.txt
diff output.txt output1.txt


echo "test_segs"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_segs.um > output.txt
um ./test/test_segs.um > output1.txt
diff output.txt output1.txt


echo "test_in"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_in.um < ./test/test_in.0 > output.txt
um ./test/test_in.um < ./test/test_in.0 > output1.txt
diff output.txt output1.txt


echo "test_map"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_map.um > output.txt
um ./test/test_map.um > output1.txt
diff output.txt output1.txt


echo "test_loadp"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_loadp.um > output.txt
um ./test/test_loadp.um > output1.txt
diff output.txt output1.txt


echo "test_unmap"
valgrind --log-fd=9 9>>valgrind_output.log ./um ./test/test_unmap.um > output.txt
um ./test/test_unmap.um > output1.txt
diff output.txt output1.txt

rm output.txt output1.txt

