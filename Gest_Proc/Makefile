.PHONY: rps ecouteur demandeur testAll test1 test2 ecouteur1 demandeur1 ecouteur2 demandeur2
CC=gcc
BIN=bin
INC=include
OBJ=obj
SRC=src
T=test

clean :
	rm -f ${OBJ}/* ${BIN}/*

testAll: test1 test2

test1 : ${T}/test1.c
	${CC} -o ${T}/test1 ${T}/test1.c 
	
test2 : ${T}/test2.c
	${CC} -o ${T}/test2 ${T}/test2.c 
	
	
${BIN}/rps : ${SRC}/rps.c
	${CC} -o ${BIN}/rps ${SRC}/rps.c -I${INC} -lrt
rps : ${BIN}/rps
	${BIN}/rps
	
	
${BIN}/ecouteur : ${SRC}/ecouteur.c
	${CC} -o ${BIN}/ecouteur ${SRC}/ecouteur.c -I${INC} -lrt
ecouteur : ${BIN}/ecouteur
	${BIN}/ecouteur

${BIN}/demandeur : ${SRC}/demandeur.c
	${CC} -o ${BIN}/demandeur ${SRC}/demandeur.c -I${INC} -lrt
demandeur : ${BIN}/demandeur
	${BIN}/demandeur


${BIN}/ecouteur1 : M1/ecouteur.c
	${CC} -o ${BIN}/ecouteur1 M1/ecouteur.c -I${INC} -lrt
ecouteur1 : ${BIN}/ecouteur1
	${BIN}/ecouteur1

${BIN}/demandeur1 : M1/demandeur1.c
	${CC} -o ${BIN}/demandeur1 M1/demandeur1.c -I${INC} -lrt
demandeur1 : ${BIN}/demandeur1
	${BIN}/demandeur1
	
	
${BIN}/ecouteur2 : M2/ecouteur.c
	${CC} -o ${BIN}/ecouteur2 M2/ecouteur.c -I${INC} -lrt
ecouteur2 : ${BIN}/ecouteur2
	${BIN}/ecouteur2

${BIN}/demandeur2 : M2/demandeur2.c
	${CC} -o ${BIN}/demandeur2 M1/demandeur1.c -I${INC} -lrt
demandeur2 : ${BIN}/demandeur2
	${BIN}/demandeur2
