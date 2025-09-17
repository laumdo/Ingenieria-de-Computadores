/*---------------------------------------------------------------------
**
**  Fichero:
**    pr2_a.asm  19/10/2022
**
**    (c) Daniel Báscones García
**    Fundamentos de Computadores II
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Fichero de código para la práctica 2a
**
**  Notas de diseño:
**
** 	# define N 8
** 	# define INT_MAX 65536
**	int V [ N ] = { -7 ,3 , -9 ,8 ,15 , -16 ,0 ,3};
**	int min = INT_MAX ;
**	for ( i = 0; i < N ; i ++) {
**		if ( V [ i ] < min )
**			min = V [ i ];
**	}
**
**-------------------------------------------------------------------*/

.global main
.equ N, 8
.equ INT_MAX, 65536
.data
V:   .word -7,3,-9,8,15,-16,0,3

.bss
//i: .space 4 No hace falta pq no hay que guardarla
min: .space 4

.text
main:
    la s0, min //en s0 tengo min
    li t0, INT_MAX //t0 contiene INT_MAX (65536)
    sw t0, 0(s0) //min=INT_MAX

    //la t1,i
    //li s1,0
    //sw s1,0(t1)

    li s1, N //s1=N
    mv s2,zero //s2 es i (i=0)

for:
	bge s2,s1,end //i<N (operación inversa)
	la t1,V //t1 tiene la dirección base de V (t1=-7, al principio)
	//s1 contiene la dirección base de V y t1 contiene el índice i
	slli t2,s2,2 //t2 contiene (t1 << 2), igual a (i*4)
	add t3, t1, t2 //t3 contiene "s1 + s2*4", justo la dirección buscada (t3=efectiva de v[i]
	lw s3, 0(t3) //cargo en s3 el valor de V[i]
if:
	bge s3,s0,fin_if // V[i]<min (operación inversa)
	sw s3,0(t3) //guardo en s3 el valor de V[i]
	mv s0, s3 //sobreescribo min con V[i] (min=V[i])

fin_if:
	addi s2, s2, 1 //i++
	//sw s1,0(t1)
	j for

end:
	j .
.end

