/*---------------------------------------------------------------------
**
**  Fichero:
**    pr1_a.asm  19/10/2022
**
**    (c) Daniel Báscones García
**    Fundamentos de Computadores II
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Fichero de código para la práctica 1a
**
**  Notas de diseño:
**
**	# define N 10
**	int res = 0;
**	for (int i = 0; i < N ; i ++) {
**		res += i ;
**	}
**
**-------------------------------------------------------------------*/

//defino la constante N
.equ N, 10
//reservo espacio para el resultado
.bss
	res: 	.space 4
	i:		.space 4
//programa
.text
.global main
//Completar el código
main:
	li s0, N //s0=N

	la t1, res //Carga en t1 la dirección de res
	li s1, 0 //s1 es res inicializado a 0
	lw s1, 0(t1) //Usamos la dirección para cargar el valor de res

	la t2, i //Carga en t2 la dirección de i
	li s2, 0 //s2 es i inicializado a 0
	sw s2, 0(t2) //Guarda s2 (inicializado a 0) en i

for:
	bge s2,s0,end //Salta si i>=N (condición inversa)
	add s1, s1, s2 //res += i
	sw s1, 0(t1) //res=s1
	addi s2, s2, 1 //i++
	sw s2, 0(t2)
	j for

end:
	j . //Salta sobre sí mismo
.end
