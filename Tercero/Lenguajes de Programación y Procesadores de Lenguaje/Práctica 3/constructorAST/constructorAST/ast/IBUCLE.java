package ast;

public abstract class IBUCLE extends I {
   private I instru;
   private E value;

   public IBUCLE(I inst, E val) {
     this.instru=inst;
     this.value=val;
   }

   public String tostring(){
    return "bucle(" + instru.toString() +"," +value.tostring()+")";
   }
}
