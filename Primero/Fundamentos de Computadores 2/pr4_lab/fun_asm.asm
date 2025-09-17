/*---------------------------------------------------------------------
**
**  Fichero:
**    fun_asm.asm  19/10/2022
**
**    (c) Daniel Báscones García
**    Fundamentos de Computadores II
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Fichero de código para la práctica 4_lab
**
**-------------------------------------------------------------------*/
.global guardar_vector
.global eucl_dist
.global guardar
.extern mul
.extern guardar_word

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

	addi s3, s3,1 //i++
	addi s0, s0, 4 //W++
	j for
return_eucl:
    //devuelvo el resultado en a0
    mv a0, s2
    call i_sqrt

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
guardar_vector:
	addi sp, sp, -24
    sw ra, 20(sp)
    sw s0, 16(sp)
    sw s1, 12(sp)
    sw s2, 8(sp)
    sw s3, 4(sp)
    sw s4, 0(sp)

	mv s0, a0 //s0 guarda vector[]
    mv s1, a1 //s1 guarda destino[]
    mv s2, a2 //s2 guarda N (size)
    //li s3, 0 //s3 guarda valor
    li s4, 0 //s4 guarda i
for_guardar:
	bge s4,s2,fin_for_guardar

	mv s3, a0 //s3 guarda el valor de vector[i]
	mv s5, s1 //s5 guarda el valor de destino[i]

	lw a0, 0(s3) //1er arg: valor
	lw a1, 0(s5) //2º arg: direccion
	call guardar_word
	//mv t1, a0
	sw a0, 0(s5)

	addi s4, s4,1 //i++
	addi s0, s0, 4 //vector[i]++
	addi s1, s1, 4 //destino[i]++

	j for_guardar

fin_for_guardar:
    lw ra, 20(sp)
    lw s0, 16(sp)
    lw s1, 12(sp)
    lw s2, 8(sp)
    lw s3, 4(sp)
    lw s4, 0(sp)
    addi sp, sp, 24
    ret
