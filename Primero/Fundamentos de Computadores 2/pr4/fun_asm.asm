/*---------------------------------------------------------------------
**
**  Fichero:
**    fun_asm.c  19/10/2022
**
**    (c) Daniel Báscones García
**    Fundamentos de Computadores II
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Fichero de código para la práctica 4
**
**-------------------------------------------------------------------*/


//rellenar con directivas .extern y .global
//con las funciones apropiadas
.global eucl_dist
.global guardar
.extern mul

//int eucl_dist(int * w, int size);
eucl_dist:
    //recibo dirección de W en a0, y tamaño N en a1
    addi sp, sp, -20
    sw ra, 16(sp)
    sw s0, 12(sp)
    sw s1, 8(sp)
    sw s2, 4(sp)
    sw s3, 0(sp)

    mv s0, a0 //s0 guarda W
    mv s1, a1 //s1 guarda N
    li s2, 0 //s2 guarda acc
    li s3, 0 //s3 guarda i

    //realizo los cálculos pertinentes
for:
	bge s3, s1, return_eucl
	lw a0, 0(s0) //1er arg: W[i]
	lw a1, 0(s0) //2º arg: W[i]
	call mul //llamada a mul
	mv t0, a0
	//sw a0, 0(t0) //guardo el resultado de mul
	add s2, s2, t0 //acc+=mul
	//sw s2, 0(s2)
	//Hay que guardar acc???
//continua:
	addi s3, s3,1 //i++
	addi s0, s0, 4 //W++
	j for
return_eucl:
    //devuelvo el resultado en a0
    mv a0, s2
    //lw a0,0(s2)?
    call i_sqrt
    //sw a0,

    lw ra, 16(sp)
    lw s0, 12(sp)
    lw s1, 8(sp)
    lw s2, 4(sp)
    lw s3, 0(sp)
    addi sp, sp, 20
    ret //ret i_sqrt?

//int guardar(char valor, char * ubicacion);
guardar:
    //recibo el valor en a0, y la dirección destino en a1
    addi sp, sp, -8
    sw s0, 4(sp)
    sw s1, 0(sp)

    mv s0, a0 //s0 guarda valor
    mv s1, a1 //s1 guarda ubicación
    mv a1, a0

    lw s0, 4(sp)
    lw s1, 0(sp)
    addi sp, sp, 8
    ret

    //asegurarse que sólo se guarda UN BYTE!!
