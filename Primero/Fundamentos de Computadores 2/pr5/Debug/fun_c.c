/*---------------------------------------------------------------------
**
**  Fichero:
**    fun_c.c  19/10/2022
**
**    (c) Daniel Báscones García
**    Fundamentos de Computadores II
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Fichero de código para la práctica 5_lab
**
**-------------------------------------------------------------------*/
int minimo(int V[], int size) {
	int min = V[0];
	for (int i = 0; i < size; i++) {
		if (V[i] < min) {
			min = V[i];
		}
	}
	return min;
}
