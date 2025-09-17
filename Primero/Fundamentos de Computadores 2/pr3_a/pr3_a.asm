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
**	int mul(int a, int b) {
**	    int res = 0;
**	    while (b > 0) {
**	        res += a;

**	        b--;
**	    }
**	    return res;
**	}
**
**	int dotprod(int V[], int W[], int n) {
**	    int acc = 0;
**	    for (int i = 0; i < n; i++) {
**	        acc += mul(V[i], W[i]);
**	    }
**	    return acc;
**	}
**
**	#define N 4
**	int A[] = {3, 5, 1, 9}
**	int B[] = {1, 6, 2, 3}
**
**	int res;
**
**	void main() {
**	    int normA = dotprod(A, A, N);
**	    int normB = dotprod(B, B, N);
**	    if (normA > normB)
**	        res = 0xa;
**	    else
**	        res = 0xb;
**	}
**

**-------------------------------------------------------------------*/
.extern _stack
.global main
.equ N, 4

.data
	A: .word 3, 5, 1, 9
	B: .word 1, 6, 2, 3

.bss
	res: .space 4

.text
main:
	la sp, _stack //inicializa sp
	la a0, A //1er argumento: dir de A
	la a1, A //2º argumento: dir de A
	la a2, N //3er argumento: N
	call dotprod //llama a dotprod
	mv s0, a0 //s0 guarda normA

	la a0, B //1er argumento: dir de B
	la a1, B //2º argumento: dir de B
	la a2, N //3er argumento: N
	call dotprod //llama a dotprod
	mv s1, a0 //s1 guarda normB

	la t0, res

if:
	ble s0, s1, else //normA>normB
	li s2, 10 //cargo 10 (0xa)
	sw s2, 0(t0) //guardo res
	j fin
else:
	li s0, 11 //cargo 11 (0xb)
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
	mv s0, a0
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
	li a0, 0 //a0 guarda res=0
while_mul:
	ble a1, zero, return_mul //comparo b>0
	add a0, a0, a0 //res= res + a
	addi a1, a1, -1 //b--
	//guardar b???
	j while_mul
return_mul:
	ret //devuelvo valor en a0
