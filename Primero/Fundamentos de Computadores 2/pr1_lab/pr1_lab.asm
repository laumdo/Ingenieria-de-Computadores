/*---------------------------------------------------------------------
**
**  Fichero:
**    pr1_lab.asm  19/10/2022
**
**    (c) Daniel Báscones García
**    Fundamentos de Computadores II
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Fichero de código para la práctica 1_lab
**
**  Notas de diseño:
**
**-------------------------------------------------------------------*/

//defino la constante N
.equ N, 7
.equ START, 3
//reservo espacio para el resultado
.bss
	num: 	.space 4
	i:		.space 4
//programa
.text
.global main
//Completar el código
main:
	li s0, N //s0=N
	li s3, START //s3=START

	la t1, num //Carga en t1 la dirección de num
	//la s1, s3 //s1 es num inicializado con el valor de START
	mv s1,s3 //Copia el valor de s3(START) en t1 (num)
	sw s1, 0(t1) //Usamos la dirección de num para cargar el valor de START

	la t2, i //Carga en t2 la dirección de i
	li s2, 0 //s2 es i inicializado a 0
	sw s2, 0(t2) //Guarda s2 (inicializado a 0) en i

for:
	bge s2,s0,end //Salta si i>=N (condición inversa)
	li s4, 1 //Cargo el valor 1
	and t3, s1, s4 //num & 1
	bnez t3, else //Si (num & 1) es distinto de 0, salto a else

if:
	li t4, 2 //Cargo 2 en t4
	div  s1,s1, t4
	sw s1, 0(t1)
	j fin_if

else:
	li t5, 3 //Cargo el valor de 3
	mul s1, s1, t5 //num*3
	addi s1, s1, 1 //num*3+1
	sw s1,0(t1)

fin_if:
	addi s2, s2, 1 //i++
	sw s2, 0(t2)
	j for

end:
	j . //Salta sobre sí mismo

.end
