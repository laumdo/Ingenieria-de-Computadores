package ast;

public abstract class IIF extends I {
   private I instru;
   private E value;

   public IIF(I inst, E val) {
     this.instru=inst;
     this.value=val;
   }

   public String tostring(){
    return "si(" + instru.toString() +"," +value.tostring()+")";
   }
}
