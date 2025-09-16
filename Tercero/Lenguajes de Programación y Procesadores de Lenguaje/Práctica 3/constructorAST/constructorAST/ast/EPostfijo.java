package ast;

public abstract class EPostfijo extends E {
   private E opnd1;
   private String operacion;

   public EPostfijo(E opnd1, String op) {
     this.opnd1 = opnd1;
     this.operacion = op;
   }
   public E opnd1() {return opnd1;}

   public KindE kind() {
   if(this.operacion == "++"){
   	return KindE.INCREMENTO;
   	}
   else if(this.operacion =="--"){
   	return KindE.DECREMENTO;
   }

   }

   public String toString() {
   if(this.operacion == "++"){
   	return "incremento("+opnd1().toString()+")";
   }
   else if(this.operacion == "--"){
   	return "decremento("+opnd1().toString()+")";
   }

   }
}
