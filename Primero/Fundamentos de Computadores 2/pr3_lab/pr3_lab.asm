/*---------------------------------------------------------------------
**
**  Fichero:
**    pr3_lab.asm  19/10/2022
**
**    (c) Daniel Báscones García
**    Fundamentos de Computadores II
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Fichero de código para la práctica 3_lab
**
**-------------------------------------------------------------------*/
.extern _stack
.global main
.equ N, 3

.data
	 A: .word 7, 5, 3
	 B: .word 3, 8, 4
	 C: .word 2, 4, 2
.bss
 	res: .space 4
 	V1: .space 12
 	V2: .space 12
.text
main:
	 la sp, _stack //inicializa sp

	 la a0, A
	 la a1, C
	 la a2, V1
	 la a3, N
	 call resta_array
	 mv s0, a0 //s0 guarda A-C en V1

	 la a0, B
	 la a1, C
	 la a2, V2
	 la a3, N
	 call resta_array
	 mv s1, a0 //s1 guarda B-C en V2

	 la a0, A //1er argumento: dir de A
	 la a1, A //2º argumento: dir de A
	 la a2, N //3er argumento: N
	 call dotprod //llama a dotprod
	 mv s2, a0 //s2 guarda normA

	 la a0, B //1er argumento: dir de B
	 la a1, B //2º argumento: dir de B
	 la a2, N //3er argumento: N
	 call dotprod //llama a dotprod
	 mv s3, a0 //s3 guarda normB
	 la t0, res
if:
	 ble s0, s1, else //normA>normB
	 li s2, 10 //cargo 10 (0xa)
	 sw s2, 0(t0) //guardo res
	 j fin
else:
	 li s2, 11 //cargo 11 (0xb)
	 sw s2, 0(t0) //guardo res
	 j fin
fin:
	j fin
dotprod:
	 addi sp, sp, -24
	 //Prólogo:
	 sw ra, 20(sp)
	 sw s0, 16(sp)
	 sw s1, 12(sp)
	 sw s2, 8(sp)
	 sw s3, 4(sp)
	 sw s4, 0(sp)
	 li s0, 0 //s0 guarda acc
	 li s1, 0 //s1 guarda i
	 mv s2, a0 //s2 guarda V
	 mv s3, a1 //s3 guarda W
	 mv s4, a2 //s4 guarda n
for:
	 bge s1, s4, return_dotprod //i<n
	 lw a0, 0(s2) //1er argumento: V[i]
	 lw a1, 0(s3) //2º argumento: W[i]
	 call mul //llamada a mul
 	 add s0,s0, a0 //res+mul()

sigue_bucle:
	 addi s1, s1, 1 //i++
	 addi s2, s2, 4 //X++
	 addi s3, s3, 4 //Y++
	 j for
return_dotprod:
	 mv a0, s0 //coloco el valor de retorno
	 //Epílogo:
	 lw ra, 20(sp)
	 lw s0, 16(sp)
	 lw s1, 12(sp)
	 lw s2, 8(sp)
	 lw s3, 4(sp)
	 lw s4, 0(sp)
	 addi sp, sp, 24
	 ret
mul:
	 li s0,0 //s0 guarda res

while_mul:
	 ble a1, zero, return_mul //comparo b>0
	 add s0,s0,a0 //res+a
	 addi a1, a1, -1 //b--
	 j while_mul
return_mul:
 	mv a0,s0
 	ret //devuelvo valor en a0

resta_array:
	li t0, 0 //t0 guarda i
for_resta:
	bge t0, a3, fin_for_resta

	lw t1, 0(a0) // load V1[i] to t1
    lw t2, 0(a1) //load V2[i] to t2
    sub t3, t1, t2 //V1[i]-V2[i]=VRES[i]
    sw t3, 0(a2) //guarda VRES[i]

    addi a0, a0, 4  // V1++
    addi a1, a1, 4  // V2++
    addi a2, a2, 4 //VRES++

	addi t0, t0, 1 //i++
	j for_resta
fin_for_resta:
	ret
