package ast;

public abstract class EBin extends E {
   private E opnd1;
   private E opnd2;
   private String operacion;

   public EBin(E opnd1, E opnd2, String op) {
     this.opnd1 = opnd1;
     this.opnd2 = opnd2;
     this.operacion = op;
   }
   public E opnd1() {return opnd1;}
   public E opnd2() {return opnd2;}

   public KindE kind() {
   if(this.operacion == "*"){
   	return KindE.POR;
   	}
   else if(this.operacion =="+"){
   	return KindE.MAS;
   }
   else if(this.operacion =="-"){
   	return KindE.MENOS;
   }
   else if(this.operacion =="/"){
   	return KindE.DIV;
   }
   else if(this.operacion ==">"){
   	return KindE.MAYOR;
   }
   else if(this.operacion =="<"){
   	return KindE.MENOR;
   }
   else if(this.operacion =="=="){
   	return KindE.IGIGUAL;
   }
   else if(this.operacion =="&&"){
   	return KindE.AND;
   }
   else if(this.operacion =="||"){
   	return KindE.OR;
   }
   }

   public String toString() {
   if(this.operacion == "*"){
   	return "por("+opnd1().toString()+","+opnd2().toString()+")";
   }
   else if(this.operacion == "+"){
   	return "mas("+opnd1().toString()+","+opnd2().toString()+")";
   }
    else if(this.operacion == "-"){
   	return "menos("+opnd1().toString()+","+opnd2().toString()+")";
   }
    else if(this.operacion == "/"){
   	return "div("+opnd1().toString()+","+opnd2().toString()+")";
   }
    else if(this.operacion == ">"){
   	return "mayor("+opnd1().toString()+","+opnd2().toString()+")";
   }
    else if(this.operacion == "<"){
   	return "menor("+opnd1().toString()+","+opnd2().toString()+")";
   }
    else if(this.operacion == "=="){
   	return "igigual("+opnd1().toString()+","+opnd2().toString()+")";
   }
    else if(this.operacion == "&&"){
   	return "and("+opnd1().toString()+","+opnd2().toString()+")";
   }
    else if(this.operacion == "||"){
   	return "or("+opnd1().toString()+","+opnd2().toString()+")";
   }
   }
}
