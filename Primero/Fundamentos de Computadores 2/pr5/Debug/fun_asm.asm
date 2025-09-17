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
**    Fichero de código para la práctica 5_lab
**	Notas de diseño:

int suma(int V[], int size) {
		int sum = 0;
		for (int i = 0; i < size; i++) {
		sum += V[i]; //Suma los elementos del vector
		}
		return sum;
	}

	int compara(int V[], int W[], int size) {
		int sol = 0; //0 indica que la suma de elementos del primer vector es mayor
		int prim = suma(V, size);
		int seg = suma(W, size);
		if (seg > prim) {
		sol = 1; //La suma de los elementos del segundo vector es mayor
		}
		return sol;
	}

	#define N 3
	int A[N] = { 3,5,9 };
	int B[N] = { -2,8,3 };

	int min; //Almacenará el mínimo valor del array


	void main() {
		int mayor = compara(A, B, N);
		if (mayor == 0) { //El vector A suma más
		min = minimo(A, N);
		}
		else { //El vector B suma más
		min = minimo(B, N);
		}
	}


**-------------------------------------------------------------------*/
.extern minimo
.global main
.equ N, 3

.data
	A: .word 5,3,9
	B: .word -2,8,3
.bss
	min: .space 4
.text

main:
	la a0, A //1er argumento: dir de A
	la a1, B //2º argumento: dir de B
	la a2, N //3er argumento: N
	call compara //llama a compara
	mv s0, a0 //s0 guarda mayor
if:
	bnez s0, else //salta si es distinto de 0 (operación contraria)
	la a0, A //1er argumento: dir de A
	la a1, N //2º argumento: N
	call minimo //llama a minimo (está en C)
	sw s1, 0(a0)
	//mv s1, a0 //s1 guarda min
	j fin
else:
	la a0, B //1er argumento: dir de B
	la a1, N //2º argumento: N
	call minimo //llama a minimo (está en C)
	sw s1, 0(a0)
	//mv s1, a0 //s1 guarda min
	j fin
fin:
	j fin

compara:
	addi sp, sp, -28
	//Prólogo:

	sw ra, 24(sp)
	sw s0, 20(sp)
	sw s1, 16(sp)
	sw s2, 12(sp)
	sw s3, 8(sp)
	sw s4, 4(sp)
	sw s5, 0(sp)

	mv s0, a0 //s1 guarda V
	mv s1, a1 //s2 guarda W
	mv s2, a2 //s3 guarda N
	li s3, 0 //s0 guarda sol


	//lw a0, 0(s0) //1er argumento: V[i]
	mv a0, s0
	//lw a1, 0(s2) //2º argumento: N
	mv a1, s2
	call suma
	mv s4, a0 //s4 guarda prim

	//lw a0, 0(s1) //1er argumento: W[i]
	//lw a1, 0(s2) //2º argumento: N
	mv a0, s1
	mv a1, s2
	call suma
	mv s5, a0 //s5 guarda seg
if_compara:
	ble s5, s4, fin_compara //seg>prim (condición inversa)
	lui s0, 1 //carga inmediato (1)
fin_compara:
	mv a0, s0 //coloco el valor de retorno (sol)
	//lw a0,0(s0)
	//Epílogo:
	lw ra, 24(sp)
	lw s0, 20(sp)
	lw s1, 16(sp)
	lw s2, 12(sp)
	lw s3, 8(sp)
	lw s4, 4(sp)
	lw s5, 0(sp)
	addi sp, sp, 28
	ret


suma:
	li t5, 0 //t5 guarda i
	li t6, 0 //t6 guarda sum=0
for_suma:
	bge t5, a1, return_suma //i<size
	//mv t1, a0 //guarda la dirección base de V
	slli t2, t5,2 //t2 contiene (t1<<2) igual a (i*4)
	add t3, a0,t2

	lw t4, 0(t3) //cargo en s2 el valor de V[i]

	add t6, t6, t4 //sum+=V[i]
	addi t5, t5, 1 //i++
	//addi t1, t1, 4 //V++
	j for_suma
return_suma:
	mv a0, t6 //devuelvo valor en a0
	ret
