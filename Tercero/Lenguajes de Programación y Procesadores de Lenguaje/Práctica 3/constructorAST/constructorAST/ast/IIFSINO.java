package ast;

public abstract class IIFSINO extends I {
   private I instru;
   private E value;

   public IIFSINO(I inst, E val) {
     this.instru=inst;
     this.value=val;
   }

   public String tostring(){
    return "sino(" + instru.toString() +"," +value.tostring()+")";
   }
}
