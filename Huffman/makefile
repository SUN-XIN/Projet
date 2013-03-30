.PHONY: all t1_comp t2_comp t3_comp t1_decomp t2_decomp t3_decomp t1B_comp t2B_comp t3B_comp
BIN=bin
SRC=src

all :	t1_comp t2_comp t3_comp t1_decomp t2_decomp t3_decomp t1B_comp t2B_comp t3B_comp

clean :
	rm -f ${BIN}/*
	rm -f ./CODE_SORTIE*

${BIN}/t1_comp : ${SRC}/compression.c
	gcc -o ${BIN}/t1_comp ${SRC}/compression.c -lm
t1_comp : ${BIN}/t1_comp
	${BIN}/t1_comp test1_comp



${BIN}/t2_comp : ${SRC}/compression.c
	gcc -o ${BIN}/t2_comp ${SRC}/compression.c -lm
t2_comp : ${BIN}/t2_comp
	${BIN}/t2_comp test2_comp



${BIN}/t3_comp : ${SRC}/compression.c
	gcc -o ${BIN}/t3_comp ${SRC}/compression.c -lm
t3_comp : ${BIN}/t3_comp
	${BIN}/t3_comp test3_comp






${BIN}/t1_decomp : ${SRC}/decompression.c
	gcc -o ${BIN}/t1_decomp ${SRC}/decompression.c -lm
t1_decomp : ${BIN}/t1_decomp
	${BIN}/t1_decomp test1_decomp
	
	

${BIN}/t2_decomp : ${SRC}/decompression.c
	gcc -o ${BIN}/t2_decomp ${SRC}/decompression.c -lm
t2_decomp : ${BIN}/t2_decomp
	${BIN}/t2_decomp test2_decomp
	
	
	
${BIN}/t3_decomp : ${SRC}/decompression.c
	gcc -o ${BIN}/t3_decomp ${SRC}/decompression.c -lm
t3_decomp : ${BIN}/t3_decomp
	${BIN}/t3_decomp test3_decomp






${BIN}/t1B_comp : ${SRC}/compressionBIS.c
	gcc -o ${BIN}/t1B_comp ${SRC}/compressionBIS.c -lm
t1B_comp : ${BIN}/t1B_comp
	${BIN}/t1B_comp test1_comp



${BIN}/t2B_comp : ${SRC}/compressionBIS.c
	gcc -o ${BIN}/t2B_comp ${SRC}/compressionBIS.c -lm
t2B_comp : ${BIN}/t2B_comp
	${BIN}/t2B_comp test2_comp



${BIN}/t3B_comp : ${SRC}/compressionBIS.c
	gcc -o ${BIN}/t3B_comp ${SRC}/compressionBIS.c -lm
t3B_comp : ${BIN}/t3B_comp
	${BIN}/t3B_comp test3_comp
