/*---------------------------------------------------------------------
**
**  Fichero:
**    pr2_b.asm  19/10/2022
**
**    (c) Daniel Báscones García
**    Fundamentos de Computadores II
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Fichero de código para la práctica 2b
**
**  Notas de diseño:
**
**	# define N 8
**	# define INT_MAX 65536
**	int V [ N ] = { -7 ,3 , -9 ,8 ,15 , -16 ,0 ,3};
**	int W [ N ];
**	int min , index ;
**	for ( j = 0; j < N ; j ++) {
**		min = INT_MAX ;
**		for ( i = 0; i < N ; i ++) {
**			if ( V [ i ] < min ) {
**				min = V [ i ];
**				index = i ;
**			}
**		}
**		W [ j ] = V [ index ];
**		V [ index ] = INT_MAX ;
**	}
**
**-------------------------------------------------------------------*/

.global main
.equ N , 8
.equ INT_MAX , 65536

.data
V: 	 .word -7 ,3 , -9 ,8 ,15 , -16 ,0 ,3
.bss
W: 	 .space N *4
min: .space 4
ind: .space 4


.text
main:
	la s0, min
	li t0, INT_MAX //t0=INT_MAX
	li s1, N //s1=N
	la s2, ind

	mv s3, zero //s3 es j
	mv s4, zero //s4 es i

for_j:
	bge s3,s1,fin_for_j
	mv s0, t0
for_i:
	bge s4,s1,fin_for_i
	la t1,V //t1 tiene la dirección base de V (t1=-7)
	slli t2,s4,2 //t2 contiene (t1 << 2), igual a (i*4)
	add t3, t1, t2 //t3 contiene "s1 + t1*4", justo la dirección buscada (t3=efectiva de v[i]
	lw s5, 0(t3) //cargo en s5 el valor de V[i]
if:
	bge s5,s0,fin_if // V[i]<min (operación inversa)
	mv s0,s5 //min=V[i]
	mv s2,s4 //ind=i
fin_if:
	addi s4,s4,1 //i++
	j for_i
fin_for_i:
	la t4,W
	slli t5,s3,2 //j*4
	add t6, t5, t4 //t6=efectiva de W[j]
	sw s0, 0(t6) //s0 es v[index]


	la t1,V
	slli t2,s2,2 //ind*4
	add t3, t1, t2 //t2=efectiva de V[index]
	sw t0, 0(t3) //guarda intMax en v[index]
	mv s4, zero //s4 es i

	addi s3,s3,1 //j++
	j for_j
fin_for_j:
	j .

.end



